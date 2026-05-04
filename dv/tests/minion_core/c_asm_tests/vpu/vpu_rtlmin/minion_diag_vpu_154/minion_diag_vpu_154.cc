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
			 0xe245c37f, /// 0x0
			 0x0fde94c8, /// 0x4
			 0xf14ac9c2, /// 0x8
			 0xd7f2f33d, /// 0xc
			 0xd8d86787, /// 0x10
			 0x758883d5, /// 0x14
			 0x922d97fe, /// 0x18
			 0x1cf4caed, /// 0x1c
			 0x6f3d21f1, /// 0x20
			 0x33ae3ba9, /// 0x24
			 0x66b7d45c, /// 0x28
			 0x72814bb7, /// 0x2c
			 0xf0faecdd, /// 0x30
			 0xa86486c8, /// 0x34
			 0x138b582b, /// 0x38
			 0x2245bf1b, /// 0x3c
			 0x495909be, /// 0x40
			 0x86ba97b8, /// 0x44
			 0x45daccc6, /// 0x48
			 0x6b1d371c, /// 0x4c
			 0x41d3e4bb, /// 0x50
			 0x8f8a6142, /// 0x54
			 0xaf699100, /// 0x58
			 0xdbc56382, /// 0x5c
			 0xc032011f, /// 0x60
			 0x5a503808, /// 0x64
			 0xe1740ccf, /// 0x68
			 0xc90c240a, /// 0x6c
			 0x3e49d45c, /// 0x70
			 0x56b8640a, /// 0x74
			 0x74008297, /// 0x78
			 0xcaa67c6d, /// 0x7c
			 0x109bed30, /// 0x80
			 0x68915f32, /// 0x84
			 0xd719f489, /// 0x88
			 0x42e63c41, /// 0x8c
			 0xaccd76a7, /// 0x90
			 0x17981997, /// 0x94
			 0x57c90391, /// 0x98
			 0x07d0bba3, /// 0x9c
			 0x8729cbc4, /// 0xa0
			 0xe0036f6e, /// 0xa4
			 0xe4fb0526, /// 0xa8
			 0x8ea8bea7, /// 0xac
			 0x779381e5, /// 0xb0
			 0xbe7944d3, /// 0xb4
			 0xce2d9108, /// 0xb8
			 0xc4275da2, /// 0xbc
			 0x1bc15806, /// 0xc0
			 0x26c1729b, /// 0xc4
			 0x57943d5b, /// 0xc8
			 0x84a0b806, /// 0xcc
			 0x815cae1d, /// 0xd0
			 0xbbb600c4, /// 0xd4
			 0x164378b9, /// 0xd8
			 0xa40da366, /// 0xdc
			 0xbbe2a6aa, /// 0xe0
			 0x0549eba3, /// 0xe4
			 0xa90b3d34, /// 0xe8
			 0x2203f5e5, /// 0xec
			 0x04b5e5e7, /// 0xf0
			 0x237b66fe, /// 0xf4
			 0xbe4f5690, /// 0xf8
			 0x2b800ad7, /// 0xfc
			 0xd4dcefe1, /// 0x100
			 0x40d72f6e, /// 0x104
			 0xa0f86b20, /// 0x108
			 0xbcda0e7d, /// 0x10c
			 0x5abae1a8, /// 0x110
			 0x88c818b0, /// 0x114
			 0x0c46de11, /// 0x118
			 0x45865e69, /// 0x11c
			 0x8673cb3c, /// 0x120
			 0x281ede48, /// 0x124
			 0x8881e1c6, /// 0x128
			 0x07ce2ec1, /// 0x12c
			 0xd4e0db63, /// 0x130
			 0xce786083, /// 0x134
			 0x7413ed83, /// 0x138
			 0x6a385419, /// 0x13c
			 0xb5e9bb01, /// 0x140
			 0x1b00cfcc, /// 0x144
			 0x811fce76, /// 0x148
			 0x4eaaa4d3, /// 0x14c
			 0x84b9cef6, /// 0x150
			 0x61526d2e, /// 0x154
			 0x43112dd4, /// 0x158
			 0xd31c473c, /// 0x15c
			 0xe1765b74, /// 0x160
			 0x4fa99a55, /// 0x164
			 0xb7f113c3, /// 0x168
			 0x76d6fb14, /// 0x16c
			 0x44530cea, /// 0x170
			 0xcb2ea1a4, /// 0x174
			 0x3db47b1e, /// 0x178
			 0xc9f7be21, /// 0x17c
			 0x56fa3dcb, /// 0x180
			 0xea15e747, /// 0x184
			 0xbbccc7f0, /// 0x188
			 0xf86bc620, /// 0x18c
			 0xb966b29b, /// 0x190
			 0x77608320, /// 0x194
			 0x3bb60859, /// 0x198
			 0x9f2b1fcd, /// 0x19c
			 0xb4e3808c, /// 0x1a0
			 0xddb0fb57, /// 0x1a4
			 0x2f17c55e, /// 0x1a8
			 0x86f32867, /// 0x1ac
			 0xae170c1f, /// 0x1b0
			 0x0f20a4a1, /// 0x1b4
			 0xd0dac65e, /// 0x1b8
			 0xd1457e90, /// 0x1bc
			 0xfe45e60a, /// 0x1c0
			 0xad99c0f2, /// 0x1c4
			 0x3037a434, /// 0x1c8
			 0xca127211, /// 0x1cc
			 0x1cbb0450, /// 0x1d0
			 0x7e244c84, /// 0x1d4
			 0xdcc16587, /// 0x1d8
			 0xa983fbcc, /// 0x1dc
			 0xe7b09410, /// 0x1e0
			 0x5126593d, /// 0x1e4
			 0xe817e20c, /// 0x1e8
			 0x6339bc1e, /// 0x1ec
			 0x91b2b73c, /// 0x1f0
			 0x07a59cde, /// 0x1f4
			 0xa7767cc6, /// 0x1f8
			 0xe23d46de, /// 0x1fc
			 0x5e80e2a4, /// 0x200
			 0x8b38f3c4, /// 0x204
			 0x3b7026f9, /// 0x208
			 0xa300917c, /// 0x20c
			 0x48212444, /// 0x210
			 0xe238269e, /// 0x214
			 0x0146117c, /// 0x218
			 0xdfc79f78, /// 0x21c
			 0xec252edf, /// 0x220
			 0x6b4725ba, /// 0x224
			 0x7735221a, /// 0x228
			 0xc8f4666b, /// 0x22c
			 0x745fac20, /// 0x230
			 0xb3fa6552, /// 0x234
			 0xc9398172, /// 0x238
			 0xf87e8299, /// 0x23c
			 0xa7870b94, /// 0x240
			 0x1df79261, /// 0x244
			 0xff14026d, /// 0x248
			 0x9b5add81, /// 0x24c
			 0xbd2ba395, /// 0x250
			 0xb69993e1, /// 0x254
			 0x3ec850bb, /// 0x258
			 0x980c36a0, /// 0x25c
			 0x456f015b, /// 0x260
			 0xeb4836b1, /// 0x264
			 0x4d4e35af, /// 0x268
			 0x4804b766, /// 0x26c
			 0x17690fdd, /// 0x270
			 0xd8b15068, /// 0x274
			 0x5bad86e5, /// 0x278
			 0xbac2b04c, /// 0x27c
			 0x2dbfa710, /// 0x280
			 0x4157361d, /// 0x284
			 0x8b1f508d, /// 0x288
			 0x6ac2b968, /// 0x28c
			 0x427d4178, /// 0x290
			 0x893a4e02, /// 0x294
			 0x433f4804, /// 0x298
			 0xe2665ba3, /// 0x29c
			 0xbe77e312, /// 0x2a0
			 0x9965fa52, /// 0x2a4
			 0xd4327b09, /// 0x2a8
			 0xd47eeb28, /// 0x2ac
			 0x3b0b1e83, /// 0x2b0
			 0xca16def7, /// 0x2b4
			 0xe8abd0c0, /// 0x2b8
			 0x35d50f52, /// 0x2bc
			 0x240643ef, /// 0x2c0
			 0x5c42905b, /// 0x2c4
			 0x0d320f16, /// 0x2c8
			 0x50d7764b, /// 0x2cc
			 0x3258a881, /// 0x2d0
			 0xd8a3148e, /// 0x2d4
			 0xc570bdf1, /// 0x2d8
			 0xc712c122, /// 0x2dc
			 0x2d9a78c6, /// 0x2e0
			 0x570ded85, /// 0x2e4
			 0x72dce22e, /// 0x2e8
			 0xcf31bddb, /// 0x2ec
			 0x484e1425, /// 0x2f0
			 0x1c5920aa, /// 0x2f4
			 0xe9c9592e, /// 0x2f8
			 0xeef8c920, /// 0x2fc
			 0x20dc336f, /// 0x300
			 0xb658f628, /// 0x304
			 0xfb68402e, /// 0x308
			 0xba6c04a4, /// 0x30c
			 0x22a6c6c6, /// 0x310
			 0x90d458de, /// 0x314
			 0x8cb92202, /// 0x318
			 0xf6f8f3ad, /// 0x31c
			 0xa5011c46, /// 0x320
			 0x333566f3, /// 0x324
			 0x2a348bfd, /// 0x328
			 0xa0b86b5a, /// 0x32c
			 0x94c042d1, /// 0x330
			 0xd33d30fd, /// 0x334
			 0x6e6ff633, /// 0x338
			 0x3f882037, /// 0x33c
			 0x1ea2853e, /// 0x340
			 0x69ea1550, /// 0x344
			 0x33ad39f3, /// 0x348
			 0x24220d03, /// 0x34c
			 0xc1b805ea, /// 0x350
			 0x0664fa4f, /// 0x354
			 0x937ea2e0, /// 0x358
			 0x579f28ac, /// 0x35c
			 0x2cdee5bf, /// 0x360
			 0xbe555fae, /// 0x364
			 0xe4eed2d3, /// 0x368
			 0xe7e2af45, /// 0x36c
			 0x0fedd940, /// 0x370
			 0xc489ed3c, /// 0x374
			 0x128e38bf, /// 0x378
			 0x5eb22756, /// 0x37c
			 0x848c92cc, /// 0x380
			 0x2d223374, /// 0x384
			 0xe76dbc3e, /// 0x388
			 0xeaaf3ba6, /// 0x38c
			 0x2963cfb8, /// 0x390
			 0x360f2bc6, /// 0x394
			 0x50335a78, /// 0x398
			 0x08f5fda2, /// 0x39c
			 0xcbfe3301, /// 0x3a0
			 0x291f5459, /// 0x3a4
			 0x3c8a2310, /// 0x3a8
			 0xcf758312, /// 0x3ac
			 0xfdc35f17, /// 0x3b0
			 0xc12dd0fb, /// 0x3b4
			 0xeee9177d, /// 0x3b8
			 0xf31cbe27, /// 0x3bc
			 0x3afaee18, /// 0x3c0
			 0xacaef19f, /// 0x3c4
			 0xdcd3c256, /// 0x3c8
			 0x33c28914, /// 0x3cc
			 0x34660034, /// 0x3d0
			 0x4df9c827, /// 0x3d4
			 0xd338609c, /// 0x3d8
			 0xc895198f, /// 0x3dc
			 0x2c76fe36, /// 0x3e0
			 0xeb1fa4b9, /// 0x3e4
			 0xf7033cdd, /// 0x3e8
			 0x00049804, /// 0x3ec
			 0x2f7468da, /// 0x3f0
			 0xc64ee22e, /// 0x3f4
			 0x7a6b7d74, /// 0x3f8
			 0xd7a2a7d4, /// 0x3fc
			 0x64d99266, /// 0x400
			 0x7396a1f2, /// 0x404
			 0x801ac39a, /// 0x408
			 0x99f3cbde, /// 0x40c
			 0x9ff32dd8, /// 0x410
			 0x167cfea3, /// 0x414
			 0x438a0ed6, /// 0x418
			 0x015ff04c, /// 0x41c
			 0x56168fb3, /// 0x420
			 0x294b5197, /// 0x424
			 0xe73b651e, /// 0x428
			 0xb0e1705d, /// 0x42c
			 0xcd250472, /// 0x430
			 0x2abbaf10, /// 0x434
			 0x1544afd5, /// 0x438
			 0xc898f145, /// 0x43c
			 0xa8213377, /// 0x440
			 0x94c5a7d6, /// 0x444
			 0x802a24c4, /// 0x448
			 0x5cc7bc86, /// 0x44c
			 0xa01a12df, /// 0x450
			 0x0d50a69c, /// 0x454
			 0x12e7f483, /// 0x458
			 0x423c5c9e, /// 0x45c
			 0x6315cd2c, /// 0x460
			 0x0976f6ad, /// 0x464
			 0x7a3770a2, /// 0x468
			 0xced4eacd, /// 0x46c
			 0xe6e1e1c3, /// 0x470
			 0x272afb09, /// 0x474
			 0x343ba105, /// 0x478
			 0x0658b1cc, /// 0x47c
			 0x40a7b118, /// 0x480
			 0xdb51d04f, /// 0x484
			 0x7c7bb703, /// 0x488
			 0xc6e2ddf5, /// 0x48c
			 0x1b6962f2, /// 0x490
			 0xcd0adafe, /// 0x494
			 0x23039de6, /// 0x498
			 0xe882028b, /// 0x49c
			 0xe34cf171, /// 0x4a0
			 0x589df561, /// 0x4a4
			 0x58aec385, /// 0x4a8
			 0x6a40f49a, /// 0x4ac
			 0x9c0c63bf, /// 0x4b0
			 0xbf2faf9a, /// 0x4b4
			 0x7d45f59b, /// 0x4b8
			 0x4dfc9064, /// 0x4bc
			 0x3f32a817, /// 0x4c0
			 0xdd93986b, /// 0x4c4
			 0x8e899bdb, /// 0x4c8
			 0xc1cb3d2a, /// 0x4cc
			 0xa082ff18, /// 0x4d0
			 0xa0f3cb85, /// 0x4d4
			 0x03d79550, /// 0x4d8
			 0x100653c5, /// 0x4dc
			 0xd512688b, /// 0x4e0
			 0x3787c028, /// 0x4e4
			 0x18fc9cb6, /// 0x4e8
			 0x63903bfd, /// 0x4ec
			 0x65caf4ff, /// 0x4f0
			 0x50a322b0, /// 0x4f4
			 0xe11e9714, /// 0x4f8
			 0x18909a65, /// 0x4fc
			 0x12a09827, /// 0x500
			 0x4bd84924, /// 0x504
			 0x726b4cce, /// 0x508
			 0x287b026d, /// 0x50c
			 0xfa1f30e5, /// 0x510
			 0x1a5c4521, /// 0x514
			 0x9e0ca4ac, /// 0x518
			 0xb20915a1, /// 0x51c
			 0x8eb097a5, /// 0x520
			 0x3d7b505a, /// 0x524
			 0xd04ec5e6, /// 0x528
			 0x54a15cff, /// 0x52c
			 0x1ee662c2, /// 0x530
			 0xac3ea406, /// 0x534
			 0x908a4d93, /// 0x538
			 0xfcf0359a, /// 0x53c
			 0xf3ac6b59, /// 0x540
			 0xb51416e0, /// 0x544
			 0xc34668ff, /// 0x548
			 0x1ef682be, /// 0x54c
			 0x37fbf397, /// 0x550
			 0x68dfd818, /// 0x554
			 0x585d8289, /// 0x558
			 0x1353334d, /// 0x55c
			 0x924e67b0, /// 0x560
			 0x603fb368, /// 0x564
			 0xf8764018, /// 0x568
			 0xef73a817, /// 0x56c
			 0xb0250129, /// 0x570
			 0x9533d73c, /// 0x574
			 0x41efa7b3, /// 0x578
			 0x66590360, /// 0x57c
			 0x4cc4273c, /// 0x580
			 0xd21590e4, /// 0x584
			 0xde7fbd63, /// 0x588
			 0x46f8b677, /// 0x58c
			 0x240f1d82, /// 0x590
			 0x5ab13882, /// 0x594
			 0x6d136058, /// 0x598
			 0x5b69a8ae, /// 0x59c
			 0x1d3add45, /// 0x5a0
			 0xb11befcd, /// 0x5a4
			 0x19ffbd8e, /// 0x5a8
			 0x2af64d9b, /// 0x5ac
			 0xb5f7fb95, /// 0x5b0
			 0x0fe078c8, /// 0x5b4
			 0x99383504, /// 0x5b8
			 0x64f9f2b3, /// 0x5bc
			 0xc6143b3a, /// 0x5c0
			 0x0df40160, /// 0x5c4
			 0xe2121370, /// 0x5c8
			 0x397ac24f, /// 0x5cc
			 0xe2ebd4a4, /// 0x5d0
			 0x5a61b3a1, /// 0x5d4
			 0x4e957a52, /// 0x5d8
			 0x6495313d, /// 0x5dc
			 0xb894e31b, /// 0x5e0
			 0x5941baac, /// 0x5e4
			 0x33bae0b5, /// 0x5e8
			 0x34f2ec6c, /// 0x5ec
			 0x3d8fe601, /// 0x5f0
			 0x5efe804f, /// 0x5f4
			 0x15f8a21d, /// 0x5f8
			 0x23912c19, /// 0x5fc
			 0xa8f7c6c8, /// 0x600
			 0xc5432eda, /// 0x604
			 0x02d4b51b, /// 0x608
			 0x5514514f, /// 0x60c
			 0xd8058bfe, /// 0x610
			 0x38547200, /// 0x614
			 0xff5f556f, /// 0x618
			 0x29ab33c0, /// 0x61c
			 0x8f0008bc, /// 0x620
			 0x9c8a4dad, /// 0x624
			 0xa1bd65db, /// 0x628
			 0x740bf4e3, /// 0x62c
			 0x8ed0fc10, /// 0x630
			 0xf2b616ae, /// 0x634
			 0x0f8ef433, /// 0x638
			 0x9622c498, /// 0x63c
			 0xb4a23802, /// 0x640
			 0xdac49c5b, /// 0x644
			 0x7b78137f, /// 0x648
			 0xed68e015, /// 0x64c
			 0xcddeff21, /// 0x650
			 0xfaab69ad, /// 0x654
			 0x103159a4, /// 0x658
			 0x49f03a91, /// 0x65c
			 0x670f02d9, /// 0x660
			 0x102dc73d, /// 0x664
			 0x1fa247d6, /// 0x668
			 0x0f284f35, /// 0x66c
			 0xdb6df3f7, /// 0x670
			 0x7a17d139, /// 0x674
			 0x297774e2, /// 0x678
			 0xa6be0d8e, /// 0x67c
			 0xe31dba15, /// 0x680
			 0xdbc258b6, /// 0x684
			 0x69c424f7, /// 0x688
			 0xe36ff673, /// 0x68c
			 0x1f032cf3, /// 0x690
			 0x1869520a, /// 0x694
			 0x37e717c3, /// 0x698
			 0x633eeba9, /// 0x69c
			 0x382c1ac2, /// 0x6a0
			 0xe4c0e028, /// 0x6a4
			 0x16df105e, /// 0x6a8
			 0x72871139, /// 0x6ac
			 0x2706b5d5, /// 0x6b0
			 0xf57d97cd, /// 0x6b4
			 0xe059f3e8, /// 0x6b8
			 0xe5ac62c0, /// 0x6bc
			 0x0e99c76b, /// 0x6c0
			 0xd5beb9ba, /// 0x6c4
			 0x4507e6bd, /// 0x6c8
			 0x877e8324, /// 0x6cc
			 0x0763fdfd, /// 0x6d0
			 0xa720c2c1, /// 0x6d4
			 0x9ffb9795, /// 0x6d8
			 0x13ea3888, /// 0x6dc
			 0x3748fd73, /// 0x6e0
			 0xd1b6f7c8, /// 0x6e4
			 0xd0e26d47, /// 0x6e8
			 0x93805469, /// 0x6ec
			 0x06bbd1c3, /// 0x6f0
			 0xdb0677f6, /// 0x6f4
			 0x722780ce, /// 0x6f8
			 0x14ca0688, /// 0x6fc
			 0xe0ca602f, /// 0x700
			 0x453844f8, /// 0x704
			 0x56c2b7e1, /// 0x708
			 0x2a9df654, /// 0x70c
			 0x8a93cb84, /// 0x710
			 0x5c68ae2d, /// 0x714
			 0xb9a0e412, /// 0x718
			 0x9b5d85cb, /// 0x71c
			 0xc60eee4c, /// 0x720
			 0x51a5b33b, /// 0x724
			 0x7bfcabe2, /// 0x728
			 0x730b9bbe, /// 0x72c
			 0x39442011, /// 0x730
			 0xb3d4c144, /// 0x734
			 0xe3dcfe9e, /// 0x738
			 0x659f4482, /// 0x73c
			 0x3161a200, /// 0x740
			 0x8681ea6d, /// 0x744
			 0xd0c0e078, /// 0x748
			 0xf578696b, /// 0x74c
			 0xbde81fb1, /// 0x750
			 0xe05e216e, /// 0x754
			 0x91bc29ab, /// 0x758
			 0xe8e39768, /// 0x75c
			 0xce9a3209, /// 0x760
			 0x871af141, /// 0x764
			 0xca6c10ae, /// 0x768
			 0xf4615ecd, /// 0x76c
			 0xb1673478, /// 0x770
			 0x376ae0cc, /// 0x774
			 0xae755f44, /// 0x778
			 0xcebe007f, /// 0x77c
			 0x51f30245, /// 0x780
			 0x2a42393c, /// 0x784
			 0x27e47103, /// 0x788
			 0x58e2d732, /// 0x78c
			 0x3e86bd98, /// 0x790
			 0x76fc5c33, /// 0x794
			 0xa9200080, /// 0x798
			 0x1d88bd98, /// 0x79c
			 0x58c05c3d, /// 0x7a0
			 0x316937e8, /// 0x7a4
			 0x29c7d42d, /// 0x7a8
			 0x6b387f10, /// 0x7ac
			 0x29fcff3f, /// 0x7b0
			 0x7522aaf0, /// 0x7b4
			 0x14d8b4f3, /// 0x7b8
			 0x6493651a, /// 0x7bc
			 0xdc83ad02, /// 0x7c0
			 0x0a648fdd, /// 0x7c4
			 0xfeeb1188, /// 0x7c8
			 0xede17e81, /// 0x7cc
			 0x5ece123f, /// 0x7d0
			 0x59403e32, /// 0x7d4
			 0x3ccd67c6, /// 0x7d8
			 0x1d8148be, /// 0x7dc
			 0x1895712e, /// 0x7e0
			 0x824b54b9, /// 0x7e4
			 0xa0da4757, /// 0x7e8
			 0x31ada9aa, /// 0x7ec
			 0xf8d2cf48, /// 0x7f0
			 0xd0f0e5e7, /// 0x7f4
			 0x490c7a01, /// 0x7f8
			 0xd97a26df, /// 0x7fc
			 0x6c7c9a90, /// 0x800
			 0xdc36e51d, /// 0x804
			 0x2a1a59aa, /// 0x808
			 0x6150f410, /// 0x80c
			 0xf8c8f203, /// 0x810
			 0xfc21b271, /// 0x814
			 0x60dbeb7e, /// 0x818
			 0xce2be7ba, /// 0x81c
			 0xc5d2cd5d, /// 0x820
			 0xb0eb1b36, /// 0x824
			 0xfc8ef98a, /// 0x828
			 0xce7a68da, /// 0x82c
			 0xa73c04ee, /// 0x830
			 0x79330430, /// 0x834
			 0x6c8ce492, /// 0x838
			 0x95e37087, /// 0x83c
			 0x8a5961c6, /// 0x840
			 0x3bb95251, /// 0x844
			 0xbdea98e6, /// 0x848
			 0x41ed3d05, /// 0x84c
			 0xf72f7c96, /// 0x850
			 0xffdef3c8, /// 0x854
			 0x856f2ce0, /// 0x858
			 0xb1455630, /// 0x85c
			 0x34b4db4a, /// 0x860
			 0x8db377f1, /// 0x864
			 0xe4960e19, /// 0x868
			 0xaa61d235, /// 0x86c
			 0xe2e205b8, /// 0x870
			 0x74e5e815, /// 0x874
			 0x0412e3b0, /// 0x878
			 0x18d6761d, /// 0x87c
			 0xa9ca0545, /// 0x880
			 0x5fb6eee5, /// 0x884
			 0xbcd98bc8, /// 0x888
			 0xf16f50cd, /// 0x88c
			 0x0517eccd, /// 0x890
			 0x252a3db4, /// 0x894
			 0xb9ad37f8, /// 0x898
			 0xc2ea07c0, /// 0x89c
			 0xf7f31a33, /// 0x8a0
			 0x447446aa, /// 0x8a4
			 0x3bad85bd, /// 0x8a8
			 0xc045c28c, /// 0x8ac
			 0xac35b7ed, /// 0x8b0
			 0x4982954d, /// 0x8b4
			 0x384c0373, /// 0x8b8
			 0xda6b64c3, /// 0x8bc
			 0xb183a6fe, /// 0x8c0
			 0x33f14bbe, /// 0x8c4
			 0x00e13ebf, /// 0x8c8
			 0x594b9955, /// 0x8cc
			 0x80d499de, /// 0x8d0
			 0xda34d3f7, /// 0x8d4
			 0xa17df2c7, /// 0x8d8
			 0xcc917f05, /// 0x8dc
			 0x6d93e067, /// 0x8e0
			 0x02b86e16, /// 0x8e4
			 0xfe60fe65, /// 0x8e8
			 0x2ab1f715, /// 0x8ec
			 0x143c04e9, /// 0x8f0
			 0x01b33a59, /// 0x8f4
			 0x3954ad77, /// 0x8f8
			 0x2e5aa188, /// 0x8fc
			 0x46005312, /// 0x900
			 0x89864c60, /// 0x904
			 0x45e30b97, /// 0x908
			 0x087392f2, /// 0x90c
			 0xbf3a68ea, /// 0x910
			 0xff8fb46c, /// 0x914
			 0xaf4cf995, /// 0x918
			 0x75079d29, /// 0x91c
			 0xcbee29ab, /// 0x920
			 0x399b08c1, /// 0x924
			 0xaee7dd15, /// 0x928
			 0x2643857d, /// 0x92c
			 0x9741c6fc, /// 0x930
			 0xf5c6b54f, /// 0x934
			 0x558e8021, /// 0x938
			 0x617db1f5, /// 0x93c
			 0x3c4633a9, /// 0x940
			 0xe439b28f, /// 0x944
			 0x630b3c84, /// 0x948
			 0xc7d92aae, /// 0x94c
			 0x62108a00, /// 0x950
			 0x77aba5a0, /// 0x954
			 0x53d8a6bc, /// 0x958
			 0x69c54c2e, /// 0x95c
			 0xb2fe705e, /// 0x960
			 0x59ef0745, /// 0x964
			 0xf2d474c5, /// 0x968
			 0x7bed73dd, /// 0x96c
			 0x0f67159a, /// 0x970
			 0xe97aa953, /// 0x974
			 0x3a64ed3c, /// 0x978
			 0xf3f202c6, /// 0x97c
			 0x0525f99a, /// 0x980
			 0x95dd5f4f, /// 0x984
			 0xd871fa36, /// 0x988
			 0x474618af, /// 0x98c
			 0x3248b1fe, /// 0x990
			 0x88994b24, /// 0x994
			 0xa59278a8, /// 0x998
			 0x87552013, /// 0x99c
			 0x4e6b2ae0, /// 0x9a0
			 0x1b72986b, /// 0x9a4
			 0xe718b649, /// 0x9a8
			 0x0137f07a, /// 0x9ac
			 0x9e91352f, /// 0x9b0
			 0x9e9e29aa, /// 0x9b4
			 0x762b1aca, /// 0x9b8
			 0xfa19c816, /// 0x9bc
			 0x3542124b, /// 0x9c0
			 0xd1f9a3f5, /// 0x9c4
			 0xf814482e, /// 0x9c8
			 0xd662a4fb, /// 0x9cc
			 0x096adbf8, /// 0x9d0
			 0x00bb4f6f, /// 0x9d4
			 0x9eb90867, /// 0x9d8
			 0x1bfb56a1, /// 0x9dc
			 0x1af2828e, /// 0x9e0
			 0x23cf4b17, /// 0x9e4
			 0xda1a299b, /// 0x9e8
			 0x84b97f64, /// 0x9ec
			 0x1f7a717d, /// 0x9f0
			 0x3a16ddde, /// 0x9f4
			 0x929cb249, /// 0x9f8
			 0x492ffc34, /// 0x9fc
			 0x89909910, /// 0xa00
			 0x74fd8471, /// 0xa04
			 0x296f2fca, /// 0xa08
			 0xa2ffc669, /// 0xa0c
			 0xc1703e8f, /// 0xa10
			 0x3e096f20, /// 0xa14
			 0x59f54a60, /// 0xa18
			 0x4d2ca1c1, /// 0xa1c
			 0x7bfe4adb, /// 0xa20
			 0xf214a0de, /// 0xa24
			 0x42ef1e95, /// 0xa28
			 0x5d87cfd2, /// 0xa2c
			 0x15159c9d, /// 0xa30
			 0xfce5dfb7, /// 0xa34
			 0x09abccf5, /// 0xa38
			 0x3bd29cd2, /// 0xa3c
			 0x93597fc3, /// 0xa40
			 0xd1242632, /// 0xa44
			 0xee17d63c, /// 0xa48
			 0x9cacce1c, /// 0xa4c
			 0xb3b32224, /// 0xa50
			 0x5ea747b1, /// 0xa54
			 0x269e966f, /// 0xa58
			 0xad57817d, /// 0xa5c
			 0xe11c3d07, /// 0xa60
			 0xe8a88df1, /// 0xa64
			 0x669dd089, /// 0xa68
			 0x9d84b149, /// 0xa6c
			 0x1c73d67e, /// 0xa70
			 0x150aa0a1, /// 0xa74
			 0x4c7394ca, /// 0xa78
			 0xa4eae313, /// 0xa7c
			 0x05c2e24d, /// 0xa80
			 0x3fcfa112, /// 0xa84
			 0xc61c452a, /// 0xa88
			 0x64cade13, /// 0xa8c
			 0xb040d3dc, /// 0xa90
			 0x241b7816, /// 0xa94
			 0xdf08561b, /// 0xa98
			 0x437abe18, /// 0xa9c
			 0xfa66ec38, /// 0xaa0
			 0xd822a498, /// 0xaa4
			 0xfacf4432, /// 0xaa8
			 0x59c16f2a, /// 0xaac
			 0x72d47073, /// 0xab0
			 0x77422062, /// 0xab4
			 0xc297df86, /// 0xab8
			 0x15b98bb4, /// 0xabc
			 0xa9e56088, /// 0xac0
			 0xba933903, /// 0xac4
			 0xd3fc8557, /// 0xac8
			 0x192bb043, /// 0xacc
			 0x634af65e, /// 0xad0
			 0x2426fdb1, /// 0xad4
			 0x9b94e635, /// 0xad8
			 0x77c5927b, /// 0xadc
			 0x30b70914, /// 0xae0
			 0x54e90c8f, /// 0xae4
			 0x103d3e69, /// 0xae8
			 0xfb819329, /// 0xaec
			 0xbabd2f45, /// 0xaf0
			 0x7c7df98c, /// 0xaf4
			 0x5a77593c, /// 0xaf8
			 0xb2d9425f, /// 0xafc
			 0xe27f50ce, /// 0xb00
			 0xbf2c2d2b, /// 0xb04
			 0x1b6fc350, /// 0xb08
			 0x5b6454d8, /// 0xb0c
			 0xd43194f4, /// 0xb10
			 0xd3101a7d, /// 0xb14
			 0x4d2d1234, /// 0xb18
			 0xb49a50de, /// 0xb1c
			 0xefdb16a7, /// 0xb20
			 0xffcbd49c, /// 0xb24
			 0x4837a038, /// 0xb28
			 0x9eca20e7, /// 0xb2c
			 0x95b143e9, /// 0xb30
			 0x367033be, /// 0xb34
			 0xc0ac7bfe, /// 0xb38
			 0x8e9cdd3f, /// 0xb3c
			 0x3ff2749f, /// 0xb40
			 0x575103d3, /// 0xb44
			 0x1114162a, /// 0xb48
			 0x4e7ad31a, /// 0xb4c
			 0x585b52b0, /// 0xb50
			 0xa3212f71, /// 0xb54
			 0x552c3f42, /// 0xb58
			 0xd9baf9bc, /// 0xb5c
			 0xc79019b8, /// 0xb60
			 0x43a4b1de, /// 0xb64
			 0xbef8d55c, /// 0xb68
			 0x0b231add, /// 0xb6c
			 0x5b16e8e8, /// 0xb70
			 0x03847ef5, /// 0xb74
			 0xcf6b28fc, /// 0xb78
			 0xcf47b2e9, /// 0xb7c
			 0x0ee570c0, /// 0xb80
			 0x746a58ea, /// 0xb84
			 0x4863e36d, /// 0xb88
			 0x9878e77d, /// 0xb8c
			 0x37fad87b, /// 0xb90
			 0x6d7d3ed1, /// 0xb94
			 0x7c34e82d, /// 0xb98
			 0x7ba6255b, /// 0xb9c
			 0xee343f5a, /// 0xba0
			 0x3bd203d8, /// 0xba4
			 0x07a7be27, /// 0xba8
			 0x2de050e0, /// 0xbac
			 0x3700b25e, /// 0xbb0
			 0x6514bb3c, /// 0xbb4
			 0xea6e060f, /// 0xbb8
			 0x9baec382, /// 0xbbc
			 0xe500052d, /// 0xbc0
			 0x0e8d90c4, /// 0xbc4
			 0x71873589, /// 0xbc8
			 0xb07c6beb, /// 0xbcc
			 0x81a5cd6a, /// 0xbd0
			 0xed2a8e07, /// 0xbd4
			 0xa9a7f7eb, /// 0xbd8
			 0x19c7505b, /// 0xbdc
			 0xb3cbb291, /// 0xbe0
			 0xdaee28d5, /// 0xbe4
			 0x14acbc71, /// 0xbe8
			 0xa4bb7b27, /// 0xbec
			 0x409c278b, /// 0xbf0
			 0xa2f2ea6a, /// 0xbf4
			 0x1928cb93, /// 0xbf8
			 0x81642ff5, /// 0xbfc
			 0xe6914aac, /// 0xc00
			 0x0d5c04f6, /// 0xc04
			 0x162bcad8, /// 0xc08
			 0x587664dd, /// 0xc0c
			 0x19318971, /// 0xc10
			 0x861bb117, /// 0xc14
			 0x9e294a2b, /// 0xc18
			 0x699976ed, /// 0xc1c
			 0xd4f3ec29, /// 0xc20
			 0x7bbbaa9b, /// 0xc24
			 0x8d40bfb8, /// 0xc28
			 0x037a5bf7, /// 0xc2c
			 0x2d9851c7, /// 0xc30
			 0x71f0ce58, /// 0xc34
			 0x3683644b, /// 0xc38
			 0x8f0e8ab0, /// 0xc3c
			 0x5b930807, /// 0xc40
			 0xb0f4e007, /// 0xc44
			 0xa749aacd, /// 0xc48
			 0xcbfa0223, /// 0xc4c
			 0xa96dd3e9, /// 0xc50
			 0x1915caeb, /// 0xc54
			 0x329efa73, /// 0xc58
			 0xbb254fa7, /// 0xc5c
			 0x097143aa, /// 0xc60
			 0x633cb6a2, /// 0xc64
			 0x52fd08b7, /// 0xc68
			 0x97508a28, /// 0xc6c
			 0x5fe38450, /// 0xc70
			 0xee8dcdc9, /// 0xc74
			 0x14530481, /// 0xc78
			 0xa0b8add3, /// 0xc7c
			 0xe0c77cf0, /// 0xc80
			 0x1ca3b1ba, /// 0xc84
			 0x9e3a1659, /// 0xc88
			 0xb5b2b141, /// 0xc8c
			 0x0c4a08fd, /// 0xc90
			 0xe5d9c671, /// 0xc94
			 0x98036beb, /// 0xc98
			 0x63d252da, /// 0xc9c
			 0x72e70128, /// 0xca0
			 0x7488b43e, /// 0xca4
			 0x8bfc6034, /// 0xca8
			 0xa1f310fb, /// 0xcac
			 0xc82ca22a, /// 0xcb0
			 0x8340c75f, /// 0xcb4
			 0xe2f9834e, /// 0xcb8
			 0xe42afac6, /// 0xcbc
			 0xc2f9e98e, /// 0xcc0
			 0x9ed82830, /// 0xcc4
			 0x7bf9efdf, /// 0xcc8
			 0x0bebe235, /// 0xccc
			 0x614f4581, /// 0xcd0
			 0x8a27e5b7, /// 0xcd4
			 0xbc474046, /// 0xcd8
			 0x80a79131, /// 0xcdc
			 0x0e81749b, /// 0xce0
			 0x096ccb1c, /// 0xce4
			 0x9e2f7261, /// 0xce8
			 0x4f9a6c9b, /// 0xcec
			 0xbc629e0d, /// 0xcf0
			 0x3e6c8368, /// 0xcf4
			 0x489b1edc, /// 0xcf8
			 0x43c729f2, /// 0xcfc
			 0x77621bf3, /// 0xd00
			 0x37d04370, /// 0xd04
			 0x27d04171, /// 0xd08
			 0xb25e39d9, /// 0xd0c
			 0x52c523bb, /// 0xd10
			 0x9599d039, /// 0xd14
			 0x0c3c2b8a, /// 0xd18
			 0x944691f9, /// 0xd1c
			 0x4a6ef5f1, /// 0xd20
			 0x34c46467, /// 0xd24
			 0x1c8a6b75, /// 0xd28
			 0x0b5280e4, /// 0xd2c
			 0x6cd57c4d, /// 0xd30
			 0x56c7c061, /// 0xd34
			 0x0d2ccbf7, /// 0xd38
			 0xee1beb06, /// 0xd3c
			 0xce950ee5, /// 0xd40
			 0xb64ccc63, /// 0xd44
			 0xbeaa10bc, /// 0xd48
			 0x530c849b, /// 0xd4c
			 0xb2549140, /// 0xd50
			 0xe04f29b7, /// 0xd54
			 0x87ac5172, /// 0xd58
			 0x3215a06c, /// 0xd5c
			 0xc0b5eca3, /// 0xd60
			 0xac29af86, /// 0xd64
			 0xc9966e7d, /// 0xd68
			 0x6a83a948, /// 0xd6c
			 0x6d72ec6e, /// 0xd70
			 0x4904fc8e, /// 0xd74
			 0xbd579e6f, /// 0xd78
			 0xd11a6770, /// 0xd7c
			 0x2d263c76, /// 0xd80
			 0x80bc05c2, /// 0xd84
			 0x7093e619, /// 0xd88
			 0x3e6e2e56, /// 0xd8c
			 0x85f22308, /// 0xd90
			 0xba08592a, /// 0xd94
			 0x2b0639a6, /// 0xd98
			 0xaffa2ed3, /// 0xd9c
			 0x805fca93, /// 0xda0
			 0x0542822f, /// 0xda4
			 0xd94859ac, /// 0xda8
			 0x029eb9ec, /// 0xdac
			 0xe9c38fb0, /// 0xdb0
			 0xf2eb7891, /// 0xdb4
			 0x743c9834, /// 0xdb8
			 0x00309513, /// 0xdbc
			 0xfa7c78be, /// 0xdc0
			 0xbdebbd1a, /// 0xdc4
			 0x8b14f79e, /// 0xdc8
			 0xbcfff5c3, /// 0xdcc
			 0xe96e1ba6, /// 0xdd0
			 0x7c3f3a8a, /// 0xdd4
			 0x4da67369, /// 0xdd8
			 0x2e09813d, /// 0xddc
			 0x8c4a68b6, /// 0xde0
			 0xd5b1639e, /// 0xde4
			 0xfb0ad726, /// 0xde8
			 0x8349a899, /// 0xdec
			 0x4ca63742, /// 0xdf0
			 0xb6ba472f, /// 0xdf4
			 0x23de0f13, /// 0xdf8
			 0x3fa2644d, /// 0xdfc
			 0x05829bcf, /// 0xe00
			 0xc4e4dbda, /// 0xe04
			 0xf3e7872a, /// 0xe08
			 0x5bd964a2, /// 0xe0c
			 0xf61879a9, /// 0xe10
			 0xbc39636a, /// 0xe14
			 0xdd94819b, /// 0xe18
			 0x598c5951, /// 0xe1c
			 0x740c9d56, /// 0xe20
			 0x02c4deaa, /// 0xe24
			 0x45efc46e, /// 0xe28
			 0xc8de2da3, /// 0xe2c
			 0xe7b0f919, /// 0xe30
			 0x5935e1d2, /// 0xe34
			 0x433bfd3c, /// 0xe38
			 0x38365733, /// 0xe3c
			 0x2a085356, /// 0xe40
			 0x38b66e4a, /// 0xe44
			 0x4015fdbd, /// 0xe48
			 0x5d592ab8, /// 0xe4c
			 0x2c430abb, /// 0xe50
			 0x19854783, /// 0xe54
			 0x8dbf2337, /// 0xe58
			 0x5a011bc0, /// 0xe5c
			 0x6f2a7419, /// 0xe60
			 0x88541c1b, /// 0xe64
			 0x5a44a2a8, /// 0xe68
			 0xa6d1bb48, /// 0xe6c
			 0x734f542a, /// 0xe70
			 0xad6a0068, /// 0xe74
			 0xceb28e1d, /// 0xe78
			 0xbad5bf91, /// 0xe7c
			 0x53c179ef, /// 0xe80
			 0xd4d5d2c8, /// 0xe84
			 0x86fa49cf, /// 0xe88
			 0x6272283f, /// 0xe8c
			 0xa1fbf150, /// 0xe90
			 0x0273345a, /// 0xe94
			 0xcdd483d9, /// 0xe98
			 0x6822270c, /// 0xe9c
			 0x92ac7def, /// 0xea0
			 0xd940693c, /// 0xea4
			 0xb380b40f, /// 0xea8
			 0xf3008204, /// 0xeac
			 0xad541ed7, /// 0xeb0
			 0xb7567833, /// 0xeb4
			 0xd6a6ec20, /// 0xeb8
			 0xe348e5a1, /// 0xebc
			 0x637e05c5, /// 0xec0
			 0xa8be83db, /// 0xec4
			 0xa33d8046, /// 0xec8
			 0xe12b49dc, /// 0xecc
			 0xb169a7f7, /// 0xed0
			 0xe72fff7d, /// 0xed4
			 0x9f3f7a88, /// 0xed8
			 0x0b860e39, /// 0xedc
			 0xe3ff6b28, /// 0xee0
			 0xfadb7faf, /// 0xee4
			 0x262ac9fa, /// 0xee8
			 0x8d75aa71, /// 0xeec
			 0xac1b7814, /// 0xef0
			 0xe1e0d596, /// 0xef4
			 0x656259fa, /// 0xef8
			 0x5ec5c9b5, /// 0xefc
			 0x019df7e0, /// 0xf00
			 0xdc898cda, /// 0xf04
			 0x8d45c303, /// 0xf08
			 0xdbd1f262, /// 0xf0c
			 0x145afd07, /// 0xf10
			 0xc943c01f, /// 0xf14
			 0x954fb109, /// 0xf18
			 0x9e285223, /// 0xf1c
			 0x52600ea4, /// 0xf20
			 0xf70e664b, /// 0xf24
			 0x878b463d, /// 0xf28
			 0xf1ef3987, /// 0xf2c
			 0x087d5e98, /// 0xf30
			 0x8cec002a, /// 0xf34
			 0x35b87114, /// 0xf38
			 0x74f98281, /// 0xf3c
			 0x01bf4268, /// 0xf40
			 0xbf2b144b, /// 0xf44
			 0x057207ef, /// 0xf48
			 0x7f16759d, /// 0xf4c
			 0xa3bf979c, /// 0xf50
			 0xd5bb862a, /// 0xf54
			 0x088bd018, /// 0xf58
			 0x72d453c3, /// 0xf5c
			 0x33da6a69, /// 0xf60
			 0xb9877391, /// 0xf64
			 0x832389c5, /// 0xf68
			 0xb300d70b, /// 0xf6c
			 0xe5e36fec, /// 0xf70
			 0xf38b8c95, /// 0xf74
			 0xe5e558f0, /// 0xf78
			 0xe7084ad8, /// 0xf7c
			 0xc8655247, /// 0xf80
			 0x69e60a93, /// 0xf84
			 0xd1d6fc87, /// 0xf88
			 0x32199239, /// 0xf8c
			 0x73c34bb3, /// 0xf90
			 0xb7124cdb, /// 0xf94
			 0xd277be81, /// 0xf98
			 0xa8af102c, /// 0xf9c
			 0xb9907351, /// 0xfa0
			 0xbf31bbe3, /// 0xfa4
			 0xcd5e02cb, /// 0xfa8
			 0x0a0d22b9, /// 0xfac
			 0x932a14ca, /// 0xfb0
			 0x1e248913, /// 0xfb4
			 0xdc8cf5f4, /// 0xfb8
			 0x4b1a8032, /// 0xfbc
			 0x4e0f2c41, /// 0xfc0
			 0x459e2479, /// 0xfc4
			 0xa68b9b53, /// 0xfc8
			 0x08a976bf, /// 0xfcc
			 0xc53e45a0, /// 0xfd0
			 0x011b8e26, /// 0xfd4
			 0x531454bb, /// 0xfd8
			 0xdfd05607, /// 0xfdc
			 0x7ee246fa, /// 0xfe0
			 0x8aefde7f, /// 0xfe4
			 0x28f2bf90, /// 0xfe8
			 0x1a2a4a5e, /// 0xfec
			 0x2e3c3d40, /// 0xff0
			 0x35b99919, /// 0xff4
			 0x054b8735, /// 0xff8
			 0xa23a71e0 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00000
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00004
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00008
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0000c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00010
			 0x55555555,                                                  // 4 random values                             /// 00014
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0001c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00020
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00028
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00038
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0003c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00040
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00044
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0004c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00058
			 0x0e000003,                                                  // Trailing 1s:                                /// 0005c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00064
			 0x80000000,                                                  // -zero                                       /// 00068
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0007c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00080
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00088
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0008c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00090
			 0x00000000,                                                  // zero                                        /// 00094
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000a4
			 0x00000000,                                                  // zero                                        /// 000a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000ac
			 0x3f028f5c,                                                  // 0.51                                        /// 000b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000c0
			 0xff800000,                                                  // -inf                                        /// 000c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 000c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000d8
			 0xff800000,                                                  // -inf                                        /// 000dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0c780000,                                                  // Leading 1s:                                 /// 00104
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00108
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0010c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00110
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00118
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0011c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00120
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x0e000001,                                                  // Trailing 1s:                                /// 0012c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00134
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00138
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00140
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00148
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0014c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x0e000001,                                                  // Trailing 1s:                                /// 00154
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00160
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00168
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0016c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00170
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00178
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0017c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00180
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00188
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0018c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00194
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0019c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x33333333,                                                  // 4 random values                             /// 001a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001b0
			 0xffc00001,                                                  // -signaling NaN                              /// 001b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001d8
			 0xbf028f5c,                                                  // -0.51                                       /// 001dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00200
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00204
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00208
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0020c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00210
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00214
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00218
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00220
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0022c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00230
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00234
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x7fc00001,                                                  // signaling NaN                               /// 00240
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0024c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00250
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00254
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00260
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00268
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0026c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00270
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00288
			 0x3f028f5c,                                                  // 0.51                                        /// 0028c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00294
			 0x00000000,                                                  // zero                                        /// 00298
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0029c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00300
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0e000007,                                                  // Trailing 1s:                                /// 00308
			 0xff800000,                                                  // -inf                                        /// 0030c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00314
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0031c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00320
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00324
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00330
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00334
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00338
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00340
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00344
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0034c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00354
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0035c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00360
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00364
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00370
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00374
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00378
			 0x33333333,                                                  // 4 random values                             /// 0037c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00380
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00384
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00388
			 0x0c780000,                                                  // Leading 1s:                                 /// 0038c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00390
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00398
			 0x55555555,                                                  // 4 random values                             /// 0039c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c0
			 0x4b000000,                                                  // 8388608.0                                   /// 003c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003d0
			 0xbf028f5c,                                                  // -0.51                                       /// 003d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00404
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00408
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0040c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00414
			 0x0c780000,                                                  // Leading 1s:                                 /// 00418
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0041c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00424
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00428
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x55555555,                                                  // 4 random values                             /// 00430
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00434
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0043c
			 0xbf028f5c,                                                  // -0.51                                       /// 00440
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0044c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00450
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00454
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00458
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00460
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0046c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00470
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00474
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0047c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00480
			 0x4b000000,                                                  // 8388608.0                                   /// 00484
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x4b000000,                                                  // 8388608.0                                   /// 0048c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00498
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0049c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004a4
			 0xbf028f5c,                                                  // -0.51                                       /// 004a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x3f028f5c,                                                  // 0.51                                        /// 004dc
			 0x33333333,                                                  // 4 random values                             /// 004e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004e4
			 0x7f800000,                                                  // inf                                         /// 004e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00500
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00504
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00508
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0050c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00510
			 0x80000000,                                                  // -zero                                       /// 00514
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00518
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0051c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00520
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00524
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00528
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00534
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00540
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00544
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00548
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0054c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00550
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00554
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00558
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0055c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00560
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00564
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0056c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00570
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00574
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00578
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x80000000,                                                  // -zero                                       /// 00580
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00584
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x00000000,                                                  // zero                                        /// 00590
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00598
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0059c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005c8
			 0x7fc00001,                                                  // signaling NaN                               /// 005cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 005fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00604
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00608
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0060c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0061c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00620
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00624
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00628
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0062c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00630
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00634
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00644
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00648
			 0x0c700000,                                                  // Leading 1s:                                 /// 0064c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00650
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00654
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00658
			 0x3f028f5c,                                                  // 0.51                                        /// 0065c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00660
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0066c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x55555555,                                                  // 4 random values                             /// 00678
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0067c
			 0x80000000,                                                  // -zero                                       /// 00680
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00684
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00688
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00690
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00698
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0069c
			 0x0e000007,                                                  // Trailing 1s:                                /// 006a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006f0
			 0x80000000,                                                  // -zero                                       /// 006f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00704
			 0x0c400000,                                                  // Leading 1s:                                 /// 00708
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0070c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0071c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00724
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0072c
			 0xbf028f5c,                                                  // -0.51                                       /// 00730
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0073c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00740
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00744
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00748
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00750
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00754
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00758
			 0x0e000001,                                                  // Trailing 1s:                                /// 0075c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00764
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0076c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00774
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00778
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00784
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0078c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00790
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0c700000,                                                  // Leading 1s:                                 /// 00798
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x7fc00001,                                                  // signaling NaN                               /// 007bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00800
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0080c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00810
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00814
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00818
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0081c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00820
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00824
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00828
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00830
			 0x0c700000,                                                  // Leading 1s:                                 /// 00834
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0083c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00840
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00844
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00848
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0084c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00858
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0085c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00860
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00868
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0086c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00870
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00874
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00878
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0087c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00880
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00884
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00888
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00898
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0089c
			 0x00000000,                                                  // zero                                        /// 008a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 008f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00900
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00908
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0090c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00910
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00918
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0091c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00924
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x80000000,                                                  // -zero                                       /// 0092c
			 0x00000000,                                                  // zero                                        /// 00930
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0c600000,                                                  // Leading 1s:                                 /// 00938
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00940
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00948
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0094c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00954
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00958
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00960
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00964
			 0x0e000007,                                                  // Trailing 1s:                                /// 00968
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0096c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0097c
			 0x3f028f5c,                                                  // 0.51                                        /// 00980
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00988
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00990
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00998
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 009e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a04
			 0x55555555,                                                  // 4 random values                             /// 00a08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a4c
			 0x80000000,                                                  // -zero                                       /// 00a50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a88
			 0x80000000,                                                  // -zero                                       /// 00a8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a90
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aa0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80000000,                                                  // -zero                                       /// 00aa8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00abc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ac0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ac8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00acc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ad0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ad4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00adc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ae4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00af4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00af8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00afc
			 0xffc00001,                                                  // -signaling NaN                              /// 00b00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b08
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x00000000,                                                  // zero                                        /// 00b14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b18
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b1c
			 0xff800000,                                                  // -inf                                        /// 00b20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b24
			 0xff800000,                                                  // -inf                                        /// 00b28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b30
			 0x80000000,                                                  // -zero                                       /// 00b34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b6c
			 0x33333333,                                                  // 4 random values                             /// 00b70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ba0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ba8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bb0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bc0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bc4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bcc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bd4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bd8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0xcb000000,                                                  // -8388608.0                                  /// 00bf0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bf4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bf8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c20
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c24
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c40
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c64
			 0x55555555,                                                  // 4 random values                             /// 00c68
			 0x00000000,                                                  // zero                                        /// 00c6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c78
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ca0
			 0xff800000,                                                  // -inf                                        /// 00ca4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cb0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cb8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cc4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ccc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cd0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ce0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d10
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d34
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d48
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d54
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d58
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d68
			 0x80000000,                                                  // -zero                                       /// 00d6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00da0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00db0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dc8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dcc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dd4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ddc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00de0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00de4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00de8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00df0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00df4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00df8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0xbf028f5c,                                                  // -0.51                                       /// 00e0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e1c
			 0x80000000,                                                  // -zero                                       /// 00e20
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e30
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e48
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e50
			 0xcb000000,                                                  // -8388608.0                                  /// 00e54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x7f800000,                                                  // inf                                         /// 00e7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e84
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ea0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ea8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ebc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ec4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ed0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ee0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ee8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eec
			 0x55555555,                                                  // 4 random values                             /// 00ef0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ef4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0xff800000,                                                  // -inf                                        /// 00efc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f48
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f80
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x4b000000,                                                  // 8388608.0                                   /// 00f88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f90
			 0x4b000000,                                                  // 8388608.0                                   /// 00f94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fa0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fa8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fb8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fc0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fc4
			 0x55555555,                                                  // 4 random values                             /// 00fc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fcc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fe4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fe8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ff4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ff8
			 0x0c7ffffe                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x214e2393, /// 0x0
			 0x1002de8b, /// 0x4
			 0x38c4ea4c, /// 0x8
			 0x6e893a7c, /// 0xc
			 0x59865868, /// 0x10
			 0x0177657c, /// 0x14
			 0xb53a5148, /// 0x18
			 0x01c80092, /// 0x1c
			 0x7f4ea18c, /// 0x20
			 0x4661fa68, /// 0x24
			 0x7cc9a424, /// 0x28
			 0x39d61c86, /// 0x2c
			 0x787976bd, /// 0x30
			 0x524d7854, /// 0x34
			 0xb41f5c44, /// 0x38
			 0x512b18b8, /// 0x3c
			 0x64e13bf7, /// 0x40
			 0x614dd977, /// 0x44
			 0x93e4a98b, /// 0x48
			 0xdadfb15a, /// 0x4c
			 0xbbfe3981, /// 0x50
			 0xb15d2c7b, /// 0x54
			 0x18f5c17b, /// 0x58
			 0xfe4c1bf0, /// 0x5c
			 0x37be4068, /// 0x60
			 0x399a67a8, /// 0x64
			 0x788a308f, /// 0x68
			 0x84d002ff, /// 0x6c
			 0x1f505694, /// 0x70
			 0x3f2f827e, /// 0x74
			 0xcf517eb2, /// 0x78
			 0x694e89bb, /// 0x7c
			 0x38523cf3, /// 0x80
			 0x8b6daa56, /// 0x84
			 0x0b13ef2f, /// 0x88
			 0x8c655381, /// 0x8c
			 0x8684a0a6, /// 0x90
			 0xf52aeabe, /// 0x94
			 0xb95c15ae, /// 0x98
			 0xd8046c3b, /// 0x9c
			 0x23ab43e2, /// 0xa0
			 0x4fa0db72, /// 0xa4
			 0x88db2530, /// 0xa8
			 0x903ff23e, /// 0xac
			 0x8bfe3ebc, /// 0xb0
			 0x80323bba, /// 0xb4
			 0x1253237c, /// 0xb8
			 0x3243f7c0, /// 0xbc
			 0xf15a51e1, /// 0xc0
			 0x21366bfc, /// 0xc4
			 0xf3d3e256, /// 0xc8
			 0x408b4d38, /// 0xcc
			 0x89c9be59, /// 0xd0
			 0x91db5fa7, /// 0xd4
			 0x1af6e81c, /// 0xd8
			 0xf59632e1, /// 0xdc
			 0x593a2f9a, /// 0xe0
			 0x06be3ca9, /// 0xe4
			 0x977461dc, /// 0xe8
			 0xe5d209c6, /// 0xec
			 0x97a26eb4, /// 0xf0
			 0x8c2ec5bb, /// 0xf4
			 0xaacf9a2a, /// 0xf8
			 0xe4185268, /// 0xfc
			 0x7486bded, /// 0x100
			 0x3785ef93, /// 0x104
			 0xf87d1e76, /// 0x108
			 0x409bb77e, /// 0x10c
			 0xa3849743, /// 0x110
			 0x83204e37, /// 0x114
			 0xf989247f, /// 0x118
			 0x20cce318, /// 0x11c
			 0xa53aae29, /// 0x120
			 0xf88b7968, /// 0x124
			 0x6af147ef, /// 0x128
			 0xc1f8ad17, /// 0x12c
			 0x54d81a7b, /// 0x130
			 0x390f5170, /// 0x134
			 0xb9942168, /// 0x138
			 0x7a9b2335, /// 0x13c
			 0x40b76ba6, /// 0x140
			 0x6dad6dcb, /// 0x144
			 0x2f0686a5, /// 0x148
			 0xba7448a1, /// 0x14c
			 0x168d0ec3, /// 0x150
			 0x9865d352, /// 0x154
			 0x2aa4a87a, /// 0x158
			 0x4844b6cc, /// 0x15c
			 0xd9122431, /// 0x160
			 0x1ba0b3ae, /// 0x164
			 0x7e6db5be, /// 0x168
			 0x9dd3a8c4, /// 0x16c
			 0xd5968a78, /// 0x170
			 0xd935a639, /// 0x174
			 0x42e18840, /// 0x178
			 0x789bbcdb, /// 0x17c
			 0x85712c00, /// 0x180
			 0x59e83dca, /// 0x184
			 0x254b952b, /// 0x188
			 0x64266cd1, /// 0x18c
			 0x820673e1, /// 0x190
			 0x4e52a18b, /// 0x194
			 0x9e37f0c3, /// 0x198
			 0xa2e4a3b8, /// 0x19c
			 0x48da897e, /// 0x1a0
			 0x24cbdfe8, /// 0x1a4
			 0x9fa8cb89, /// 0x1a8
			 0x130ae156, /// 0x1ac
			 0xf9cc380b, /// 0x1b0
			 0x136dac4b, /// 0x1b4
			 0x0ac0a64c, /// 0x1b8
			 0x42c91eb9, /// 0x1bc
			 0x4ef60dd1, /// 0x1c0
			 0x83f32c6d, /// 0x1c4
			 0x88076b2f, /// 0x1c8
			 0x13c03aba, /// 0x1cc
			 0xad0bae56, /// 0x1d0
			 0xb53eeceb, /// 0x1d4
			 0xebbc55ab, /// 0x1d8
			 0x12eaf81b, /// 0x1dc
			 0x875cede8, /// 0x1e0
			 0xb586bfc0, /// 0x1e4
			 0x1203412a, /// 0x1e8
			 0x42b2d477, /// 0x1ec
			 0xbc250a40, /// 0x1f0
			 0xc8714558, /// 0x1f4
			 0xe00d503e, /// 0x1f8
			 0x32a729ae, /// 0x1fc
			 0x877be716, /// 0x200
			 0x910bcc22, /// 0x204
			 0x4c99fb49, /// 0x208
			 0xa5d0af28, /// 0x20c
			 0xbcf3aac8, /// 0x210
			 0x6d60f183, /// 0x214
			 0xfac92815, /// 0x218
			 0x9c1e3ec4, /// 0x21c
			 0x713771c7, /// 0x220
			 0x9a48ceb3, /// 0x224
			 0x0fceb64f, /// 0x228
			 0x75d5b690, /// 0x22c
			 0xc6b344b5, /// 0x230
			 0x6057a77e, /// 0x234
			 0x41c22a5a, /// 0x238
			 0x8f81a691, /// 0x23c
			 0xf206ea07, /// 0x240
			 0x41f81149, /// 0x244
			 0xde3b1f86, /// 0x248
			 0xe386ca8c, /// 0x24c
			 0x32eba1c2, /// 0x250
			 0x34b662dd, /// 0x254
			 0x480710c6, /// 0x258
			 0x55c2d6c5, /// 0x25c
			 0xdc8a380a, /// 0x260
			 0xc92d9999, /// 0x264
			 0x846e9419, /// 0x268
			 0x66c170cc, /// 0x26c
			 0xdef4bf50, /// 0x270
			 0x1779e1ad, /// 0x274
			 0x8b4bd8cb, /// 0x278
			 0x1b81b066, /// 0x27c
			 0x95f50f83, /// 0x280
			 0xa21b682c, /// 0x284
			 0xfde4275d, /// 0x288
			 0x73984356, /// 0x28c
			 0xaf5508b2, /// 0x290
			 0x1dfa2fb7, /// 0x294
			 0x25539c5c, /// 0x298
			 0x5045e96b, /// 0x29c
			 0x67817c28, /// 0x2a0
			 0xd04102a3, /// 0x2a4
			 0x5787f552, /// 0x2a8
			 0xc507eb9e, /// 0x2ac
			 0x5cda5ec0, /// 0x2b0
			 0x3b69dc15, /// 0x2b4
			 0x0bef8fac, /// 0x2b8
			 0x951c7fe4, /// 0x2bc
			 0x51861858, /// 0x2c0
			 0xc7ac1b12, /// 0x2c4
			 0xf5739627, /// 0x2c8
			 0xf67214cf, /// 0x2cc
			 0x4c1a1ba2, /// 0x2d0
			 0x77c5a6a6, /// 0x2d4
			 0x9a779aca, /// 0x2d8
			 0x69b02683, /// 0x2dc
			 0xab562ed8, /// 0x2e0
			 0xc860e11c, /// 0x2e4
			 0xfbde8f6f, /// 0x2e8
			 0x3c17e36c, /// 0x2ec
			 0xf4790658, /// 0x2f0
			 0x4d429f03, /// 0x2f4
			 0xbee03641, /// 0x2f8
			 0xf5fcc2dd, /// 0x2fc
			 0x60309634, /// 0x300
			 0x7084d9a4, /// 0x304
			 0xae877678, /// 0x308
			 0x5cd0f432, /// 0x30c
			 0x681afb06, /// 0x310
			 0x5f66bf36, /// 0x314
			 0x59a481a4, /// 0x318
			 0xd6f826e7, /// 0x31c
			 0x1b590f96, /// 0x320
			 0xa4dab49e, /// 0x324
			 0x4588abfe, /// 0x328
			 0x42ebd3ec, /// 0x32c
			 0x304333e5, /// 0x330
			 0xbd24434a, /// 0x334
			 0xd4322cde, /// 0x338
			 0xab131986, /// 0x33c
			 0xa4581bc6, /// 0x340
			 0xcc8ac283, /// 0x344
			 0x0a655fb5, /// 0x348
			 0x27a5ac11, /// 0x34c
			 0x9cfef1d2, /// 0x350
			 0x567d8cb7, /// 0x354
			 0xef8b6d70, /// 0x358
			 0x03f95960, /// 0x35c
			 0xb6b44451, /// 0x360
			 0xb54351ec, /// 0x364
			 0x71f2efe9, /// 0x368
			 0x543f09df, /// 0x36c
			 0x6312c1cc, /// 0x370
			 0x2ac0a140, /// 0x374
			 0xdb71bd9a, /// 0x378
			 0x28bfde0a, /// 0x37c
			 0x6b19e6cb, /// 0x380
			 0xc2646382, /// 0x384
			 0x00768734, /// 0x388
			 0x70cfc1a7, /// 0x38c
			 0x96e9e7c3, /// 0x390
			 0xd7d6fdb4, /// 0x394
			 0x7a9ec7a2, /// 0x398
			 0x1900ef30, /// 0x39c
			 0x4001e46b, /// 0x3a0
			 0x82393831, /// 0x3a4
			 0xfaea7665, /// 0x3a8
			 0x988330ed, /// 0x3ac
			 0xd236c6cc, /// 0x3b0
			 0x9c145cda, /// 0x3b4
			 0x30b7b436, /// 0x3b8
			 0xb3c0f035, /// 0x3bc
			 0x2fd25742, /// 0x3c0
			 0x4fff01d2, /// 0x3c4
			 0x4e32d6cf, /// 0x3c8
			 0xc795708c, /// 0x3cc
			 0xb3ccc5d4, /// 0x3d0
			 0x125a8a3e, /// 0x3d4
			 0x4ee202d5, /// 0x3d8
			 0xd136474c, /// 0x3dc
			 0x4375240c, /// 0x3e0
			 0x28aad4d5, /// 0x3e4
			 0x060bae86, /// 0x3e8
			 0x5ca55402, /// 0x3ec
			 0x72a21124, /// 0x3f0
			 0x535cbf2c, /// 0x3f4
			 0x19b2d75a, /// 0x3f8
			 0x13478291, /// 0x3fc
			 0x3256d775, /// 0x400
			 0x15bc4aef, /// 0x404
			 0x6f111434, /// 0x408
			 0x9f2adc98, /// 0x40c
			 0xc0e6e433, /// 0x410
			 0xe865957b, /// 0x414
			 0x0166b6d6, /// 0x418
			 0x8a180a9f, /// 0x41c
			 0xbc23702e, /// 0x420
			 0xfac9f84b, /// 0x424
			 0xb62c52d9, /// 0x428
			 0x08a1cd1e, /// 0x42c
			 0x2b102621, /// 0x430
			 0x26f83f1f, /// 0x434
			 0x7300a857, /// 0x438
			 0xb5a72e4d, /// 0x43c
			 0xf97cd124, /// 0x440
			 0x6adda707, /// 0x444
			 0xf0a8b172, /// 0x448
			 0x5a8caa72, /// 0x44c
			 0xa23a7755, /// 0x450
			 0x56a23a97, /// 0x454
			 0xfab30192, /// 0x458
			 0x37c44d84, /// 0x45c
			 0x57a68d2c, /// 0x460
			 0x9edff289, /// 0x464
			 0xc7e8426a, /// 0x468
			 0x4188a8da, /// 0x46c
			 0x7e2dba3d, /// 0x470
			 0xa867514c, /// 0x474
			 0x4533848f, /// 0x478
			 0x88a28187, /// 0x47c
			 0x67c88664, /// 0x480
			 0xe57426cc, /// 0x484
			 0x28545a9c, /// 0x488
			 0x72f2d218, /// 0x48c
			 0x54379369, /// 0x490
			 0x9d96c143, /// 0x494
			 0x3262a5de, /// 0x498
			 0xb49eca4c, /// 0x49c
			 0x52fec9f2, /// 0x4a0
			 0x9cbe3626, /// 0x4a4
			 0x9a5ebee1, /// 0x4a8
			 0x2bfa8bfc, /// 0x4ac
			 0xeffce79b, /// 0x4b0
			 0xbb5706c7, /// 0x4b4
			 0xb30501bb, /// 0x4b8
			 0xe7959f39, /// 0x4bc
			 0x2fd9e137, /// 0x4c0
			 0x138ba2b6, /// 0x4c4
			 0xe1ac024a, /// 0x4c8
			 0xa0776fcf, /// 0x4cc
			 0xcadb45ed, /// 0x4d0
			 0x63e8ff5f, /// 0x4d4
			 0xc54cfa4d, /// 0x4d8
			 0x4dbafc4e, /// 0x4dc
			 0x9284d69b, /// 0x4e0
			 0x506dbe5a, /// 0x4e4
			 0x5a08b6cd, /// 0x4e8
			 0xb3523d5c, /// 0x4ec
			 0xb9c58b96, /// 0x4f0
			 0xed3b6f73, /// 0x4f4
			 0x62ab482e, /// 0x4f8
			 0x10b7c535, /// 0x4fc
			 0xb1c0dada, /// 0x500
			 0xbc6306c7, /// 0x504
			 0x9cd6124d, /// 0x508
			 0xcc8f1dbb, /// 0x50c
			 0x096fc0e2, /// 0x510
			 0xfe3ebba8, /// 0x514
			 0x118854e7, /// 0x518
			 0x669ba0a2, /// 0x51c
			 0x1a004a47, /// 0x520
			 0xf5abdfa1, /// 0x524
			 0x89406593, /// 0x528
			 0x281fe585, /// 0x52c
			 0xa1a237d8, /// 0x530
			 0x273f3b17, /// 0x534
			 0x20e277eb, /// 0x538
			 0xcafada42, /// 0x53c
			 0x6e029479, /// 0x540
			 0xbec1a29e, /// 0x544
			 0xcd9c401b, /// 0x548
			 0xda55639d, /// 0x54c
			 0x5b0bb0e4, /// 0x550
			 0x84a5ff0d, /// 0x554
			 0xcc5767e4, /// 0x558
			 0x2ab93b44, /// 0x55c
			 0xe9e47d0e, /// 0x560
			 0xc1781c31, /// 0x564
			 0x9eabbfe8, /// 0x568
			 0xfeb18f38, /// 0x56c
			 0x863e64f0, /// 0x570
			 0x8f1c2174, /// 0x574
			 0x4fd50e8b, /// 0x578
			 0xd9d27dae, /// 0x57c
			 0xe9554357, /// 0x580
			 0xf30cfaba, /// 0x584
			 0xf36671d0, /// 0x588
			 0x486aa484, /// 0x58c
			 0xdf8352b1, /// 0x590
			 0xbbd196b9, /// 0x594
			 0x429827ca, /// 0x598
			 0x0b0a71f8, /// 0x59c
			 0xe1c863b9, /// 0x5a0
			 0xfb8e5e35, /// 0x5a4
			 0xb8b68631, /// 0x5a8
			 0xd74c0134, /// 0x5ac
			 0x536d6eab, /// 0x5b0
			 0xd28c93a8, /// 0x5b4
			 0x6b277601, /// 0x5b8
			 0x25809215, /// 0x5bc
			 0x1af77846, /// 0x5c0
			 0x669c28b4, /// 0x5c4
			 0xdd6666ea, /// 0x5c8
			 0xcea30c11, /// 0x5cc
			 0xaa7be5fd, /// 0x5d0
			 0x16f78234, /// 0x5d4
			 0x4d812934, /// 0x5d8
			 0x2ada1eb2, /// 0x5dc
			 0xbefeb825, /// 0x5e0
			 0x670199eb, /// 0x5e4
			 0xb2358683, /// 0x5e8
			 0x16b5bc7a, /// 0x5ec
			 0xe3610ac4, /// 0x5f0
			 0xbe06143e, /// 0x5f4
			 0x9f6457fa, /// 0x5f8
			 0xd81b7ea1, /// 0x5fc
			 0xb7a263f3, /// 0x600
			 0x561c6f90, /// 0x604
			 0xbd7fc05d, /// 0x608
			 0xe0def8f7, /// 0x60c
			 0x41a56c38, /// 0x610
			 0x776236ed, /// 0x614
			 0x801186d4, /// 0x618
			 0x308779e8, /// 0x61c
			 0x6bda9d2f, /// 0x620
			 0x5825ee33, /// 0x624
			 0xf6dd0782, /// 0x628
			 0xfb6f6f45, /// 0x62c
			 0xc2b41d16, /// 0x630
			 0xe3641654, /// 0x634
			 0x6db3455d, /// 0x638
			 0x7342e2d3, /// 0x63c
			 0xe6f5b503, /// 0x640
			 0x54e7b0d0, /// 0x644
			 0xeb2c425a, /// 0x648
			 0xd21636c9, /// 0x64c
			 0x9ac25b50, /// 0x650
			 0xa52f96c3, /// 0x654
			 0x203a3c6e, /// 0x658
			 0xfb58c22e, /// 0x65c
			 0x5fca56ca, /// 0x660
			 0xf54c47b3, /// 0x664
			 0x950689f4, /// 0x668
			 0xca3efb9d, /// 0x66c
			 0x980aa1e9, /// 0x670
			 0x63c592cd, /// 0x674
			 0xb1ffacdc, /// 0x678
			 0x43cbafde, /// 0x67c
			 0x42418c20, /// 0x680
			 0xda6049a7, /// 0x684
			 0xf97441ce, /// 0x688
			 0x0ade2052, /// 0x68c
			 0x7c56aaad, /// 0x690
			 0x364200ad, /// 0x694
			 0xa7b7397e, /// 0x698
			 0x0700e4d1, /// 0x69c
			 0x2ab1c3a2, /// 0x6a0
			 0x74546795, /// 0x6a4
			 0x7af6bdee, /// 0x6a8
			 0xa7a2623d, /// 0x6ac
			 0xd1b52812, /// 0x6b0
			 0xfd68e800, /// 0x6b4
			 0xf74758b9, /// 0x6b8
			 0xf9872b18, /// 0x6bc
			 0x38b49c9e, /// 0x6c0
			 0x17a765b7, /// 0x6c4
			 0x25a29d7a, /// 0x6c8
			 0xc3a70a74, /// 0x6cc
			 0x5a750460, /// 0x6d0
			 0x01844e17, /// 0x6d4
			 0x011a0bcf, /// 0x6d8
			 0x40e73114, /// 0x6dc
			 0xed4579a7, /// 0x6e0
			 0xc1cf7644, /// 0x6e4
			 0x260c5208, /// 0x6e8
			 0x01ecd180, /// 0x6ec
			 0x90f8e733, /// 0x6f0
			 0xa0ad1795, /// 0x6f4
			 0x0dec83f1, /// 0x6f8
			 0xb6df9919, /// 0x6fc
			 0xa749eb97, /// 0x700
			 0x14d806ae, /// 0x704
			 0xded770ce, /// 0x708
			 0xd707672f, /// 0x70c
			 0x08c9d47c, /// 0x710
			 0xe21e7c38, /// 0x714
			 0x46ecde5b, /// 0x718
			 0xa85d9b5d, /// 0x71c
			 0x76dffed4, /// 0x720
			 0x32c3e02d, /// 0x724
			 0x276e54eb, /// 0x728
			 0x979a1207, /// 0x72c
			 0xb9878651, /// 0x730
			 0x13538bbe, /// 0x734
			 0xe5b03a95, /// 0x738
			 0xbb4b2e67, /// 0x73c
			 0x1169cfcf, /// 0x740
			 0x290f16c9, /// 0x744
			 0x1185139f, /// 0x748
			 0x19c36049, /// 0x74c
			 0x43de1260, /// 0x750
			 0x94361136, /// 0x754
			 0xbdf657cf, /// 0x758
			 0xc63e6931, /// 0x75c
			 0xa5d64b3f, /// 0x760
			 0x7946dff4, /// 0x764
			 0x5f3f2885, /// 0x768
			 0x5b6345f1, /// 0x76c
			 0x986217e2, /// 0x770
			 0x257842eb, /// 0x774
			 0xad9f0750, /// 0x778
			 0xe65df2fc, /// 0x77c
			 0x048071c4, /// 0x780
			 0x304eacac, /// 0x784
			 0xd4c46d71, /// 0x788
			 0xeea5a18d, /// 0x78c
			 0xf018629f, /// 0x790
			 0x2db01988, /// 0x794
			 0xa8d42389, /// 0x798
			 0xb4fa8d54, /// 0x79c
			 0x1ffcbb26, /// 0x7a0
			 0x5db29c07, /// 0x7a4
			 0xe49ed27b, /// 0x7a8
			 0x6590c4ad, /// 0x7ac
			 0x2a85ac1d, /// 0x7b0
			 0xa2cf4875, /// 0x7b4
			 0xf7d65566, /// 0x7b8
			 0xbb5a0479, /// 0x7bc
			 0xd5f9593c, /// 0x7c0
			 0x13b247f4, /// 0x7c4
			 0xb8336e28, /// 0x7c8
			 0x1a41e1d1, /// 0x7cc
			 0xfe4b09da, /// 0x7d0
			 0x98601dae, /// 0x7d4
			 0x9014a3ad, /// 0x7d8
			 0xb64bf3eb, /// 0x7dc
			 0x8ab2addd, /// 0x7e0
			 0x2b5ba4a4, /// 0x7e4
			 0x361260ba, /// 0x7e8
			 0x074897dd, /// 0x7ec
			 0x9c80a16f, /// 0x7f0
			 0x0c18e2e8, /// 0x7f4
			 0xda9af2f4, /// 0x7f8
			 0x2804b7c1, /// 0x7fc
			 0x8638fac1, /// 0x800
			 0xdffad0a5, /// 0x804
			 0xa67dec35, /// 0x808
			 0x4e2666ba, /// 0x80c
			 0xf1694926, /// 0x810
			 0xf0b84882, /// 0x814
			 0x117c33c1, /// 0x818
			 0x01023910, /// 0x81c
			 0xd242c929, /// 0x820
			 0x03278f30, /// 0x824
			 0xb5bdedff, /// 0x828
			 0xea22f0ea, /// 0x82c
			 0x3d1c1818, /// 0x830
			 0x58f2cb8f, /// 0x834
			 0xd19b48f3, /// 0x838
			 0x92a4df0f, /// 0x83c
			 0x931c6efe, /// 0x840
			 0x822262a4, /// 0x844
			 0x1a66b6ba, /// 0x848
			 0xace6598d, /// 0x84c
			 0x48ad1235, /// 0x850
			 0xae95eadd, /// 0x854
			 0xbd1ba8c8, /// 0x858
			 0x714eff20, /// 0x85c
			 0x472af4be, /// 0x860
			 0xfb90b35c, /// 0x864
			 0x19f3ba6a, /// 0x868
			 0xcf18adc6, /// 0x86c
			 0xb5083354, /// 0x870
			 0x60b7950c, /// 0x874
			 0x2cc14826, /// 0x878
			 0x7da8a71c, /// 0x87c
			 0xfc076908, /// 0x880
			 0xc8028156, /// 0x884
			 0x9186ef9f, /// 0x888
			 0xbf15cf77, /// 0x88c
			 0x63c946b4, /// 0x890
			 0x7816b71f, /// 0x894
			 0xad6082bb, /// 0x898
			 0x1bd78b34, /// 0x89c
			 0xf57498c5, /// 0x8a0
			 0x278fe29b, /// 0x8a4
			 0x7417154c, /// 0x8a8
			 0x473fa6fa, /// 0x8ac
			 0x7b7738a4, /// 0x8b0
			 0xc022e6c9, /// 0x8b4
			 0x5cf33ae0, /// 0x8b8
			 0x9a1dced9, /// 0x8bc
			 0xf5a2f018, /// 0x8c0
			 0x5f90f293, /// 0x8c4
			 0xff828355, /// 0x8c8
			 0xca55c42f, /// 0x8cc
			 0x02a75d01, /// 0x8d0
			 0xae080159, /// 0x8d4
			 0xc2c71494, /// 0x8d8
			 0x86058a2e, /// 0x8dc
			 0x710dbe3e, /// 0x8e0
			 0x342535b2, /// 0x8e4
			 0x400a2cfe, /// 0x8e8
			 0x168dc87f, /// 0x8ec
			 0xecbc8ba1, /// 0x8f0
			 0xed099800, /// 0x8f4
			 0x0b503be8, /// 0x8f8
			 0xf12a5b33, /// 0x8fc
			 0x380052d8, /// 0x900
			 0x496d4a74, /// 0x904
			 0x0519b2bc, /// 0x908
			 0x76e7d7d4, /// 0x90c
			 0x53b90859, /// 0x910
			 0x8d948fe7, /// 0x914
			 0xf44162d3, /// 0x918
			 0x22737013, /// 0x91c
			 0xe75089f0, /// 0x920
			 0x85506883, /// 0x924
			 0x88b3fdc5, /// 0x928
			 0xaf36da93, /// 0x92c
			 0xaef12562, /// 0x930
			 0xe284ed8c, /// 0x934
			 0x6b7e1644, /// 0x938
			 0xd61f9ace, /// 0x93c
			 0xadd71a67, /// 0x940
			 0x24a03766, /// 0x944
			 0x0caa0c2f, /// 0x948
			 0x7922d781, /// 0x94c
			 0xefd91e9d, /// 0x950
			 0xaae6cc46, /// 0x954
			 0x2fbd92c0, /// 0x958
			 0xf514b6a5, /// 0x95c
			 0x4e35c331, /// 0x960
			 0x82bb9306, /// 0x964
			 0x97f90281, /// 0x968
			 0xc81f3e05, /// 0x96c
			 0x7b3e9953, /// 0x970
			 0x142b14fe, /// 0x974
			 0xf10bed4e, /// 0x978
			 0xc10caa88, /// 0x97c
			 0x5c48faab, /// 0x980
			 0xa08cfc8e, /// 0x984
			 0x2ed954be, /// 0x988
			 0x514d89fe, /// 0x98c
			 0xf01a371e, /// 0x990
			 0xb4b49712, /// 0x994
			 0xe305fcc8, /// 0x998
			 0x7a141aae, /// 0x99c
			 0x36d4f70d, /// 0x9a0
			 0xb1583d98, /// 0x9a4
			 0xd5a08d13, /// 0x9a8
			 0xe3af44fc, /// 0x9ac
			 0x58eff56f, /// 0x9b0
			 0xd23cf088, /// 0x9b4
			 0x8161aedf, /// 0x9b8
			 0x6f9ede1d, /// 0x9bc
			 0x28ea3f52, /// 0x9c0
			 0xd528387b, /// 0x9c4
			 0x2b571b75, /// 0x9c8
			 0x8ecc69c6, /// 0x9cc
			 0xde99836b, /// 0x9d0
			 0xb9a7d5f2, /// 0x9d4
			 0x8275c901, /// 0x9d8
			 0xffc51cdb, /// 0x9dc
			 0x0afc122a, /// 0x9e0
			 0x0e0a2d09, /// 0x9e4
			 0x1ba6dd86, /// 0x9e8
			 0x5504a8ab, /// 0x9ec
			 0x053c04b5, /// 0x9f0
			 0x5e66a075, /// 0x9f4
			 0x734c2445, /// 0x9f8
			 0x94bf9599, /// 0x9fc
			 0x7fec2630, /// 0xa00
			 0x8830019e, /// 0xa04
			 0x1554bd0f, /// 0xa08
			 0x17766a12, /// 0xa0c
			 0x42b67f16, /// 0xa10
			 0xa29dc3cf, /// 0xa14
			 0xbf881a4b, /// 0xa18
			 0x61aca3bc, /// 0xa1c
			 0xdf941534, /// 0xa20
			 0x3ea7953e, /// 0xa24
			 0x323d2518, /// 0xa28
			 0x930c5ec2, /// 0xa2c
			 0x293d0ae1, /// 0xa30
			 0x919cb19f, /// 0xa34
			 0x66205c8b, /// 0xa38
			 0x7ade41a7, /// 0xa3c
			 0xaae570b4, /// 0xa40
			 0x1069018a, /// 0xa44
			 0xc5d240d1, /// 0xa48
			 0x412db1bb, /// 0xa4c
			 0x0b8303aa, /// 0xa50
			 0xb5a31619, /// 0xa54
			 0xb720b4fb, /// 0xa58
			 0x80c4a2d3, /// 0xa5c
			 0x90da9294, /// 0xa60
			 0xbb574d16, /// 0xa64
			 0x23407575, /// 0xa68
			 0x3a3c9ee1, /// 0xa6c
			 0x49c32647, /// 0xa70
			 0x41070ac7, /// 0xa74
			 0x2328f1a9, /// 0xa78
			 0xd1e89200, /// 0xa7c
			 0x7142616f, /// 0xa80
			 0x91509fab, /// 0xa84
			 0xb5f5da2d, /// 0xa88
			 0x183e14d6, /// 0xa8c
			 0x15af531a, /// 0xa90
			 0x6dd66cae, /// 0xa94
			 0x39351b5a, /// 0xa98
			 0xdd2a7e43, /// 0xa9c
			 0xa620653f, /// 0xaa0
			 0x3f4f66f6, /// 0xaa4
			 0x8687d375, /// 0xaa8
			 0x3628fc0e, /// 0xaac
			 0xce021998, /// 0xab0
			 0x5b5a6922, /// 0xab4
			 0xad92e1ef, /// 0xab8
			 0xd6ea4294, /// 0xabc
			 0x137a73b7, /// 0xac0
			 0x6b9521d1, /// 0xac4
			 0x27ad1679, /// 0xac8
			 0xbcab1390, /// 0xacc
			 0x9656178d, /// 0xad0
			 0xe90f7475, /// 0xad4
			 0x31943daa, /// 0xad8
			 0xb62fefad, /// 0xadc
			 0x54663493, /// 0xae0
			 0x47f72482, /// 0xae4
			 0x956647c3, /// 0xae8
			 0x951449c1, /// 0xaec
			 0xfbac82a0, /// 0xaf0
			 0x73b4362a, /// 0xaf4
			 0xef2f07b9, /// 0xaf8
			 0xfafe0f07, /// 0xafc
			 0x8cf461d8, /// 0xb00
			 0xab38ea86, /// 0xb04
			 0x787ce1b7, /// 0xb08
			 0xbefa973a, /// 0xb0c
			 0x7361592b, /// 0xb10
			 0x197acd5f, /// 0xb14
			 0x23af679b, /// 0xb18
			 0xab3b16ad, /// 0xb1c
			 0xcf256b6a, /// 0xb20
			 0xa7378d2b, /// 0xb24
			 0xcaad336b, /// 0xb28
			 0xf613677f, /// 0xb2c
			 0xec3173e5, /// 0xb30
			 0x85d5e223, /// 0xb34
			 0x01ed2aaf, /// 0xb38
			 0x6ff07eb9, /// 0xb3c
			 0x88bda92c, /// 0xb40
			 0xb1b460a8, /// 0xb44
			 0x94eec8aa, /// 0xb48
			 0xb3bea237, /// 0xb4c
			 0xcd80ca8e, /// 0xb50
			 0xb5d4744b, /// 0xb54
			 0xe5b0cc4b, /// 0xb58
			 0x589fcfbc, /// 0xb5c
			 0xf7831c4f, /// 0xb60
			 0x72e923cc, /// 0xb64
			 0x5115ddab, /// 0xb68
			 0xc103a1af, /// 0xb6c
			 0x84e69cb4, /// 0xb70
			 0x7d20e991, /// 0xb74
			 0xd0f74248, /// 0xb78
			 0x26a53475, /// 0xb7c
			 0xb3262c89, /// 0xb80
			 0x014ec941, /// 0xb84
			 0xe595bde1, /// 0xb88
			 0x6e6b14e1, /// 0xb8c
			 0x53553495, /// 0xb90
			 0xda278b51, /// 0xb94
			 0x9923c1e9, /// 0xb98
			 0xd77bb553, /// 0xb9c
			 0xc9b8da4f, /// 0xba0
			 0xecdd698e, /// 0xba4
			 0x9d29123f, /// 0xba8
			 0x1a38d367, /// 0xbac
			 0x3ec477d4, /// 0xbb0
			 0x6cb41bf1, /// 0xbb4
			 0x2cdab107, /// 0xbb8
			 0xc51b26ba, /// 0xbbc
			 0x9f951104, /// 0xbc0
			 0x65d99541, /// 0xbc4
			 0xac359354, /// 0xbc8
			 0xab1f6677, /// 0xbcc
			 0x1de774d3, /// 0xbd0
			 0x912a592f, /// 0xbd4
			 0x7e0a9527, /// 0xbd8
			 0x1d926366, /// 0xbdc
			 0x99813fd3, /// 0xbe0
			 0xeb210cd6, /// 0xbe4
			 0x6239e8e7, /// 0xbe8
			 0xd39adc4f, /// 0xbec
			 0x408fbe3e, /// 0xbf0
			 0x5f1fec04, /// 0xbf4
			 0x3457482d, /// 0xbf8
			 0x3a071826, /// 0xbfc
			 0xddaa451f, /// 0xc00
			 0x6bae39b6, /// 0xc04
			 0x103df9c0, /// 0xc08
			 0x517cbd57, /// 0xc0c
			 0x7655e8e5, /// 0xc10
			 0x67a58ce3, /// 0xc14
			 0x29798f8c, /// 0xc18
			 0x7f347c48, /// 0xc1c
			 0xb9a95c89, /// 0xc20
			 0xe5d6c82d, /// 0xc24
			 0xa547e60c, /// 0xc28
			 0xe3766faa, /// 0xc2c
			 0x282f9dd8, /// 0xc30
			 0x8e877e98, /// 0xc34
			 0xcf534a97, /// 0xc38
			 0xe6d635f9, /// 0xc3c
			 0x2eb14f8f, /// 0xc40
			 0x929b3eba, /// 0xc44
			 0x2b5365f3, /// 0xc48
			 0xcce27eca, /// 0xc4c
			 0x484a72df, /// 0xc50
			 0x8fee833e, /// 0xc54
			 0x2d696963, /// 0xc58
			 0x1bb8a7db, /// 0xc5c
			 0x11695acd, /// 0xc60
			 0xad05cf3d, /// 0xc64
			 0x42212c10, /// 0xc68
			 0x455558b6, /// 0xc6c
			 0xe9a935b2, /// 0xc70
			 0x630dd547, /// 0xc74
			 0x873c4bdb, /// 0xc78
			 0xb5f18c42, /// 0xc7c
			 0x4760d994, /// 0xc80
			 0x32a8e460, /// 0xc84
			 0x39e61a5a, /// 0xc88
			 0xf16b4ad7, /// 0xc8c
			 0x12ce225b, /// 0xc90
			 0x52f0cf99, /// 0xc94
			 0x31548dfc, /// 0xc98
			 0xd883a638, /// 0xc9c
			 0x71b4b6bb, /// 0xca0
			 0x28517927, /// 0xca4
			 0x27134c76, /// 0xca8
			 0x32ef11e3, /// 0xcac
			 0x5869af94, /// 0xcb0
			 0x15955a2e, /// 0xcb4
			 0xf3dfc49b, /// 0xcb8
			 0x92bf25a2, /// 0xcbc
			 0xbeada033, /// 0xcc0
			 0x1a6fd79d, /// 0xcc4
			 0x1a4a66df, /// 0xcc8
			 0xba28aeaa, /// 0xccc
			 0x98a25cb3, /// 0xcd0
			 0x2424f1c7, /// 0xcd4
			 0x88a98d3f, /// 0xcd8
			 0x7174be2f, /// 0xcdc
			 0x933f98ce, /// 0xce0
			 0x0506d711, /// 0xce4
			 0x4f71c26d, /// 0xce8
			 0xe0c7eee0, /// 0xcec
			 0x2f8d5086, /// 0xcf0
			 0x8d156b22, /// 0xcf4
			 0x5b680873, /// 0xcf8
			 0xdba459fa, /// 0xcfc
			 0x2ca3aff6, /// 0xd00
			 0xd512aba3, /// 0xd04
			 0x063ada66, /// 0xd08
			 0xb81bfdaf, /// 0xd0c
			 0xf10b87f0, /// 0xd10
			 0xaa92def3, /// 0xd14
			 0x8e1865f2, /// 0xd18
			 0x934ece95, /// 0xd1c
			 0x3c9a9750, /// 0xd20
			 0x54edc8fa, /// 0xd24
			 0x5d0895f4, /// 0xd28
			 0x2887356c, /// 0xd2c
			 0xa6f2e506, /// 0xd30
			 0xace4d1ef, /// 0xd34
			 0x8032d0cf, /// 0xd38
			 0x77c5f541, /// 0xd3c
			 0xd7ebdf6a, /// 0xd40
			 0xc415b1ff, /// 0xd44
			 0x3896c37a, /// 0xd48
			 0xe22f12a6, /// 0xd4c
			 0xb9c1d20e, /// 0xd50
			 0xbc269704, /// 0xd54
			 0x540985e3, /// 0xd58
			 0x33341c9e, /// 0xd5c
			 0xf4214764, /// 0xd60
			 0xa804546b, /// 0xd64
			 0xb9007a59, /// 0xd68
			 0x7dc7b2a0, /// 0xd6c
			 0x8462585d, /// 0xd70
			 0xba521076, /// 0xd74
			 0x5fd36911, /// 0xd78
			 0xca8df5ac, /// 0xd7c
			 0x966c1e64, /// 0xd80
			 0x38b90f3b, /// 0xd84
			 0x7ffe4c08, /// 0xd88
			 0xa981a691, /// 0xd8c
			 0x28b5bd24, /// 0xd90
			 0x931fe636, /// 0xd94
			 0xadc62a2a, /// 0xd98
			 0x6fbea6ad, /// 0xd9c
			 0xa781bea9, /// 0xda0
			 0x2b2aac1a, /// 0xda4
			 0x50f55600, /// 0xda8
			 0x6b4fe41c, /// 0xdac
			 0x6841b1d3, /// 0xdb0
			 0xa55babc4, /// 0xdb4
			 0xb403a08d, /// 0xdb8
			 0xe3dbb317, /// 0xdbc
			 0xf0bb17f9, /// 0xdc0
			 0xc88694de, /// 0xdc4
			 0x6c939f86, /// 0xdc8
			 0xf68ae30b, /// 0xdcc
			 0x4c629cfe, /// 0xdd0
			 0xa839208a, /// 0xdd4
			 0x0ca4bfe3, /// 0xdd8
			 0x9f5cea77, /// 0xddc
			 0x9bdeecc6, /// 0xde0
			 0xa3395a64, /// 0xde4
			 0xdc0a1e9c, /// 0xde8
			 0x7c79818f, /// 0xdec
			 0xb70b3443, /// 0xdf0
			 0xd1066093, /// 0xdf4
			 0xe503c606, /// 0xdf8
			 0x34f2c31a, /// 0xdfc
			 0xae45a530, /// 0xe00
			 0x1b3f7f7a, /// 0xe04
			 0x15945f31, /// 0xe08
			 0xb6b4488e, /// 0xe0c
			 0x5566bb62, /// 0xe10
			 0x07cf672a, /// 0xe14
			 0xef1c5caf, /// 0xe18
			 0x2b1e1129, /// 0xe1c
			 0x74dd4963, /// 0xe20
			 0xb6e5bf86, /// 0xe24
			 0xf1834fef, /// 0xe28
			 0x6e30da86, /// 0xe2c
			 0x3f64b01a, /// 0xe30
			 0xa221b6d3, /// 0xe34
			 0xe6c896ec, /// 0xe38
			 0xd9589902, /// 0xe3c
			 0xc2fb1c5d, /// 0xe40
			 0x7266aca5, /// 0xe44
			 0x1181d046, /// 0xe48
			 0x4cef4fd0, /// 0xe4c
			 0x4b97ffa1, /// 0xe50
			 0xad6f2c5b, /// 0xe54
			 0xc362494f, /// 0xe58
			 0xe815cccf, /// 0xe5c
			 0x1372d146, /// 0xe60
			 0x6f4330cf, /// 0xe64
			 0x9dc3127f, /// 0xe68
			 0x70475669, /// 0xe6c
			 0xf67c2fe3, /// 0xe70
			 0x9269cf67, /// 0xe74
			 0xc0b25274, /// 0xe78
			 0xb09f5abc, /// 0xe7c
			 0xdba87c24, /// 0xe80
			 0x3a09f146, /// 0xe84
			 0x7f40f016, /// 0xe88
			 0x07287c40, /// 0xe8c
			 0xbb3f8de0, /// 0xe90
			 0x3df1336f, /// 0xe94
			 0x92823ec8, /// 0xe98
			 0x39157156, /// 0xe9c
			 0x08f00dce, /// 0xea0
			 0x2163c5c4, /// 0xea4
			 0x32de4ab9, /// 0xea8
			 0x6653b8c3, /// 0xeac
			 0x5b67903a, /// 0xeb0
			 0x7736dce1, /// 0xeb4
			 0x27b518eb, /// 0xeb8
			 0x5fae34c3, /// 0xebc
			 0x4a27235c, /// 0xec0
			 0x8fb28664, /// 0xec4
			 0x2ea08cf9, /// 0xec8
			 0xf337e651, /// 0xecc
			 0x841e237d, /// 0xed0
			 0x5e14e691, /// 0xed4
			 0x6d9070a4, /// 0xed8
			 0xcf3928f4, /// 0xedc
			 0x519d7967, /// 0xee0
			 0x421c11a4, /// 0xee4
			 0xf7c0e222, /// 0xee8
			 0xa032b3b1, /// 0xeec
			 0x88f7461d, /// 0xef0
			 0xd260dda7, /// 0xef4
			 0x2b139c9a, /// 0xef8
			 0xfccf1d48, /// 0xefc
			 0x9c3b6a61, /// 0xf00
			 0x033cfbf0, /// 0xf04
			 0xe3a30957, /// 0xf08
			 0x8c532c94, /// 0xf0c
			 0x37dcf61e, /// 0xf10
			 0x88fe0aa7, /// 0xf14
			 0xb26c90cc, /// 0xf18
			 0x92ccaf2c, /// 0xf1c
			 0x5fc5ebbb, /// 0xf20
			 0x88bfe53f, /// 0xf24
			 0xf6a42ae4, /// 0xf28
			 0x613b880b, /// 0xf2c
			 0x8d88c374, /// 0xf30
			 0x96c0f369, /// 0xf34
			 0x321f60b8, /// 0xf38
			 0x58a08f9e, /// 0xf3c
			 0x2a522e3f, /// 0xf40
			 0xef1e0110, /// 0xf44
			 0x2774973a, /// 0xf48
			 0x00e08282, /// 0xf4c
			 0x1e791e26, /// 0xf50
			 0xc882b3cd, /// 0xf54
			 0x28e5ef98, /// 0xf58
			 0xa0422d40, /// 0xf5c
			 0x18d7d201, /// 0xf60
			 0xb40db544, /// 0xf64
			 0x57af9006, /// 0xf68
			 0x3b797881, /// 0xf6c
			 0xd7fea472, /// 0xf70
			 0x6b8c246b, /// 0xf74
			 0x5a5be4ab, /// 0xf78
			 0x0db2376d, /// 0xf7c
			 0x5b3de1e6, /// 0xf80
			 0xde0dc3e7, /// 0xf84
			 0x970c5ea3, /// 0xf88
			 0x6e9c6bb4, /// 0xf8c
			 0x030861b4, /// 0xf90
			 0xd31f6795, /// 0xf94
			 0x302f1ba8, /// 0xf98
			 0x44d8cc20, /// 0xf9c
			 0xa65917e0, /// 0xfa0
			 0xa3166153, /// 0xfa4
			 0x0475da42, /// 0xfa8
			 0x2ec317b2, /// 0xfac
			 0x796f3f85, /// 0xfb0
			 0x1b58f899, /// 0xfb4
			 0x0ec766eb, /// 0xfb8
			 0x345ec1f6, /// 0xfbc
			 0xfca581e2, /// 0xfc0
			 0xb5fc98b4, /// 0xfc4
			 0xb8032cf0, /// 0xfc8
			 0xdf15b18f, /// 0xfcc
			 0xead92696, /// 0xfd0
			 0xd547ab15, /// 0xfd4
			 0x2a46030b, /// 0xfd8
			 0x3ff5a717, /// 0xfdc
			 0x926fd685, /// 0xfe0
			 0x7c235e06, /// 0xfe4
			 0x761a44e9, /// 0xfe8
			 0x96b4fe9d, /// 0xfec
			 0x95467f5e, /// 0xff0
			 0x1bd9b8a4, /// 0xff4
			 0x012d0a2a, /// 0xff8
			 0x4cfd07ab /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00000
			 0x0c600000,                                                  // Leading 1s:                                 /// 00004
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xffc00001,                                                  // -signaling NaN                              /// 00010
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00014
			 0xbf028f5c,                                                  // -0.51                                       /// 00018
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x7fc00001,                                                  // signaling NaN                               /// 00024
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0xff800000,                                                  // -inf                                        /// 0002c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00030
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00034
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0003c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00040
			 0x0c600000,                                                  // Leading 1s:                                 /// 00044
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00048
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00050
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00054
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00058
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0005c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00060
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00064
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00068
			 0x80000000,                                                  // -zero                                       /// 0006c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00070
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00074
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00078
			 0x0e000007,                                                  // Trailing 1s:                                /// 0007c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0008c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00090
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00098
			 0xffc00001,                                                  // -signaling NaN                              /// 0009c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000cc
			 0x4b000000,                                                  // 8388608.0                                   /// 000d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 000e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00100
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00104
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0010c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00110
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00114
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00118
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00120
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00124
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0012c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00130
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00134
			 0x0c400000,                                                  // Leading 1s:                                 /// 00138
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0013c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00140
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00144
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00148
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0014c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00154
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00160
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00164
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00168
			 0x0c780000,                                                  // Leading 1s:                                 /// 0016c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00170
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00174
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00178
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0017c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00184
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00188
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00190
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00194
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00198
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001d0
			 0xcb000000,                                                  // -8388608.0                                  /// 001d4
			 0x4b000000,                                                  // 8388608.0                                   /// 001d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 001f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00200
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00204
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x80000000,                                                  // -zero                                       /// 00214
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0021c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00224
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00228
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0022c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00230
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00238
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0023c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00240
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00244
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00250
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00254
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00258
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0025c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00264
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00268
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0026c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00270
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00274
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00278
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0027c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00280
			 0x7f800000,                                                  // inf                                         /// 00284
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00288
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00294
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00298
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0029c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x4b000000,                                                  // 8388608.0                                   /// 002f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00300
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00308
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00310
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00314
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0031c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00324
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00328
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0032c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00330
			 0x00000000,                                                  // zero                                        /// 00334
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00338
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00340
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00344
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0034c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00354
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00358
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00364
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00368
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0037c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00380
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00384
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0038c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00390
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00394
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0039c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 003f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x3f028f5c,                                                  // 0.51                                        /// 003f8
			 0xbf028f5c,                                                  // -0.51                                       /// 003fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00400
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00404
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00408
			 0x80000000,                                                  // -zero                                       /// 0040c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00418
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0041c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00424
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00428
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00430
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00434
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00438
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x4b000000,                                                  // 8388608.0                                   /// 00440
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x55555555,                                                  // 4 random values                             /// 00450
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00454
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x3f028f5c,                                                  // 0.51                                        /// 0045c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00460
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00464
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00468
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00474
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0047c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00480
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00484
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00488
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00490
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00498
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0049c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0xff800000,                                                  // -inf                                        /// 004f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00500
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00504
			 0x33333333,                                                  // 4 random values                             /// 00508
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0050c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00510
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0051c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00520
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00524
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00528
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0052c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x00011111,                                                  // 9.7958E-41                                  /// 0053c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00540
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00544
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0054c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00550
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00554
			 0x0e000007,                                                  // Trailing 1s:                                /// 00558
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0055c
			 0x7f800000,                                                  // inf                                         /// 00560
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00568
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00570
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00574
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0057c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00580
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00588
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00594
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00598
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005b0
			 0xbf028f5c,                                                  // -0.51                                       /// 005b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x33333333,                                                  // 4 random values                             /// 005c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005dc
			 0x4b000000,                                                  // 8388608.0                                   /// 005e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x3f028f5c,                                                  // 0.51                                        /// 005ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00600
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00604
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00610
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0061c
			 0xcb000000,                                                  // -8388608.0                                  /// 00620
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00624
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00628
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0062c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00630
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00634
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00644
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0064c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00654
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00658
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0065c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00660
			 0x00011111,                                                  // 9.7958E-41                                  /// 00664
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0066c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00674
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00680
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0068c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x3f028f5c,                                                  // 0.51                                        /// 00694
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00698
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0069c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 006b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006e4
			 0x4b000000,                                                  // 8388608.0                                   /// 006e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00700
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x7fc00001,                                                  // signaling NaN                               /// 00708
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0070c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00710
			 0xcb000000,                                                  // -8388608.0                                  /// 00714
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00718
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0071c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0072c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00734
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00738
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0073c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00740
			 0x7fc00001,                                                  // signaling NaN                               /// 00744
			 0x80011111,                                                  // -9.7958E-41                                 /// 00748
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0074c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00750
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00754
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0075c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00760
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00764
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0076c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00770
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00778
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00780
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00784
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00790
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00798
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0079c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007a4
			 0x80000000,                                                  // -zero                                       /// 007a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007ac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00800
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00804
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xffc00001,                                                  // -signaling NaN                              /// 0080c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00810
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00814
			 0xff800000,                                                  // -inf                                        /// 00818
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00820
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0xcb000000,                                                  // -8388608.0                                  /// 00828
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0082c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00830
			 0xff800000,                                                  // -inf                                        /// 00834
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00838
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0083c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00840
			 0xffc00001,                                                  // -signaling NaN                              /// 00844
			 0xbf028f5c,                                                  // -0.51                                       /// 00848
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00858
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00864
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0086c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00870
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00874
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00878
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0087c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00888
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0088c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00890
			 0x00000000,                                                  // zero                                        /// 00894
			 0xff800000,                                                  // -inf                                        /// 00898
			 0xcb000000,                                                  // -8388608.0                                  /// 0089c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 008a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x7f800000,                                                  // inf                                         /// 008e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x00000000,                                                  // zero                                        /// 008f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00900
			 0x0e000007,                                                  // Trailing 1s:                                /// 00904
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00908
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00910
			 0x0e000003,                                                  // Trailing 1s:                                /// 00914
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0091c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00920
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00924
			 0x7f800000,                                                  // inf                                         /// 00928
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x4b000000,                                                  // 8388608.0                                   /// 00930
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00938
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00940
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00944
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00948
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0094c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00954
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00958
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0095c
			 0x00000000,                                                  // zero                                        /// 00960
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x0e000007,                                                  // Trailing 1s:                                /// 0096c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00978
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00988
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00990
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00000000,                                                  // zero                                        /// 009b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 009bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 009c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 009e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 009f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00000000,                                                  // zero                                        /// 009f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a08
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a0c
			 0x80000000,                                                  // -zero                                       /// 00a10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a88
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00aa0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0xffc00001,                                                  // -signaling NaN                              /// 00aac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ab0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ab4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00abc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ac8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ad0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ad8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00adc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ae8
			 0xff800000,                                                  // -inf                                        /// 00aec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00af0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00af8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00afc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b18
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b28
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b7c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b90
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ba0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ba8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bc8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bf4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bf8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0xbf028f5c,                                                  // -0.51                                       /// 00c04
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c58
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c60
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x3f028f5c,                                                  // 0.51                                        /// 00c7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c90
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c94
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ca8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cbc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cc0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cc4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cc8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cd4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cd8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cdc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ce4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x80000000,                                                  // -zero                                       /// 00cec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cf0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cf8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80000000,                                                  // -zero                                       /// 00d00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0xff800000,                                                  // -inf                                        /// 00d18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d74
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d8c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00da0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00da4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00000000,                                                  // zero                                        /// 00db4
			 0x80000000,                                                  // -zero                                       /// 00db8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dcc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00de8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00df0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dfc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e0c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0xffc00001,                                                  // -signaling NaN                              /// 00e24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xbf028f5c,                                                  // -0.51                                       /// 00e98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e9c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eb0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eb8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ebc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ec0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ec8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ecc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ed0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ed4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ed8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ee8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00eec
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ef0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ef4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ef8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x7f800000,                                                  // inf                                         /// 00f0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f28
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f50
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f68
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f78
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f84
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f98
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fa0
			 0x7fc00001,                                                  // signaling NaN                               /// 00fa4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fa8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fbc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fc0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fc8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fcc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fdc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fe0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fe8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fec
			 0x80000000,                                                  // -zero                                       /// 00ff0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ff8
			 0xbf800000 // 1                                             // SP - ve numbers                             /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x645c351f, /// 0x0
			 0x38544d57, /// 0x4
			 0x7f461e12, /// 0x8
			 0xc1444ce2, /// 0xc
			 0x2efe7b8e, /// 0x10
			 0x936764a9, /// 0x14
			 0xc070cfcf, /// 0x18
			 0xdc0d117e, /// 0x1c
			 0xe3bda258, /// 0x20
			 0xa83f3d9d, /// 0x24
			 0x1058dae5, /// 0x28
			 0x87640ca9, /// 0x2c
			 0x91d351a6, /// 0x30
			 0xae6c023e, /// 0x34
			 0xcfcc91a7, /// 0x38
			 0x696fd69f, /// 0x3c
			 0xf7daa88a, /// 0x40
			 0x43fd1c94, /// 0x44
			 0x7d431f22, /// 0x48
			 0x552385b7, /// 0x4c
			 0x86eb4b8b, /// 0x50
			 0x48cc8bb2, /// 0x54
			 0xccf7212e, /// 0x58
			 0xb4ab3b4c, /// 0x5c
			 0x93a94524, /// 0x60
			 0x97f36b38, /// 0x64
			 0x28b649a5, /// 0x68
			 0x844b2965, /// 0x6c
			 0x6e3a232d, /// 0x70
			 0x8ae64de3, /// 0x74
			 0x6fa1d5be, /// 0x78
			 0xdd78d824, /// 0x7c
			 0x015521f5, /// 0x80
			 0x2a2a95fc, /// 0x84
			 0xbf156345, /// 0x88
			 0x2d40e0ae, /// 0x8c
			 0x0b6d6a41, /// 0x90
			 0x85cdf082, /// 0x94
			 0xff0bd129, /// 0x98
			 0xaeba1496, /// 0x9c
			 0x51bfbf3a, /// 0xa0
			 0xf4e94a47, /// 0xa4
			 0x0deb5a71, /// 0xa8
			 0x9a43c784, /// 0xac
			 0x4c30adca, /// 0xb0
			 0xbd9e67c3, /// 0xb4
			 0x5b388fae, /// 0xb8
			 0xd32674d2, /// 0xbc
			 0x4c1e2c68, /// 0xc0
			 0xf46ca348, /// 0xc4
			 0x3438313a, /// 0xc8
			 0x74981508, /// 0xcc
			 0xf286fb7b, /// 0xd0
			 0x41659dae, /// 0xd4
			 0x42f72036, /// 0xd8
			 0x5f368df9, /// 0xdc
			 0xa0a0260f, /// 0xe0
			 0x5dadbf70, /// 0xe4
			 0xd950db9d, /// 0xe8
			 0x32939f25, /// 0xec
			 0x674ec024, /// 0xf0
			 0x4a5812a4, /// 0xf4
			 0xac693658, /// 0xf8
			 0x5e4b9af1, /// 0xfc
			 0x0aa4eaad, /// 0x100
			 0xfad98643, /// 0x104
			 0xf18603c0, /// 0x108
			 0x61698ddb, /// 0x10c
			 0x8a5bac42, /// 0x110
			 0xeea76a15, /// 0x114
			 0x144db6d0, /// 0x118
			 0xba54fb29, /// 0x11c
			 0xefe7acbf, /// 0x120
			 0xf6bbff05, /// 0x124
			 0xce5ea524, /// 0x128
			 0xbdfef393, /// 0x12c
			 0x0130fd27, /// 0x130
			 0xd693861f, /// 0x134
			 0x6c688e53, /// 0x138
			 0x4b6d64d5, /// 0x13c
			 0x519bfbeb, /// 0x140
			 0xd1d266ae, /// 0x144
			 0x08d622a5, /// 0x148
			 0x2d095e00, /// 0x14c
			 0x6d0ae96b, /// 0x150
			 0x69634741, /// 0x154
			 0xc7a02528, /// 0x158
			 0x276da8aa, /// 0x15c
			 0x9b6d9c40, /// 0x160
			 0x782dcc2e, /// 0x164
			 0xc28c1e88, /// 0x168
			 0x0f382135, /// 0x16c
			 0x0e093f16, /// 0x170
			 0x708be60b, /// 0x174
			 0x8ae27c5a, /// 0x178
			 0xaef1746c, /// 0x17c
			 0x4b7b9201, /// 0x180
			 0x0978718c, /// 0x184
			 0xafba5070, /// 0x188
			 0x8c22392d, /// 0x18c
			 0x6a97a333, /// 0x190
			 0x161f32a1, /// 0x194
			 0x931ea3e4, /// 0x198
			 0xe9bd135d, /// 0x19c
			 0xb9cae036, /// 0x1a0
			 0xcb8f0d23, /// 0x1a4
			 0xd70e7fb3, /// 0x1a8
			 0x2c0e98ec, /// 0x1ac
			 0x5e6c9680, /// 0x1b0
			 0xdd49a199, /// 0x1b4
			 0x51e4af03, /// 0x1b8
			 0x598edda4, /// 0x1bc
			 0x36a8b64b, /// 0x1c0
			 0xa3633b8e, /// 0x1c4
			 0x13ee87b7, /// 0x1c8
			 0x370b9d46, /// 0x1cc
			 0x68d4c275, /// 0x1d0
			 0xf83ac1a8, /// 0x1d4
			 0xdbe2a661, /// 0x1d8
			 0x20434e22, /// 0x1dc
			 0x82c39eab, /// 0x1e0
			 0x3f00b780, /// 0x1e4
			 0x088e5873, /// 0x1e8
			 0x20af997d, /// 0x1ec
			 0xd87ca367, /// 0x1f0
			 0x62b04870, /// 0x1f4
			 0x33ea5302, /// 0x1f8
			 0x9de02e2b, /// 0x1fc
			 0x82fbb84d, /// 0x200
			 0x52b17b67, /// 0x204
			 0x21c6a21a, /// 0x208
			 0x849778e4, /// 0x20c
			 0xd08268d9, /// 0x210
			 0x9dd9827e, /// 0x214
			 0xee8d6b33, /// 0x218
			 0xdb855390, /// 0x21c
			 0xeee0c614, /// 0x220
			 0xb5cb8024, /// 0x224
			 0x103d20f3, /// 0x228
			 0xc3c27b80, /// 0x22c
			 0xf31fcfc8, /// 0x230
			 0x7c14f0bf, /// 0x234
			 0x420db03e, /// 0x238
			 0x577cb9f0, /// 0x23c
			 0x16f0ca4a, /// 0x240
			 0xea87a14b, /// 0x244
			 0x8003b2de, /// 0x248
			 0xad1a841f, /// 0x24c
			 0x81c95b05, /// 0x250
			 0x5b22d0d7, /// 0x254
			 0xafbcc246, /// 0x258
			 0x280a6d75, /// 0x25c
			 0xb934cc9e, /// 0x260
			 0x7edef41a, /// 0x264
			 0x030bcffc, /// 0x268
			 0x96e07f24, /// 0x26c
			 0x8908c3b2, /// 0x270
			 0x31ebd967, /// 0x274
			 0xd29dad31, /// 0x278
			 0x909f46c8, /// 0x27c
			 0x1514b45d, /// 0x280
			 0x7ae09db5, /// 0x284
			 0x1730ba24, /// 0x288
			 0xc5ce2bd4, /// 0x28c
			 0xdf83588f, /// 0x290
			 0xcf3b1606, /// 0x294
			 0xa34c38e1, /// 0x298
			 0x7e3b88eb, /// 0x29c
			 0x57cb0bc1, /// 0x2a0
			 0x050f5267, /// 0x2a4
			 0x51a2b14a, /// 0x2a8
			 0xa3e63815, /// 0x2ac
			 0x23cf866a, /// 0x2b0
			 0x32b58186, /// 0x2b4
			 0x463c003f, /// 0x2b8
			 0x8ebedab8, /// 0x2bc
			 0x6c47df45, /// 0x2c0
			 0x66be1ac9, /// 0x2c4
			 0x571b6020, /// 0x2c8
			 0x41dbd108, /// 0x2cc
			 0xcd840455, /// 0x2d0
			 0x395aaf3d, /// 0x2d4
			 0x03dd7f3f, /// 0x2d8
			 0x531d6172, /// 0x2dc
			 0x6380ce04, /// 0x2e0
			 0xe37ef8ec, /// 0x2e4
			 0xf8a11e29, /// 0x2e8
			 0xad87c890, /// 0x2ec
			 0x77a61a06, /// 0x2f0
			 0x0586c54a, /// 0x2f4
			 0x64a003fc, /// 0x2f8
			 0x18c8dfbb, /// 0x2fc
			 0x420782ff, /// 0x300
			 0xa5e064c7, /// 0x304
			 0x7028ab28, /// 0x308
			 0x0c0defb3, /// 0x30c
			 0x2d295aa7, /// 0x310
			 0xb2b10faa, /// 0x314
			 0xd92b4fab, /// 0x318
			 0x9a114e5c, /// 0x31c
			 0xef3b2912, /// 0x320
			 0x4ae47b85, /// 0x324
			 0xec569719, /// 0x328
			 0x5b0c53ca, /// 0x32c
			 0xe8cef484, /// 0x330
			 0xa087aa79, /// 0x334
			 0x4d54f126, /// 0x338
			 0x72c3be46, /// 0x33c
			 0x8e923f0c, /// 0x340
			 0xa671d571, /// 0x344
			 0x8e9ca1d3, /// 0x348
			 0x6bb8facf, /// 0x34c
			 0x028fac06, /// 0x350
			 0xe17824e3, /// 0x354
			 0xb5d8348d, /// 0x358
			 0x33dc3782, /// 0x35c
			 0xd3099dcc, /// 0x360
			 0xe7b21968, /// 0x364
			 0x0b38a5b3, /// 0x368
			 0x05fc0b92, /// 0x36c
			 0x6eedad44, /// 0x370
			 0xf287ea2d, /// 0x374
			 0xbfd019fd, /// 0x378
			 0xc293c17a, /// 0x37c
			 0x412006df, /// 0x380
			 0x8f41c760, /// 0x384
			 0xc34c73ae, /// 0x388
			 0xb0ca6960, /// 0x38c
			 0xf45b63fe, /// 0x390
			 0x482857aa, /// 0x394
			 0xa1ef2463, /// 0x398
			 0xac56bc46, /// 0x39c
			 0xe5d85312, /// 0x3a0
			 0xe9b4d6d8, /// 0x3a4
			 0xe4e3a3bd, /// 0x3a8
			 0x549876e2, /// 0x3ac
			 0x67c18d7b, /// 0x3b0
			 0x248dfe46, /// 0x3b4
			 0x01b61768, /// 0x3b8
			 0xb3586e29, /// 0x3bc
			 0x9f1fd23d, /// 0x3c0
			 0x9b1bdd40, /// 0x3c4
			 0x538a424a, /// 0x3c8
			 0xe627a2e6, /// 0x3cc
			 0x7a920871, /// 0x3d0
			 0x62dd6e64, /// 0x3d4
			 0xcc65fb4f, /// 0x3d8
			 0x0ac2efbe, /// 0x3dc
			 0x7b5bc69c, /// 0x3e0
			 0x653c5d8c, /// 0x3e4
			 0x03bf57d0, /// 0x3e8
			 0xc9c0f706, /// 0x3ec
			 0xef5476a3, /// 0x3f0
			 0x0f04caae, /// 0x3f4
			 0x69ad5785, /// 0x3f8
			 0xe3a6d524, /// 0x3fc
			 0x6f5441c6, /// 0x400
			 0x5b5ae3d7, /// 0x404
			 0x147ecad4, /// 0x408
			 0x9cf832a1, /// 0x40c
			 0x3f0b3da4, /// 0x410
			 0x36a9dbb5, /// 0x414
			 0xf9d80c92, /// 0x418
			 0xff3e7ffb, /// 0x41c
			 0xa126d6fd, /// 0x420
			 0x5232321f, /// 0x424
			 0x1d5ed81e, /// 0x428
			 0x10f1a0df, /// 0x42c
			 0x896b109a, /// 0x430
			 0x653a60b1, /// 0x434
			 0xb2921b78, /// 0x438
			 0xbe7ebbf7, /// 0x43c
			 0xc1143e24, /// 0x440
			 0xc068b470, /// 0x444
			 0x4ba0aff8, /// 0x448
			 0x57c4b186, /// 0x44c
			 0x286fe564, /// 0x450
			 0xa22da1ea, /// 0x454
			 0xbc456dfb, /// 0x458
			 0x72088226, /// 0x45c
			 0x8be7f5cc, /// 0x460
			 0x16c4f55a, /// 0x464
			 0xf3e8ead5, /// 0x468
			 0x34c29244, /// 0x46c
			 0xd204009a, /// 0x470
			 0x5ba037d1, /// 0x474
			 0x643a3637, /// 0x478
			 0x53bf4c82, /// 0x47c
			 0x3dd46468, /// 0x480
			 0xcd1e082e, /// 0x484
			 0xe678b7b0, /// 0x488
			 0xc88060e9, /// 0x48c
			 0x12e634f7, /// 0x490
			 0xa202ac1a, /// 0x494
			 0x9af0936b, /// 0x498
			 0x3512bb29, /// 0x49c
			 0x999e2434, /// 0x4a0
			 0x0ebdd85c, /// 0x4a4
			 0x9e2a8110, /// 0x4a8
			 0x80191065, /// 0x4ac
			 0x3571322e, /// 0x4b0
			 0x620aef29, /// 0x4b4
			 0xb6b3b62b, /// 0x4b8
			 0xd4bf81b9, /// 0x4bc
			 0xed9398a7, /// 0x4c0
			 0x470bf881, /// 0x4c4
			 0x5727d075, /// 0x4c8
			 0x6e3aa232, /// 0x4cc
			 0xe9020026, /// 0x4d0
			 0xfb2faef4, /// 0x4d4
			 0x64104ec7, /// 0x4d8
			 0x8b0e7835, /// 0x4dc
			 0xc84cf33f, /// 0x4e0
			 0xf22fceb0, /// 0x4e4
			 0xf31ce683, /// 0x4e8
			 0x9811ad15, /// 0x4ec
			 0x6a1715b7, /// 0x4f0
			 0x000ea4d2, /// 0x4f4
			 0x37e747ab, /// 0x4f8
			 0x71c1e8f2, /// 0x4fc
			 0xe4d4bdc6, /// 0x500
			 0x8c9542db, /// 0x504
			 0x205e2323, /// 0x508
			 0xfba1b9a9, /// 0x50c
			 0x528bcf3e, /// 0x510
			 0x8bfa19d1, /// 0x514
			 0x2745a346, /// 0x518
			 0xf37898de, /// 0x51c
			 0xef54767b, /// 0x520
			 0x19cd21ad, /// 0x524
			 0x9f0161cd, /// 0x528
			 0x18d70c4f, /// 0x52c
			 0xd52d4008, /// 0x530
			 0x29d69b41, /// 0x534
			 0x948cb0bc, /// 0x538
			 0xabb6a9a4, /// 0x53c
			 0xe6706e43, /// 0x540
			 0x1b8efc99, /// 0x544
			 0xb3e8165b, /// 0x548
			 0xd8555b2e, /// 0x54c
			 0xfbece50b, /// 0x550
			 0x979dca8d, /// 0x554
			 0x41cbd6e4, /// 0x558
			 0x02b27f06, /// 0x55c
			 0x4743b984, /// 0x560
			 0x7856f702, /// 0x564
			 0x1c5093dd, /// 0x568
			 0x2ae3c0a7, /// 0x56c
			 0x3a21200b, /// 0x570
			 0xe4669432, /// 0x574
			 0xd3d6e523, /// 0x578
			 0x382e7312, /// 0x57c
			 0x52473970, /// 0x580
			 0x5fc2b274, /// 0x584
			 0xf5186fe1, /// 0x588
			 0x754621d0, /// 0x58c
			 0x150b0f68, /// 0x590
			 0x946f19e9, /// 0x594
			 0x41921d8e, /// 0x598
			 0x507162a7, /// 0x59c
			 0x51f42903, /// 0x5a0
			 0x0d6b5840, /// 0x5a4
			 0x43ecc26c, /// 0x5a8
			 0x4802b8ad, /// 0x5ac
			 0x4722abb2, /// 0x5b0
			 0xbe7874bd, /// 0x5b4
			 0xa45b9da8, /// 0x5b8
			 0x8c204e0b, /// 0x5bc
			 0x8e663d48, /// 0x5c0
			 0x067c4819, /// 0x5c4
			 0x8bfed8f0, /// 0x5c8
			 0xdda68940, /// 0x5cc
			 0x6f48088f, /// 0x5d0
			 0xc1f37a20, /// 0x5d4
			 0x924b99f8, /// 0x5d8
			 0x65ef4a02, /// 0x5dc
			 0xc7661f57, /// 0x5e0
			 0x96df72c5, /// 0x5e4
			 0x646afaf0, /// 0x5e8
			 0x2015a0ca, /// 0x5ec
			 0xdc5297bc, /// 0x5f0
			 0x74696b97, /// 0x5f4
			 0x0ef56486, /// 0x5f8
			 0xf1c5dc25, /// 0x5fc
			 0x5b3adecd, /// 0x600
			 0xc359a2ea, /// 0x604
			 0x9722004f, /// 0x608
			 0x295009f5, /// 0x60c
			 0x0d1ed731, /// 0x610
			 0x3445fd3c, /// 0x614
			 0x6a996ac8, /// 0x618
			 0xb78f309a, /// 0x61c
			 0x65d29f4b, /// 0x620
			 0x43a675df, /// 0x624
			 0xeb35e21b, /// 0x628
			 0x52682ade, /// 0x62c
			 0xfbd0334e, /// 0x630
			 0x8d941651, /// 0x634
			 0xce88b8a7, /// 0x638
			 0x764aaf3f, /// 0x63c
			 0x0ef539a8, /// 0x640
			 0x0b024980, /// 0x644
			 0xdc906188, /// 0x648
			 0x3a4da653, /// 0x64c
			 0x57cc0bb1, /// 0x650
			 0xdd23df07, /// 0x654
			 0x44e46f28, /// 0x658
			 0x0395a02e, /// 0x65c
			 0xc3d45f2e, /// 0x660
			 0x7336cba4, /// 0x664
			 0x337d5b92, /// 0x668
			 0x2ed6fced, /// 0x66c
			 0x2d77fd29, /// 0x670
			 0x3ea677ad, /// 0x674
			 0xc330e963, /// 0x678
			 0xc5886b4f, /// 0x67c
			 0x1e6ca902, /// 0x680
			 0x75c0c174, /// 0x684
			 0xca8b5935, /// 0x688
			 0x90507561, /// 0x68c
			 0x5f25f4f8, /// 0x690
			 0xf3095637, /// 0x694
			 0xb2c0b24b, /// 0x698
			 0x55322fc5, /// 0x69c
			 0x4779d3df, /// 0x6a0
			 0x018801bd, /// 0x6a4
			 0xb28ffe82, /// 0x6a8
			 0x2c7779fe, /// 0x6ac
			 0xf22807dc, /// 0x6b0
			 0x537bb645, /// 0x6b4
			 0xae547ab2, /// 0x6b8
			 0xc42c7e4c, /// 0x6bc
			 0x8ee25d5c, /// 0x6c0
			 0xff6bcc4a, /// 0x6c4
			 0xd930e1cf, /// 0x6c8
			 0x9520abec, /// 0x6cc
			 0xa5b7cc61, /// 0x6d0
			 0x8999b561, /// 0x6d4
			 0x11daf160, /// 0x6d8
			 0xb843e4fb, /// 0x6dc
			 0xa597f36d, /// 0x6e0
			 0xb7c1fe4c, /// 0x6e4
			 0xe351a3a4, /// 0x6e8
			 0x2239e891, /// 0x6ec
			 0xb3d6d027, /// 0x6f0
			 0x8ec64d10, /// 0x6f4
			 0xe41fb724, /// 0x6f8
			 0x639d51cc, /// 0x6fc
			 0x85f841c9, /// 0x700
			 0x7d85aa6a, /// 0x704
			 0x910c2fa8, /// 0x708
			 0xcc53da75, /// 0x70c
			 0x3606811f, /// 0x710
			 0x4a5ad2d1, /// 0x714
			 0x67af245c, /// 0x718
			 0x7792f8f3, /// 0x71c
			 0xc35dc512, /// 0x720
			 0x18b1ca4d, /// 0x724
			 0x77b211af, /// 0x728
			 0xd6c88c8e, /// 0x72c
			 0xd90a3f38, /// 0x730
			 0xef65c163, /// 0x734
			 0x5f462d41, /// 0x738
			 0xa9dad68f, /// 0x73c
			 0x6c919ab2, /// 0x740
			 0x201fb3f8, /// 0x744
			 0x7f4b4538, /// 0x748
			 0xe67af90f, /// 0x74c
			 0xd853783f, /// 0x750
			 0x658a9af4, /// 0x754
			 0x013dde36, /// 0x758
			 0x413cfc6e, /// 0x75c
			 0xfc2ba21e, /// 0x760
			 0xa47438a8, /// 0x764
			 0xa55c7a3b, /// 0x768
			 0x12b73c38, /// 0x76c
			 0x62ee9b97, /// 0x770
			 0x0609a22b, /// 0x774
			 0xadd29605, /// 0x778
			 0xdee20516, /// 0x77c
			 0x12a3b319, /// 0x780
			 0x09bad4d7, /// 0x784
			 0xb96e80f6, /// 0x788
			 0x8922ced3, /// 0x78c
			 0xc5b3c0d4, /// 0x790
			 0xfbce2afc, /// 0x794
			 0xb27ac2da, /// 0x798
			 0x4616dbc5, /// 0x79c
			 0xade19dad, /// 0x7a0
			 0xa94d0966, /// 0x7a4
			 0xdbbd8d4f, /// 0x7a8
			 0xe6bf595a, /// 0x7ac
			 0x86b8b23b, /// 0x7b0
			 0xa2478677, /// 0x7b4
			 0xa3ab191a, /// 0x7b8
			 0xbfee09d9, /// 0x7bc
			 0x364c85ff, /// 0x7c0
			 0x26513c52, /// 0x7c4
			 0x5f7e0289, /// 0x7c8
			 0x89837d2b, /// 0x7cc
			 0xc4b898a8, /// 0x7d0
			 0x01ce791d, /// 0x7d4
			 0xeeab54e3, /// 0x7d8
			 0x0619cc2b, /// 0x7dc
			 0xfa9fb7ed, /// 0x7e0
			 0xc95533ec, /// 0x7e4
			 0x33c5012b, /// 0x7e8
			 0xeb6d7850, /// 0x7ec
			 0xe8033eb5, /// 0x7f0
			 0xc53f66fb, /// 0x7f4
			 0x2bec4419, /// 0x7f8
			 0xa05bd57d, /// 0x7fc
			 0x2b2bb5ad, /// 0x800
			 0x70490bd9, /// 0x804
			 0xeba62823, /// 0x808
			 0xf5af9ddf, /// 0x80c
			 0x0196f498, /// 0x810
			 0x5d15f743, /// 0x814
			 0x7eb63d94, /// 0x818
			 0x0d134083, /// 0x81c
			 0xc6b436cb, /// 0x820
			 0x6756892c, /// 0x824
			 0xd84c6392, /// 0x828
			 0x930b494b, /// 0x82c
			 0x156cac2b, /// 0x830
			 0x96d419d8, /// 0x834
			 0xb8a5dc09, /// 0x838
			 0x0335381b, /// 0x83c
			 0xa9561de6, /// 0x840
			 0xf0164bdc, /// 0x844
			 0x133466cf, /// 0x848
			 0x4edc18cc, /// 0x84c
			 0xa87caad6, /// 0x850
			 0x58bd3b06, /// 0x854
			 0x64cb77bd, /// 0x858
			 0xbe0ba596, /// 0x85c
			 0xfe0312e8, /// 0x860
			 0x2b24fd10, /// 0x864
			 0x8d340a1e, /// 0x868
			 0xd95c6a31, /// 0x86c
			 0x96633835, /// 0x870
			 0x60527a0a, /// 0x874
			 0x6f8bcd08, /// 0x878
			 0x8f05bf20, /// 0x87c
			 0xb3fd0beb, /// 0x880
			 0xfffcff04, /// 0x884
			 0xc75bdb2a, /// 0x888
			 0xdb8daf74, /// 0x88c
			 0x64c336a3, /// 0x890
			 0x8191f096, /// 0x894
			 0x37ac490e, /// 0x898
			 0xf62efb61, /// 0x89c
			 0xe846280e, /// 0x8a0
			 0xc1580515, /// 0x8a4
			 0xb3c7ae52, /// 0x8a8
			 0x2e186d62, /// 0x8ac
			 0xc8ad17ad, /// 0x8b0
			 0x239502de, /// 0x8b4
			 0xfb9582c9, /// 0x8b8
			 0xb5021459, /// 0x8bc
			 0x32651149, /// 0x8c0
			 0x5f78669b, /// 0x8c4
			 0x05dd1f9c, /// 0x8c8
			 0x7450c01f, /// 0x8cc
			 0x3238e82d, /// 0x8d0
			 0x08f06965, /// 0x8d4
			 0xae813e24, /// 0x8d8
			 0x2771e852, /// 0x8dc
			 0xe91250f1, /// 0x8e0
			 0x59228161, /// 0x8e4
			 0xb864402b, /// 0x8e8
			 0x17161c51, /// 0x8ec
			 0xc9da5136, /// 0x8f0
			 0x28827b67, /// 0x8f4
			 0x722966bd, /// 0x8f8
			 0xc65e2089, /// 0x8fc
			 0x950846b7, /// 0x900
			 0x04c9f004, /// 0x904
			 0xd5c29267, /// 0x908
			 0x5049653d, /// 0x90c
			 0x310cac88, /// 0x910
			 0xa87b29a6, /// 0x914
			 0x70ee0435, /// 0x918
			 0x51a48564, /// 0x91c
			 0x3a237307, /// 0x920
			 0xe4c0310b, /// 0x924
			 0x2d0bbadf, /// 0x928
			 0x01bc0440, /// 0x92c
			 0x17fcb119, /// 0x930
			 0xd0dfe658, /// 0x934
			 0xe665b2df, /// 0x938
			 0x3136fecb, /// 0x93c
			 0x0679f4ab, /// 0x940
			 0x8f375cf3, /// 0x944
			 0x25eb3a94, /// 0x948
			 0xb0b97dd9, /// 0x94c
			 0x9d76f8a5, /// 0x950
			 0x4554719b, /// 0x954
			 0xaa82635b, /// 0x958
			 0x02367f14, /// 0x95c
			 0xfc46afb9, /// 0x960
			 0x36c79512, /// 0x964
			 0xabb10d8c, /// 0x968
			 0x0254095a, /// 0x96c
			 0x3fea6eaa, /// 0x970
			 0xd4da39ca, /// 0x974
			 0x62ec783a, /// 0x978
			 0x66f8c6fe, /// 0x97c
			 0x31ce7878, /// 0x980
			 0xa9b6cc58, /// 0x984
			 0x8636af64, /// 0x988
			 0x1a9c10d5, /// 0x98c
			 0x14e9198a, /// 0x990
			 0x404c639a, /// 0x994
			 0xf88c995f, /// 0x998
			 0x860151b5, /// 0x99c
			 0x87ae3ed8, /// 0x9a0
			 0x056db648, /// 0x9a4
			 0x3e388af7, /// 0x9a8
			 0x9377cc6e, /// 0x9ac
			 0x7a49e6f4, /// 0x9b0
			 0xf6e03e2d, /// 0x9b4
			 0x922cc8ef, /// 0x9b8
			 0x5f6bb773, /// 0x9bc
			 0xf1202165, /// 0x9c0
			 0x4f5462f6, /// 0x9c4
			 0x8a38382b, /// 0x9c8
			 0x798de595, /// 0x9cc
			 0x554f1a32, /// 0x9d0
			 0x0256767c, /// 0x9d4
			 0x05cc406c, /// 0x9d8
			 0x776d1e98, /// 0x9dc
			 0x5cbc815e, /// 0x9e0
			 0xf816b2e9, /// 0x9e4
			 0x2b24f4e6, /// 0x9e8
			 0x1044bd31, /// 0x9ec
			 0xfbf343c7, /// 0x9f0
			 0xed3c9897, /// 0x9f4
			 0x9baeb4e9, /// 0x9f8
			 0x1bf069a2, /// 0x9fc
			 0x39d9afaf, /// 0xa00
			 0xea903a5d, /// 0xa04
			 0x6c056c75, /// 0xa08
			 0x7ae28e17, /// 0xa0c
			 0x8bba6916, /// 0xa10
			 0x489a675b, /// 0xa14
			 0xa86723c1, /// 0xa18
			 0xb954da13, /// 0xa1c
			 0x5a24d4fe, /// 0xa20
			 0x59264c64, /// 0xa24
			 0x877803b8, /// 0xa28
			 0xc0774786, /// 0xa2c
			 0x8d5280d5, /// 0xa30
			 0xa340bbbf, /// 0xa34
			 0xfb2408bd, /// 0xa38
			 0x3974d87e, /// 0xa3c
			 0x5ca90ec5, /// 0xa40
			 0xfe43da3d, /// 0xa44
			 0xe78d8939, /// 0xa48
			 0x96dcfcfb, /// 0xa4c
			 0x68e69cc6, /// 0xa50
			 0xf30491dd, /// 0xa54
			 0x7b939ea3, /// 0xa58
			 0x674e3066, /// 0xa5c
			 0xed22519e, /// 0xa60
			 0x59759909, /// 0xa64
			 0x6f10a423, /// 0xa68
			 0x4afbc74c, /// 0xa6c
			 0xd9af8dda, /// 0xa70
			 0xf2358b5c, /// 0xa74
			 0x6762c6d3, /// 0xa78
			 0xd47c0b1b, /// 0xa7c
			 0xfb03237e, /// 0xa80
			 0x15df1129, /// 0xa84
			 0x2f305f16, /// 0xa88
			 0x81e7d119, /// 0xa8c
			 0xed59ff9b, /// 0xa90
			 0x87878a90, /// 0xa94
			 0x4703872c, /// 0xa98
			 0x3fe9eca3, /// 0xa9c
			 0x866a9f3e, /// 0xaa0
			 0xd2aa9d73, /// 0xaa4
			 0x9a7d670c, /// 0xaa8
			 0x7da85eb6, /// 0xaac
			 0xdd4299da, /// 0xab0
			 0xeba66c4c, /// 0xab4
			 0x905843fd, /// 0xab8
			 0x074e2f2f, /// 0xabc
			 0x2375f21c, /// 0xac0
			 0x8653e8d5, /// 0xac4
			 0x718aea13, /// 0xac8
			 0x187a64eb, /// 0xacc
			 0x5c88b2dd, /// 0xad0
			 0x4d1b16dd, /// 0xad4
			 0xdcbee7c4, /// 0xad8
			 0xeafe9cba, /// 0xadc
			 0x7797345f, /// 0xae0
			 0xb582c32a, /// 0xae4
			 0xc671f832, /// 0xae8
			 0x1852ba3e, /// 0xaec
			 0xdf8e6bf5, /// 0xaf0
			 0xf2fd8dfc, /// 0xaf4
			 0x7f50082f, /// 0xaf8
			 0x1c64635a, /// 0xafc
			 0x9d94c6c9, /// 0xb00
			 0x43903dc9, /// 0xb04
			 0xf8ee5989, /// 0xb08
			 0x76f35826, /// 0xb0c
			 0xbdce333f, /// 0xb10
			 0xd47d1153, /// 0xb14
			 0x1fc1da25, /// 0xb18
			 0x27399890, /// 0xb1c
			 0x5e396d04, /// 0xb20
			 0x2029169e, /// 0xb24
			 0x095c8896, /// 0xb28
			 0x15ddfb0f, /// 0xb2c
			 0x3517dcff, /// 0xb30
			 0x2e8fcb5e, /// 0xb34
			 0x92981626, /// 0xb38
			 0x0c6d4fb5, /// 0xb3c
			 0x9ee508e1, /// 0xb40
			 0x49bc7d22, /// 0xb44
			 0x6492779f, /// 0xb48
			 0x943aa633, /// 0xb4c
			 0xcaeebdeb, /// 0xb50
			 0xa953ee6e, /// 0xb54
			 0xad742871, /// 0xb58
			 0x28c77bc2, /// 0xb5c
			 0x14c5ab5e, /// 0xb60
			 0xf80972e1, /// 0xb64
			 0x6d353e32, /// 0xb68
			 0xce3db974, /// 0xb6c
			 0xf29d365c, /// 0xb70
			 0x16039d84, /// 0xb74
			 0x2c2e7c5d, /// 0xb78
			 0x1810132e, /// 0xb7c
			 0xd2fc9057, /// 0xb80
			 0xebb58031, /// 0xb84
			 0xec850c89, /// 0xb88
			 0x78c54444, /// 0xb8c
			 0x68aa68e7, /// 0xb90
			 0x7116d6a4, /// 0xb94
			 0x37c83c40, /// 0xb98
			 0xfef78ca4, /// 0xb9c
			 0xa7ad0b1f, /// 0xba0
			 0x1dd0349f, /// 0xba4
			 0xb995ae82, /// 0xba8
			 0x5ec50ed7, /// 0xbac
			 0x738d8ef1, /// 0xbb0
			 0xc400aedc, /// 0xbb4
			 0xb284bddb, /// 0xbb8
			 0xdd8f8740, /// 0xbbc
			 0xd5176d44, /// 0xbc0
			 0xc511ffd3, /// 0xbc4
			 0x211d2ed7, /// 0xbc8
			 0x4c75d516, /// 0xbcc
			 0x4a732686, /// 0xbd0
			 0x9124f807, /// 0xbd4
			 0x63b8e443, /// 0xbd8
			 0x2e11a2a9, /// 0xbdc
			 0x91b83b23, /// 0xbe0
			 0xf2d7bad1, /// 0xbe4
			 0xac46e85c, /// 0xbe8
			 0x067a2581, /// 0xbec
			 0x65bd3bea, /// 0xbf0
			 0x400d6c3a, /// 0xbf4
			 0xd5d97899, /// 0xbf8
			 0xa4a2dae7, /// 0xbfc
			 0x59b54b17, /// 0xc00
			 0x866df25d, /// 0xc04
			 0x267a8c70, /// 0xc08
			 0x69fd71de, /// 0xc0c
			 0x84aae765, /// 0xc10
			 0xc3ecf210, /// 0xc14
			 0x2366df69, /// 0xc18
			 0x23e93694, /// 0xc1c
			 0x90ad3bb6, /// 0xc20
			 0xdbe10cec, /// 0xc24
			 0x02f6a4d3, /// 0xc28
			 0x404b4f5d, /// 0xc2c
			 0x859434cd, /// 0xc30
			 0xce491f3c, /// 0xc34
			 0x41ff2afe, /// 0xc38
			 0x50be564b, /// 0xc3c
			 0x0f224ddc, /// 0xc40
			 0xe87e0d4e, /// 0xc44
			 0xfc968275, /// 0xc48
			 0xf4fef1a8, /// 0xc4c
			 0x19a26244, /// 0xc50
			 0xd24bd4f5, /// 0xc54
			 0xcc355960, /// 0xc58
			 0x6c7567fe, /// 0xc5c
			 0x41269db5, /// 0xc60
			 0xd21ccef4, /// 0xc64
			 0xdd9a4b2f, /// 0xc68
			 0x9c460a7f, /// 0xc6c
			 0x1779239f, /// 0xc70
			 0xa9c4ef51, /// 0xc74
			 0x2b14771d, /// 0xc78
			 0x1375371f, /// 0xc7c
			 0xaf8f7f6c, /// 0xc80
			 0x40511f56, /// 0xc84
			 0x35631ed7, /// 0xc88
			 0x9fbb7125, /// 0xc8c
			 0x6a7433aa, /// 0xc90
			 0xfbb2c297, /// 0xc94
			 0x0fca476d, /// 0xc98
			 0xab3d9608, /// 0xc9c
			 0x74d31e3d, /// 0xca0
			 0xbb8de171, /// 0xca4
			 0xc6188fe4, /// 0xca8
			 0xd364bfc6, /// 0xcac
			 0xe7ed594a, /// 0xcb0
			 0x55ec024a, /// 0xcb4
			 0x7d69625d, /// 0xcb8
			 0xb50eda14, /// 0xcbc
			 0x8ac2ca20, /// 0xcc0
			 0xce703307, /// 0xcc4
			 0xa4331734, /// 0xcc8
			 0xea7d100a, /// 0xccc
			 0x82296fd5, /// 0xcd0
			 0xd56d607f, /// 0xcd4
			 0xf48ed576, /// 0xcd8
			 0xebe4a9fd, /// 0xcdc
			 0x53a4e504, /// 0xce0
			 0xbd9298d2, /// 0xce4
			 0x32a71636, /// 0xce8
			 0x63c89c9a, /// 0xcec
			 0x129b2df9, /// 0xcf0
			 0x78d1cb7b, /// 0xcf4
			 0x853fc536, /// 0xcf8
			 0xd71f4ea4, /// 0xcfc
			 0x7d6d10f4, /// 0xd00
			 0x49ae06a8, /// 0xd04
			 0x98cc682e, /// 0xd08
			 0xfbee76ea, /// 0xd0c
			 0x1200248f, /// 0xd10
			 0x58873480, /// 0xd14
			 0x385230c8, /// 0xd18
			 0x2cea51be, /// 0xd1c
			 0xb22ebeff, /// 0xd20
			 0xb23f64d8, /// 0xd24
			 0xa99ad10d, /// 0xd28
			 0xfc0af64e, /// 0xd2c
			 0x6b9978ea, /// 0xd30
			 0xa578e29f, /// 0xd34
			 0x1760c2f3, /// 0xd38
			 0xb2c65646, /// 0xd3c
			 0xef4fc7ad, /// 0xd40
			 0x022203cd, /// 0xd44
			 0xf05cd205, /// 0xd48
			 0x7e7e6423, /// 0xd4c
			 0x370a89b7, /// 0xd50
			 0xe219933d, /// 0xd54
			 0x6abd3861, /// 0xd58
			 0x5da88ba8, /// 0xd5c
			 0xa856c208, /// 0xd60
			 0x4775c3d0, /// 0xd64
			 0x0cf37360, /// 0xd68
			 0xecbec6fa, /// 0xd6c
			 0x811b673f, /// 0xd70
			 0x1245243f, /// 0xd74
			 0x36d52fe0, /// 0xd78
			 0xbee8dccd, /// 0xd7c
			 0x53f0a0d2, /// 0xd80
			 0xdb9ae4b1, /// 0xd84
			 0xf6c3476f, /// 0xd88
			 0x5b665b57, /// 0xd8c
			 0xd76d4ac4, /// 0xd90
			 0x305ea4f7, /// 0xd94
			 0x55152471, /// 0xd98
			 0x2c5caf92, /// 0xd9c
			 0xb2404c01, /// 0xda0
			 0x5e7469bf, /// 0xda4
			 0xca7d9bce, /// 0xda8
			 0x0ca47176, /// 0xdac
			 0x356f349c, /// 0xdb0
			 0x3796808b, /// 0xdb4
			 0x508dbc8b, /// 0xdb8
			 0xfc831e4a, /// 0xdbc
			 0xa9107fd7, /// 0xdc0
			 0xeaf3a5f9, /// 0xdc4
			 0x71dfbd6c, /// 0xdc8
			 0x119af4fb, /// 0xdcc
			 0x4c3eab2c, /// 0xdd0
			 0x0595ad50, /// 0xdd4
			 0xc71845d7, /// 0xdd8
			 0x3192c116, /// 0xddc
			 0xf95751bf, /// 0xde0
			 0x8af21cb4, /// 0xde4
			 0xf31b98d7, /// 0xde8
			 0xfe5218c3, /// 0xdec
			 0x4793e541, /// 0xdf0
			 0xa7e6e0b6, /// 0xdf4
			 0x01c5be49, /// 0xdf8
			 0x0251f6ac, /// 0xdfc
			 0xd88f82bf, /// 0xe00
			 0x5259acfb, /// 0xe04
			 0x2dad8204, /// 0xe08
			 0x205324be, /// 0xe0c
			 0xfbff712d, /// 0xe10
			 0xd11e6809, /// 0xe14
			 0x1306023f, /// 0xe18
			 0xfb1bea5a, /// 0xe1c
			 0x8713829a, /// 0xe20
			 0xbb58f311, /// 0xe24
			 0xf4db9738, /// 0xe28
			 0x8af7c525, /// 0xe2c
			 0x27ad3bf5, /// 0xe30
			 0x10f435ee, /// 0xe34
			 0x1037e324, /// 0xe38
			 0x20367c8a, /// 0xe3c
			 0x03f4811e, /// 0xe40
			 0xc46dd8d7, /// 0xe44
			 0x6fa1000a, /// 0xe48
			 0x7b4726f4, /// 0xe4c
			 0x7ca5cdb5, /// 0xe50
			 0x3eba1d21, /// 0xe54
			 0x18b099a5, /// 0xe58
			 0xaed5b6f3, /// 0xe5c
			 0x33d662e8, /// 0xe60
			 0x9413acf8, /// 0xe64
			 0x785650fa, /// 0xe68
			 0xfdaa0904, /// 0xe6c
			 0xa4293f97, /// 0xe70
			 0x925720bf, /// 0xe74
			 0xb6f3a88a, /// 0xe78
			 0x4b9e83eb, /// 0xe7c
			 0x192f1b66, /// 0xe80
			 0x150268f9, /// 0xe84
			 0x8637f1e8, /// 0xe88
			 0xb5fe4643, /// 0xe8c
			 0xb8e90d2e, /// 0xe90
			 0xfac6412f, /// 0xe94
			 0xcf00d044, /// 0xe98
			 0x457cdca6, /// 0xe9c
			 0x30898d57, /// 0xea0
			 0x96d36b97, /// 0xea4
			 0x94e66d52, /// 0xea8
			 0x84e22320, /// 0xeac
			 0x506a3748, /// 0xeb0
			 0x9db571fd, /// 0xeb4
			 0x68b9a8f1, /// 0xeb8
			 0x90e97024, /// 0xebc
			 0xcc0f7053, /// 0xec0
			 0xcc555e0c, /// 0xec4
			 0xb8f7ba5d, /// 0xec8
			 0x74528667, /// 0xecc
			 0x2c3d61c9, /// 0xed0
			 0x0e719e56, /// 0xed4
			 0xa82ee7b5, /// 0xed8
			 0x08a403eb, /// 0xedc
			 0xd88b1f97, /// 0xee0
			 0xa640afe4, /// 0xee4
			 0x331156af, /// 0xee8
			 0x52eeb6cd, /// 0xeec
			 0x088bd539, /// 0xef0
			 0x48c398f3, /// 0xef4
			 0xe535ffe9, /// 0xef8
			 0x133911eb, /// 0xefc
			 0xea89322d, /// 0xf00
			 0x6d9ae134, /// 0xf04
			 0x0882e058, /// 0xf08
			 0x578763f1, /// 0xf0c
			 0x41a53000, /// 0xf10
			 0x81ad63d1, /// 0xf14
			 0xd789dc5a, /// 0xf18
			 0x88ba44d1, /// 0xf1c
			 0xbe89abfd, /// 0xf20
			 0x2dfc5fd9, /// 0xf24
			 0xc597589f, /// 0xf28
			 0xc7084e3b, /// 0xf2c
			 0x8d4e5a24, /// 0xf30
			 0x7c0e61be, /// 0xf34
			 0x8899c718, /// 0xf38
			 0xdb6c0728, /// 0xf3c
			 0xc99f11c7, /// 0xf40
			 0x63e7d79f, /// 0xf44
			 0x6c67f995, /// 0xf48
			 0x4ecaeedd, /// 0xf4c
			 0xf2fbc65c, /// 0xf50
			 0x19300b73, /// 0xf54
			 0x007e5185, /// 0xf58
			 0x61ad003a, /// 0xf5c
			 0xa89bafe3, /// 0xf60
			 0x76e75f70, /// 0xf64
			 0xa7fd1a76, /// 0xf68
			 0xdc1e5d6f, /// 0xf6c
			 0x9fec7d8a, /// 0xf70
			 0x55369a45, /// 0xf74
			 0xccf53342, /// 0xf78
			 0xb3d46864, /// 0xf7c
			 0x4a941fdf, /// 0xf80
			 0xc383a697, /// 0xf84
			 0x42c81557, /// 0xf88
			 0xe0b6d836, /// 0xf8c
			 0x55e87d1a, /// 0xf90
			 0x48a804ec, /// 0xf94
			 0xb319b50c, /// 0xf98
			 0x893bf5d5, /// 0xf9c
			 0x461034c8, /// 0xfa0
			 0x44ba4aea, /// 0xfa4
			 0xf80b9a42, /// 0xfa8
			 0xca947c2a, /// 0xfac
			 0x90c9da16, /// 0xfb0
			 0xa3a14676, /// 0xfb4
			 0xa4673920, /// 0xfb8
			 0x605cf7fa, /// 0xfbc
			 0x94ab6f91, /// 0xfc0
			 0x9aeef809, /// 0xfc4
			 0x610492de, /// 0xfc8
			 0x3a6d616e, /// 0xfcc
			 0xa36c2a93, /// 0xfd0
			 0xe3886cf6, /// 0xfd4
			 0x5ebf136f, /// 0xfd8
			 0x3df19dc7, /// 0xfdc
			 0xf569672e, /// 0xfe0
			 0x8369333e, /// 0xfe4
			 0x04891c51, /// 0xfe8
			 0x2a9d6349, /// 0xfec
			 0xd71cfe53, /// 0xff0
			 0x9d2c58ff, /// 0xff4
			 0xc62b025e, /// 0xff8
			 0x6545778d /// last
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
			 0x0000071c,
			 0x000000e0,
			 0x00000214,
			 0x00000270,
			 0x000007d0,
			 0x00000174,
			 0x00000188,
			 0x0000020c,

			 /// vpu register f2
			 0x40c00000,
			 0x41f80000,
			 0x41a80000,
			 0x41000000,
			 0x41b80000,
			 0x41e80000,
			 0x41880000,
			 0x3f800000,

			 /// vpu register f3
			 0x41800000,
			 0x41c00000,
			 0x41800000,
			 0x41b00000,
			 0x41300000,
			 0x41200000,
			 0x41880000,
			 0x41300000,

			 /// vpu register f4
			 0x40a00000,
			 0x41880000,
			 0x41300000,
			 0x41980000,
			 0x41500000,
			 0x40000000,
			 0x41b00000,
			 0x41a00000,

			 /// vpu register f5
			 0x41880000,
			 0x42000000,
			 0x40c00000,
			 0x41400000,
			 0x41600000,
			 0x42000000,
			 0x41000000,
			 0x40800000,

			 /// vpu register f6
			 0x41d80000,
			 0x40000000,
			 0x41c80000,
			 0x41980000,
			 0x41980000,
			 0x42000000,
			 0x41a00000,
			 0x41b00000,

			 /// vpu register f7
			 0x40c00000,
			 0x40000000,
			 0x41a80000,
			 0x41200000,
			 0x41d80000,
			 0x41a80000,
			 0x41100000,
			 0x41d00000,

			 /// vpu register f8
			 0x41300000,
			 0x41f80000,
			 0x41a00000,
			 0x41a00000,
			 0x41a00000,
			 0x40c00000,
			 0x41300000,
			 0x40000000,

			 /// vpu register f9
			 0x41d80000,
			 0x41d80000,
			 0x41600000,
			 0x40400000,
			 0x40c00000,
			 0x41e80000,
			 0x40c00000,
			 0x41a80000,

			 /// vpu register f10
			 0x41a00000,
			 0x41c80000,
			 0x41100000,
			 0x40a00000,
			 0x41800000,
			 0x41500000,
			 0x41800000,
			 0x41e80000,

			 /// vpu register f11
			 0x41600000,
			 0x41880000,
			 0x42000000,
			 0x41000000,
			 0x40a00000,
			 0x40e00000,
			 0x40400000,
			 0x41e80000,

			 /// vpu register f12
			 0x41100000,
			 0x41f80000,
			 0x40e00000,
			 0x41000000,
			 0x41700000,
			 0x42000000,
			 0x41900000,
			 0x41d80000,

			 /// vpu register f13
			 0x41c80000,
			 0x40000000,
			 0x41880000,
			 0x41200000,
			 0x41f80000,
			 0x41e80000,
			 0x41300000,
			 0x41d80000,

			 /// vpu register f14
			 0x41800000,
			 0x41a80000,
			 0x41c80000,
			 0x41200000,
			 0x41b80000,
			 0x41e80000,
			 0x41200000,
			 0x42000000,

			 /// vpu register f15
			 0x40400000,
			 0x41300000,
			 0x41b80000,
			 0x41d80000,
			 0x41300000,
			 0x41500000,
			 0x41b80000,
			 0x41f80000,

			 /// vpu register f16
			 0x42000000,
			 0x41d80000,
			 0x41500000,
			 0x41400000,
			 0x41880000,
			 0x41800000,
			 0x41800000,
			 0x41a00000,

			 /// vpu register f17
			 0x40400000,
			 0x41800000,
			 0x41300000,
			 0x41400000,
			 0x41300000,
			 0x41d00000,
			 0x41f80000,
			 0x41900000,

			 /// vpu register f18
			 0x41d80000,
			 0x41800000,
			 0x41900000,
			 0x41800000,
			 0x41f00000,
			 0x41a80000,
			 0x41d00000,
			 0x41500000,

			 /// vpu register f19
			 0x41200000,
			 0x40000000,
			 0x41a80000,
			 0x40a00000,
			 0x41300000,
			 0x41500000,
			 0x41a80000,
			 0x3f800000,

			 /// vpu register f20
			 0x40800000,
			 0x40c00000,
			 0x41800000,
			 0x42000000,
			 0x41a80000,
			 0x41900000,
			 0x41e80000,
			 0x40c00000,

			 /// vpu register f21
			 0x41700000,
			 0x41300000,
			 0x41d00000,
			 0x41c00000,
			 0x41980000,
			 0x40400000,
			 0x41d80000,
			 0x41200000,

			 /// vpu register f22
			 0x40400000,
			 0x41c00000,
			 0x41400000,
			 0x41400000,
			 0x40c00000,
			 0x41c00000,
			 0x42000000,
			 0x41980000,

			 /// vpu register f23
			 0x41d80000,
			 0x40800000,
			 0x41000000,
			 0x41880000,
			 0x41c00000,
			 0x41000000,
			 0x40c00000,
			 0x41900000,

			 /// vpu register f24
			 0x41700000,
			 0x41880000,
			 0x40400000,
			 0x3f800000,
			 0x42000000,
			 0x41400000,
			 0x41100000,
			 0x41800000,

			 /// vpu register f25
			 0x41100000,
			 0x40e00000,
			 0x41980000,
			 0x41500000,
			 0x41c00000,
			 0x41000000,
			 0x40c00000,
			 0x41880000,

			 /// vpu register f26
			 0x41c00000,
			 0x41d80000,
			 0x41500000,
			 0x41800000,
			 0x40c00000,
			 0x41980000,
			 0x41b00000,
			 0x41f00000,

			 /// vpu register f27
			 0x41100000,
			 0x41500000,
			 0x41f00000,
			 0x41400000,
			 0x40e00000,
			 0x41f80000,
			 0x41600000,
			 0x41000000,

			 /// vpu register f28
			 0x41e80000,
			 0x41200000,
			 0x41880000,
			 0x40a00000,
			 0x40800000,
			 0x41b80000,
			 0x40a00000,
			 0x41d80000,

			 /// vpu register f29
			 0x41800000,
			 0x3f800000,
			 0x41a80000,
			 0x41e00000,
			 0x41000000,
			 0x41b00000,
			 0x40800000,
			 0x40a00000,

			 /// vpu register f30
			 0x41a80000,
			 0x41d00000,
			 0x41880000,
			 0x41a80000,
			 0x40000000,
			 0x40c00000,
			 0x41900000,
			 0x41000000,

			 /// vpu register f31
			 0x41400000,
			 0x41f00000,
			 0x41a00000,
			 0x41c80000,
			 0x41500000,
			 0x40400000,
			 0x41000000,
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
		"fsra.pi f22, f27, f20\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f7, f9, f1\n"                                ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f5, f4, f15\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f26, f27, f23\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f11, f19, f18\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f2, f17, f7\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f18, f20, f16\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f5, f9, f12\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f22, f2, f8\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f30, f2, f3\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f2, f29, f17\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f6, f29, f5\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f28, f7, f19\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f9, f20, f3\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f10, f26, f8\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f7, f5, f20\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f22, f24, f2\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f30, f6, f13\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f10, f3, f10\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f22, f1, f12\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f5, f0, f2\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f2, f20, f19\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f13, f2, f4\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f18, f3, f25\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f29, f2, f27\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f30, f20, f24\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f23, f15, f12\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f10, f26, f8\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f5, f30, f8\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f9, f1, f24\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f16, f26, f25\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f20, f29, f17\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f10, f11, f20\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f3, f27, f2\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f20, f4, f9\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f3, f25, f27\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f3, f25, f19\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f26, f17, f16\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f4, f4, f16\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f4, f16, f26\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f30, f4, f3\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f13, f0, f23\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f2, f20, f23\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f12, f4, f7\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f14, f27, f21\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f28, f17, f29\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f22, f3, f4\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f15, f8, f8\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f10, f30, f4\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f21, f30, f9\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f14, f22, f14\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f29, f14, f28\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f2, f20, f14\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f8, f14, f20\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f17, f2, f16\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f15, f27, f14\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f15, f20, f13\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f13, f20, f2\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f22, f24, f9\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f21, f6, f26\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f19, f18, f4\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f29, f14, f28\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f7, f0, f0\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f20, f20, f21\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f25, f2, f0\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f6, f29, f11\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f20, f20, f13\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f2, f14, f12\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f21, f12, f18\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f5, f13, f14\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f18, f3, f21\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f9, f5, f5\n"                                ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f11, f18, f4\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f6, f22, f26\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f26, f3, f9\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f30, f21, f9\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f5, f20, f7\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f26, f15, f16\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f6, f2, f20\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f6, f3, f21\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f19, f27, f20\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f29, f16, f15\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f27, f22, f13\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f6, f30, f17\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f29, f13, f25\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f11, f3, f25\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f16, f16, f23\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f12, f6, f17\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f14, f0, f27\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f18, f3, f5\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f9, f16, f25\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f5, f25, f11\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f7, f11, f7\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f6, f7, f12\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f28, f17, f27\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f21, f22, f24\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f13, f14, f18\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f17, f14, f8\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsra.pi f7, f10, f17\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
