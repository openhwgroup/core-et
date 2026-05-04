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
			 0x4d9f36c9, /// 0x0
			 0x95d41e9d, /// 0x4
			 0x9aa0f484, /// 0x8
			 0x9602e083, /// 0xc
			 0xe9752714, /// 0x10
			 0x038dcb74, /// 0x14
			 0x969ca03d, /// 0x18
			 0x82ac536d, /// 0x1c
			 0xa5e3ab7a, /// 0x20
			 0x6a6d2744, /// 0x24
			 0x2b7da3b8, /// 0x28
			 0x6d281226, /// 0x2c
			 0x45e666f5, /// 0x30
			 0x9aa83f06, /// 0x34
			 0x5b112e5b, /// 0x38
			 0x94c8e711, /// 0x3c
			 0x5ee06aa1, /// 0x40
			 0x45cde7c8, /// 0x44
			 0x739aed81, /// 0x48
			 0x549c0463, /// 0x4c
			 0x5f5e3ec4, /// 0x50
			 0x8281c078, /// 0x54
			 0x0deb1e1a, /// 0x58
			 0xe385365d, /// 0x5c
			 0xbbae0639, /// 0x60
			 0x412b0703, /// 0x64
			 0x8a49aca1, /// 0x68
			 0x2ae128b6, /// 0x6c
			 0xa23d521d, /// 0x70
			 0xeef090c9, /// 0x74
			 0x71dfeb17, /// 0x78
			 0xa76d324e, /// 0x7c
			 0x5744ee36, /// 0x80
			 0x5e1dc3a9, /// 0x84
			 0x217e92ba, /// 0x88
			 0xf8d70f00, /// 0x8c
			 0x1372b095, /// 0x90
			 0x1876a2c6, /// 0x94
			 0x36600222, /// 0x98
			 0x106b0e28, /// 0x9c
			 0xaaeaecca, /// 0xa0
			 0xb7bb5e06, /// 0xa4
			 0x393cdd35, /// 0xa8
			 0x3e0c3bd5, /// 0xac
			 0x3afff6c1, /// 0xb0
			 0x4f54e5da, /// 0xb4
			 0x4c03c2dd, /// 0xb8
			 0x7aefbd13, /// 0xbc
			 0xeec48f4c, /// 0xc0
			 0xce586381, /// 0xc4
			 0x9163b85c, /// 0xc8
			 0xc9b58fa3, /// 0xcc
			 0x909abadf, /// 0xd0
			 0x2410cdbc, /// 0xd4
			 0xe96ed03b, /// 0xd8
			 0x32b4566a, /// 0xdc
			 0x33103b38, /// 0xe0
			 0xc3b100fc, /// 0xe4
			 0xb2ffc530, /// 0xe8
			 0x0846fcf5, /// 0xec
			 0x6177fe5b, /// 0xf0
			 0xa5716113, /// 0xf4
			 0x4709d873, /// 0xf8
			 0x5fdf143a, /// 0xfc
			 0xd45a6f42, /// 0x100
			 0xd31d78ac, /// 0x104
			 0x0b958f3a, /// 0x108
			 0x94b93a29, /// 0x10c
			 0xe5851ed7, /// 0x110
			 0x5f10983f, /// 0x114
			 0x544f950b, /// 0x118
			 0xde493e4c, /// 0x11c
			 0xe1ce1c51, /// 0x120
			 0xdc715c0f, /// 0x124
			 0x25d7d772, /// 0x128
			 0x8268c385, /// 0x12c
			 0x57d56ccb, /// 0x130
			 0x83db2abd, /// 0x134
			 0x33eb18be, /// 0x138
			 0x738575cc, /// 0x13c
			 0xbbc9301f, /// 0x140
			 0xfd719d65, /// 0x144
			 0x5a5aebab, /// 0x148
			 0xbb0fe4a7, /// 0x14c
			 0x21c3f5a7, /// 0x150
			 0xcee96170, /// 0x154
			 0x9d4b0735, /// 0x158
			 0x193d572e, /// 0x15c
			 0xb8ce1bb1, /// 0x160
			 0x23d8f971, /// 0x164
			 0x24d9f953, /// 0x168
			 0x5ba566ee, /// 0x16c
			 0x24c0fe11, /// 0x170
			 0x3e346f80, /// 0x174
			 0xcb6edee8, /// 0x178
			 0x21fda887, /// 0x17c
			 0x870f0a7c, /// 0x180
			 0x2b7870ec, /// 0x184
			 0xd4887d51, /// 0x188
			 0x938f8c60, /// 0x18c
			 0xb29d5be9, /// 0x190
			 0x319ee76c, /// 0x194
			 0x87485c05, /// 0x198
			 0x924db7f7, /// 0x19c
			 0x6a7e0c60, /// 0x1a0
			 0x22cc90ff, /// 0x1a4
			 0xfb12c419, /// 0x1a8
			 0xecc83406, /// 0x1ac
			 0x71e04eb8, /// 0x1b0
			 0x1abde069, /// 0x1b4
			 0x57be7afd, /// 0x1b8
			 0xb7caa463, /// 0x1bc
			 0x0970689e, /// 0x1c0
			 0xc4c71da6, /// 0x1c4
			 0xffd3e9fd, /// 0x1c8
			 0x2a3df731, /// 0x1cc
			 0x62ff87b6, /// 0x1d0
			 0xd0040e37, /// 0x1d4
			 0x490c2e97, /// 0x1d8
			 0xd0d07e3c, /// 0x1dc
			 0x057aa5ad, /// 0x1e0
			 0x94a53132, /// 0x1e4
			 0xd65ac62f, /// 0x1e8
			 0x813d8b39, /// 0x1ec
			 0x8bcfb7fa, /// 0x1f0
			 0x34360fe0, /// 0x1f4
			 0xeb8c1a9c, /// 0x1f8
			 0x90fed254, /// 0x1fc
			 0xdd1a111b, /// 0x200
			 0x5ae2cd8f, /// 0x204
			 0x347885d9, /// 0x208
			 0xc93f46db, /// 0x20c
			 0xffe467a2, /// 0x210
			 0x82a9aa97, /// 0x214
			 0xdf77e482, /// 0x218
			 0x8e2ecbaa, /// 0x21c
			 0x637a3aad, /// 0x220
			 0x7a3b9b90, /// 0x224
			 0x57a832f8, /// 0x228
			 0x4c5fbccb, /// 0x22c
			 0x9992a626, /// 0x230
			 0xb8632036, /// 0x234
			 0xfcc13bc8, /// 0x238
			 0xc8785589, /// 0x23c
			 0x038423d5, /// 0x240
			 0x3eb2a71d, /// 0x244
			 0x5999b6bd, /// 0x248
			 0x468a4f5d, /// 0x24c
			 0x05558b3c, /// 0x250
			 0x1b18d181, /// 0x254
			 0x669fe139, /// 0x258
			 0x614228f6, /// 0x25c
			 0x1b56bb30, /// 0x260
			 0xde3af1bf, /// 0x264
			 0x3da8e136, /// 0x268
			 0x4b7e0d00, /// 0x26c
			 0x656a88d1, /// 0x270
			 0x24bcf790, /// 0x274
			 0xa982d9b5, /// 0x278
			 0x24131755, /// 0x27c
			 0xc153756a, /// 0x280
			 0xb3b48e07, /// 0x284
			 0x1b668170, /// 0x288
			 0xe328919b, /// 0x28c
			 0x23aa739d, /// 0x290
			 0x41f82045, /// 0x294
			 0x74f2d698, /// 0x298
			 0x02ee9f18, /// 0x29c
			 0x40f2ae82, /// 0x2a0
			 0x4edc7e13, /// 0x2a4
			 0xe2056b2c, /// 0x2a8
			 0xa05035e5, /// 0x2ac
			 0x371c2b01, /// 0x2b0
			 0x43e6e9a3, /// 0x2b4
			 0x22906eb1, /// 0x2b8
			 0xca5b7afc, /// 0x2bc
			 0x18c21808, /// 0x2c0
			 0xd9a2eabe, /// 0x2c4
			 0x69133368, /// 0x2c8
			 0x5514902b, /// 0x2cc
			 0x0d9f0b41, /// 0x2d0
			 0x3f6c5a1a, /// 0x2d4
			 0x282ccdb2, /// 0x2d8
			 0x1a742e0c, /// 0x2dc
			 0x5572b11e, /// 0x2e0
			 0x30b55a9d, /// 0x2e4
			 0xd8a7813c, /// 0x2e8
			 0xcb6e3708, /// 0x2ec
			 0x57537206, /// 0x2f0
			 0xba01d676, /// 0x2f4
			 0x1b819eac, /// 0x2f8
			 0x6d4d0299, /// 0x2fc
			 0x086c11fd, /// 0x300
			 0xed51b83f, /// 0x304
			 0x6afbc44a, /// 0x308
			 0xb29cafd5, /// 0x30c
			 0xca533591, /// 0x310
			 0x34e4cbcb, /// 0x314
			 0x6f197917, /// 0x318
			 0x1afb81e8, /// 0x31c
			 0xf4dbc5ba, /// 0x320
			 0xad002da9, /// 0x324
			 0x68fb8231, /// 0x328
			 0x5ab87f6c, /// 0x32c
			 0x7e24ff37, /// 0x330
			 0xcbfd486c, /// 0x334
			 0xefc3066f, /// 0x338
			 0x92b4b6fa, /// 0x33c
			 0xb1b8dda0, /// 0x340
			 0xe399897c, /// 0x344
			 0x2cc16a26, /// 0x348
			 0x84df8467, /// 0x34c
			 0xe871ebc4, /// 0x350
			 0x34d988f3, /// 0x354
			 0x4cd46784, /// 0x358
			 0x23a2e6c2, /// 0x35c
			 0x34e4edf6, /// 0x360
			 0x1b62f50d, /// 0x364
			 0xfe70781c, /// 0x368
			 0x2f9de1c9, /// 0x36c
			 0x7241ba6f, /// 0x370
			 0xa73ed4c0, /// 0x374
			 0x28db96d3, /// 0x378
			 0x592ce655, /// 0x37c
			 0x89f99508, /// 0x380
			 0xbdbe3374, /// 0x384
			 0xe5f9d650, /// 0x388
			 0xf4adef97, /// 0x38c
			 0x47c20326, /// 0x390
			 0x38f579e7, /// 0x394
			 0x31d3ef39, /// 0x398
			 0x40535dfb, /// 0x39c
			 0xfe0de0dc, /// 0x3a0
			 0xa69b6cea, /// 0x3a4
			 0x4ea3ab61, /// 0x3a8
			 0x82d79e47, /// 0x3ac
			 0xdbf3a9cc, /// 0x3b0
			 0xc987fcc3, /// 0x3b4
			 0xb976b940, /// 0x3b8
			 0x36312188, /// 0x3bc
			 0xb2d233ce, /// 0x3c0
			 0x206d7743, /// 0x3c4
			 0xfc715cd9, /// 0x3c8
			 0x7c9b7063, /// 0x3cc
			 0x28fddf54, /// 0x3d0
			 0x64ec7336, /// 0x3d4
			 0xe803f992, /// 0x3d8
			 0xe51c063b, /// 0x3dc
			 0xa05819f9, /// 0x3e0
			 0xad80dd7d, /// 0x3e4
			 0x86ab4e89, /// 0x3e8
			 0xc2400737, /// 0x3ec
			 0x7fb3f00c, /// 0x3f0
			 0xc7b2cfe3, /// 0x3f4
			 0xfe31c128, /// 0x3f8
			 0x77c8fdb1, /// 0x3fc
			 0x338b24c4, /// 0x400
			 0xa748f9a4, /// 0x404
			 0xb58fe5a9, /// 0x408
			 0xc3f895b9, /// 0x40c
			 0x162c1225, /// 0x410
			 0x1a74feb4, /// 0x414
			 0xd5f458b9, /// 0x418
			 0xe6622922, /// 0x41c
			 0x9e805670, /// 0x420
			 0x7ed83d4c, /// 0x424
			 0x079dd2be, /// 0x428
			 0xa937022e, /// 0x42c
			 0xfd45e7af, /// 0x430
			 0x0717df5d, /// 0x434
			 0xfa12cb0e, /// 0x438
			 0xf9869a72, /// 0x43c
			 0x78e8f5ae, /// 0x440
			 0x3d126f3a, /// 0x444
			 0x3d9ba81e, /// 0x448
			 0xf16698ab, /// 0x44c
			 0x31783d44, /// 0x450
			 0x05926c6d, /// 0x454
			 0x979f5382, /// 0x458
			 0xefa978b6, /// 0x45c
			 0x5b683f98, /// 0x460
			 0xbb4134c8, /// 0x464
			 0x1ad5625d, /// 0x468
			 0x1b7aa4b9, /// 0x46c
			 0x5cbaf4b6, /// 0x470
			 0x002703bf, /// 0x474
			 0xe5ad63d2, /// 0x478
			 0xbf24483f, /// 0x47c
			 0xac2b4679, /// 0x480
			 0x094be04e, /// 0x484
			 0xd2ff83df, /// 0x488
			 0x80e63cc4, /// 0x48c
			 0xd6370e3d, /// 0x490
			 0x51e05f9f, /// 0x494
			 0xac5ecb40, /// 0x498
			 0x11453992, /// 0x49c
			 0xaa9c4710, /// 0x4a0
			 0x1836cdcc, /// 0x4a4
			 0x118598c1, /// 0x4a8
			 0x6eb8553c, /// 0x4ac
			 0xb4aeff9d, /// 0x4b0
			 0x8d26bfc9, /// 0x4b4
			 0x06f85e7a, /// 0x4b8
			 0xc9dec337, /// 0x4bc
			 0x9bd7de7e, /// 0x4c0
			 0x6c176cfa, /// 0x4c4
			 0x6e8b7c49, /// 0x4c8
			 0x4a86b5dd, /// 0x4cc
			 0x6a17026c, /// 0x4d0
			 0xb36469d8, /// 0x4d4
			 0x26e5029e, /// 0x4d8
			 0x8f9ca310, /// 0x4dc
			 0x26446d01, /// 0x4e0
			 0x9e81a661, /// 0x4e4
			 0xd03c12d1, /// 0x4e8
			 0x2507fc2c, /// 0x4ec
			 0x03fb3628, /// 0x4f0
			 0x8900ed4c, /// 0x4f4
			 0x084df8e4, /// 0x4f8
			 0xc58e0011, /// 0x4fc
			 0x07d30954, /// 0x500
			 0x18f4d1e9, /// 0x504
			 0xb1576d06, /// 0x508
			 0x2401d445, /// 0x50c
			 0x0547a308, /// 0x510
			 0x11b1e60b, /// 0x514
			 0x8788e4b2, /// 0x518
			 0x8877549c, /// 0x51c
			 0xf9997078, /// 0x520
			 0xf6719b77, /// 0x524
			 0x352dde7a, /// 0x528
			 0x076e4e2c, /// 0x52c
			 0x4f5a106b, /// 0x530
			 0x3a23b7d9, /// 0x534
			 0x5a1667f6, /// 0x538
			 0xe10b0c7e, /// 0x53c
			 0xfa9e909d, /// 0x540
			 0xdfe91b38, /// 0x544
			 0x0559240b, /// 0x548
			 0x0bceb762, /// 0x54c
			 0x3c8746b6, /// 0x550
			 0xac8d138f, /// 0x554
			 0xdae67db0, /// 0x558
			 0xee6f8320, /// 0x55c
			 0x75a31c34, /// 0x560
			 0x1e24a1ec, /// 0x564
			 0xdbe73503, /// 0x568
			 0x811d6ecf, /// 0x56c
			 0x40df5b6e, /// 0x570
			 0x990d8f32, /// 0x574
			 0xb656b937, /// 0x578
			 0xb31f4eeb, /// 0x57c
			 0x8ac3e6d5, /// 0x580
			 0x53913acb, /// 0x584
			 0xa0a99e42, /// 0x588
			 0x96888dcb, /// 0x58c
			 0x310030ce, /// 0x590
			 0x19894eef, /// 0x594
			 0x2dde2109, /// 0x598
			 0xf0eed00b, /// 0x59c
			 0x10341813, /// 0x5a0
			 0xbc254953, /// 0x5a4
			 0x62cf21b5, /// 0x5a8
			 0x8b944c42, /// 0x5ac
			 0x77bbd277, /// 0x5b0
			 0xcf3d1193, /// 0x5b4
			 0x307b644f, /// 0x5b8
			 0x6c7397a1, /// 0x5bc
			 0xade4fa83, /// 0x5c0
			 0x4207c377, /// 0x5c4
			 0xd349b389, /// 0x5c8
			 0xe79149e6, /// 0x5cc
			 0x4c0b64cb, /// 0x5d0
			 0x0b8713ce, /// 0x5d4
			 0xc1827abf, /// 0x5d8
			 0x5b4053c3, /// 0x5dc
			 0xc9cc0616, /// 0x5e0
			 0xd5a91ebe, /// 0x5e4
			 0xd4104f73, /// 0x5e8
			 0x7d0c9afd, /// 0x5ec
			 0xe7c5508c, /// 0x5f0
			 0x38432d53, /// 0x5f4
			 0x3db40b9f, /// 0x5f8
			 0x4a3cadbe, /// 0x5fc
			 0xbd0e245f, /// 0x600
			 0x3d4412da, /// 0x604
			 0x5ba63529, /// 0x608
			 0x58e84ed7, /// 0x60c
			 0x357099f6, /// 0x610
			 0xf1c5bf2b, /// 0x614
			 0x83ab2a39, /// 0x618
			 0x7cfb1b7a, /// 0x61c
			 0x106e49d7, /// 0x620
			 0x866a6311, /// 0x624
			 0xc0f6b592, /// 0x628
			 0x1d810479, /// 0x62c
			 0x614aaaa3, /// 0x630
			 0x6b0017b8, /// 0x634
			 0xcaa0d688, /// 0x638
			 0xe0db4f5a, /// 0x63c
			 0x27665e12, /// 0x640
			 0x8b469974, /// 0x644
			 0x6aa29205, /// 0x648
			 0xe6fb37a0, /// 0x64c
			 0x16c6d4b7, /// 0x650
			 0xf4400519, /// 0x654
			 0xa1a15216, /// 0x658
			 0x856b7199, /// 0x65c
			 0x5ad990d0, /// 0x660
			 0x9662dd5c, /// 0x664
			 0xd9be3dac, /// 0x668
			 0xc5c3903b, /// 0x66c
			 0xbe759d0a, /// 0x670
			 0xf1b6bd13, /// 0x674
			 0x96c02241, /// 0x678
			 0x5f87f9fd, /// 0x67c
			 0x2d1b9d95, /// 0x680
			 0xc7b6b49f, /// 0x684
			 0x7da1d4b5, /// 0x688
			 0x1468d398, /// 0x68c
			 0x8638decf, /// 0x690
			 0xe829015b, /// 0x694
			 0xdc43afc6, /// 0x698
			 0xf83f6db7, /// 0x69c
			 0x1a33a207, /// 0x6a0
			 0xe6948b8e, /// 0x6a4
			 0x80ef4b1a, /// 0x6a8
			 0x85dbf337, /// 0x6ac
			 0x87f5d273, /// 0x6b0
			 0x5492606a, /// 0x6b4
			 0x3f07278d, /// 0x6b8
			 0x0678b8c9, /// 0x6bc
			 0x2d63a344, /// 0x6c0
			 0xa8f8be33, /// 0x6c4
			 0xe0b2edfc, /// 0x6c8
			 0x735f1432, /// 0x6cc
			 0x8bd9c7a7, /// 0x6d0
			 0xf9834e7c, /// 0x6d4
			 0x66dd6507, /// 0x6d8
			 0x80833c56, /// 0x6dc
			 0x998cb16e, /// 0x6e0
			 0x86842bba, /// 0x6e4
			 0x7c46a88c, /// 0x6e8
			 0xb99fbf7d, /// 0x6ec
			 0x4b17547b, /// 0x6f0
			 0x8a233b95, /// 0x6f4
			 0x2a92a55c, /// 0x6f8
			 0x61f0e954, /// 0x6fc
			 0xc8ca989f, /// 0x700
			 0xc64fc17c, /// 0x704
			 0x393247f6, /// 0x708
			 0x8d8c728c, /// 0x70c
			 0x166c2769, /// 0x710
			 0x7ae0dc19, /// 0x714
			 0xe9bc4c1c, /// 0x718
			 0xe2e277a5, /// 0x71c
			 0xc925426a, /// 0x720
			 0x9dcf93f3, /// 0x724
			 0xf225b371, /// 0x728
			 0x8648552b, /// 0x72c
			 0x6c6a569b, /// 0x730
			 0x8102b3d8, /// 0x734
			 0x94c0ac28, /// 0x738
			 0x50b4d64c, /// 0x73c
			 0xa730b575, /// 0x740
			 0x592ebc25, /// 0x744
			 0x40c4bae6, /// 0x748
			 0x06576b23, /// 0x74c
			 0x60905c37, /// 0x750
			 0x2881d506, /// 0x754
			 0x86a49241, /// 0x758
			 0x9009c246, /// 0x75c
			 0xd3c21399, /// 0x760
			 0xab1c23cd, /// 0x764
			 0x45bc94b1, /// 0x768
			 0x40c49a64, /// 0x76c
			 0x7345e14b, /// 0x770
			 0xedccfd05, /// 0x774
			 0x6fb860cf, /// 0x778
			 0x3b0bf801, /// 0x77c
			 0xcdbbeb56, /// 0x780
			 0x0683a927, /// 0x784
			 0xc451489f, /// 0x788
			 0x0037fca8, /// 0x78c
			 0x819774dd, /// 0x790
			 0xfe46b6bc, /// 0x794
			 0x1f349885, /// 0x798
			 0xf72404a9, /// 0x79c
			 0x70469425, /// 0x7a0
			 0x10f7265e, /// 0x7a4
			 0x714832cb, /// 0x7a8
			 0x63f6fbdb, /// 0x7ac
			 0x0f64c377, /// 0x7b0
			 0xf5ada1b4, /// 0x7b4
			 0xee1d9ff1, /// 0x7b8
			 0x9e37adaf, /// 0x7bc
			 0x225ba3dc, /// 0x7c0
			 0xccfc2802, /// 0x7c4
			 0x9cf320d0, /// 0x7c8
			 0x48c247b8, /// 0x7cc
			 0xa0d2cb77, /// 0x7d0
			 0x93131f88, /// 0x7d4
			 0x4b5edb59, /// 0x7d8
			 0x36887027, /// 0x7dc
			 0x9d9118c5, /// 0x7e0
			 0x65b838a2, /// 0x7e4
			 0xccfd1cdd, /// 0x7e8
			 0xcb0eea73, /// 0x7ec
			 0x29d3a39b, /// 0x7f0
			 0x44fc6324, /// 0x7f4
			 0xe66f4776, /// 0x7f8
			 0xfb396c9c, /// 0x7fc
			 0xa01d35ec, /// 0x800
			 0xe0ee0b84, /// 0x804
			 0x824034f4, /// 0x808
			 0x9ef00bd0, /// 0x80c
			 0xe6cedc2f, /// 0x810
			 0x02846ca3, /// 0x814
			 0x37274c57, /// 0x818
			 0xd34edd3a, /// 0x81c
			 0xf9bbfb38, /// 0x820
			 0x605b0ab3, /// 0x824
			 0xe8182110, /// 0x828
			 0xab0df785, /// 0x82c
			 0xf68a392b, /// 0x830
			 0x3f39adbe, /// 0x834
			 0x4b682c11, /// 0x838
			 0x9e236b79, /// 0x83c
			 0x201ad193, /// 0x840
			 0x56c4ce96, /// 0x844
			 0x2801201f, /// 0x848
			 0xf22ede26, /// 0x84c
			 0x7da51618, /// 0x850
			 0xba48da31, /// 0x854
			 0xaf678b83, /// 0x858
			 0x4d35cf22, /// 0x85c
			 0x7349cd0c, /// 0x860
			 0x9211c2fe, /// 0x864
			 0x07821e7e, /// 0x868
			 0xa8589c5c, /// 0x86c
			 0xf70bcd20, /// 0x870
			 0xeb6bc8f1, /// 0x874
			 0x0b643eea, /// 0x878
			 0xf71f0bfb, /// 0x87c
			 0x549f298c, /// 0x880
			 0x301192bb, /// 0x884
			 0xe4cacd7a, /// 0x888
			 0x8017dc25, /// 0x88c
			 0x40b2ab02, /// 0x890
			 0x425232d0, /// 0x894
			 0x45d4a2c4, /// 0x898
			 0xce5db3ff, /// 0x89c
			 0x09d44fe4, /// 0x8a0
			 0x8d24ce72, /// 0x8a4
			 0x67eb631c, /// 0x8a8
			 0x3c65ccc1, /// 0x8ac
			 0x185f4ada, /// 0x8b0
			 0x87df41f9, /// 0x8b4
			 0x843fff6b, /// 0x8b8
			 0xf5fd0210, /// 0x8bc
			 0xed3da9b7, /// 0x8c0
			 0x53400db0, /// 0x8c4
			 0xe1e6ebf9, /// 0x8c8
			 0x59612cb6, /// 0x8cc
			 0xd37a4231, /// 0x8d0
			 0xf64657d3, /// 0x8d4
			 0x9b5dd3d9, /// 0x8d8
			 0x5cc69bd7, /// 0x8dc
			 0xd7071f53, /// 0x8e0
			 0x2c9f7d77, /// 0x8e4
			 0x8a8fc3ea, /// 0x8e8
			 0x84dd5996, /// 0x8ec
			 0x2aca199b, /// 0x8f0
			 0x49f53af1, /// 0x8f4
			 0x18632031, /// 0x8f8
			 0xcd7e2506, /// 0x8fc
			 0x65c69f01, /// 0x900
			 0x03253c9e, /// 0x904
			 0x47723a25, /// 0x908
			 0x002978f7, /// 0x90c
			 0x3ab70ce9, /// 0x910
			 0x157bd80d, /// 0x914
			 0x867dc8df, /// 0x918
			 0x2c7d57da, /// 0x91c
			 0xf569a8b6, /// 0x920
			 0xef905bf5, /// 0x924
			 0x03884db5, /// 0x928
			 0xcc881daa, /// 0x92c
			 0xd88c49f0, /// 0x930
			 0x62966e08, /// 0x934
			 0x43fdf267, /// 0x938
			 0xe9d2c606, /// 0x93c
			 0xd62fdeed, /// 0x940
			 0x995e71d3, /// 0x944
			 0xdc7eaac9, /// 0x948
			 0x344bde2c, /// 0x94c
			 0xfe148662, /// 0x950
			 0xaef2f539, /// 0x954
			 0xf5a969a5, /// 0x958
			 0xcf6c6c93, /// 0x95c
			 0x2137a649, /// 0x960
			 0x12d6bd43, /// 0x964
			 0x2a66a3c6, /// 0x968
			 0x5655ba41, /// 0x96c
			 0x7ea6b264, /// 0x970
			 0xda65fd84, /// 0x974
			 0x79c623ef, /// 0x978
			 0x8483ed7b, /// 0x97c
			 0x0de18b29, /// 0x980
			 0x813595c5, /// 0x984
			 0xb336d18b, /// 0x988
			 0x07f60d62, /// 0x98c
			 0x813a876a, /// 0x990
			 0x10bfaed3, /// 0x994
			 0x12cfe1ae, /// 0x998
			 0x9ef8863f, /// 0x99c
			 0x26ce88bb, /// 0x9a0
			 0xeee530e2, /// 0x9a4
			 0x2e72400d, /// 0x9a8
			 0x8187fd05, /// 0x9ac
			 0x3c2a71a2, /// 0x9b0
			 0xf57d7c5d, /// 0x9b4
			 0xa6d1ee30, /// 0x9b8
			 0x7024d252, /// 0x9bc
			 0x598ef9f1, /// 0x9c0
			 0x3534d146, /// 0x9c4
			 0x0172da19, /// 0x9c8
			 0x30176d33, /// 0x9cc
			 0x9d872cde, /// 0x9d0
			 0x853a4428, /// 0x9d4
			 0x938ed04f, /// 0x9d8
			 0xcb6af810, /// 0x9dc
			 0x25649ee2, /// 0x9e0
			 0xc831b4c0, /// 0x9e4
			 0xdfa17904, /// 0x9e8
			 0xacc94761, /// 0x9ec
			 0x1e0be308, /// 0x9f0
			 0x617f4324, /// 0x9f4
			 0x41d70c3a, /// 0x9f8
			 0xcdfa549b, /// 0x9fc
			 0x09e8cb1c, /// 0xa00
			 0xf6e585e0, /// 0xa04
			 0x5f7b76f8, /// 0xa08
			 0x0a742e6b, /// 0xa0c
			 0x36299df0, /// 0xa10
			 0x1b325ac7, /// 0xa14
			 0x0562b642, /// 0xa18
			 0x47d7b67a, /// 0xa1c
			 0x1f0eaace, /// 0xa20
			 0x2679b267, /// 0xa24
			 0xde0a0864, /// 0xa28
			 0x8fa794e8, /// 0xa2c
			 0x220c9c12, /// 0xa30
			 0x43754bd3, /// 0xa34
			 0xf653091c, /// 0xa38
			 0xd829b392, /// 0xa3c
			 0x078dc24c, /// 0xa40
			 0x8e4988de, /// 0xa44
			 0xea64cb69, /// 0xa48
			 0x3af67ea5, /// 0xa4c
			 0xb562f260, /// 0xa50
			 0xbc3bed6c, /// 0xa54
			 0x4b81fcad, /// 0xa58
			 0x1d7fac4d, /// 0xa5c
			 0xd0c708b6, /// 0xa60
			 0xde0a7fca, /// 0xa64
			 0x31fd76b7, /// 0xa68
			 0xc86e2d8c, /// 0xa6c
			 0x9fe8e080, /// 0xa70
			 0x7df237bb, /// 0xa74
			 0xac0eb52b, /// 0xa78
			 0x8a6fa788, /// 0xa7c
			 0xf7648cd6, /// 0xa80
			 0x91fd7fab, /// 0xa84
			 0x9046d4bd, /// 0xa88
			 0x37db8d5a, /// 0xa8c
			 0x6a82734b, /// 0xa90
			 0x5bdc19d9, /// 0xa94
			 0x659340b7, /// 0xa98
			 0x1937d384, /// 0xa9c
			 0x6300d69f, /// 0xaa0
			 0x5805ba80, /// 0xaa4
			 0xc6c3f223, /// 0xaa8
			 0xe0f9a51b, /// 0xaac
			 0x3b8a4956, /// 0xab0
			 0x8279d425, /// 0xab4
			 0x256ee29b, /// 0xab8
			 0x088304d7, /// 0xabc
			 0x05360da0, /// 0xac0
			 0x51fb3791, /// 0xac4
			 0x1ec22db0, /// 0xac8
			 0x7e2d2044, /// 0xacc
			 0xc0a0e284, /// 0xad0
			 0x4d3a9888, /// 0xad4
			 0x3d74ce08, /// 0xad8
			 0xdd493c3e, /// 0xadc
			 0x890a5228, /// 0xae0
			 0xb1ea4e4a, /// 0xae4
			 0x9b6cf1e0, /// 0xae8
			 0xcf6d6378, /// 0xaec
			 0xdf691886, /// 0xaf0
			 0x7c06f982, /// 0xaf4
			 0xcdde3985, /// 0xaf8
			 0x029f4a39, /// 0xafc
			 0x378f2cac, /// 0xb00
			 0xaa472df6, /// 0xb04
			 0xd0f85945, /// 0xb08
			 0xd2780d12, /// 0xb0c
			 0x2372bf58, /// 0xb10
			 0x99472bf1, /// 0xb14
			 0xf7eb22fc, /// 0xb18
			 0x3e485309, /// 0xb1c
			 0x21cb95d3, /// 0xb20
			 0x96a0ebd7, /// 0xb24
			 0x57bcf038, /// 0xb28
			 0xe1c2796a, /// 0xb2c
			 0xb7380a15, /// 0xb30
			 0x13121a92, /// 0xb34
			 0x6ff7b724, /// 0xb38
			 0x9021f3d3, /// 0xb3c
			 0xf5e5e1f9, /// 0xb40
			 0x4520bc89, /// 0xb44
			 0x730ac89e, /// 0xb48
			 0xe5753ca0, /// 0xb4c
			 0x2fdbf19e, /// 0xb50
			 0x961fb782, /// 0xb54
			 0x3c141e41, /// 0xb58
			 0x7a63befd, /// 0xb5c
			 0x0a006476, /// 0xb60
			 0xc4b20235, /// 0xb64
			 0x4f3c5e8b, /// 0xb68
			 0x1345d5af, /// 0xb6c
			 0x7c3e3a68, /// 0xb70
			 0xe4178858, /// 0xb74
			 0x8c37b53a, /// 0xb78
			 0xfb353c5d, /// 0xb7c
			 0x859875fa, /// 0xb80
			 0x6ba61f75, /// 0xb84
			 0xa6ea9ba5, /// 0xb88
			 0x3fea2cc3, /// 0xb8c
			 0x48a2adfa, /// 0xb90
			 0x9ffecd29, /// 0xb94
			 0x61e6f48c, /// 0xb98
			 0x6377bf27, /// 0xb9c
			 0x8f49594a, /// 0xba0
			 0x35a38d39, /// 0xba4
			 0x7e6b368d, /// 0xba8
			 0x6bf62ce0, /// 0xbac
			 0xa91409f1, /// 0xbb0
			 0x33654841, /// 0xbb4
			 0xb83dca4e, /// 0xbb8
			 0x498b92e3, /// 0xbbc
			 0x437efd40, /// 0xbc0
			 0x677255dd, /// 0xbc4
			 0xd19deb96, /// 0xbc8
			 0x6cdfb384, /// 0xbcc
			 0x3456da18, /// 0xbd0
			 0x9e2a2ca3, /// 0xbd4
			 0x81092aa5, /// 0xbd8
			 0x1710c193, /// 0xbdc
			 0x5afe7875, /// 0xbe0
			 0xf3be9a14, /// 0xbe4
			 0xc92f4acc, /// 0xbe8
			 0xf535dc9d, /// 0xbec
			 0x09778a4c, /// 0xbf0
			 0x8788f7f5, /// 0xbf4
			 0x7434ec69, /// 0xbf8
			 0x6fbbbb61, /// 0xbfc
			 0x8a55f154, /// 0xc00
			 0x69b183de, /// 0xc04
			 0x73f45b6b, /// 0xc08
			 0xbd2eead0, /// 0xc0c
			 0x4b59bfbc, /// 0xc10
			 0xe54c911d, /// 0xc14
			 0x72dc8dcf, /// 0xc18
			 0xc54d080f, /// 0xc1c
			 0x67995526, /// 0xc20
			 0xd6b66020, /// 0xc24
			 0x8197dc8e, /// 0xc28
			 0xd6623dac, /// 0xc2c
			 0x10408f32, /// 0xc30
			 0x2d1fd2e7, /// 0xc34
			 0xebf41bde, /// 0xc38
			 0xafda7862, /// 0xc3c
			 0x070430ad, /// 0xc40
			 0x47c40b75, /// 0xc44
			 0x4dcb71fd, /// 0xc48
			 0xbf5c74a7, /// 0xc4c
			 0x5e41221f, /// 0xc50
			 0x64a05949, /// 0xc54
			 0xad32d169, /// 0xc58
			 0xcb418ba9, /// 0xc5c
			 0x29c90ee1, /// 0xc60
			 0x45334efb, /// 0xc64
			 0x9128677a, /// 0xc68
			 0x15fbbf28, /// 0xc6c
			 0x92f29a48, /// 0xc70
			 0x1748e7ba, /// 0xc74
			 0xd92d28a7, /// 0xc78
			 0xa8c13334, /// 0xc7c
			 0x35520603, /// 0xc80
			 0x8220c470, /// 0xc84
			 0x48a164cd, /// 0xc88
			 0x09c0b3e9, /// 0xc8c
			 0x85a246db, /// 0xc90
			 0xdc62b01a, /// 0xc94
			 0x74db0c0d, /// 0xc98
			 0xe2880d35, /// 0xc9c
			 0x55d34a78, /// 0xca0
			 0x5dc21688, /// 0xca4
			 0x6d84f0ff, /// 0xca8
			 0x5ced4b9c, /// 0xcac
			 0x7e43bd6c, /// 0xcb0
			 0x5be2e2e5, /// 0xcb4
			 0x9d64d5e6, /// 0xcb8
			 0x5324a442, /// 0xcbc
			 0x3b68ee9d, /// 0xcc0
			 0x5f3d0a76, /// 0xcc4
			 0xc165e6b6, /// 0xcc8
			 0x4fa7ea28, /// 0xccc
			 0x5f0200e9, /// 0xcd0
			 0x9051c22e, /// 0xcd4
			 0xad83eaf8, /// 0xcd8
			 0x020d2cd9, /// 0xcdc
			 0x1ff8947e, /// 0xce0
			 0x93ec382e, /// 0xce4
			 0x0321d1bc, /// 0xce8
			 0x0a540c9c, /// 0xcec
			 0x23d7ffcb, /// 0xcf0
			 0xfe68c5a1, /// 0xcf4
			 0x8e93ba77, /// 0xcf8
			 0x3487e9b7, /// 0xcfc
			 0xb75d8927, /// 0xd00
			 0xe4282ccb, /// 0xd04
			 0x18df9504, /// 0xd08
			 0x4260cc01, /// 0xd0c
			 0x8c5f4db5, /// 0xd10
			 0xbcf5b431, /// 0xd14
			 0x9c2111c3, /// 0xd18
			 0x45079768, /// 0xd1c
			 0x17f88c2a, /// 0xd20
			 0x4e411e7b, /// 0xd24
			 0x7669944c, /// 0xd28
			 0x6d5344d7, /// 0xd2c
			 0x23424707, /// 0xd30
			 0x6fdbb65e, /// 0xd34
			 0xc2bb226c, /// 0xd38
			 0x3940043a, /// 0xd3c
			 0xc0d350df, /// 0xd40
			 0x3cb96c79, /// 0xd44
			 0x56fa112d, /// 0xd48
			 0xf287c903, /// 0xd4c
			 0x79203bc7, /// 0xd50
			 0xbd3d4180, /// 0xd54
			 0x67f8698f, /// 0xd58
			 0xe6db8ba0, /// 0xd5c
			 0x14716c60, /// 0xd60
			 0x4b3aafa3, /// 0xd64
			 0x6bc4920a, /// 0xd68
			 0xf14a9fb6, /// 0xd6c
			 0x52439f3d, /// 0xd70
			 0x27994299, /// 0xd74
			 0x18d5194d, /// 0xd78
			 0x9f84889f, /// 0xd7c
			 0x4d8039d1, /// 0xd80
			 0xfe914882, /// 0xd84
			 0xef650f1d, /// 0xd88
			 0xe35b510a, /// 0xd8c
			 0xaf6338a9, /// 0xd90
			 0x9634f3f7, /// 0xd94
			 0xb4bc2018, /// 0xd98
			 0xf893334b, /// 0xd9c
			 0x92d9e608, /// 0xda0
			 0xfd414307, /// 0xda4
			 0x847f2339, /// 0xda8
			 0x4fc652dd, /// 0xdac
			 0x3231797b, /// 0xdb0
			 0xc2bacc2b, /// 0xdb4
			 0xe4f2e21e, /// 0xdb8
			 0x7a2c6a9f, /// 0xdbc
			 0xb2427336, /// 0xdc0
			 0xc9968fe7, /// 0xdc4
			 0x64ab46b0, /// 0xdc8
			 0x9a351cb6, /// 0xdcc
			 0xb8644a87, /// 0xdd0
			 0x76555a0f, /// 0xdd4
			 0xbe095335, /// 0xdd8
			 0x68232205, /// 0xddc
			 0xf7a9f6d9, /// 0xde0
			 0xab92850c, /// 0xde4
			 0x908640ec, /// 0xde8
			 0xcf62c25b, /// 0xdec
			 0xb95809b2, /// 0xdf0
			 0x60de3aa5, /// 0xdf4
			 0xa239f358, /// 0xdf8
			 0x483bd683, /// 0xdfc
			 0x789e152d, /// 0xe00
			 0xc4140c3b, /// 0xe04
			 0xf0e730b5, /// 0xe08
			 0x5fd10d74, /// 0xe0c
			 0x5e5dfece, /// 0xe10
			 0x97b792f0, /// 0xe14
			 0xc57982b2, /// 0xe18
			 0xb6da17b0, /// 0xe1c
			 0xd909b2ab, /// 0xe20
			 0x421589f1, /// 0xe24
			 0xbdb6039c, /// 0xe28
			 0xd8693902, /// 0xe2c
			 0x406a577b, /// 0xe30
			 0x947125ef, /// 0xe34
			 0xb669a157, /// 0xe38
			 0x4e3be9fa, /// 0xe3c
			 0x40980649, /// 0xe40
			 0xe137dbe0, /// 0xe44
			 0x0680943a, /// 0xe48
			 0x229aec9d, /// 0xe4c
			 0x481e0b0a, /// 0xe50
			 0x23252270, /// 0xe54
			 0x7fdbc32a, /// 0xe58
			 0x5b4a48d6, /// 0xe5c
			 0x4d425586, /// 0xe60
			 0x8f1b82f0, /// 0xe64
			 0x2526bdae, /// 0xe68
			 0x585a85eb, /// 0xe6c
			 0x7448af8d, /// 0xe70
			 0x6b330ef9, /// 0xe74
			 0xbec57fb8, /// 0xe78
			 0xcc11ea9f, /// 0xe7c
			 0xdc828c7e, /// 0xe80
			 0x78c29dc2, /// 0xe84
			 0x180fa6d6, /// 0xe88
			 0xf554d451, /// 0xe8c
			 0x2546dfb8, /// 0xe90
			 0xb2ab7c5d, /// 0xe94
			 0xedca336c, /// 0xe98
			 0x873e95a7, /// 0xe9c
			 0x7b0586a2, /// 0xea0
			 0x172c29f6, /// 0xea4
			 0x8196d483, /// 0xea8
			 0x695db44b, /// 0xeac
			 0x6d417edc, /// 0xeb0
			 0x21710013, /// 0xeb4
			 0xd6ae5ada, /// 0xeb8
			 0xe1b98a88, /// 0xebc
			 0xb1d6b4de, /// 0xec0
			 0x8bfb6385, /// 0xec4
			 0x0da2fa13, /// 0xec8
			 0xa15e95eb, /// 0xecc
			 0xce454d28, /// 0xed0
			 0xa54f55f6, /// 0xed4
			 0x361c3480, /// 0xed8
			 0x41924161, /// 0xedc
			 0xcd8f6b03, /// 0xee0
			 0x1663b21b, /// 0xee4
			 0x12b08755, /// 0xee8
			 0x7c1e6ed1, /// 0xeec
			 0xd24861c2, /// 0xef0
			 0x4bfa8a35, /// 0xef4
			 0xa69f6ab3, /// 0xef8
			 0x8f6d4970, /// 0xefc
			 0x8b740fb0, /// 0xf00
			 0x5146949d, /// 0xf04
			 0x06ca3701, /// 0xf08
			 0x4b233ec9, /// 0xf0c
			 0xc5042de3, /// 0xf10
			 0x270773ad, /// 0xf14
			 0x670f8c46, /// 0xf18
			 0xfe4b9638, /// 0xf1c
			 0xb3c054fa, /// 0xf20
			 0xd291146f, /// 0xf24
			 0x37e4b6e5, /// 0xf28
			 0x49f02798, /// 0xf2c
			 0x402d7d46, /// 0xf30
			 0x3ce9d3a9, /// 0xf34
			 0x6bb4c2b0, /// 0xf38
			 0x639a6efb, /// 0xf3c
			 0xd3cea790, /// 0xf40
			 0xeed9e970, /// 0xf44
			 0x18e3b1fd, /// 0xf48
			 0x9c1465e5, /// 0xf4c
			 0x6146f7db, /// 0xf50
			 0xdf1db74b, /// 0xf54
			 0x47632ee8, /// 0xf58
			 0x35755d24, /// 0xf5c
			 0xc1279351, /// 0xf60
			 0xa3a62b52, /// 0xf64
			 0xe6815f6d, /// 0xf68
			 0x0e2a01de, /// 0xf6c
			 0x6f860c73, /// 0xf70
			 0xd6530a58, /// 0xf74
			 0xefb9a752, /// 0xf78
			 0x6ea9ab70, /// 0xf7c
			 0x76b2d482, /// 0xf80
			 0x559dabdb, /// 0xf84
			 0xf352725e, /// 0xf88
			 0x75bd1d02, /// 0xf8c
			 0x0a946682, /// 0xf90
			 0xf52717be, /// 0xf94
			 0xf523f3f7, /// 0xf98
			 0x426886da, /// 0xf9c
			 0x25e9e015, /// 0xfa0
			 0x8f383476, /// 0xfa4
			 0x68cee408, /// 0xfa8
			 0x5f9c81e4, /// 0xfac
			 0x07fa64a8, /// 0xfb0
			 0xdb37e216, /// 0xfb4
			 0xe5637840, /// 0xfb8
			 0xb7bcecbe, /// 0xfbc
			 0x61efa93f, /// 0xfc0
			 0xe42adedf, /// 0xfc4
			 0x4ac19ac9, /// 0xfc8
			 0xba33f40e, /// 0xfcc
			 0xae86f3ec, /// 0xfd0
			 0x4640fbfc, /// 0xfd4
			 0xdef2cc30, /// 0xfd8
			 0x1e257dbc, /// 0xfdc
			 0x9f7d646d, /// 0xfe0
			 0x3b42b97e, /// 0xfe4
			 0x06069243, /// 0xfe8
			 0xe562652a, /// 0xfec
			 0x2da0c350, /// 0xff0
			 0x92e0c193, /// 0xff4
			 0xa1dbf05f, /// 0xff8
			 0xe6eb8dcc /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00004
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00008
			 0x0c600000,                                                  // Leading 1s:                                 /// 0000c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00010
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00014
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00018
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0001c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x0c780000,                                                  // Leading 1s:                                 /// 00028
			 0x33333333,                                                  // 4 random values                             /// 0002c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x7f800000,                                                  // inf                                         /// 00038
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0003c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00040
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00048
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0004c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00050
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0005c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00068
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00070
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00078
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00080
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00084
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0008c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00090
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00094
			 0xbf028f5c,                                                  // -0.51                                       /// 00098
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0009c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 000b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000c8
			 0xcb000000,                                                  // -8388608.0                                  /// 000cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 000f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00104
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00108
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00118
			 0x80011111,                                                  // -9.7958E-41                                 /// 0011c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0012c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00138
			 0xff800000,                                                  // -inf                                        /// 0013c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00140
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00150
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00158
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0015c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00164
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0016c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00174
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00184
			 0x0e000001,                                                  // Trailing 1s:                                /// 00188
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00190
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00194
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00198
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00200
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00204
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00208
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0020c
			 0xffc00001,                                                  // -signaling NaN                              /// 00210
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x7f800000,                                                  // inf                                         /// 00218
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x0c600000,                                                  // Leading 1s:                                 /// 00224
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0022c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00234
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00238
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0023c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00248
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0024c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00250
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00258
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00260
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00270
			 0x55555555,                                                  // 4 random values                             /// 00274
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00278
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0027c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00284
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00288
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00294
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00298
			 0x80011111,                                                  // -9.7958E-41                                 /// 0029c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e8
			 0x3f028f5c,                                                  // 0.51                                        /// 002ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002f0
			 0x55555555,                                                  // 4 random values                             /// 002f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00304
			 0x7fc00001,                                                  // signaling NaN                               /// 00308
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0030c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00314
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00318
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0031c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00328
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00330
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00334
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00338
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0033c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00340
			 0x3f028f5c,                                                  // 0.51                                        /// 00344
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00348
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0034c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00354
			 0x80011111,                                                  // -9.7958E-41                                 /// 00358
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0035c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00360
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00368
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0036c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00374
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0x0e000007,                                                  // Trailing 1s:                                /// 0037c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00380
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00384
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00388
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0038c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00390
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00394
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00398
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0039c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003c4
			 0x00000000,                                                  // zero                                        /// 003c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00404
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00408
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00414
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x33333333,                                                  // 4 random values                             /// 00420
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00424
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00428
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0042c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x55555555,                                                  // 4 random values                             /// 0043c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00440
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00444
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00448
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0044c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00454
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00458
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0045c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00460
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00464
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00468
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0046c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00470
			 0x0c400000,                                                  // Leading 1s:                                 /// 00474
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0047c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00480
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00484
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00488
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00494
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00498
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0049c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004c8
			 0x3f028f5c,                                                  // 0.51                                        /// 004cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004f4
			 0x7f800000,                                                  // inf                                         /// 004f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00500
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00504
			 0x00000000,                                                  // zero                                        /// 00508
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00510
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00518
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0051c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00520
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00524
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0052c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00530
			 0xffc00001,                                                  // -signaling NaN                              /// 00534
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00538
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00544
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00548
			 0x0c600000,                                                  // Leading 1s:                                 /// 0054c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00550
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00558
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0055c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0xbf028f5c,                                                  // -0.51                                       /// 00564
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0xbf028f5c,                                                  // -0.51                                       /// 00574
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0057c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00580
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00588
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00590
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 005b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0xcb000000,                                                  // -8388608.0                                  /// 005c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005cc
			 0xbf028f5c,                                                  // -0.51                                       /// 005d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x3f028f5c,                                                  // 0.51                                        /// 005d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00600
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00608
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0060c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0xbf028f5c,                                                  // -0.51                                       /// 00614
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00618
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00620
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00624
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00628
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0062c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00630
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00638
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0e000001,                                                  // Trailing 1s:                                /// 00644
			 0x0e000007,                                                  // Trailing 1s:                                /// 00648
			 0x7f800000,                                                  // inf                                         /// 0064c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00654
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0065c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00664
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00668
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0066c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00670
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00674
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00680
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00684
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00688
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0068c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00690
			 0xffc00001,                                                  // -signaling NaN                              /// 00694
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00698
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0069c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 006dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00700
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00704
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00718
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00730
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00738
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00740
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00748
			 0x0c780000,                                                  // Leading 1s:                                 /// 0074c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00750
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00758
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00764
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00768
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0076c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00770
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00774
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00778
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00780
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00784
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00788
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0078c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00790
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0079c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00810
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0081c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00820
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x33333333,                                                  // 4 random values                             /// 00828
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0082c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00830
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00838
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0083c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00840
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00844
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00854
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0085c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00860
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0086c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00878
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00880
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00884
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00888
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0088c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00894
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0089c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008d4
			 0x33333333,                                                  // 4 random values                             /// 008d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00900
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00904
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00908
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0090c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00910
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00918
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00920
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00928
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0092c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00930
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00934
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00938
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00944
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00948
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0094c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00950
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00958
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00964
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0096c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00970
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00978
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0097c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00980
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0098c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00998
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0099c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0xffc00001,                                                  // -signaling NaN                              /// 009e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x55555555,                                                  // 4 random values                             /// 00a30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a3c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a64
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a68
			 0x55555555,                                                  // 4 random values                             /// 00a6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a74
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a84
			 0xff800000,                                                  // -inf                                        /// 00a88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a90
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a9c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00aa4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ab0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ab8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00abc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ac0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ac8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00acc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ad4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00adc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ae0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ae8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00af4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00af8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00afc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b1c
			 0x33333333,                                                  // 4 random values                             /// 00b20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b50
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b68
			 0xcb000000,                                                  // -8388608.0                                  /// 00b6c
			 0x33333333,                                                  // 4 random values                             /// 00b70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b74
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b98
			 0xff800000,                                                  // -inf                                        /// 00b9c
			 0x80000000,                                                  // -zero                                       /// 00ba0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ba8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bb0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bb4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bb8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bbc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bc0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bcc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bd4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bd8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bdc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bf0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bf8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bfc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c00
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0xffc00001,                                                  // -signaling NaN                              /// 00c14
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0xbf028f5c,                                                  // -0.51                                       /// 00c5c
			 0xff800000,                                                  // -inf                                        /// 00c60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c80
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ca0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ca4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cb8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cbc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cdc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ce0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x80000000,                                                  // -zero                                       /// 00ce8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x3f028f5c,                                                  // 0.51                                        /// 00cf0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cf4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cf8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cfc
			 0x4b000000,                                                  // 8388608.0                                   /// 00d00
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x33333333,                                                  // 4 random values                             /// 00d0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d18
			 0x7fc00001,                                                  // signaling NaN                               /// 00d1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d20
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d40
			 0xffc00001,                                                  // -signaling NaN                              /// 00d44
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d50
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d64
			 0x7f800000,                                                  // inf                                         /// 00d68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d80
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00da0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00da8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00db0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00db4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dc8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dcc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00de0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00de8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00df0
			 0x55555555,                                                  // 4 random values                             /// 00df4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dfc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e0c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e40
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e54
			 0x7f800000,                                                  // inf                                         /// 00e58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e94
			 0x7f800000,                                                  // inf                                         /// 00e98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ea8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00eac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eb0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00eb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ec0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ec8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ecc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ed4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ed8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ee0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ee8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ef0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ef4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ef8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00efc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f00
			 0x4b000000,                                                  // 8388608.0                                   /// 00f04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f5c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fb0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fc0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fcc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fe4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fe8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ff0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x7f7ffffe // max norm - 1ulp                               // SP +ve numbers                              /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xff800000,                                                  // -inf                                        /// 00000
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00004
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x7f800000,                                                  // inf                                         /// 0000c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00010
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00014
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00018
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0001c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00020
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0002c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00030
			 0x00011111,                                                  // 9.7958E-41                                  /// 00034
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00038
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00040
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00044
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00048
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0005c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00070
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00074
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00078
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0007c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00080
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00084
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00088
			 0x33333333,                                                  // 4 random values                             /// 0008c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00090
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00098
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0009c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000a4
			 0x4b000000,                                                  // 8388608.0                                   /// 000a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00100
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00104
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0xcb000000,                                                  // -8388608.0                                  /// 0010c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00110
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00118
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x3f028f5c,                                                  // 0.51                                        /// 00120
			 0x0c700000,                                                  // Leading 1s:                                 /// 00124
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00128
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0012c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00138
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0013c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00144
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00148
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0014c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0xff800000,                                                  // -inf                                        /// 0015c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00160
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x80011111,                                                  // -9.7958E-41                                 /// 0016c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00174
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0017c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00180
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00188
			 0x0c700000,                                                  // Leading 1s:                                 /// 0018c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00190
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00194
			 0x0e000003,                                                  // Trailing 1s:                                /// 00198
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001a0
			 0x7fc00001,                                                  // signaling NaN                               /// 001a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 001b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x55555555,                                                  // 4 random values                             /// 001e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00200
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x33333333,                                                  // 4 random values                             /// 00208
			 0x0e000001,                                                  // Trailing 1s:                                /// 0020c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00210
			 0xff800000,                                                  // -inf                                        /// 00214
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00218
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0021c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00220
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00224
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00228
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0022c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00230
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00238
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0023c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00240
			 0xbf028f5c,                                                  // -0.51                                       /// 00244
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00248
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00250
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00254
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00258
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0025c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0026c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00270
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00274
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0027c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00280
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00288
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0028c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00290
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0029c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 002bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 002e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00300
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00304
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00308
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0xffc00001,                                                  // -signaling NaN                              /// 00314
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00318
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0031c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00320
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00338
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0033c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00344
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00348
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00358
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00360
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00364
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00368
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00370
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00374
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00378
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x33333333,                                                  // 4 random values                             /// 00380
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00388
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x4b000000,                                                  // 8388608.0                                   /// 00390
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00394
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00398
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003c8
			 0x00000000,                                                  // zero                                        /// 003cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003e4
			 0x4b000000,                                                  // 8388608.0                                   /// 003e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00400
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00404
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00408
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0xffc00001,                                                  // -signaling NaN                              /// 00414
			 0x80000000,                                                  // -zero                                       /// 00418
			 0x0e000007,                                                  // Trailing 1s:                                /// 0041c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00420
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0042c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x0c780000,                                                  // Leading 1s:                                 /// 00434
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0043c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00011111,                                                  // 9.7958E-41                                  /// 00444
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0044c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0045c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00464
			 0x4b000000,                                                  // 8388608.0                                   /// 00468
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0046c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00470
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00478
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0047c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00480
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00484
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00488
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xbf028f5c,                                                  // -0.51                                       /// 00490
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00498
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x00000000,                                                  // zero                                        /// 00504
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00508
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x4b000000,                                                  // 8388608.0                                   /// 00514
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00518
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00520
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00524
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00528
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0052c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00530
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00538
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0053c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00544
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00548
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00554
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0xbf028f5c,                                                  // -0.51                                       /// 0055c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00564
			 0x0e000001,                                                  // Trailing 1s:                                /// 00568
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00578
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0057c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00590
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00594
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00598
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0059c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005c8
			 0xff800000,                                                  // -inf                                        /// 005cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005f4
			 0x00000000,                                                  // zero                                        /// 005f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00600
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00604
			 0x0c600000,                                                  // Leading 1s:                                 /// 00608
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x7f800000,                                                  // inf                                         /// 00610
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00614
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00618
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0061c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00620
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0062c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00634
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0063c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x00011111,                                                  // 9.7958E-41                                  /// 00644
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00648
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0064c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00650
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00654
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00658
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00660
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00664
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00668
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0066c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00674
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00678
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0067c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00680
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00684
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00688
			 0x80011111,                                                  // -9.7958E-41                                 /// 0068c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00694
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00698
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0069c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0xff800000,                                                  // -inf                                        /// 006c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00000000,                                                  // zero                                        /// 006d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00700
			 0x33333333,                                                  // 4 random values                             /// 00704
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00708
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0070c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00710
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00714
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00718
			 0x55555555,                                                  // 4 random values                             /// 0071c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00720
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00724
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00728
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0072c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x4b000000,                                                  // 8388608.0                                   /// 00734
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00738
			 0x0c600000,                                                  // Leading 1s:                                 /// 0073c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00740
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00744
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00748
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00750
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00754
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00758
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0075c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00760
			 0x80000000,                                                  // -zero                                       /// 00764
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00768
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0076c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00770
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00780
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00784
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00798
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0079c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x55555555,                                                  // 4 random values                             /// 007c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00800
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00804
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00808
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x7fc00001,                                                  // signaling NaN                               /// 00810
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00814
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00824
			 0x0c600000,                                                  // Leading 1s:                                 /// 00828
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0082c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00834
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00838
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00840
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00844
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0084c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00854
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00860
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00864
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00868
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00870
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00874
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00878
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x33333333,                                                  // 4 random values                             /// 00884
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00888
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0088c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00890
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00894
			 0x0c780000,                                                  // Leading 1s:                                 /// 00898
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0089c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008a4
			 0x7fc00001,                                                  // signaling NaN                               /// 008a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 008b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00900
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00904
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00908
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00910
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00914
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0091c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00920
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0092c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00934
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00938
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0093c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00944
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00948
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0094c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00958
			 0x00000000,                                                  // zero                                        /// 0095c
			 0xff800000,                                                  // -inf                                        /// 00960
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0096c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00970
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00974
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0097c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00980
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00984
			 0x0c400000,                                                  // Leading 1s:                                 /// 00988
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00990
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00994
			 0xbf028f5c,                                                  // -0.51                                       /// 00998
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0099c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x4b000000,                                                  // 8388608.0                                   /// 009a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a38
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a70
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a78
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a80
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x7f800000,                                                  // inf                                         /// 00a8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00aa0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00aa4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ab0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ab8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00abc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ac0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00acc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ad0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ad8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00adc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ae0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ae4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ae8
			 0xffc00001,                                                  // -signaling NaN                              /// 00aec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00af0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00af4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00af8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00afc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b00
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b0c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b1c
			 0x7f800000,                                                  // inf                                         /// 00b20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x33333333,                                                  // 4 random values                             /// 00b54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x55555555,                                                  // 4 random values                             /// 00b84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b90
			 0x80000000,                                                  // -zero                                       /// 00b94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b98
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ba0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ba4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ba8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bb0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bb4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bb8
			 0xff800000,                                                  // -inf                                        /// 00bbc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bd0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00be0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bf0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bf4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bfc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c00
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c10
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c1c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c24
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c4c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c5c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c80
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c88
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c9c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ca0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ca4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0xffc00001,                                                  // -signaling NaN                              /// 00cb4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cc0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cc8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cdc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ce8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cf0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cf4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cfc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d28
			 0xcb000000,                                                  // -8388608.0                                  /// 00d2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d50
			 0xcb000000,                                                  // -8388608.0                                  /// 00d54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00da0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00da4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dc8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dd8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00de0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00de4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00df0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00df8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dfc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e00
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e20
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e2c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e48
			 0x7f800000,                                                  // inf                                         /// 00e4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e80
			 0xffc00001,                                                  // -signaling NaN                              /// 00e84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e98
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ea4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ea8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ec0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ec4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ec8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ed0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ed4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ed8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00edc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ee0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ef0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ef4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00efc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f14
			 0xbf028f5c,                                                  // -0.51                                       /// 00f18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f54
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f60
			 0x80000000,                                                  // -zero                                       /// 00f64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f88
			 0xcb000000,                                                  // -8388608.0                                  /// 00f8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f90
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x4b000000,                                                  // 8388608.0                                   /// 00f9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fc4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fcc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fd4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fd8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ff8
			 0x0e0003ff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x2bb8e912, /// 0x0
			 0xd2d86863, /// 0x4
			 0xffc4a531, /// 0x8
			 0x825a3abf, /// 0xc
			 0xec3023c6, /// 0x10
			 0x0e7321ad, /// 0x14
			 0x30d9cf92, /// 0x18
			 0x22573c8c, /// 0x1c
			 0x143a559d, /// 0x20
			 0xdd1d10f2, /// 0x24
			 0xdbfbf7ac, /// 0x28
			 0x3ce1fc9d, /// 0x2c
			 0xc7989f64, /// 0x30
			 0xbb33a5d1, /// 0x34
			 0x84e5b718, /// 0x38
			 0x746b4d50, /// 0x3c
			 0x6515d014, /// 0x40
			 0x0448cdda, /// 0x44
			 0xee5bdb5a, /// 0x48
			 0xfe1657f7, /// 0x4c
			 0x057cfcf1, /// 0x50
			 0x1940d919, /// 0x54
			 0x614856af, /// 0x58
			 0x4dfbcfb7, /// 0x5c
			 0x0e7ea271, /// 0x60
			 0x9ca96bfc, /// 0x64
			 0xc40b53a4, /// 0x68
			 0x0702b796, /// 0x6c
			 0x3c30989d, /// 0x70
			 0x8460e686, /// 0x74
			 0x9467e79c, /// 0x78
			 0x7f1c78ac, /// 0x7c
			 0x9050c187, /// 0x80
			 0x54656001, /// 0x84
			 0xb19dd6e6, /// 0x88
			 0x0f96a1eb, /// 0x8c
			 0xac0fb35d, /// 0x90
			 0x19aa569d, /// 0x94
			 0x4d3c34c2, /// 0x98
			 0x36656b7b, /// 0x9c
			 0x77211361, /// 0xa0
			 0xcdf7c44e, /// 0xa4
			 0x26ced44b, /// 0xa8
			 0x4fd9cf0c, /// 0xac
			 0xc9077a40, /// 0xb0
			 0xd54ccba2, /// 0xb4
			 0xfba9fb18, /// 0xb8
			 0x3a63c71e, /// 0xbc
			 0xa216518d, /// 0xc0
			 0x98f042e4, /// 0xc4
			 0x24a5cd4a, /// 0xc8
			 0x33d4f581, /// 0xcc
			 0xded4e531, /// 0xd0
			 0x227bfd65, /// 0xd4
			 0x0d6e83d7, /// 0xd8
			 0x7ec83399, /// 0xdc
			 0x5cb7ed8f, /// 0xe0
			 0xe77b6477, /// 0xe4
			 0x8dd59129, /// 0xe8
			 0x74ccb9d2, /// 0xec
			 0xa309ff0e, /// 0xf0
			 0x3583c401, /// 0xf4
			 0x63fcdb19, /// 0xf8
			 0xe7edc29f, /// 0xfc
			 0x1f14ccff, /// 0x100
			 0x660ec30d, /// 0x104
			 0x0a0629f5, /// 0x108
			 0xb0c5ec03, /// 0x10c
			 0x363c7bac, /// 0x110
			 0xe62254dd, /// 0x114
			 0xaa4a426b, /// 0x118
			 0x490cf1e0, /// 0x11c
			 0x326a72b2, /// 0x120
			 0xa32023bb, /// 0x124
			 0xd12f3d65, /// 0x128
			 0x8251c938, /// 0x12c
			 0x1b994b0e, /// 0x130
			 0x034dd8a3, /// 0x134
			 0xf5e540db, /// 0x138
			 0xa531bb33, /// 0x13c
			 0x9e451ee6, /// 0x140
			 0xa0b5efa2, /// 0x144
			 0x2450ccff, /// 0x148
			 0x723da7dd, /// 0x14c
			 0x350f1a10, /// 0x150
			 0xfd8be11f, /// 0x154
			 0x260cb993, /// 0x158
			 0xdc1bc23d, /// 0x15c
			 0x9ef580f7, /// 0x160
			 0x5eb15415, /// 0x164
			 0xd4afd1b3, /// 0x168
			 0xce9fead1, /// 0x16c
			 0x5763e8a7, /// 0x170
			 0xaba3d7de, /// 0x174
			 0x7d3f5f0b, /// 0x178
			 0x69fed707, /// 0x17c
			 0x19d38142, /// 0x180
			 0x0656c481, /// 0x184
			 0x9eddbec9, /// 0x188
			 0xc594891e, /// 0x18c
			 0x0ec4a915, /// 0x190
			 0xa23967e1, /// 0x194
			 0x7095b016, /// 0x198
			 0x57daf8fb, /// 0x19c
			 0x462096ca, /// 0x1a0
			 0xc9724958, /// 0x1a4
			 0xb1670eb8, /// 0x1a8
			 0x3e266d9a, /// 0x1ac
			 0x8639497e, /// 0x1b0
			 0xeeb8f32a, /// 0x1b4
			 0x417f4ce3, /// 0x1b8
			 0xb0a03e85, /// 0x1bc
			 0xb8c8dd5a, /// 0x1c0
			 0xa5e232f9, /// 0x1c4
			 0xa66650a5, /// 0x1c8
			 0x6d06f31d, /// 0x1cc
			 0xa7469f9c, /// 0x1d0
			 0x18142a2a, /// 0x1d4
			 0x6f6ad242, /// 0x1d8
			 0x93b249bc, /// 0x1dc
			 0xe139afce, /// 0x1e0
			 0x42ea60bc, /// 0x1e4
			 0x1dd7fe28, /// 0x1e8
			 0x4c989088, /// 0x1ec
			 0x8a313df7, /// 0x1f0
			 0x3f2eba7d, /// 0x1f4
			 0x3127869d, /// 0x1f8
			 0xabc3622b, /// 0x1fc
			 0xfdd93929, /// 0x200
			 0x34c7feba, /// 0x204
			 0x54bf2380, /// 0x208
			 0xca14a72d, /// 0x20c
			 0xb37e6f13, /// 0x210
			 0x59655023, /// 0x214
			 0x3ee626cf, /// 0x218
			 0x08539544, /// 0x21c
			 0xef8f4337, /// 0x220
			 0x0d1f7173, /// 0x224
			 0xe1cc2b2e, /// 0x228
			 0x1778637f, /// 0x22c
			 0x037bf7ab, /// 0x230
			 0x0f72e294, /// 0x234
			 0xdd902292, /// 0x238
			 0x52c82602, /// 0x23c
			 0xb1f3950d, /// 0x240
			 0x71ca0865, /// 0x244
			 0xc5bc8943, /// 0x248
			 0xca9cf983, /// 0x24c
			 0xe74fea7b, /// 0x250
			 0xf50dbf5d, /// 0x254
			 0xcff2da77, /// 0x258
			 0x81d8f202, /// 0x25c
			 0xedf33aea, /// 0x260
			 0xf76ee39e, /// 0x264
			 0x3a02ee9c, /// 0x268
			 0x816ce3ac, /// 0x26c
			 0x80226722, /// 0x270
			 0x8ab478f7, /// 0x274
			 0x51d236ee, /// 0x278
			 0xcf6a1236, /// 0x27c
			 0x111f055e, /// 0x280
			 0x999a5a40, /// 0x284
			 0x60d33050, /// 0x288
			 0xc89a852d, /// 0x28c
			 0x7396b0a4, /// 0x290
			 0x2c1a7bad, /// 0x294
			 0x5b5dd30e, /// 0x298
			 0x201ae15a, /// 0x29c
			 0x6ebf8a54, /// 0x2a0
			 0xb37e44ab, /// 0x2a4
			 0x267c494a, /// 0x2a8
			 0x14ff72bb, /// 0x2ac
			 0x8c392b4c, /// 0x2b0
			 0xbdcb26f8, /// 0x2b4
			 0x8a2717e8, /// 0x2b8
			 0xd622d4e1, /// 0x2bc
			 0x7481b0ba, /// 0x2c0
			 0x636b0991, /// 0x2c4
			 0x127918b9, /// 0x2c8
			 0x1c6eb5c8, /// 0x2cc
			 0x7e26daab, /// 0x2d0
			 0x36510c33, /// 0x2d4
			 0x8ac9ff30, /// 0x2d8
			 0xec3a1a19, /// 0x2dc
			 0xcc163e6f, /// 0x2e0
			 0xf0e33185, /// 0x2e4
			 0x44c9cb67, /// 0x2e8
			 0x219db9c8, /// 0x2ec
			 0xa6ba6277, /// 0x2f0
			 0x832a8934, /// 0x2f4
			 0x8a5a9c06, /// 0x2f8
			 0xe923adad, /// 0x2fc
			 0xbbd8a649, /// 0x300
			 0xeeb7560d, /// 0x304
			 0x5a6e16d8, /// 0x308
			 0x0127fff6, /// 0x30c
			 0x26ff7e95, /// 0x310
			 0x9fd7c350, /// 0x314
			 0x88314eb2, /// 0x318
			 0x30281fe4, /// 0x31c
			 0xe753a1be, /// 0x320
			 0x5bb548c9, /// 0x324
			 0x9639fe8c, /// 0x328
			 0x33204c99, /// 0x32c
			 0x49b222a9, /// 0x330
			 0xa9d3551a, /// 0x334
			 0x61430658, /// 0x338
			 0x5a1f79f3, /// 0x33c
			 0x487959a1, /// 0x340
			 0x1a55974b, /// 0x344
			 0x2a3bd611, /// 0x348
			 0x329660e5, /// 0x34c
			 0xeab9ce42, /// 0x350
			 0x4e0c72d5, /// 0x354
			 0xdc325638, /// 0x358
			 0xda008faa, /// 0x35c
			 0x356050be, /// 0x360
			 0x724d9a01, /// 0x364
			 0x34f46ee4, /// 0x368
			 0x42b8d6da, /// 0x36c
			 0x5d69db32, /// 0x370
			 0x8d3e9629, /// 0x374
			 0x614b7cb0, /// 0x378
			 0xc98f7745, /// 0x37c
			 0x92a829cd, /// 0x380
			 0x919844e4, /// 0x384
			 0xf6303f97, /// 0x388
			 0x6d664f96, /// 0x38c
			 0x37460b14, /// 0x390
			 0x41953fe8, /// 0x394
			 0x607b7a22, /// 0x398
			 0x0bdb0bf4, /// 0x39c
			 0x85adc79f, /// 0x3a0
			 0x0e3017c6, /// 0x3a4
			 0x16e3e8ec, /// 0x3a8
			 0x6c95c069, /// 0x3ac
			 0x869f9543, /// 0x3b0
			 0x48d91915, /// 0x3b4
			 0x9d75e691, /// 0x3b8
			 0x474307d9, /// 0x3bc
			 0xa2915fc7, /// 0x3c0
			 0xbb8a786e, /// 0x3c4
			 0xd98beefb, /// 0x3c8
			 0xadce0b5b, /// 0x3cc
			 0x6473f4f4, /// 0x3d0
			 0xf7171f54, /// 0x3d4
			 0x7ec578b8, /// 0x3d8
			 0xa5a12a5c, /// 0x3dc
			 0x07850f88, /// 0x3e0
			 0x2db96f06, /// 0x3e4
			 0xc40be861, /// 0x3e8
			 0x5694386a, /// 0x3ec
			 0x2179170a, /// 0x3f0
			 0x29d14261, /// 0x3f4
			 0x55e43c97, /// 0x3f8
			 0xa9aae9c5, /// 0x3fc
			 0x9c042b83, /// 0x400
			 0x6b605fcb, /// 0x404
			 0x6cc55425, /// 0x408
			 0x6e2bf5ed, /// 0x40c
			 0xa330be2a, /// 0x410
			 0x269fbed7, /// 0x414
			 0xb90d4db1, /// 0x418
			 0x78086009, /// 0x41c
			 0x6faea075, /// 0x420
			 0x3db9258a, /// 0x424
			 0x77ae4953, /// 0x428
			 0xed9fbc53, /// 0x42c
			 0x12e7a739, /// 0x430
			 0x5bb2b10a, /// 0x434
			 0x57208cde, /// 0x438
			 0x45ac881b, /// 0x43c
			 0x8050d391, /// 0x440
			 0xe15a8f9d, /// 0x444
			 0xf837155a, /// 0x448
			 0x2fb10120, /// 0x44c
			 0xe3738a2c, /// 0x450
			 0xd7d751be, /// 0x454
			 0xf342fa84, /// 0x458
			 0x0ace0280, /// 0x45c
			 0x7c78715b, /// 0x460
			 0xd0166400, /// 0x464
			 0xae95f2c4, /// 0x468
			 0x2ffafede, /// 0x46c
			 0x286a7926, /// 0x470
			 0xf15d6009, /// 0x474
			 0x1f95cee2, /// 0x478
			 0x2f41fc1a, /// 0x47c
			 0x7c6adc6d, /// 0x480
			 0x3e2d8a07, /// 0x484
			 0x2a75f78c, /// 0x488
			 0x2ae5ff9e, /// 0x48c
			 0x2d608062, /// 0x490
			 0x6a12842f, /// 0x494
			 0xbb274b13, /// 0x498
			 0x25133f7d, /// 0x49c
			 0x957cd1cb, /// 0x4a0
			 0x307f1a57, /// 0x4a4
			 0xff962ab5, /// 0x4a8
			 0x6e61e405, /// 0x4ac
			 0x14b624c9, /// 0x4b0
			 0x1a249b2b, /// 0x4b4
			 0x86f01142, /// 0x4b8
			 0x76f8ca30, /// 0x4bc
			 0x48520850, /// 0x4c0
			 0xb917c346, /// 0x4c4
			 0x573ad305, /// 0x4c8
			 0x23415982, /// 0x4cc
			 0x45d1bedf, /// 0x4d0
			 0x5500d9ac, /// 0x4d4
			 0xaf5914af, /// 0x4d8
			 0x9fb4fd8e, /// 0x4dc
			 0xe4f6e4c7, /// 0x4e0
			 0xbffe49f7, /// 0x4e4
			 0x3e942b27, /// 0x4e8
			 0x60f717e2, /// 0x4ec
			 0xa6aedcf3, /// 0x4f0
			 0x78b992bd, /// 0x4f4
			 0x6e0f4d54, /// 0x4f8
			 0xb3bbacbd, /// 0x4fc
			 0x3caf679d, /// 0x500
			 0x1938e810, /// 0x504
			 0x3af35c46, /// 0x508
			 0xa2db40c9, /// 0x50c
			 0x30cefca6, /// 0x510
			 0xec85d30e, /// 0x514
			 0x18dc7c58, /// 0x518
			 0x6210417d, /// 0x51c
			 0xd3a382c0, /// 0x520
			 0x1d05b8ba, /// 0x524
			 0xfddc177f, /// 0x528
			 0xa54d8762, /// 0x52c
			 0xf8d33069, /// 0x530
			 0xb08c5bd1, /// 0x534
			 0x3d2b9fbf, /// 0x538
			 0x57ae7f66, /// 0x53c
			 0x92036876, /// 0x540
			 0xc5d3ef8e, /// 0x544
			 0x16eb4b0a, /// 0x548
			 0x546feb33, /// 0x54c
			 0x63207f44, /// 0x550
			 0x8d05ba71, /// 0x554
			 0x1c911121, /// 0x558
			 0x9de3b174, /// 0x55c
			 0x04d00cda, /// 0x560
			 0x2d61cc9a, /// 0x564
			 0x61ddf670, /// 0x568
			 0x3b479693, /// 0x56c
			 0x93432585, /// 0x570
			 0x44430e9b, /// 0x574
			 0x070de5bd, /// 0x578
			 0x6c34ddbe, /// 0x57c
			 0x6fd971ed, /// 0x580
			 0xf31a5af7, /// 0x584
			 0x03ac752f, /// 0x588
			 0x601a68f7, /// 0x58c
			 0x74553660, /// 0x590
			 0xcc2f4225, /// 0x594
			 0x28c74b98, /// 0x598
			 0x2d4c0f32, /// 0x59c
			 0x01d61bd5, /// 0x5a0
			 0xe129796c, /// 0x5a4
			 0x3d1718e7, /// 0x5a8
			 0x5afe9386, /// 0x5ac
			 0x20bf0ffd, /// 0x5b0
			 0xebc603a7, /// 0x5b4
			 0xa282396c, /// 0x5b8
			 0x42983f17, /// 0x5bc
			 0xc34cb10f, /// 0x5c0
			 0xc51ad892, /// 0x5c4
			 0x7733bcbd, /// 0x5c8
			 0x8f91ddd9, /// 0x5cc
			 0xf32a8ae3, /// 0x5d0
			 0xd4f80d6b, /// 0x5d4
			 0x6c767447, /// 0x5d8
			 0x913c6eba, /// 0x5dc
			 0x7083eebc, /// 0x5e0
			 0x0c718e00, /// 0x5e4
			 0xa7d1fc94, /// 0x5e8
			 0x309a232e, /// 0x5ec
			 0x7e9e0f15, /// 0x5f0
			 0x9ce04510, /// 0x5f4
			 0xcf3589dc, /// 0x5f8
			 0x73b28702, /// 0x5fc
			 0xff1e430a, /// 0x600
			 0x51487165, /// 0x604
			 0x138e364a, /// 0x608
			 0x31498be0, /// 0x60c
			 0xb40a4bfd, /// 0x610
			 0x123ddaf4, /// 0x614
			 0xf98a7fe2, /// 0x618
			 0x5d77b1f7, /// 0x61c
			 0xd291e2cc, /// 0x620
			 0xc08246cf, /// 0x624
			 0xcefa1d88, /// 0x628
			 0x20e43c2e, /// 0x62c
			 0x0914bc1e, /// 0x630
			 0x5134084e, /// 0x634
			 0x2acb3b8d, /// 0x638
			 0x3c3daeef, /// 0x63c
			 0xac99ed23, /// 0x640
			 0x1a438ce0, /// 0x644
			 0x56c9b23a, /// 0x648
			 0x2e00ad6c, /// 0x64c
			 0x51a321f3, /// 0x650
			 0xff98b14a, /// 0x654
			 0xf0795571, /// 0x658
			 0x853c3f86, /// 0x65c
			 0x83756a8e, /// 0x660
			 0xefb270cb, /// 0x664
			 0x83431154, /// 0x668
			 0x8e1539e0, /// 0x66c
			 0x1b2265d1, /// 0x670
			 0x841db142, /// 0x674
			 0x0fff1372, /// 0x678
			 0x5ba4c8f6, /// 0x67c
			 0x1decd698, /// 0x680
			 0x2ba8aa70, /// 0x684
			 0xba81a6f9, /// 0x688
			 0x85b9fb7a, /// 0x68c
			 0x51b59c12, /// 0x690
			 0x3f7b71eb, /// 0x694
			 0x244368bd, /// 0x698
			 0xf5232ecd, /// 0x69c
			 0xa0e0673d, /// 0x6a0
			 0x85916ecd, /// 0x6a4
			 0x1fdd081f, /// 0x6a8
			 0x9ee36961, /// 0x6ac
			 0x6cc2feeb, /// 0x6b0
			 0x3b5f241a, /// 0x6b4
			 0x214556da, /// 0x6b8
			 0x648191e2, /// 0x6bc
			 0x32c43947, /// 0x6c0
			 0xfe9d2823, /// 0x6c4
			 0x39c7dde8, /// 0x6c8
			 0xf537a33e, /// 0x6cc
			 0xca5f257c, /// 0x6d0
			 0x740aab8a, /// 0x6d4
			 0x59ebdf44, /// 0x6d8
			 0xc85ca87c, /// 0x6dc
			 0x47c9381f, /// 0x6e0
			 0x0d3f4f00, /// 0x6e4
			 0x76f723ae, /// 0x6e8
			 0x901cb4a2, /// 0x6ec
			 0x48198596, /// 0x6f0
			 0xbc37d1d0, /// 0x6f4
			 0x8e9f6043, /// 0x6f8
			 0xf5d86b5b, /// 0x6fc
			 0xa9efdf75, /// 0x700
			 0xc34a899f, /// 0x704
			 0x76d04424, /// 0x708
			 0x862278fa, /// 0x70c
			 0xf0400b9b, /// 0x710
			 0x833ee27b, /// 0x714
			 0x50e3e5d1, /// 0x718
			 0x97e37b34, /// 0x71c
			 0xb93bace5, /// 0x720
			 0xce05ad2c, /// 0x724
			 0x6f931292, /// 0x728
			 0x57eb69bd, /// 0x72c
			 0xec1093f1, /// 0x730
			 0x87c9f113, /// 0x734
			 0xa40779db, /// 0x738
			 0x26fd0f61, /// 0x73c
			 0xa98c83d3, /// 0x740
			 0x38551707, /// 0x744
			 0x50f7ad2e, /// 0x748
			 0xd5a94de7, /// 0x74c
			 0xe9b261da, /// 0x750
			 0x6c59e03f, /// 0x754
			 0xa5cf5630, /// 0x758
			 0xc9ea0158, /// 0x75c
			 0xb49d942a, /// 0x760
			 0x2b839d63, /// 0x764
			 0xa148e068, /// 0x768
			 0x6fad27dc, /// 0x76c
			 0xc26c601e, /// 0x770
			 0x50b99e98, /// 0x774
			 0x0808d2bd, /// 0x778
			 0xc5220967, /// 0x77c
			 0xa8b49b5a, /// 0x780
			 0xf3edbb07, /// 0x784
			 0x684f5669, /// 0x788
			 0xddd278af, /// 0x78c
			 0xcf5f3b43, /// 0x790
			 0xbaf6c54e, /// 0x794
			 0x65691eec, /// 0x798
			 0xc51007d6, /// 0x79c
			 0xcfecf16a, /// 0x7a0
			 0x3f021f83, /// 0x7a4
			 0x3c71d2ae, /// 0x7a8
			 0x427f84bd, /// 0x7ac
			 0x9fc4ea83, /// 0x7b0
			 0xf9b921a8, /// 0x7b4
			 0xd33c2024, /// 0x7b8
			 0x133e5ba7, /// 0x7bc
			 0x12d1be1d, /// 0x7c0
			 0x9e1fa9e2, /// 0x7c4
			 0x9073567a, /// 0x7c8
			 0x96417cf5, /// 0x7cc
			 0xc64c46be, /// 0x7d0
			 0xdc63bde9, /// 0x7d4
			 0xb6e994a8, /// 0x7d8
			 0xa7134ff0, /// 0x7dc
			 0xc17be3f1, /// 0x7e0
			 0x32ea04b4, /// 0x7e4
			 0xc7aac866, /// 0x7e8
			 0xc4b3848c, /// 0x7ec
			 0xe67eb7a5, /// 0x7f0
			 0x89cd5c59, /// 0x7f4
			 0xfdd7b01a, /// 0x7f8
			 0xb2dbf306, /// 0x7fc
			 0x9924debc, /// 0x800
			 0xcad929d4, /// 0x804
			 0x285fe9ff, /// 0x808
			 0xd74f0aae, /// 0x80c
			 0xc52c4468, /// 0x810
			 0xda5cf57a, /// 0x814
			 0xc596c52c, /// 0x818
			 0xd1cac77b, /// 0x81c
			 0x61a31581, /// 0x820
			 0x1eb312e5, /// 0x824
			 0x2dc4c1f0, /// 0x828
			 0x9e94382f, /// 0x82c
			 0xb8032fd0, /// 0x830
			 0x057fdef9, /// 0x834
			 0xa348a339, /// 0x838
			 0x3e01c730, /// 0x83c
			 0x9a556e1c, /// 0x840
			 0x0ae63ec3, /// 0x844
			 0xa21051f1, /// 0x848
			 0x3b1151bb, /// 0x84c
			 0xb544270f, /// 0x850
			 0xd1a47ebe, /// 0x854
			 0x9b806625, /// 0x858
			 0x40437374, /// 0x85c
			 0x85019d54, /// 0x860
			 0xfcb3df32, /// 0x864
			 0xdbbf2d9d, /// 0x868
			 0x173ad214, /// 0x86c
			 0x6b437d1b, /// 0x870
			 0x16b50b0b, /// 0x874
			 0x8c390187, /// 0x878
			 0x08ce1b59, /// 0x87c
			 0xc4ecd7e4, /// 0x880
			 0x73ae7fae, /// 0x884
			 0x9a0e788a, /// 0x888
			 0x182388a4, /// 0x88c
			 0xf4b30b0f, /// 0x890
			 0xd1f64db6, /// 0x894
			 0x91666019, /// 0x898
			 0x78472375, /// 0x89c
			 0x8fbec800, /// 0x8a0
			 0x743f45f7, /// 0x8a4
			 0xd87c7b8a, /// 0x8a8
			 0x0af4d204, /// 0x8ac
			 0x1b22f0ff, /// 0x8b0
			 0xeedb68b3, /// 0x8b4
			 0xe6ab6ca3, /// 0x8b8
			 0xa1819cf6, /// 0x8bc
			 0x1597ca44, /// 0x8c0
			 0xe9132271, /// 0x8c4
			 0x6eca52b6, /// 0x8c8
			 0x54a02caa, /// 0x8cc
			 0xae38665c, /// 0x8d0
			 0x4388ec28, /// 0x8d4
			 0xebcf6d8a, /// 0x8d8
			 0x817b8af0, /// 0x8dc
			 0x40fe96bd, /// 0x8e0
			 0x0b2a3497, /// 0x8e4
			 0xf4f538b0, /// 0x8e8
			 0x70ca3bc9, /// 0x8ec
			 0x8dca472c, /// 0x8f0
			 0xc35e75c6, /// 0x8f4
			 0x96015ee4, /// 0x8f8
			 0x46d48163, /// 0x8fc
			 0x77f0b903, /// 0x900
			 0xea58897e, /// 0x904
			 0xf0619292, /// 0x908
			 0xc0f7fd54, /// 0x90c
			 0x60f976d0, /// 0x910
			 0xdb9da172, /// 0x914
			 0xf4f84576, /// 0x918
			 0x1378042a, /// 0x91c
			 0x1d42df5c, /// 0x920
			 0x971ef592, /// 0x924
			 0xcca1516d, /// 0x928
			 0x89d420f9, /// 0x92c
			 0xdf635ea4, /// 0x930
			 0xe5aef09e, /// 0x934
			 0x31640f50, /// 0x938
			 0x72229fe5, /// 0x93c
			 0xd3c0cbca, /// 0x940
			 0x5908120b, /// 0x944
			 0xb87db772, /// 0x948
			 0x54fbd368, /// 0x94c
			 0x88614ff2, /// 0x950
			 0x7b6b84ff, /// 0x954
			 0xba7710e3, /// 0x958
			 0xf2cd23fe, /// 0x95c
			 0x59144e41, /// 0x960
			 0x454fb78f, /// 0x964
			 0x34f117a8, /// 0x968
			 0xacca443e, /// 0x96c
			 0xc7780b7b, /// 0x970
			 0x56757fa1, /// 0x974
			 0x23c1aa2b, /// 0x978
			 0xd0e31a78, /// 0x97c
			 0x96c447ea, /// 0x980
			 0xcf1f1c96, /// 0x984
			 0x2c53e810, /// 0x988
			 0x72eb5af4, /// 0x98c
			 0xeafdb59c, /// 0x990
			 0x888e16c4, /// 0x994
			 0xa4ec4060, /// 0x998
			 0x306f656f, /// 0x99c
			 0x4bf34a42, /// 0x9a0
			 0xc8d57aa7, /// 0x9a4
			 0x082fa043, /// 0x9a8
			 0x0dc26724, /// 0x9ac
			 0xc2d2d9c7, /// 0x9b0
			 0xe071fffc, /// 0x9b4
			 0x2f760820, /// 0x9b8
			 0xba3a91f6, /// 0x9bc
			 0x41524618, /// 0x9c0
			 0x986c95fb, /// 0x9c4
			 0xf39c8389, /// 0x9c8
			 0x027b81c7, /// 0x9cc
			 0xd12fcf33, /// 0x9d0
			 0x9875f3c5, /// 0x9d4
			 0x6d19064a, /// 0x9d8
			 0xfda0b110, /// 0x9dc
			 0x14f68090, /// 0x9e0
			 0x42372ee1, /// 0x9e4
			 0x230528c1, /// 0x9e8
			 0x4194e86a, /// 0x9ec
			 0x8468d8af, /// 0x9f0
			 0xa4fe0637, /// 0x9f4
			 0x1d24ec62, /// 0x9f8
			 0x1fc0ff71, /// 0x9fc
			 0x598d0666, /// 0xa00
			 0x47177a50, /// 0xa04
			 0xe4377f28, /// 0xa08
			 0x016b5ca6, /// 0xa0c
			 0xca06158c, /// 0xa10
			 0x71687d6e, /// 0xa14
			 0x654284cc, /// 0xa18
			 0xc3c3cb4d, /// 0xa1c
			 0xbe22f2c6, /// 0xa20
			 0xa1710f66, /// 0xa24
			 0xdd7225cd, /// 0xa28
			 0x850e229b, /// 0xa2c
			 0xc0c3e599, /// 0xa30
			 0x6be312dd, /// 0xa34
			 0x2684c19b, /// 0xa38
			 0xa85570ab, /// 0xa3c
			 0xe8689aec, /// 0xa40
			 0x4654e225, /// 0xa44
			 0xa5144977, /// 0xa48
			 0x4cb72a2d, /// 0xa4c
			 0x58547878, /// 0xa50
			 0x2599c5da, /// 0xa54
			 0x38ecfa95, /// 0xa58
			 0xb91e69e3, /// 0xa5c
			 0xb4e01ac5, /// 0xa60
			 0xde56717f, /// 0xa64
			 0xff15603e, /// 0xa68
			 0x7c9d45d7, /// 0xa6c
			 0xde4aad22, /// 0xa70
			 0x75f9cf21, /// 0xa74
			 0x579290b5, /// 0xa78
			 0x0a4b30bc, /// 0xa7c
			 0x51c62cec, /// 0xa80
			 0xfaa38e47, /// 0xa84
			 0x6eeff4ef, /// 0xa88
			 0x0121a065, /// 0xa8c
			 0x89ddad6f, /// 0xa90
			 0xd62df0d0, /// 0xa94
			 0x1272f01e, /// 0xa98
			 0xfd93dd95, /// 0xa9c
			 0x5527b9c4, /// 0xaa0
			 0x6e67218e, /// 0xaa4
			 0xacf5cd23, /// 0xaa8
			 0xec119dca, /// 0xaac
			 0xfd0f5386, /// 0xab0
			 0xde4552c4, /// 0xab4
			 0x1a745edc, /// 0xab8
			 0x1e810c1a, /// 0xabc
			 0x641dcafd, /// 0xac0
			 0x48f85698, /// 0xac4
			 0x7d83f778, /// 0xac8
			 0xa0d6761d, /// 0xacc
			 0xbd48557a, /// 0xad0
			 0x7cfa57c8, /// 0xad4
			 0x2337fc09, /// 0xad8
			 0x7cee7b1e, /// 0xadc
			 0xe3bac4d4, /// 0xae0
			 0x67c76454, /// 0xae4
			 0xc6b5dd02, /// 0xae8
			 0xbb0cc3a3, /// 0xaec
			 0x0e8a6329, /// 0xaf0
			 0x61b4efed, /// 0xaf4
			 0x4eb80e80, /// 0xaf8
			 0xd469613b, /// 0xafc
			 0x50cb1e7b, /// 0xb00
			 0x754c18d2, /// 0xb04
			 0x837022e6, /// 0xb08
			 0xbdcc7308, /// 0xb0c
			 0x4a4f8f25, /// 0xb10
			 0x01d7dc09, /// 0xb14
			 0xaef5a49c, /// 0xb18
			 0x87dd8e2c, /// 0xb1c
			 0x7f06e89f, /// 0xb20
			 0xaf14f8f0, /// 0xb24
			 0x6d67c528, /// 0xb28
			 0xb931ebf0, /// 0xb2c
			 0x5a93c513, /// 0xb30
			 0x2eac1494, /// 0xb34
			 0xa097c9cf, /// 0xb38
			 0xee519efa, /// 0xb3c
			 0x7975c95d, /// 0xb40
			 0xf3d7e81a, /// 0xb44
			 0x1242695e, /// 0xb48
			 0x40bec414, /// 0xb4c
			 0xf3922175, /// 0xb50
			 0x10b9a11a, /// 0xb54
			 0x85bc8efd, /// 0xb58
			 0xfa8b138a, /// 0xb5c
			 0x95686efb, /// 0xb60
			 0xd796424d, /// 0xb64
			 0x103db956, /// 0xb68
			 0xbc0fcf99, /// 0xb6c
			 0xf79615ed, /// 0xb70
			 0x3db701a7, /// 0xb74
			 0xc4895084, /// 0xb78
			 0xd60abc6f, /// 0xb7c
			 0x9e907f19, /// 0xb80
			 0x10319268, /// 0xb84
			 0x913aca06, /// 0xb88
			 0xccf651f4, /// 0xb8c
			 0xdf277ca6, /// 0xb90
			 0x86b04a8e, /// 0xb94
			 0xaab7624c, /// 0xb98
			 0xc83d54e4, /// 0xb9c
			 0x9f7554e4, /// 0xba0
			 0x995e1d94, /// 0xba4
			 0xc1bfa25b, /// 0xba8
			 0x5aabc7b6, /// 0xbac
			 0x9b98232a, /// 0xbb0
			 0x8e773a18, /// 0xbb4
			 0x170473ba, /// 0xbb8
			 0xe9537d1e, /// 0xbbc
			 0x526e07ec, /// 0xbc0
			 0xe473a1f0, /// 0xbc4
			 0x35605be8, /// 0xbc8
			 0xa2155179, /// 0xbcc
			 0xaf693729, /// 0xbd0
			 0x2927773f, /// 0xbd4
			 0xb68fd60b, /// 0xbd8
			 0xd876c1dd, /// 0xbdc
			 0x3b29ff15, /// 0xbe0
			 0x6369e368, /// 0xbe4
			 0x87a54130, /// 0xbe8
			 0x98538cdd, /// 0xbec
			 0x394b04f1, /// 0xbf0
			 0x1036b7a3, /// 0xbf4
			 0xe8aeb263, /// 0xbf8
			 0x58749fad, /// 0xbfc
			 0xbae67b57, /// 0xc00
			 0xbd3b4c22, /// 0xc04
			 0x7a7df261, /// 0xc08
			 0x006fc01e, /// 0xc0c
			 0x181d6acf, /// 0xc10
			 0x6a111fad, /// 0xc14
			 0x53919d12, /// 0xc18
			 0x589da092, /// 0xc1c
			 0xc78c7092, /// 0xc20
			 0x5359d0de, /// 0xc24
			 0x635fc994, /// 0xc28
			 0x2bdb8ee4, /// 0xc2c
			 0x9db91237, /// 0xc30
			 0xe7b36124, /// 0xc34
			 0x1acb5823, /// 0xc38
			 0x1d12ddfc, /// 0xc3c
			 0x16a4364f, /// 0xc40
			 0x55a31a43, /// 0xc44
			 0xb05e3355, /// 0xc48
			 0x61f58976, /// 0xc4c
			 0x89c67710, /// 0xc50
			 0x5709bf6f, /// 0xc54
			 0x31562452, /// 0xc58
			 0xf4748c87, /// 0xc5c
			 0xeb932c0c, /// 0xc60
			 0xe88a269d, /// 0xc64
			 0xf5d2cacb, /// 0xc68
			 0xfb318747, /// 0xc6c
			 0xbdc8d680, /// 0xc70
			 0x9ea78256, /// 0xc74
			 0x51880e00, /// 0xc78
			 0x0fa58805, /// 0xc7c
			 0xff49e0f2, /// 0xc80
			 0x6f7680c7, /// 0xc84
			 0x0edb898e, /// 0xc88
			 0x34f7a51e, /// 0xc8c
			 0xb8a89b15, /// 0xc90
			 0xfc9b3e84, /// 0xc94
			 0xf2b040b5, /// 0xc98
			 0x5aa79b5e, /// 0xc9c
			 0x8c6480aa, /// 0xca0
			 0x478f6189, /// 0xca4
			 0x1ffc2084, /// 0xca8
			 0x35b514ec, /// 0xcac
			 0x1342f935, /// 0xcb0
			 0xc6751480, /// 0xcb4
			 0x7834f42a, /// 0xcb8
			 0x9a24d8c0, /// 0xcbc
			 0x7819d047, /// 0xcc0
			 0xedbddaca, /// 0xcc4
			 0x760eaea7, /// 0xcc8
			 0x77140fb3, /// 0xccc
			 0x76967ea8, /// 0xcd0
			 0x636df8c8, /// 0xcd4
			 0x0622db51, /// 0xcd8
			 0x0273e823, /// 0xcdc
			 0x96ce16a9, /// 0xce0
			 0xc7c61a48, /// 0xce4
			 0xfddb1749, /// 0xce8
			 0x32f566c6, /// 0xcec
			 0x07a6730d, /// 0xcf0
			 0x50b00d40, /// 0xcf4
			 0xf955d0e5, /// 0xcf8
			 0x3cc34a67, /// 0xcfc
			 0xd56c3bcd, /// 0xd00
			 0xda6c32c6, /// 0xd04
			 0x2b2ecfbd, /// 0xd08
			 0x038b109c, /// 0xd0c
			 0x21c70328, /// 0xd10
			 0x811fd0ba, /// 0xd14
			 0xc8c74cd8, /// 0xd18
			 0xc99b85ba, /// 0xd1c
			 0xfe5cd478, /// 0xd20
			 0x12dd3133, /// 0xd24
			 0xd309c21c, /// 0xd28
			 0x759460fd, /// 0xd2c
			 0x18464a61, /// 0xd30
			 0xa11c3c55, /// 0xd34
			 0x22766b63, /// 0xd38
			 0xaca8e5da, /// 0xd3c
			 0x55b74b1a, /// 0xd40
			 0xb1afeee1, /// 0xd44
			 0x42f5b5dc, /// 0xd48
			 0x57fc1663, /// 0xd4c
			 0xf22dc56b, /// 0xd50
			 0xdeb7a5e7, /// 0xd54
			 0x8f3285dc, /// 0xd58
			 0x7b8dddea, /// 0xd5c
			 0xe66d5fa7, /// 0xd60
			 0x12e657c8, /// 0xd64
			 0x6dd0e0dc, /// 0xd68
			 0x23a7f6a9, /// 0xd6c
			 0x19c21da1, /// 0xd70
			 0x8fac7290, /// 0xd74
			 0xe6783ace, /// 0xd78
			 0x550542de, /// 0xd7c
			 0xbf66b48c, /// 0xd80
			 0x7412c82b, /// 0xd84
			 0x33522708, /// 0xd88
			 0xf2496121, /// 0xd8c
			 0x721123c3, /// 0xd90
			 0xb9ef2138, /// 0xd94
			 0xafb8389e, /// 0xd98
			 0x9ae227f1, /// 0xd9c
			 0x03109a5c, /// 0xda0
			 0x3c7b8d7a, /// 0xda4
			 0x660ed48c, /// 0xda8
			 0x564c8ccb, /// 0xdac
			 0xfa9d20d9, /// 0xdb0
			 0x37907022, /// 0xdb4
			 0x82a05c49, /// 0xdb8
			 0xe01f5afd, /// 0xdbc
			 0xfa3a2399, /// 0xdc0
			 0xf5cd0eb1, /// 0xdc4
			 0x4a360695, /// 0xdc8
			 0x6c3187d4, /// 0xdcc
			 0xa9705524, /// 0xdd0
			 0xd332c8bb, /// 0xdd4
			 0xcf14a2e4, /// 0xdd8
			 0x13bb9f65, /// 0xddc
			 0x1d50c41f, /// 0xde0
			 0x5dc5ba1a, /// 0xde4
			 0x0ab709d1, /// 0xde8
			 0x2d8977d3, /// 0xdec
			 0xd8a98522, /// 0xdf0
			 0x1d5376ba, /// 0xdf4
			 0x159ed2e4, /// 0xdf8
			 0x6448aad3, /// 0xdfc
			 0xf9b18635, /// 0xe00
			 0xec433275, /// 0xe04
			 0xdfb2e22f, /// 0xe08
			 0x0e73048c, /// 0xe0c
			 0x8daa176d, /// 0xe10
			 0xb13958a4, /// 0xe14
			 0xc05feaa7, /// 0xe18
			 0x2f564b4d, /// 0xe1c
			 0xc0e15fb4, /// 0xe20
			 0x91b05b55, /// 0xe24
			 0xbac32533, /// 0xe28
			 0xfde7505e, /// 0xe2c
			 0x85d1c42e, /// 0xe30
			 0xf16c09db, /// 0xe34
			 0x4e572869, /// 0xe38
			 0x719df54f, /// 0xe3c
			 0x714f41e3, /// 0xe40
			 0x5890d929, /// 0xe44
			 0xb6b42ccb, /// 0xe48
			 0xee6003eb, /// 0xe4c
			 0x7a4e7592, /// 0xe50
			 0xe4c56181, /// 0xe54
			 0x89dd70b2, /// 0xe58
			 0x36e6c917, /// 0xe5c
			 0xd1bbde55, /// 0xe60
			 0x584a3e37, /// 0xe64
			 0x8da9f88f, /// 0xe68
			 0xf57f049c, /// 0xe6c
			 0xb876bf42, /// 0xe70
			 0x6ec0a3e1, /// 0xe74
			 0xc98a989e, /// 0xe78
			 0x46174d98, /// 0xe7c
			 0x62e2c3ab, /// 0xe80
			 0x2c4fb1c0, /// 0xe84
			 0xbd7aa5d1, /// 0xe88
			 0x7653fb0d, /// 0xe8c
			 0xf3de497c, /// 0xe90
			 0x8c0cf790, /// 0xe94
			 0x044bea8b, /// 0xe98
			 0x910696f1, /// 0xe9c
			 0x9933d0ff, /// 0xea0
			 0xf4d3fd79, /// 0xea4
			 0xda9e74c7, /// 0xea8
			 0x9a84ca04, /// 0xeac
			 0xad07c041, /// 0xeb0
			 0xc8be3510, /// 0xeb4
			 0xfc228642, /// 0xeb8
			 0x8e176e17, /// 0xebc
			 0x21011fde, /// 0xec0
			 0x250ea190, /// 0xec4
			 0x0e8b17d3, /// 0xec8
			 0xad332dd7, /// 0xecc
			 0x647d9cef, /// 0xed0
			 0xad3d7d91, /// 0xed4
			 0xee874347, /// 0xed8
			 0xfcefc955, /// 0xedc
			 0x84b87033, /// 0xee0
			 0x640eb88b, /// 0xee4
			 0x8ce8376e, /// 0xee8
			 0x8e41cd36, /// 0xeec
			 0x10b36f96, /// 0xef0
			 0xf25c21a8, /// 0xef4
			 0x79aab0fe, /// 0xef8
			 0xff84191e, /// 0xefc
			 0xf6f3f98d, /// 0xf00
			 0xc940d8ea, /// 0xf04
			 0x05cb9939, /// 0xf08
			 0x1c94f0e8, /// 0xf0c
			 0x79e3a03d, /// 0xf10
			 0xcc877e5d, /// 0xf14
			 0x19f1b8b5, /// 0xf18
			 0xa5866033, /// 0xf1c
			 0x648aa362, /// 0xf20
			 0x23d49961, /// 0xf24
			 0x9f8a2351, /// 0xf28
			 0x8d027c4d, /// 0xf2c
			 0x72fae04b, /// 0xf30
			 0xa086aac6, /// 0xf34
			 0x9eb1e059, /// 0xf38
			 0xea32ef1d, /// 0xf3c
			 0x11aa8569, /// 0xf40
			 0xab4f4cd1, /// 0xf44
			 0x755cec35, /// 0xf48
			 0xfbc879f5, /// 0xf4c
			 0x199e8236, /// 0xf50
			 0xe8db1b8e, /// 0xf54
			 0x6e35edbf, /// 0xf58
			 0xd532d6e0, /// 0xf5c
			 0xe45d3b4f, /// 0xf60
			 0x4fffd57e, /// 0xf64
			 0x391ebcaa, /// 0xf68
			 0xf49321b4, /// 0xf6c
			 0xa29ec81c, /// 0xf70
			 0x3ae4d27f, /// 0xf74
			 0x6b92a39c, /// 0xf78
			 0x943901e2, /// 0xf7c
			 0x787eef40, /// 0xf80
			 0xdd87f2e4, /// 0xf84
			 0x35d106b5, /// 0xf88
			 0xd6805662, /// 0xf8c
			 0x5ac3ac82, /// 0xf90
			 0x7e673497, /// 0xf94
			 0xfa92366c, /// 0xf98
			 0xfe5b05fc, /// 0xf9c
			 0x81c1645f, /// 0xfa0
			 0x51e9ed5e, /// 0xfa4
			 0xcf89f717, /// 0xfa8
			 0xfaf5b948, /// 0xfac
			 0xc44ef9a8, /// 0xfb0
			 0x4347f997, /// 0xfb4
			 0x545bc61b, /// 0xfb8
			 0x954dbd37, /// 0xfbc
			 0x1ca31df5, /// 0xfc0
			 0xc6b244b0, /// 0xfc4
			 0xb3e8db71, /// 0xfc8
			 0x9809e5f7, /// 0xfcc
			 0xf7baaaa7, /// 0xfd0
			 0x1c54550e, /// 0xfd4
			 0xe5eae2e1, /// 0xfd8
			 0x479d4d72, /// 0xfdc
			 0xbb8d7a8d, /// 0xfe0
			 0x9adb15c8, /// 0xfe4
			 0x66f05d99, /// 0xfe8
			 0xf09e66e9, /// 0xfec
			 0x8979cb29, /// 0xff0
			 0xddd01bb0, /// 0xff4
			 0x679d9740, /// 0xff8
			 0x3623f9bf /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00000
			 0x7f800000,                                                  // inf                                         /// 00004
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00008
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0000c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00010
			 0x33333333,                                                  // 4 random values                             /// 00014
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00018
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0001c
			 0x33333333,                                                  // 4 random values                             /// 00020
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00024
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0002c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00030
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00034
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00038
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0003c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x33333333,                                                  // 4 random values                             /// 00048
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00050
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00054
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00058
			 0x0c400000,                                                  // Leading 1s:                                 /// 0005c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00060
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00064
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00068
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0006c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00070
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00074
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00078
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00080
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00088
			 0x0c700000,                                                  // Leading 1s:                                 /// 0008c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00098
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0009c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x7f800000,                                                  // inf                                         /// 00104
			 0x00011111,                                                  // 9.7958E-41                                  /// 00108
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0010c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00110
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00114
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00120
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00124
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00130
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00134
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00138
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00144
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00148
			 0x33333333,                                                  // 4 random values                             /// 0014c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x3f028f5c,                                                  // 0.51                                        /// 00154
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00158
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0015c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00160
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00164
			 0xbf028f5c,                                                  // -0.51                                       /// 00168
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00170
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00174
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00178
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0xffc00001,                                                  // -signaling NaN                              /// 00188
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0018c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00194
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0xffc00001,                                                  // -signaling NaN                              /// 0019c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00210
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00214
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00218
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0021c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00224
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x55555555,                                                  // 4 random values                             /// 0022c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00230
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x0c780000,                                                  // Leading 1s:                                 /// 00238
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00240
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00248
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00250
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00260
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00268
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00270
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00274
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0027c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00280
			 0xffc00001,                                                  // -signaling NaN                              /// 00284
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0028c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00294
			 0x55555555,                                                  // 4 random values                             /// 00298
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0029c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002dc
			 0xbf028f5c,                                                  // -0.51                                       /// 002e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 002f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00300
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00308
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0030c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00310
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00314
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00318
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0031c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00320
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00324
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00328
			 0x0e000001,                                                  // Trailing 1s:                                /// 0032c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00330
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00334
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00344
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00348
			 0x0e000001,                                                  // Trailing 1s:                                /// 0034c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00350
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00354
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00358
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0035c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00364
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00368
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0036c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00370
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00374
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00378
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00384
			 0x00011111,                                                  // 9.7958E-41                                  /// 00388
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00394
			 0x00000000,                                                  // zero                                        /// 00398
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0039c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0xffc00001,                                                  // -signaling NaN                              /// 003ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003f8
			 0x7f800000,                                                  // inf                                         /// 003fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00404
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00408
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00410
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00420
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0042c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00430
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0e000001,                                                  // Trailing 1s:                                /// 00438
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0043c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0044c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00450
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00454
			 0x0e000007,                                                  // Trailing 1s:                                /// 00458
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0045c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00460
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00464
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00468
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00470
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00474
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0047c
			 0xbf028f5c,                                                  // -0.51                                       /// 00480
			 0xbf028f5c,                                                  // -0.51                                       /// 00484
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00488
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0048c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00490
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00494
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00498
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x7fc00001,                                                  // signaling NaN                               /// 004a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 004b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 004b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00500
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00508
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00510
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00514
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00518
			 0x0c600000,                                                  // Leading 1s:                                 /// 0051c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00520
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00524
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0052c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00530
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0053c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00540
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00544
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0054c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0e000007,                                                  // Trailing 1s:                                /// 00554
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00558
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0055c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00564
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0056c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00570
			 0x80011111,                                                  // -9.7958E-41                                 /// 00574
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0057c
			 0xff800000,                                                  // -inf                                        /// 00580
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00588
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0058c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00590
			 0x0c780000,                                                  // Leading 1s:                                 /// 00594
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00598
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0059c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00600
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00604
			 0xcb000000,                                                  // -8388608.0                                  /// 00608
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0060c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0xbf028f5c,                                                  // -0.51                                       /// 0061c
			 0xff800000,                                                  // -inf                                        /// 00620
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x4b000000,                                                  // 8388608.0                                   /// 00630
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00634
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00638
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00640
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00648
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00650
			 0x33333333,                                                  // 4 random values                             /// 00654
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00660
			 0x4b000000,                                                  // 8388608.0                                   /// 00664
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00668
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00674
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00678
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00680
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0068c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00690
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00694
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0xbf028f5c,                                                  // -0.51                                       /// 006dc
			 0x7fc00001,                                                  // signaling NaN                               /// 006e0
			 0xff800000,                                                  // -inf                                        /// 006e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00700
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00704
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00714
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00718
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x55555555,                                                  // 4 random values                             /// 00720
			 0xffc00001,                                                  // -signaling NaN                              /// 00724
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00728
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0072c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00734
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00738
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0073c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00740
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00744
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00748
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00754
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00758
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00760
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00768
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0076c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00770
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00774
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00778
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0077c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00780
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00784
			 0x00011111,                                                  // 9.7958E-41                                  /// 00788
			 0xbf028f5c,                                                  // -0.51                                       /// 0078c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00798
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 007e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007e8
			 0xff800000,                                                  // -inf                                        /// 007ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0080c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00810
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00814
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0081c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00820
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00824
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00828
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00834
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0083c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00840
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00844
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00848
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00850
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x4b000000,                                                  // 8388608.0                                   /// 00858
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00860
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00864
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00868
			 0x0c780000,                                                  // Leading 1s:                                 /// 0086c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00870
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00874
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0087c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00880
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00884
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00888
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x55555555,                                                  // 4 random values                             /// 00890
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00898
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0089c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008d4
			 0x4b000000,                                                  // 8388608.0                                   /// 008d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x33333333,                                                  // 4 random values                             /// 008e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008e8
			 0x00000000,                                                  // zero                                        /// 008ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00900
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0090c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00910
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00914
			 0x0c600000,                                                  // Leading 1s:                                 /// 00918
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0091c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00920
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00924
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00928
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0092c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00930
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00934
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00938
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0093c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00944
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00948
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0094c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00950
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00958
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0095c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00968
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00974
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00978
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0097c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00980
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00984
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00988
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x7f800000,                                                  // inf                                         /// 00990
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00994
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00998
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0099c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x3f028f5c,                                                  // 0.51                                        /// 009ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009e8
			 0xcb000000,                                                  // -8388608.0                                  /// 009ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a18
			 0xcb000000,                                                  // -8388608.0                                  /// 00a1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a28
			 0xff800000,                                                  // -inf                                        /// 00a2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x55555555,                                                  // 4 random values                             /// 00a34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a40
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x80000000,                                                  // -zero                                       /// 00a58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a7c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a88
			 0xcb000000,                                                  // -8388608.0                                  /// 00a8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00aa0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aa4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aa8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ab0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ab4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ab8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00abc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ac4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00acc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ad0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00adc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ae0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ae4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00af0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00af4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00af8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00afc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b08
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b24
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b34
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b84
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ba8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bb4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bb8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bc4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bc8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bcc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bd0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00be0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00be4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00be8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bf4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bfc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c14
			 0x7fc00001,                                                  // signaling NaN                               /// 00c18
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c5c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0xff800000,                                                  // -inf                                        /// 00c6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ca4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ca8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cb4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cbc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ccc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cdc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x7f800000,                                                  // inf                                         /// 00ce4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ce8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cf8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cfc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d08
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d10
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x7fc00001,                                                  // signaling NaN                               /// 00d44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d60
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d78
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d7c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d94
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00da0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00da4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00da8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00db0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00db4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00db8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dcc
			 0xff800000,                                                  // -inf                                        /// 00dd0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ddc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dfc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e10
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e20
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e34
			 0x7f800000,                                                  // inf                                         /// 00e38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e40
			 0x80000000,                                                  // -zero                                       /// 00e44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e54
			 0xff800000,                                                  // -inf                                        /// 00e58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e7c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x7fc00001,                                                  // signaling NaN                               /// 00e84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e88
			 0xff800000,                                                  // -inf                                        /// 00e8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ea0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ea4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00eb0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00eb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb8
			 0x7fc00001,                                                  // signaling NaN                               /// 00ebc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ec4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ec8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ecc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ed0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00edc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ee8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ef0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00efc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f18
			 0x00000000,                                                  // zero                                        /// 00f1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x7fc00001,                                                  // signaling NaN                               /// 00f40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f64
			 0x4b000000,                                                  // 8388608.0                                   /// 00f68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f80
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f8c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fa0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fa8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fb8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x7f800000,                                                  // inf                                         /// 00fc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fcc
			 0x55555555,                                                  // 4 random values                             /// 00fd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fd4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fd8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fdc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fe0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fe4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ff0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ff4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x7fffffff // QNan                                          // SP +ve numbers                              /// last
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
			 0x000000bc,
			 0x000003a4,
			 0x00000140,
			 0x0000063c,
			 0x000000c8,
			 0x000000b0,
			 0x00000584,

			 /// vpu register f2
			 0x40000000,
			 0x41980000,
			 0x3f800000,
			 0x41100000,
			 0x41600000,
			 0x41800000,
			 0x41b80000,
			 0x41700000,

			 /// vpu register f3
			 0x41980000,
			 0x41f80000,
			 0x40e00000,
			 0x40400000,
			 0x41800000,
			 0x3f800000,
			 0x41000000,
			 0x41880000,

			 /// vpu register f4
			 0x41700000,
			 0x41400000,
			 0x41a80000,
			 0x41800000,
			 0x41500000,
			 0x41100000,
			 0x42000000,
			 0x41d80000,

			 /// vpu register f5
			 0x40400000,
			 0x40000000,
			 0x41d80000,
			 0x40800000,
			 0x41b80000,
			 0x41100000,
			 0x40c00000,
			 0x41c00000,

			 /// vpu register f6
			 0x41e80000,
			 0x41900000,
			 0x3f800000,
			 0x40e00000,
			 0x40400000,
			 0x41600000,
			 0x41d00000,
			 0x41500000,

			 /// vpu register f7
			 0x41300000,
			 0x41600000,
			 0x40e00000,
			 0x41880000,
			 0x40e00000,
			 0x41f80000,
			 0x41200000,
			 0x41c80000,

			 /// vpu register f8
			 0x41a80000,
			 0x41880000,
			 0x41800000,
			 0x41000000,
			 0x41a80000,
			 0x41a80000,
			 0x41200000,
			 0x41a00000,

			 /// vpu register f9
			 0x41c80000,
			 0x41100000,
			 0x41900000,
			 0x40400000,
			 0x41000000,
			 0x41800000,
			 0x41100000,
			 0x41100000,

			 /// vpu register f10
			 0x41a00000,
			 0x41880000,
			 0x41b80000,
			 0x41c00000,
			 0x42000000,
			 0x41200000,
			 0x42000000,
			 0x41b80000,

			 /// vpu register f11
			 0x41000000,
			 0x41c00000,
			 0x40000000,
			 0x41b80000,
			 0x41f00000,
			 0x41f80000,
			 0x41c80000,
			 0x41400000,

			 /// vpu register f12
			 0x41f80000,
			 0x41b80000,
			 0x42000000,
			 0x41a80000,
			 0x40e00000,
			 0x41980000,
			 0x41b80000,
			 0x41880000,

			 /// vpu register f13
			 0x41d80000,
			 0x41200000,
			 0x41980000,
			 0x41500000,
			 0x41c80000,
			 0x41200000,
			 0x40800000,
			 0x41900000,

			 /// vpu register f14
			 0x41980000,
			 0x40000000,
			 0x41000000,
			 0x40000000,
			 0x41f80000,
			 0x41800000,
			 0x41f00000,
			 0x41c00000,

			 /// vpu register f15
			 0x40c00000,
			 0x41f00000,
			 0x40a00000,
			 0x41d80000,
			 0x41c80000,
			 0x41e80000,
			 0x40800000,
			 0x41a80000,

			 /// vpu register f16
			 0x41600000,
			 0x41000000,
			 0x41600000,
			 0x40e00000,
			 0x41500000,
			 0x41880000,
			 0x40a00000,
			 0x41d00000,

			 /// vpu register f17
			 0x40e00000,
			 0x41600000,
			 0x41600000,
			 0x40000000,
			 0x41500000,
			 0x41700000,
			 0x41e00000,
			 0x41980000,

			 /// vpu register f18
			 0x41500000,
			 0x41300000,
			 0x41900000,
			 0x40800000,
			 0x41900000,
			 0x41700000,
			 0x40800000,
			 0x40c00000,

			 /// vpu register f19
			 0x41800000,
			 0x41c00000,
			 0x41c00000,
			 0x41e80000,
			 0x41600000,
			 0x41a80000,
			 0x41e80000,
			 0x41700000,

			 /// vpu register f20
			 0x41600000,
			 0x41e80000,
			 0x41980000,
			 0x41000000,
			 0x41200000,
			 0x40800000,
			 0x41e80000,
			 0x41e80000,

			 /// vpu register f21
			 0x41f00000,
			 0x41600000,
			 0x41300000,
			 0x40a00000,
			 0x41500000,
			 0x41d80000,
			 0x41000000,
			 0x41600000,

			 /// vpu register f22
			 0x41100000,
			 0x41980000,
			 0x41c00000,
			 0x41f80000,
			 0x41200000,
			 0x41c80000,
			 0x40c00000,
			 0x41100000,

			 /// vpu register f23
			 0x41700000,
			 0x41000000,
			 0x40c00000,
			 0x41f80000,
			 0x40000000,
			 0x41b80000,
			 0x41f00000,
			 0x3f800000,

			 /// vpu register f24
			 0x40800000,
			 0x40a00000,
			 0x41900000,
			 0x41f00000,
			 0x40400000,
			 0x41a80000,
			 0x41d80000,
			 0x40a00000,

			 /// vpu register f25
			 0x41c00000,
			 0x41600000,
			 0x42000000,
			 0x41e80000,
			 0x41c00000,
			 0x40000000,
			 0x41900000,
			 0x40a00000,

			 /// vpu register f26
			 0x41880000,
			 0x41880000,
			 0x41e80000,
			 0x40800000,
			 0x41d00000,
			 0x41000000,
			 0x41f00000,
			 0x40000000,

			 /// vpu register f27
			 0x42000000,
			 0x40000000,
			 0x41c00000,
			 0x41c80000,
			 0x41600000,
			 0x41d00000,
			 0x41100000,
			 0x41c00000,

			 /// vpu register f28
			 0x41900000,
			 0x41900000,
			 0x40000000,
			 0x41a00000,
			 0x41e80000,
			 0x41600000,
			 0x41880000,
			 0x41100000,

			 /// vpu register f29
			 0x41b00000,
			 0x41d80000,
			 0x41b00000,
			 0x41000000,
			 0x41800000,
			 0x41700000,
			 0x42000000,
			 0x41f00000,

			 /// vpu register f30
			 0x3f800000,
			 0x41800000,
			 0x40c00000,
			 0x41880000,
			 0x41300000,
			 0x41b00000,
			 0x41700000,
			 0x41b80000,

			 /// vpu register f31
			 0x3f800000,
			 0x41100000,
			 0x41a00000,
			 0x40800000,
			 0x3f800000,
			 0x41800000,
			 0x41f00000,
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
		"famoaddg.pi f16, f1 (x15)\n"                         ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f21, f1 (x15)\n"                         ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f9, f1 (x13)\n"                          ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f21, f1 (x11)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f6, f1 (x12)\n"                          ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f18, f1 (x15)\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f21, f1 (x14)\n"                         ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f20, f1 (x15)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f27, f1 (x11)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f17, f1 (x15)\n"                         ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f9, f1 (x13)\n"                          ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f17, f1 (x13)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f9, f1 (x13)\n"                          ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f13, f1 (x12)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f20, f1 (x13)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f29, f1 (x15)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f2, f1 (x14)\n"                          ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f29, f1 (x13)\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f15, f1 (x15)\n"                         ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f14, f1 (x13)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f15, f1 (x14)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f22, f1 (x15)\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f15, f1 (x13)\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f21, f1 (x14)\n"                         ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f6, f1 (x11)\n"                          ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f22, f1 (x14)\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f23, f1 (x14)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f30, f1 (x13)\n"                         ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f7, f1 (x15)\n"                          ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f19, f1 (x13)\n"                         ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f9, f1 (x15)\n"                          ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f21, f1 (x12)\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f5, f1 (x15)\n"                          ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f20, f1 (x11)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f8, f1 (x12)\n"                          ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f20, f1 (x14)\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f27, f1 (x11)\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f13, f1 (x13)\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f19, f1 (x12)\n"                         ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f23, f1 (x13)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f27, f1 (x13)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f21, f1 (x13)\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f27, f1 (x14)\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f24, f1 (x13)\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f14, f1 (x13)\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f28, f1 (x14)\n"                         ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f21, f1 (x15)\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f13, f1 (x15)\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f25, f1 (x12)\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f14, f1 (x15)\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f6, f1 (x13)\n"                          ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f10, f1 (x12)\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f13, f1 (x13)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f4, f1 (x15)\n"                          ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f23, f1 (x15)\n"                         ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f17, f1 (x12)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f16, f1 (x11)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f24, f1 (x13)\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f7, f1 (x12)\n"                          ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f17, f1 (x11)\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f13, f1 (x15)\n"                         ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f30, f1 (x14)\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f7, f1 (x11)\n"                          ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f17, f1 (x12)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f30, f1 (x11)\n"                         ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f2, f1 (x12)\n"                          ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f27, f1 (x13)\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f25, f1 (x12)\n"                         ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f5, f1 (x13)\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f24, f1 (x15)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f17, f1 (x12)\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f29, f1 (x11)\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f23, f1 (x12)\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f19, f1 (x13)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f3, f1 (x14)\n"                          ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f6, f1 (x13)\n"                          ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f13, f1 (x15)\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f8, f1 (x12)\n"                          ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f5, f1 (x15)\n"                          ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f8, f1 (x14)\n"                          ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f15, f1 (x12)\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f21, f1 (x11)\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f17, f1 (x13)\n"                         ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f23, f1 (x13)\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f13, f1 (x13)\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f4, f1 (x15)\n"                          ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f26, f1 (x11)\n"                         ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f11, f1 (x12)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f21, f1 (x14)\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f20, f1 (x11)\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f8, f1 (x11)\n"                          ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f11, f1 (x11)\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f7, f1 (x15)\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f22, f1 (x11)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f20, f1 (x11)\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f21, f1 (x15)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f12, f1 (x14)\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f13, f1 (x13)\n"                         ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddg.pi f27, f1 (x11)\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
