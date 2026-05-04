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
			 0x89c81a52, /// 0x0
			 0xab88d27e, /// 0x4
			 0x229f862c, /// 0x8
			 0x405fcdae, /// 0xc
			 0x80f9e908, /// 0x10
			 0xb20a40e6, /// 0x14
			 0x26d09d84, /// 0x18
			 0x106587b4, /// 0x1c
			 0xeff21bbd, /// 0x20
			 0x3beed7d9, /// 0x24
			 0x1359f000, /// 0x28
			 0xf8581df0, /// 0x2c
			 0x1eff20e2, /// 0x30
			 0x575d8c4d, /// 0x34
			 0xfde87a3a, /// 0x38
			 0xd43c9786, /// 0x3c
			 0x19724775, /// 0x40
			 0x510404bb, /// 0x44
			 0xb4921eb0, /// 0x48
			 0x3f84795e, /// 0x4c
			 0xdb8278ff, /// 0x50
			 0x4563b7ad, /// 0x54
			 0x78eab9c8, /// 0x58
			 0xb6b33c91, /// 0x5c
			 0x5f8157d4, /// 0x60
			 0x11c3a424, /// 0x64
			 0x3b6654e8, /// 0x68
			 0x9b326770, /// 0x6c
			 0xa1266ec0, /// 0x70
			 0x9d1db85a, /// 0x74
			 0x0b06909e, /// 0x78
			 0xef6e0db1, /// 0x7c
			 0x3f023f43, /// 0x80
			 0xafde204e, /// 0x84
			 0x68f01e2f, /// 0x88
			 0xa95e8969, /// 0x8c
			 0x1b64d3fe, /// 0x90
			 0xbf50519a, /// 0x94
			 0x6ec85b9d, /// 0x98
			 0x11a8fffd, /// 0x9c
			 0x9541d517, /// 0xa0
			 0xededc2b6, /// 0xa4
			 0x05fdbec7, /// 0xa8
			 0xd91da133, /// 0xac
			 0x1e4aa334, /// 0xb0
			 0xba3fa2db, /// 0xb4
			 0x924f8f74, /// 0xb8
			 0x4ca5f3cf, /// 0xbc
			 0x890e28cc, /// 0xc0
			 0x578fa999, /// 0xc4
			 0xf72212e5, /// 0xc8
			 0xaf4381c8, /// 0xcc
			 0x0fe67518, /// 0xd0
			 0xeb95e1fe, /// 0xd4
			 0xdd57297a, /// 0xd8
			 0x59c616f3, /// 0xdc
			 0xe39fa855, /// 0xe0
			 0x55193e96, /// 0xe4
			 0x65100709, /// 0xe8
			 0xca6d7122, /// 0xec
			 0xbd07c278, /// 0xf0
			 0x3fba5764, /// 0xf4
			 0x7d282a75, /// 0xf8
			 0x43b3b859, /// 0xfc
			 0xb7cef319, /// 0x100
			 0xf8c7dda4, /// 0x104
			 0xf6a5ca87, /// 0x108
			 0xa689d287, /// 0x10c
			 0x96dcc58a, /// 0x110
			 0x8e3945d4, /// 0x114
			 0x7a372370, /// 0x118
			 0x244f277a, /// 0x11c
			 0x72ef4d6b, /// 0x120
			 0xf992df0d, /// 0x124
			 0x58114491, /// 0x128
			 0x0eb6302a, /// 0x12c
			 0x84c0d0e8, /// 0x130
			 0x16b89798, /// 0x134
			 0x9919fea2, /// 0x138
			 0x1d97e908, /// 0x13c
			 0xf029405f, /// 0x140
			 0xcad10d8a, /// 0x144
			 0x1db0fd91, /// 0x148
			 0x2164d6bc, /// 0x14c
			 0x4b7bc32b, /// 0x150
			 0x1b852257, /// 0x154
			 0x6236ca80, /// 0x158
			 0x1ec4b32f, /// 0x15c
			 0x64f1e43e, /// 0x160
			 0x646e712a, /// 0x164
			 0xd5144830, /// 0x168
			 0xf170a1d0, /// 0x16c
			 0x13a1d1ad, /// 0x170
			 0xc0d93000, /// 0x174
			 0x53d133b7, /// 0x178
			 0x4191f337, /// 0x17c
			 0xd9fae225, /// 0x180
			 0xb0cd7bd7, /// 0x184
			 0x62d280aa, /// 0x188
			 0x235638da, /// 0x18c
			 0x97ce9293, /// 0x190
			 0xca2de7b0, /// 0x194
			 0x1e09120d, /// 0x198
			 0x85f14db1, /// 0x19c
			 0x02ccd1c3, /// 0x1a0
			 0x11798934, /// 0x1a4
			 0x80d8d2cc, /// 0x1a8
			 0x6a467354, /// 0x1ac
			 0x1258db45, /// 0x1b0
			 0x2628455e, /// 0x1b4
			 0x7a294b8f, /// 0x1b8
			 0x0be175c0, /// 0x1bc
			 0x6fce2497, /// 0x1c0
			 0x11f84a45, /// 0x1c4
			 0x54b5d8be, /// 0x1c8
			 0x839c7a26, /// 0x1cc
			 0xbf4b83d5, /// 0x1d0
			 0x2cbedb2b, /// 0x1d4
			 0x72eddbc7, /// 0x1d8
			 0x389621b0, /// 0x1dc
			 0x42c3f507, /// 0x1e0
			 0x6bc4a416, /// 0x1e4
			 0x0864b673, /// 0x1e8
			 0x39c609bc, /// 0x1ec
			 0xa6bf4166, /// 0x1f0
			 0x00abb6cf, /// 0x1f4
			 0x5b011b40, /// 0x1f8
			 0xad0d054f, /// 0x1fc
			 0x66886d4c, /// 0x200
			 0x9dd3b7a5, /// 0x204
			 0x4bcf8400, /// 0x208
			 0x328c700b, /// 0x20c
			 0x6b87ab0a, /// 0x210
			 0xfbcae3ea, /// 0x214
			 0xaf7a75e6, /// 0x218
			 0x4dc98a3b, /// 0x21c
			 0xd91600a5, /// 0x220
			 0xc962efb3, /// 0x224
			 0x3cb05b31, /// 0x228
			 0x8eb78a98, /// 0x22c
			 0x54bcc5a8, /// 0x230
			 0x7c53debc, /// 0x234
			 0x5e9597f5, /// 0x238
			 0xb63f4401, /// 0x23c
			 0xd9c674b4, /// 0x240
			 0x0e6b0fd1, /// 0x244
			 0x1c640bd9, /// 0x248
			 0xd1c21a69, /// 0x24c
			 0xb17a96af, /// 0x250
			 0xf7473819, /// 0x254
			 0xded2b945, /// 0x258
			 0x7836417a, /// 0x25c
			 0x896dce31, /// 0x260
			 0x888c2464, /// 0x264
			 0x0d208d99, /// 0x268
			 0xaeb9fec1, /// 0x26c
			 0x6eb2b903, /// 0x270
			 0xe1e4811e, /// 0x274
			 0x3ea2ab2c, /// 0x278
			 0x8140e364, /// 0x27c
			 0xdef3caa2, /// 0x280
			 0x64b66e12, /// 0x284
			 0x5cba9724, /// 0x288
			 0x1c74a38b, /// 0x28c
			 0x3ff14b8a, /// 0x290
			 0x7b8be840, /// 0x294
			 0x84886d95, /// 0x298
			 0xa66b9479, /// 0x29c
			 0xcb2ae103, /// 0x2a0
			 0x9df818b5, /// 0x2a4
			 0xea75d79f, /// 0x2a8
			 0x804aebc5, /// 0x2ac
			 0x8e8ed767, /// 0x2b0
			 0x566da15e, /// 0x2b4
			 0x5c156de9, /// 0x2b8
			 0xe3faa18d, /// 0x2bc
			 0xb6c5ad6e, /// 0x2c0
			 0x796c6f06, /// 0x2c4
			 0xfe65b9cf, /// 0x2c8
			 0x86201cab, /// 0x2cc
			 0x44112ccb, /// 0x2d0
			 0x03998ee0, /// 0x2d4
			 0xbe78a07f, /// 0x2d8
			 0x4219da51, /// 0x2dc
			 0x002d53c3, /// 0x2e0
			 0x744da9b9, /// 0x2e4
			 0x68f5db35, /// 0x2e8
			 0xead17ef8, /// 0x2ec
			 0x0c3ae161, /// 0x2f0
			 0xf0722fd9, /// 0x2f4
			 0x1b792b0a, /// 0x2f8
			 0x1763369d, /// 0x2fc
			 0x3f7b7496, /// 0x300
			 0x81c924f6, /// 0x304
			 0x7c90cf36, /// 0x308
			 0x43e01f2e, /// 0x30c
			 0x12961240, /// 0x310
			 0x44066f0a, /// 0x314
			 0x5827ac36, /// 0x318
			 0xd9829d68, /// 0x31c
			 0xe7fbd3ce, /// 0x320
			 0x2ab1d55f, /// 0x324
			 0x400ba048, /// 0x328
			 0xbb30f2a3, /// 0x32c
			 0xa09ada0e, /// 0x330
			 0xb53ecd49, /// 0x334
			 0xe200ede7, /// 0x338
			 0x3e4d65c1, /// 0x33c
			 0x4fb27f1c, /// 0x340
			 0x79becb35, /// 0x344
			 0x1264e72a, /// 0x348
			 0xa282d75d, /// 0x34c
			 0x6293748a, /// 0x350
			 0x14bddb4a, /// 0x354
			 0xe0b011cc, /// 0x358
			 0x9cd17d1d, /// 0x35c
			 0x7d19a270, /// 0x360
			 0x61cfa9a9, /// 0x364
			 0x7d2b91b2, /// 0x368
			 0x5bcac1d8, /// 0x36c
			 0xf808111f, /// 0x370
			 0xed461fc7, /// 0x374
			 0x6ac1c1ba, /// 0x378
			 0x3f91e968, /// 0x37c
			 0x4b6a4622, /// 0x380
			 0x1e4adacf, /// 0x384
			 0xfc0ccb28, /// 0x388
			 0x9f7dea80, /// 0x38c
			 0x5e3cc251, /// 0x390
			 0xcb79b053, /// 0x394
			 0x10ff2de8, /// 0x398
			 0x14a3807d, /// 0x39c
			 0xb1d7e3c0, /// 0x3a0
			 0xa284bfdc, /// 0x3a4
			 0xfe21b59e, /// 0x3a8
			 0xc45dd0d0, /// 0x3ac
			 0x735f238a, /// 0x3b0
			 0x9540cc29, /// 0x3b4
			 0x212c363a, /// 0x3b8
			 0xbab64f54, /// 0x3bc
			 0x0b96dc6a, /// 0x3c0
			 0x47cf7189, /// 0x3c4
			 0x61c26c33, /// 0x3c8
			 0xb9891e48, /// 0x3cc
			 0x72235d57, /// 0x3d0
			 0x7cdd11fe, /// 0x3d4
			 0xf1511c35, /// 0x3d8
			 0xd6264100, /// 0x3dc
			 0xcd532cea, /// 0x3e0
			 0x571322c0, /// 0x3e4
			 0xcc93d2e8, /// 0x3e8
			 0x795bf5a9, /// 0x3ec
			 0xbee5e307, /// 0x3f0
			 0x0e0be6e2, /// 0x3f4
			 0xfdb05fec, /// 0x3f8
			 0x55d3d5e8, /// 0x3fc
			 0x0c65df83, /// 0x400
			 0x1e38dbc4, /// 0x404
			 0xd5734bb7, /// 0x408
			 0x0f67fea6, /// 0x40c
			 0x50ccc250, /// 0x410
			 0xf07a6f4d, /// 0x414
			 0x05877022, /// 0x418
			 0x4f4532bd, /// 0x41c
			 0x41eb87d4, /// 0x420
			 0xe2c28487, /// 0x424
			 0x0b5f9933, /// 0x428
			 0xe3493b6f, /// 0x42c
			 0x3ea34dcd, /// 0x430
			 0xf44400b1, /// 0x434
			 0x25c6a51e, /// 0x438
			 0xa3a65107, /// 0x43c
			 0xdc66ec04, /// 0x440
			 0xab4e1a68, /// 0x444
			 0x3bb01881, /// 0x448
			 0x607d2e46, /// 0x44c
			 0x45a2afa8, /// 0x450
			 0x9aae5290, /// 0x454
			 0xc161a64f, /// 0x458
			 0x14df8872, /// 0x45c
			 0x5610d8a5, /// 0x460
			 0x82465769, /// 0x464
			 0x50c2fba0, /// 0x468
			 0xdfa9bb40, /// 0x46c
			 0xcfb691b0, /// 0x470
			 0xc2f113f5, /// 0x474
			 0x89d0f095, /// 0x478
			 0x8fc0c92b, /// 0x47c
			 0x492dabae, /// 0x480
			 0x32cd7c1a, /// 0x484
			 0x45391aa9, /// 0x488
			 0x04a7f561, /// 0x48c
			 0x84eb646d, /// 0x490
			 0xb1b524eb, /// 0x494
			 0x9002b171, /// 0x498
			 0xb0471fc6, /// 0x49c
			 0x65760bc6, /// 0x4a0
			 0x8566bb47, /// 0x4a4
			 0x54e74bc8, /// 0x4a8
			 0x256c9e2f, /// 0x4ac
			 0x734a2eaf, /// 0x4b0
			 0x74fd62f1, /// 0x4b4
			 0x3e315bc7, /// 0x4b8
			 0x26ef7e71, /// 0x4bc
			 0xd6076108, /// 0x4c0
			 0x7a17206d, /// 0x4c4
			 0x752ab036, /// 0x4c8
			 0x730496f0, /// 0x4cc
			 0x84a9abb8, /// 0x4d0
			 0x408c7642, /// 0x4d4
			 0x1d6dd9af, /// 0x4d8
			 0x26d34f23, /// 0x4dc
			 0x340ad989, /// 0x4e0
			 0xd9ddcfe3, /// 0x4e4
			 0xd927c0d6, /// 0x4e8
			 0xc101126d, /// 0x4ec
			 0x9f4267c4, /// 0x4f0
			 0x0108ddad, /// 0x4f4
			 0x4267c8a7, /// 0x4f8
			 0xa1b1ef23, /// 0x4fc
			 0xce358478, /// 0x500
			 0xfc391794, /// 0x504
			 0x3b02e4fa, /// 0x508
			 0x8fcb4cae, /// 0x50c
			 0xf44d0f20, /// 0x510
			 0xe37415d8, /// 0x514
			 0xd063e636, /// 0x518
			 0x74d52995, /// 0x51c
			 0x81e7a605, /// 0x520
			 0x36b85ed7, /// 0x524
			 0xd0cec8e8, /// 0x528
			 0xa8de8a96, /// 0x52c
			 0x7b72311f, /// 0x530
			 0x1b463034, /// 0x534
			 0x22932a4a, /// 0x538
			 0xe30fe47a, /// 0x53c
			 0xd5172695, /// 0x540
			 0x491906f7, /// 0x544
			 0xe97c5df8, /// 0x548
			 0x52151c5a, /// 0x54c
			 0x0211ed77, /// 0x550
			 0x2bfd8c8d, /// 0x554
			 0x05b4530b, /// 0x558
			 0x5c9abd61, /// 0x55c
			 0x638fd8d3, /// 0x560
			 0x23e79fa4, /// 0x564
			 0x361bafbd, /// 0x568
			 0xf7298215, /// 0x56c
			 0xa1d0b855, /// 0x570
			 0x10467fab, /// 0x574
			 0x0ce752cd, /// 0x578
			 0x0fa4b989, /// 0x57c
			 0x3fda945e, /// 0x580
			 0x822b5ebc, /// 0x584
			 0x3073f696, /// 0x588
			 0x10fcd8cf, /// 0x58c
			 0x85c4c139, /// 0x590
			 0xed3eb078, /// 0x594
			 0xea50b679, /// 0x598
			 0xdf6e9006, /// 0x59c
			 0x57057fcf, /// 0x5a0
			 0x4056439d, /// 0x5a4
			 0xbec1722b, /// 0x5a8
			 0x9bb296d5, /// 0x5ac
			 0xe8f80f28, /// 0x5b0
			 0x3db37531, /// 0x5b4
			 0x962cf87b, /// 0x5b8
			 0x9e31cbb5, /// 0x5bc
			 0x51aea259, /// 0x5c0
			 0x46de19f3, /// 0x5c4
			 0x31e9fe14, /// 0x5c8
			 0xbad5d7a0, /// 0x5cc
			 0xfcd8b801, /// 0x5d0
			 0x7abff13d, /// 0x5d4
			 0x2cb59ade, /// 0x5d8
			 0x6b5d079c, /// 0x5dc
			 0x0414281b, /// 0x5e0
			 0x739b58a8, /// 0x5e4
			 0x0ad518e9, /// 0x5e8
			 0x150445fa, /// 0x5ec
			 0x0a25b80b, /// 0x5f0
			 0x478c2346, /// 0x5f4
			 0x8e520dc7, /// 0x5f8
			 0x6b9c059e, /// 0x5fc
			 0xc37e5918, /// 0x600
			 0x2ef08bc7, /// 0x604
			 0xef2bfe84, /// 0x608
			 0x84365d13, /// 0x60c
			 0xad817751, /// 0x610
			 0xacb5f1ae, /// 0x614
			 0x68caf89e, /// 0x618
			 0xaa3e73b4, /// 0x61c
			 0x71ec4f75, /// 0x620
			 0xf2f7d79c, /// 0x624
			 0x5f9842dd, /// 0x628
			 0x5f75e7a7, /// 0x62c
			 0x90fe9197, /// 0x630
			 0xbb5e65a0, /// 0x634
			 0x1fb58413, /// 0x638
			 0xb3e75998, /// 0x63c
			 0x0afd0115, /// 0x640
			 0x31e80868, /// 0x644
			 0xcc7e5f8f, /// 0x648
			 0xf8a4af56, /// 0x64c
			 0x64e5f755, /// 0x650
			 0x9abe9228, /// 0x654
			 0x8fa8adb1, /// 0x658
			 0xb5f622ca, /// 0x65c
			 0xed018816, /// 0x660
			 0x21db7012, /// 0x664
			 0x67686520, /// 0x668
			 0xc1fca586, /// 0x66c
			 0x5ea7ad25, /// 0x670
			 0x221fee8b, /// 0x674
			 0x79a8c5c9, /// 0x678
			 0x9412bd09, /// 0x67c
			 0xf3c68171, /// 0x680
			 0xbc7e3039, /// 0x684
			 0x699dcd05, /// 0x688
			 0xfa3a422f, /// 0x68c
			 0xa233bb46, /// 0x690
			 0x8802c1e9, /// 0x694
			 0xbd3171ff, /// 0x698
			 0x11c744df, /// 0x69c
			 0xb19c2a6c, /// 0x6a0
			 0x1343da8d, /// 0x6a4
			 0x9f9331ef, /// 0x6a8
			 0x863a326a, /// 0x6ac
			 0xb32de6d4, /// 0x6b0
			 0xdc773aa2, /// 0x6b4
			 0xcb63956a, /// 0x6b8
			 0x70ecf26c, /// 0x6bc
			 0x3cd751b7, /// 0x6c0
			 0x65d64021, /// 0x6c4
			 0x23ed0e74, /// 0x6c8
			 0xb623647f, /// 0x6cc
			 0x485c56dd, /// 0x6d0
			 0x2ceabd1f, /// 0x6d4
			 0xf4410723, /// 0x6d8
			 0xa2289790, /// 0x6dc
			 0x5885e7b5, /// 0x6e0
			 0x5d9a17f7, /// 0x6e4
			 0x1708aba8, /// 0x6e8
			 0x20e733a9, /// 0x6ec
			 0x7f9df9a1, /// 0x6f0
			 0x2cf9f3a9, /// 0x6f4
			 0x46a79f0c, /// 0x6f8
			 0x85ee80e5, /// 0x6fc
			 0x47244df4, /// 0x700
			 0x6b2f60ef, /// 0x704
			 0xea44f912, /// 0x708
			 0xad5b2d66, /// 0x70c
			 0x35b801ff, /// 0x710
			 0x8b114371, /// 0x714
			 0xaa7a2190, /// 0x718
			 0x57fc64c6, /// 0x71c
			 0xcb1cf39e, /// 0x720
			 0xf572a3ae, /// 0x724
			 0x191b4893, /// 0x728
			 0xbafa4822, /// 0x72c
			 0xb3152a44, /// 0x730
			 0xe86cd1ee, /// 0x734
			 0xb63680fc, /// 0x738
			 0x40952a59, /// 0x73c
			 0x6a9b244b, /// 0x740
			 0x66773f59, /// 0x744
			 0x69f6191b, /// 0x748
			 0xbe9f089f, /// 0x74c
			 0xdf854b2d, /// 0x750
			 0xa78c8748, /// 0x754
			 0x108bb923, /// 0x758
			 0xce3f214f, /// 0x75c
			 0x90884983, /// 0x760
			 0xb0580355, /// 0x764
			 0xc8fc0efa, /// 0x768
			 0xbdb75884, /// 0x76c
			 0x0086d7fb, /// 0x770
			 0xb825d0c1, /// 0x774
			 0xf929e02a, /// 0x778
			 0x42724b6b, /// 0x77c
			 0x4b9c4381, /// 0x780
			 0xb2c0635f, /// 0x784
			 0xe88c2847, /// 0x788
			 0x129d9e2f, /// 0x78c
			 0xf8e6d72a, /// 0x790
			 0x5518b596, /// 0x794
			 0xa6aaaf99, /// 0x798
			 0x6bea5311, /// 0x79c
			 0x16884cec, /// 0x7a0
			 0x7df85f7b, /// 0x7a4
			 0x519e8a61, /// 0x7a8
			 0x407cee9f, /// 0x7ac
			 0x81d5c4e5, /// 0x7b0
			 0x8cf3a5ce, /// 0x7b4
			 0x0634e63e, /// 0x7b8
			 0x608fd83e, /// 0x7bc
			 0x2ab07b72, /// 0x7c0
			 0x08210aab, /// 0x7c4
			 0x95e00dc1, /// 0x7c8
			 0xf73143ff, /// 0x7cc
			 0x62d1b430, /// 0x7d0
			 0x338983d9, /// 0x7d4
			 0x33e93764, /// 0x7d8
			 0xf3f0322f, /// 0x7dc
			 0xb995ba7c, /// 0x7e0
			 0x1f02de68, /// 0x7e4
			 0xd1fb04c2, /// 0x7e8
			 0xaf0dd947, /// 0x7ec
			 0xdae20bde, /// 0x7f0
			 0x62fa9ee8, /// 0x7f4
			 0x31f0d524, /// 0x7f8
			 0x1d11d904, /// 0x7fc
			 0x1b31113c, /// 0x800
			 0xc46c42d0, /// 0x804
			 0xde9214a8, /// 0x808
			 0x9c3d012e, /// 0x80c
			 0x3f3d07a8, /// 0x810
			 0x14fba15d, /// 0x814
			 0x273ea68e, /// 0x818
			 0x8a777d58, /// 0x81c
			 0xb6b7f785, /// 0x820
			 0xc9973102, /// 0x824
			 0x74572aac, /// 0x828
			 0xfe7027d0, /// 0x82c
			 0xfe3a5b63, /// 0x830
			 0xac2fa6fb, /// 0x834
			 0xde3c5029, /// 0x838
			 0x41754108, /// 0x83c
			 0x6f900c65, /// 0x840
			 0x6cd7fe82, /// 0x844
			 0x6b20f057, /// 0x848
			 0x1b5cdc29, /// 0x84c
			 0x1c214402, /// 0x850
			 0x2d6abdda, /// 0x854
			 0xcd19b7e6, /// 0x858
			 0x4a56e465, /// 0x85c
			 0x8f19ab61, /// 0x860
			 0xb8d4cf30, /// 0x864
			 0xbb3afa36, /// 0x868
			 0x9a8ad81d, /// 0x86c
			 0x47198d70, /// 0x870
			 0x83347bc0, /// 0x874
			 0xf96c6102, /// 0x878
			 0x7913768a, /// 0x87c
			 0x1e9dc0ca, /// 0x880
			 0xf96b9ae5, /// 0x884
			 0x3cb869b9, /// 0x888
			 0xa9f26c00, /// 0x88c
			 0xfed1a89c, /// 0x890
			 0xc40c1914, /// 0x894
			 0x84ce91da, /// 0x898
			 0xd2387619, /// 0x89c
			 0x14ac7e69, /// 0x8a0
			 0xf9fb6fe6, /// 0x8a4
			 0x2095a493, /// 0x8a8
			 0x561d5d2e, /// 0x8ac
			 0xeecbdf54, /// 0x8b0
			 0x10ff5822, /// 0x8b4
			 0xe9402710, /// 0x8b8
			 0x5f742711, /// 0x8bc
			 0xe2adbf21, /// 0x8c0
			 0x9eb4ed17, /// 0x8c4
			 0x43c55bea, /// 0x8c8
			 0x5e9a35df, /// 0x8cc
			 0x2950cea2, /// 0x8d0
			 0x95b3dd75, /// 0x8d4
			 0xff758964, /// 0x8d8
			 0xf9b5871d, /// 0x8dc
			 0x4ec66154, /// 0x8e0
			 0xf77236e2, /// 0x8e4
			 0x72fd1470, /// 0x8e8
			 0x60ab7498, /// 0x8ec
			 0xd9f5c16f, /// 0x8f0
			 0x8900c541, /// 0x8f4
			 0x8594450f, /// 0x8f8
			 0x9f716305, /// 0x8fc
			 0x683731da, /// 0x900
			 0xcfff614e, /// 0x904
			 0x31addb9b, /// 0x908
			 0x41a94dd5, /// 0x90c
			 0xd8b9fe94, /// 0x910
			 0xf1c87491, /// 0x914
			 0xde78a4e8, /// 0x918
			 0x108fbb03, /// 0x91c
			 0xfc898873, /// 0x920
			 0x5e4dba2c, /// 0x924
			 0xdd414141, /// 0x928
			 0x9691c150, /// 0x92c
			 0xf30bada9, /// 0x930
			 0x2be53e8f, /// 0x934
			 0x79ef4d1b, /// 0x938
			 0xe981be3d, /// 0x93c
			 0xab8e269e, /// 0x940
			 0xa781f300, /// 0x944
			 0x7d2ebfd9, /// 0x948
			 0x51f8aa7e, /// 0x94c
			 0x3dd15b5a, /// 0x950
			 0xbc7667fa, /// 0x954
			 0x7a3e5a9e, /// 0x958
			 0xbdb7215f, /// 0x95c
			 0x7d1e3ec8, /// 0x960
			 0x3b465c5f, /// 0x964
			 0x407ec3e8, /// 0x968
			 0x224581ca, /// 0x96c
			 0xa11e9aa8, /// 0x970
			 0x9068b7ed, /// 0x974
			 0xe6b39ea2, /// 0x978
			 0xdb357165, /// 0x97c
			 0x84052166, /// 0x980
			 0x2045330c, /// 0x984
			 0xe1df899d, /// 0x988
			 0xc1185705, /// 0x98c
			 0x7429947c, /// 0x990
			 0xc79e0eaf, /// 0x994
			 0x44a31025, /// 0x998
			 0xe01761b5, /// 0x99c
			 0xe5bf23bd, /// 0x9a0
			 0x4518f776, /// 0x9a4
			 0xcd44e524, /// 0x9a8
			 0x0c4fc523, /// 0x9ac
			 0x02ff450f, /// 0x9b0
			 0x469bb7e2, /// 0x9b4
			 0x633bbfda, /// 0x9b8
			 0xd25dc2dd, /// 0x9bc
			 0xae552e23, /// 0x9c0
			 0x7c763699, /// 0x9c4
			 0x766e6020, /// 0x9c8
			 0xdb5b656d, /// 0x9cc
			 0xf265a38c, /// 0x9d0
			 0xc6d1f1a3, /// 0x9d4
			 0x4a09a39d, /// 0x9d8
			 0x677fe28f, /// 0x9dc
			 0xbe998efd, /// 0x9e0
			 0xa6001feb, /// 0x9e4
			 0x5d1f4b8a, /// 0x9e8
			 0x37093356, /// 0x9ec
			 0x3395afaa, /// 0x9f0
			 0x1d626f5a, /// 0x9f4
			 0x17af6dc0, /// 0x9f8
			 0xc52537f8, /// 0x9fc
			 0xb6d300d5, /// 0xa00
			 0xd2a6b45e, /// 0xa04
			 0x990171e4, /// 0xa08
			 0x3727333d, /// 0xa0c
			 0x6e0405c4, /// 0xa10
			 0x3e1e06cd, /// 0xa14
			 0xd95b0aab, /// 0xa18
			 0x2d537048, /// 0xa1c
			 0x5ee5d5f4, /// 0xa20
			 0xf36af423, /// 0xa24
			 0x6b852c79, /// 0xa28
			 0xfc270276, /// 0xa2c
			 0x3e951230, /// 0xa30
			 0xf01ce186, /// 0xa34
			 0xae5711f2, /// 0xa38
			 0x60b9a131, /// 0xa3c
			 0x6d541aec, /// 0xa40
			 0x8ca156b3, /// 0xa44
			 0x9ff150ba, /// 0xa48
			 0x615c2ab3, /// 0xa4c
			 0xa7d0e141, /// 0xa50
			 0x396f9626, /// 0xa54
			 0x378198e5, /// 0xa58
			 0x8be06564, /// 0xa5c
			 0x02c275e7, /// 0xa60
			 0x93a90316, /// 0xa64
			 0xe48000e1, /// 0xa68
			 0xb8ac29ab, /// 0xa6c
			 0x9853e98c, /// 0xa70
			 0xbc267928, /// 0xa74
			 0xe1fda9f2, /// 0xa78
			 0x02253e48, /// 0xa7c
			 0xe6d78f44, /// 0xa80
			 0x3e592fcf, /// 0xa84
			 0x4df699cc, /// 0xa88
			 0x6f2268f7, /// 0xa8c
			 0x8b5b6bf0, /// 0xa90
			 0x8e04d609, /// 0xa94
			 0x727c8277, /// 0xa98
			 0x72373c4b, /// 0xa9c
			 0x733c008a, /// 0xaa0
			 0xa2dcba86, /// 0xaa4
			 0x5482c992, /// 0xaa8
			 0xf32e5f02, /// 0xaac
			 0x47f56234, /// 0xab0
			 0x049300b0, /// 0xab4
			 0xfeb8f6fc, /// 0xab8
			 0xa2c6a86b, /// 0xabc
			 0x462d00f0, /// 0xac0
			 0xf3aae1c8, /// 0xac4
			 0xc70f1a09, /// 0xac8
			 0xf7712013, /// 0xacc
			 0x50f55824, /// 0xad0
			 0x4cdfa302, /// 0xad4
			 0x4d276ed4, /// 0xad8
			 0x7ba365fb, /// 0xadc
			 0xf15e34f7, /// 0xae0
			 0x108397bb, /// 0xae4
			 0xf634799f, /// 0xae8
			 0xe863cf34, /// 0xaec
			 0x757273ce, /// 0xaf0
			 0x753cc8e6, /// 0xaf4
			 0xb505fc1e, /// 0xaf8
			 0x0cffbf38, /// 0xafc
			 0x7837eced, /// 0xb00
			 0x3e747b64, /// 0xb04
			 0xfca3d240, /// 0xb08
			 0xa8996455, /// 0xb0c
			 0x32611f21, /// 0xb10
			 0x90837247, /// 0xb14
			 0x46ff31e7, /// 0xb18
			 0x71657165, /// 0xb1c
			 0x8bfb9b61, /// 0xb20
			 0xa89bd1ad, /// 0xb24
			 0x234ebfca, /// 0xb28
			 0xc0b8b4b5, /// 0xb2c
			 0x1edb48fc, /// 0xb30
			 0xd00e5543, /// 0xb34
			 0x90c2fda2, /// 0xb38
			 0xffe87ec3, /// 0xb3c
			 0xb181b402, /// 0xb40
			 0xdc2d81bb, /// 0xb44
			 0xc4075f4a, /// 0xb48
			 0xb8a93a8a, /// 0xb4c
			 0x0ff80e46, /// 0xb50
			 0x6adbd7a1, /// 0xb54
			 0xf6b72ab0, /// 0xb58
			 0x80797266, /// 0xb5c
			 0xbf498552, /// 0xb60
			 0x8bf304aa, /// 0xb64
			 0x0470984b, /// 0xb68
			 0xad047659, /// 0xb6c
			 0x32d42756, /// 0xb70
			 0x76b1ca81, /// 0xb74
			 0xecf6e0fe, /// 0xb78
			 0x70329b60, /// 0xb7c
			 0x95f6ac52, /// 0xb80
			 0x8d7e8374, /// 0xb84
			 0xe6291214, /// 0xb88
			 0x5e7b0cb3, /// 0xb8c
			 0x0bc7dd14, /// 0xb90
			 0xe8ea5fd1, /// 0xb94
			 0xc3e3b2a0, /// 0xb98
			 0xbd9cba03, /// 0xb9c
			 0xac57f5b3, /// 0xba0
			 0xe5fbfec1, /// 0xba4
			 0x679e631c, /// 0xba8
			 0x3e0663ea, /// 0xbac
			 0xf6d060c7, /// 0xbb0
			 0xbdaa3924, /// 0xbb4
			 0x32b8d28c, /// 0xbb8
			 0x428a4156, /// 0xbbc
			 0x07fa969d, /// 0xbc0
			 0x3388840c, /// 0xbc4
			 0x3ddec444, /// 0xbc8
			 0x0b7d6074, /// 0xbcc
			 0xa72ba00d, /// 0xbd0
			 0xc737ffa5, /// 0xbd4
			 0x6e732d58, /// 0xbd8
			 0x17a6d3f1, /// 0xbdc
			 0x54eb73f6, /// 0xbe0
			 0xfbbcbca5, /// 0xbe4
			 0x7049e888, /// 0xbe8
			 0xc8dd47be, /// 0xbec
			 0xfe423a66, /// 0xbf0
			 0xf31d64d8, /// 0xbf4
			 0xd9ca1b3f, /// 0xbf8
			 0xc1d59d30, /// 0xbfc
			 0x88adddc3, /// 0xc00
			 0x106d7003, /// 0xc04
			 0x282a28f0, /// 0xc08
			 0xb795acdc, /// 0xc0c
			 0xfe75874b, /// 0xc10
			 0x5e282968, /// 0xc14
			 0xc7514185, /// 0xc18
			 0x29fc5ab7, /// 0xc1c
			 0x3e5eb15f, /// 0xc20
			 0x2c1b1fcf, /// 0xc24
			 0x6aa769d6, /// 0xc28
			 0x5371a88f, /// 0xc2c
			 0x2382616b, /// 0xc30
			 0x95faa594, /// 0xc34
			 0x4a122801, /// 0xc38
			 0x1fdecba2, /// 0xc3c
			 0xba3aa4fc, /// 0xc40
			 0x63cf652c, /// 0xc44
			 0x6b4252f7, /// 0xc48
			 0xe5bcf357, /// 0xc4c
			 0x7ea5c193, /// 0xc50
			 0xadfd2905, /// 0xc54
			 0xae3ad527, /// 0xc58
			 0xa9fd04de, /// 0xc5c
			 0x33133886, /// 0xc60
			 0xc86becb7, /// 0xc64
			 0xd10633f0, /// 0xc68
			 0xab43332f, /// 0xc6c
			 0x533b204c, /// 0xc70
			 0xacbea518, /// 0xc74
			 0xdb8efb46, /// 0xc78
			 0x9c5201ad, /// 0xc7c
			 0x4cb1608c, /// 0xc80
			 0x07f8bef9, /// 0xc84
			 0xc443825b, /// 0xc88
			 0x7de4bbb1, /// 0xc8c
			 0x723606cc, /// 0xc90
			 0x70524794, /// 0xc94
			 0xfccf8479, /// 0xc98
			 0x6afa21d5, /// 0xc9c
			 0x8d564335, /// 0xca0
			 0x46b3be81, /// 0xca4
			 0x5b1c8d74, /// 0xca8
			 0x76c45d7f, /// 0xcac
			 0x8b8dbc7c, /// 0xcb0
			 0xd2cf491e, /// 0xcb4
			 0x3f0537c7, /// 0xcb8
			 0x77a44791, /// 0xcbc
			 0x195de2cd, /// 0xcc0
			 0x5b7a3eb0, /// 0xcc4
			 0x2042c32f, /// 0xcc8
			 0x80e6bd35, /// 0xccc
			 0x46618e4f, /// 0xcd0
			 0xfef45ca1, /// 0xcd4
			 0x0884b530, /// 0xcd8
			 0x5e55c925, /// 0xcdc
			 0xa0fdc157, /// 0xce0
			 0xb75b0c4a, /// 0xce4
			 0x24441c8b, /// 0xce8
			 0x7e9ec6bf, /// 0xcec
			 0x8febe1c0, /// 0xcf0
			 0xd5c91568, /// 0xcf4
			 0x846ef87d, /// 0xcf8
			 0x8128d6e0, /// 0xcfc
			 0x0cacb46c, /// 0xd00
			 0x982a46e4, /// 0xd04
			 0xce398a7e, /// 0xd08
			 0x2dc29e76, /// 0xd0c
			 0x328af7ad, /// 0xd10
			 0x045a8231, /// 0xd14
			 0x45f613a4, /// 0xd18
			 0xde2bc559, /// 0xd1c
			 0xea41f50d, /// 0xd20
			 0x8607149e, /// 0xd24
			 0xa63fc587, /// 0xd28
			 0x67a3b526, /// 0xd2c
			 0x43eb9d93, /// 0xd30
			 0x3241e22d, /// 0xd34
			 0x0f2c1b56, /// 0xd38
			 0xf447d103, /// 0xd3c
			 0x49e5d2da, /// 0xd40
			 0xa763ba96, /// 0xd44
			 0x5c6e43eb, /// 0xd48
			 0x292e8852, /// 0xd4c
			 0x3f6aaef6, /// 0xd50
			 0xc5d51a08, /// 0xd54
			 0x7cd8c39d, /// 0xd58
			 0x2acf5937, /// 0xd5c
			 0x79f7ad6c, /// 0xd60
			 0xb01f2a1d, /// 0xd64
			 0x93ec15f5, /// 0xd68
			 0x51df0ddf, /// 0xd6c
			 0xe54fd03c, /// 0xd70
			 0x53d5d060, /// 0xd74
			 0x4fa1a69a, /// 0xd78
			 0x4312406d, /// 0xd7c
			 0x63f1dc83, /// 0xd80
			 0x6150360b, /// 0xd84
			 0xf58ea45c, /// 0xd88
			 0x286b35b4, /// 0xd8c
			 0x77a70b40, /// 0xd90
			 0xf7098522, /// 0xd94
			 0x1db8b85e, /// 0xd98
			 0xd4676a1e, /// 0xd9c
			 0x547e89a8, /// 0xda0
			 0x43c122a1, /// 0xda4
			 0xe62733b7, /// 0xda8
			 0x0fe00b8b, /// 0xdac
			 0xd38d3f14, /// 0xdb0
			 0xb06279ec, /// 0xdb4
			 0x4ba0edfd, /// 0xdb8
			 0x0848cc75, /// 0xdbc
			 0x64b2f4d3, /// 0xdc0
			 0x2d9f0aa8, /// 0xdc4
			 0x8ea2671b, /// 0xdc8
			 0x8ff738f8, /// 0xdcc
			 0xbcb56e4e, /// 0xdd0
			 0x7bc8f3e1, /// 0xdd4
			 0x3b869ae6, /// 0xdd8
			 0x26322bd6, /// 0xddc
			 0xa1908584, /// 0xde0
			 0x2a9a9e18, /// 0xde4
			 0x7483dbd9, /// 0xde8
			 0xb0f87850, /// 0xdec
			 0xf1a578a8, /// 0xdf0
			 0x6c136f04, /// 0xdf4
			 0x7b3a8d87, /// 0xdf8
			 0x8ac19a52, /// 0xdfc
			 0x79532f7e, /// 0xe00
			 0xebb2c91a, /// 0xe04
			 0x01503229, /// 0xe08
			 0x21d26d02, /// 0xe0c
			 0x0da22fd0, /// 0xe10
			 0xd43186ef, /// 0xe14
			 0x996702ca, /// 0xe18
			 0x80744b53, /// 0xe1c
			 0x99859e2d, /// 0xe20
			 0x11565c3b, /// 0xe24
			 0xbcb53b35, /// 0xe28
			 0xe704a751, /// 0xe2c
			 0x5a654992, /// 0xe30
			 0x312ef9f5, /// 0xe34
			 0x7bd3d335, /// 0xe38
			 0x3cb0edcd, /// 0xe3c
			 0x81bdef91, /// 0xe40
			 0x5360432b, /// 0xe44
			 0x144a36e5, /// 0xe48
			 0xda543847, /// 0xe4c
			 0x3159ab12, /// 0xe50
			 0x5c4827a1, /// 0xe54
			 0x01881d92, /// 0xe58
			 0x75ceeca6, /// 0xe5c
			 0xfdf02e36, /// 0xe60
			 0xb28cdd68, /// 0xe64
			 0xfd220fdc, /// 0xe68
			 0x5521f816, /// 0xe6c
			 0x1820e1f4, /// 0xe70
			 0xee2e39e8, /// 0xe74
			 0x955d8455, /// 0xe78
			 0x7a8ed7b5, /// 0xe7c
			 0xd2923eb0, /// 0xe80
			 0x4973db23, /// 0xe84
			 0x44159488, /// 0xe88
			 0xe5abf849, /// 0xe8c
			 0x55879c50, /// 0xe90
			 0xf12d0a99, /// 0xe94
			 0x664984e2, /// 0xe98
			 0xf583f1fc, /// 0xe9c
			 0x7955a522, /// 0xea0
			 0xdebc6ec0, /// 0xea4
			 0x35816e24, /// 0xea8
			 0xdc57e932, /// 0xeac
			 0xeb23fd1c, /// 0xeb0
			 0xf8237389, /// 0xeb4
			 0xe9f28143, /// 0xeb8
			 0x19e793f9, /// 0xebc
			 0xd8cfeef2, /// 0xec0
			 0x395aac62, /// 0xec4
			 0x9855c0ea, /// 0xec8
			 0x993e4464, /// 0xecc
			 0xe6fab89a, /// 0xed0
			 0x4150c8f4, /// 0xed4
			 0x90a11f2a, /// 0xed8
			 0x97c529a4, /// 0xedc
			 0x80904172, /// 0xee0
			 0x67790e73, /// 0xee4
			 0xe72bd629, /// 0xee8
			 0x0c8df15e, /// 0xeec
			 0x03f4da19, /// 0xef0
			 0xec45528b, /// 0xef4
			 0x7ea220cd, /// 0xef8
			 0xbe673e04, /// 0xefc
			 0x6b903d2a, /// 0xf00
			 0x1ca4e4e1, /// 0xf04
			 0x609c458c, /// 0xf08
			 0x6ccf1afb, /// 0xf0c
			 0xba5deeca, /// 0xf10
			 0xd1b444fc, /// 0xf14
			 0x071526ed, /// 0xf18
			 0xb3d8096b, /// 0xf1c
			 0xe1d6afea, /// 0xf20
			 0x933da84b, /// 0xf24
			 0x963553d3, /// 0xf28
			 0xfd706bce, /// 0xf2c
			 0x27da8890, /// 0xf30
			 0x4c772aca, /// 0xf34
			 0x60454946, /// 0xf38
			 0x5d3f7c02, /// 0xf3c
			 0xe8cec843, /// 0xf40
			 0xa7601ef9, /// 0xf44
			 0x47ab75e9, /// 0xf48
			 0xadae0ada, /// 0xf4c
			 0xc7676784, /// 0xf50
			 0xf73aa9c7, /// 0xf54
			 0x8ff70db4, /// 0xf58
			 0x636c0cfc, /// 0xf5c
			 0xba8c8d57, /// 0xf60
			 0xac89dc80, /// 0xf64
			 0x38af6605, /// 0xf68
			 0x8892cfac, /// 0xf6c
			 0x5d02204a, /// 0xf70
			 0x3320fc2a, /// 0xf74
			 0xce70cee1, /// 0xf78
			 0x2a9309e3, /// 0xf7c
			 0xd95d6309, /// 0xf80
			 0x697beccb, /// 0xf84
			 0x1aecd662, /// 0xf88
			 0x58564d02, /// 0xf8c
			 0xbfba2bb8, /// 0xf90
			 0x79346731, /// 0xf94
			 0x47f1c83a, /// 0xf98
			 0x40060903, /// 0xf9c
			 0xcd619669, /// 0xfa0
			 0xdf22e363, /// 0xfa4
			 0x2a4815fe, /// 0xfa8
			 0x6a2a2a2b, /// 0xfac
			 0x3274e966, /// 0xfb0
			 0x1158bac7, /// 0xfb4
			 0xb624e379, /// 0xfb8
			 0xc5a2d188, /// 0xfbc
			 0x534fb432, /// 0xfc0
			 0xbd4f4b62, /// 0xfc4
			 0x99cd5972, /// 0xfc8
			 0x90cb99ab, /// 0xfcc
			 0x765092f3, /// 0xfd0
			 0x487b5d17, /// 0xfd4
			 0x200ff2bb, /// 0xfd8
			 0x443b6094, /// 0xfdc
			 0x68fe2b48, /// 0xfe0
			 0x9284ab09, /// 0xfe4
			 0x2ab91e37, /// 0xfe8
			 0x0f21d356, /// 0xfec
			 0x7dc6447c, /// 0xff0
			 0xbcb074a8, /// 0xff4
			 0xf6ae9f72, /// 0xff8
			 0xf09986cc /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00000
			 0x0c700000,                                                  // Leading 1s:                                 /// 00004
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0000c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00010
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00014
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00018
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0001c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00020
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0c600000,                                                  // Leading 1s:                                 /// 00028
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0002c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00030
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00038
			 0x0e000001,                                                  // Trailing 1s:                                /// 0003c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00044
			 0xffc00001,                                                  // -signaling NaN                              /// 00048
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0004c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00050
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00054
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00058
			 0x00000000,                                                  // zero                                        /// 0005c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00060
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00064
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00068
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0006c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00070
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0007c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00080
			 0xbf028f5c,                                                  // -0.51                                       /// 00084
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00088
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0008c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00090
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00094
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0009c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00100
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00104
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00108
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00110
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00114
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0011c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00124
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00128
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x7f800000,                                                  // inf                                         /// 00130
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00134
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00138
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0013c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00144
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00154
			 0x3f028f5c,                                                  // 0.51                                        /// 00158
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00160
			 0x80000000,                                                  // -zero                                       /// 00164
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00170
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00174
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0017c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x80000000,                                                  // -zero                                       /// 0018c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001b8
			 0xff800000,                                                  // -inf                                        /// 001bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001c8
			 0x7f800000,                                                  // inf                                         /// 001cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001d4
			 0x4b000000,                                                  // 8388608.0                                   /// 001d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00200
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00204
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00218
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0021c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00220
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00224
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00228
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0022c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00230
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00238
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0023c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00240
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00244
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0024c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00250
			 0xcb000000,                                                  // -8388608.0                                  /// 00254
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00258
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0025c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00264
			 0x80011111,                                                  // -9.7958E-41                                 /// 00268
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00270
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x7fc00001,                                                  // signaling NaN                               /// 00278
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00284
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0028c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00290
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0x0c400000,                                                  // Leading 1s:                                 /// 00298
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x7fc00001,                                                  // signaling NaN                               /// 002d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00304
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00308
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0030c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00310
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00318
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0031c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00320
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00324
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00328
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0032c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00334
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00338
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0033c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00348
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0xbf028f5c,                                                  // -0.51                                       /// 00350
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00354
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0035c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00360
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00364
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0036c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00370
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00374
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x33333333,                                                  // 4 random values                             /// 0037c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00380
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x80011111,                                                  // -9.7958E-41                                 /// 00388
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0038c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00390
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0039c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x80000000,                                                  // -zero                                       /// 003c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x80011111,                                                  // -9.7958E-41                                 /// 0040c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00410
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00414
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00418
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0041c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00420
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00424
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0042c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00430
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00434
			 0x0c600000,                                                  // Leading 1s:                                 /// 00438
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00011111,                                                  // 9.7958E-41                                  /// 0044c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00454
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00458
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0045c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00460
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00468
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00470
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00480
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00484
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00488
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0048c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00490
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00498
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0049c
			 0x80000000,                                                  // -zero                                       /// 004a0
			 0xffc00001,                                                  // -signaling NaN                              /// 004a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004c4
			 0xffc00001,                                                  // -signaling NaN                              /// 004c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 004dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00500
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00504
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0e000003,                                                  // Trailing 1s:                                /// 0050c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00518
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0051c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00520
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00524
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0052c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00534
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00538
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00540
			 0x0e000003,                                                  // Trailing 1s:                                /// 00544
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00548
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0054c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00554
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00558
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0055c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00564
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0056c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00570
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00574
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00580
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00584
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00588
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00594
			 0x4b000000,                                                  // 8388608.0                                   /// 00598
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0xcb000000,                                                  // -8388608.0                                  /// 005a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x4b000000,                                                  // 8388608.0                                   /// 005dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00600
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00604
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00608
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0060c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00614
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00618
			 0x00000000,                                                  // zero                                        /// 0061c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00620
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00624
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00628
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00630
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00634
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00638
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0063c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00640
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00644
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00658
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0065c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x80011111,                                                  // -9.7958E-41                                 /// 00664
			 0x0e000003,                                                  // Trailing 1s:                                /// 00668
			 0x55555555,                                                  // 4 random values                             /// 0066c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00670
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00674
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00678
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00680
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00688
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0068c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0xcb000000,                                                  // -8388608.0                                  /// 00694
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00700
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e000007,                                                  // Trailing 1s:                                /// 00708
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0070c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00710
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00714
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0071c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00720
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00724
			 0x0e000007,                                                  // Trailing 1s:                                /// 00728
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0072c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0e000007,                                                  // Trailing 1s:                                /// 00734
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00738
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0073c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00740
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00744
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00748
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00750
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00758
			 0xbf028f5c,                                                  // -0.51                                       /// 0075c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00760
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00764
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00768
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0076c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00778
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00780
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00788
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0078c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00794
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00798
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0079c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 007f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00800
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x0e000007,                                                  // Trailing 1s:                                /// 00808
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00814
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0081c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0xffc00001,                                                  // -signaling NaN                              /// 00824
			 0x0c780000,                                                  // Leading 1s:                                 /// 00828
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0082c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00830
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00838
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00840
			 0x33333333,                                                  // 4 random values                             /// 00844
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00848
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0084c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7fc00001,                                                  // signaling NaN                               /// 00854
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x00011111,                                                  // 9.7958E-41                                  /// 0085c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00868
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0086c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00870
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00874
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0087c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00880
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00888
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0088c
			 0x3f028f5c,                                                  // 0.51                                        /// 00890
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00894
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00898
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0089c
			 0x0c600000,                                                  // Leading 1s:                                 /// 008a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008a4
			 0xffc00001,                                                  // -signaling NaN                              /// 008a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008b0
			 0xff800000,                                                  // -inf                                        /// 008b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x55555555,                                                  // 4 random values                             /// 008f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00900
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00904
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0090c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00910
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00914
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0091c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00920
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00924
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00928
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00930
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0093c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00944
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0094c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00950
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x3f028f5c,                                                  // 0.51                                        /// 00958
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00964
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00970
			 0x80011111,                                                  // -9.7958E-41                                 /// 00974
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00978
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00984
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00994
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00998
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0099c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009b0
			 0x80000000,                                                  // -zero                                       /// 009b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00000000,                                                  // zero                                        /// 009d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 009f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a14
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a2c
			 0x55555555,                                                  // 4 random values                             /// 00a30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a40
			 0x33333333,                                                  // 4 random values                             /// 00a44
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x55555555,                                                  // 4 random values                             /// 00a5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a64
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a90
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00aa4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aa8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80000000,                                                  // -zero                                       /// 00ab0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ab8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00abc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac4
			 0xff800000,                                                  // -inf                                        /// 00ac8
			 0x4b000000,                                                  // 8388608.0                                   /// 00acc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ad8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00adc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ae4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ae8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aec
			 0x0c400000,                                                  // Leading 1s:                                 /// 00af0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00af4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00af8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00afc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80000000,                                                  // -zero                                       /// 00b24
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b2c
			 0x80000000,                                                  // -zero                                       /// 00b30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b50
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b7c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ba0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x55555555,                                                  // 4 random values                             /// 00bac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bb4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bbc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bc0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bc4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bcc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bd8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00be4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00be8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bf4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bf8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bfc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c04
			 0x7f800000,                                                  // inf                                         /// 00c08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c18
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c30
			 0x33333333,                                                  // 4 random values                             /// 00c34
			 0x3f028f5c,                                                  // 0.51                                        /// 00c38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c48
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c5c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c68
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c74
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c84
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c90
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ca0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ca4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ca8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cb0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cbc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cc8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cd0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cdc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ce0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ce4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cf0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cfc
			 0xbf028f5c,                                                  // -0.51                                       /// 00d00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d04
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d08
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x55555555,                                                  // 4 random values                             /// 00d14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d18
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d64
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00da4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00db0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00db4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00db8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dc4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dd4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dd8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ddc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00de0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00de8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x3f028f5c,                                                  // 0.51                                        /// 00df4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00df8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dfc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0xcb000000,                                                  // -8388608.0                                  /// 00e0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x7f800000,                                                  // inf                                         /// 00e38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x55555555,                                                  // 4 random values                             /// 00e64
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e70
			 0x3f028f5c,                                                  // 0.51                                        /// 00e74
			 0x7fc00001,                                                  // signaling NaN                               /// 00e78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e9c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00eac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ebc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ec0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ec8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ecc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ed8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ee0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ee4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00eec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ef0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ef4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00efc
			 0xcb000000,                                                  // -8388608.0                                  /// 00f00
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f10
			 0x3f028f5c,                                                  // 0.51                                        /// 00f14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f1c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f28
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f4c
			 0xff800000,                                                  // -inf                                        /// 00f50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x80000000,                                                  // -zero                                       /// 00f6c
			 0x00000000,                                                  // zero                                        /// 00f70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f8c
			 0x7f800000,                                                  // inf                                         /// 00f90
			 0x3f028f5c,                                                  // 0.51                                        /// 00f94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fa0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fa8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fb4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fb8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fbc
			 0x00000000,                                                  // zero                                        /// 00fc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x55555555,                                                  // 4 random values                             /// 00fc8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fd0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fd8
			 0xff800000,                                                  // -inf                                        /// 00fdc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fe0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fe4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ff4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00080000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00000
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00008
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0000c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00014
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00018
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00020
			 0x0e000001,                                                  // Trailing 1s:                                /// 00024
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00034
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0003c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00040
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00050
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00058
			 0xbf028f5c,                                                  // -0.51                                       /// 0005c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00060
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00064
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0006c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00070
			 0x33333333,                                                  // 4 random values                             /// 00074
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00078
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0007c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00080
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00084
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00088
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0008c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0009c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x3f028f5c,                                                  // 0.51                                        /// 000cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00100
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00104
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00108
			 0xff800000,                                                  // -inf                                        /// 0010c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00110
			 0xffc00001,                                                  // -signaling NaN                              /// 00114
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00118
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x7fc00001,                                                  // signaling NaN                               /// 00120
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00124
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0012c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00134
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00138
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0013c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0c600000,                                                  // Leading 1s:                                 /// 00144
			 0x0c400000,                                                  // Leading 1s:                                 /// 00148
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0014c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00150
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00154
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x7fc00001,                                                  // signaling NaN                               /// 00160
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0016c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00170
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00178
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x3f028f5c,                                                  // 0.51                                        /// 00180
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00184
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00188
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0018c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0019c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001b8
			 0x7f800000,                                                  // inf                                         /// 001bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e8
			 0x4b000000,                                                  // 8388608.0                                   /// 001ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00200
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00208
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00218
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00220
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00224
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00230
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00234
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00238
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0023c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00244
			 0x7fc00001,                                                  // signaling NaN                               /// 00248
			 0x0c700000,                                                  // Leading 1s:                                 /// 0024c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0c600000,                                                  // Leading 1s:                                 /// 00254
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00260
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00264
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00274
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00278
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0027c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00280
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00284
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00294
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0029c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002a0
			 0xcb000000,                                                  // -8388608.0                                  /// 002a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x4b000000,                                                  // 8388608.0                                   /// 002d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00300
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00304
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0030c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00318
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0031c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00320
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00328
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00330
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00334
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00338
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00340
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00344
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0034c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00350
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00354
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00358
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x80000000,                                                  // -zero                                       /// 00368
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00370
			 0x0c700000,                                                  // Leading 1s:                                 /// 00374
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0037c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00380
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00384
			 0x7fc00001,                                                  // signaling NaN                               /// 00388
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0038c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00394
			 0x4b000000,                                                  // 8388608.0                                   /// 00398
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003a8
			 0x80000000,                                                  // -zero                                       /// 003ac
			 0x33333333,                                                  // 4 random values                             /// 003b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00400
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00408
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0040c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00410
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00414
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00418
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0041c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00428
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0042c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00434
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0043c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00448
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0044c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00450
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00458
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0045c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00460
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00464
			 0x33333333,                                                  // 4 random values                             /// 00468
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0046c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00478
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00480
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00484
			 0xffc00001,                                                  // -signaling NaN                              /// 00488
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0048c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00490
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00494
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00498
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0049c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0xbf028f5c,                                                  // -0.51                                       /// 004d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00000000,                                                  // zero                                        /// 004f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00508
			 0x0c700000,                                                  // Leading 1s:                                 /// 0050c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00518
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0051c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00524
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0052c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00534
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00538
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00540
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00544
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00548
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00550
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00558
			 0x80011111,                                                  // -9.7958E-41                                 /// 0055c
			 0xcb000000,                                                  // -8388608.0                                  /// 00560
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00564
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00568
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0056c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00570
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00584
			 0x7fc00001,                                                  // signaling NaN                               /// 00588
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0058c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00590
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00598
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0059c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0xff800000,                                                  // -inf                                        /// 005f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00600
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00610
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00618
			 0x0c780000,                                                  // Leading 1s:                                 /// 0061c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00620
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00630
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00634
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0e000001,                                                  // Trailing 1s:                                /// 0063c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00644
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00648
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0064c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00654
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00664
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00668
			 0xcb000000,                                                  // -8388608.0                                  /// 0066c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00670
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00674
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0067c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00680
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00684
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00688
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0068c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00694
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00698
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0069c
			 0x7f800000,                                                  // inf                                         /// 006a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0xbf028f5c,                                                  // -0.51                                       /// 006b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006dc
			 0x4b000000,                                                  // 8388608.0                                   /// 006e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00700
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00714
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00718
			 0x33333333,                                                  // 4 random values                             /// 0071c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00720
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00724
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0072c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00730
			 0x80000000,                                                  // -zero                                       /// 00734
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00738
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0073c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00744
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00748
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0074c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00750
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00754
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00758
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0075c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00760
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0076c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00770
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00774
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0077c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00780
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00784
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00788
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00798
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0xcb000000,                                                  // -8388608.0                                  /// 007bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007d0
			 0x55555555,                                                  // 4 random values                             /// 007d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00804
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00814
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00818
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00828
			 0x4b000000,                                                  // 8388608.0                                   /// 0082c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00830
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00834
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00838
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0083c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0xcb000000,                                                  // -8388608.0                                  /// 00844
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00848
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0084c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00854
			 0x7fc00001,                                                  // signaling NaN                               /// 00858
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0085c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00860
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0e000003,                                                  // Trailing 1s:                                /// 0086c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00870
			 0x0e000001,                                                  // Trailing 1s:                                /// 00874
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00880
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00888
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0088c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00898
			 0x3f028f5c,                                                  // 0.51                                        /// 0089c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008f0
			 0x80000000,                                                  // -zero                                       /// 008f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00900
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00904
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x0e000007,                                                  // Trailing 1s:                                /// 0090c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0091c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00920
			 0xbf028f5c,                                                  // -0.51                                       /// 00924
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0092c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00938
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0093c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00958
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0095c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00970
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00974
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0097c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00980
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00984
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0098c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00990
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0099c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009bc
			 0x00000000,                                                  // zero                                        /// 009c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009d4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a74
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a80
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00aa0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aa8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ab8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00abc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ac4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ac8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00acc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ad0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ad8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ae4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x80000000,                                                  // -zero                                       /// 00af4
			 0x33333333,                                                  // 4 random values                             /// 00af8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00afc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b04
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b0c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b34
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b60
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x3f028f5c,                                                  // 0.51                                        /// 00b8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ba8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0xbf028f5c,                                                  // -0.51                                       /// 00bd0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bd8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bdc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00be0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00be8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bf0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c20
			 0x80000000,                                                  // -zero                                       /// 00c24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c30
			 0x80000000,                                                  // -zero                                       /// 00c34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c70
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ca0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ca8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ccc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x7fc00001,                                                  // signaling NaN                               /// 00cd8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ce0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ce4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ce8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cfc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d08
			 0xff800000,                                                  // -inf                                        /// 00d0c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d2c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x80000000,                                                  // -zero                                       /// 00d48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d70
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d8c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00da4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00da8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00db0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00db4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00db8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dc0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dcc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ddc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00de0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00de4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00de8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00df4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00df8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0xbf028f5c,                                                  // -0.51                                       /// 00e00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e14
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e18
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e2c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0xff800000,                                                  // -inf                                        /// 00e34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e38
			 0x3f028f5c,                                                  // 0.51                                        /// 00e3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80000000,                                                  // -zero                                       /// 00e54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e8c
			 0x80000000,                                                  // -zero                                       /// 00e90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ea0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ea4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eb0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eb8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ebc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ec0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ec4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ec8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ecc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ee4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ee8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00eec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ef0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ef4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f0c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f18
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f2c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f30
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f3c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f48
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f64
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f68
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f70
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fa0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fa8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fb0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fb4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fb8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fc0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fc8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fcc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fd0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fd4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fdc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fe4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ff0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ff4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ff8
			 0x80000001                                                  // -1.4E-45 (-denorm)                          /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00000
			 0xffc00001,                                                  // -signaling NaN                              /// 00004
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00010
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00014
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0001c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00020
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00024
			 0x00000000,                                                  // zero                                        /// 00028
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0002c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00030
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00034
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0003c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00040
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00044
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00048
			 0xff800000,                                                  // -inf                                        /// 0004c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00054
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00058
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00060
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x3f028f5c,                                                  // 0.51                                        /// 00068
			 0x80011111,                                                  // -9.7958E-41                                 /// 0006c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00070
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00080
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00084
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00088
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0008c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00090
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00094
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00098
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0009c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b4
			 0xff800000,                                                  // -inf                                        /// 000b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000e4
			 0x7f800000,                                                  // inf                                         /// 000e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00100
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x55555555,                                                  // 4 random values                             /// 00108
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0010c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0011c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00120
			 0x80000000,                                                  // -zero                                       /// 00124
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00128
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00130
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0013c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00140
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00148
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00154
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00158
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00160
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00164
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00168
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00170
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00178
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00180
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00184
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00188
			 0xffc00001,                                                  // -signaling NaN                              /// 0018c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00190
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00194
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00198
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001a0
			 0x33333333,                                                  // 4 random values                             /// 001a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 001e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001ec
			 0xff800000,                                                  // -inf                                        /// 001f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00200
			 0x0c700000,                                                  // Leading 1s:                                 /// 00204
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00208
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0020c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00210
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00218
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0021c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00224
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00228
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0022c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00230
			 0x0c700000,                                                  // Leading 1s:                                 /// 00234
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00238
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x4b000000,                                                  // 8388608.0                                   /// 00240
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00248
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0024c
			 0x7f800000,                                                  // inf                                         /// 00250
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0c600000,                                                  // Leading 1s:                                 /// 00258
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0025c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00260
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00264
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00270
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00280
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00284
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00294
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 002b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 002c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e0
			 0xbf028f5c,                                                  // -0.51                                       /// 002e4
			 0x80000000,                                                  // -zero                                       /// 002e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00310
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0031c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00320
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00324
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00328
			 0x00011111,                                                  // 9.7958E-41                                  /// 0032c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00330
			 0x0e000003,                                                  // Trailing 1s:                                /// 00334
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00338
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0033c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00344
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00348
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00350
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00354
			 0x3f028f5c,                                                  // 0.51                                        /// 00358
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0035c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00360
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00364
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0036c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00370
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00374
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00378
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0037c
			 0x3f028f5c,                                                  // 0.51                                        /// 00380
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00384
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0038c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00390
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00394
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00398
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80000000,                                                  // -zero                                       /// 003a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003d4
			 0x3f028f5c,                                                  // 0.51                                        /// 003d8
			 0x7f800000,                                                  // inf                                         /// 003dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 003e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003f0
			 0x33333333,                                                  // 4 random values                             /// 003f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00400
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0c400000,                                                  // Leading 1s:                                 /// 00408
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0040c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00410
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00414
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0041c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00420
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00424
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00428
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00430
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00434
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00438
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0043c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00440
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00444
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00448
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0044c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00450
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00458
			 0x0e000007,                                                  // Trailing 1s:                                /// 0045c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00464
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00468
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0046c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00470
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00474
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00480
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00488
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0048c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00498
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0049c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004ac
			 0x80000000,                                                  // -zero                                       /// 004b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 004c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 004d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00500
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00508
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0050c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00510
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0051c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00528
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0053c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00544
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00548
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0054c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00550
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00558
			 0x4b000000,                                                  // 8388608.0                                   /// 0055c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00560
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00568
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00578
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0xff800000,                                                  // -inf                                        /// 00580
			 0x80011111,                                                  // -9.7958E-41                                 /// 00584
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00588
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0058c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00590
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00594
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00598
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0059c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x33333333,                                                  // 4 random values                             /// 005d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005f4
			 0x7f800000,                                                  // inf                                         /// 005f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00600
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00608
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0060c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00610
			 0x0c700000,                                                  // Leading 1s:                                 /// 00614
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0e000003,                                                  // Trailing 1s:                                /// 0061c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00620
			 0x0c600000,                                                  // Leading 1s:                                 /// 00624
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00628
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0062c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00634
			 0x00011111,                                                  // 9.7958E-41                                  /// 00638
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0063c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00640
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0064c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00654
			 0xff800000,                                                  // -inf                                        /// 00658
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0065c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00660
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00664
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00668
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00674
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00678
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00680
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00688
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00690
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00694
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00698
			 0x3f028f5c,                                                  // 0.51                                        /// 0069c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006a4
			 0x4b000000,                                                  // 8388608.0                                   /// 006a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006ac
			 0xff800000,                                                  // -inf                                        /// 006b0
			 0xbf028f5c,                                                  // -0.51                                       /// 006b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00704
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00708
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00710
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00718
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00720
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00724
			 0xff800000,                                                  // -inf                                        /// 00728
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0072c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00730
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00734
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00740
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00744
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00748
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0074c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00750
			 0x0c400000,                                                  // Leading 1s:                                 /// 00754
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0075c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00760
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0076c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00770
			 0x00000000,                                                  // zero                                        /// 00774
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0078c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00790
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007a4
			 0x3f028f5c,                                                  // 0.51                                        /// 007a8
			 0x3f028f5c,                                                  // 0.51                                        /// 007ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00800
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00804
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0080c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00810
			 0xff800000,                                                  // -inf                                        /// 00814
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0081c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00820
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00828
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0082c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00830
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00834
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00838
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0083c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00840
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00844
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00848
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0084c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00850
			 0x0c600000,                                                  // Leading 1s:                                 /// 00854
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00858
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0085c
			 0x4b000000,                                                  // 8388608.0                                   /// 00860
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00864
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00868
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0087c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00884
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0088c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00890
			 0x80000000,                                                  // -zero                                       /// 00894
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xcb000000,                                                  // -8388608.0                                  /// 0089c
			 0x55555555,                                                  // 4 random values                             /// 008a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x7fc00001,                                                  // signaling NaN                               /// 008b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008f0
			 0xffc00001,                                                  // -signaling NaN                              /// 008f4
			 0xff800000,                                                  // -inf                                        /// 008f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008fc
			 0x55555555,                                                  // 4 random values                             /// 00900
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00904
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00908
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x7f800000,                                                  // inf                                         /// 00910
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x0c400000,                                                  // Leading 1s:                                 /// 00918
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00920
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00928
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0092c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00940
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00944
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0095c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00960
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00964
			 0x7fc00001,                                                  // signaling NaN                               /// 00968
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0096c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00970
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00974
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00978
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0097c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00980
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c600000,                                                  // Leading 1s:                                 /// 00988
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0098c
			 0x00000000,                                                  // zero                                        /// 00990
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00998
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x7f800000,                                                  // inf                                         /// 009cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009fc
			 0x55555555,                                                  // 4 random values                             /// 00a00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a28
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aa0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aa8
			 0xff800000,                                                  // -inf                                        /// 00aac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ab0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ab4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ac4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ac8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00adc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ae8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00aec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00af0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00afc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b04
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b18
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b2c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b34
			 0x3f028f5c,                                                  // 0.51                                        /// 00b38
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b94
			 0x00000000,                                                  // zero                                        /// 00b98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ba4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bb4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bc8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bd4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bd8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bdc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00be8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bec
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bf4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80000000,                                                  // -zero                                       /// 00c00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x55555555,                                                  // 4 random values                             /// 00c10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c30
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c38
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c68
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c74
			 0xcb000000,                                                  // -8388608.0                                  /// 00c78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c88
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0xbf028f5c,                                                  // -0.51                                       /// 00c94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c98
			 0xcb000000,                                                  // -8388608.0                                  /// 00c9c
			 0x55555555,                                                  // 4 random values                             /// 00ca0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cb0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cb4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cbc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cc0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cc8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x4b000000,                                                  // 8388608.0                                   /// 00cd0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cdc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ce4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d28
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d38
			 0x00000000,                                                  // zero                                        /// 00d3c
			 0x7f800000,                                                  // inf                                         /// 00d40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0xbf028f5c,                                                  // -0.51                                       /// 00d54
			 0x4b000000,                                                  // 8388608.0                                   /// 00d58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d5c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d6c
			 0x80000000,                                                  // -zero                                       /// 00d70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d88
			 0x80000000,                                                  // -zero                                       /// 00d8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00da8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00db0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00db4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00000000,                                                  // zero                                        /// 00dbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dc0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dc4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dc8
			 0x80000000,                                                  // -zero                                       /// 00dcc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dd0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00dd8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ddc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00de0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00de4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00df4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00df8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e54
			 0xcb000000,                                                  // -8388608.0                                  /// 00e58
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e68
			 0x7fc00001,                                                  // signaling NaN                               /// 00e6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e70
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e78
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ea0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ea4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ea8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00eb0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eb4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00eb8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ebc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ec8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ed4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ed8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ee0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00eec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ef0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ef4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ef8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00efc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f04
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f14
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f24
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f28
			 0xffc00001,                                                  // -signaling NaN                              /// 00f2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f30
			 0xffc00001,                                                  // -signaling NaN                              /// 00f34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00f50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f54
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f5c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f70
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f78
			 0x3f028f5c,                                                  // 0.51                                        /// 00f7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f90
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fa0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fa4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fa8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fb4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fbc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fcc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fd4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x4b000000,                                                  // 8388608.0                                   /// 00fe4
			 0x7fc00001,                                                  // signaling NaN                               /// 00fe8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ff0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x0c7fffc0                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x1604e034, /// 0x0
			 0xe82c2a58, /// 0x4
			 0xc68c6bf2, /// 0x8
			 0x3dd60445, /// 0xc
			 0x11a417ce, /// 0x10
			 0x46f6c361, /// 0x14
			 0x8c4e7afb, /// 0x18
			 0x5f84f177, /// 0x1c
			 0xf9926807, /// 0x20
			 0x189e264d, /// 0x24
			 0xb050e810, /// 0x28
			 0x5910e356, /// 0x2c
			 0x29c791e4, /// 0x30
			 0x101ef142, /// 0x34
			 0x4e804449, /// 0x38
			 0x90228ff1, /// 0x3c
			 0x57f431d3, /// 0x40
			 0x1ba11d26, /// 0x44
			 0xbf323e61, /// 0x48
			 0xeabae720, /// 0x4c
			 0x0de841eb, /// 0x50
			 0x178bc008, /// 0x54
			 0x0d5371ec, /// 0x58
			 0xdc4b6248, /// 0x5c
			 0x2e8d6662, /// 0x60
			 0x97b08ace, /// 0x64
			 0xd945963c, /// 0x68
			 0x2392cb85, /// 0x6c
			 0xd15c429b, /// 0x70
			 0x8e20df1a, /// 0x74
			 0x5ae86b2b, /// 0x78
			 0xd7bcee85, /// 0x7c
			 0xea3d4ea3, /// 0x80
			 0x38890ec2, /// 0x84
			 0xcaeaee75, /// 0x88
			 0xe8d08e93, /// 0x8c
			 0x66d02c72, /// 0x90
			 0x738073fe, /// 0x94
			 0x1c9bbb8c, /// 0x98
			 0x5659aa53, /// 0x9c
			 0x8592a911, /// 0xa0
			 0x57cf4e44, /// 0xa4
			 0x8077b629, /// 0xa8
			 0x28189832, /// 0xac
			 0xe45e1876, /// 0xb0
			 0x44e81501, /// 0xb4
			 0xb7e0635c, /// 0xb8
			 0xa8a1b813, /// 0xbc
			 0xa3cf29c2, /// 0xc0
			 0x6d08c7bf, /// 0xc4
			 0xe600e9f2, /// 0xc8
			 0xe8a0e45e, /// 0xcc
			 0x1b733a93, /// 0xd0
			 0x15c5ce6c, /// 0xd4
			 0x1ff815b1, /// 0xd8
			 0x3fc808da, /// 0xdc
			 0x438ed281, /// 0xe0
			 0x8b744604, /// 0xe4
			 0x84cb7361, /// 0xe8
			 0xc436e063, /// 0xec
			 0x583a8710, /// 0xf0
			 0x2004774a, /// 0xf4
			 0x225ed51f, /// 0xf8
			 0xe0066052, /// 0xfc
			 0x72db38eb, /// 0x100
			 0x8f3c08f5, /// 0x104
			 0x6659e486, /// 0x108
			 0x1614b084, /// 0x10c
			 0x6995e5ef, /// 0x110
			 0x705136ef, /// 0x114
			 0x592a0f68, /// 0x118
			 0x917b1c90, /// 0x11c
			 0x3e085d32, /// 0x120
			 0x2fd13388, /// 0x124
			 0x74a7a6a8, /// 0x128
			 0x8667d612, /// 0x12c
			 0xda96b781, /// 0x130
			 0xce354f70, /// 0x134
			 0xaf05b16c, /// 0x138
			 0x8c9f1789, /// 0x13c
			 0xf7399fa8, /// 0x140
			 0x77fc8d12, /// 0x144
			 0xc7dbe911, /// 0x148
			 0x456ad786, /// 0x14c
			 0xfa79b80a, /// 0x150
			 0x02412ba1, /// 0x154
			 0xf11b7f73, /// 0x158
			 0x5d8d3dd5, /// 0x15c
			 0x762a06d8, /// 0x160
			 0xb30c0a50, /// 0x164
			 0x64d8d957, /// 0x168
			 0x95b1d0de, /// 0x16c
			 0x5800e2e3, /// 0x170
			 0xef4dd5fb, /// 0x174
			 0x671c17a5, /// 0x178
			 0xfbba3bc1, /// 0x17c
			 0x697e9d43, /// 0x180
			 0xfbef3f3d, /// 0x184
			 0xad49b1a4, /// 0x188
			 0xb82b8285, /// 0x18c
			 0xe7419435, /// 0x190
			 0x3e013c08, /// 0x194
			 0x4be96bd9, /// 0x198
			 0xfaf681cd, /// 0x19c
			 0x6c103dcf, /// 0x1a0
			 0xf27f0bdb, /// 0x1a4
			 0x52d0ddb7, /// 0x1a8
			 0x230ca933, /// 0x1ac
			 0xf4fe809f, /// 0x1b0
			 0xc600ab85, /// 0x1b4
			 0x10b52df8, /// 0x1b8
			 0x189fd209, /// 0x1bc
			 0x2abd14c6, /// 0x1c0
			 0x92f08289, /// 0x1c4
			 0x03bb0c1e, /// 0x1c8
			 0xdcc8d35b, /// 0x1cc
			 0xa28cc9c7, /// 0x1d0
			 0xadfaa49b, /// 0x1d4
			 0xdd83f765, /// 0x1d8
			 0x63399c03, /// 0x1dc
			 0x1974b039, /// 0x1e0
			 0x091c6e71, /// 0x1e4
			 0xfd65e90e, /// 0x1e8
			 0xb05cc3f8, /// 0x1ec
			 0x757afa82, /// 0x1f0
			 0x09219222, /// 0x1f4
			 0xaa50c87c, /// 0x1f8
			 0x446c359f, /// 0x1fc
			 0x8505bbe2, /// 0x200
			 0xdb498490, /// 0x204
			 0xa3631cb9, /// 0x208
			 0x6dbd68c4, /// 0x20c
			 0x43fe4111, /// 0x210
			 0x6bd55025, /// 0x214
			 0x041dcede, /// 0x218
			 0x776485f6, /// 0x21c
			 0x66a54a9c, /// 0x220
			 0x33337204, /// 0x224
			 0x625a2d2a, /// 0x228
			 0x713e709b, /// 0x22c
			 0x30435ad4, /// 0x230
			 0x4419176a, /// 0x234
			 0xd0a82f02, /// 0x238
			 0x03c1e0fe, /// 0x23c
			 0x192a7a8c, /// 0x240
			 0x96571209, /// 0x244
			 0xec99e8a0, /// 0x248
			 0x0121edb8, /// 0x24c
			 0x403ea163, /// 0x250
			 0x91b0d0a5, /// 0x254
			 0xfe03fe64, /// 0x258
			 0x4cc73a7d, /// 0x25c
			 0xe3882fcc, /// 0x260
			 0xd86d92be, /// 0x264
			 0xac1d0ce2, /// 0x268
			 0x4cf209f1, /// 0x26c
			 0xeab1f3e5, /// 0x270
			 0xdf61bcab, /// 0x274
			 0x3d45be92, /// 0x278
			 0x208eedef, /// 0x27c
			 0x3d8efbb1, /// 0x280
			 0x700bdbad, /// 0x284
			 0x5f5a1020, /// 0x288
			 0xf52c161c, /// 0x28c
			 0x9e891615, /// 0x290
			 0x72568ddd, /// 0x294
			 0x6cdea49d, /// 0x298
			 0xed379d2a, /// 0x29c
			 0xd4f1ad8a, /// 0x2a0
			 0x09b99ce3, /// 0x2a4
			 0x7a829f32, /// 0x2a8
			 0xe7af9730, /// 0x2ac
			 0xb06503a5, /// 0x2b0
			 0x38942eca, /// 0x2b4
			 0x33c675e5, /// 0x2b8
			 0x875f0c4c, /// 0x2bc
			 0xce18af85, /// 0x2c0
			 0x3b6afcee, /// 0x2c4
			 0x02250df4, /// 0x2c8
			 0x6beedb74, /// 0x2cc
			 0xad4170af, /// 0x2d0
			 0x33bdcbb6, /// 0x2d4
			 0x15680ae0, /// 0x2d8
			 0x63313550, /// 0x2dc
			 0xa22544ef, /// 0x2e0
			 0x49518f8f, /// 0x2e4
			 0x7aba088b, /// 0x2e8
			 0xfa7156ef, /// 0x2ec
			 0x006bf414, /// 0x2f0
			 0xc0db8ef0, /// 0x2f4
			 0xa403e915, /// 0x2f8
			 0x059abe56, /// 0x2fc
			 0xf83599ba, /// 0x300
			 0x8aa9b66d, /// 0x304
			 0x12a8a4c8, /// 0x308
			 0x94b226a7, /// 0x30c
			 0xc606b47e, /// 0x310
			 0x4e7d5196, /// 0x314
			 0xa2203b70, /// 0x318
			 0x54b2046d, /// 0x31c
			 0xb0b5bcdf, /// 0x320
			 0x9e1ce511, /// 0x324
			 0x4c80781e, /// 0x328
			 0x7cd473f5, /// 0x32c
			 0x0b1b00d2, /// 0x330
			 0x6128d750, /// 0x334
			 0x065033e0, /// 0x338
			 0xe32b61e9, /// 0x33c
			 0x8e5d92a5, /// 0x340
			 0xe34eaf06, /// 0x344
			 0x55a6e3c5, /// 0x348
			 0xe48cc64c, /// 0x34c
			 0x86a60db2, /// 0x350
			 0xb1ecadc2, /// 0x354
			 0x868f2c5f, /// 0x358
			 0xc0e248dc, /// 0x35c
			 0xadd9c758, /// 0x360
			 0xf829de4c, /// 0x364
			 0x3c2c4c6d, /// 0x368
			 0xe530231f, /// 0x36c
			 0xe2ccca22, /// 0x370
			 0x68cc5ca4, /// 0x374
			 0x97eed7b7, /// 0x378
			 0x664128ea, /// 0x37c
			 0x8a317249, /// 0x380
			 0xcd572f91, /// 0x384
			 0xa3ffd1cc, /// 0x388
			 0x53012a06, /// 0x38c
			 0x3c72b2df, /// 0x390
			 0x7a3b0a83, /// 0x394
			 0x6725d2d7, /// 0x398
			 0x50c9e31c, /// 0x39c
			 0xe348194e, /// 0x3a0
			 0xf33fb4d2, /// 0x3a4
			 0x583aff5a, /// 0x3a8
			 0x106ca472, /// 0x3ac
			 0x3d5e6935, /// 0x3b0
			 0xf2fe4079, /// 0x3b4
			 0x60b40934, /// 0x3b8
			 0x429d1edd, /// 0x3bc
			 0x721de61a, /// 0x3c0
			 0x61ec4d29, /// 0x3c4
			 0x30be134f, /// 0x3c8
			 0x54c94d94, /// 0x3cc
			 0x1e8f38a7, /// 0x3d0
			 0x0a8c32ec, /// 0x3d4
			 0x30fd62aa, /// 0x3d8
			 0xd7373816, /// 0x3dc
			 0x1026fbd3, /// 0x3e0
			 0x077049c7, /// 0x3e4
			 0xd8bae97f, /// 0x3e8
			 0x244a613d, /// 0x3ec
			 0x7a15c96b, /// 0x3f0
			 0x32ab9698, /// 0x3f4
			 0xdcfc5e29, /// 0x3f8
			 0x01e3ab4d, /// 0x3fc
			 0x866cf8aa, /// 0x400
			 0xcebb344f, /// 0x404
			 0xd2654f9e, /// 0x408
			 0xf0d8120c, /// 0x40c
			 0xbccd5bc8, /// 0x410
			 0x9060f8e4, /// 0x414
			 0xf1c78d61, /// 0x418
			 0x3e9751c8, /// 0x41c
			 0xc960adc1, /// 0x420
			 0xe1113386, /// 0x424
			 0xc6e027cb, /// 0x428
			 0x4f8cbafd, /// 0x42c
			 0x1b00e1a6, /// 0x430
			 0x350371d3, /// 0x434
			 0x7fb90f75, /// 0x438
			 0x35692c75, /// 0x43c
			 0xa9d5f14a, /// 0x440
			 0x8f03bf9e, /// 0x444
			 0xa8c7e3ed, /// 0x448
			 0xf2c24bda, /// 0x44c
			 0xa1a23287, /// 0x450
			 0x4fbdcb0a, /// 0x454
			 0xd80aea89, /// 0x458
			 0x4d7e7c86, /// 0x45c
			 0x80832f61, /// 0x460
			 0x1f4fd9a6, /// 0x464
			 0x19e3f765, /// 0x468
			 0x93463fcd, /// 0x46c
			 0xd9c006ac, /// 0x470
			 0xb5a79337, /// 0x474
			 0x67b1db6f, /// 0x478
			 0x7d31f6a7, /// 0x47c
			 0xf37121e0, /// 0x480
			 0x249a03f1, /// 0x484
			 0x6fe1e739, /// 0x488
			 0x954fad24, /// 0x48c
			 0x6d8cb667, /// 0x490
			 0x843db44f, /// 0x494
			 0x6652e3df, /// 0x498
			 0xc084f91e, /// 0x49c
			 0x7275f646, /// 0x4a0
			 0x2783ab46, /// 0x4a4
			 0xf8f4448b, /// 0x4a8
			 0x08e59457, /// 0x4ac
			 0xcef5db81, /// 0x4b0
			 0x115df079, /// 0x4b4
			 0xcad2e4bd, /// 0x4b8
			 0xe6298116, /// 0x4bc
			 0x8bf07ff3, /// 0x4c0
			 0xbdeaed9b, /// 0x4c4
			 0x80227716, /// 0x4c8
			 0x62c5d913, /// 0x4cc
			 0x0d212158, /// 0x4d0
			 0x316ae665, /// 0x4d4
			 0xd94b76c4, /// 0x4d8
			 0xae8f62b3, /// 0x4dc
			 0xbd565cc6, /// 0x4e0
			 0x5f3deb45, /// 0x4e4
			 0xca917937, /// 0x4e8
			 0xe441d79e, /// 0x4ec
			 0x610918b8, /// 0x4f0
			 0x4bcef2fd, /// 0x4f4
			 0x3b15a53c, /// 0x4f8
			 0x0c2b8115, /// 0x4fc
			 0x0bd70ec2, /// 0x500
			 0xe3d89183, /// 0x504
			 0xb60a4c07, /// 0x508
			 0x23e3ec5c, /// 0x50c
			 0xa466f762, /// 0x510
			 0xeddd6e95, /// 0x514
			 0xb7302a38, /// 0x518
			 0x879af3dc, /// 0x51c
			 0xf8f04510, /// 0x520
			 0xdce7e789, /// 0x524
			 0xb57e4eff, /// 0x528
			 0x7a8a8a36, /// 0x52c
			 0x2b903127, /// 0x530
			 0x8ab69f04, /// 0x534
			 0x6a703e92, /// 0x538
			 0xb5fab39e, /// 0x53c
			 0x0553be38, /// 0x540
			 0xb6f011cf, /// 0x544
			 0xabf13f92, /// 0x548
			 0x59df4001, /// 0x54c
			 0xc8fa5d72, /// 0x550
			 0x2e303e10, /// 0x554
			 0x079227bf, /// 0x558
			 0x12c42f78, /// 0x55c
			 0xc3645346, /// 0x560
			 0x3be28293, /// 0x564
			 0xe6b41eab, /// 0x568
			 0x77235282, /// 0x56c
			 0x5c9b716c, /// 0x570
			 0xee616961, /// 0x574
			 0x6a35b5b8, /// 0x578
			 0xa50e5dc9, /// 0x57c
			 0xe2f6e0f9, /// 0x580
			 0x405576cd, /// 0x584
			 0xc893e833, /// 0x588
			 0xe1d62fc9, /// 0x58c
			 0xda9c8958, /// 0x590
			 0x702e9333, /// 0x594
			 0x057fff67, /// 0x598
			 0xc2c0d060, /// 0x59c
			 0xd2492085, /// 0x5a0
			 0xb09f7d3e, /// 0x5a4
			 0xdedb7436, /// 0x5a8
			 0x83771bfd, /// 0x5ac
			 0x65448f91, /// 0x5b0
			 0x9ed6fb96, /// 0x5b4
			 0x39f244e5, /// 0x5b8
			 0x03d1d6af, /// 0x5bc
			 0xd851d40f, /// 0x5c0
			 0x82140f0e, /// 0x5c4
			 0xeb13316b, /// 0x5c8
			 0x7b949dae, /// 0x5cc
			 0x423daead, /// 0x5d0
			 0x276e608b, /// 0x5d4
			 0x0271c0c5, /// 0x5d8
			 0x05d57e28, /// 0x5dc
			 0xfeca0a08, /// 0x5e0
			 0x3053bc1f, /// 0x5e4
			 0x021d1940, /// 0x5e8
			 0x811b4a77, /// 0x5ec
			 0xf9566188, /// 0x5f0
			 0x107c6242, /// 0x5f4
			 0xbd82ea0c, /// 0x5f8
			 0x55137ff1, /// 0x5fc
			 0xb7f029a0, /// 0x600
			 0x419fcc7d, /// 0x604
			 0xdb2b82c2, /// 0x608
			 0xd3f7aacd, /// 0x60c
			 0x1b6d6f76, /// 0x610
			 0x279e3c61, /// 0x614
			 0x76caa725, /// 0x618
			 0x1e3860dc, /// 0x61c
			 0xf498699f, /// 0x620
			 0x8e352397, /// 0x624
			 0xc4ff5400, /// 0x628
			 0xcc2ed260, /// 0x62c
			 0xe1fa6e9b, /// 0x630
			 0xf06a8a2d, /// 0x634
			 0x238095bc, /// 0x638
			 0x9069c03a, /// 0x63c
			 0xe79b29c1, /// 0x640
			 0xfe69cf54, /// 0x644
			 0x8b092206, /// 0x648
			 0xb5265b91, /// 0x64c
			 0x9a84745e, /// 0x650
			 0x3d8bea7a, /// 0x654
			 0x565d0fbe, /// 0x658
			 0x8aba0208, /// 0x65c
			 0xedddb544, /// 0x660
			 0xe496d65b, /// 0x664
			 0x013657e5, /// 0x668
			 0x983bbb36, /// 0x66c
			 0x00acdf07, /// 0x670
			 0x0742b54e, /// 0x674
			 0xe455b957, /// 0x678
			 0x2019fe4d, /// 0x67c
			 0xec808e2f, /// 0x680
			 0x070dcd82, /// 0x684
			 0x0f534c9c, /// 0x688
			 0xbe3401b8, /// 0x68c
			 0xba35d2d1, /// 0x690
			 0xf5a62799, /// 0x694
			 0xe9e6500b, /// 0x698
			 0x70c2f1bb, /// 0x69c
			 0x692f6d94, /// 0x6a0
			 0x8034110d, /// 0x6a4
			 0xe3469bf4, /// 0x6a8
			 0x84c98e9a, /// 0x6ac
			 0x595142d1, /// 0x6b0
			 0xabc6c34d, /// 0x6b4
			 0x51ddc4d0, /// 0x6b8
			 0x6aa8ad90, /// 0x6bc
			 0xc9daf847, /// 0x6c0
			 0xda0d6902, /// 0x6c4
			 0x25fcb83d, /// 0x6c8
			 0x1ffddcc7, /// 0x6cc
			 0x7cf2a904, /// 0x6d0
			 0xe47388e3, /// 0x6d4
			 0x8a0bb837, /// 0x6d8
			 0x5fd363ba, /// 0x6dc
			 0x2ea15e07, /// 0x6e0
			 0xc8eda12a, /// 0x6e4
			 0x673212b8, /// 0x6e8
			 0x1ce463ec, /// 0x6ec
			 0x020486e0, /// 0x6f0
			 0xd22ea5f1, /// 0x6f4
			 0xdc6ce2c9, /// 0x6f8
			 0x89e2fbe4, /// 0x6fc
			 0xfaf0288c, /// 0x700
			 0x22186429, /// 0x704
			 0x220e4a16, /// 0x708
			 0x4bc84231, /// 0x70c
			 0xc2e2d843, /// 0x710
			 0x72bf2a08, /// 0x714
			 0x15c79d7c, /// 0x718
			 0x95068b3a, /// 0x71c
			 0x2d04cf8d, /// 0x720
			 0x3f377345, /// 0x724
			 0xa8c81309, /// 0x728
			 0x9f9ad751, /// 0x72c
			 0x3dbc83fc, /// 0x730
			 0x6b12c95e, /// 0x734
			 0xb3896e58, /// 0x738
			 0xa0a52813, /// 0x73c
			 0x440a8523, /// 0x740
			 0x3cd2ce90, /// 0x744
			 0xfee1a0d1, /// 0x748
			 0x47fb7b1a, /// 0x74c
			 0xf627de03, /// 0x750
			 0xe73a887b, /// 0x754
			 0x7b583fd2, /// 0x758
			 0x64909861, /// 0x75c
			 0x3120e797, /// 0x760
			 0xce577510, /// 0x764
			 0x24753ac1, /// 0x768
			 0xafe1d915, /// 0x76c
			 0x94fc2785, /// 0x770
			 0x51f2a206, /// 0x774
			 0x26905bce, /// 0x778
			 0x2dba976d, /// 0x77c
			 0xc1bc3eba, /// 0x780
			 0x846176e0, /// 0x784
			 0xcce31d58, /// 0x788
			 0x8554ca06, /// 0x78c
			 0x807470d5, /// 0x790
			 0x5845ac3d, /// 0x794
			 0x788e5aef, /// 0x798
			 0x7ee244f7, /// 0x79c
			 0xe55e0250, /// 0x7a0
			 0x5bd7d921, /// 0x7a4
			 0x7f4748a3, /// 0x7a8
			 0x7261ff97, /// 0x7ac
			 0x72f0431d, /// 0x7b0
			 0xd912069e, /// 0x7b4
			 0x636274ae, /// 0x7b8
			 0xe4ee84c8, /// 0x7bc
			 0x252655ab, /// 0x7c0
			 0x30d8b62c, /// 0x7c4
			 0x34944e68, /// 0x7c8
			 0xef73d407, /// 0x7cc
			 0x281736cc, /// 0x7d0
			 0x15a5752b, /// 0x7d4
			 0x708df630, /// 0x7d8
			 0xb4d5ed58, /// 0x7dc
			 0x836402c2, /// 0x7e0
			 0x9c12cb55, /// 0x7e4
			 0xc486e212, /// 0x7e8
			 0xd181a410, /// 0x7ec
			 0xb84e80e5, /// 0x7f0
			 0x08cf678b, /// 0x7f4
			 0x0dc45e0b, /// 0x7f8
			 0x0a012858, /// 0x7fc
			 0x388572f8, /// 0x800
			 0x4e2326a3, /// 0x804
			 0x9591731c, /// 0x808
			 0x6a094eb2, /// 0x80c
			 0x2826558f, /// 0x810
			 0x36c440f8, /// 0x814
			 0x896ce9f2, /// 0x818
			 0x623d83fb, /// 0x81c
			 0x9f949421, /// 0x820
			 0x64774fa2, /// 0x824
			 0xdafdd4bb, /// 0x828
			 0x5129225b, /// 0x82c
			 0x9dccfe48, /// 0x830
			 0xcfa49e77, /// 0x834
			 0xc2be41cb, /// 0x838
			 0x87740914, /// 0x83c
			 0xcac24f0b, /// 0x840
			 0xab8033b6, /// 0x844
			 0xb92701e2, /// 0x848
			 0x8181daff, /// 0x84c
			 0x0bac3f67, /// 0x850
			 0x94bc01b0, /// 0x854
			 0x6b15f8af, /// 0x858
			 0x68487a65, /// 0x85c
			 0xd70e3c6f, /// 0x860
			 0xba8c6d94, /// 0x864
			 0xee8745cc, /// 0x868
			 0xe00b2dea, /// 0x86c
			 0x863e34d5, /// 0x870
			 0x7a249324, /// 0x874
			 0xbd42de9d, /// 0x878
			 0x0531f59d, /// 0x87c
			 0xdf688d58, /// 0x880
			 0x3b3d46a6, /// 0x884
			 0x88f87edf, /// 0x888
			 0x816765f8, /// 0x88c
			 0x7498cc52, /// 0x890
			 0xbcaa5774, /// 0x894
			 0xf4c6acd7, /// 0x898
			 0xd63d39f4, /// 0x89c
			 0x6fe07bd8, /// 0x8a0
			 0xbe42a265, /// 0x8a4
			 0xbf55230e, /// 0x8a8
			 0x685df6df, /// 0x8ac
			 0xed07cad0, /// 0x8b0
			 0xb334bc03, /// 0x8b4
			 0x2301db2f, /// 0x8b8
			 0x1a76a016, /// 0x8bc
			 0x8db0ea0c, /// 0x8c0
			 0xdb1d092c, /// 0x8c4
			 0x60d21ee8, /// 0x8c8
			 0xeb52e58a, /// 0x8cc
			 0xc88baf53, /// 0x8d0
			 0x54ebab00, /// 0x8d4
			 0x6fdc9be7, /// 0x8d8
			 0x5f02dde8, /// 0x8dc
			 0xc9a2d40b, /// 0x8e0
			 0x48b87105, /// 0x8e4
			 0x823591b2, /// 0x8e8
			 0x781a5c7f, /// 0x8ec
			 0xaf38fd5b, /// 0x8f0
			 0xcdfd6e8d, /// 0x8f4
			 0x7f9c37ad, /// 0x8f8
			 0xe5d996d0, /// 0x8fc
			 0x5e5c438a, /// 0x900
			 0x107c64ce, /// 0x904
			 0x61718c1a, /// 0x908
			 0x8f7bcb11, /// 0x90c
			 0xbf662ea7, /// 0x910
			 0x851c44e0, /// 0x914
			 0x4f2a7e85, /// 0x918
			 0xfc5c255c, /// 0x91c
			 0x1797cbdb, /// 0x920
			 0x0186c11f, /// 0x924
			 0x256b74f3, /// 0x928
			 0xaf4ab91a, /// 0x92c
			 0xfca1db32, /// 0x930
			 0x69282a2d, /// 0x934
			 0xb5f003d9, /// 0x938
			 0xbea029fc, /// 0x93c
			 0x4058ebcf, /// 0x940
			 0x749d9392, /// 0x944
			 0x657a4f14, /// 0x948
			 0x79bcbd86, /// 0x94c
			 0x45c8ad4d, /// 0x950
			 0x9dcb8a0a, /// 0x954
			 0x729962d9, /// 0x958
			 0xf46e1e6d, /// 0x95c
			 0x031325ee, /// 0x960
			 0x87245fc7, /// 0x964
			 0x6714ff43, /// 0x968
			 0x7b98de2b, /// 0x96c
			 0x686e3eee, /// 0x970
			 0xc84159cb, /// 0x974
			 0x15169c21, /// 0x978
			 0x6c043f1e, /// 0x97c
			 0x3ed0f254, /// 0x980
			 0x22b5d89b, /// 0x984
			 0xd48f6112, /// 0x988
			 0x778aeff0, /// 0x98c
			 0xf4f2ebd5, /// 0x990
			 0x76eb1c3d, /// 0x994
			 0xbcfd204b, /// 0x998
			 0xdc570e7a, /// 0x99c
			 0x65e74e7e, /// 0x9a0
			 0x58c941de, /// 0x9a4
			 0x9bd53db3, /// 0x9a8
			 0xf5089f14, /// 0x9ac
			 0x6107ade3, /// 0x9b0
			 0xc511e179, /// 0x9b4
			 0xdee8da7d, /// 0x9b8
			 0xccd65ecb, /// 0x9bc
			 0x9d8c98eb, /// 0x9c0
			 0xefd92709, /// 0x9c4
			 0xc46ea800, /// 0x9c8
			 0x7b60489d, /// 0x9cc
			 0xeee3eb03, /// 0x9d0
			 0x5e3af40b, /// 0x9d4
			 0xd25574ff, /// 0x9d8
			 0xaf2f2ea5, /// 0x9dc
			 0xb4105ef8, /// 0x9e0
			 0x039116a9, /// 0x9e4
			 0x92b5698d, /// 0x9e8
			 0x769efb5a, /// 0x9ec
			 0x4a3cf5ea, /// 0x9f0
			 0xe6dbbb1f, /// 0x9f4
			 0xcbe0b9d5, /// 0x9f8
			 0x72ce5b85, /// 0x9fc
			 0x8bf743d1, /// 0xa00
			 0x10bd7ae5, /// 0xa04
			 0xae479d70, /// 0xa08
			 0x5edbf213, /// 0xa0c
			 0xebe90b0a, /// 0xa10
			 0xe92f58ca, /// 0xa14
			 0x7cf825db, /// 0xa18
			 0x5eb21868, /// 0xa1c
			 0x10fb0aa5, /// 0xa20
			 0x9a37b2d4, /// 0xa24
			 0x9b12afa7, /// 0xa28
			 0x88b0dd96, /// 0xa2c
			 0x78bf36e5, /// 0xa30
			 0x091f80ac, /// 0xa34
			 0xafe7acb9, /// 0xa38
			 0xdc4d2548, /// 0xa3c
			 0x438201d7, /// 0xa40
			 0x94a020e7, /// 0xa44
			 0xfd068b4d, /// 0xa48
			 0x8c49d87c, /// 0xa4c
			 0x0e660b0d, /// 0xa50
			 0x37cb5c1d, /// 0xa54
			 0x5016111d, /// 0xa58
			 0x559e9dc8, /// 0xa5c
			 0x8f705841, /// 0xa60
			 0xab55696f, /// 0xa64
			 0xbb08029c, /// 0xa68
			 0xcef6f22c, /// 0xa6c
			 0xd99d1c10, /// 0xa70
			 0xda925fec, /// 0xa74
			 0xce9da194, /// 0xa78
			 0x43c90b08, /// 0xa7c
			 0x8118655a, /// 0xa80
			 0x6661c787, /// 0xa84
			 0xeaf6ef9f, /// 0xa88
			 0x0947e687, /// 0xa8c
			 0x0c8672fd, /// 0xa90
			 0x4bc1be2e, /// 0xa94
			 0x62b6fceb, /// 0xa98
			 0x5de990de, /// 0xa9c
			 0xe900a7a0, /// 0xaa0
			 0xd98e6d7e, /// 0xaa4
			 0x11b0c062, /// 0xaa8
			 0x88d2bdd6, /// 0xaac
			 0x8245beda, /// 0xab0
			 0x9d67fc4a, /// 0xab4
			 0xf877cc61, /// 0xab8
			 0xd199415c, /// 0xabc
			 0x4dbdc58f, /// 0xac0
			 0xa2603a92, /// 0xac4
			 0x017c979c, /// 0xac8
			 0xafacb780, /// 0xacc
			 0xf96ef275, /// 0xad0
			 0xcff92360, /// 0xad4
			 0xa887962f, /// 0xad8
			 0xe1ef25a0, /// 0xadc
			 0xbbd4a4f1, /// 0xae0
			 0x1d9a7250, /// 0xae4
			 0x5c4dabae, /// 0xae8
			 0x35ee1aba, /// 0xaec
			 0x7a9796a2, /// 0xaf0
			 0x155efc1a, /// 0xaf4
			 0xa366e55c, /// 0xaf8
			 0x1d3a39f9, /// 0xafc
			 0x1592e6a4, /// 0xb00
			 0x6bb78c11, /// 0xb04
			 0x1f1ae708, /// 0xb08
			 0x4a6e32ad, /// 0xb0c
			 0x18ae8e0f, /// 0xb10
			 0x9416701b, /// 0xb14
			 0x3b682129, /// 0xb18
			 0xac41c3fe, /// 0xb1c
			 0xea31bc03, /// 0xb20
			 0x9320e8ec, /// 0xb24
			 0xbc9e5f22, /// 0xb28
			 0x1b06f5fb, /// 0xb2c
			 0x034789c1, /// 0xb30
			 0x442dbee9, /// 0xb34
			 0x094f56de, /// 0xb38
			 0x5bd5094f, /// 0xb3c
			 0x060f73b0, /// 0xb40
			 0x212da858, /// 0xb44
			 0xc5335e73, /// 0xb48
			 0x8e882569, /// 0xb4c
			 0xe9ae3a59, /// 0xb50
			 0xf4119fc9, /// 0xb54
			 0x1c8cbb2c, /// 0xb58
			 0x95231fed, /// 0xb5c
			 0x8c27aeab, /// 0xb60
			 0x867165e5, /// 0xb64
			 0xee41bccf, /// 0xb68
			 0x66f7df32, /// 0xb6c
			 0x11df9dbb, /// 0xb70
			 0x262cbc58, /// 0xb74
			 0xd94497a7, /// 0xb78
			 0xfe8a7c61, /// 0xb7c
			 0xab7dc730, /// 0xb80
			 0xbe66d673, /// 0xb84
			 0x3d90829f, /// 0xb88
			 0x63a1a65e, /// 0xb8c
			 0x366c6bfe, /// 0xb90
			 0x92c42c54, /// 0xb94
			 0x69769c42, /// 0xb98
			 0x0d2e0ed7, /// 0xb9c
			 0xc05a0f1d, /// 0xba0
			 0xbe56a744, /// 0xba4
			 0x8b0be4ba, /// 0xba8
			 0x0e3c3bcc, /// 0xbac
			 0xe2abc930, /// 0xbb0
			 0x65bcb562, /// 0xbb4
			 0x87901428, /// 0xbb8
			 0x4001d008, /// 0xbbc
			 0x9c9a4f98, /// 0xbc0
			 0x4f6fed5d, /// 0xbc4
			 0xc8de2bad, /// 0xbc8
			 0x7cf72494, /// 0xbcc
			 0x61e56a46, /// 0xbd0
			 0x64125adf, /// 0xbd4
			 0x00e56af4, /// 0xbd8
			 0x16382b06, /// 0xbdc
			 0xc5d6e01e, /// 0xbe0
			 0xb62d1814, /// 0xbe4
			 0x86f0a4ea, /// 0xbe8
			 0xf1e80a23, /// 0xbec
			 0x121fbc98, /// 0xbf0
			 0xe60b0fec, /// 0xbf4
			 0xda807b06, /// 0xbf8
			 0xa7b08356, /// 0xbfc
			 0x5bcccb01, /// 0xc00
			 0x5befe4dd, /// 0xc04
			 0x7ea64456, /// 0xc08
			 0x99d64046, /// 0xc0c
			 0xd1cf8b1c, /// 0xc10
			 0xe19a14e2, /// 0xc14
			 0x3f5da65e, /// 0xc18
			 0x62fc38c4, /// 0xc1c
			 0x558bc5d7, /// 0xc20
			 0xbb7a6280, /// 0xc24
			 0x6a1d515e, /// 0xc28
			 0x34115a28, /// 0xc2c
			 0xc603867d, /// 0xc30
			 0xdf6c7686, /// 0xc34
			 0xe8088322, /// 0xc38
			 0xaffeebfc, /// 0xc3c
			 0xcb894651, /// 0xc40
			 0x7bec8246, /// 0xc44
			 0xfa60ff36, /// 0xc48
			 0xe8ea3917, /// 0xc4c
			 0x324e0017, /// 0xc50
			 0xee09cd82, /// 0xc54
			 0xd5a23fff, /// 0xc58
			 0xab2194fb, /// 0xc5c
			 0x31a5b592, /// 0xc60
			 0x2ef44073, /// 0xc64
			 0xbaf68f55, /// 0xc68
			 0xaf2ec3b1, /// 0xc6c
			 0x7acd110d, /// 0xc70
			 0xae996e58, /// 0xc74
			 0x2b9399fb, /// 0xc78
			 0x5fd89457, /// 0xc7c
			 0xf9b45f2b, /// 0xc80
			 0x8e8ddcee, /// 0xc84
			 0x6d31818e, /// 0xc88
			 0x0521ac40, /// 0xc8c
			 0xccd330ef, /// 0xc90
			 0x258de978, /// 0xc94
			 0x678ba969, /// 0xc98
			 0x471bf66a, /// 0xc9c
			 0x2cb6d06c, /// 0xca0
			 0xdc67f596, /// 0xca4
			 0x62ec6d99, /// 0xca8
			 0xa4625128, /// 0xcac
			 0xb574c9e9, /// 0xcb0
			 0xcd55230c, /// 0xcb4
			 0x4174b60a, /// 0xcb8
			 0x7f94ecd8, /// 0xcbc
			 0xea84f29f, /// 0xcc0
			 0x0a46a199, /// 0xcc4
			 0x6443b102, /// 0xcc8
			 0xa2ecca32, /// 0xccc
			 0xb424f128, /// 0xcd0
			 0x46c1035d, /// 0xcd4
			 0x6636f93e, /// 0xcd8
			 0x5d9d647a, /// 0xcdc
			 0x97dcbd8a, /// 0xce0
			 0x5bdaafa8, /// 0xce4
			 0x3df09328, /// 0xce8
			 0xeb6cdf86, /// 0xcec
			 0x35961529, /// 0xcf0
			 0x52f45135, /// 0xcf4
			 0x3969f396, /// 0xcf8
			 0x059d233c, /// 0xcfc
			 0xc0f4ec6c, /// 0xd00
			 0x97b87dfb, /// 0xd04
			 0x47f421f2, /// 0xd08
			 0xce56c54c, /// 0xd0c
			 0xadacaa96, /// 0xd10
			 0x15e2b80a, /// 0xd14
			 0xdadd418a, /// 0xd18
			 0x59b3483b, /// 0xd1c
			 0xc838f9a7, /// 0xd20
			 0x3fd1eb38, /// 0xd24
			 0x084d6990, /// 0xd28
			 0xde17a34e, /// 0xd2c
			 0xd03014d8, /// 0xd30
			 0xd47007f4, /// 0xd34
			 0x1be3165e, /// 0xd38
			 0xe30f7455, /// 0xd3c
			 0xb07ef3f7, /// 0xd40
			 0xda2f4bbc, /// 0xd44
			 0xb7525224, /// 0xd48
			 0xc002c4da, /// 0xd4c
			 0x15e9690e, /// 0xd50
			 0x90a9a1c3, /// 0xd54
			 0xd5c87964, /// 0xd58
			 0xf057a42b, /// 0xd5c
			 0x035242c1, /// 0xd60
			 0x670a206f, /// 0xd64
			 0xc99fd5b2, /// 0xd68
			 0xdd75249b, /// 0xd6c
			 0x9f56a7e8, /// 0xd70
			 0xacdd29fc, /// 0xd74
			 0xa2279ca0, /// 0xd78
			 0x417ba400, /// 0xd7c
			 0xdca3f0ae, /// 0xd80
			 0x7e62ecf6, /// 0xd84
			 0xa2549b01, /// 0xd88
			 0x6d64d01c, /// 0xd8c
			 0xd67b83ff, /// 0xd90
			 0xa5b4c39f, /// 0xd94
			 0x4f99570f, /// 0xd98
			 0xb3f06900, /// 0xd9c
			 0xaf98c3c3, /// 0xda0
			 0x31ca91e6, /// 0xda4
			 0x34e94b0b, /// 0xda8
			 0x3e56e3e3, /// 0xdac
			 0x0d871697, /// 0xdb0
			 0x8b19fb65, /// 0xdb4
			 0x8c532ee4, /// 0xdb8
			 0xca9349ec, /// 0xdbc
			 0xe82538f7, /// 0xdc0
			 0xf3ab0eb8, /// 0xdc4
			 0x2124058c, /// 0xdc8
			 0xf850d114, /// 0xdcc
			 0x21cc26f0, /// 0xdd0
			 0x10f149eb, /// 0xdd4
			 0x3f5ae1d3, /// 0xdd8
			 0xcebf5dc0, /// 0xddc
			 0x540ea5b0, /// 0xde0
			 0x95ae2dd4, /// 0xde4
			 0x088813c8, /// 0xde8
			 0xf94bb8af, /// 0xdec
			 0xd9dc6520, /// 0xdf0
			 0xf02a1b0e, /// 0xdf4
			 0x214687c3, /// 0xdf8
			 0x8b2d8135, /// 0xdfc
			 0x44909836, /// 0xe00
			 0xef722242, /// 0xe04
			 0x0be70c15, /// 0xe08
			 0x4cd1a4e6, /// 0xe0c
			 0x120142d8, /// 0xe10
			 0x934bdb61, /// 0xe14
			 0xaa888931, /// 0xe18
			 0x6a9f63df, /// 0xe1c
			 0x4146e2e2, /// 0xe20
			 0x1ecfed68, /// 0xe24
			 0x9d9cd04e, /// 0xe28
			 0x3b82aa44, /// 0xe2c
			 0xe857b058, /// 0xe30
			 0x71552328, /// 0xe34
			 0xc5f7241d, /// 0xe38
			 0x722904ef, /// 0xe3c
			 0xb1476e16, /// 0xe40
			 0xa6f8053e, /// 0xe44
			 0x61acc89f, /// 0xe48
			 0x1c474e61, /// 0xe4c
			 0x980dfb70, /// 0xe50
			 0x875e39ba, /// 0xe54
			 0xaa257ea2, /// 0xe58
			 0x78f46f32, /// 0xe5c
			 0x21b3ed9b, /// 0xe60
			 0x48f026bd, /// 0xe64
			 0xa31abfd1, /// 0xe68
			 0xa76cec6c, /// 0xe6c
			 0xa06804fc, /// 0xe70
			 0xe9b199ae, /// 0xe74
			 0x74d94889, /// 0xe78
			 0x3eb50271, /// 0xe7c
			 0xa022d31f, /// 0xe80
			 0xa2bdcebc, /// 0xe84
			 0x28b162d1, /// 0xe88
			 0xf73df4ea, /// 0xe8c
			 0xcaf620ae, /// 0xe90
			 0x63ba04f6, /// 0xe94
			 0xb263e650, /// 0xe98
			 0x11f01a57, /// 0xe9c
			 0x095814f6, /// 0xea0
			 0x1c7bb7bd, /// 0xea4
			 0xbb1140ce, /// 0xea8
			 0x94b338d5, /// 0xeac
			 0xd856c3f2, /// 0xeb0
			 0xc6caab67, /// 0xeb4
			 0x14de4920, /// 0xeb8
			 0x7df76bb4, /// 0xebc
			 0x69eb8b51, /// 0xec0
			 0x1273b2bc, /// 0xec4
			 0xc269e806, /// 0xec8
			 0x621157bd, /// 0xecc
			 0xd898b57c, /// 0xed0
			 0xb21ec470, /// 0xed4
			 0x360cda94, /// 0xed8
			 0xc8506fd6, /// 0xedc
			 0x32f93091, /// 0xee0
			 0x5d387a98, /// 0xee4
			 0x16ea8d64, /// 0xee8
			 0xcd7df8de, /// 0xeec
			 0x0017f943, /// 0xef0
			 0x78253c8c, /// 0xef4
			 0x29e65677, /// 0xef8
			 0xc1f5ed3a, /// 0xefc
			 0x2f28a79b, /// 0xf00
			 0x8d1313b1, /// 0xf04
			 0x4ee0ea15, /// 0xf08
			 0xb18000f9, /// 0xf0c
			 0x6febcfa6, /// 0xf10
			 0x24d1e521, /// 0xf14
			 0x3d4449a7, /// 0xf18
			 0x732ec44e, /// 0xf1c
			 0x39a8a2f7, /// 0xf20
			 0x5b2f5eab, /// 0xf24
			 0x1f957ad2, /// 0xf28
			 0x05cd09f2, /// 0xf2c
			 0x25c3aa12, /// 0xf30
			 0xa427189e, /// 0xf34
			 0x8b0e48e8, /// 0xf38
			 0xcf8cc852, /// 0xf3c
			 0x3f734b54, /// 0xf40
			 0x7fa284ec, /// 0xf44
			 0x81857d59, /// 0xf48
			 0x5df07689, /// 0xf4c
			 0xa35b1806, /// 0xf50
			 0x90b56016, /// 0xf54
			 0xe5d60bdb, /// 0xf58
			 0x345fcfd0, /// 0xf5c
			 0xb90a1a28, /// 0xf60
			 0xd5125b95, /// 0xf64
			 0x5f4a94e7, /// 0xf68
			 0xe566cb81, /// 0xf6c
			 0x1a76829d, /// 0xf70
			 0xd3400fe0, /// 0xf74
			 0x310a5fc2, /// 0xf78
			 0x993e9561, /// 0xf7c
			 0x6ce3a92a, /// 0xf80
			 0xced049fe, /// 0xf84
			 0xfaa0df4e, /// 0xf88
			 0x07634705, /// 0xf8c
			 0x2290759d, /// 0xf90
			 0xb8aafbde, /// 0xf94
			 0x931d7ecd, /// 0xf98
			 0x83261705, /// 0xf9c
			 0x8775fd32, /// 0xfa0
			 0x9f1a9928, /// 0xfa4
			 0x898030fa, /// 0xfa8
			 0x9200d9f8, /// 0xfac
			 0x1049eb82, /// 0xfb0
			 0x9e84daec, /// 0xfb4
			 0x71459b37, /// 0xfb8
			 0x62dd750d, /// 0xfbc
			 0xce3404ee, /// 0xfc0
			 0xcd9b4bda, /// 0xfc4
			 0x1cc8dfae, /// 0xfc8
			 0xed2a12b9, /// 0xfcc
			 0x92b42ba3, /// 0xfd0
			 0x93bedbcb, /// 0xfd4
			 0xeb767f3a, /// 0xfd8
			 0x3fbc8e88, /// 0xfdc
			 0xf8e58091, /// 0xfe0
			 0xf2ad6733, /// 0xfe4
			 0x8d88fe79, /// 0xfe8
			 0x08844b33, /// 0xfec
			 0x7b46b0f7, /// 0xff0
			 0x00de24d0, /// 0xff4
			 0x0b7466a7, /// 0xff8
			 0xcadcc36a /// last
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
			 0x000003bc,
			 0x00000680,
			 0x00000650,
			 0x0000054c,
			 0x00000480,
			 0x000006fc,
			 0x00000734,
			 0x000006b8,

			 /// vpu register f2
			 0x41700000,
			 0x41600000,
			 0x41000000,
			 0x41c00000,
			 0x41a80000,
			 0x41000000,
			 0x41300000,
			 0x41500000,

			 /// vpu register f3
			 0x41980000,
			 0x41700000,
			 0x41f80000,
			 0x41300000,
			 0x41e00000,
			 0x41c00000,
			 0x41c80000,
			 0x41e80000,

			 /// vpu register f4
			 0x41d00000,
			 0x41980000,
			 0x41400000,
			 0x41b00000,
			 0x41d80000,
			 0x41200000,
			 0x41b80000,
			 0x41d80000,

			 /// vpu register f5
			 0x41d00000,
			 0x40000000,
			 0x41f00000,
			 0x41e00000,
			 0x41f00000,
			 0x41880000,
			 0x41400000,
			 0x41300000,

			 /// vpu register f6
			 0x41f80000,
			 0x41a00000,
			 0x40a00000,
			 0x41b80000,
			 0x40e00000,
			 0x41400000,
			 0x3f800000,
			 0x41100000,

			 /// vpu register f7
			 0x40400000,
			 0x41900000,
			 0x41500000,
			 0x41e00000,
			 0x41c80000,
			 0x41d80000,
			 0x41400000,
			 0x42000000,

			 /// vpu register f8
			 0x41c80000,
			 0x41800000,
			 0x41200000,
			 0x41a80000,
			 0x40800000,
			 0x41600000,
			 0x41c00000,
			 0x41900000,

			 /// vpu register f9
			 0x40800000,
			 0x40000000,
			 0x41500000,
			 0x41d80000,
			 0x41880000,
			 0x41200000,
			 0x40800000,
			 0x41a00000,

			 /// vpu register f10
			 0x40a00000,
			 0x41900000,
			 0x41300000,
			 0x41600000,
			 0x40800000,
			 0x41d00000,
			 0x41400000,
			 0x40800000,

			 /// vpu register f11
			 0x40e00000,
			 0x41200000,
			 0x41100000,
			 0x41c00000,
			 0x41d00000,
			 0x40800000,
			 0x41a80000,
			 0x41000000,

			 /// vpu register f12
			 0x41300000,
			 0x41c00000,
			 0x41300000,
			 0x42000000,
			 0x41600000,
			 0x40e00000,
			 0x41f00000,
			 0x41100000,

			 /// vpu register f13
			 0x41700000,
			 0x41b00000,
			 0x41600000,
			 0x41100000,
			 0x41d80000,
			 0x41800000,
			 0x40a00000,
			 0x42000000,

			 /// vpu register f14
			 0x41980000,
			 0x40000000,
			 0x41d00000,
			 0x41400000,
			 0x3f800000,
			 0x41200000,
			 0x41200000,
			 0x40800000,

			 /// vpu register f15
			 0x41200000,
			 0x41500000,
			 0x40800000,
			 0x40a00000,
			 0x41d00000,
			 0x41400000,
			 0x41c00000,
			 0x40e00000,

			 /// vpu register f16
			 0x41600000,
			 0x41900000,
			 0x41a00000,
			 0x41200000,
			 0x41d80000,
			 0x40e00000,
			 0x40c00000,
			 0x40a00000,

			 /// vpu register f17
			 0x42000000,
			 0x41880000,
			 0x41400000,
			 0x41700000,
			 0x40000000,
			 0x40800000,
			 0x41b80000,
			 0x3f800000,

			 /// vpu register f18
			 0x41880000,
			 0x41600000,
			 0x41f80000,
			 0x41b80000,
			 0x40800000,
			 0x40000000,
			 0x41d80000,
			 0x40e00000,

			 /// vpu register f19
			 0x40400000,
			 0x41b00000,
			 0x41800000,
			 0x41c80000,
			 0x41b00000,
			 0x41600000,
			 0x41400000,
			 0x40a00000,

			 /// vpu register f20
			 0x41800000,
			 0x41c80000,
			 0x40e00000,
			 0x41200000,
			 0x41a00000,
			 0x41e00000,
			 0x41880000,
			 0x41900000,

			 /// vpu register f21
			 0x41a80000,
			 0x40c00000,
			 0x41880000,
			 0x41980000,
			 0x41f80000,
			 0x40400000,
			 0x41800000,
			 0x41900000,

			 /// vpu register f22
			 0x41980000,
			 0x40c00000,
			 0x41400000,
			 0x40800000,
			 0x41d00000,
			 0x41e00000,
			 0x41500000,
			 0x41a00000,

			 /// vpu register f23
			 0x41a80000,
			 0x41e00000,
			 0x41b00000,
			 0x40000000,
			 0x41900000,
			 0x41b80000,
			 0x40000000,
			 0x41a80000,

			 /// vpu register f24
			 0x41700000,
			 0x41e00000,
			 0x41c00000,
			 0x41100000,
			 0x41b80000,
			 0x41a00000,
			 0x41700000,
			 0x41b00000,

			 /// vpu register f25
			 0x41c00000,
			 0x41d00000,
			 0x41980000,
			 0x41880000,
			 0x40000000,
			 0x41f00000,
			 0x41500000,
			 0x41f80000,

			 /// vpu register f26
			 0x41200000,
			 0x41d00000,
			 0x41300000,
			 0x41e80000,
			 0x41700000,
			 0x41f80000,
			 0x41300000,
			 0x41e80000,

			 /// vpu register f27
			 0x41e00000,
			 0x40000000,
			 0x41980000,
			 0x41d80000,
			 0x42000000,
			 0x40e00000,
			 0x41880000,
			 0x41300000,

			 /// vpu register f28
			 0x40e00000,
			 0x40a00000,
			 0x41700000,
			 0x42000000,
			 0x41a80000,
			 0x41c80000,
			 0x41880000,
			 0x41c80000,

			 /// vpu register f29
			 0x40c00000,
			 0x3f800000,
			 0x41900000,
			 0x40800000,
			 0x41400000,
			 0x41a00000,
			 0x40800000,
			 0x41800000,

			 /// vpu register f30
			 0x41700000,
			 0x41500000,
			 0x41300000,
			 0x41e00000,
			 0x41d80000,
			 0x41c80000,
			 0x41980000,
			 0x41c80000,

			 /// vpu register f31
			 0x41300000,
			 0x41c00000,
			 0x40800000,
			 0x41b80000,
			 0x41d80000,
			 0x41a80000,
			 0x41300000,
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
		"fcvt.ps.sn8 f10, f7\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f20, f29\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f25, f18\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f30, f24\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f6, f23\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f8, f26\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f21, f1\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f16, f11\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f8, f8\n"                                ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f16, f9\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f15, f30\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f29, f8\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f14, f23\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f30, f0\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f30, f4\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f27, f11\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f21, f21\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f24, f2\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f20, f9\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f15, f4\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f20, f3\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f15, f16\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f2, f25\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f27, f13\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f19, f14\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f19, f5\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f12, f9\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f4, f24\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f24, f0\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f29, f7\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f27, f0\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f5, f14\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f5, f28\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f20, f6\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f18, f13\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f20, f10\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f25, f0\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f25, f10\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f6, f16\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f13, f9\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f6, f22\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f2, f6\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f8, f25\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f4, f18\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f9, f20\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f14, f4\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f4, f16\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f26, f5\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f11, f28\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f18, f12\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f23, f19\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f28, f23\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f12, f5\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f11, f20\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f9, f15\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f29, f29\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f15, f24\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f28, f14\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f4, f13\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f13, f18\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f11, f5\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f19, f5\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f26, f25\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f28, f5\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f6, f2\n"                                ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f4, f19\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f22, f21\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f28, f19\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f25, f14\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f7, f21\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f2, f14\n"                               ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f29, f22\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f28, f5\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f16, f8\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f25, f22\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f7, f5\n"                                ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f19, f4\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f3, f28\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f26, f8\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f7, f3\n"                                ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f17, f16\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f17, f10\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f20, f8\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f22, f30\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f3, f25\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f26, f10\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f10, f7\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f10, f9\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f27, f5\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f13, f7\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f22, f22\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f16, f0\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f16, f7\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f18, f24\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f24, f16\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f12, f17\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f15, f1\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f11, f19\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.sn8 f21, f5\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
