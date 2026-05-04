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
			 0x07abb276, /// 0x0
			 0x952e38bb, /// 0x4
			 0x7fdc3ccf, /// 0x8
			 0x23083ccd, /// 0xc
			 0x0bf4fc49, /// 0x10
			 0x130ed12f, /// 0x14
			 0x40734b72, /// 0x18
			 0x27cd3381, /// 0x1c
			 0x8b31b8a5, /// 0x20
			 0xe1eb010c, /// 0x24
			 0x27f545c1, /// 0x28
			 0x00914b06, /// 0x2c
			 0xe31080fa, /// 0x30
			 0xc342b97b, /// 0x34
			 0xbcf8d0c6, /// 0x38
			 0xb7286cab, /// 0x3c
			 0x1879f34d, /// 0x40
			 0x9c1ed6a5, /// 0x44
			 0x7b1c68dc, /// 0x48
			 0xd5a12457, /// 0x4c
			 0xf994d27e, /// 0x50
			 0xeaecdf35, /// 0x54
			 0xa1ccdf07, /// 0x58
			 0xba77cd2f, /// 0x5c
			 0xd8f57314, /// 0x60
			 0x648c85ea, /// 0x64
			 0xafa20d7d, /// 0x68
			 0xca993207, /// 0x6c
			 0x31cac562, /// 0x70
			 0x2d7dc818, /// 0x74
			 0xd2b20999, /// 0x78
			 0x7718d856, /// 0x7c
			 0x8d496515, /// 0x80
			 0x3d0be9fb, /// 0x84
			 0x01830da7, /// 0x88
			 0x418cfd2b, /// 0x8c
			 0xbf2bbbff, /// 0x90
			 0xcb626178, /// 0x94
			 0x4b2291e1, /// 0x98
			 0xb72e363c, /// 0x9c
			 0xca54d914, /// 0xa0
			 0xd683eddf, /// 0xa4
			 0x57d69089, /// 0xa8
			 0xc58ba05a, /// 0xac
			 0x20d9af14, /// 0xb0
			 0x6821fb89, /// 0xb4
			 0x6ea6ca01, /// 0xb8
			 0xd0f288ec, /// 0xbc
			 0xd1445aff, /// 0xc0
			 0x68abeb17, /// 0xc4
			 0x6553533c, /// 0xc8
			 0xb2b627d1, /// 0xcc
			 0x43510f40, /// 0xd0
			 0xd69ede65, /// 0xd4
			 0xf95a7ba1, /// 0xd8
			 0x3526248b, /// 0xdc
			 0x9ece53d9, /// 0xe0
			 0x9fe2466c, /// 0xe4
			 0x39656051, /// 0xe8
			 0x5cddbb76, /// 0xec
			 0xdbbaa80e, /// 0xf0
			 0x4ae8c909, /// 0xf4
			 0x097ac7df, /// 0xf8
			 0xb95bb049, /// 0xfc
			 0xd3203d64, /// 0x100
			 0xb6450e96, /// 0x104
			 0x0affe88e, /// 0x108
			 0xf72b4e9a, /// 0x10c
			 0x2ad12af1, /// 0x110
			 0x7eeeb0f6, /// 0x114
			 0x908110ba, /// 0x118
			 0x07530ed1, /// 0x11c
			 0xdac66a8c, /// 0x120
			 0xfd9d32dd, /// 0x124
			 0x9cf834d3, /// 0x128
			 0xddd0758e, /// 0x12c
			 0x8312a8c3, /// 0x130
			 0xc417a1c3, /// 0x134
			 0xcb41d9f1, /// 0x138
			 0x055eaa8d, /// 0x13c
			 0x59883bb3, /// 0x140
			 0x9876a13c, /// 0x144
			 0x067a7353, /// 0x148
			 0xc185679b, /// 0x14c
			 0x7d62f52a, /// 0x150
			 0x1853fa27, /// 0x154
			 0x8396c818, /// 0x158
			 0x0422d440, /// 0x15c
			 0xc7d35abf, /// 0x160
			 0xc0a7fe19, /// 0x164
			 0x617b4de8, /// 0x168
			 0xe1e659de, /// 0x16c
			 0xbff2a3ed, /// 0x170
			 0x97204f2d, /// 0x174
			 0xb70b8737, /// 0x178
			 0xd781f61f, /// 0x17c
			 0x5720894a, /// 0x180
			 0xf35e3a29, /// 0x184
			 0x88181118, /// 0x188
			 0xfee34c38, /// 0x18c
			 0x0f974a0f, /// 0x190
			 0x2c5df6c5, /// 0x194
			 0x1ae49fdb, /// 0x198
			 0xafc37138, /// 0x19c
			 0xf11338ac, /// 0x1a0
			 0x6d1544fb, /// 0x1a4
			 0x56af80dc, /// 0x1a8
			 0xdc8f4334, /// 0x1ac
			 0xf85eac75, /// 0x1b0
			 0xa8c1044a, /// 0x1b4
			 0xf84ff75d, /// 0x1b8
			 0x58082fd2, /// 0x1bc
			 0xe64131cb, /// 0x1c0
			 0xe2f27c92, /// 0x1c4
			 0x086f20e8, /// 0x1c8
			 0x5e734b93, /// 0x1cc
			 0x49139038, /// 0x1d0
			 0x56ac8cf7, /// 0x1d4
			 0xb1bf0324, /// 0x1d8
			 0x8f296307, /// 0x1dc
			 0x12631de0, /// 0x1e0
			 0x51333e81, /// 0x1e4
			 0xbd4772db, /// 0x1e8
			 0x8f01076a, /// 0x1ec
			 0x424f970e, /// 0x1f0
			 0xd8c9a26c, /// 0x1f4
			 0xcbaba6d8, /// 0x1f8
			 0x29a9bf31, /// 0x1fc
			 0x086057c9, /// 0x200
			 0x9ab10413, /// 0x204
			 0xb1d9cbe4, /// 0x208
			 0xeeed32ff, /// 0x20c
			 0x9d7b61c6, /// 0x210
			 0x3a47ef5f, /// 0x214
			 0x1c149ba7, /// 0x218
			 0x3892eed7, /// 0x21c
			 0xf1ffbd35, /// 0x220
			 0xc66461b6, /// 0x224
			 0xc07ab185, /// 0x228
			 0x215d74ef, /// 0x22c
			 0xf1cd7b5a, /// 0x230
			 0x762d5082, /// 0x234
			 0xc8b1ab3f, /// 0x238
			 0xe2cf87aa, /// 0x23c
			 0x02aaf4d2, /// 0x240
			 0x19bde04a, /// 0x244
			 0xdd10042a, /// 0x248
			 0x11a6a63f, /// 0x24c
			 0x69e2e0ba, /// 0x250
			 0xc68500fc, /// 0x254
			 0xbb23dc68, /// 0x258
			 0x7133b88d, /// 0x25c
			 0x68166717, /// 0x260
			 0x9f836601, /// 0x264
			 0x34d5e63f, /// 0x268
			 0x9685ed2f, /// 0x26c
			 0x610c7660, /// 0x270
			 0x89288ad3, /// 0x274
			 0x9505e79b, /// 0x278
			 0xf9e2a515, /// 0x27c
			 0x6879dd52, /// 0x280
			 0x91680fc6, /// 0x284
			 0x988cf06c, /// 0x288
			 0xd56d8c42, /// 0x28c
			 0xd0323030, /// 0x290
			 0x05824fc9, /// 0x294
			 0x944c3ece, /// 0x298
			 0xf3d623b3, /// 0x29c
			 0x6639e1fc, /// 0x2a0
			 0xb7214c28, /// 0x2a4
			 0x368a30b6, /// 0x2a8
			 0xd3826185, /// 0x2ac
			 0xf7fc55e4, /// 0x2b0
			 0x4eb9ab77, /// 0x2b4
			 0x0d2e91d0, /// 0x2b8
			 0xff409b49, /// 0x2bc
			 0xabf41a04, /// 0x2c0
			 0x9f559a72, /// 0x2c4
			 0xf86b6c52, /// 0x2c8
			 0x0be76323, /// 0x2cc
			 0x23916d46, /// 0x2d0
			 0x9afdc0e9, /// 0x2d4
			 0x1209486b, /// 0x2d8
			 0x60e710a5, /// 0x2dc
			 0x78314f7b, /// 0x2e0
			 0x8411cfb6, /// 0x2e4
			 0xd630de0a, /// 0x2e8
			 0xc27cf8cd, /// 0x2ec
			 0xa5af2a37, /// 0x2f0
			 0xb0125a33, /// 0x2f4
			 0x937d4360, /// 0x2f8
			 0xfcae6105, /// 0x2fc
			 0x53e8c1fe, /// 0x300
			 0xfbf7f176, /// 0x304
			 0x575c3381, /// 0x308
			 0x17386123, /// 0x30c
			 0x9dec44a2, /// 0x310
			 0xb22e8bf2, /// 0x314
			 0xe49e0f73, /// 0x318
			 0xb69f3502, /// 0x31c
			 0xeef1e834, /// 0x320
			 0xdf0ccfd6, /// 0x324
			 0xce335470, /// 0x328
			 0xe6db52f1, /// 0x32c
			 0x484b679f, /// 0x330
			 0x8c3d5920, /// 0x334
			 0x685c2cf1, /// 0x338
			 0xedd56024, /// 0x33c
			 0x1c4c5f27, /// 0x340
			 0xb326cbeb, /// 0x344
			 0x65b61001, /// 0x348
			 0x38fcbc11, /// 0x34c
			 0xe90163c2, /// 0x350
			 0x2559a015, /// 0x354
			 0x6c229cdd, /// 0x358
			 0x420e1035, /// 0x35c
			 0x16959ba4, /// 0x360
			 0x82fa5f54, /// 0x364
			 0x4f358485, /// 0x368
			 0xe5ab4765, /// 0x36c
			 0x7eeaf5f7, /// 0x370
			 0x098f3285, /// 0x374
			 0x21b7b262, /// 0x378
			 0xf7336203, /// 0x37c
			 0x6858213d, /// 0x380
			 0xa2085b97, /// 0x384
			 0xea96680d, /// 0x388
			 0x88a7cea0, /// 0x38c
			 0x191a99d5, /// 0x390
			 0x0e667733, /// 0x394
			 0x27e47104, /// 0x398
			 0x7bff2109, /// 0x39c
			 0x0010cda8, /// 0x3a0
			 0xb72d6cbd, /// 0x3a4
			 0xd8b8d359, /// 0x3a8
			 0xb8c680be, /// 0x3ac
			 0x0833cce8, /// 0x3b0
			 0x449b61b0, /// 0x3b4
			 0x3ea528de, /// 0x3b8
			 0x88a424fd, /// 0x3bc
			 0xb5644bcb, /// 0x3c0
			 0x10ce42e4, /// 0x3c4
			 0xab422c69, /// 0x3c8
			 0xc8681b49, /// 0x3cc
			 0x5910db4d, /// 0x3d0
			 0x7ca25599, /// 0x3d4
			 0x66115b9e, /// 0x3d8
			 0x6f664b3c, /// 0x3dc
			 0xd5c8b108, /// 0x3e0
			 0x87b4f358, /// 0x3e4
			 0x4f4252de, /// 0x3e8
			 0xcb8ec9f8, /// 0x3ec
			 0x0635b877, /// 0x3f0
			 0x99fdce34, /// 0x3f4
			 0x901a6a36, /// 0x3f8
			 0xf4bc4bc3, /// 0x3fc
			 0x58f82baf, /// 0x400
			 0xd4ad988a, /// 0x404
			 0xcf67c396, /// 0x408
			 0xe5f8a40d, /// 0x40c
			 0x4eba4aad, /// 0x410
			 0x4f071b90, /// 0x414
			 0x3784aa73, /// 0x418
			 0xf10d2547, /// 0x41c
			 0x141980fe, /// 0x420
			 0xcaddcbce, /// 0x424
			 0x489c4d6c, /// 0x428
			 0xdf341d4e, /// 0x42c
			 0xf1a702b6, /// 0x430
			 0xd4cba347, /// 0x434
			 0xafe909d8, /// 0x438
			 0xbe270ba4, /// 0x43c
			 0x96298f96, /// 0x440
			 0x6726b9ac, /// 0x444
			 0x1315fecd, /// 0x448
			 0x8474de63, /// 0x44c
			 0xb35523dd, /// 0x450
			 0x8e3ccf07, /// 0x454
			 0xd287c8dc, /// 0x458
			 0x4362bc5d, /// 0x45c
			 0x1a1ef3c0, /// 0x460
			 0xe49914ad, /// 0x464
			 0x43e35625, /// 0x468
			 0xf15df88b, /// 0x46c
			 0xfe0ea52c, /// 0x470
			 0xb9f32383, /// 0x474
			 0xb3fe4097, /// 0x478
			 0x8f36b94a, /// 0x47c
			 0xed1a6c7d, /// 0x480
			 0x6ad8c305, /// 0x484
			 0x057c3276, /// 0x488
			 0x69fcb526, /// 0x48c
			 0x8b4c7edd, /// 0x490
			 0xf6a6d34a, /// 0x494
			 0x84b9b9fa, /// 0x498
			 0x54d23954, /// 0x49c
			 0x91ff0fc1, /// 0x4a0
			 0xf7f60fc2, /// 0x4a4
			 0x6ae6a5aa, /// 0x4a8
			 0xa175b44a, /// 0x4ac
			 0x0b6da665, /// 0x4b0
			 0x9b0d93c2, /// 0x4b4
			 0xfa32dd87, /// 0x4b8
			 0x3da2b317, /// 0x4bc
			 0x49eefba4, /// 0x4c0
			 0x8b84056b, /// 0x4c4
			 0xb18cd53b, /// 0x4c8
			 0xdb174d7d, /// 0x4cc
			 0x4e4617e3, /// 0x4d0
			 0xefcec1fd, /// 0x4d4
			 0xe71203d4, /// 0x4d8
			 0x50969bc1, /// 0x4dc
			 0x6a4a9f75, /// 0x4e0
			 0x40d84fc4, /// 0x4e4
			 0x00e51b50, /// 0x4e8
			 0xd01e4628, /// 0x4ec
			 0x75360e82, /// 0x4f0
			 0x00e5411f, /// 0x4f4
			 0x454ca465, /// 0x4f8
			 0xeaff077e, /// 0x4fc
			 0x54d39f9f, /// 0x500
			 0x818f036e, /// 0x504
			 0xda4e797f, /// 0x508
			 0xe2728ef3, /// 0x50c
			 0x27cee78a, /// 0x510
			 0xfed81204, /// 0x514
			 0xe5fd424d, /// 0x518
			 0x15d4cf0c, /// 0x51c
			 0x992cb453, /// 0x520
			 0x610566db, /// 0x524
			 0x849441ac, /// 0x528
			 0x4cd55c03, /// 0x52c
			 0xa44b909f, /// 0x530
			 0xe4ab99f8, /// 0x534
			 0x3509c2e0, /// 0x538
			 0x82ecda54, /// 0x53c
			 0x0d00937e, /// 0x540
			 0x756ce4ca, /// 0x544
			 0xdbf2e754, /// 0x548
			 0x7df27d4f, /// 0x54c
			 0x67eb09e9, /// 0x550
			 0x016d4470, /// 0x554
			 0xcbb4f87a, /// 0x558
			 0x92158767, /// 0x55c
			 0x1c9c9330, /// 0x560
			 0xf7269927, /// 0x564
			 0x3c57281a, /// 0x568
			 0xb3d5d3a7, /// 0x56c
			 0x2a8183a9, /// 0x570
			 0x291ffafe, /// 0x574
			 0x4c8c08df, /// 0x578
			 0xc5943aac, /// 0x57c
			 0xe6bf609c, /// 0x580
			 0xf9dc6e4a, /// 0x584
			 0x9f6fdc7c, /// 0x588
			 0x93cb677b, /// 0x58c
			 0xad05b58f, /// 0x590
			 0x359bc9ea, /// 0x594
			 0x37223abe, /// 0x598
			 0x1b24b24c, /// 0x59c
			 0x235b94ba, /// 0x5a0
			 0x7dc442d1, /// 0x5a4
			 0xcac149b4, /// 0x5a8
			 0x2649f2a3, /// 0x5ac
			 0x854060e5, /// 0x5b0
			 0x398091d9, /// 0x5b4
			 0xcc49ce11, /// 0x5b8
			 0x536249eb, /// 0x5bc
			 0xa562e5dc, /// 0x5c0
			 0xff8a2ec5, /// 0x5c4
			 0x3338cd88, /// 0x5c8
			 0x7ae12029, /// 0x5cc
			 0x89fc5e6d, /// 0x5d0
			 0xae4835dd, /// 0x5d4
			 0x42749e96, /// 0x5d8
			 0x6eba3d1a, /// 0x5dc
			 0x196cf113, /// 0x5e0
			 0x33cf1213, /// 0x5e4
			 0xeafa9ee4, /// 0x5e8
			 0x17d4cf22, /// 0x5ec
			 0x032d173d, /// 0x5f0
			 0x5b9e5206, /// 0x5f4
			 0x9c872e68, /// 0x5f8
			 0x5e68a8dd, /// 0x5fc
			 0xac59267d, /// 0x600
			 0xf8ddaedb, /// 0x604
			 0xb329a37f, /// 0x608
			 0x375b8bd5, /// 0x60c
			 0x3f7b7229, /// 0x610
			 0x6eeabee1, /// 0x614
			 0xd66ee874, /// 0x618
			 0x496d073a, /// 0x61c
			 0x69b44f14, /// 0x620
			 0xad0aa2ef, /// 0x624
			 0xd3250f8c, /// 0x628
			 0xf5c3907d, /// 0x62c
			 0x09bb80ce, /// 0x630
			 0xb498f2ff, /// 0x634
			 0x818d0351, /// 0x638
			 0x911425b2, /// 0x63c
			 0x03100510, /// 0x640
			 0x6d555804, /// 0x644
			 0x38cd6361, /// 0x648
			 0x221f6e64, /// 0x64c
			 0x9f027a8a, /// 0x650
			 0xbddaf47b, /// 0x654
			 0x39f0dcf1, /// 0x658
			 0xac9894bb, /// 0x65c
			 0xac8b3139, /// 0x660
			 0x1aa6d4f8, /// 0x664
			 0xb3c52955, /// 0x668
			 0x3ff9ac00, /// 0x66c
			 0x17d19fc7, /// 0x670
			 0x6cdfffde, /// 0x674
			 0x1bfd54af, /// 0x678
			 0xf2b99aee, /// 0x67c
			 0x3a3afc65, /// 0x680
			 0x77b9a644, /// 0x684
			 0xa8706fd9, /// 0x688
			 0xd4a2b4dc, /// 0x68c
			 0x216bc118, /// 0x690
			 0x2785c80e, /// 0x694
			 0x13d5e41a, /// 0x698
			 0x8ab88615, /// 0x69c
			 0x030358a6, /// 0x6a0
			 0x0fd47c1b, /// 0x6a4
			 0x01213adb, /// 0x6a8
			 0x5d824739, /// 0x6ac
			 0x570ed96b, /// 0x6b0
			 0x166f3315, /// 0x6b4
			 0xb53505a7, /// 0x6b8
			 0x37756297, /// 0x6bc
			 0x6f549e9b, /// 0x6c0
			 0xb5d21da1, /// 0x6c4
			 0x89fddccd, /// 0x6c8
			 0x23c3034b, /// 0x6cc
			 0xae09c1bf, /// 0x6d0
			 0x219fe556, /// 0x6d4
			 0x06655b19, /// 0x6d8
			 0x5010b8eb, /// 0x6dc
			 0x57904978, /// 0x6e0
			 0x82d15487, /// 0x6e4
			 0x7ae612c1, /// 0x6e8
			 0x91e3cad1, /// 0x6ec
			 0xe084cb9b, /// 0x6f0
			 0xcf2ff48c, /// 0x6f4
			 0x17d00f92, /// 0x6f8
			 0x2da2ab75, /// 0x6fc
			 0xebd674da, /// 0x700
			 0x1d1e9b55, /// 0x704
			 0x7803fb15, /// 0x708
			 0xc86369be, /// 0x70c
			 0xb0890fb1, /// 0x710
			 0xa65babcc, /// 0x714
			 0x4b006a2e, /// 0x718
			 0x35331ad1, /// 0x71c
			 0xffcff604, /// 0x720
			 0xf8f5878f, /// 0x724
			 0x91d2214d, /// 0x728
			 0xd6842435, /// 0x72c
			 0x2ddd859c, /// 0x730
			 0x0d43994a, /// 0x734
			 0x52bc4f4c, /// 0x738
			 0x4cbdbdba, /// 0x73c
			 0x0639f62a, /// 0x740
			 0xc9152ab2, /// 0x744
			 0x271fb52b, /// 0x748
			 0x4fc5984c, /// 0x74c
			 0xef3f8f52, /// 0x750
			 0x7c1351b6, /// 0x754
			 0xf45ad406, /// 0x758
			 0xd97ad55a, /// 0x75c
			 0xb270144a, /// 0x760
			 0x7bc7b1a9, /// 0x764
			 0xbfd3eb8f, /// 0x768
			 0xd9beeba9, /// 0x76c
			 0xb09c73ee, /// 0x770
			 0xcf20b0ee, /// 0x774
			 0xce995c38, /// 0x778
			 0x9331831a, /// 0x77c
			 0x8706d194, /// 0x780
			 0xf73b7549, /// 0x784
			 0xb2730321, /// 0x788
			 0x707ec54e, /// 0x78c
			 0x7da72faa, /// 0x790
			 0xff764d05, /// 0x794
			 0xf833049b, /// 0x798
			 0x57e43883, /// 0x79c
			 0x03f24f03, /// 0x7a0
			 0x8c2f22bd, /// 0x7a4
			 0x62873aa6, /// 0x7a8
			 0xee44c020, /// 0x7ac
			 0x5fe67492, /// 0x7b0
			 0x967fd2d9, /// 0x7b4
			 0xd1043a4d, /// 0x7b8
			 0xf3c96a05, /// 0x7bc
			 0x25d5bb32, /// 0x7c0
			 0x920c7ace, /// 0x7c4
			 0x8b23e294, /// 0x7c8
			 0xe827aa4b, /// 0x7cc
			 0xb9c9e650, /// 0x7d0
			 0xad881119, /// 0x7d4
			 0x4643549f, /// 0x7d8
			 0x7f745a6f, /// 0x7dc
			 0xe32441d2, /// 0x7e0
			 0x14832186, /// 0x7e4
			 0x0bc84bda, /// 0x7e8
			 0x78c89c46, /// 0x7ec
			 0x93aedb98, /// 0x7f0
			 0x53ce4234, /// 0x7f4
			 0x454ddf12, /// 0x7f8
			 0x46badca8, /// 0x7fc
			 0x8a7e2c11, /// 0x800
			 0x707438a3, /// 0x804
			 0xf6bb1b0a, /// 0x808
			 0x723fcba0, /// 0x80c
			 0x2071855f, /// 0x810
			 0xbd7d088c, /// 0x814
			 0x3f37154f, /// 0x818
			 0x844b7e66, /// 0x81c
			 0x6945e57c, /// 0x820
			 0xcc063b18, /// 0x824
			 0x59e14f0b, /// 0x828
			 0x3bfc80a7, /// 0x82c
			 0x600c5129, /// 0x830
			 0x5ea88df2, /// 0x834
			 0x7afcf3aa, /// 0x838
			 0x8249e43e, /// 0x83c
			 0x00549871, /// 0x840
			 0x1f97df89, /// 0x844
			 0x7e4a142a, /// 0x848
			 0xd8c2463a, /// 0x84c
			 0x1084c315, /// 0x850
			 0x8150793e, /// 0x854
			 0x15039587, /// 0x858
			 0x3919f773, /// 0x85c
			 0x0f862eea, /// 0x860
			 0xec2fff89, /// 0x864
			 0x058168a8, /// 0x868
			 0xb6589076, /// 0x86c
			 0xf8e0f578, /// 0x870
			 0xca9c5d91, /// 0x874
			 0xe9b0504a, /// 0x878
			 0x3b29aab1, /// 0x87c
			 0x581e709d, /// 0x880
			 0x86f5697c, /// 0x884
			 0x0ade781f, /// 0x888
			 0x9992476d, /// 0x88c
			 0x5b58e8e0, /// 0x890
			 0x915e1a73, /// 0x894
			 0x47a5177a, /// 0x898
			 0x0ea7c3be, /// 0x89c
			 0xcf6c6028, /// 0x8a0
			 0x5d49368a, /// 0x8a4
			 0x098eaa6d, /// 0x8a8
			 0xc5314eef, /// 0x8ac
			 0x9d390f78, /// 0x8b0
			 0x57c927b3, /// 0x8b4
			 0xc1197d4b, /// 0x8b8
			 0x6d016d6f, /// 0x8bc
			 0xac4a457d, /// 0x8c0
			 0x5e1b8617, /// 0x8c4
			 0x1374cd04, /// 0x8c8
			 0x27f78306, /// 0x8cc
			 0x33c53cc5, /// 0x8d0
			 0xb7cca786, /// 0x8d4
			 0xe109a530, /// 0x8d8
			 0x032438ae, /// 0x8dc
			 0x7deffa65, /// 0x8e0
			 0x0b9f2fbd, /// 0x8e4
			 0x84bed387, /// 0x8e8
			 0xad9a62b7, /// 0x8ec
			 0x20cd8dc9, /// 0x8f0
			 0xe5657d3d, /// 0x8f4
			 0xbe4cc5fc, /// 0x8f8
			 0x2cc48bca, /// 0x8fc
			 0xd373add7, /// 0x900
			 0xd689de14, /// 0x904
			 0x2ada4e3a, /// 0x908
			 0x803031c4, /// 0x90c
			 0x0fa8cd36, /// 0x910
			 0x53ce5458, /// 0x914
			 0xeac670f6, /// 0x918
			 0x6f7cf805, /// 0x91c
			 0xa752e688, /// 0x920
			 0x9f1694e1, /// 0x924
			 0xb1e20c14, /// 0x928
			 0x1b82122f, /// 0x92c
			 0x403cc587, /// 0x930
			 0x0f3d2a39, /// 0x934
			 0xef2e152d, /// 0x938
			 0x7a6810de, /// 0x93c
			 0x44ff4255, /// 0x940
			 0x5ba07b63, /// 0x944
			 0x6b10fab0, /// 0x948
			 0x7c38e971, /// 0x94c
			 0x7d5222ea, /// 0x950
			 0x8284d361, /// 0x954
			 0x6af2d29b, /// 0x958
			 0xda696132, /// 0x95c
			 0x5e836126, /// 0x960
			 0xc4312ed9, /// 0x964
			 0x89fb0700, /// 0x968
			 0x978aea9b, /// 0x96c
			 0x131bd7ad, /// 0x970
			 0x55b412a7, /// 0x974
			 0xfa43b63c, /// 0x978
			 0x70fe8e83, /// 0x97c
			 0x750d9a5d, /// 0x980
			 0x05ecbe4d, /// 0x984
			 0xfa42c424, /// 0x988
			 0x26bb06d1, /// 0x98c
			 0xce016f20, /// 0x990
			 0x31af73e1, /// 0x994
			 0x76b3db45, /// 0x998
			 0xf8ea141b, /// 0x99c
			 0x5fdbd3b1, /// 0x9a0
			 0x4dc07068, /// 0x9a4
			 0xa3cc360a, /// 0x9a8
			 0x5fcba2d2, /// 0x9ac
			 0x361d2825, /// 0x9b0
			 0x894863fd, /// 0x9b4
			 0xa308a388, /// 0x9b8
			 0x044961b6, /// 0x9bc
			 0x1c76ad99, /// 0x9c0
			 0xb36bbf7f, /// 0x9c4
			 0xecf6ff13, /// 0x9c8
			 0x651a2ccf, /// 0x9cc
			 0xd70f60c6, /// 0x9d0
			 0x5acb7ee4, /// 0x9d4
			 0x56067cf6, /// 0x9d8
			 0x1bc11036, /// 0x9dc
			 0x72a135f5, /// 0x9e0
			 0x6fd19098, /// 0x9e4
			 0x3d62723b, /// 0x9e8
			 0xb925142f, /// 0x9ec
			 0x9aa4268e, /// 0x9f0
			 0x4ef9ccfc, /// 0x9f4
			 0x5b7b31b4, /// 0x9f8
			 0xf9bc7509, /// 0x9fc
			 0xe89cd745, /// 0xa00
			 0x9cd88738, /// 0xa04
			 0x970b9262, /// 0xa08
			 0x90114036, /// 0xa0c
			 0x1d84170b, /// 0xa10
			 0x99598556, /// 0xa14
			 0x3aa971fe, /// 0xa18
			 0x556fc5b8, /// 0xa1c
			 0x783b4034, /// 0xa20
			 0x81a712fa, /// 0xa24
			 0x048a8b30, /// 0xa28
			 0xefc3176e, /// 0xa2c
			 0x019a761f, /// 0xa30
			 0xa7f3b47a, /// 0xa34
			 0x0d5942c9, /// 0xa38
			 0xaa0296f4, /// 0xa3c
			 0x1a45e33d, /// 0xa40
			 0x0c04b5fb, /// 0xa44
			 0x5b751194, /// 0xa48
			 0x3b5c987b, /// 0xa4c
			 0x8f5317f4, /// 0xa50
			 0x5824b781, /// 0xa54
			 0x491e6367, /// 0xa58
			 0x84ff495b, /// 0xa5c
			 0x6834e7f5, /// 0xa60
			 0xdfbf03d8, /// 0xa64
			 0x6266ee36, /// 0xa68
			 0x5e820fb5, /// 0xa6c
			 0xfaa5c568, /// 0xa70
			 0x944e798b, /// 0xa74
			 0x96159af5, /// 0xa78
			 0x7b578781, /// 0xa7c
			 0xc61c73e3, /// 0xa80
			 0xb61119dd, /// 0xa84
			 0xef8872a8, /// 0xa88
			 0x4fee67a7, /// 0xa8c
			 0x5eb92d94, /// 0xa90
			 0x7e7576ef, /// 0xa94
			 0x49198228, /// 0xa98
			 0x5408e997, /// 0xa9c
			 0x3c44733b, /// 0xaa0
			 0xe4ac2bb3, /// 0xaa4
			 0x653513ea, /// 0xaa8
			 0x904db614, /// 0xaac
			 0x2abb87a2, /// 0xab0
			 0x7b8c49f6, /// 0xab4
			 0x06ff33a0, /// 0xab8
			 0x625f5d81, /// 0xabc
			 0x7128a3dc, /// 0xac0
			 0xbbf9a8e6, /// 0xac4
			 0x46dad0ec, /// 0xac8
			 0x762c82cc, /// 0xacc
			 0x2460bae7, /// 0xad0
			 0x4cab1f80, /// 0xad4
			 0x750457a1, /// 0xad8
			 0xa020c3fb, /// 0xadc
			 0xf79c27da, /// 0xae0
			 0x16e05d1c, /// 0xae4
			 0x82564e48, /// 0xae8
			 0x95be0679, /// 0xaec
			 0xaa217b95, /// 0xaf0
			 0x970311ae, /// 0xaf4
			 0x14cb6841, /// 0xaf8
			 0xe8f24f18, /// 0xafc
			 0xa64bfb85, /// 0xb00
			 0x3b28ea6a, /// 0xb04
			 0x8b5de808, /// 0xb08
			 0xe4663111, /// 0xb0c
			 0x9075b5c3, /// 0xb10
			 0xb17239e4, /// 0xb14
			 0x6916f92b, /// 0xb18
			 0xbe9ba991, /// 0xb1c
			 0xe8449624, /// 0xb20
			 0x4bb1727d, /// 0xb24
			 0x59ba9be1, /// 0xb28
			 0x78550bc8, /// 0xb2c
			 0xb5d4c55b, /// 0xb30
			 0x952f48ba, /// 0xb34
			 0x3b0842a9, /// 0xb38
			 0xde34c1f8, /// 0xb3c
			 0x89c44933, /// 0xb40
			 0x525d30aa, /// 0xb44
			 0xa9c0f1f0, /// 0xb48
			 0x0227bf99, /// 0xb4c
			 0x3d4cd1e0, /// 0xb50
			 0xb66423e5, /// 0xb54
			 0x42afdf2c, /// 0xb58
			 0xda9576bb, /// 0xb5c
			 0x47198346, /// 0xb60
			 0x30ae3d8d, /// 0xb64
			 0xfa33377d, /// 0xb68
			 0x4acc48c3, /// 0xb6c
			 0x67a55388, /// 0xb70
			 0x462e06a8, /// 0xb74
			 0x0b6c2e34, /// 0xb78
			 0x508fd4c9, /// 0xb7c
			 0x815b2de8, /// 0xb80
			 0xa1626861, /// 0xb84
			 0xf09ac3ed, /// 0xb88
			 0x72b66ce7, /// 0xb8c
			 0xffb9e7d9, /// 0xb90
			 0x46c21326, /// 0xb94
			 0xfcd3e032, /// 0xb98
			 0xb83fd6aa, /// 0xb9c
			 0x352f5efb, /// 0xba0
			 0xe25cfb0c, /// 0xba4
			 0x4aa45be4, /// 0xba8
			 0xe5989c31, /// 0xbac
			 0x754f91c1, /// 0xbb0
			 0xe8ce1bf0, /// 0xbb4
			 0xbaaaed49, /// 0xbb8
			 0x0cb3d434, /// 0xbbc
			 0x63f5ff27, /// 0xbc0
			 0x18ae8b5d, /// 0xbc4
			 0x271c3805, /// 0xbc8
			 0x4bb9c401, /// 0xbcc
			 0x181a202a, /// 0xbd0
			 0x2b162b34, /// 0xbd4
			 0x76afbd48, /// 0xbd8
			 0x6a0ef6e7, /// 0xbdc
			 0x65ffb7e0, /// 0xbe0
			 0x7064eaeb, /// 0xbe4
			 0x99411cc4, /// 0xbe8
			 0x8391dfd4, /// 0xbec
			 0xef40b206, /// 0xbf0
			 0xfdcbdf06, /// 0xbf4
			 0x87332111, /// 0xbf8
			 0xb63ac3c6, /// 0xbfc
			 0xb6e3f332, /// 0xc00
			 0xa8fdc65c, /// 0xc04
			 0xab22d661, /// 0xc08
			 0x9f512a7e, /// 0xc0c
			 0xc2b8bfae, /// 0xc10
			 0x7e97d9c8, /// 0xc14
			 0xb4c8b652, /// 0xc18
			 0x10404441, /// 0xc1c
			 0x5b29aa1b, /// 0xc20
			 0x00100b11, /// 0xc24
			 0xd1963f91, /// 0xc28
			 0xcac599bd, /// 0xc2c
			 0xd894a1b7, /// 0xc30
			 0x4c23554f, /// 0xc34
			 0xa12de939, /// 0xc38
			 0x65c609ad, /// 0xc3c
			 0x7b7bff12, /// 0xc40
			 0x2017907f, /// 0xc44
			 0xde7500ec, /// 0xc48
			 0x1537d5ca, /// 0xc4c
			 0xd0324eb0, /// 0xc50
			 0x752b76c6, /// 0xc54
			 0x5f5b444b, /// 0xc58
			 0x400b5c2d, /// 0xc5c
			 0xc385cdda, /// 0xc60
			 0xaa16374c, /// 0xc64
			 0x0bdd41db, /// 0xc68
			 0x888b42a2, /// 0xc6c
			 0x766e23b2, /// 0xc70
			 0xdd2f0f64, /// 0xc74
			 0x4061dcd1, /// 0xc78
			 0x48c941fd, /// 0xc7c
			 0xc0037bd7, /// 0xc80
			 0x48c80964, /// 0xc84
			 0xe0aa6f7a, /// 0xc88
			 0x8308c094, /// 0xc8c
			 0xd5567050, /// 0xc90
			 0x3d66cefb, /// 0xc94
			 0xe1f6852f, /// 0xc98
			 0x9370c4f6, /// 0xc9c
			 0x4479a4e8, /// 0xca0
			 0xfb7d27f9, /// 0xca4
			 0xc90e5690, /// 0xca8
			 0xf2a433ad, /// 0xcac
			 0xd43e3741, /// 0xcb0
			 0x16deefbb, /// 0xcb4
			 0x3ac50855, /// 0xcb8
			 0x86babcc8, /// 0xcbc
			 0xa63a6e67, /// 0xcc0
			 0xde13c550, /// 0xcc4
			 0x255876aa, /// 0xcc8
			 0xf36e246c, /// 0xccc
			 0xc7b585c9, /// 0xcd0
			 0x63081dfb, /// 0xcd4
			 0xad644004, /// 0xcd8
			 0xf5b5a302, /// 0xcdc
			 0x0ddbdac3, /// 0xce0
			 0x212502ca, /// 0xce4
			 0x458717d5, /// 0xce8
			 0xaef0457d, /// 0xcec
			 0x8de556cb, /// 0xcf0
			 0x01e9c1d9, /// 0xcf4
			 0x10820a4d, /// 0xcf8
			 0xe950d8b6, /// 0xcfc
			 0x4460f350, /// 0xd00
			 0x2f0d1052, /// 0xd04
			 0xf7892c07, /// 0xd08
			 0x512f1bcb, /// 0xd0c
			 0x70a4e104, /// 0xd10
			 0x4b0a58ee, /// 0xd14
			 0x2c3038c3, /// 0xd18
			 0x25d4e709, /// 0xd1c
			 0x69680a9a, /// 0xd20
			 0x3e3e16dc, /// 0xd24
			 0xa4e82906, /// 0xd28
			 0x47b3c96c, /// 0xd2c
			 0x33f8e682, /// 0xd30
			 0x7c56b104, /// 0xd34
			 0xced31619, /// 0xd38
			 0xafbd6f65, /// 0xd3c
			 0x34e4b523, /// 0xd40
			 0x60becbf9, /// 0xd44
			 0xa83b4a01, /// 0xd48
			 0x63a73490, /// 0xd4c
			 0xcdedaf4f, /// 0xd50
			 0xb0d90256, /// 0xd54
			 0xc7feb543, /// 0xd58
			 0x760479a4, /// 0xd5c
			 0x99a5ed3e, /// 0xd60
			 0xedb35686, /// 0xd64
			 0xbaa0cde7, /// 0xd68
			 0x9a075eea, /// 0xd6c
			 0x0b0f681e, /// 0xd70
			 0xd4434c14, /// 0xd74
			 0x2edf96e5, /// 0xd78
			 0x24975dd7, /// 0xd7c
			 0xdfb846d1, /// 0xd80
			 0x9fcd1814, /// 0xd84
			 0x23e61937, /// 0xd88
			 0x3c4c6124, /// 0xd8c
			 0x166dbbae, /// 0xd90
			 0xa13b6f02, /// 0xd94
			 0x4f067de9, /// 0xd98
			 0x6f9d2b98, /// 0xd9c
			 0x873addf9, /// 0xda0
			 0x9ed066ae, /// 0xda4
			 0xa0308e3d, /// 0xda8
			 0xf8af1c5d, /// 0xdac
			 0x6e719d1a, /// 0xdb0
			 0x7e8871b3, /// 0xdb4
			 0xb6b37dba, /// 0xdb8
			 0x78a6b28d, /// 0xdbc
			 0xa290d39b, /// 0xdc0
			 0xae5d28b7, /// 0xdc4
			 0x7bf84a6c, /// 0xdc8
			 0x26dfa55b, /// 0xdcc
			 0xa5397b6c, /// 0xdd0
			 0x5e5c3494, /// 0xdd4
			 0xbf55bcb9, /// 0xdd8
			 0xa493afc8, /// 0xddc
			 0x9084afa5, /// 0xde0
			 0x710de241, /// 0xde4
			 0x48f288e1, /// 0xde8
			 0xef08284b, /// 0xdec
			 0x84b7f86f, /// 0xdf0
			 0x4dfbfa2f, /// 0xdf4
			 0xf639fc55, /// 0xdf8
			 0x6453a87b, /// 0xdfc
			 0xb99f6294, /// 0xe00
			 0x51815c0e, /// 0xe04
			 0x17a9d368, /// 0xe08
			 0x247918f3, /// 0xe0c
			 0x1725df18, /// 0xe10
			 0xd60c2e25, /// 0xe14
			 0xae7d63b4, /// 0xe18
			 0x5d3d994d, /// 0xe1c
			 0x7297d6c8, /// 0xe20
			 0x9576a186, /// 0xe24
			 0x6ab72298, /// 0xe28
			 0x40c2bca2, /// 0xe2c
			 0x141c7523, /// 0xe30
			 0x4c4068f5, /// 0xe34
			 0x5b4fa902, /// 0xe38
			 0x27c14609, /// 0xe3c
			 0xbb9e1a7a, /// 0xe40
			 0xb851fa15, /// 0xe44
			 0x0919a169, /// 0xe48
			 0x9429b348, /// 0xe4c
			 0xb7816ecc, /// 0xe50
			 0x5945afb0, /// 0xe54
			 0xd7dcd26f, /// 0xe58
			 0x041658f8, /// 0xe5c
			 0x89eb9122, /// 0xe60
			 0x48ac031a, /// 0xe64
			 0xd6181808, /// 0xe68
			 0xceff29fc, /// 0xe6c
			 0x8528237d, /// 0xe70
			 0x9f92f4b6, /// 0xe74
			 0xd325f586, /// 0xe78
			 0x15459505, /// 0xe7c
			 0xc56920c2, /// 0xe80
			 0x2acde389, /// 0xe84
			 0x0f0123e2, /// 0xe88
			 0xf50ef609, /// 0xe8c
			 0x141b2808, /// 0xe90
			 0x4f0cd1c6, /// 0xe94
			 0xf75eab02, /// 0xe98
			 0x059e1fdc, /// 0xe9c
			 0xab82b079, /// 0xea0
			 0x5cac92e9, /// 0xea4
			 0x77afe40d, /// 0xea8
			 0x76c174d4, /// 0xeac
			 0xb2028d6b, /// 0xeb0
			 0x285d77c9, /// 0xeb4
			 0x0207ebfd, /// 0xeb8
			 0x7f48210f, /// 0xebc
			 0xf8582adb, /// 0xec0
			 0x6462bb58, /// 0xec4
			 0xf15164e1, /// 0xec8
			 0xfa35932f, /// 0xecc
			 0x9e3e1ab3, /// 0xed0
			 0x3d5e9a84, /// 0xed4
			 0xf28b42f0, /// 0xed8
			 0xa26bbb59, /// 0xedc
			 0x03216b11, /// 0xee0
			 0x61e343d2, /// 0xee4
			 0x473dd907, /// 0xee8
			 0x4642411f, /// 0xeec
			 0x9511e10e, /// 0xef0
			 0x195ed524, /// 0xef4
			 0x86f55b9f, /// 0xef8
			 0xf85461ef, /// 0xefc
			 0x9b64693a, /// 0xf00
			 0xb9f0debb, /// 0xf04
			 0x232fceb7, /// 0xf08
			 0xdab6bc13, /// 0xf0c
			 0xdd51e2cd, /// 0xf10
			 0x80988a0f, /// 0xf14
			 0xb8924d47, /// 0xf18
			 0xe2975d19, /// 0xf1c
			 0xa61c30d7, /// 0xf20
			 0x468e3d7f, /// 0xf24
			 0x79d543af, /// 0xf28
			 0x9027bd81, /// 0xf2c
			 0xd4aa5a1b, /// 0xf30
			 0x44df8777, /// 0xf34
			 0x0e92e00c, /// 0xf38
			 0x518ab65f, /// 0xf3c
			 0x8bee187d, /// 0xf40
			 0x3861bce2, /// 0xf44
			 0x507a6509, /// 0xf48
			 0xb19ea5a7, /// 0xf4c
			 0xf23164ec, /// 0xf50
			 0x02040aea, /// 0xf54
			 0x461c8133, /// 0xf58
			 0x511600ec, /// 0xf5c
			 0x55b976b9, /// 0xf60
			 0xc58ef124, /// 0xf64
			 0xe9ec84b7, /// 0xf68
			 0x42025897, /// 0xf6c
			 0xf925a035, /// 0xf70
			 0x7676cef6, /// 0xf74
			 0x073a967f, /// 0xf78
			 0x83d13a2a, /// 0xf7c
			 0x0a3474ac, /// 0xf80
			 0x4123b6ce, /// 0xf84
			 0x3e71cd8a, /// 0xf88
			 0x702a0654, /// 0xf8c
			 0x9b87063f, /// 0xf90
			 0xb0ac9331, /// 0xf94
			 0x5a3ff03c, /// 0xf98
			 0x9be6fece, /// 0xf9c
			 0x690318d7, /// 0xfa0
			 0x4b084933, /// 0xfa4
			 0xc0a02fc2, /// 0xfa8
			 0xccd09142, /// 0xfac
			 0x949a0ffb, /// 0xfb0
			 0x808a2eb2, /// 0xfb4
			 0x00739788, /// 0xfb8
			 0x0217ccf4, /// 0xfbc
			 0x4d64a2e3, /// 0xfc0
			 0xe06d2d09, /// 0xfc4
			 0xa4525bac, /// 0xfc8
			 0xe335ab3a, /// 0xfcc
			 0x150ae7cb, /// 0xfd0
			 0xd6defc21, /// 0xfd4
			 0xbe8528c7, /// 0xfd8
			 0xe9cfb529, /// 0xfdc
			 0xf70d2753, /// 0xfe0
			 0x635694e0, /// 0xfe4
			 0xbf2f83bc, /// 0xfe8
			 0xff996ac2, /// 0xfec
			 0xe03dbd7f, /// 0xff0
			 0xd9c59afa, /// 0xff4
			 0x8702a9d1, /// 0xff8
			 0x5df6bb1b /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xb56b99d4, /// 0x0
			 0x6b9f37f4, /// 0x4
			 0x87175e00, /// 0x8
			 0x49d7d29c, /// 0xc
			 0x13861bab, /// 0x10
			 0xd077139d, /// 0x14
			 0xf714616f, /// 0x18
			 0x50803ece, /// 0x1c
			 0xea0f963b, /// 0x20
			 0xf995c08b, /// 0x24
			 0xb8934fd5, /// 0x28
			 0xff2cac87, /// 0x2c
			 0x1925b868, /// 0x30
			 0x0b8d6e3a, /// 0x34
			 0xb63dd7ff, /// 0x38
			 0xdf9ea26e, /// 0x3c
			 0x9904248c, /// 0x40
			 0x08895b87, /// 0x44
			 0x3fc0f0ad, /// 0x48
			 0xcc777c53, /// 0x4c
			 0x944d820b, /// 0x50
			 0x3f4565fd, /// 0x54
			 0x16771250, /// 0x58
			 0x5bd37f78, /// 0x5c
			 0x562fda46, /// 0x60
			 0x637ecccf, /// 0x64
			 0xf0826acc, /// 0x68
			 0x8a917314, /// 0x6c
			 0x9ef3dd0d, /// 0x70
			 0x44cf7903, /// 0x74
			 0x843cabeb, /// 0x78
			 0x53ac2c4e, /// 0x7c
			 0xca9100e7, /// 0x80
			 0xed97ceaf, /// 0x84
			 0xdd84ac8a, /// 0x88
			 0x541f90fb, /// 0x8c
			 0x148798a0, /// 0x90
			 0x565589e8, /// 0x94
			 0x763039df, /// 0x98
			 0xd25ca0ff, /// 0x9c
			 0xde2d099b, /// 0xa0
			 0xdd2f6ffd, /// 0xa4
			 0xae0a65e3, /// 0xa8
			 0x821c7d69, /// 0xac
			 0x69fe51cf, /// 0xb0
			 0xd5885894, /// 0xb4
			 0x3160115f, /// 0xb8
			 0xe615ff54, /// 0xbc
			 0x5589b65b, /// 0xc0
			 0x0f39a5fc, /// 0xc4
			 0xc2f5f45b, /// 0xc8
			 0xae1eded0, /// 0xcc
			 0xb0003190, /// 0xd0
			 0x605d425d, /// 0xd4
			 0xbde7c54b, /// 0xd8
			 0x5d5909f4, /// 0xdc
			 0xff2ba673, /// 0xe0
			 0x70112d7b, /// 0xe4
			 0x3d7ea9d5, /// 0xe8
			 0xc1e57140, /// 0xec
			 0x889607f1, /// 0xf0
			 0xae6f97b4, /// 0xf4
			 0xe3b5f7fd, /// 0xf8
			 0x459f514b, /// 0xfc
			 0x86885ec7, /// 0x100
			 0xe99fedf3, /// 0x104
			 0x738e71a1, /// 0x108
			 0xf0260cac, /// 0x10c
			 0x24654415, /// 0x110
			 0x98ace92f, /// 0x114
			 0x84236cb1, /// 0x118
			 0xa7374a79, /// 0x11c
			 0xe4996100, /// 0x120
			 0x7212cc37, /// 0x124
			 0x0eb8ace3, /// 0x128
			 0x4c752f6f, /// 0x12c
			 0xe0b6b588, /// 0x130
			 0x9ba4a0cd, /// 0x134
			 0xe6ad60a9, /// 0x138
			 0x56ac6e47, /// 0x13c
			 0xe6e3e778, /// 0x140
			 0xd8f682c4, /// 0x144
			 0x69986848, /// 0x148
			 0xfae8e6a4, /// 0x14c
			 0x161cbeeb, /// 0x150
			 0x0df35cd3, /// 0x154
			 0x143603ae, /// 0x158
			 0x325e3ed2, /// 0x15c
			 0x8dc1543b, /// 0x160
			 0x96e1b11e, /// 0x164
			 0x5b98a5b8, /// 0x168
			 0x69123f6c, /// 0x16c
			 0x7e24c913, /// 0x170
			 0x7f793404, /// 0x174
			 0xcc305d53, /// 0x178
			 0x3df33cbe, /// 0x17c
			 0x5f88f50b, /// 0x180
			 0x343ad9ad, /// 0x184
			 0xdc4528c0, /// 0x188
			 0x52274150, /// 0x18c
			 0xb3cef4e8, /// 0x190
			 0xdc629491, /// 0x194
			 0x9664e56d, /// 0x198
			 0x720f36e9, /// 0x19c
			 0x3a3b732d, /// 0x1a0
			 0x55ac25a6, /// 0x1a4
			 0xd659edbc, /// 0x1a8
			 0x8342508d, /// 0x1ac
			 0x772076df, /// 0x1b0
			 0xb8ddcfe3, /// 0x1b4
			 0xfc9672ed, /// 0x1b8
			 0x615bff77, /// 0x1bc
			 0x74087d37, /// 0x1c0
			 0x0b4fec63, /// 0x1c4
			 0xd326566a, /// 0x1c8
			 0x947fe293, /// 0x1cc
			 0x87ba376b, /// 0x1d0
			 0x68dac8ec, /// 0x1d4
			 0x4309cfce, /// 0x1d8
			 0xe48e87b4, /// 0x1dc
			 0x9bae5d22, /// 0x1e0
			 0x43ec0c19, /// 0x1e4
			 0x4b46eb0b, /// 0x1e8
			 0x4576d99c, /// 0x1ec
			 0xab56e8d7, /// 0x1f0
			 0xe0a9cc44, /// 0x1f4
			 0x7ecade8a, /// 0x1f8
			 0xe4a64bbe, /// 0x1fc
			 0x25a604e6, /// 0x200
			 0x3ef512c9, /// 0x204
			 0x9b6f58f9, /// 0x208
			 0x3885f090, /// 0x20c
			 0x8cb64c0c, /// 0x210
			 0x934e5834, /// 0x214
			 0x49574213, /// 0x218
			 0x56fe4841, /// 0x21c
			 0xc6cc7f09, /// 0x220
			 0xb548272e, /// 0x224
			 0x4ce9c99d, /// 0x228
			 0x9bc6a997, /// 0x22c
			 0x72bf0751, /// 0x230
			 0x577e706e, /// 0x234
			 0x86a41529, /// 0x238
			 0x2234ad51, /// 0x23c
			 0x7450b6c0, /// 0x240
			 0x842b9bb6, /// 0x244
			 0xadabc50b, /// 0x248
			 0xb230acd6, /// 0x24c
			 0x6e3cad6f, /// 0x250
			 0xbd91a13f, /// 0x254
			 0x3a2bed73, /// 0x258
			 0x0241f8d1, /// 0x25c
			 0xaed42493, /// 0x260
			 0xae285067, /// 0x264
			 0xab512c6b, /// 0x268
			 0xed718480, /// 0x26c
			 0x637c763c, /// 0x270
			 0xf42662e6, /// 0x274
			 0xe101b016, /// 0x278
			 0x8dd6d1ef, /// 0x27c
			 0x94eda587, /// 0x280
			 0x3b722644, /// 0x284
			 0x013e8303, /// 0x288
			 0x87962f78, /// 0x28c
			 0xf4ce18b3, /// 0x290
			 0x827bf065, /// 0x294
			 0x6ebb05cc, /// 0x298
			 0x557bf714, /// 0x29c
			 0x3d10b7ae, /// 0x2a0
			 0x44f7aff0, /// 0x2a4
			 0x6cd0cf90, /// 0x2a8
			 0x05c8c851, /// 0x2ac
			 0x3a8d5d82, /// 0x2b0
			 0xf3a8da17, /// 0x2b4
			 0x62a0014a, /// 0x2b8
			 0xde3c6fe7, /// 0x2bc
			 0xc0c34f11, /// 0x2c0
			 0x16db916f, /// 0x2c4
			 0x00ec21b7, /// 0x2c8
			 0xf121f0f3, /// 0x2cc
			 0xd415bb69, /// 0x2d0
			 0x196d6665, /// 0x2d4
			 0x9771d500, /// 0x2d8
			 0xa9596de7, /// 0x2dc
			 0xd0b57435, /// 0x2e0
			 0x7e905c39, /// 0x2e4
			 0xe1ae7303, /// 0x2e8
			 0xba93ed3d, /// 0x2ec
			 0x45f32234, /// 0x2f0
			 0x24ec1728, /// 0x2f4
			 0x503c2dd7, /// 0x2f8
			 0xff98e956, /// 0x2fc
			 0x07494c7c, /// 0x300
			 0x3ff20f8d, /// 0x304
			 0xa75da4b8, /// 0x308
			 0xf17bbbb3, /// 0x30c
			 0xb348e902, /// 0x310
			 0xe311bcb6, /// 0x314
			 0x45de5047, /// 0x318
			 0x515f67f0, /// 0x31c
			 0x8c0df14c, /// 0x320
			 0x8134bd0a, /// 0x324
			 0xe7e9e650, /// 0x328
			 0x3c0ab149, /// 0x32c
			 0xc20f20cc, /// 0x330
			 0xca9a66f4, /// 0x334
			 0x39af36b3, /// 0x338
			 0xbb024211, /// 0x33c
			 0x7dc0c619, /// 0x340
			 0xc80a444c, /// 0x344
			 0x1e338fcc, /// 0x348
			 0x87570657, /// 0x34c
			 0xe1217216, /// 0x350
			 0xe9007d38, /// 0x354
			 0x65f39665, /// 0x358
			 0x5ee7f38d, /// 0x35c
			 0xfcdfb76c, /// 0x360
			 0x43694198, /// 0x364
			 0xc891a8fb, /// 0x368
			 0xca75f161, /// 0x36c
			 0x8f2f6e18, /// 0x370
			 0xc56de255, /// 0x374
			 0xf1e63f4e, /// 0x378
			 0xeb08e220, /// 0x37c
			 0x2df55ed5, /// 0x380
			 0xeb3e5b1c, /// 0x384
			 0xbd7b54e7, /// 0x388
			 0xce99d1b3, /// 0x38c
			 0x8c37aa0a, /// 0x390
			 0x6af46603, /// 0x394
			 0xdcd7f379, /// 0x398
			 0xb082d03b, /// 0x39c
			 0xf225e77e, /// 0x3a0
			 0xbbc05b54, /// 0x3a4
			 0x12140370, /// 0x3a8
			 0x0f5aafbc, /// 0x3ac
			 0x8c85c2da, /// 0x3b0
			 0x20206127, /// 0x3b4
			 0x0f6fc5dd, /// 0x3b8
			 0x504ab288, /// 0x3bc
			 0x27af315f, /// 0x3c0
			 0xf897f996, /// 0x3c4
			 0x265d1171, /// 0x3c8
			 0xe3435e49, /// 0x3cc
			 0xba4ccf35, /// 0x3d0
			 0x5426af9e, /// 0x3d4
			 0x738cdbf8, /// 0x3d8
			 0xe26e9c0a, /// 0x3dc
			 0x6916cd54, /// 0x3e0
			 0x0387a1b8, /// 0x3e4
			 0x0073ac37, /// 0x3e8
			 0xb5fa07ff, /// 0x3ec
			 0x74f3bc87, /// 0x3f0
			 0xe83761b2, /// 0x3f4
			 0x18c14d84, /// 0x3f8
			 0xcf9c556c, /// 0x3fc
			 0xd4ce174e, /// 0x400
			 0x0874b5db, /// 0x404
			 0x9c05e6d0, /// 0x408
			 0x7d7c9bd0, /// 0x40c
			 0x12b7e9a4, /// 0x410
			 0x730fc463, /// 0x414
			 0x3fabdf30, /// 0x418
			 0xe347460d, /// 0x41c
			 0x83533d1e, /// 0x420
			 0xc5095784, /// 0x424
			 0x9cf50858, /// 0x428
			 0x8f65a710, /// 0x42c
			 0x827d5312, /// 0x430
			 0xa705d59e, /// 0x434
			 0xe8920132, /// 0x438
			 0x28a73ca5, /// 0x43c
			 0x26e0b0c2, /// 0x440
			 0x59ded450, /// 0x444
			 0xe604e200, /// 0x448
			 0x8751c8fd, /// 0x44c
			 0x3c114b5b, /// 0x450
			 0x23d798a5, /// 0x454
			 0x88da73c6, /// 0x458
			 0x9287fade, /// 0x45c
			 0xbcf813fc, /// 0x460
			 0xca6e48c0, /// 0x464
			 0xff6e50a9, /// 0x468
			 0xf92c4b33, /// 0x46c
			 0xf7eef030, /// 0x470
			 0x58c42729, /// 0x474
			 0x82e05e34, /// 0x478
			 0x4996cd45, /// 0x47c
			 0x5dd1ffa2, /// 0x480
			 0xd70f2382, /// 0x484
			 0xabc74482, /// 0x488
			 0x89072bd5, /// 0x48c
			 0xa7a0aa67, /// 0x490
			 0xa39a2ba4, /// 0x494
			 0xb6a0acc8, /// 0x498
			 0xb101c62e, /// 0x49c
			 0x493ccb58, /// 0x4a0
			 0x1f74c18b, /// 0x4a4
			 0xca5c798a, /// 0x4a8
			 0xed0e9ff0, /// 0x4ac
			 0xd9e58951, /// 0x4b0
			 0xfc4cbe23, /// 0x4b4
			 0x38fdcc26, /// 0x4b8
			 0x375f1ebb, /// 0x4bc
			 0x0ebb7ed2, /// 0x4c0
			 0xabbf3c12, /// 0x4c4
			 0xaafcd6bf, /// 0x4c8
			 0xdb591ea3, /// 0x4cc
			 0xa0ca9a1f, /// 0x4d0
			 0xd64e9fbb, /// 0x4d4
			 0x336bf907, /// 0x4d8
			 0xbbd7d5bd, /// 0x4dc
			 0x66079803, /// 0x4e0
			 0x70749ae3, /// 0x4e4
			 0x3383775a, /// 0x4e8
			 0x7782f6c8, /// 0x4ec
			 0x358745c3, /// 0x4f0
			 0xf03db9e1, /// 0x4f4
			 0xdf9f025a, /// 0x4f8
			 0x6af37609, /// 0x4fc
			 0xa8d5a115, /// 0x500
			 0xd0c5c29c, /// 0x504
			 0xf8135862, /// 0x508
			 0x03174ede, /// 0x50c
			 0xc115291b, /// 0x510
			 0x58966d73, /// 0x514
			 0xb8002059, /// 0x518
			 0x2b7b6b4f, /// 0x51c
			 0xfa62174a, /// 0x520
			 0x6f0770ac, /// 0x524
			 0xc5cfe80c, /// 0x528
			 0xa9c5f187, /// 0x52c
			 0x9a48dfaf, /// 0x530
			 0xdb5b2e6a, /// 0x534
			 0x62069f88, /// 0x538
			 0xf7bb8c63, /// 0x53c
			 0x698c2650, /// 0x540
			 0x52564db4, /// 0x544
			 0x75d4c7e6, /// 0x548
			 0x33a39f6e, /// 0x54c
			 0xdabdcad8, /// 0x550
			 0xb64e1b34, /// 0x554
			 0x7797e0bd, /// 0x558
			 0x5e9b2452, /// 0x55c
			 0x037ca528, /// 0x560
			 0xef8d2c40, /// 0x564
			 0xb1efd356, /// 0x568
			 0x1d6d92ac, /// 0x56c
			 0x4b5663c9, /// 0x570
			 0x0b4cb744, /// 0x574
			 0xfabb6789, /// 0x578
			 0xbc5fa6a1, /// 0x57c
			 0xe46d596a, /// 0x580
			 0x195f98f4, /// 0x584
			 0xb3ff3adf, /// 0x588
			 0x24f2c564, /// 0x58c
			 0x8344ba12, /// 0x590
			 0x9eda064e, /// 0x594
			 0xce8d2947, /// 0x598
			 0xed900e25, /// 0x59c
			 0x56654766, /// 0x5a0
			 0x876a7ac7, /// 0x5a4
			 0xa7b2c3af, /// 0x5a8
			 0xd4e7dcb0, /// 0x5ac
			 0x07d0e86c, /// 0x5b0
			 0x11bf8fea, /// 0x5b4
			 0x4ebec2c6, /// 0x5b8
			 0xb53995af, /// 0x5bc
			 0x874d682d, /// 0x5c0
			 0xcc13d8cf, /// 0x5c4
			 0x8833dc23, /// 0x5c8
			 0x0ed4681c, /// 0x5cc
			 0x1ad9f012, /// 0x5d0
			 0xee534a8f, /// 0x5d4
			 0xca6401ba, /// 0x5d8
			 0x20da0438, /// 0x5dc
			 0x0be3102d, /// 0x5e0
			 0xd90b4434, /// 0x5e4
			 0xe0a5bfa6, /// 0x5e8
			 0x8502163e, /// 0x5ec
			 0x49e90be9, /// 0x5f0
			 0x1661d009, /// 0x5f4
			 0x340bade4, /// 0x5f8
			 0x2e574dab, /// 0x5fc
			 0xf4901aeb, /// 0x600
			 0x1020bd03, /// 0x604
			 0x1bfd5174, /// 0x608
			 0xdc2b498d, /// 0x60c
			 0xf0b6fd1b, /// 0x610
			 0xcea3326e, /// 0x614
			 0xbd357fad, /// 0x618
			 0x173ce67e, /// 0x61c
			 0xfdb9d79f, /// 0x620
			 0x42d1bf64, /// 0x624
			 0xbde6d967, /// 0x628
			 0x232b1b1c, /// 0x62c
			 0x261a2476, /// 0x630
			 0xd946add9, /// 0x634
			 0x7451c8ce, /// 0x638
			 0xe3d02157, /// 0x63c
			 0x2088e793, /// 0x640
			 0x28804b56, /// 0x644
			 0x569338ce, /// 0x648
			 0xd66774ca, /// 0x64c
			 0x922efe04, /// 0x650
			 0x9407139b, /// 0x654
			 0xdcfa0324, /// 0x658
			 0xa056aedd, /// 0x65c
			 0x4fb7ee90, /// 0x660
			 0xc4477e1a, /// 0x664
			 0x6082a7f5, /// 0x668
			 0x53db78d5, /// 0x66c
			 0x76da5fb7, /// 0x670
			 0x79ebd6f8, /// 0x674
			 0xc946e12c, /// 0x678
			 0xabb299e6, /// 0x67c
			 0xe28c1609, /// 0x680
			 0x7e326a20, /// 0x684
			 0x6e1807e1, /// 0x688
			 0xf233ba8c, /// 0x68c
			 0x926b26ec, /// 0x690
			 0x83aacb17, /// 0x694
			 0x44163c50, /// 0x698
			 0x3b22645e, /// 0x69c
			 0xab78b428, /// 0x6a0
			 0x90abde88, /// 0x6a4
			 0x15078424, /// 0x6a8
			 0xc9cf6d15, /// 0x6ac
			 0xd2187219, /// 0x6b0
			 0x0d6b4412, /// 0x6b4
			 0xd0e61e65, /// 0x6b8
			 0xa5255bff, /// 0x6bc
			 0xecb885ee, /// 0x6c0
			 0x53d0f93b, /// 0x6c4
			 0xf8e3e2ec, /// 0x6c8
			 0x8870ece7, /// 0x6cc
			 0x83aa0e71, /// 0x6d0
			 0x83af1118, /// 0x6d4
			 0x3ead4b88, /// 0x6d8
			 0x8a0e8ce8, /// 0x6dc
			 0x29dad709, /// 0x6e0
			 0xc1fe5c91, /// 0x6e4
			 0xfe853db7, /// 0x6e8
			 0x4f051ea6, /// 0x6ec
			 0x4fe2896b, /// 0x6f0
			 0x8e78fa46, /// 0x6f4
			 0x311ef2bd, /// 0x6f8
			 0x794cc84e, /// 0x6fc
			 0x5080fc3e, /// 0x700
			 0xcbf6129b, /// 0x704
			 0x8052fd75, /// 0x708
			 0x76a32489, /// 0x70c
			 0x535b2968, /// 0x710
			 0xe7c1c16c, /// 0x714
			 0xbe3eb46b, /// 0x718
			 0x8888a453, /// 0x71c
			 0xb2f64593, /// 0x720
			 0x03d98ecd, /// 0x724
			 0x788a2cb8, /// 0x728
			 0x78060a89, /// 0x72c
			 0x0e467660, /// 0x730
			 0x94c91a61, /// 0x734
			 0x9b98feed, /// 0x738
			 0x8e37cec6, /// 0x73c
			 0x1c34eeb0, /// 0x740
			 0x451326a3, /// 0x744
			 0x09b0ab6b, /// 0x748
			 0x9013d3c1, /// 0x74c
			 0x24f7e2a1, /// 0x750
			 0x680875e4, /// 0x754
			 0x7f38923c, /// 0x758
			 0xbc7bebed, /// 0x75c
			 0xd993fed1, /// 0x760
			 0x9540bcd3, /// 0x764
			 0x5c30a4ae, /// 0x768
			 0x63b8c411, /// 0x76c
			 0x26a7ff4b, /// 0x770
			 0x42fd661b, /// 0x774
			 0x1eabbb38, /// 0x778
			 0x4d06bf4e, /// 0x77c
			 0xe3f84f60, /// 0x780
			 0x8c838cdd, /// 0x784
			 0xc5477c1f, /// 0x788
			 0xf006ea65, /// 0x78c
			 0x864c819b, /// 0x790
			 0xd2c24a61, /// 0x794
			 0x0c0ed75f, /// 0x798
			 0xad84437e, /// 0x79c
			 0x730b11e1, /// 0x7a0
			 0x40a9078a, /// 0x7a4
			 0xc5bb5c4f, /// 0x7a8
			 0x826c289b, /// 0x7ac
			 0xb6a2ff70, /// 0x7b0
			 0x03680287, /// 0x7b4
			 0xcd5ff7e4, /// 0x7b8
			 0xc0390680, /// 0x7bc
			 0xdbfc9adb, /// 0x7c0
			 0x361e5fd8, /// 0x7c4
			 0x6520b310, /// 0x7c8
			 0x01658230, /// 0x7cc
			 0x4bd3286f, /// 0x7d0
			 0xa83da4a2, /// 0x7d4
			 0xccae4f80, /// 0x7d8
			 0xf0ba8b20, /// 0x7dc
			 0xa5af6181, /// 0x7e0
			 0x49b0bea2, /// 0x7e4
			 0x1d6a9815, /// 0x7e8
			 0x0420f06d, /// 0x7ec
			 0x8da94352, /// 0x7f0
			 0x441e62af, /// 0x7f4
			 0x0479fb50, /// 0x7f8
			 0x7c4f6183, /// 0x7fc
			 0x1dd33d4b, /// 0x800
			 0xcbff26dc, /// 0x804
			 0xc0ecbf1e, /// 0x808
			 0xb3ae2ed5, /// 0x80c
			 0x6994d906, /// 0x810
			 0xd65de5c6, /// 0x814
			 0xa82ac117, /// 0x818
			 0xb595ef9a, /// 0x81c
			 0x769d4e84, /// 0x820
			 0x24332ace, /// 0x824
			 0xf6bfa63b, /// 0x828
			 0xd6f45f9b, /// 0x82c
			 0x2e85dae4, /// 0x830
			 0x0e256a19, /// 0x834
			 0x814ac638, /// 0x838
			 0xe8cbe68f, /// 0x83c
			 0x68ea6e8b, /// 0x840
			 0x756dab16, /// 0x844
			 0x280ce222, /// 0x848
			 0xa88bfea4, /// 0x84c
			 0xba7c5abd, /// 0x850
			 0x6cf28fca, /// 0x854
			 0x0560fcb4, /// 0x858
			 0xcbf5929d, /// 0x85c
			 0x02f119a3, /// 0x860
			 0x29794b6c, /// 0x864
			 0xc8b1eb4a, /// 0x868
			 0xa39cdf68, /// 0x86c
			 0x78f117df, /// 0x870
			 0xae007754, /// 0x874
			 0x4d76bd5b, /// 0x878
			 0xf11cba0f, /// 0x87c
			 0xf33ddcc6, /// 0x880
			 0x731a2cc3, /// 0x884
			 0xb57b229d, /// 0x888
			 0xe0293018, /// 0x88c
			 0xb1984b93, /// 0x890
			 0xa3b3c44f, /// 0x894
			 0x958c6efa, /// 0x898
			 0x40f5f8f3, /// 0x89c
			 0x1fa534fe, /// 0x8a0
			 0x1bde7535, /// 0x8a4
			 0x5157c6fb, /// 0x8a8
			 0x0de5b87c, /// 0x8ac
			 0xa13f6b13, /// 0x8b0
			 0xe1161229, /// 0x8b4
			 0x9873270a, /// 0x8b8
			 0x0f94144e, /// 0x8bc
			 0x20fa3a67, /// 0x8c0
			 0xe9ec3054, /// 0x8c4
			 0x50703fc5, /// 0x8c8
			 0x070703ae, /// 0x8cc
			 0x5fb9ee73, /// 0x8d0
			 0x006516f4, /// 0x8d4
			 0x1f2e58c3, /// 0x8d8
			 0xa6628f5e, /// 0x8dc
			 0x23aacdee, /// 0x8e0
			 0x01cb1608, /// 0x8e4
			 0x6ca95bd8, /// 0x8e8
			 0x8509f4fc, /// 0x8ec
			 0x79c3d15a, /// 0x8f0
			 0x6acf55d7, /// 0x8f4
			 0x82618ab8, /// 0x8f8
			 0x902743e1, /// 0x8fc
			 0x5ea3fe67, /// 0x900
			 0xb6b9075d, /// 0x904
			 0x89bc74b0, /// 0x908
			 0x559796aa, /// 0x90c
			 0xa2975109, /// 0x910
			 0x06b98e7f, /// 0x914
			 0x5704f44d, /// 0x918
			 0x6bccdb29, /// 0x91c
			 0xa7e3c61a, /// 0x920
			 0xd5f21885, /// 0x924
			 0xf41ebfca, /// 0x928
			 0xddbaa0b5, /// 0x92c
			 0x8135cd86, /// 0x930
			 0x6fecaf17, /// 0x934
			 0xf144851e, /// 0x938
			 0x9b9e0238, /// 0x93c
			 0xd513a309, /// 0x940
			 0x57a90e07, /// 0x944
			 0x4a3b772c, /// 0x948
			 0x47f62426, /// 0x94c
			 0xb2e47400, /// 0x950
			 0x8e0fbc56, /// 0x954
			 0x95325cb1, /// 0x958
			 0x11df839b, /// 0x95c
			 0x08dffbcc, /// 0x960
			 0x569d81a1, /// 0x964
			 0xb649b61a, /// 0x968
			 0xe30b0208, /// 0x96c
			 0x04b722c5, /// 0x970
			 0x09a43733, /// 0x974
			 0x2f2908b2, /// 0x978
			 0x9033a352, /// 0x97c
			 0xa0a4ec87, /// 0x980
			 0xa248884d, /// 0x984
			 0x5983064a, /// 0x988
			 0x327c14af, /// 0x98c
			 0x91ccf839, /// 0x990
			 0x2382f5e8, /// 0x994
			 0xee24e306, /// 0x998
			 0xe2eea825, /// 0x99c
			 0x377688a7, /// 0x9a0
			 0x22684c9b, /// 0x9a4
			 0xde532f24, /// 0x9a8
			 0x4d6d24b7, /// 0x9ac
			 0xa76bf7d6, /// 0x9b0
			 0x2d703a94, /// 0x9b4
			 0x04b6a7a5, /// 0x9b8
			 0x78f4adc1, /// 0x9bc
			 0xa9951c83, /// 0x9c0
			 0x9dc00614, /// 0x9c4
			 0xd5f79645, /// 0x9c8
			 0xc58ccc6c, /// 0x9cc
			 0x79178201, /// 0x9d0
			 0x7a2d3b01, /// 0x9d4
			 0x6944a957, /// 0x9d8
			 0x3a6c8fd0, /// 0x9dc
			 0x727e6006, /// 0x9e0
			 0x81aa7684, /// 0x9e4
			 0x812d3179, /// 0x9e8
			 0xde809086, /// 0x9ec
			 0x9348e817, /// 0x9f0
			 0x4191ada6, /// 0x9f4
			 0x0c99b5ab, /// 0x9f8
			 0x4ada4b69, /// 0x9fc
			 0x30637307, /// 0xa00
			 0xf27b8d40, /// 0xa04
			 0x28d11d8a, /// 0xa08
			 0x125d4415, /// 0xa0c
			 0x5b3e629d, /// 0xa10
			 0x12be4ff4, /// 0xa14
			 0xa7c33590, /// 0xa18
			 0xd3e649e2, /// 0xa1c
			 0xe332010b, /// 0xa20
			 0xb560e9c9, /// 0xa24
			 0x5352664f, /// 0xa28
			 0xda8bcc2a, /// 0xa2c
			 0x89381083, /// 0xa30
			 0xbc2f7ae8, /// 0xa34
			 0x637ad74f, /// 0xa38
			 0x17715f17, /// 0xa3c
			 0xe3daa2e6, /// 0xa40
			 0x205be9b8, /// 0xa44
			 0xc02421bd, /// 0xa48
			 0xf40fa6c1, /// 0xa4c
			 0x8a4250ab, /// 0xa50
			 0xce936d69, /// 0xa54
			 0xba88fd64, /// 0xa58
			 0xb1d228d8, /// 0xa5c
			 0xdc5df4a4, /// 0xa60
			 0x57aeb335, /// 0xa64
			 0x9ea17d70, /// 0xa68
			 0x6155e5b2, /// 0xa6c
			 0xd5d6fc11, /// 0xa70
			 0x1097ad76, /// 0xa74
			 0x2721b7ab, /// 0xa78
			 0x116d74ac, /// 0xa7c
			 0x714e9ac0, /// 0xa80
			 0xa1342903, /// 0xa84
			 0x742230d7, /// 0xa88
			 0xa1ab268b, /// 0xa8c
			 0xbc5cdb9d, /// 0xa90
			 0x90c29a85, /// 0xa94
			 0x7548a7db, /// 0xa98
			 0x53deb5f6, /// 0xa9c
			 0x46f25ac1, /// 0xaa0
			 0x1ce71da5, /// 0xaa4
			 0x619f3e27, /// 0xaa8
			 0xd4dd4681, /// 0xaac
			 0x4c6c6a20, /// 0xab0
			 0x73519d0a, /// 0xab4
			 0x6f5c969f, /// 0xab8
			 0x44403e8f, /// 0xabc
			 0x0eee2361, /// 0xac0
			 0x60154b4c, /// 0xac4
			 0x3c6c70ba, /// 0xac8
			 0x817af11b, /// 0xacc
			 0xf3b84883, /// 0xad0
			 0x32f1b3b7, /// 0xad4
			 0x4ec2ebbf, /// 0xad8
			 0x5897fc16, /// 0xadc
			 0x997a95a2, /// 0xae0
			 0x67baafd2, /// 0xae4
			 0x86852c4d, /// 0xae8
			 0xf006e0f2, /// 0xaec
			 0x6706b960, /// 0xaf0
			 0xd9e054c1, /// 0xaf4
			 0x9cd49a86, /// 0xaf8
			 0x15926ca3, /// 0xafc
			 0x374a3dad, /// 0xb00
			 0xc5f34d45, /// 0xb04
			 0x7daa6b6c, /// 0xb08
			 0x180d7fbe, /// 0xb0c
			 0x2792af9f, /// 0xb10
			 0xc5b1cd7e, /// 0xb14
			 0xef85d304, /// 0xb18
			 0x051f7f8f, /// 0xb1c
			 0xec7c217f, /// 0xb20
			 0x66c3c670, /// 0xb24
			 0x12d5d6e0, /// 0xb28
			 0x9ff7ea17, /// 0xb2c
			 0xb9222a7d, /// 0xb30
			 0xacac66f4, /// 0xb34
			 0x766ee6f9, /// 0xb38
			 0x272d47f4, /// 0xb3c
			 0xe8cb91f6, /// 0xb40
			 0x5c3b07f9, /// 0xb44
			 0xd01343e2, /// 0xb48
			 0xe862c827, /// 0xb4c
			 0xc676a790, /// 0xb50
			 0x520a4443, /// 0xb54
			 0xc84273cc, /// 0xb58
			 0x707bde1f, /// 0xb5c
			 0x31a9f483, /// 0xb60
			 0x5a3c17e2, /// 0xb64
			 0x79efb1d5, /// 0xb68
			 0xc93dc9d0, /// 0xb6c
			 0xb8924ae3, /// 0xb70
			 0xd1d47e33, /// 0xb74
			 0x7faead01, /// 0xb78
			 0xf3014405, /// 0xb7c
			 0x9a77d9e8, /// 0xb80
			 0x5dd55c89, /// 0xb84
			 0x84e89dc8, /// 0xb88
			 0x1d9ed4a5, /// 0xb8c
			 0x548f187f, /// 0xb90
			 0x5eccdd0b, /// 0xb94
			 0x866541bd, /// 0xb98
			 0xfb720590, /// 0xb9c
			 0x8f14693a, /// 0xba0
			 0x47f68b1e, /// 0xba4
			 0xb03fef89, /// 0xba8
			 0x2a511716, /// 0xbac
			 0x25b038e4, /// 0xbb0
			 0x3da13041, /// 0xbb4
			 0x721d3cd5, /// 0xbb8
			 0xe1e052c6, /// 0xbbc
			 0x16e9bbb1, /// 0xbc0
			 0x4b68df91, /// 0xbc4
			 0x99f09d96, /// 0xbc8
			 0x3de3ab7b, /// 0xbcc
			 0x74265c7a, /// 0xbd0
			 0xd8f1429d, /// 0xbd4
			 0x3f329b2f, /// 0xbd8
			 0x2bf865b7, /// 0xbdc
			 0xf9118d50, /// 0xbe0
			 0x44864dcf, /// 0xbe4
			 0xb1c5193a, /// 0xbe8
			 0xf0738500, /// 0xbec
			 0xab8a8eb0, /// 0xbf0
			 0xb1d880c0, /// 0xbf4
			 0xc6d4f313, /// 0xbf8
			 0xa66dea0b, /// 0xbfc
			 0x145c4a80, /// 0xc00
			 0xe3c470c0, /// 0xc04
			 0x30999750, /// 0xc08
			 0x3bc79adb, /// 0xc0c
			 0x1b748831, /// 0xc10
			 0xc8fd8742, /// 0xc14
			 0x90a66143, /// 0xc18
			 0x99c6421d, /// 0xc1c
			 0x9c38a283, /// 0xc20
			 0x73aaedca, /// 0xc24
			 0x9a08e23c, /// 0xc28
			 0xd187cc7d, /// 0xc2c
			 0x469f7054, /// 0xc30
			 0x720675fb, /// 0xc34
			 0xc51a0655, /// 0xc38
			 0x09294417, /// 0xc3c
			 0x59142dba, /// 0xc40
			 0x42aac0d5, /// 0xc44
			 0xa3e27346, /// 0xc48
			 0x82e8af9a, /// 0xc4c
			 0xe9e11d9e, /// 0xc50
			 0xf8e92fc6, /// 0xc54
			 0xf24d8e63, /// 0xc58
			 0x48d0d7f8, /// 0xc5c
			 0x676450f5, /// 0xc60
			 0x403a61c3, /// 0xc64
			 0xeb28b0d8, /// 0xc68
			 0x5dbafd12, /// 0xc6c
			 0x6d349664, /// 0xc70
			 0x5bc4f2c2, /// 0xc74
			 0xc33e4da7, /// 0xc78
			 0x4f0ee03b, /// 0xc7c
			 0xebb4b62d, /// 0xc80
			 0x28e9454f, /// 0xc84
			 0x1f4843b4, /// 0xc88
			 0x146fe8f5, /// 0xc8c
			 0x238151e8, /// 0xc90
			 0x9818de41, /// 0xc94
			 0x05d11afe, /// 0xc98
			 0x0eab955c, /// 0xc9c
			 0xcf7fbc14, /// 0xca0
			 0xdf37c889, /// 0xca4
			 0x7ea4bfe0, /// 0xca8
			 0x05ad3953, /// 0xcac
			 0xba1b8d03, /// 0xcb0
			 0xc54eaa0b, /// 0xcb4
			 0xdcac01ba, /// 0xcb8
			 0x6e8864a3, /// 0xcbc
			 0x0d3d331a, /// 0xcc0
			 0xd1e7472d, /// 0xcc4
			 0x093a0bd8, /// 0xcc8
			 0xced47807, /// 0xccc
			 0x002cdbc1, /// 0xcd0
			 0x4d9294c7, /// 0xcd4
			 0x14d42a15, /// 0xcd8
			 0xa8c276bd, /// 0xcdc
			 0xb97709b3, /// 0xce0
			 0x00682852, /// 0xce4
			 0x77ce2833, /// 0xce8
			 0xbc68d4bd, /// 0xcec
			 0xf6310427, /// 0xcf0
			 0x54a8d5cb, /// 0xcf4
			 0xc7fafee8, /// 0xcf8
			 0xa23c91bd, /// 0xcfc
			 0x13fb6aac, /// 0xd00
			 0xd9fdd528, /// 0xd04
			 0xb3715bae, /// 0xd08
			 0x3cb90cbd, /// 0xd0c
			 0x10cac527, /// 0xd10
			 0x13d8932b, /// 0xd14
			 0x6fb4c76e, /// 0xd18
			 0xadba14c7, /// 0xd1c
			 0xcb8dc846, /// 0xd20
			 0xff301820, /// 0xd24
			 0xd74cb3ac, /// 0xd28
			 0x944f09d1, /// 0xd2c
			 0x51e07f03, /// 0xd30
			 0xea08c0c1, /// 0xd34
			 0x865c706d, /// 0xd38
			 0xe46927b4, /// 0xd3c
			 0x3cdb4091, /// 0xd40
			 0x94bdf391, /// 0xd44
			 0xe9973264, /// 0xd48
			 0x5528efa0, /// 0xd4c
			 0x9a1d4de5, /// 0xd50
			 0x3a531e7a, /// 0xd54
			 0x313be4eb, /// 0xd58
			 0x5d065520, /// 0xd5c
			 0x8cf1d4e7, /// 0xd60
			 0xa6dc8d00, /// 0xd64
			 0x7fef0c01, /// 0xd68
			 0x73b47856, /// 0xd6c
			 0x2ea2603d, /// 0xd70
			 0x2731c0c7, /// 0xd74
			 0x7070a104, /// 0xd78
			 0x90eb56c8, /// 0xd7c
			 0x0ae7a821, /// 0xd80
			 0xb0002750, /// 0xd84
			 0x8d835591, /// 0xd88
			 0x03c113a2, /// 0xd8c
			 0x6939fcb6, /// 0xd90
			 0x7bb49e2b, /// 0xd94
			 0xa82fe904, /// 0xd98
			 0x2206d1db, /// 0xd9c
			 0x08b5c4f7, /// 0xda0
			 0xcca4b142, /// 0xda4
			 0x3545d8b2, /// 0xda8
			 0x38e75e5e, /// 0xdac
			 0xe80c5e89, /// 0xdb0
			 0x91836f73, /// 0xdb4
			 0x04b78034, /// 0xdb8
			 0xd8c5a232, /// 0xdbc
			 0xa7b17613, /// 0xdc0
			 0x3110a177, /// 0xdc4
			 0xa41ae901, /// 0xdc8
			 0x627b4a04, /// 0xdcc
			 0xb180e197, /// 0xdd0
			 0x408e0bc0, /// 0xdd4
			 0xb436194f, /// 0xdd8
			 0x440f49ab, /// 0xddc
			 0x5cc6262b, /// 0xde0
			 0x098e479f, /// 0xde4
			 0x8d1a78ff, /// 0xde8
			 0xcd2033b4, /// 0xdec
			 0x2bfdd524, /// 0xdf0
			 0xd72babec, /// 0xdf4
			 0x38824e46, /// 0xdf8
			 0xef9469c1, /// 0xdfc
			 0x273107df, /// 0xe00
			 0xa9059332, /// 0xe04
			 0x73712c68, /// 0xe08
			 0xd2c4640f, /// 0xe0c
			 0x8e37e025, /// 0xe10
			 0xa5fe7379, /// 0xe14
			 0x8439a947, /// 0xe18
			 0x05ef10d8, /// 0xe1c
			 0x3dd80429, /// 0xe20
			 0x821572ef, /// 0xe24
			 0xfab6eea3, /// 0xe28
			 0xad77e7c7, /// 0xe2c
			 0xb84bb481, /// 0xe30
			 0x2495c817, /// 0xe34
			 0x16ab7d23, /// 0xe38
			 0xa17c6811, /// 0xe3c
			 0x15aa0e26, /// 0xe40
			 0x2f1a337c, /// 0xe44
			 0x009ad047, /// 0xe48
			 0x5a017acc, /// 0xe4c
			 0xff9614ff, /// 0xe50
			 0x7b32b4e1, /// 0xe54
			 0x71358c95, /// 0xe58
			 0xa87200b2, /// 0xe5c
			 0x0daea448, /// 0xe60
			 0xd2a36bc7, /// 0xe64
			 0x6677b7dc, /// 0xe68
			 0x123945c7, /// 0xe6c
			 0x0506fa5d, /// 0xe70
			 0xbb16f99c, /// 0xe74
			 0xaf97ee38, /// 0xe78
			 0x3d8ff32c, /// 0xe7c
			 0xf79884ea, /// 0xe80
			 0x0d7fd8e9, /// 0xe84
			 0xea850ea1, /// 0xe88
			 0x004ffabb, /// 0xe8c
			 0x9f629014, /// 0xe90
			 0x99680270, /// 0xe94
			 0xcaf96b62, /// 0xe98
			 0x0a0144ea, /// 0xe9c
			 0xe23402f8, /// 0xea0
			 0x0f206820, /// 0xea4
			 0xd81c93fc, /// 0xea8
			 0x59b1999b, /// 0xeac
			 0xfcb16b7e, /// 0xeb0
			 0x7d414ddc, /// 0xeb4
			 0x52cf38b2, /// 0xeb8
			 0xfbf31b86, /// 0xebc
			 0xad9f87f8, /// 0xec0
			 0x5dff6384, /// 0xec4
			 0x8a35adc4, /// 0xec8
			 0x35a2aa78, /// 0xecc
			 0x7e50d6c9, /// 0xed0
			 0xddbce282, /// 0xed4
			 0xe082a36c, /// 0xed8
			 0x11530f4c, /// 0xedc
			 0x87a8a7f2, /// 0xee0
			 0xd6c1f6b7, /// 0xee4
			 0x7dbfcaf4, /// 0xee8
			 0x7c4d5e16, /// 0xeec
			 0x8497fa6b, /// 0xef0
			 0x5efe2684, /// 0xef4
			 0x4f676898, /// 0xef8
			 0xd13c01de, /// 0xefc
			 0x38bd0b67, /// 0xf00
			 0xc1efb805, /// 0xf04
			 0x1d5377dc, /// 0xf08
			 0x8eeddc44, /// 0xf0c
			 0xfb6fd777, /// 0xf10
			 0x4cb52ff5, /// 0xf14
			 0x4a14964c, /// 0xf18
			 0xd57414db, /// 0xf1c
			 0xcf061896, /// 0xf20
			 0x6bb96c66, /// 0xf24
			 0xd42440e4, /// 0xf28
			 0x8ea56e34, /// 0xf2c
			 0x0052c941, /// 0xf30
			 0xec7de1a8, /// 0xf34
			 0x2a9dce79, /// 0xf38
			 0xdff4f17e, /// 0xf3c
			 0x1b16ae7f, /// 0xf40
			 0x68bb7ba7, /// 0xf44
			 0x978398a8, /// 0xf48
			 0x8d705f0e, /// 0xf4c
			 0xed677569, /// 0xf50
			 0x6ebbc731, /// 0xf54
			 0x4f1f37be, /// 0xf58
			 0x30c7cd03, /// 0xf5c
			 0x01494407, /// 0xf60
			 0xe4dc743f, /// 0xf64
			 0x8fe27b59, /// 0xf68
			 0xa3277853, /// 0xf6c
			 0xd2e2d738, /// 0xf70
			 0x7aa8d166, /// 0xf74
			 0x4aff8768, /// 0xf78
			 0x6805318f, /// 0xf7c
			 0xa68521aa, /// 0xf80
			 0x805840fa, /// 0xf84
			 0x6dd9bdda, /// 0xf88
			 0x079c8c80, /// 0xf8c
			 0x0a7d926e, /// 0xf90
			 0xd528c716, /// 0xf94
			 0x615332bf, /// 0xf98
			 0x2dbdcf9d, /// 0xf9c
			 0x662b6f77, /// 0xfa0
			 0x0f2452d5, /// 0xfa4
			 0xdf8edd27, /// 0xfa8
			 0x36254497, /// 0xfac
			 0xa348bb09, /// 0xfb0
			 0xd48913dc, /// 0xfb4
			 0x3f94234e, /// 0xfb8
			 0x7afa1ea6, /// 0xfbc
			 0xdc8ec173, /// 0xfc0
			 0x8d3a14f5, /// 0xfc4
			 0x104720b2, /// 0xfc8
			 0xa64652a4, /// 0xfcc
			 0xc992b510, /// 0xfd0
			 0xa09cc959, /// 0xfd4
			 0x235cf4b9, /// 0xfd8
			 0xbe37eb6a, /// 0xfdc
			 0x9c02284e, /// 0xfe0
			 0xff27b827, /// 0xfe4
			 0x3a0de053, /// 0xfe8
			 0x19700f1c, /// 0xfec
			 0x3d3055f4, /// 0xff0
			 0x6970e86b, /// 0xff4
			 0x9c36c000, /// 0xff8
			 0xfa9a0398 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x55555555,                                                  // 4 random values                             /// 00000
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00004
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00008
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00010
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00014
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00018
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00024
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00028
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0003c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00048
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00050
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00054
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00058
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0005c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00060
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0006c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00074
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00080
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00084
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00088
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0008c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00090
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00098
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0009c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00108
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0010c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00118
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00120
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00124
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00128
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00138
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00140
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00148
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00150
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00154
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00160
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00164
			 0xff800000,                                                  // -inf                                        /// 00168
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0016c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00178
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0017c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00184
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00188
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0018c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0019c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x55555555,                                                  // 4 random values                             /// 001c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001d8
			 0xffc00001,                                                  // -signaling NaN                              /// 001dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00208
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0020c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00210
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00214
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00218
			 0x7fc00001,                                                  // signaling NaN                               /// 0021c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00220
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00224
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00234
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0023c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00240
			 0x0e000001,                                                  // Trailing 1s:                                /// 00244
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00248
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0024c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00254
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00258
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0025c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00260
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00264
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00268
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00274
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00278
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0027c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00280
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00284
			 0x00000000,                                                  // zero                                        /// 00288
			 0x0c700000,                                                  // Leading 1s:                                 /// 0028c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 002d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00300
			 0x55555555,                                                  // 4 random values                             /// 00304
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00308
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0030c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00318
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0031c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00320
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00324
			 0x00000000,                                                  // zero                                        /// 00328
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00330
			 0xff800000,                                                  // -inf                                        /// 00334
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0033c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00340
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0034c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00350
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00354
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00358
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0035c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00360
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00364
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00368
			 0x7fc00001,                                                  // signaling NaN                               /// 0036c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00378
			 0x7f800000,                                                  // inf                                         /// 0037c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00380
			 0xbf028f5c,                                                  // -0.51                                       /// 00384
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00388
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0038c
			 0xff800000,                                                  // -inf                                        /// 00390
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0039c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80000000,                                                  // -zero                                       /// 003a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0x33333333,                                                  // 4 random values                             /// 003c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00400
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00404
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00408
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0040c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00410
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00414
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00418
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00420
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00428
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00430
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00438
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0043c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00440
			 0x00011111,                                                  // 9.7958E-41                                  /// 00444
			 0x3f028f5c,                                                  // 0.51                                        /// 00448
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0044c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00454
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00458
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00464
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00468
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0046c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00470
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00474
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0047c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00480
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00484
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0048c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00490
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00498
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0049c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00500
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00504
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00508
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0050c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00510
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00518
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0051c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00520
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00524
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00528
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00530
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00534
			 0xcb000000,                                                  // -8388608.0                                  /// 00538
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00540
			 0x0e000003,                                                  // Trailing 1s:                                /// 00544
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x33333333,                                                  // 4 random values                             /// 00550
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00554
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0e000003,                                                  // Trailing 1s:                                /// 0055c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00560
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00564
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00568
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0056c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00570
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00574
			 0x0c600000,                                                  // Leading 1s:                                 /// 00578
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0057c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00580
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00584
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0058c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00590
			 0x33333333,                                                  // 4 random values                             /// 00594
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00598
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0xcb000000,                                                  // -8388608.0                                  /// 005a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x55555555,                                                  // 4 random values                             /// 005ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005dc
			 0x33333333,                                                  // 4 random values                             /// 005e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00600
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00604
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00608
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00610
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0061c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00620
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00624
			 0x33333333,                                                  // 4 random values                             /// 00628
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00630
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0xcb000000,                                                  // -8388608.0                                  /// 00638
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0063c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00640
			 0x33333333,                                                  // 4 random values                             /// 00644
			 0x80000000,                                                  // -zero                                       /// 00648
			 0x80000000,                                                  // -zero                                       /// 0064c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00650
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00654
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0065c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00664
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00668
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0066c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00678
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x0c600000,                                                  // Leading 1s:                                 /// 00684
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00688
			 0xffc00001,                                                  // -signaling NaN                              /// 0068c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00694
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0069c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00700
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e000001,                                                  // Trailing 1s:                                /// 0070c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00710
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00718
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0071c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00720
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00724
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00728
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00730
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00734
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00738
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0073c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00740
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00758
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0075c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00760
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x7fc00001,                                                  // signaling NaN                               /// 0076c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00778
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0xcb000000,                                                  // -8388608.0                                  /// 00780
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00784
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00788
			 0xbf028f5c,                                                  // -0.51                                       /// 0078c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00794
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00798
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007b0
			 0x80000000,                                                  // -zero                                       /// 007b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d0
			 0x7fc00001,                                                  // signaling NaN                               /// 007d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00800
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00804
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0080c
			 0xffc00001,                                                  // -signaling NaN                              /// 00810
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00818
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0081c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00820
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00828
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00000000,                                                  // zero                                        /// 00830
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00834
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0083c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00840
			 0x0c780000,                                                  // Leading 1s:                                 /// 00844
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0084c
			 0x7f800000,                                                  // inf                                         /// 00850
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00854
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0085c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00860
			 0x0c400000,                                                  // Leading 1s:                                 /// 00864
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00868
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00874
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00878
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0087c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00884
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00888
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00890
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00894
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00898
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0089c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008a8
			 0x80000000,                                                  // -zero                                       /// 008ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 008b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008c8
			 0xcb000000,                                                  // -8388608.0                                  /// 008cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 008fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00900
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0e000003,                                                  // Trailing 1s:                                /// 0090c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00910
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00918
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x7fc00001,                                                  // signaling NaN                               /// 00920
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00924
			 0x0c780000,                                                  // Leading 1s:                                 /// 00928
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0092c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00930
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00938
			 0x33333333,                                                  // 4 random values                             /// 0093c
			 0x55555555,                                                  // 4 random values                             /// 00940
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00948
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0094c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00950
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00954
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00958
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0095c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00960
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0096c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00974
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0097c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00980
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0e000007,                                                  // Trailing 1s:                                /// 0098c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00990
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0xbf028f5c,                                                  // -0.51                                       /// 009a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a00
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a24
			 0x00000000,                                                  // zero                                        /// 00a28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a48
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a6c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a98
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a9c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00aa0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00aa4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aa8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00aac
			 0xcb000000,                                                  // -8388608.0                                  /// 00ab0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ab8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ac4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00acc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ad4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ad8
			 0x7f800000,                                                  // inf                                         /// 00adc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ae4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ae8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00af0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00af4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00af8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b14
			 0xcb000000,                                                  // -8388608.0                                  /// 00b18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b1c
			 0xff800000,                                                  // -inf                                        /// 00b20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b30
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b34
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b68
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b6c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b98
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ba0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ba8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bb4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bb8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bbc
			 0xff800000,                                                  // -inf                                        /// 00bc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bc8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bcc
			 0x7f800000,                                                  // inf                                         /// 00bd0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00be0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x3f028f5c,                                                  // 0.51                                        /// 00bf4
			 0xcb000000,                                                  // -8388608.0                                  /// 00bf8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bfc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c00
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c08
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x33333333,                                                  // 4 random values                             /// 00c20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c6c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c80
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c90
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ca0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ca8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cac
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cb0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cb4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cc0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cc8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ccc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cd8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cdc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ce4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cfc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d24
			 0xff800000,                                                  // -inf                                        /// 00d28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d58
			 0x7fc00001,                                                  // signaling NaN                               /// 00d5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d90
			 0x55555555,                                                  // 4 random values                             /// 00d94
			 0x4b000000,                                                  // 8388608.0                                   /// 00d98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00da4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00da8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00db8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dc4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dc8
			 0x33333333,                                                  // 4 random values                             /// 00dcc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00dd4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ddc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00de4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dec
			 0x55555555,                                                  // 4 random values                             /// 00df0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00df8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dfc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0xff800000,                                                  // -inf                                        /// 00e08
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e0c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0xcb000000,                                                  // -8388608.0                                  /// 00e24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e2c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e44
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e48
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e60
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e84
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e94
			 0xff800000,                                                  // -inf                                        /// 00e98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e9c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ea0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ea4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ea8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00eac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00eb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ec4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ed0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ee0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ee8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x3f028f5c,                                                  // 0.51                                        /// 00ef0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ef4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f24
			 0xcb000000,                                                  // -8388608.0                                  /// 00f28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f58
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f5c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f60
			 0xcb000000,                                                  // -8388608.0                                  /// 00f64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fa8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fc0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fcc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fdc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fec
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ff0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ff8
			 0x80000004                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00000
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00004
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0000c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00020
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00028
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0002c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00030
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0003c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x7fc00001,                                                  // signaling NaN                               /// 00044
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00048
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00050
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00060
			 0x0c400000,                                                  // Leading 1s:                                 /// 00064
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00068
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00070
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00074
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00078
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00080
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00088
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0008c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00098
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0009c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000cc
			 0x3f028f5c,                                                  // 0.51                                        /// 000d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000e4
			 0x7fc00001,                                                  // signaling NaN                               /// 000e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00100
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00104
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00108
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0010c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00110
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00114
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00118
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0011c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00120
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00128
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0012c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00134
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00138
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0013c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0014c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0c780000,                                                  // Leading 1s:                                 /// 00154
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0015c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00160
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00168
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00170
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00178
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0017c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00184
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0018c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001ec
			 0xcb000000,                                                  // -8388608.0                                  /// 001f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00200
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00208
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00214
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00220
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00228
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0022c
			 0x80000000,                                                  // -zero                                       /// 00230
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00234
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00240
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0024c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00250
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00254
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00258
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0025c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00260
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00264
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00268
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0026c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00274
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0027c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00284
			 0x4b000000,                                                  // 8388608.0                                   /// 00288
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00294
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0029c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 002b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00300
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0xffc00001,                                                  // -signaling NaN                              /// 0030c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00310
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0031c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00320
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00328
			 0x00000000,                                                  // zero                                        /// 0032c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00330
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00334
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00338
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0033c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00340
			 0x00000000,                                                  // zero                                        /// 00344
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00348
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0034c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00358
			 0x7f800000,                                                  // inf                                         /// 0035c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00360
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00368
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0036c
			 0xff800000,                                                  // -inf                                        /// 00370
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00378
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00384
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00388
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00390
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00398
			 0xffc00001,                                                  // -signaling NaN                              /// 0039c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00000000,                                                  // zero                                        /// 00404
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00408
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00410
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00414
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00420
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00424
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0xffc00001,                                                  // -signaling NaN                              /// 00434
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00438
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0043c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00440
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00444
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00448
			 0x0e000003,                                                  // Trailing 1s:                                /// 0044c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00450
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00458
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0045c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00460
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00468
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00470
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00474
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00478
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0047c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00480
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00488
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0048c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00490
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00494
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00498
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004d8
			 0x55555555,                                                  // 4 random values                             /// 004dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00500
			 0xffc00001,                                                  // -signaling NaN                              /// 00504
			 0x0e000001,                                                  // Trailing 1s:                                /// 00508
			 0x33333333,                                                  // 4 random values                             /// 0050c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00510
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00514
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00520
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00528
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0052c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00530
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00534
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00540
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00544
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00548
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00550
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00558
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00560
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00574
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00578
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0057c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00000000,                                                  // zero                                        /// 00584
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00588
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0058c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00590
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00598
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005a0
			 0x00000000,                                                  // zero                                        /// 005a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x33333333,                                                  // 4 random values                             /// 005c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005dc
			 0x33333333,                                                  // 4 random values                             /// 005e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005fc
			 0x33333333,                                                  // 4 random values                             /// 00600
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00604
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00608
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0060c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00610
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00614
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00624
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00628
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x00000000,                                                  // zero                                        /// 00630
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0063c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00644
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0064c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00654
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00658
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00660
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00674
			 0x0c600000,                                                  // Leading 1s:                                 /// 00678
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0067c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00680
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00684
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00690
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00694
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00011111,                                                  // 9.7958E-41                                  /// 0069c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006e0
			 0x7fc00001,                                                  // signaling NaN                               /// 006e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00704
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00708
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0070c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00714
			 0x0c780000,                                                  // Leading 1s:                                 /// 00718
			 0x80011111,                                                  // -9.7958E-41                                 /// 0071c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00720
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00724
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00738
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0073c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00740
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00744
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00748
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00750
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00758
			 0xbf028f5c,                                                  // -0.51                                       /// 0075c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00764
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00768
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0076c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0xffc00001,                                                  // -signaling NaN                              /// 00778
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0077c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00780
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00790
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00794
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00798
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0079c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007b0
			 0x3f028f5c,                                                  // 0.51                                        /// 007b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00800
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00804
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00820
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00824
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0082c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00834
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00838
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0083c
			 0xbf028f5c,                                                  // -0.51                                       /// 00840
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00844
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00850
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00854
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0085c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00864
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00868
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00870
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00874
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x55555555,                                                  // 4 random values                             /// 0087c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00880
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00884
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0088c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00890
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x55555555,                                                  // 4 random values                             /// 00898
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0089c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 008bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00900
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00904
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00910
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00924
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00928
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0092c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00930
			 0x80000000,                                                  // -zero                                       /// 00934
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00938
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0093c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00940
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00948
			 0x7fc00001,                                                  // signaling NaN                               /// 0094c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00950
			 0x80011111,                                                  // -9.7958E-41                                 /// 00954
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0095c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0096c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00974
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00978
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0097c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x7f800000,                                                  // inf                                         /// 00994
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0099c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b4
			 0xffc00001,                                                  // -signaling NaN                              /// 009b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009c4
			 0xffc00001,                                                  // -signaling NaN                              /// 009c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 009d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f4
			 0x4b000000,                                                  // 8388608.0                                   /// 009f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00a00
			 0xffc00001,                                                  // -signaling NaN                              /// 00a04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a18
			 0x4b000000,                                                  // 8388608.0                                   /// 00a1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a40
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a48
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a64
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x7f800000,                                                  // inf                                         /// 00a7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x80000000,                                                  // -zero                                       /// 00a90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aa0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aa4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aa8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ab8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ac4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ac8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00acc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ad0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ae0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ae4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ae8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00aec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00af4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b10
			 0xbf028f5c,                                                  // -0.51                                       /// 00b14
			 0x7fc00001,                                                  // signaling NaN                               /// 00b18
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b28
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b2c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b30
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b38
			 0x7fc00001,                                                  // signaling NaN                               /// 00b3c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b60
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b98
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ba4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x7f800000,                                                  // inf                                         /// 00bac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bb0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bb4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bb8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bc0
			 0x80000000,                                                  // -zero                                       /// 00bc4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bc8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bcc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x80000000,                                                  // -zero                                       /// 00bd4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00be0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0xff800000,                                                  // -inf                                        /// 00be8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bf8
			 0x80000000,                                                  // -zero                                       /// 00bfc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c38
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x7f800000,                                                  // inf                                         /// 00c58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c5c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c64
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c70
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c74
			 0x55555555,                                                  // 4 random values                             /// 00c78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c94
			 0x7fc00001,                                                  // signaling NaN                               /// 00c98
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c9c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ca4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cbc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ccc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cd0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cd8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cdc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ce4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ce8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cf8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cfc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d00
			 0x80000000,                                                  // -zero                                       /// 00d04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d10
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d2c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d30
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d38
			 0x4b000000,                                                  // 8388608.0                                   /// 00d3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d74
			 0x55555555,                                                  // 4 random values                             /// 00d78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x7f800000,                                                  // inf                                         /// 00d90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d94
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00db0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00db4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dc0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dcc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dd8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ddc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00de0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00de8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dec
			 0x33333333,                                                  // 4 random values                             /// 00df0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dfc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e30
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e34
			 0x7fc00001,                                                  // signaling NaN                               /// 00e38
			 0xbf028f5c,                                                  // -0.51                                       /// 00e3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e48
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e50
			 0xbf028f5c,                                                  // -0.51                                       /// 00e54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e5c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ea4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ea8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x7f800000,                                                  // inf                                         /// 00eb4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00eb8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ebc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x7f800000,                                                  // inf                                         /// 00ec8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ecc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ed0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ed8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ee0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ee8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ef0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ef4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00efc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f1c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f20
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f28
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f48
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f58
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f8c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f90
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f94
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fa4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fa8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fcc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fd0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fd4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fe4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ff4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0x00200000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x385125b2, /// 0x0
			 0xce6c65d4, /// 0x4
			 0x472a65ab, /// 0x8
			 0xd95f7d4d, /// 0xc
			 0x31c0e85d, /// 0x10
			 0x8daa65cf, /// 0x14
			 0x84a8e9b4, /// 0x18
			 0xb2db33d8, /// 0x1c
			 0x45bb058f, /// 0x20
			 0xfd8ade0a, /// 0x24
			 0xfe7cf8cc, /// 0x28
			 0x1ba03751, /// 0x2c
			 0x0b6218d0, /// 0x30
			 0xa7acfdb6, /// 0x34
			 0xb3a8cada, /// 0x38
			 0x59074505, /// 0x3c
			 0x72798759, /// 0x40
			 0x012a73a6, /// 0x44
			 0xf27c9ad5, /// 0x48
			 0xc37548b9, /// 0x4c
			 0xfd7a7843, /// 0x50
			 0xfe470d45, /// 0x54
			 0x3194d130, /// 0x58
			 0x966bf16f, /// 0x5c
			 0x45292a8a, /// 0x60
			 0xe05e41ba, /// 0x64
			 0xef2954d5, /// 0x68
			 0x1b817e9a, /// 0x6c
			 0x6a0da3ee, /// 0x70
			 0x31879080, /// 0x74
			 0x63312867, /// 0x78
			 0xe9746d63, /// 0x7c
			 0x243dded4, /// 0x80
			 0x16067ff0, /// 0x84
			 0x0e5effa3, /// 0x88
			 0xd0670023, /// 0x8c
			 0xc1147c81, /// 0x90
			 0xf95e6d94, /// 0x94
			 0xb67283e4, /// 0x98
			 0xaf21fa2d, /// 0x9c
			 0xfb362fb5, /// 0xa0
			 0x812be00e, /// 0xa4
			 0xd7cf419d, /// 0xa8
			 0x86702007, /// 0xac
			 0x38c84aae, /// 0xb0
			 0x8066887a, /// 0xb4
			 0x3e8854cc, /// 0xb8
			 0xb83da0fa, /// 0xbc
			 0x3b2e0d8c, /// 0xc0
			 0x8324496c, /// 0xc4
			 0x41cf928a, /// 0xc8
			 0x9869798a, /// 0xcc
			 0x04494382, /// 0xd0
			 0xea2ccf1f, /// 0xd4
			 0xb44ce134, /// 0xd8
			 0x778f3bc4, /// 0xdc
			 0xc2f6970e, /// 0xe0
			 0x29b5e2a3, /// 0xe4
			 0xaa89de8a, /// 0xe8
			 0x9e05f793, /// 0xec
			 0xdb59b002, /// 0xf0
			 0x5e6c285e, /// 0xf4
			 0x91156dc1, /// 0xf8
			 0xc53c91f9, /// 0xfc
			 0x26921867, /// 0x100
			 0x4a91ee00, /// 0x104
			 0x18e216ef, /// 0x108
			 0x5edd0868, /// 0x10c
			 0x011200dd, /// 0x110
			 0x2c1c99cd, /// 0x114
			 0x9ff96389, /// 0x118
			 0xb2d3b260, /// 0x11c
			 0x37bdf7cc, /// 0x120
			 0xbf62685a, /// 0x124
			 0x29ccf75b, /// 0x128
			 0x0a68f6ba, /// 0x12c
			 0x6230e611, /// 0x130
			 0x64f30c90, /// 0x134
			 0x8ef021c6, /// 0x138
			 0xfced863f, /// 0x13c
			 0x3f752da9, /// 0x140
			 0xb58c15a9, /// 0x144
			 0x0af50778, /// 0x148
			 0xfc501600, /// 0x14c
			 0x20ebf1cf, /// 0x150
			 0xdbe2cf73, /// 0x154
			 0x199232de, /// 0x158
			 0x99fc5002, /// 0x15c
			 0x529e833a, /// 0x160
			 0x0a5a9f54, /// 0x164
			 0x73dee410, /// 0x168
			 0xf0dbd993, /// 0x16c
			 0x6b8d37f6, /// 0x170
			 0x75c28b57, /// 0x174
			 0xe93b0117, /// 0x178
			 0x942caf79, /// 0x17c
			 0x43619c98, /// 0x180
			 0x2ed0e690, /// 0x184
			 0x07cd5735, /// 0x188
			 0x8d0f85f0, /// 0x18c
			 0x676ad7f7, /// 0x190
			 0x8a627665, /// 0x194
			 0xd485616b, /// 0x198
			 0xbd432440, /// 0x19c
			 0xf99411ff, /// 0x1a0
			 0x19cb41d7, /// 0x1a4
			 0xeadf13f8, /// 0x1a8
			 0xe0656c37, /// 0x1ac
			 0x8525fcd6, /// 0x1b0
			 0xd5186604, /// 0x1b4
			 0x792a25df, /// 0x1b8
			 0x69167e8b, /// 0x1bc
			 0xd82028ef, /// 0x1c0
			 0x338fc422, /// 0x1c4
			 0xf5015bd5, /// 0x1c8
			 0x1613dd46, /// 0x1cc
			 0xf7e1c491, /// 0x1d0
			 0xad9ae923, /// 0x1d4
			 0x8d5af60a, /// 0x1d8
			 0xaa988615, /// 0x1dc
			 0x680f498a, /// 0x1e0
			 0x50c14858, /// 0x1e4
			 0xe30bf97d, /// 0x1e8
			 0x878613e0, /// 0x1ec
			 0xf417ce9d, /// 0x1f0
			 0x64670972, /// 0x1f4
			 0x16d59b25, /// 0x1f8
			 0x93e9eab7, /// 0x1fc
			 0xe9f5733e, /// 0x200
			 0x1227500c, /// 0x204
			 0x1da47680, /// 0x208
			 0xb1d5fa09, /// 0x20c
			 0xbb020819, /// 0x210
			 0xa7bf1fe7, /// 0x214
			 0x1959faa0, /// 0x218
			 0x560f21c8, /// 0x21c
			 0x4101e76a, /// 0x220
			 0x94565de1, /// 0x224
			 0xde3dbdd9, /// 0x228
			 0x0827e5c5, /// 0x22c
			 0x5bf1be9f, /// 0x230
			 0x256914c8, /// 0x234
			 0xbc5cba51, /// 0x238
			 0x67b077fd, /// 0x23c
			 0x056cf2a9, /// 0x240
			 0x34ce94db, /// 0x244
			 0x8edfb784, /// 0x248
			 0xd51ce72a, /// 0x24c
			 0xf0641868, /// 0x250
			 0xccd528f0, /// 0x254
			 0x5bb97584, /// 0x258
			 0xea1e8e07, /// 0x25c
			 0x79b26d19, /// 0x260
			 0x4e7a4c7e, /// 0x264
			 0xda10df5d, /// 0x268
			 0xfb0c631f, /// 0x26c
			 0xae9fc46c, /// 0x270
			 0x1902442d, /// 0x274
			 0x508f975d, /// 0x278
			 0x01b58fd0, /// 0x27c
			 0x3183abfa, /// 0x280
			 0xba097a89, /// 0x284
			 0x619650e9, /// 0x288
			 0xc31e55ce, /// 0x28c
			 0x512c01d3, /// 0x290
			 0x06bf75f4, /// 0x294
			 0x14015764, /// 0x298
			 0xf4db89a7, /// 0x29c
			 0xa0b91092, /// 0x2a0
			 0xbfc80681, /// 0x2a4
			 0x1718304f, /// 0x2a8
			 0x03364fef, /// 0x2ac
			 0xddfcecbe, /// 0x2b0
			 0xe50f4ab2, /// 0x2b4
			 0x8561324b, /// 0x2b8
			 0x5a3f962b, /// 0x2bc
			 0xc17c06fc, /// 0x2c0
			 0xa8f2a9a2, /// 0x2c4
			 0x392f6e2f, /// 0x2c8
			 0x935cee63, /// 0x2cc
			 0x2cad451a, /// 0x2d0
			 0x44f1acdd, /// 0x2d4
			 0x240ece5c, /// 0x2d8
			 0x68084a44, /// 0x2dc
			 0x4ce0dd73, /// 0x2e0
			 0x2943f154, /// 0x2e4
			 0x65ec304c, /// 0x2e8
			 0x27cc96ed, /// 0x2ec
			 0x13fcbc77, /// 0x2f0
			 0x643c1816, /// 0x2f4
			 0x8f31a569, /// 0x2f8
			 0xc16b53e7, /// 0x2fc
			 0x10ecc5e2, /// 0x300
			 0x3f950fff, /// 0x304
			 0x7d61c136, /// 0x308
			 0x1f55e648, /// 0x30c
			 0x312c5591, /// 0x310
			 0x0de2e515, /// 0x314
			 0xcbf60f3a, /// 0x318
			 0x3e1fed54, /// 0x31c
			 0xfd560c10, /// 0x320
			 0x7f2489dc, /// 0x324
			 0x361337b8, /// 0x328
			 0xa34f151a, /// 0x32c
			 0x5ad91c3c, /// 0x330
			 0x74ea4260, /// 0x334
			 0x0cdba899, /// 0x338
			 0xe1e9c640, /// 0x33c
			 0x4d2e7858, /// 0x340
			 0xa424f8a6, /// 0x344
			 0x23f2c2f2, /// 0x348
			 0x8392a010, /// 0x34c
			 0xd1b6db9b, /// 0x350
			 0xea628f1e, /// 0x354
			 0x2fc26154, /// 0x358
			 0x72211f1a, /// 0x35c
			 0x52195be9, /// 0x360
			 0xf69b7bb1, /// 0x364
			 0xf6068aee, /// 0x368
			 0x685807e3, /// 0x36c
			 0x1b69c9d5, /// 0x370
			 0xc23fe8ab, /// 0x374
			 0xfbaf3c13, /// 0x378
			 0x6be74a7c, /// 0x37c
			 0xac976572, /// 0x380
			 0x299476f7, /// 0x384
			 0xe6241ba3, /// 0x388
			 0x2fc2fb02, /// 0x38c
			 0xc0856471, /// 0x390
			 0xa1ec9829, /// 0x394
			 0x3c112eb6, /// 0x398
			 0x9a3cb0d9, /// 0x39c
			 0x2f757587, /// 0x3a0
			 0xd33c7806, /// 0x3a4
			 0x606a9a1b, /// 0x3a8
			 0x73603c0a, /// 0x3ac
			 0x652f9c28, /// 0x3b0
			 0xdccf83b2, /// 0x3b4
			 0xea4ed64e, /// 0x3b8
			 0xb419edb9, /// 0x3bc
			 0x21f91dec, /// 0x3c0
			 0x0e3fa5bd, /// 0x3c4
			 0xf5e734b9, /// 0x3c8
			 0xcd800bbf, /// 0x3cc
			 0x0dd0de0d, /// 0x3d0
			 0xa406fecf, /// 0x3d4
			 0x471c2883, /// 0x3d8
			 0x0ddc4a37, /// 0x3dc
			 0x18689a47, /// 0x3e0
			 0x4d83f346, /// 0x3e4
			 0xcd30de47, /// 0x3e8
			 0x3d006441, /// 0x3ec
			 0xbfb721a0, /// 0x3f0
			 0xaa963513, /// 0x3f4
			 0x5a3723be, /// 0x3f8
			 0x1e437337, /// 0x3fc
			 0xe36eb0c3, /// 0x400
			 0x2d468a85, /// 0x404
			 0xb06eb5f5, /// 0x408
			 0xa8c6756b, /// 0x40c
			 0x1b984c71, /// 0x410
			 0xc66eef5e, /// 0x414
			 0xfe188860, /// 0x418
			 0x631ef16f, /// 0x41c
			 0x84defc7a, /// 0x420
			 0x22d43db7, /// 0x424
			 0x1113866e, /// 0x428
			 0xe5522af5, /// 0x42c
			 0xf754c5af, /// 0x430
			 0x9dd5fba5, /// 0x434
			 0xdfce6e82, /// 0x438
			 0xf9bbb4d4, /// 0x43c
			 0xde7a4963, /// 0x440
			 0x4d982918, /// 0x444
			 0x811602d7, /// 0x448
			 0x7796cad5, /// 0x44c
			 0x6d3e1106, /// 0x450
			 0x9cad8eb9, /// 0x454
			 0x83f63996, /// 0x458
			 0xa8aafc22, /// 0x45c
			 0x2109ef19, /// 0x460
			 0x96a1f637, /// 0x464
			 0x796a5d37, /// 0x468
			 0x53f7acee, /// 0x46c
			 0xb572cb1b, /// 0x470
			 0x1110a8a1, /// 0x474
			 0x055bee3d, /// 0x478
			 0x69f296df, /// 0x47c
			 0x17711d72, /// 0x480
			 0x68d1e6aa, /// 0x484
			 0x8f93b51e, /// 0x488
			 0xfdc9ba13, /// 0x48c
			 0xe0fbac34, /// 0x490
			 0x0da95ac8, /// 0x494
			 0x9ed8e04d, /// 0x498
			 0x7dee516d, /// 0x49c
			 0x98dd0876, /// 0x4a0
			 0xc8de3785, /// 0x4a4
			 0xe525a83d, /// 0x4a8
			 0x61264530, /// 0x4ac
			 0xe359f55a, /// 0x4b0
			 0x1211e73d, /// 0x4b4
			 0x1a0123bf, /// 0x4b8
			 0xe77d2bab, /// 0x4bc
			 0x35960bc8, /// 0x4c0
			 0x857edaf3, /// 0x4c4
			 0x60938aaf, /// 0x4c8
			 0xf5ea72a7, /// 0x4cc
			 0x2e4081a1, /// 0x4d0
			 0xab369a01, /// 0x4d4
			 0xa853f9f0, /// 0x4d8
			 0x992f37bf, /// 0x4dc
			 0x73a58bcd, /// 0x4e0
			 0xa8ca8d8c, /// 0x4e4
			 0xc9662429, /// 0x4e8
			 0x6a342797, /// 0x4ec
			 0x8a8498fc, /// 0x4f0
			 0xdbeb561a, /// 0x4f4
			 0x34acb937, /// 0x4f8
			 0xaada31ba, /// 0x4fc
			 0xed3370f6, /// 0x500
			 0x33e543fc, /// 0x504
			 0xe1f4b45f, /// 0x508
			 0x03f2a400, /// 0x50c
			 0xce4d2d2a, /// 0x510
			 0x04a78626, /// 0x514
			 0xa4c0a7a3, /// 0x518
			 0x4ace1bdf, /// 0x51c
			 0x1def7274, /// 0x520
			 0xbabef2e2, /// 0x524
			 0x2d871182, /// 0x528
			 0x1323a254, /// 0x52c
			 0x56741130, /// 0x530
			 0x486e7327, /// 0x534
			 0xb7c185ed, /// 0x538
			 0x193ea4f0, /// 0x53c
			 0x657af5ff, /// 0x540
			 0x6e9707b9, /// 0x544
			 0x39fdf08b, /// 0x548
			 0xd7c503cd, /// 0x54c
			 0xad7ce776, /// 0x550
			 0xa5b4767f, /// 0x554
			 0x94e3b52f, /// 0x558
			 0x44dc5433, /// 0x55c
			 0x13b64e5d, /// 0x560
			 0x5423e549, /// 0x564
			 0x3c9407cb, /// 0x568
			 0x684489fc, /// 0x56c
			 0x33d1faa8, /// 0x570
			 0x035d82a3, /// 0x574
			 0x2572e72f, /// 0x578
			 0x2761a1b8, /// 0x57c
			 0xd1823400, /// 0x580
			 0x5e46b04c, /// 0x584
			 0xedda2a5c, /// 0x588
			 0xa26f59f9, /// 0x58c
			 0x60f9d32e, /// 0x590
			 0x8eefb88f, /// 0x594
			 0x6cab91ae, /// 0x598
			 0x88586aa3, /// 0x59c
			 0x5e2342be, /// 0x5a0
			 0xc70a41d3, /// 0x5a4
			 0x38d64047, /// 0x5a8
			 0x6c929e22, /// 0x5ac
			 0x2ce19966, /// 0x5b0
			 0x14ad1cd2, /// 0x5b4
			 0x4f59ea09, /// 0x5b8
			 0x5366f930, /// 0x5bc
			 0x8cd4ee62, /// 0x5c0
			 0x71c485e8, /// 0x5c4
			 0x60d38b3f, /// 0x5c8
			 0x657ea10d, /// 0x5cc
			 0x9ce27a2b, /// 0x5d0
			 0x12af7ece, /// 0x5d4
			 0xc16f475c, /// 0x5d8
			 0x5ffef94c, /// 0x5dc
			 0xf492fc33, /// 0x5e0
			 0xed00825b, /// 0x5e4
			 0x139d0a57, /// 0x5e8
			 0x4c95df47, /// 0x5ec
			 0x4d0d19d4, /// 0x5f0
			 0x3191d3d8, /// 0x5f4
			 0xfedac670, /// 0x5f8
			 0x725af221, /// 0x5fc
			 0xd3b30cd0, /// 0x600
			 0x35f198bd, /// 0x604
			 0xcbf7173a, /// 0x608
			 0x4f72c84d, /// 0x60c
			 0x7b3878fd, /// 0x610
			 0xe74ed65c, /// 0x614
			 0xfa200a93, /// 0x618
			 0xdcbbbfa2, /// 0x61c
			 0x2fa2c77c, /// 0x620
			 0x688d7542, /// 0x624
			 0xcb176d4b, /// 0x628
			 0xf9edc570, /// 0x62c
			 0x05b71cfa, /// 0x630
			 0xa1754967, /// 0x634
			 0xf0b26184, /// 0x638
			 0x3f81f224, /// 0x63c
			 0xb463210b, /// 0x640
			 0x7e2f32a7, /// 0x644
			 0x86fed32f, /// 0x648
			 0x9130ce64, /// 0x64c
			 0xcfaadfef, /// 0x650
			 0x724a0af4, /// 0x654
			 0xa351af12, /// 0x658
			 0xe890974a, /// 0x65c
			 0x6eb4cb30, /// 0x660
			 0x8fbc87a7, /// 0x664
			 0x40b0f2f6, /// 0x668
			 0x543d1dbb, /// 0x66c
			 0xd5f09cbc, /// 0x670
			 0xd2dc2959, /// 0x674
			 0x7bbebcb4, /// 0x678
			 0x1c65e34d, /// 0x67c
			 0x22eb509c, /// 0x680
			 0x72e70fcd, /// 0x684
			 0x65951678, /// 0x688
			 0xe3afc73e, /// 0x68c
			 0x6efc2c9a, /// 0x690
			 0x339cfb10, /// 0x694
			 0xe0f5eb56, /// 0x698
			 0xade80fc6, /// 0x69c
			 0x49ddc65e, /// 0x6a0
			 0xb9f4360b, /// 0x6a4
			 0x3f9435cc, /// 0x6a8
			 0x6593a4b8, /// 0x6ac
			 0xcdcab697, /// 0x6b0
			 0x54368659, /// 0x6b4
			 0x6cfe2ec9, /// 0x6b8
			 0x4af1d8f8, /// 0x6bc
			 0x9b3f8867, /// 0x6c0
			 0x4b20f942, /// 0x6c4
			 0x421fab31, /// 0x6c8
			 0xc84870e4, /// 0x6cc
			 0xd277d4d1, /// 0x6d0
			 0xdff5af79, /// 0x6d4
			 0xca375cdc, /// 0x6d8
			 0xdd88f696, /// 0x6dc
			 0xf33e66fd, /// 0x6e0
			 0x885201c9, /// 0x6e4
			 0xd4f5321c, /// 0x6e8
			 0x968e0119, /// 0x6ec
			 0x5c129ef4, /// 0x6f0
			 0xf0a6bac2, /// 0x6f4
			 0x943812fa, /// 0x6f8
			 0x050aee01, /// 0x6fc
			 0xd0bc3137, /// 0x700
			 0x227fadff, /// 0x704
			 0xd7e19ab3, /// 0x708
			 0x0453e00e, /// 0x70c
			 0xc257f386, /// 0x710
			 0x8addbbd5, /// 0x714
			 0x5ada1a76, /// 0x718
			 0x33de2477, /// 0x71c
			 0x2d335420, /// 0x720
			 0x25c198d5, /// 0x724
			 0x84b721a3, /// 0x728
			 0x6c0ce07d, /// 0x72c
			 0x7bcde47c, /// 0x730
			 0x98e09380, /// 0x734
			 0x0adebea8, /// 0x738
			 0xdfcb4fb8, /// 0x73c
			 0x4eac964a, /// 0x740
			 0x3a038c46, /// 0x744
			 0xa9da14d2, /// 0x748
			 0x4b2ecabe, /// 0x74c
			 0x9736fe14, /// 0x750
			 0x47dc02ce, /// 0x754
			 0x32945e5f, /// 0x758
			 0xb5876037, /// 0x75c
			 0x73ed9f1b, /// 0x760
			 0xa8a75078, /// 0x764
			 0x2aa152f2, /// 0x768
			 0xf1e62ec9, /// 0x76c
			 0x982ef7d5, /// 0x770
			 0x5e192437, /// 0x774
			 0x67c61091, /// 0x778
			 0x6e328a94, /// 0x77c
			 0x847312f6, /// 0x780
			 0xfa3fa66d, /// 0x784
			 0x84874c6d, /// 0x788
			 0x445b316d, /// 0x78c
			 0x38a1749c, /// 0x790
			 0x43d075bb, /// 0x794
			 0x6d8ee959, /// 0x798
			 0x4d889777, /// 0x79c
			 0x3f76ab12, /// 0x7a0
			 0x72f7a85b, /// 0x7a4
			 0xac28bc06, /// 0x7a8
			 0x47542c6a, /// 0x7ac
			 0x80405a79, /// 0x7b0
			 0xe4f69221, /// 0x7b4
			 0x45af224f, /// 0x7b8
			 0x9ba1dd47, /// 0x7bc
			 0x97f913bb, /// 0x7c0
			 0x7cd6c488, /// 0x7c4
			 0x66cd9877, /// 0x7c8
			 0xb2db4f58, /// 0x7cc
			 0x8e0ddb5a, /// 0x7d0
			 0x236fe743, /// 0x7d4
			 0x93a54c92, /// 0x7d8
			 0x7446ecb7, /// 0x7dc
			 0x1cfe70ac, /// 0x7e0
			 0x2f4a06ba, /// 0x7e4
			 0x2930bf11, /// 0x7e8
			 0x1cbe06f2, /// 0x7ec
			 0xe407f12b, /// 0x7f0
			 0xd0aff785, /// 0x7f4
			 0x8b5e888d, /// 0x7f8
			 0x12628abb, /// 0x7fc
			 0x5dcdab6b, /// 0x800
			 0xeba557a2, /// 0x804
			 0x8dbd53d9, /// 0x808
			 0xfcdd867c, /// 0x80c
			 0x9ad566c9, /// 0x810
			 0x9322b93d, /// 0x814
			 0x74a4d871, /// 0x818
			 0xd40ff84f, /// 0x81c
			 0x497dab9f, /// 0x820
			 0xb3d6256f, /// 0x824
			 0xa1b3380c, /// 0x828
			 0x1cd4c2f7, /// 0x82c
			 0x86749576, /// 0x830
			 0x673dcd74, /// 0x834
			 0xefa770b9, /// 0x838
			 0x33b382e1, /// 0x83c
			 0x77e7918b, /// 0x840
			 0x321aff98, /// 0x844
			 0x7479eaf7, /// 0x848
			 0x6e777e43, /// 0x84c
			 0x79dddbaf, /// 0x850
			 0xa9d6aefb, /// 0x854
			 0x1fa262fa, /// 0x858
			 0xc4e9a5cc, /// 0x85c
			 0x840b038f, /// 0x860
			 0x0ec049b2, /// 0x864
			 0x127e871a, /// 0x868
			 0x2f41dd7a, /// 0x86c
			 0x5cf02997, /// 0x870
			 0xc9941c31, /// 0x874
			 0xea0c90f4, /// 0x878
			 0xe6e10125, /// 0x87c
			 0x5651447d, /// 0x880
			 0x26634812, /// 0x884
			 0x0f6e2ed7, /// 0x888
			 0x9cc897e0, /// 0x88c
			 0x70abd0e2, /// 0x890
			 0xf8b37162, /// 0x894
			 0x615b4977, /// 0x898
			 0xd6371b6d, /// 0x89c
			 0xd60c70ba, /// 0x8a0
			 0x43e18a18, /// 0x8a4
			 0xf3b3ee4f, /// 0x8a8
			 0x4d808060, /// 0x8ac
			 0x0a8b3951, /// 0x8b0
			 0xe12b9126, /// 0x8b4
			 0xbd68d7f1, /// 0x8b8
			 0x9ce5b50a, /// 0x8bc
			 0xcda56c18, /// 0x8c0
			 0x2acd97b6, /// 0x8c4
			 0xfbe3122e, /// 0x8c8
			 0x1502b43d, /// 0x8cc
			 0x8dfb4981, /// 0x8d0
			 0x9d1b9790, /// 0x8d4
			 0x1d317411, /// 0x8d8
			 0xe00b2c88, /// 0x8dc
			 0xca585977, /// 0x8e0
			 0xef8bd6ae, /// 0x8e4
			 0xf290afd7, /// 0x8e8
			 0xdb11454a, /// 0x8ec
			 0x62cc4668, /// 0x8f0
			 0xa3404ce0, /// 0x8f4
			 0xef4c40fc, /// 0x8f8
			 0x12e01719, /// 0x8fc
			 0xb288adfa, /// 0x900
			 0x8fce5a91, /// 0x904
			 0x6e60fe2e, /// 0x908
			 0xfaa8d120, /// 0x90c
			 0x1a147c08, /// 0x910
			 0xe0425a0c, /// 0x914
			 0x8664e0ba, /// 0x918
			 0xa0a4ca7a, /// 0x91c
			 0xda01d812, /// 0x920
			 0x383a0c1f, /// 0x924
			 0x2aa297b9, /// 0x928
			 0x1f7d036f, /// 0x92c
			 0xb4953c0f, /// 0x930
			 0x522f89b1, /// 0x934
			 0x8d5412f1, /// 0x938
			 0x667de48c, /// 0x93c
			 0x30270798, /// 0x940
			 0xf9fd444a, /// 0x944
			 0x239ad398, /// 0x948
			 0xbfc09adc, /// 0x94c
			 0xb83fd81e, /// 0x950
			 0xd01aaf14, /// 0x954
			 0x034be4df, /// 0x958
			 0xda3b1b40, /// 0x95c
			 0xaaaf4d8d, /// 0x960
			 0x32ca35a7, /// 0x964
			 0x5a56e0ac, /// 0x968
			 0x14db246f, /// 0x96c
			 0x44abebbc, /// 0x970
			 0x433e5069, /// 0x974
			 0xe447554f, /// 0x978
			 0xfda0d804, /// 0x97c
			 0x2f2f9738, /// 0x980
			 0x936d5ced, /// 0x984
			 0x4dc4b6f2, /// 0x988
			 0xd3b77a02, /// 0x98c
			 0x06b4346a, /// 0x990
			 0x3aea4ad7, /// 0x994
			 0x39e7a3c2, /// 0x998
			 0xeb9a9e7e, /// 0x99c
			 0x25469775, /// 0x9a0
			 0xb48828aa, /// 0x9a4
			 0xf2798d78, /// 0x9a8
			 0xabe759bc, /// 0x9ac
			 0x994b3cbe, /// 0x9b0
			 0x991b2fae, /// 0x9b4
			 0xe287cdf8, /// 0x9b8
			 0x52fb401b, /// 0x9bc
			 0xe2154022, /// 0x9c0
			 0x829ffaf3, /// 0x9c4
			 0x18660989, /// 0x9c8
			 0x2b6ddaad, /// 0x9cc
			 0x2564d18d, /// 0x9d0
			 0x432815c5, /// 0x9d4
			 0xdc379457, /// 0x9d8
			 0xa4b5007f, /// 0x9dc
			 0xeca53042, /// 0x9e0
			 0x3be7b251, /// 0x9e4
			 0x20501481, /// 0x9e8
			 0x0e2690d4, /// 0x9ec
			 0xcd82d254, /// 0x9f0
			 0x9a92ece9, /// 0x9f4
			 0x83d149be, /// 0x9f8
			 0xde83ee09, /// 0x9fc
			 0x4446ea74, /// 0xa00
			 0x28de4b36, /// 0xa04
			 0x9e0e296d, /// 0xa08
			 0x1cd977c6, /// 0xa0c
			 0x106c96d5, /// 0xa10
			 0xc3d3190b, /// 0xa14
			 0x7454251c, /// 0xa18
			 0xfbc6f4b5, /// 0xa1c
			 0x7e8d30ed, /// 0xa20
			 0x7ec96870, /// 0xa24
			 0xadf98022, /// 0xa28
			 0x1f5c9dd4, /// 0xa2c
			 0x5583874a, /// 0xa30
			 0x071b5477, /// 0xa34
			 0xfb9a101f, /// 0xa38
			 0x42129178, /// 0xa3c
			 0xf4c805c3, /// 0xa40
			 0xd37daada, /// 0xa44
			 0x06d026d5, /// 0xa48
			 0x75f1e191, /// 0xa4c
			 0x89ea69ee, /// 0xa50
			 0xe058a99b, /// 0xa54
			 0x31964ecc, /// 0xa58
			 0x8d6b8358, /// 0xa5c
			 0xfff5651f, /// 0xa60
			 0xa7bbf2a3, /// 0xa64
			 0xd19b7f9f, /// 0xa68
			 0x1fc9d3aa, /// 0xa6c
			 0x71f8b39a, /// 0xa70
			 0xb70424d1, /// 0xa74
			 0x31926ffe, /// 0xa78
			 0x7fe22e4c, /// 0xa7c
			 0xbb5e8636, /// 0xa80
			 0x767e6bbd, /// 0xa84
			 0xe3c3ce97, /// 0xa88
			 0xd3de0f0e, /// 0xa8c
			 0x0b9c0f98, /// 0xa90
			 0x1be043a0, /// 0xa94
			 0xf7c996ed, /// 0xa98
			 0xc6017a4e, /// 0xa9c
			 0xe4848b1a, /// 0xaa0
			 0x21a3bca9, /// 0xaa4
			 0x40694c83, /// 0xaa8
			 0xcc141f80, /// 0xaac
			 0x54dfe9f4, /// 0xab0
			 0xc83e4b02, /// 0xab4
			 0xd7fef5a7, /// 0xab8
			 0xfbac01a0, /// 0xabc
			 0x7223ea4b, /// 0xac0
			 0xd566cd7f, /// 0xac4
			 0x84e52852, /// 0xac8
			 0x71177e64, /// 0xacc
			 0x9df95c36, /// 0xad0
			 0xd0d4a90f, /// 0xad4
			 0xe960c7bd, /// 0xad8
			 0xf5a2ffe7, /// 0xadc
			 0xbc192572, /// 0xae0
			 0x751935e2, /// 0xae4
			 0x27a114a7, /// 0xae8
			 0x88ada457, /// 0xaec
			 0xc7f67033, /// 0xaf0
			 0x5dd254a7, /// 0xaf4
			 0x11e62ca0, /// 0xaf8
			 0x4fcfd192, /// 0xafc
			 0xd9fa4088, /// 0xb00
			 0x49ba516b, /// 0xb04
			 0xa00a017f, /// 0xb08
			 0xd12f80b0, /// 0xb0c
			 0x5787fd68, /// 0xb10
			 0x82f7ade2, /// 0xb14
			 0xf09c4a3d, /// 0xb18
			 0x6c144ec4, /// 0xb1c
			 0x93313704, /// 0xb20
			 0xb4318ebf, /// 0xb24
			 0xb7f8d0ae, /// 0xb28
			 0xaa7f907e, /// 0xb2c
			 0x8063731d, /// 0xb30
			 0x13cddaed, /// 0xb34
			 0x76ad4cd4, /// 0xb38
			 0x3c4ebdfd, /// 0xb3c
			 0x2ae32aeb, /// 0xb40
			 0x5ada9be3, /// 0xb44
			 0x6e76f3b4, /// 0xb48
			 0xfe66cac5, /// 0xb4c
			 0xaa3a2cf6, /// 0xb50
			 0x8aa7f2cb, /// 0xb54
			 0x7d25a461, /// 0xb58
			 0xf7868749, /// 0xb5c
			 0xe5baefd9, /// 0xb60
			 0x66ee7d8c, /// 0xb64
			 0xf6d7fb5d, /// 0xb68
			 0x3a44a3d8, /// 0xb6c
			 0xe8bcfa07, /// 0xb70
			 0x1238fbca, /// 0xb74
			 0xa642d124, /// 0xb78
			 0xb138e639, /// 0xb7c
			 0x5c9f74b7, /// 0xb80
			 0xc6d0454c, /// 0xb84
			 0x5e30818b, /// 0xb88
			 0xdc507a72, /// 0xb8c
			 0xeb556e63, /// 0xb90
			 0xee879244, /// 0xb94
			 0x1fbd28cf, /// 0xb98
			 0x32de99ed, /// 0xb9c
			 0xff2571cc, /// 0xba0
			 0xbe207567, /// 0xba4
			 0xec83238c, /// 0xba8
			 0xf7377ddc, /// 0xbac
			 0x41c68722, /// 0xbb0
			 0x35f41bb2, /// 0xbb4
			 0xe9eec848, /// 0xbb8
			 0x4061c5f3, /// 0xbbc
			 0xd164f622, /// 0xbc0
			 0x639ae095, /// 0xbc4
			 0xd55efd46, /// 0xbc8
			 0xcfd6df5e, /// 0xbcc
			 0x0b981552, /// 0xbd0
			 0x7729fe1a, /// 0xbd4
			 0xbd583be8, /// 0xbd8
			 0x92751485, /// 0xbdc
			 0x504902dc, /// 0xbe0
			 0x4a8eaa1b, /// 0xbe4
			 0x3a5509a0, /// 0xbe8
			 0x9179df9a, /// 0xbec
			 0x36fa0566, /// 0xbf0
			 0x94377de7, /// 0xbf4
			 0x80b8f89c, /// 0xbf8
			 0x531bda29, /// 0xbfc
			 0x49188f3f, /// 0xc00
			 0xda9bff13, /// 0xc04
			 0x4144096d, /// 0xc08
			 0xe5bae89d, /// 0xc0c
			 0xc43eea58, /// 0xc10
			 0x2890329a, /// 0xc14
			 0x54b07f3c, /// 0xc18
			 0x49366531, /// 0xc1c
			 0xed68920b, /// 0xc20
			 0x0e2ac2fb, /// 0xc24
			 0xbc9da958, /// 0xc28
			 0xba70c97f, /// 0xc2c
			 0x552782cb, /// 0xc30
			 0x6897880f, /// 0xc34
			 0x541e300e, /// 0xc38
			 0xc9f04bcb, /// 0xc3c
			 0xa65e5d78, /// 0xc40
			 0x27784988, /// 0xc44
			 0x11206b4d, /// 0xc48
			 0x3946a234, /// 0xc4c
			 0xd4281fc9, /// 0xc50
			 0xcecf03db, /// 0xc54
			 0x374e8731, /// 0xc58
			 0x8cf2fa3e, /// 0xc5c
			 0x853b8512, /// 0xc60
			 0x6777e26a, /// 0xc64
			 0x72b18344, /// 0xc68
			 0xbc0b8d42, /// 0xc6c
			 0xe22c44ad, /// 0xc70
			 0x8cbe23d3, /// 0xc74
			 0xd05850f0, /// 0xc78
			 0x6bda4c47, /// 0xc7c
			 0xcca35846, /// 0xc80
			 0x6a8b63dd, /// 0xc84
			 0xa4a59583, /// 0xc88
			 0x46a3b3cb, /// 0xc8c
			 0x16570a16, /// 0xc90
			 0x972e4f37, /// 0xc94
			 0x796b6a7c, /// 0xc98
			 0xf24b6c4e, /// 0xc9c
			 0x2108da32, /// 0xca0
			 0x5bb795c3, /// 0xca4
			 0x5fe6fd43, /// 0xca8
			 0x6bcbe431, /// 0xcac
			 0x9e570ba0, /// 0xcb0
			 0x0c334a85, /// 0xcb4
			 0x28575f1b, /// 0xcb8
			 0xaf5d0758, /// 0xcbc
			 0xf5164bd3, /// 0xcc0
			 0x25269932, /// 0xcc4
			 0x82a90893, /// 0xcc8
			 0xdd37e5dd, /// 0xccc
			 0x05b4a6c6, /// 0xcd0
			 0x927a8c80, /// 0xcd4
			 0x7959fa61, /// 0xcd8
			 0x52346b73, /// 0xcdc
			 0x4a8d28a3, /// 0xce0
			 0x64c3abf2, /// 0xce4
			 0xee8164e0, /// 0xce8
			 0x3c5c0c2c, /// 0xcec
			 0xef052b15, /// 0xcf0
			 0x9b71f743, /// 0xcf4
			 0xb4ead029, /// 0xcf8
			 0x0c4f6258, /// 0xcfc
			 0xb1e6188e, /// 0xd00
			 0x2b6fd09c, /// 0xd04
			 0xe7e0dcb5, /// 0xd08
			 0x0b022293, /// 0xd0c
			 0xc80412b7, /// 0xd10
			 0xf6315a8f, /// 0xd14
			 0x4b0208ac, /// 0xd18
			 0x0b896ee0, /// 0xd1c
			 0xfe2df872, /// 0xd20
			 0x3e7e2377, /// 0xd24
			 0x4302b156, /// 0xd28
			 0x804f21fe, /// 0xd2c
			 0x54c31ee1, /// 0xd30
			 0xe369e84e, /// 0xd34
			 0x90f33097, /// 0xd38
			 0xd478a780, /// 0xd3c
			 0xf886fce9, /// 0xd40
			 0xdef6c5a5, /// 0xd44
			 0x4cf2ffa9, /// 0xd48
			 0xac1726ea, /// 0xd4c
			 0x555d2a09, /// 0xd50
			 0xaf30dd6b, /// 0xd54
			 0x215de669, /// 0xd58
			 0xebd8b9a7, /// 0xd5c
			 0x008e60d1, /// 0xd60
			 0x881260d5, /// 0xd64
			 0xa0cfeaf3, /// 0xd68
			 0x629367d1, /// 0xd6c
			 0x949d3b2b, /// 0xd70
			 0x628d88da, /// 0xd74
			 0x6b1c87ea, /// 0xd78
			 0x0bf6da17, /// 0xd7c
			 0xa993685f, /// 0xd80
			 0x0365ec60, /// 0xd84
			 0x4a47b70d, /// 0xd88
			 0x5f9711ad, /// 0xd8c
			 0xa57f9d2d, /// 0xd90
			 0x192b6538, /// 0xd94
			 0x884a0e2f, /// 0xd98
			 0x6c18a433, /// 0xd9c
			 0xd47a6f58, /// 0xda0
			 0xa0b4a2b6, /// 0xda4
			 0x18e0f4e8, /// 0xda8
			 0x216b3924, /// 0xdac
			 0x05257d3d, /// 0xdb0
			 0xa3ac5349, /// 0xdb4
			 0x873bf210, /// 0xdb8
			 0x8742608f, /// 0xdbc
			 0x54dd2fc7, /// 0xdc0
			 0xfa14492e, /// 0xdc4
			 0xfed9842a, /// 0xdc8
			 0x57057f85, /// 0xdcc
			 0xa20c4171, /// 0xdd0
			 0x2dc3d92f, /// 0xdd4
			 0xeaf7b328, /// 0xdd8
			 0xb635da97, /// 0xddc
			 0xde8fa43a, /// 0xde0
			 0x444271f4, /// 0xde4
			 0x137ac7da, /// 0xde8
			 0xcdfb8f06, /// 0xdec
			 0x8eedec68, /// 0xdf0
			 0x47e93679, /// 0xdf4
			 0xf405e2ad, /// 0xdf8
			 0xd762c52e, /// 0xdfc
			 0xdf380a74, /// 0xe00
			 0xd93bde7f, /// 0xe04
			 0x998c6152, /// 0xe08
			 0x0d7323f6, /// 0xe0c
			 0x16b3bbab, /// 0xe10
			 0x52dfafdf, /// 0xe14
			 0x3086bc99, /// 0xe18
			 0x5a3968e7, /// 0xe1c
			 0x4dbac3e4, /// 0xe20
			 0x56b9a185, /// 0xe24
			 0xf81076da, /// 0xe28
			 0xd5beadd1, /// 0xe2c
			 0x3753d771, /// 0xe30
			 0x7e18a50e, /// 0xe34
			 0xc2791962, /// 0xe38
			 0x63d8a202, /// 0xe3c
			 0x661e10fa, /// 0xe40
			 0x41a05c89, /// 0xe44
			 0x45957632, /// 0xe48
			 0x9e0a352d, /// 0xe4c
			 0x45fb41a9, /// 0xe50
			 0xced7e051, /// 0xe54
			 0x9cf77624, /// 0xe58
			 0xac8a1896, /// 0xe5c
			 0xf2bcd1a1, /// 0xe60
			 0xdd53a27e, /// 0xe64
			 0x8067a609, /// 0xe68
			 0xab80ef7b, /// 0xe6c
			 0x2c5f3cc6, /// 0xe70
			 0xf3d3a7d4, /// 0xe74
			 0x00faae91, /// 0xe78
			 0x76e2694b, /// 0xe7c
			 0xcb94fa18, /// 0xe80
			 0x71a709e6, /// 0xe84
			 0x5a0abac5, /// 0xe88
			 0xbddfc5be, /// 0xe8c
			 0xf551e712, /// 0xe90
			 0x8e3368d4, /// 0xe94
			 0x356b85fa, /// 0xe98
			 0xebec9fe5, /// 0xe9c
			 0xd8ac4921, /// 0xea0
			 0xe07ebb5d, /// 0xea4
			 0x0cbac563, /// 0xea8
			 0x9a9a170d, /// 0xeac
			 0xde4c421b, /// 0xeb0
			 0x95fc36c4, /// 0xeb4
			 0xba5dfe75, /// 0xeb8
			 0x380a064a, /// 0xebc
			 0xf4e1dd99, /// 0xec0
			 0xc591e0b4, /// 0xec4
			 0xb7ef2732, /// 0xec8
			 0x238beed1, /// 0xecc
			 0x430e769a, /// 0xed0
			 0x6cebe601, /// 0xed4
			 0xbc5980ae, /// 0xed8
			 0xadbf3c10, /// 0xedc
			 0x3c03ea3c, /// 0xee0
			 0x26812688, /// 0xee4
			 0x26c6b457, /// 0xee8
			 0x51537411, /// 0xeec
			 0x02aa7b68, /// 0xef0
			 0x405090d4, /// 0xef4
			 0x384cc152, /// 0xef8
			 0x244e18a6, /// 0xefc
			 0x47858c59, /// 0xf00
			 0x14082f18, /// 0xf04
			 0xa3c6f502, /// 0xf08
			 0x3e5cf343, /// 0xf0c
			 0x0a18564b, /// 0xf10
			 0xb23f6297, /// 0xf14
			 0x4f157e60, /// 0xf18
			 0xabd464d8, /// 0xf1c
			 0x0f351801, /// 0xf20
			 0x76e5bc27, /// 0xf24
			 0x7849f88d, /// 0xf28
			 0x43e3d6d9, /// 0xf2c
			 0xe3005680, /// 0xf30
			 0x85281803, /// 0xf34
			 0xdf566f92, /// 0xf38
			 0xddb4b17e, /// 0xf3c
			 0xa30e3e0e, /// 0xf40
			 0x562f4abc, /// 0xf44
			 0x485b4b56, /// 0xf48
			 0x5711a3c4, /// 0xf4c
			 0x8181f8d1, /// 0xf50
			 0xe4bd70f4, /// 0xf54
			 0x9079f00c, /// 0xf58
			 0x905ada26, /// 0xf5c
			 0x23e861fe, /// 0xf60
			 0x72380069, /// 0xf64
			 0xf0f2dfd9, /// 0xf68
			 0x807a2888, /// 0xf6c
			 0x26e4cabb, /// 0xf70
			 0x7e63cfc2, /// 0xf74
			 0x66562de0, /// 0xf78
			 0x3d85523d, /// 0xf7c
			 0x9eba7956, /// 0xf80
			 0xb50c0130, /// 0xf84
			 0x873357e2, /// 0xf88
			 0x6c926c8f, /// 0xf8c
			 0x52d6fbe8, /// 0xf90
			 0x76f9befd, /// 0xf94
			 0x4e7b928c, /// 0xf98
			 0x04c9bb50, /// 0xf9c
			 0x10ce2092, /// 0xfa0
			 0x58a8e48b, /// 0xfa4
			 0x2109cc3a, /// 0xfa8
			 0xf5c9308c, /// 0xfac
			 0xccc71203, /// 0xfb0
			 0xc3199de7, /// 0xfb4
			 0xcae64ec7, /// 0xfb8
			 0xacddb4a1, /// 0xfbc
			 0x0de1ccf4, /// 0xfc0
			 0xa911abc9, /// 0xfc4
			 0x69f14778, /// 0xfc8
			 0x889c5526, /// 0xfcc
			 0xf3235dbf, /// 0xfd0
			 0xd0700105, /// 0xfd4
			 0xf980e967, /// 0xfd8
			 0xc7d46147, /// 0xfdc
			 0x7083ffed, /// 0xfe0
			 0x87e78118, /// 0xfe4
			 0x8ec11308, /// 0xfe8
			 0x47c2c205, /// 0xfec
			 0x430b8e18, /// 0xff0
			 0xc178d64c, /// 0xff4
			 0xfa839d78, /// 0xff8
			 0xb1017578 /// last
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
			 0x00000198,
			 0x0000009c,
			 0x000001b4,
			 0x00000344,
			 0x00000038,
			 0x0000049c,
			 0x00000680,
			 0x00000098,

			 /// vpu register f2
			 0x41300000,
			 0x41200000,
			 0x40c00000,
			 0x41c80000,
			 0x41e00000,
			 0x41d80000,
			 0x40a00000,
			 0x41d80000,

			 /// vpu register f3
			 0x41d00000,
			 0x41e80000,
			 0x41a00000,
			 0x40c00000,
			 0x41900000,
			 0x41400000,
			 0x41700000,
			 0x40400000,

			 /// vpu register f4
			 0x41a00000,
			 0x41a00000,
			 0x41a80000,
			 0x41d80000,
			 0x41880000,
			 0x41e80000,
			 0x40800000,
			 0x40800000,

			 /// vpu register f5
			 0x41500000,
			 0x41c80000,
			 0x41f80000,
			 0x41500000,
			 0x40800000,
			 0x41700000,
			 0x40e00000,
			 0x40c00000,

			 /// vpu register f6
			 0x41d80000,
			 0x41300000,
			 0x42000000,
			 0x41100000,
			 0x41e80000,
			 0x41500000,
			 0x41500000,
			 0x41500000,

			 /// vpu register f7
			 0x40400000,
			 0x41600000,
			 0x41c00000,
			 0x41f80000,
			 0x41c00000,
			 0x41a00000,
			 0x41700000,
			 0x3f800000,

			 /// vpu register f8
			 0x41f00000,
			 0x41600000,
			 0x41c00000,
			 0x41e00000,
			 0x41b00000,
			 0x41c80000,
			 0x40800000,
			 0x41a00000,

			 /// vpu register f9
			 0x41a80000,
			 0x41a80000,
			 0x40a00000,
			 0x41400000,
			 0x41e00000,
			 0x3f800000,
			 0x40a00000,
			 0x41a80000,

			 /// vpu register f10
			 0x41d00000,
			 0x41c80000,
			 0x40e00000,
			 0x41c00000,
			 0x3f800000,
			 0x41980000,
			 0x41980000,
			 0x41100000,

			 /// vpu register f11
			 0x41f80000,
			 0x41c00000,
			 0x41a00000,
			 0x41100000,
			 0x41d00000,
			 0x41200000,
			 0x40400000,
			 0x41900000,

			 /// vpu register f12
			 0x40800000,
			 0x41880000,
			 0x3f800000,
			 0x41c00000,
			 0x40e00000,
			 0x40800000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f13
			 0x41a80000,
			 0x40a00000,
			 0x41f00000,
			 0x41b00000,
			 0x40e00000,
			 0x41b00000,
			 0x3f800000,
			 0x3f800000,

			 /// vpu register f14
			 0x40400000,
			 0x41b00000,
			 0x41d00000,
			 0x40c00000,
			 0x41400000,
			 0x41400000,
			 0x41b00000,
			 0x41800000,

			 /// vpu register f15
			 0x41300000,
			 0x40c00000,
			 0x41b80000,
			 0x40a00000,
			 0x41c00000,
			 0x41b80000,
			 0x40e00000,
			 0x41b80000,

			 /// vpu register f16
			 0x41a80000,
			 0x41880000,
			 0x42000000,
			 0x41d80000,
			 0x41200000,
			 0x41c80000,
			 0x41700000,
			 0x3f800000,

			 /// vpu register f17
			 0x40400000,
			 0x41700000,
			 0x41500000,
			 0x42000000,
			 0x41d80000,
			 0x41600000,
			 0x41000000,
			 0x41800000,

			 /// vpu register f18
			 0x41a80000,
			 0x41a00000,
			 0x41880000,
			 0x41f00000,
			 0x41c80000,
			 0x40800000,
			 0x41e00000,
			 0x41b00000,

			 /// vpu register f19
			 0x41300000,
			 0x41200000,
			 0x41f80000,
			 0x41b00000,
			 0x41300000,
			 0x42000000,
			 0x41980000,
			 0x41c80000,

			 /// vpu register f20
			 0x41000000,
			 0x41100000,
			 0x41600000,
			 0x41980000,
			 0x41200000,
			 0x41100000,
			 0x41980000,
			 0x41e00000,

			 /// vpu register f21
			 0x41900000,
			 0x41d80000,
			 0x41500000,
			 0x41d80000,
			 0x41900000,
			 0x40800000,
			 0x41600000,
			 0x41900000,

			 /// vpu register f22
			 0x41e80000,
			 0x41a80000,
			 0x40e00000,
			 0x41a00000,
			 0x41f80000,
			 0x41980000,
			 0x3f800000,
			 0x40800000,

			 /// vpu register f23
			 0x41b80000,
			 0x41f00000,
			 0x41200000,
			 0x41900000,
			 0x41b80000,
			 0x40800000,
			 0x42000000,
			 0x41700000,

			 /// vpu register f24
			 0x41e80000,
			 0x41300000,
			 0x41600000,
			 0x41600000,
			 0x40800000,
			 0x41800000,
			 0x41a80000,
			 0x41a00000,

			 /// vpu register f25
			 0x41100000,
			 0x41c80000,
			 0x41a00000,
			 0x41d00000,
			 0x41c00000,
			 0x41a00000,
			 0x42000000,
			 0x40a00000,

			 /// vpu register f26
			 0x3f800000,
			 0x41800000,
			 0x41d00000,
			 0x41b80000,
			 0x41e00000,
			 0x41880000,
			 0x41300000,
			 0x41e80000,

			 /// vpu register f27
			 0x42000000,
			 0x41d00000,
			 0x42000000,
			 0x41200000,
			 0x41b80000,
			 0x41f00000,
			 0x41d00000,
			 0x41d80000,

			 /// vpu register f28
			 0x41d80000,
			 0x42000000,
			 0x41f00000,
			 0x40c00000,
			 0x40400000,
			 0x40c00000,
			 0x41b80000,
			 0x41d80000,

			 /// vpu register f29
			 0x41d80000,
			 0x41d80000,
			 0x41b00000,
			 0x41a00000,
			 0x41900000,
			 0x41200000,
			 0x41b00000,
			 0x41700000,

			 /// vpu register f30
			 0x41f80000,
			 0x41b00000,
			 0x3f800000,
			 0x41300000,
			 0x3f800000,
			 0x41f80000,
			 0x41500000,
			 0x41d00000,

			 /// vpu register f31
			 0x40000000,
			 0x40000000,
			 0x41e00000,
			 0x41400000,
			 0x41000000,
			 0x41000000,
			 0x40a00000,
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
		"fmadd.ps f7, f5, f12, f20, rtz\n"                    ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f16, f29, f4, f20, rdn\n"                   ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f13, f17, f14, f28, rmm\n"                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f9, f23, f16, f16, rtz\n"                   ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f10, f23, f17, f12, rmm\n"                  ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f2, f14, f4, f4, rmm\n"                     ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f3, f8, f18, f6, rtz\n"                     ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f17, f5, f15, f22, dyn\n"                   ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f18, f21, f14, f23, rtz\n"                  ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f16, f29, f10, f1, rmm\n"                   ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f6, f20, f0, f24, rtz\n"                    ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f4, f3, f27, f15, rup\n"                    ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f7, f27, f4, f19, rdn\n"                    ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f27, f18, f12, f11, rmm\n"                  ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f25, f26, f15, f22, rup\n"                  ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f24, f16, f15, f1, rne\n"                   ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f26, f10, f23, f2, rtz\n"                   ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f2, f18, f5, f9, rup\n"                     ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f27, f1, f5, f16, rtz\n"                    ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f7, f18, f6, f10, dyn\n"                    ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f26, f15, f5, f15, rne\n"                   ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f30, f30, f11, f21, rmm\n"                  ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f13, f3, f3, f20, dyn\n"                    ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f17, f4, f29, f20, rne\n"                   ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f20, f6, f2, f2, rup\n"                     ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f6, f10, f17, f2, rne\n"                    ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f27, f11, f24, f21, rne\n"                  ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f30, f11, f15, f21, rne\n"                  ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f11, f30, f24, f22, dyn\n"                  ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f15, f30, f21, f22, rne\n"                  ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f28, f27, f20, f6, rmm\n"                   ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f2, f8, f13, f10, rmm\n"                    ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f26, f14, f17, f10, rdn\n"                  ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f17, f15, f21, f10, rmm\n"                  ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f3, f26, f28, f4, rup\n"                    ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f28, f22, f18, f11, rdn\n"                  ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f25, f19, f25, f30, rne\n"                  ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f17, f4, f7, f20, rtz\n"                    ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f7, f24, f30, f4, dyn\n"                    ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f5, f5, f23, f11, dyn\n"                    ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f2, f7, f2, f18, dyn\n"                     ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f13, f29, f26, f23, rne\n"                  ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f4, f9, f10, f19, rtz\n"                    ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f5, f30, f27, f2, dyn\n"                    ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f18, f7, f27, f6, rmm\n"                    ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f4, f7, f6, f6, rdn\n"                      ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f19, f1, f2, f29, rmm\n"                    ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f6, f18, f28, f10, rdn\n"                   ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f29, f12, f16, f30, rne\n"                  ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f30, f1, f20, f29, rmm\n"                   ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f11, f15, f26, f15, rdn\n"                  ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f14, f17, f27, f5, rup\n"                   ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f30, f20, f14, f26, dyn\n"                  ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f9, f13, f27, f29, rmm\n"                   ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f12, f21, f8, f18, rne\n"                   ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f29, f14, f23, f27, rmm\n"                  ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f12, f28, f24, f19, rmm\n"                  ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f8, f17, f27, f19, rmm\n"                   ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f8, f17, f29, f10, rtz\n"                   ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f2, f12, f29, f12, rmm\n"                   ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f29, f27, f30, f17, rup\n"                  ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f17, f3, f0, f24, dyn\n"                    ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f18, f2, f21, f8, rtz\n"                    ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f30, f2, f3, f10, rdn\n"                    ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f9, f7, f29, f12, dyn\n"                    ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f17, f9, f16, f4, rne\n"                    ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f3, f2, f4, f2, rtz\n"                      ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f14, f16, f18, f21, rdn\n"                  ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f24, f24, f24, f5, rdn\n"                   ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f16, f9, f0, f18, rup\n"                    ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f6, f13, f23, f8, rmm\n"                    ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f22, f2, f24, f29, rdn\n"                   ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f27, f2, f16, f11, rdn\n"                   ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f3, f30, f0, f10, rmm\n"                    ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f23, f18, f10, f12, rtz\n"                  ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f25, f4, f22, f22, rdn\n"                   ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f27, f19, f10, f24, rup\n"                  ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f19, f24, f4, f30, rup\n"                   ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f26, f13, f15, f27, rup\n"                  ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f6, f18, f28, f3, rtz\n"                    ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f19, f2, f21, f12, dyn\n"                   ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f29, f2, f2, f26, rdn\n"                    ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f23, f1, f22, f4, dyn\n"                    ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f23, f14, f20, f18, rtz\n"                  ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f25, f22, f10, f24, dyn\n"                  ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f27, f11, f18, f11, rtz\n"                  ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f8, f6, f8, f8, rmm\n"                      ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f18, f22, f8, f12, rup\n"                   ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f25, f9, f4, f29, rup\n"                    ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f30, f14, f14, f27, rup\n"                  ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f28, f5, f29, f24, dyn\n"                   ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f22, f10, f21, f3, rne\n"                   ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f27, f0, f11, f20, dyn\n"                   ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f6, f17, f24, f18, rtz\n"                   ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f24, f14, f13, f7, dyn\n"                   ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f24, f30, f20, f13, rne\n"                  ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f20, f15, f18, f15, rmm\n"                  ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f27, f10, f21, f23, rtz\n"                  ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmadd.ps f5, f30, f29, f26, rmm\n"                   ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
