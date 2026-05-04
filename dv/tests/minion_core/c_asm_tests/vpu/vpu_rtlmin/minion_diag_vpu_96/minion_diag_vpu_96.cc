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
			 0x674a2af5, /// 0x0
			 0xd1dec215, /// 0x4
			 0xb8049bd7, /// 0x8
			 0xa9200f19, /// 0xc
			 0x77c43e72, /// 0x10
			 0x4e977f83, /// 0x14
			 0x2e5bddcb, /// 0x18
			 0xc7f6ba04, /// 0x1c
			 0x40490cac, /// 0x20
			 0xa521cfd9, /// 0x24
			 0x65825ade, /// 0x28
			 0xd6fd2dad, /// 0x2c
			 0x88ba70f3, /// 0x30
			 0xf75a56f5, /// 0x34
			 0xfcfd1e80, /// 0x38
			 0x5582f49c, /// 0x3c
			 0x374897ab, /// 0x40
			 0x59afcad3, /// 0x44
			 0xe61f1c4c, /// 0x48
			 0x0664e6d1, /// 0x4c
			 0xb19b6e15, /// 0x50
			 0x7d6a88af, /// 0x54
			 0xbd87fc53, /// 0x58
			 0x8b32e6b8, /// 0x5c
			 0x524adce2, /// 0x60
			 0x2061b37e, /// 0x64
			 0xf70c9cf3, /// 0x68
			 0x93a8d94a, /// 0x6c
			 0xb80c599c, /// 0x70
			 0xcd196cdf, /// 0x74
			 0xeeaf6b87, /// 0x78
			 0xbac398c6, /// 0x7c
			 0xb4e8cdda, /// 0x80
			 0x0abffc4d, /// 0x84
			 0xc6c21d60, /// 0x88
			 0xfdc7b781, /// 0x8c
			 0xe7e41f75, /// 0x90
			 0xb9443bc4, /// 0x94
			 0x4ac9f45f, /// 0x98
			 0x6442bb43, /// 0x9c
			 0x5f072bd4, /// 0xa0
			 0x83efff4f, /// 0xa4
			 0xf3cb40fc, /// 0xa8
			 0x4cf99197, /// 0xac
			 0x755d76a6, /// 0xb0
			 0xb0b50aa0, /// 0xb4
			 0x84f7a6ce, /// 0xb8
			 0x5aa8c5a4, /// 0xbc
			 0xd3874de1, /// 0xc0
			 0x79d6d517, /// 0xc4
			 0x99f03e64, /// 0xc8
			 0xa75795e1, /// 0xcc
			 0xf82ad4f9, /// 0xd0
			 0xf915bd58, /// 0xd4
			 0xdaf84dc0, /// 0xd8
			 0xbcac2d54, /// 0xdc
			 0x43097b9e, /// 0xe0
			 0xe755d6b9, /// 0xe4
			 0xc930a429, /// 0xe8
			 0xb5484052, /// 0xec
			 0xa39abb4b, /// 0xf0
			 0x280f1d2c, /// 0xf4
			 0x5a57bb22, /// 0xf8
			 0xc754a23f, /// 0xfc
			 0x22b4c579, /// 0x100
			 0x3a7398b0, /// 0x104
			 0x8ab7c015, /// 0x108
			 0x03804df1, /// 0x10c
			 0x03069d2d, /// 0x110
			 0x43ad2015, /// 0x114
			 0xc14a083b, /// 0x118
			 0x5b618cd3, /// 0x11c
			 0xc4b25465, /// 0x120
			 0xc03ff9a1, /// 0x124
			 0x9f1e330c, /// 0x128
			 0x2a77abe2, /// 0x12c
			 0x7c1fdbe8, /// 0x130
			 0x33d0d5a4, /// 0x134
			 0xf06b46aa, /// 0x138
			 0xd787612d, /// 0x13c
			 0xcf988060, /// 0x140
			 0xdeabd44b, /// 0x144
			 0x104063f9, /// 0x148
			 0x15f2402a, /// 0x14c
			 0xe11df789, /// 0x150
			 0xa4089ed5, /// 0x154
			 0xe25ef392, /// 0x158
			 0xf4d6f940, /// 0x15c
			 0xbae39bcb, /// 0x160
			 0x85010142, /// 0x164
			 0xa8d89595, /// 0x168
			 0x6808b9e4, /// 0x16c
			 0x7dff889f, /// 0x170
			 0x47e29ff4, /// 0x174
			 0xa66d8d59, /// 0x178
			 0xf7d50c70, /// 0x17c
			 0x864360f2, /// 0x180
			 0xd1d3d64a, /// 0x184
			 0x99ae54e0, /// 0x188
			 0x6cfa7908, /// 0x18c
			 0xb29bbae6, /// 0x190
			 0xcf9e5039, /// 0x194
			 0x7f20767c, /// 0x198
			 0x42e785b2, /// 0x19c
			 0x73d16eb4, /// 0x1a0
			 0x9f34ca68, /// 0x1a4
			 0x01d7c771, /// 0x1a8
			 0xff26a445, /// 0x1ac
			 0x599e7e64, /// 0x1b0
			 0xe0b09eda, /// 0x1b4
			 0xf701eed7, /// 0x1b8
			 0x8a0a6cb5, /// 0x1bc
			 0x080a5b82, /// 0x1c0
			 0x4635e530, /// 0x1c4
			 0x8a64ff9d, /// 0x1c8
			 0x179ce357, /// 0x1cc
			 0xdd8fb7f7, /// 0x1d0
			 0xb2e54a6d, /// 0x1d4
			 0xcaa581d2, /// 0x1d8
			 0xd43c3cd5, /// 0x1dc
			 0x18ff1488, /// 0x1e0
			 0x8e8d6bc7, /// 0x1e4
			 0x357ffc70, /// 0x1e8
			 0xe7a3bc11, /// 0x1ec
			 0x5f49337e, /// 0x1f0
			 0xbf73a9e6, /// 0x1f4
			 0x8ae4d800, /// 0x1f8
			 0x8071f3b6, /// 0x1fc
			 0x5e3037ed, /// 0x200
			 0xd8004e3e, /// 0x204
			 0xa1f2dec9, /// 0x208
			 0xd7aea502, /// 0x20c
			 0x7e7d9181, /// 0x210
			 0xbaf1a180, /// 0x214
			 0x21d72651, /// 0x218
			 0x3aee79ec, /// 0x21c
			 0xff056907, /// 0x220
			 0x61958b39, /// 0x224
			 0x87d75681, /// 0x228
			 0x314e0fa5, /// 0x22c
			 0x44697861, /// 0x230
			 0x4011f682, /// 0x234
			 0x45020b66, /// 0x238
			 0x0225cdfd, /// 0x23c
			 0x0be051a1, /// 0x240
			 0xfa6f4f5d, /// 0x244
			 0x6041c6d4, /// 0x248
			 0xf64351a6, /// 0x24c
			 0x33b32e03, /// 0x250
			 0x424efc62, /// 0x254
			 0x6d7c0159, /// 0x258
			 0xbf787fb6, /// 0x25c
			 0x4c9b02dc, /// 0x260
			 0x25b9ef40, /// 0x264
			 0x1c63d1d0, /// 0x268
			 0x6af2465b, /// 0x26c
			 0xca4eadea, /// 0x270
			 0x4fbbf225, /// 0x274
			 0xa5f23056, /// 0x278
			 0xeb422d66, /// 0x27c
			 0x100402e1, /// 0x280
			 0x7b9fae6b, /// 0x284
			 0x3874416e, /// 0x288
			 0xc5774505, /// 0x28c
			 0x25d63749, /// 0x290
			 0xf1ed7a48, /// 0x294
			 0x8179a5f4, /// 0x298
			 0x581fa7c5, /// 0x29c
			 0x36698164, /// 0x2a0
			 0x6969d1a9, /// 0x2a4
			 0x7927d332, /// 0x2a8
			 0xff201d66, /// 0x2ac
			 0xf5ecfe0a, /// 0x2b0
			 0x49753d81, /// 0x2b4
			 0xdcce5434, /// 0x2b8
			 0xcde877f8, /// 0x2bc
			 0x444f5123, /// 0x2c0
			 0xd12c5287, /// 0x2c4
			 0xc0833c65, /// 0x2c8
			 0xfbb0de05, /// 0x2cc
			 0x09fe7fd9, /// 0x2d0
			 0x2d733838, /// 0x2d4
			 0xf64681fc, /// 0x2d8
			 0x00bbed20, /// 0x2dc
			 0x886ce031, /// 0x2e0
			 0x148e4f64, /// 0x2e4
			 0x320f4034, /// 0x2e8
			 0x7e9eb818, /// 0x2ec
			 0x5ec3cbc2, /// 0x2f0
			 0x77be9801, /// 0x2f4
			 0x7bcd061d, /// 0x2f8
			 0xc35b5f92, /// 0x2fc
			 0x6f2dd1a3, /// 0x300
			 0xb3b3ef39, /// 0x304
			 0x045393c5, /// 0x308
			 0x5d83d2bb, /// 0x30c
			 0x868b028f, /// 0x310
			 0x608bc411, /// 0x314
			 0xeef0a30b, /// 0x318
			 0x4834f584, /// 0x31c
			 0x5515d440, /// 0x320
			 0xcff1aed3, /// 0x324
			 0x83d37290, /// 0x328
			 0x946327c5, /// 0x32c
			 0xf92272b9, /// 0x330
			 0x15b4a5f2, /// 0x334
			 0xed2a8835, /// 0x338
			 0xdf27bb3f, /// 0x33c
			 0x6058639a, /// 0x340
			 0x0478a5c5, /// 0x344
			 0x70415f37, /// 0x348
			 0xb5f16b08, /// 0x34c
			 0x50fc10ae, /// 0x350
			 0x2e7f3eac, /// 0x354
			 0x1f1dfc4a, /// 0x358
			 0x04dd7aa6, /// 0x35c
			 0xb2b9ec15, /// 0x360
			 0x11beda90, /// 0x364
			 0xb6eac959, /// 0x368
			 0x31f9b562, /// 0x36c
			 0x92d3f827, /// 0x370
			 0xb2bc3d97, /// 0x374
			 0x1a1930f0, /// 0x378
			 0x532e2de0, /// 0x37c
			 0x4fe2afcc, /// 0x380
			 0x1420944d, /// 0x384
			 0xf3c5c7b8, /// 0x388
			 0xf2dcc0b4, /// 0x38c
			 0x258fdc32, /// 0x390
			 0xb8ff114c, /// 0x394
			 0x5c7d2872, /// 0x398
			 0x39d0b6aa, /// 0x39c
			 0x014eee9a, /// 0x3a0
			 0x99f18fda, /// 0x3a4
			 0xd70c05e3, /// 0x3a8
			 0x693b79f7, /// 0x3ac
			 0xbe62948f, /// 0x3b0
			 0xc5b9a188, /// 0x3b4
			 0x64522ec1, /// 0x3b8
			 0x48ada594, /// 0x3bc
			 0x817398b0, /// 0x3c0
			 0x60b6ff8b, /// 0x3c4
			 0x8cb35fab, /// 0x3c8
			 0xc5df2c0c, /// 0x3cc
			 0x47841e26, /// 0x3d0
			 0xebf6b642, /// 0x3d4
			 0xa24c17d5, /// 0x3d8
			 0xe6689903, /// 0x3dc
			 0xd0a7980f, /// 0x3e0
			 0xee12850b, /// 0x3e4
			 0xc1ebe538, /// 0x3e8
			 0x1a614d80, /// 0x3ec
			 0xaf1fcdeb, /// 0x3f0
			 0xeb3bf69c, /// 0x3f4
			 0x25b0e362, /// 0x3f8
			 0x0326d288, /// 0x3fc
			 0x3b3c39a6, /// 0x400
			 0x5d418e76, /// 0x404
			 0x5f295882, /// 0x408
			 0xdd075681, /// 0x40c
			 0xe508e911, /// 0x410
			 0x356c3375, /// 0x414
			 0xff5d0b52, /// 0x418
			 0xd4dad4ef, /// 0x41c
			 0xc0986d1a, /// 0x420
			 0xb7aa6d18, /// 0x424
			 0x72d827ed, /// 0x428
			 0x097e3b88, /// 0x42c
			 0x34b6d890, /// 0x430
			 0xf2941286, /// 0x434
			 0x2f31e89e, /// 0x438
			 0x13977551, /// 0x43c
			 0x5d1be565, /// 0x440
			 0x7b00b92a, /// 0x444
			 0x156f45dc, /// 0x448
			 0x6254e9e7, /// 0x44c
			 0xe1632214, /// 0x450
			 0xa0cdae8b, /// 0x454
			 0x2f988426, /// 0x458
			 0x6efb9bbc, /// 0x45c
			 0xabe68361, /// 0x460
			 0x262654c0, /// 0x464
			 0xf87eb7d1, /// 0x468
			 0x599eb1a0, /// 0x46c
			 0x30ab96d8, /// 0x470
			 0x8724f9d3, /// 0x474
			 0x81b4cddb, /// 0x478
			 0xd29af06c, /// 0x47c
			 0xfba0ae51, /// 0x480
			 0x8e0425b1, /// 0x484
			 0xdb25da3d, /// 0x488
			 0x5b9e7d0b, /// 0x48c
			 0xc1d34d7c, /// 0x490
			 0xc17151e7, /// 0x494
			 0xf617580b, /// 0x498
			 0xcf13c198, /// 0x49c
			 0x205af8a8, /// 0x4a0
			 0xf7f5b02b, /// 0x4a4
			 0x6b7876bd, /// 0x4a8
			 0x2a2060cf, /// 0x4ac
			 0x33fe2cc5, /// 0x4b0
			 0x5c0e1bd6, /// 0x4b4
			 0x2422547c, /// 0x4b8
			 0xe5682447, /// 0x4bc
			 0x28d8aae7, /// 0x4c0
			 0xb5736cae, /// 0x4c4
			 0xa1ffd08b, /// 0x4c8
			 0x5ca27393, /// 0x4cc
			 0x9f819d85, /// 0x4d0
			 0xbabfde7c, /// 0x4d4
			 0x34a9c26c, /// 0x4d8
			 0x1ce6422a, /// 0x4dc
			 0x99dbdb22, /// 0x4e0
			 0xc12a0e1f, /// 0x4e4
			 0x94b9a17e, /// 0x4e8
			 0xb781f763, /// 0x4ec
			 0x97973197, /// 0x4f0
			 0xe929912b, /// 0x4f4
			 0xcd3bc3a7, /// 0x4f8
			 0x960a3010, /// 0x4fc
			 0xa993ad88, /// 0x500
			 0xff075687, /// 0x504
			 0xba00a48b, /// 0x508
			 0x46e31a43, /// 0x50c
			 0x630b6e39, /// 0x510
			 0xbb96d28b, /// 0x514
			 0x98ff49b4, /// 0x518
			 0xe1673009, /// 0x51c
			 0xfc6844aa, /// 0x520
			 0x42e452be, /// 0x524
			 0xab9e2dba, /// 0x528
			 0xa2112334, /// 0x52c
			 0xbafdbcfa, /// 0x530
			 0x41c455c6, /// 0x534
			 0x1c2f6301, /// 0x538
			 0x5f9c57a0, /// 0x53c
			 0x6ac87aa1, /// 0x540
			 0xc35f68c4, /// 0x544
			 0xe4cd11fe, /// 0x548
			 0xd4ee1600, /// 0x54c
			 0x6ee4ce04, /// 0x550
			 0xa3a3b572, /// 0x554
			 0x4d0846c1, /// 0x558
			 0xc906962c, /// 0x55c
			 0xc27dab2a, /// 0x560
			 0xcdbeb500, /// 0x564
			 0x8055bd2e, /// 0x568
			 0x37f3ef06, /// 0x56c
			 0x24062277, /// 0x570
			 0x5fea280d, /// 0x574
			 0xada38c30, /// 0x578
			 0x64ee7aec, /// 0x57c
			 0x03172ea6, /// 0x580
			 0x4b43bf0f, /// 0x584
			 0xe889abfa, /// 0x588
			 0x7cdd0adf, /// 0x58c
			 0x09720c81, /// 0x590
			 0x36a1d07d, /// 0x594
			 0xea320baf, /// 0x598
			 0x20d4be1c, /// 0x59c
			 0x9df78d04, /// 0x5a0
			 0x2ec887c0, /// 0x5a4
			 0x9a4a3b31, /// 0x5a8
			 0x9730f839, /// 0x5ac
			 0xcfbc4928, /// 0x5b0
			 0x038873f7, /// 0x5b4
			 0x319e58ec, /// 0x5b8
			 0xbe0ee12f, /// 0x5bc
			 0x22ce483f, /// 0x5c0
			 0x0e13633b, /// 0x5c4
			 0x0824463d, /// 0x5c8
			 0x041c06a2, /// 0x5cc
			 0xb04bcdb7, /// 0x5d0
			 0xb9ad0b67, /// 0x5d4
			 0xf81ba8e0, /// 0x5d8
			 0x703cc99e, /// 0x5dc
			 0x2155e111, /// 0x5e0
			 0x0a4c90fa, /// 0x5e4
			 0x4c82f012, /// 0x5e8
			 0x8932f302, /// 0x5ec
			 0x28b3d577, /// 0x5f0
			 0xc85592ae, /// 0x5f4
			 0x8e2e7324, /// 0x5f8
			 0x1141b836, /// 0x5fc
			 0x72c96d32, /// 0x600
			 0x3046298e, /// 0x604
			 0x8e654a02, /// 0x608
			 0xbd836e35, /// 0x60c
			 0x7e6d179f, /// 0x610
			 0xa8ad3623, /// 0x614
			 0x9646b2a4, /// 0x618
			 0x6f1c0728, /// 0x61c
			 0x26839978, /// 0x620
			 0x21bb4e92, /// 0x624
			 0x58bbe136, /// 0x628
			 0x878c0340, /// 0x62c
			 0x1796697a, /// 0x630
			 0x4859ecba, /// 0x634
			 0x690982b7, /// 0x638
			 0xd1114fa7, /// 0x63c
			 0x2d193d8e, /// 0x640
			 0x7806cb17, /// 0x644
			 0xc0c7393e, /// 0x648
			 0xe7bdf291, /// 0x64c
			 0xc66f9d4a, /// 0x650
			 0xad3391ae, /// 0x654
			 0x4466ff3c, /// 0x658
			 0x740a35b0, /// 0x65c
			 0xc50547cd, /// 0x660
			 0xadb456cf, /// 0x664
			 0xc742faca, /// 0x668
			 0x17da6f20, /// 0x66c
			 0x08a4dca9, /// 0x670
			 0xee4ad270, /// 0x674
			 0xf833931e, /// 0x678
			 0xcd36b5d8, /// 0x67c
			 0x30b83b37, /// 0x680
			 0x1083a925, /// 0x684
			 0xa1734703, /// 0x688
			 0xcec8b91b, /// 0x68c
			 0xd2f6dca3, /// 0x690
			 0x4e6f71ae, /// 0x694
			 0xb948af5c, /// 0x698
			 0xb77d5635, /// 0x69c
			 0xa26ec423, /// 0x6a0
			 0x41bea608, /// 0x6a4
			 0xa09b91fd, /// 0x6a8
			 0x4ef5c961, /// 0x6ac
			 0xa18970f3, /// 0x6b0
			 0xb9d508c8, /// 0x6b4
			 0x6c557e1f, /// 0x6b8
			 0x4d3c2eee, /// 0x6bc
			 0x3a3705e2, /// 0x6c0
			 0x4aa9745c, /// 0x6c4
			 0x112b798d, /// 0x6c8
			 0x037b3cd0, /// 0x6cc
			 0x467ed26a, /// 0x6d0
			 0xb9ce19ca, /// 0x6d4
			 0x8c167ac1, /// 0x6d8
			 0x24a743c5, /// 0x6dc
			 0x51105199, /// 0x6e0
			 0x63c0870e, /// 0x6e4
			 0xcf6a18e2, /// 0x6e8
			 0x55be077e, /// 0x6ec
			 0x0bdc10d8, /// 0x6f0
			 0x131dda04, /// 0x6f4
			 0xe1ea5720, /// 0x6f8
			 0xe280f43b, /// 0x6fc
			 0x12f02634, /// 0x700
			 0xe777e00e, /// 0x704
			 0x0011de50, /// 0x708
			 0xb09e86f4, /// 0x70c
			 0x486422d4, /// 0x710
			 0x64122bbf, /// 0x714
			 0x1cecc625, /// 0x718
			 0x0ce2207d, /// 0x71c
			 0x05ad1a49, /// 0x720
			 0xddcef7ad, /// 0x724
			 0x59044241, /// 0x728
			 0xab576e9c, /// 0x72c
			 0x619050fe, /// 0x730
			 0x39acc379, /// 0x734
			 0x43ae3002, /// 0x738
			 0xdefee6e1, /// 0x73c
			 0xd71fdbb2, /// 0x740
			 0xd2c0abfa, /// 0x744
			 0xedd1a3c9, /// 0x748
			 0xcaa26d1c, /// 0x74c
			 0x9c46359a, /// 0x750
			 0x1fb05522, /// 0x754
			 0xed1918b0, /// 0x758
			 0x06b54f23, /// 0x75c
			 0xfc0373a6, /// 0x760
			 0x553d83ef, /// 0x764
			 0x96dec491, /// 0x768
			 0xe1051747, /// 0x76c
			 0xe205cfe9, /// 0x770
			 0x4a6f7373, /// 0x774
			 0x41953377, /// 0x778
			 0xf84c4aab, /// 0x77c
			 0xd3c1b4b6, /// 0x780
			 0x0289faef, /// 0x784
			 0xb31d5298, /// 0x788
			 0x3ba92e1b, /// 0x78c
			 0xdf95c014, /// 0x790
			 0x6b80622e, /// 0x794
			 0x4803b42a, /// 0x798
			 0xf941f165, /// 0x79c
			 0x03fdfa66, /// 0x7a0
			 0xb591b978, /// 0x7a4
			 0x1cde43fb, /// 0x7a8
			 0x36cb9232, /// 0x7ac
			 0x693ef7a5, /// 0x7b0
			 0xd4840866, /// 0x7b4
			 0x5fc1bcdf, /// 0x7b8
			 0x4ccf3ad9, /// 0x7bc
			 0xb6270537, /// 0x7c0
			 0x76beea79, /// 0x7c4
			 0xf2c98f21, /// 0x7c8
			 0xfdc5f50f, /// 0x7cc
			 0xd6385ca9, /// 0x7d0
			 0xf138dc5d, /// 0x7d4
			 0xb5d771eb, /// 0x7d8
			 0x4e7a4eae, /// 0x7dc
			 0xb2419cc9, /// 0x7e0
			 0xd95488b6, /// 0x7e4
			 0x8b4d1fd7, /// 0x7e8
			 0xefaca10a, /// 0x7ec
			 0x7f29c64d, /// 0x7f0
			 0x4ad95969, /// 0x7f4
			 0x489655e8, /// 0x7f8
			 0xd8eddf9d, /// 0x7fc
			 0xea984a85, /// 0x800
			 0xed544eaf, /// 0x804
			 0x9975c390, /// 0x808
			 0x392cb940, /// 0x80c
			 0xb7b759e4, /// 0x810
			 0xf6caff21, /// 0x814
			 0x94c7eaf5, /// 0x818
			 0x1eb87614, /// 0x81c
			 0xafd0d9fe, /// 0x820
			 0xbfc16718, /// 0x824
			 0x9a79b0ae, /// 0x828
			 0x92fef236, /// 0x82c
			 0x50758116, /// 0x830
			 0x8be9f89c, /// 0x834
			 0xe9bc979a, /// 0x838
			 0x28030132, /// 0x83c
			 0x24ee8590, /// 0x840
			 0xea0d3f28, /// 0x844
			 0x5e350ace, /// 0x848
			 0x99b14888, /// 0x84c
			 0x6b149281, /// 0x850
			 0x631bcf39, /// 0x854
			 0x68d35383, /// 0x858
			 0xf0c83623, /// 0x85c
			 0x7a780a5a, /// 0x860
			 0x891d98f2, /// 0x864
			 0xf27959ac, /// 0x868
			 0xcc58c2f3, /// 0x86c
			 0x0b5a7901, /// 0x870
			 0x4e230a53, /// 0x874
			 0xf554b145, /// 0x878
			 0xc181fa5d, /// 0x87c
			 0x79d41972, /// 0x880
			 0xd2d1f779, /// 0x884
			 0xa19f5ab6, /// 0x888
			 0x975f1c1e, /// 0x88c
			 0x24c786e8, /// 0x890
			 0x0b42565a, /// 0x894
			 0x6d9980f7, /// 0x898
			 0x181f41e4, /// 0x89c
			 0x6cf2861e, /// 0x8a0
			 0x21f1ecb5, /// 0x8a4
			 0x5c03e6f6, /// 0x8a8
			 0x4bae3058, /// 0x8ac
			 0xece3ac30, /// 0x8b0
			 0x665aa794, /// 0x8b4
			 0xf4e3cb3c, /// 0x8b8
			 0xb9c1a1c5, /// 0x8bc
			 0xcf9921db, /// 0x8c0
			 0x157d7df0, /// 0x8c4
			 0x18653e08, /// 0x8c8
			 0x58052386, /// 0x8cc
			 0x24a2d6bf, /// 0x8d0
			 0x94bb884e, /// 0x8d4
			 0x0d661cff, /// 0x8d8
			 0x98a262c5, /// 0x8dc
			 0xbabe9624, /// 0x8e0
			 0x81176180, /// 0x8e4
			 0x2303e645, /// 0x8e8
			 0x61e1fe88, /// 0x8ec
			 0x5ac1e441, /// 0x8f0
			 0xb63689ba, /// 0x8f4
			 0x10e84e84, /// 0x8f8
			 0xf9d1c0e6, /// 0x8fc
			 0x0168a23b, /// 0x900
			 0x91ff664d, /// 0x904
			 0xa4f16530, /// 0x908
			 0x31747ae0, /// 0x90c
			 0xc563a546, /// 0x910
			 0xd5f01de7, /// 0x914
			 0xbc82ae5d, /// 0x918
			 0x8f859d2d, /// 0x91c
			 0xe9dc5b2e, /// 0x920
			 0xc06e5127, /// 0x924
			 0xb00929f1, /// 0x928
			 0xf4fc9deb, /// 0x92c
			 0x51e91dac, /// 0x930
			 0x843b6c51, /// 0x934
			 0xedaee796, /// 0x938
			 0xf06eca06, /// 0x93c
			 0x3f06b561, /// 0x940
			 0xa10c1d31, /// 0x944
			 0xf7007224, /// 0x948
			 0x7d871663, /// 0x94c
			 0x70be630c, /// 0x950
			 0x9202e074, /// 0x954
			 0x5cc8796c, /// 0x958
			 0x1701ee16, /// 0x95c
			 0x98e3c900, /// 0x960
			 0xb2f908da, /// 0x964
			 0x18ae810a, /// 0x968
			 0x87860142, /// 0x96c
			 0x8b045376, /// 0x970
			 0x28bc7f68, /// 0x974
			 0x13c3795e, /// 0x978
			 0x2c9a6860, /// 0x97c
			 0x6e48f63e, /// 0x980
			 0x0276aad3, /// 0x984
			 0xd1b06436, /// 0x988
			 0x2fa391a1, /// 0x98c
			 0xd2aad447, /// 0x990
			 0x2658843c, /// 0x994
			 0x48d92517, /// 0x998
			 0xf8d739c3, /// 0x99c
			 0x7c9fe814, /// 0x9a0
			 0x07812e6f, /// 0x9a4
			 0x66ef6df8, /// 0x9a8
			 0xb5f43432, /// 0x9ac
			 0x793ad5d9, /// 0x9b0
			 0xe154353c, /// 0x9b4
			 0x65a8cd53, /// 0x9b8
			 0x20783cf6, /// 0x9bc
			 0x0b7b5379, /// 0x9c0
			 0x0226253e, /// 0x9c4
			 0xd8b6347d, /// 0x9c8
			 0x72eae8b5, /// 0x9cc
			 0x6c491868, /// 0x9d0
			 0xfb8b8711, /// 0x9d4
			 0x41cd715e, /// 0x9d8
			 0x69ce5e6e, /// 0x9dc
			 0xce4b002a, /// 0x9e0
			 0xb55301be, /// 0x9e4
			 0x515babec, /// 0x9e8
			 0x4aaf8bd7, /// 0x9ec
			 0xc2c06fd5, /// 0x9f0
			 0xcd607c86, /// 0x9f4
			 0x15d24ba4, /// 0x9f8
			 0xb5620899, /// 0x9fc
			 0xbd1c3f8c, /// 0xa00
			 0xda62eb08, /// 0xa04
			 0xd1bbd274, /// 0xa08
			 0x0a3dd996, /// 0xa0c
			 0x2a505dc0, /// 0xa10
			 0x41d95731, /// 0xa14
			 0x9ddefdf3, /// 0xa18
			 0xd38d0ccd, /// 0xa1c
			 0x29728799, /// 0xa20
			 0xcb109b0d, /// 0xa24
			 0x838228a4, /// 0xa28
			 0xe4d53292, /// 0xa2c
			 0x4ccf78f4, /// 0xa30
			 0xbc205f2c, /// 0xa34
			 0xbc3cb3c0, /// 0xa38
			 0xcbd284ba, /// 0xa3c
			 0x75132b37, /// 0xa40
			 0x293d1824, /// 0xa44
			 0xc1d039c4, /// 0xa48
			 0xfcd10648, /// 0xa4c
			 0xf02bb5ca, /// 0xa50
			 0x51757e88, /// 0xa54
			 0x760ee43a, /// 0xa58
			 0x3bc74d43, /// 0xa5c
			 0x1f79c667, /// 0xa60
			 0x5f48426f, /// 0xa64
			 0x6a29825e, /// 0xa68
			 0xd345aa1b, /// 0xa6c
			 0xdfe8fc99, /// 0xa70
			 0x4b93eb83, /// 0xa74
			 0xafcc73b9, /// 0xa78
			 0x5d5243d8, /// 0xa7c
			 0x8e1521fd, /// 0xa80
			 0x08e55088, /// 0xa84
			 0xa0caebe2, /// 0xa88
			 0x945487b1, /// 0xa8c
			 0x588a4c19, /// 0xa90
			 0x8b509024, /// 0xa94
			 0x609fcb27, /// 0xa98
			 0x64862bac, /// 0xa9c
			 0x72b31edc, /// 0xaa0
			 0xd6571a7b, /// 0xaa4
			 0xebae6eaf, /// 0xaa8
			 0x598b7abb, /// 0xaac
			 0x4b8bdb89, /// 0xab0
			 0x95a89328, /// 0xab4
			 0x341405df, /// 0xab8
			 0xc332a0f3, /// 0xabc
			 0xe120d890, /// 0xac0
			 0x98f05669, /// 0xac4
			 0x13ebf299, /// 0xac8
			 0x32647e2d, /// 0xacc
			 0xa9a2b8b7, /// 0xad0
			 0x13a603f4, /// 0xad4
			 0x2fd65017, /// 0xad8
			 0x6f63342f, /// 0xadc
			 0xa83be6d4, /// 0xae0
			 0x92b6972a, /// 0xae4
			 0xcee3c593, /// 0xae8
			 0x53c4a679, /// 0xaec
			 0xcc85426e, /// 0xaf0
			 0xca006849, /// 0xaf4
			 0x59e78d63, /// 0xaf8
			 0xae3b899d, /// 0xafc
			 0x74488aa1, /// 0xb00
			 0x4e24b0ef, /// 0xb04
			 0x763d31a4, /// 0xb08
			 0x7de6be7c, /// 0xb0c
			 0x23f1df34, /// 0xb10
			 0xab7d6175, /// 0xb14
			 0x40cb73d4, /// 0xb18
			 0xc05d3b8a, /// 0xb1c
			 0x25926973, /// 0xb20
			 0xe010f592, /// 0xb24
			 0x59272263, /// 0xb28
			 0x1d3ddaae, /// 0xb2c
			 0x5c22dcae, /// 0xb30
			 0x14fbdc6c, /// 0xb34
			 0xa2824be0, /// 0xb38
			 0x48f20e97, /// 0xb3c
			 0x7e679f50, /// 0xb40
			 0x708a254c, /// 0xb44
			 0xeb687670, /// 0xb48
			 0x9aa4bee0, /// 0xb4c
			 0xa6bcdf02, /// 0xb50
			 0x181ec2b2, /// 0xb54
			 0x6369e6cd, /// 0xb58
			 0x6f34a3ef, /// 0xb5c
			 0xa6ddc43b, /// 0xb60
			 0x8fb33947, /// 0xb64
			 0x91ec6038, /// 0xb68
			 0x64d76caa, /// 0xb6c
			 0x4ff8d355, /// 0xb70
			 0x7c833aed, /// 0xb74
			 0xbbf963d1, /// 0xb78
			 0x63622375, /// 0xb7c
			 0xfb424525, /// 0xb80
			 0x3176a697, /// 0xb84
			 0x437bf263, /// 0xb88
			 0x59c00872, /// 0xb8c
			 0x96b1d4c6, /// 0xb90
			 0x10e88c72, /// 0xb94
			 0xe81d6c99, /// 0xb98
			 0x18d16ac7, /// 0xb9c
			 0x00723040, /// 0xba0
			 0x7beba17e, /// 0xba4
			 0x354dac6e, /// 0xba8
			 0x608ae759, /// 0xbac
			 0x0292d7e8, /// 0xbb0
			 0xf0a22ac9, /// 0xbb4
			 0xd63a2f62, /// 0xbb8
			 0xc1f8f140, /// 0xbbc
			 0x45b93704, /// 0xbc0
			 0xedcd9094, /// 0xbc4
			 0xd910ccb4, /// 0xbc8
			 0xc7b6cc26, /// 0xbcc
			 0xef4a26b1, /// 0xbd0
			 0xf3fcff47, /// 0xbd4
			 0x2a798cdf, /// 0xbd8
			 0x5e322153, /// 0xbdc
			 0x23210d77, /// 0xbe0
			 0x1a540ebe, /// 0xbe4
			 0x97e44a88, /// 0xbe8
			 0x666e931c, /// 0xbec
			 0xf922dcbc, /// 0xbf0
			 0xc665da2a, /// 0xbf4
			 0x181a2dcf, /// 0xbf8
			 0xb46fc21a, /// 0xbfc
			 0xf3ecf38a, /// 0xc00
			 0x6eafaa6b, /// 0xc04
			 0x54050686, /// 0xc08
			 0xbd764c53, /// 0xc0c
			 0xafc54d8d, /// 0xc10
			 0x4351b97e, /// 0xc14
			 0xbb781a6c, /// 0xc18
			 0x79d6814a, /// 0xc1c
			 0x7f48b3d7, /// 0xc20
			 0x599fa352, /// 0xc24
			 0x45e2d603, /// 0xc28
			 0xbfda4e88, /// 0xc2c
			 0x8be79f44, /// 0xc30
			 0x20c05b65, /// 0xc34
			 0xe1bbc02a, /// 0xc38
			 0x7b39bbcc, /// 0xc3c
			 0xe75861eb, /// 0xc40
			 0xee1c1edd, /// 0xc44
			 0x0d8d63e0, /// 0xc48
			 0xc1d4de79, /// 0xc4c
			 0xf40733cc, /// 0xc50
			 0x1a44d486, /// 0xc54
			 0x57a6114d, /// 0xc58
			 0x3e59f120, /// 0xc5c
			 0x88f5f7f0, /// 0xc60
			 0xa6f85991, /// 0xc64
			 0x5fd93758, /// 0xc68
			 0xd789dfb4, /// 0xc6c
			 0x21b688ff, /// 0xc70
			 0xf8422f4b, /// 0xc74
			 0x514f9814, /// 0xc78
			 0x0b68e1d1, /// 0xc7c
			 0x5fc409c8, /// 0xc80
			 0x1c6c7d53, /// 0xc84
			 0x9947d7c2, /// 0xc88
			 0x47b28fc5, /// 0xc8c
			 0x1efd0511, /// 0xc90
			 0x682dc60d, /// 0xc94
			 0x3dae80a1, /// 0xc98
			 0x43cf3c10, /// 0xc9c
			 0x4b6b0366, /// 0xca0
			 0x183a5d38, /// 0xca4
			 0x32bd9c9b, /// 0xca8
			 0xea05dc36, /// 0xcac
			 0x2b2ddd3d, /// 0xcb0
			 0x8e8509b8, /// 0xcb4
			 0x53581987, /// 0xcb8
			 0xa5d1a1e3, /// 0xcbc
			 0x464875ea, /// 0xcc0
			 0x8354c92d, /// 0xcc4
			 0xc567cedb, /// 0xcc8
			 0x449ea3ff, /// 0xccc
			 0xb4b75a59, /// 0xcd0
			 0xf16ca9fa, /// 0xcd4
			 0x46efc446, /// 0xcd8
			 0x4bada257, /// 0xcdc
			 0x26ff65aa, /// 0xce0
			 0x6bc88b60, /// 0xce4
			 0xb022cdb2, /// 0xce8
			 0x6d0d75d9, /// 0xcec
			 0xc3626498, /// 0xcf0
			 0x8bdcaada, /// 0xcf4
			 0x8bf00768, /// 0xcf8
			 0xa9e3728b, /// 0xcfc
			 0x6c8b1307, /// 0xd00
			 0x1bd90f3b, /// 0xd04
			 0xfc6a9499, /// 0xd08
			 0x4d414a81, /// 0xd0c
			 0x26ab3146, /// 0xd10
			 0xae7be5ab, /// 0xd14
			 0x65e1654a, /// 0xd18
			 0x659c42ee, /// 0xd1c
			 0x39e30918, /// 0xd20
			 0xfd949925, /// 0xd24
			 0xf160646d, /// 0xd28
			 0x273007c1, /// 0xd2c
			 0x1d9fd765, /// 0xd30
			 0x30ea8687, /// 0xd34
			 0x66a25b86, /// 0xd38
			 0xa69eb06b, /// 0xd3c
			 0x88dc3f34, /// 0xd40
			 0x2e198b8a, /// 0xd44
			 0x6881f61b, /// 0xd48
			 0x5f2bc205, /// 0xd4c
			 0x2dc6d6c1, /// 0xd50
			 0xd3e01c43, /// 0xd54
			 0x84d67ca4, /// 0xd58
			 0x1cfa383f, /// 0xd5c
			 0xc0571b6c, /// 0xd60
			 0x01f29094, /// 0xd64
			 0x21766d57, /// 0xd68
			 0x59fed5cd, /// 0xd6c
			 0x69288c28, /// 0xd70
			 0xf50688ab, /// 0xd74
			 0x7b6e1ac5, /// 0xd78
			 0x42ffc453, /// 0xd7c
			 0xa559209f, /// 0xd80
			 0x90aae219, /// 0xd84
			 0x433ee656, /// 0xd88
			 0xdff7b1e4, /// 0xd8c
			 0x3a390748, /// 0xd90
			 0x55dfe8d8, /// 0xd94
			 0x9755b134, /// 0xd98
			 0x2091e578, /// 0xd9c
			 0xc81261d1, /// 0xda0
			 0x63a7227b, /// 0xda4
			 0xd27ffe49, /// 0xda8
			 0x80fce72d, /// 0xdac
			 0xe10e5a55, /// 0xdb0
			 0x377a5c88, /// 0xdb4
			 0x733b8d56, /// 0xdb8
			 0xb5bf4006, /// 0xdbc
			 0x1d069910, /// 0xdc0
			 0xa92fe93b, /// 0xdc4
			 0x70382d84, /// 0xdc8
			 0xd3488049, /// 0xdcc
			 0xd84c923d, /// 0xdd0
			 0x0c950dae, /// 0xdd4
			 0x54dddcb5, /// 0xdd8
			 0x0c867883, /// 0xddc
			 0x637697d4, /// 0xde0
			 0xfc499ba4, /// 0xde4
			 0x84f195ca, /// 0xde8
			 0x3a2d95f1, /// 0xdec
			 0x79fe52a5, /// 0xdf0
			 0x6a44e475, /// 0xdf4
			 0xcf951a2b, /// 0xdf8
			 0xf56148e9, /// 0xdfc
			 0xe4cf5a27, /// 0xe00
			 0xccc61606, /// 0xe04
			 0x8c85f374, /// 0xe08
			 0x7d265194, /// 0xe0c
			 0x15a25f67, /// 0xe10
			 0x4154657d, /// 0xe14
			 0x1829a812, /// 0xe18
			 0xdda00d8d, /// 0xe1c
			 0x592eba34, /// 0xe20
			 0xa84deae6, /// 0xe24
			 0x308ce2e9, /// 0xe28
			 0xb33410c8, /// 0xe2c
			 0xea1c80eb, /// 0xe30
			 0x654c89bf, /// 0xe34
			 0x36d032ce, /// 0xe38
			 0x2c6f3d58, /// 0xe3c
			 0xc1f24369, /// 0xe40
			 0x58e5b65e, /// 0xe44
			 0x2b0ab52f, /// 0xe48
			 0x1bfa91d3, /// 0xe4c
			 0x7649be36, /// 0xe50
			 0xd4ee9f6f, /// 0xe54
			 0x9b7ddaa4, /// 0xe58
			 0x3dd0f62a, /// 0xe5c
			 0x6db4d1b8, /// 0xe60
			 0xc5c3e968, /// 0xe64
			 0xbc802055, /// 0xe68
			 0xf25de8e8, /// 0xe6c
			 0x103f1eb3, /// 0xe70
			 0x5808e70e, /// 0xe74
			 0x31e6fc0d, /// 0xe78
			 0x0d210a60, /// 0xe7c
			 0xd037d77d, /// 0xe80
			 0xb36f3b15, /// 0xe84
			 0xf64c5cf8, /// 0xe88
			 0xaca26acd, /// 0xe8c
			 0xea60d5c5, /// 0xe90
			 0x9983e762, /// 0xe94
			 0x795da402, /// 0xe98
			 0x2c6924c2, /// 0xe9c
			 0xf569c60f, /// 0xea0
			 0x9ef86718, /// 0xea4
			 0x038911bc, /// 0xea8
			 0x01bee32a, /// 0xeac
			 0x9784031a, /// 0xeb0
			 0x7762e241, /// 0xeb4
			 0xd819d0bf, /// 0xeb8
			 0xb7115971, /// 0xebc
			 0xed1d8c60, /// 0xec0
			 0x457bc3b5, /// 0xec4
			 0x6b23c4fe, /// 0xec8
			 0xf25009af, /// 0xecc
			 0x4d84a072, /// 0xed0
			 0x094d68ce, /// 0xed4
			 0xb869f2a3, /// 0xed8
			 0xf24b4d6e, /// 0xedc
			 0x10deb3c2, /// 0xee0
			 0xd6c7e320, /// 0xee4
			 0xf1a1686a, /// 0xee8
			 0x136bd26d, /// 0xeec
			 0x3b0981f2, /// 0xef0
			 0x83dee66e, /// 0xef4
			 0xf57e60dc, /// 0xef8
			 0x75e4213e, /// 0xefc
			 0xa8440908, /// 0xf00
			 0x8adf792c, /// 0xf04
			 0xeb53d33e, /// 0xf08
			 0x5042297d, /// 0xf0c
			 0x7d986af1, /// 0xf10
			 0x74487812, /// 0xf14
			 0x859ddf7f, /// 0xf18
			 0x38f3c201, /// 0xf1c
			 0x89fd19c1, /// 0xf20
			 0x249d999a, /// 0xf24
			 0x73ea3303, /// 0xf28
			 0x9c893090, /// 0xf2c
			 0x0d1b33d7, /// 0xf30
			 0x9165a1d6, /// 0xf34
			 0x54804321, /// 0xf38
			 0x2dacea2c, /// 0xf3c
			 0xaf71a92a, /// 0xf40
			 0x83132b2e, /// 0xf44
			 0x8a16e32b, /// 0xf48
			 0x4852e740, /// 0xf4c
			 0xe904659b, /// 0xf50
			 0x90e5b8e8, /// 0xf54
			 0x43fa7ad0, /// 0xf58
			 0x8dbf0a23, /// 0xf5c
			 0x761dc55e, /// 0xf60
			 0xf9f1146c, /// 0xf64
			 0x367a30af, /// 0xf68
			 0x8282ebea, /// 0xf6c
			 0x69adaf59, /// 0xf70
			 0x7e490954, /// 0xf74
			 0x3446963b, /// 0xf78
			 0xa4696323, /// 0xf7c
			 0x748d32fe, /// 0xf80
			 0xd7f305d2, /// 0xf84
			 0x755b120a, /// 0xf88
			 0x96352ccf, /// 0xf8c
			 0x7d0cbc80, /// 0xf90
			 0xa4ade895, /// 0xf94
			 0xa59654df, /// 0xf98
			 0x41b8f54f, /// 0xf9c
			 0x35881de4, /// 0xfa0
			 0xa839b2b5, /// 0xfa4
			 0x14b6632f, /// 0xfa8
			 0xe4abf42f, /// 0xfac
			 0x44b38b37, /// 0xfb0
			 0xedbc53ef, /// 0xfb4
			 0xdf1dc4e4, /// 0xfb8
			 0xc104a4b0, /// 0xfbc
			 0xcbf20006, /// 0xfc0
			 0xff769595, /// 0xfc4
			 0x9f8bba7e, /// 0xfc8
			 0x798b07ac, /// 0xfcc
			 0x2e2d287b, /// 0xfd0
			 0x274a8b88, /// 0xfd4
			 0x31acc6db, /// 0xfd8
			 0xaddee2ac, /// 0xfdc
			 0xe0e542d5, /// 0xfe0
			 0x0073c4bb, /// 0xfe4
			 0x8b43387a, /// 0xfe8
			 0xd73baece, /// 0xfec
			 0x2f1f7007, /// 0xff0
			 0x7f85d170, /// 0xff4
			 0xda4394f5, /// 0xff8
			 0x17fd85ad /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x5c9669ad, /// 0x0
			 0x063bf828, /// 0x4
			 0xd1362092, /// 0x8
			 0x58c17743, /// 0xc
			 0x1d7a6478, /// 0x10
			 0xe7e924df, /// 0x14
			 0x23ee6b20, /// 0x18
			 0x4d91a624, /// 0x1c
			 0x6c4cc826, /// 0x20
			 0x102a24cd, /// 0x24
			 0x4759ccb0, /// 0x28
			 0xef2c4337, /// 0x2c
			 0x69bcd3aa, /// 0x30
			 0x61ced4bf, /// 0x34
			 0x6eaf443d, /// 0x38
			 0xb02c005c, /// 0x3c
			 0xbad4b271, /// 0x40
			 0xd3555d04, /// 0x44
			 0x632a9f12, /// 0x48
			 0x8100e439, /// 0x4c
			 0x79a5c1c3, /// 0x50
			 0xb55ca6cb, /// 0x54
			 0xba5cfe1f, /// 0x58
			 0x4497eacb, /// 0x5c
			 0xcd5da806, /// 0x60
			 0xeacfd93b, /// 0x64
			 0x07a3512c, /// 0x68
			 0x7798f091, /// 0x6c
			 0x0a2b93fe, /// 0x70
			 0xe0a7de87, /// 0x74
			 0xb22c9462, /// 0x78
			 0xdbd5cc61, /// 0x7c
			 0xd13b9eb0, /// 0x80
			 0x4a587efb, /// 0x84
			 0x06ab4832, /// 0x88
			 0x3469775a, /// 0x8c
			 0x5575f5ef, /// 0x90
			 0x7f288aa2, /// 0x94
			 0x0b07cde0, /// 0x98
			 0x05b95932, /// 0x9c
			 0x7767faab, /// 0xa0
			 0x350ddc5f, /// 0xa4
			 0x1bc16122, /// 0xa8
			 0xd067c78c, /// 0xac
			 0x00709611, /// 0xb0
			 0xf1726693, /// 0xb4
			 0x5dc74711, /// 0xb8
			 0x681b2c46, /// 0xbc
			 0xbc676cee, /// 0xc0
			 0x5fcd53cb, /// 0xc4
			 0x0d695959, /// 0xc8
			 0x23d21c61, /// 0xcc
			 0x700e58be, /// 0xd0
			 0x4f1a044a, /// 0xd4
			 0x7d4f9ea7, /// 0xd8
			 0x8929c781, /// 0xdc
			 0x464248a6, /// 0xe0
			 0x17061811, /// 0xe4
			 0x2a2c69a4, /// 0xe8
			 0x27f84301, /// 0xec
			 0x55477448, /// 0xf0
			 0x42fe1660, /// 0xf4
			 0x93c85399, /// 0xf8
			 0x34df8cd2, /// 0xfc
			 0xc93f4d37, /// 0x100
			 0x75c68a4c, /// 0x104
			 0x218ae4c2, /// 0x108
			 0xbff2b6b3, /// 0x10c
			 0x6f5b8f23, /// 0x110
			 0x29955a3e, /// 0x114
			 0x6812e2dd, /// 0x118
			 0xb2fbf30c, /// 0x11c
			 0xa1a3d357, /// 0x120
			 0xd25d489c, /// 0x124
			 0x0ab3c84a, /// 0x128
			 0x1a7be2f1, /// 0x12c
			 0x67257752, /// 0x130
			 0x87a1da4b, /// 0x134
			 0x39256763, /// 0x138
			 0x8bcf8477, /// 0x13c
			 0x474c748f, /// 0x140
			 0xc1b6d07a, /// 0x144
			 0x2420eb3a, /// 0x148
			 0x9844dfe5, /// 0x14c
			 0x2b1a5a79, /// 0x150
			 0x22c68fca, /// 0x154
			 0x7492f224, /// 0x158
			 0x0b1a41d8, /// 0x15c
			 0x1a18ec01, /// 0x160
			 0x94510c66, /// 0x164
			 0x3fe46fe4, /// 0x168
			 0x4484fb1b, /// 0x16c
			 0xc7afbf47, /// 0x170
			 0x1ce9f7fe, /// 0x174
			 0x78fa5790, /// 0x178
			 0x38366630, /// 0x17c
			 0xcc71bd7d, /// 0x180
			 0x2f735286, /// 0x184
			 0xd4041213, /// 0x188
			 0xf43e7549, /// 0x18c
			 0x3f298572, /// 0x190
			 0xf38f05bf, /// 0x194
			 0x88fd18b0, /// 0x198
			 0x063f51e3, /// 0x19c
			 0x1db219cf, /// 0x1a0
			 0x02bc5f71, /// 0x1a4
			 0x1937601d, /// 0x1a8
			 0x6b900557, /// 0x1ac
			 0xe3562406, /// 0x1b0
			 0x38635e58, /// 0x1b4
			 0x6ad1fbca, /// 0x1b8
			 0x21ab49da, /// 0x1bc
			 0x545411bd, /// 0x1c0
			 0xb1c91a28, /// 0x1c4
			 0x1d76a70f, /// 0x1c8
			 0x0180d586, /// 0x1cc
			 0x3e56c595, /// 0x1d0
			 0x7075db6c, /// 0x1d4
			 0x65be605e, /// 0x1d8
			 0xee729d2a, /// 0x1dc
			 0xd19b423f, /// 0x1e0
			 0x62efd628, /// 0x1e4
			 0xbd7ae190, /// 0x1e8
			 0xf7549cd9, /// 0x1ec
			 0xb2a25cb2, /// 0x1f0
			 0x10dc4ab0, /// 0x1f4
			 0x836ed9b7, /// 0x1f8
			 0xd3459288, /// 0x1fc
			 0x618f9d33, /// 0x200
			 0x52494e03, /// 0x204
			 0x97ca7ef7, /// 0x208
			 0x7f8f43eb, /// 0x20c
			 0xeb8e53a4, /// 0x210
			 0xede3740f, /// 0x214
			 0x042ab449, /// 0x218
			 0x26b7d7c8, /// 0x21c
			 0xdf0b0941, /// 0x220
			 0x099568f6, /// 0x224
			 0xe99710c2, /// 0x228
			 0xee7eebd6, /// 0x22c
			 0x45ec683c, /// 0x230
			 0x52749108, /// 0x234
			 0x995d3473, /// 0x238
			 0x3461de54, /// 0x23c
			 0x52f94df4, /// 0x240
			 0x382f20e1, /// 0x244
			 0xd0f3329f, /// 0x248
			 0x70a3f4ee, /// 0x24c
			 0x88846cbe, /// 0x250
			 0xf5e07a2d, /// 0x254
			 0x2c2dd86b, /// 0x258
			 0x990a661b, /// 0x25c
			 0xd457beb6, /// 0x260
			 0x951da4ce, /// 0x264
			 0x29a4c2ee, /// 0x268
			 0x368f7afa, /// 0x26c
			 0xfeed4046, /// 0x270
			 0xa7612c5c, /// 0x274
			 0xe3e47ac0, /// 0x278
			 0x54456897, /// 0x27c
			 0x7603e15c, /// 0x280
			 0x255018b7, /// 0x284
			 0x164ee601, /// 0x288
			 0x95f94ce2, /// 0x28c
			 0x36686961, /// 0x290
			 0x8fa6b590, /// 0x294
			 0x814e69f4, /// 0x298
			 0x01d4954b, /// 0x29c
			 0x108edfa4, /// 0x2a0
			 0x4fd5c612, /// 0x2a4
			 0xf126f6a5, /// 0x2a8
			 0x75c784d1, /// 0x2ac
			 0x3f25fa0e, /// 0x2b0
			 0x7c5752a0, /// 0x2b4
			 0x2c7ebd58, /// 0x2b8
			 0xca6dd2a5, /// 0x2bc
			 0x50f2e175, /// 0x2c0
			 0x6f9877bc, /// 0x2c4
			 0xd240bee1, /// 0x2c8
			 0x79158363, /// 0x2cc
			 0x4263e18d, /// 0x2d0
			 0x40404b17, /// 0x2d4
			 0x0202503b, /// 0x2d8
			 0x18ed45e9, /// 0x2dc
			 0x6eefe4ec, /// 0x2e0
			 0xe4216593, /// 0x2e4
			 0x3ee8af16, /// 0x2e8
			 0x41222712, /// 0x2ec
			 0xba1c3dc9, /// 0x2f0
			 0xbeaf7ad7, /// 0x2f4
			 0xcc221329, /// 0x2f8
			 0x72f26c38, /// 0x2fc
			 0x894c913f, /// 0x300
			 0xd65c1dd9, /// 0x304
			 0x7a663bf0, /// 0x308
			 0x17b503f4, /// 0x30c
			 0x28b282c3, /// 0x310
			 0xc55d9f2b, /// 0x314
			 0x29b44ae7, /// 0x318
			 0xd7f25d38, /// 0x31c
			 0x02127699, /// 0x320
			 0x81be3027, /// 0x324
			 0x5cd4261a, /// 0x328
			 0xaebeff94, /// 0x32c
			 0x5795de0f, /// 0x330
			 0x268e28a8, /// 0x334
			 0x51ac2ccf, /// 0x338
			 0x6c8bb4be, /// 0x33c
			 0x47ded0e8, /// 0x340
			 0x59fe426d, /// 0x344
			 0xe5ab694f, /// 0x348
			 0xf7b0dace, /// 0x34c
			 0x8c8032df, /// 0x350
			 0x0f643548, /// 0x354
			 0x94549a6c, /// 0x358
			 0x8c824b5c, /// 0x35c
			 0x1dea597f, /// 0x360
			 0xa965fb97, /// 0x364
			 0xe158645c, /// 0x368
			 0x079083b4, /// 0x36c
			 0x670d6593, /// 0x370
			 0xa07966e8, /// 0x374
			 0x87734484, /// 0x378
			 0x9c14102d, /// 0x37c
			 0x0278fadf, /// 0x380
			 0x3a65ca28, /// 0x384
			 0x51bba496, /// 0x388
			 0xc0c2455d, /// 0x38c
			 0x6c224951, /// 0x390
			 0x3f451791, /// 0x394
			 0x2d4b07f8, /// 0x398
			 0x4c8d57cf, /// 0x39c
			 0x5fa0aae7, /// 0x3a0
			 0x53abbe6a, /// 0x3a4
			 0xc28636c7, /// 0x3a8
			 0x6e751ae7, /// 0x3ac
			 0xcb3f1853, /// 0x3b0
			 0xed37e720, /// 0x3b4
			 0x982318e8, /// 0x3b8
			 0xd2cf03d0, /// 0x3bc
			 0xf84a5bfb, /// 0x3c0
			 0x6b8505f3, /// 0x3c4
			 0x1af66a68, /// 0x3c8
			 0xe9bea37f, /// 0x3cc
			 0x08e5afa4, /// 0x3d0
			 0xc62149d3, /// 0x3d4
			 0x262ca345, /// 0x3d8
			 0x8fa3d608, /// 0x3dc
			 0xf41a22a3, /// 0x3e0
			 0xb994daf0, /// 0x3e4
			 0x4e429824, /// 0x3e8
			 0x9622740f, /// 0x3ec
			 0x3d10d47b, /// 0x3f0
			 0xa2977428, /// 0x3f4
			 0xb78f3197, /// 0x3f8
			 0x70a69774, /// 0x3fc
			 0x9453a090, /// 0x400
			 0xd276ff21, /// 0x404
			 0x26c04c02, /// 0x408
			 0xd0a09775, /// 0x40c
			 0x60522580, /// 0x410
			 0xc14cf12b, /// 0x414
			 0x2ec770c2, /// 0x418
			 0x31ffcd55, /// 0x41c
			 0xd31576d9, /// 0x420
			 0x67ecfa35, /// 0x424
			 0x14dd7ef3, /// 0x428
			 0x8c550319, /// 0x42c
			 0x18f616b9, /// 0x430
			 0x4562eb79, /// 0x434
			 0xbb3eca27, /// 0x438
			 0xed9112c4, /// 0x43c
			 0x1491cbc7, /// 0x440
			 0xa0fee404, /// 0x444
			 0x42bc96cf, /// 0x448
			 0x01fe5b0b, /// 0x44c
			 0x43cebdbd, /// 0x450
			 0xec4528a7, /// 0x454
			 0x6aa366fb, /// 0x458
			 0x380d3b3e, /// 0x45c
			 0x9b331cff, /// 0x460
			 0x43d4c135, /// 0x464
			 0x9b7be120, /// 0x468
			 0xb5e46d0e, /// 0x46c
			 0x016348c2, /// 0x470
			 0x022ec748, /// 0x474
			 0x8b190980, /// 0x478
			 0x2f5d5639, /// 0x47c
			 0x318d6e39, /// 0x480
			 0xce5ad340, /// 0x484
			 0x90cb0ef2, /// 0x488
			 0x9bb1d96d, /// 0x48c
			 0xb74e029a, /// 0x490
			 0xaa8b84c1, /// 0x494
			 0xa47703b0, /// 0x498
			 0x86dae52e, /// 0x49c
			 0x85b89ab9, /// 0x4a0
			 0xaeb9f43d, /// 0x4a4
			 0x4ff104e0, /// 0x4a8
			 0xd45fecfd, /// 0x4ac
			 0x5e76569c, /// 0x4b0
			 0x428ba218, /// 0x4b4
			 0x56772b36, /// 0x4b8
			 0x0117206b, /// 0x4bc
			 0x54e5e23d, /// 0x4c0
			 0xde1be603, /// 0x4c4
			 0xeafb1aa2, /// 0x4c8
			 0xe05e9066, /// 0x4cc
			 0xd502c685, /// 0x4d0
			 0xf7a97efe, /// 0x4d4
			 0xc069c62c, /// 0x4d8
			 0x32e11b05, /// 0x4dc
			 0x046a69df, /// 0x4e0
			 0xda4e64c6, /// 0x4e4
			 0xb6000016, /// 0x4e8
			 0x84c32aaf, /// 0x4ec
			 0xd40b07d6, /// 0x4f0
			 0xd8d8fa28, /// 0x4f4
			 0xeedebfd6, /// 0x4f8
			 0x48b035d5, /// 0x4fc
			 0x61a26e90, /// 0x500
			 0xbee2674a, /// 0x504
			 0xf9057529, /// 0x508
			 0xf48108eb, /// 0x50c
			 0x9140968e, /// 0x510
			 0x29a4cae6, /// 0x514
			 0x3bf83a1e, /// 0x518
			 0xda1cd44a, /// 0x51c
			 0x4bc36b45, /// 0x520
			 0xc01237e9, /// 0x524
			 0x0f41189f, /// 0x528
			 0xcc19454c, /// 0x52c
			 0x3840d377, /// 0x530
			 0x0d1ea2d5, /// 0x534
			 0x70435c26, /// 0x538
			 0x5fbbd5a4, /// 0x53c
			 0xefd315e1, /// 0x540
			 0xad81d637, /// 0x544
			 0xe7dd49eb, /// 0x548
			 0x88899ae3, /// 0x54c
			 0xfea441e5, /// 0x550
			 0xbd0b9bc8, /// 0x554
			 0xc2d6abe2, /// 0x558
			 0x0991a54e, /// 0x55c
			 0x07e50357, /// 0x560
			 0x76fd0b06, /// 0x564
			 0x0649d481, /// 0x568
			 0x703089f8, /// 0x56c
			 0xae558017, /// 0x570
			 0x7958f90e, /// 0x574
			 0x4a8c20cd, /// 0x578
			 0x9e3e9ad3, /// 0x57c
			 0xa39aec06, /// 0x580
			 0xb002f3bb, /// 0x584
			 0x263d29a9, /// 0x588
			 0xd71a319f, /// 0x58c
			 0x6fa847d9, /// 0x590
			 0x952b3664, /// 0x594
			 0x78cef982, /// 0x598
			 0x69e4d80d, /// 0x59c
			 0xc7b90db8, /// 0x5a0
			 0x9273d8a1, /// 0x5a4
			 0x9ac65214, /// 0x5a8
			 0x770bb09a, /// 0x5ac
			 0x0f41092e, /// 0x5b0
			 0x621bd8f1, /// 0x5b4
			 0x602ed303, /// 0x5b8
			 0xae3bd317, /// 0x5bc
			 0x5e2e37d3, /// 0x5c0
			 0xe0b4a9e4, /// 0x5c4
			 0x5219e011, /// 0x5c8
			 0x6e7cfea9, /// 0x5cc
			 0x0685e91d, /// 0x5d0
			 0x00078cd6, /// 0x5d4
			 0xff85652f, /// 0x5d8
			 0x51786087, /// 0x5dc
			 0xccc407c4, /// 0x5e0
			 0x2c1d4abb, /// 0x5e4
			 0xa075481d, /// 0x5e8
			 0x2d2e50d7, /// 0x5ec
			 0x928ce506, /// 0x5f0
			 0xbeed6e45, /// 0x5f4
			 0x12c05f2e, /// 0x5f8
			 0xedd35257, /// 0x5fc
			 0xfd7d72f7, /// 0x600
			 0xfe5edb2d, /// 0x604
			 0x9dafc32f, /// 0x608
			 0x3bb210e2, /// 0x60c
			 0xbfe226d6, /// 0x610
			 0x73748b67, /// 0x614
			 0xf1251680, /// 0x618
			 0x7257c996, /// 0x61c
			 0x34589191, /// 0x620
			 0x82ee165b, /// 0x624
			 0x88f2af8d, /// 0x628
			 0xb50e5c61, /// 0x62c
			 0xb670f318, /// 0x630
			 0xe804619b, /// 0x634
			 0x488d894b, /// 0x638
			 0x3fd568a7, /// 0x63c
			 0x04a625eb, /// 0x640
			 0xb2f198a0, /// 0x644
			 0xf730f597, /// 0x648
			 0xcb9bbb0c, /// 0x64c
			 0xfb774537, /// 0x650
			 0x05825136, /// 0x654
			 0xd30944c1, /// 0x658
			 0x77240e19, /// 0x65c
			 0x402d6538, /// 0x660
			 0xd440920f, /// 0x664
			 0x3eb2bd1c, /// 0x668
			 0x785ffdfe, /// 0x66c
			 0x8b3ce20d, /// 0x670
			 0x30edce28, /// 0x674
			 0xfc1e510d, /// 0x678
			 0xec210b4c, /// 0x67c
			 0x818e2500, /// 0x680
			 0x17c80a83, /// 0x684
			 0x34744d81, /// 0x688
			 0xb0e54684, /// 0x68c
			 0x1ae4b8b1, /// 0x690
			 0x931e6068, /// 0x694
			 0xd20e110d, /// 0x698
			 0xfd4f5fe0, /// 0x69c
			 0x4f518c9c, /// 0x6a0
			 0xbd65c01c, /// 0x6a4
			 0x0c800760, /// 0x6a8
			 0xe834632d, /// 0x6ac
			 0xc9ee3023, /// 0x6b0
			 0x4d44267f, /// 0x6b4
			 0xb22cca8e, /// 0x6b8
			 0x69363d09, /// 0x6bc
			 0x65e71747, /// 0x6c0
			 0xab5e61d0, /// 0x6c4
			 0x3fdcaa9e, /// 0x6c8
			 0xd74b1973, /// 0x6cc
			 0x975aa3b2, /// 0x6d0
			 0x80e7eeaa, /// 0x6d4
			 0xd1927b9d, /// 0x6d8
			 0x2294ca12, /// 0x6dc
			 0xaab4dd3c, /// 0x6e0
			 0x0e28185d, /// 0x6e4
			 0xdd7754cc, /// 0x6e8
			 0x0d33ff47, /// 0x6ec
			 0x887eb4d2, /// 0x6f0
			 0xb1e5e68f, /// 0x6f4
			 0xde849a85, /// 0x6f8
			 0x290a79c0, /// 0x6fc
			 0xa4e126e5, /// 0x700
			 0x41c99868, /// 0x704
			 0x176f7f7c, /// 0x708
			 0xa3e853fc, /// 0x70c
			 0x8cd69ff3, /// 0x710
			 0xd0ad74b5, /// 0x714
			 0xbe1f47d7, /// 0x718
			 0x16e023f5, /// 0x71c
			 0x6b01be33, /// 0x720
			 0x2798717b, /// 0x724
			 0x2e5889a7, /// 0x728
			 0x30ab7d5b, /// 0x72c
			 0x55908f2d, /// 0x730
			 0xba92503a, /// 0x734
			 0x9d8728b3, /// 0x738
			 0x7cea216e, /// 0x73c
			 0x097db439, /// 0x740
			 0xe7dd6ecc, /// 0x744
			 0xd03e141b, /// 0x748
			 0x5ba77d30, /// 0x74c
			 0xce78d764, /// 0x750
			 0x5ba1dee5, /// 0x754
			 0x45302e53, /// 0x758
			 0x71422ead, /// 0x75c
			 0x80e7cf3d, /// 0x760
			 0xe40a9983, /// 0x764
			 0x17137a09, /// 0x768
			 0xb22ff25d, /// 0x76c
			 0xaa236b96, /// 0x770
			 0x52f600f6, /// 0x774
			 0x189585bf, /// 0x778
			 0xc034f3b2, /// 0x77c
			 0x339e1f5b, /// 0x780
			 0xf5e64546, /// 0x784
			 0x926b17eb, /// 0x788
			 0xbb8004c3, /// 0x78c
			 0x983e613b, /// 0x790
			 0xa21b6cb9, /// 0x794
			 0xa332e232, /// 0x798
			 0x4e57e60c, /// 0x79c
			 0x6e74a39d, /// 0x7a0
			 0xe1d2ebde, /// 0x7a4
			 0xefb82efa, /// 0x7a8
			 0xc2db48e9, /// 0x7ac
			 0x458dffac, /// 0x7b0
			 0x8af21b17, /// 0x7b4
			 0x8be78f61, /// 0x7b8
			 0x3202fd8e, /// 0x7bc
			 0xd6c73b59, /// 0x7c0
			 0x74f09608, /// 0x7c4
			 0x122e7c3d, /// 0x7c8
			 0x6c69d33a, /// 0x7cc
			 0x17193751, /// 0x7d0
			 0xd8a22794, /// 0x7d4
			 0x9f2070b2, /// 0x7d8
			 0xf6d40dfc, /// 0x7dc
			 0x38db21e8, /// 0x7e0
			 0x63e52ba6, /// 0x7e4
			 0xefd77246, /// 0x7e8
			 0x598f34e9, /// 0x7ec
			 0xef25fd9b, /// 0x7f0
			 0x8d8c06f8, /// 0x7f4
			 0x6d25ccb9, /// 0x7f8
			 0xc6fa8560, /// 0x7fc
			 0xbea1945e, /// 0x800
			 0x8abafd1a, /// 0x804
			 0xff7c5d6e, /// 0x808
			 0x3a1a8bec, /// 0x80c
			 0xae8c21c1, /// 0x810
			 0xeb0019fc, /// 0x814
			 0x8fb4427b, /// 0x818
			 0xa3f765a8, /// 0x81c
			 0x06d1d7bd, /// 0x820
			 0x574bbca3, /// 0x824
			 0xf14000a7, /// 0x828
			 0x6c609c3d, /// 0x82c
			 0xf922ba71, /// 0x830
			 0xafd4a07d, /// 0x834
			 0xb46bd462, /// 0x838
			 0xd266641a, /// 0x83c
			 0x41c1c281, /// 0x840
			 0xc272cdd7, /// 0x844
			 0xcb42c814, /// 0x848
			 0x737522fa, /// 0x84c
			 0x5ac6341c, /// 0x850
			 0xcf91f68d, /// 0x854
			 0xde4e0e6d, /// 0x858
			 0x2c9009ea, /// 0x85c
			 0x3cb52a50, /// 0x860
			 0x8c240fcb, /// 0x864
			 0x906ca35e, /// 0x868
			 0x0b9838f2, /// 0x86c
			 0xf3bdf756, /// 0x870
			 0x02d9f02f, /// 0x874
			 0x05bc6f85, /// 0x878
			 0xf35198c0, /// 0x87c
			 0x2f1b8578, /// 0x880
			 0xe2312b92, /// 0x884
			 0x200bf63f, /// 0x888
			 0xa3f46113, /// 0x88c
			 0x7c4037cc, /// 0x890
			 0x65f9c803, /// 0x894
			 0x76738400, /// 0x898
			 0x0e7c7745, /// 0x89c
			 0x68a584e0, /// 0x8a0
			 0x49c9bb9c, /// 0x8a4
			 0x2447dc68, /// 0x8a8
			 0x960f078e, /// 0x8ac
			 0x2c78a8ad, /// 0x8b0
			 0xfbe9ac9b, /// 0x8b4
			 0x6e3fccf5, /// 0x8b8
			 0x13f6924c, /// 0x8bc
			 0xe7528505, /// 0x8c0
			 0xaa26d039, /// 0x8c4
			 0xa0e89ab9, /// 0x8c8
			 0x94951ff9, /// 0x8cc
			 0xe3bb3056, /// 0x8d0
			 0x7f79bd64, /// 0x8d4
			 0x5d2bedcb, /// 0x8d8
			 0xbcc7489a, /// 0x8dc
			 0xdc1e5115, /// 0x8e0
			 0x7978f772, /// 0x8e4
			 0x854b8809, /// 0x8e8
			 0xc55b49c9, /// 0x8ec
			 0x3f5da3c3, /// 0x8f0
			 0x5ab914df, /// 0x8f4
			 0xfb55099b, /// 0x8f8
			 0x219ff82a, /// 0x8fc
			 0xcf70a73a, /// 0x900
			 0x41d41d86, /// 0x904
			 0x3f13f44a, /// 0x908
			 0x13beb2a2, /// 0x90c
			 0xa7127347, /// 0x910
			 0x324bb861, /// 0x914
			 0x858d2e69, /// 0x918
			 0x58d66850, /// 0x91c
			 0xed475b3b, /// 0x920
			 0x4566ee86, /// 0x924
			 0x6bfd7366, /// 0x928
			 0xdb92f12b, /// 0x92c
			 0xbf855ab9, /// 0x930
			 0x89aa45a2, /// 0x934
			 0xe2b9c99e, /// 0x938
			 0xfd20e45d, /// 0x93c
			 0x168a3af2, /// 0x940
			 0xf0babd7f, /// 0x944
			 0x35a6e34d, /// 0x948
			 0x581c29ad, /// 0x94c
			 0xe53c2b09, /// 0x950
			 0x9b67d84b, /// 0x954
			 0xac12725b, /// 0x958
			 0xa553af10, /// 0x95c
			 0x1d425449, /// 0x960
			 0x1d307036, /// 0x964
			 0x759ea724, /// 0x968
			 0x0c208d22, /// 0x96c
			 0x143eb072, /// 0x970
			 0x43f4f5b2, /// 0x974
			 0x47981530, /// 0x978
			 0x6c3d9f3e, /// 0x97c
			 0xc2b8e917, /// 0x980
			 0x0a96e4c6, /// 0x984
			 0x9eedd989, /// 0x988
			 0xac714ac5, /// 0x98c
			 0x32d5714b, /// 0x990
			 0x06914446, /// 0x994
			 0xe679711b, /// 0x998
			 0x4a9b56e2, /// 0x99c
			 0x603875a2, /// 0x9a0
			 0xbdad7857, /// 0x9a4
			 0xc5e3d2f5, /// 0x9a8
			 0xb06ffd3c, /// 0x9ac
			 0xe41f8cd9, /// 0x9b0
			 0x58932424, /// 0x9b4
			 0x2c01af10, /// 0x9b8
			 0x6994742b, /// 0x9bc
			 0xe32b27b6, /// 0x9c0
			 0xd3ca4220, /// 0x9c4
			 0xc81a0828, /// 0x9c8
			 0xd77dfbe1, /// 0x9cc
			 0x382ba319, /// 0x9d0
			 0x20f60737, /// 0x9d4
			 0x39ff5317, /// 0x9d8
			 0xa684255e, /// 0x9dc
			 0x75932b88, /// 0x9e0
			 0xce9f9f28, /// 0x9e4
			 0x49437829, /// 0x9e8
			 0xc3ec6d0b, /// 0x9ec
			 0x400ac32a, /// 0x9f0
			 0x193f7201, /// 0x9f4
			 0x6e08b158, /// 0x9f8
			 0x993b2529, /// 0x9fc
			 0x85e8b65c, /// 0xa00
			 0x0610fb36, /// 0xa04
			 0xed3012c6, /// 0xa08
			 0xf9db188a, /// 0xa0c
			 0x674e01fc, /// 0xa10
			 0xbd867b12, /// 0xa14
			 0xbaa09177, /// 0xa18
			 0xf41b2d6b, /// 0xa1c
			 0x13bef10a, /// 0xa20
			 0x563279d1, /// 0xa24
			 0xf6a59ee1, /// 0xa28
			 0x92a4d607, /// 0xa2c
			 0x00158f9b, /// 0xa30
			 0x259275f8, /// 0xa34
			 0xd4d3ae95, /// 0xa38
			 0x3c38a4b9, /// 0xa3c
			 0x91fa13c4, /// 0xa40
			 0x3db5f661, /// 0xa44
			 0x8302ae2a, /// 0xa48
			 0xc8acb59a, /// 0xa4c
			 0xa1c63764, /// 0xa50
			 0x66ab8c54, /// 0xa54
			 0x831d8296, /// 0xa58
			 0x204ed015, /// 0xa5c
			 0x2879fe39, /// 0xa60
			 0x81e1197e, /// 0xa64
			 0xdea3b8fe, /// 0xa68
			 0x16182bdf, /// 0xa6c
			 0xb0a4615f, /// 0xa70
			 0xc77d314f, /// 0xa74
			 0x85978a1b, /// 0xa78
			 0xf974f9c0, /// 0xa7c
			 0x534dc2e6, /// 0xa80
			 0xf7e3cade, /// 0xa84
			 0x1fee9985, /// 0xa88
			 0x3f79a40b, /// 0xa8c
			 0x0005bf97, /// 0xa90
			 0x647609e1, /// 0xa94
			 0xab1ca6b9, /// 0xa98
			 0xced6ff42, /// 0xa9c
			 0xc4e1e9ae, /// 0xaa0
			 0xda676d9a, /// 0xaa4
			 0xbdb10657, /// 0xaa8
			 0x9dc520b4, /// 0xaac
			 0xc9c90916, /// 0xab0
			 0x7f8b1dc1, /// 0xab4
			 0xea82ffea, /// 0xab8
			 0x1154c868, /// 0xabc
			 0x374feffc, /// 0xac0
			 0xba496b0a, /// 0xac4
			 0x3a383fcf, /// 0xac8
			 0x26bb6887, /// 0xacc
			 0x21b2c19f, /// 0xad0
			 0xb852a467, /// 0xad4
			 0xc4559d64, /// 0xad8
			 0x9bf67ab2, /// 0xadc
			 0xc8b5b6be, /// 0xae0
			 0x18e81eec, /// 0xae4
			 0x3a1695a2, /// 0xae8
			 0x1461e1a7, /// 0xaec
			 0x9d2ad86e, /// 0xaf0
			 0x22e5b1e0, /// 0xaf4
			 0x90273393, /// 0xaf8
			 0x803ceaa7, /// 0xafc
			 0x6d010acd, /// 0xb00
			 0x135befd3, /// 0xb04
			 0x96df1208, /// 0xb08
			 0xadc727a4, /// 0xb0c
			 0xb0ff9f22, /// 0xb10
			 0x72955195, /// 0xb14
			 0xc5c08bf2, /// 0xb18
			 0x4b6b1d76, /// 0xb1c
			 0x0c825d4b, /// 0xb20
			 0x791a34b5, /// 0xb24
			 0x7a7b5018, /// 0xb28
			 0x2296884d, /// 0xb2c
			 0x99e14d17, /// 0xb30
			 0xe4f349c0, /// 0xb34
			 0x089cda6c, /// 0xb38
			 0xecb15260, /// 0xb3c
			 0x757db741, /// 0xb40
			 0x7151a651, /// 0xb44
			 0x4d32b4d1, /// 0xb48
			 0xf788794a, /// 0xb4c
			 0xa73f4a74, /// 0xb50
			 0x51c0e321, /// 0xb54
			 0xcf8972fd, /// 0xb58
			 0x2a5a28e7, /// 0xb5c
			 0x827c9d6a, /// 0xb60
			 0xabf86272, /// 0xb64
			 0xe2b26b97, /// 0xb68
			 0x44c3b6ab, /// 0xb6c
			 0x2352571d, /// 0xb70
			 0x5fb5b5bd, /// 0xb74
			 0xffc1cda2, /// 0xb78
			 0xcf71e5d0, /// 0xb7c
			 0xcd372ca6, /// 0xb80
			 0x1a11fa7c, /// 0xb84
			 0x37ddba20, /// 0xb88
			 0xff1cf77c, /// 0xb8c
			 0x695a499c, /// 0xb90
			 0xc8a47457, /// 0xb94
			 0xc18453c1, /// 0xb98
			 0xc316bb61, /// 0xb9c
			 0x8e024a73, /// 0xba0
			 0x8800ca66, /// 0xba4
			 0x635cb3a0, /// 0xba8
			 0x1bebed3d, /// 0xbac
			 0xc6cbeb06, /// 0xbb0
			 0x78fbf673, /// 0xbb4
			 0x5b70c8f7, /// 0xbb8
			 0xcbebac5d, /// 0xbbc
			 0x0b97da6a, /// 0xbc0
			 0x0bce6ce2, /// 0xbc4
			 0xa5f2e5fb, /// 0xbc8
			 0x339645ec, /// 0xbcc
			 0xbfa6b799, /// 0xbd0
			 0x6112cc89, /// 0xbd4
			 0x38ccb2bb, /// 0xbd8
			 0x4dd76275, /// 0xbdc
			 0xd588390a, /// 0xbe0
			 0x13b857f0, /// 0xbe4
			 0xf44d17b5, /// 0xbe8
			 0xffddb63a, /// 0xbec
			 0x122880c1, /// 0xbf0
			 0x0251f019, /// 0xbf4
			 0xf4b7181a, /// 0xbf8
			 0x66098b78, /// 0xbfc
			 0x23d500b5, /// 0xc00
			 0x9ead15b3, /// 0xc04
			 0xb53f17ac, /// 0xc08
			 0xa780c2e3, /// 0xc0c
			 0x0b11d439, /// 0xc10
			 0x828c0533, /// 0xc14
			 0x85d416cc, /// 0xc18
			 0x8fc1ff1f, /// 0xc1c
			 0x5d6e604b, /// 0xc20
			 0x4270a29d, /// 0xc24
			 0xaf1a032c, /// 0xc28
			 0xaa495e58, /// 0xc2c
			 0x6f554456, /// 0xc30
			 0xccf6f964, /// 0xc34
			 0x261f543d, /// 0xc38
			 0xd79dfcf1, /// 0xc3c
			 0x1e24ac56, /// 0xc40
			 0xd95f230e, /// 0xc44
			 0x8718850c, /// 0xc48
			 0x9bd95627, /// 0xc4c
			 0x79c0c831, /// 0xc50
			 0x63814853, /// 0xc54
			 0x39899e9a, /// 0xc58
			 0x3701aa2f, /// 0xc5c
			 0x473f67ad, /// 0xc60
			 0x9944c25d, /// 0xc64
			 0xb3d28148, /// 0xc68
			 0x31bc6a2c, /// 0xc6c
			 0x71113942, /// 0xc70
			 0x52b7c50b, /// 0xc74
			 0x93a908f0, /// 0xc78
			 0x20bd2ba9, /// 0xc7c
			 0x63e1b142, /// 0xc80
			 0x55793f91, /// 0xc84
			 0x9bff4317, /// 0xc88
			 0x4cf62c54, /// 0xc8c
			 0x8df378df, /// 0xc90
			 0x556085f5, /// 0xc94
			 0xfa323678, /// 0xc98
			 0xe9861434, /// 0xc9c
			 0x9d956435, /// 0xca0
			 0x04e6074c, /// 0xca4
			 0xe422cd9c, /// 0xca8
			 0x3dad03f4, /// 0xcac
			 0x4929053c, /// 0xcb0
			 0x3e9821ae, /// 0xcb4
			 0x25ccb5ca, /// 0xcb8
			 0xf504190e, /// 0xcbc
			 0x0f5f5bc5, /// 0xcc0
			 0x7e339a96, /// 0xcc4
			 0x032b5fe7, /// 0xcc8
			 0x7d381e6d, /// 0xccc
			 0x53fbc59f, /// 0xcd0
			 0xf16c4d2e, /// 0xcd4
			 0x1af6b266, /// 0xcd8
			 0x4a420a8a, /// 0xcdc
			 0xabd72755, /// 0xce0
			 0xe80f91e9, /// 0xce4
			 0xb845bc65, /// 0xce8
			 0xd1bfdc7f, /// 0xcec
			 0xf943d1f9, /// 0xcf0
			 0x4a3898ff, /// 0xcf4
			 0x1ad3faf4, /// 0xcf8
			 0x25300a2b, /// 0xcfc
			 0x4259dbd2, /// 0xd00
			 0x55cf4fa8, /// 0xd04
			 0xf205f158, /// 0xd08
			 0xe2b25962, /// 0xd0c
			 0x7c6314cc, /// 0xd10
			 0xe0c70642, /// 0xd14
			 0x4b51ca28, /// 0xd18
			 0xfb80b0b3, /// 0xd1c
			 0xea51c75b, /// 0xd20
			 0x164f4610, /// 0xd24
			 0xe51690dc, /// 0xd28
			 0x3daad2f0, /// 0xd2c
			 0x42bc37dd, /// 0xd30
			 0xfe9828bc, /// 0xd34
			 0x3a75d651, /// 0xd38
			 0xc2514d65, /// 0xd3c
			 0x764ff9a1, /// 0xd40
			 0x22d85687, /// 0xd44
			 0xe0d004cb, /// 0xd48
			 0x865a68cc, /// 0xd4c
			 0x3371f6ed, /// 0xd50
			 0xbe2e308c, /// 0xd54
			 0x6b481ad6, /// 0xd58
			 0xcdef4214, /// 0xd5c
			 0x91059fdb, /// 0xd60
			 0x8ed465c4, /// 0xd64
			 0x65cbbb59, /// 0xd68
			 0xdc4bff1a, /// 0xd6c
			 0x45a701a9, /// 0xd70
			 0xb6b842d9, /// 0xd74
			 0xc9f385b1, /// 0xd78
			 0x2eb0220b, /// 0xd7c
			 0x4b9f95a6, /// 0xd80
			 0x6db7c4c5, /// 0xd84
			 0x7451ab5c, /// 0xd88
			 0x5c3301a4, /// 0xd8c
			 0xb807e1c0, /// 0xd90
			 0x9dc8cd61, /// 0xd94
			 0x5d498423, /// 0xd98
			 0x02baa6e3, /// 0xd9c
			 0x7c8ef292, /// 0xda0
			 0xa0b090bc, /// 0xda4
			 0xeb5cf798, /// 0xda8
			 0xda9c4610, /// 0xdac
			 0xe3e93e49, /// 0xdb0
			 0x85db7b3a, /// 0xdb4
			 0xf8c95707, /// 0xdb8
			 0x7b51f749, /// 0xdbc
			 0xfa77310e, /// 0xdc0
			 0x7e51c380, /// 0xdc4
			 0x157d0aa6, /// 0xdc8
			 0x660abe14, /// 0xdcc
			 0xddd8637e, /// 0xdd0
			 0x9c2109a4, /// 0xdd4
			 0xc272d877, /// 0xdd8
			 0x5026f1e1, /// 0xddc
			 0x8f9ce89d, /// 0xde0
			 0x82c25090, /// 0xde4
			 0xe42b8228, /// 0xde8
			 0xdf6f6651, /// 0xdec
			 0x9c9ebca9, /// 0xdf0
			 0xfeabd9de, /// 0xdf4
			 0x2daf25c3, /// 0xdf8
			 0xf9424092, /// 0xdfc
			 0x7ef2a265, /// 0xe00
			 0x377a51be, /// 0xe04
			 0x98c826ab, /// 0xe08
			 0x8e8d0172, /// 0xe0c
			 0x086cbf00, /// 0xe10
			 0x52704a05, /// 0xe14
			 0xe1188ee0, /// 0xe18
			 0xc3833ada, /// 0xe1c
			 0xa74c872b, /// 0xe20
			 0x9d6320e6, /// 0xe24
			 0x99b06559, /// 0xe28
			 0xf4dbc500, /// 0xe2c
			 0x4801f256, /// 0xe30
			 0xc190391f, /// 0xe34
			 0x599a2660, /// 0xe38
			 0xfb869b2e, /// 0xe3c
			 0x33b4ffc2, /// 0xe40
			 0xa0e3afce, /// 0xe44
			 0x778a6dc4, /// 0xe48
			 0xcb4b60f6, /// 0xe4c
			 0xefdc919d, /// 0xe50
			 0xa849260b, /// 0xe54
			 0x6645f757, /// 0xe58
			 0x314ec11f, /// 0xe5c
			 0xe8ecd505, /// 0xe60
			 0x35a67c4d, /// 0xe64
			 0x7dfe3319, /// 0xe68
			 0x70c1461f, /// 0xe6c
			 0x85cce1b1, /// 0xe70
			 0x3ad82cd9, /// 0xe74
			 0x1045f9ac, /// 0xe78
			 0x515f0fdc, /// 0xe7c
			 0x9fa9d7ea, /// 0xe80
			 0x4381f405, /// 0xe84
			 0xebe6f68e, /// 0xe88
			 0xaa6eba99, /// 0xe8c
			 0x033ddaad, /// 0xe90
			 0x30569c09, /// 0xe94
			 0x52fb44ff, /// 0xe98
			 0x06c3eb7b, /// 0xe9c
			 0xd1c786cc, /// 0xea0
			 0xd2cacf71, /// 0xea4
			 0x3ca9bea0, /// 0xea8
			 0x5432b1bd, /// 0xeac
			 0x75bbbdcc, /// 0xeb0
			 0x195f3bed, /// 0xeb4
			 0xd085a6b7, /// 0xeb8
			 0x21f967b2, /// 0xebc
			 0x4ff5e1c5, /// 0xec0
			 0xf04233a7, /// 0xec4
			 0x4ec1ece1, /// 0xec8
			 0x73fd784a, /// 0xecc
			 0x4b74cd14, /// 0xed0
			 0xa4560e13, /// 0xed4
			 0x981a214a, /// 0xed8
			 0x775300f3, /// 0xedc
			 0x1c994bc7, /// 0xee0
			 0x0c31d741, /// 0xee4
			 0x5e69f739, /// 0xee8
			 0xf6805cf6, /// 0xeec
			 0x99875198, /// 0xef0
			 0x8eebeacd, /// 0xef4
			 0x84b8f84f, /// 0xef8
			 0xef449f5e, /// 0xefc
			 0xde442a92, /// 0xf00
			 0xaaa76d85, /// 0xf04
			 0xeb0ea510, /// 0xf08
			 0x37c09f2c, /// 0xf0c
			 0xde90a3d9, /// 0xf10
			 0x6f6d1b56, /// 0xf14
			 0x711f123e, /// 0xf18
			 0xae583df5, /// 0xf1c
			 0x5bc45975, /// 0xf20
			 0x8f260216, /// 0xf24
			 0x1ffe8bb0, /// 0xf28
			 0x1838321a, /// 0xf2c
			 0xeff58c3a, /// 0xf30
			 0x10f2d85b, /// 0xf34
			 0xece6ab41, /// 0xf38
			 0x7d307e1e, /// 0xf3c
			 0x2efff38c, /// 0xf40
			 0x831843bc, /// 0xf44
			 0x2659b6e6, /// 0xf48
			 0xfd8fec73, /// 0xf4c
			 0x7868b58c, /// 0xf50
			 0x5334003a, /// 0xf54
			 0x29970495, /// 0xf58
			 0x1f6a07b0, /// 0xf5c
			 0xdbdd8c9c, /// 0xf60
			 0x842ec26f, /// 0xf64
			 0x0349f77b, /// 0xf68
			 0x57755e99, /// 0xf6c
			 0x250be50e, /// 0xf70
			 0xdd33dd9c, /// 0xf74
			 0xc6b4c0fd, /// 0xf78
			 0x7cb404b4, /// 0xf7c
			 0x6ed00230, /// 0xf80
			 0x833bcefb, /// 0xf84
			 0x34c1aa29, /// 0xf88
			 0x015e6ddf, /// 0xf8c
			 0x01ecf349, /// 0xf90
			 0x53a15c57, /// 0xf94
			 0x310ba85a, /// 0xf98
			 0xdaad2053, /// 0xf9c
			 0xa8a5f730, /// 0xfa0
			 0xda5329bf, /// 0xfa4
			 0xb4b604ac, /// 0xfa8
			 0xe772ed31, /// 0xfac
			 0xa90a5892, /// 0xfb0
			 0x81b0273e, /// 0xfb4
			 0x20f3a942, /// 0xfb8
			 0x5399d113, /// 0xfbc
			 0xce30852c, /// 0xfc0
			 0xd2c75365, /// 0xfc4
			 0x5e17ce07, /// 0xfc8
			 0x75fe22ec, /// 0xfcc
			 0x8193d500, /// 0xfd0
			 0x5dbd32f3, /// 0xfd4
			 0x6e090e00, /// 0xfd8
			 0x28e390c4, /// 0xfdc
			 0x1e208f07, /// 0xfe0
			 0x00cf8a3e, /// 0xfe4
			 0x91c88c8c, /// 0xfe8
			 0xc88dae9a, /// 0xfec
			 0xc0263738, /// 0xff0
			 0x71bb2b20, /// 0xff4
			 0x770ab381, /// 0xff8
			 0x0802a34f /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x6083a334, /// 0x0
			 0x0d16ca61, /// 0x4
			 0xe12b7300, /// 0x8
			 0x88292bf4, /// 0xc
			 0xd37b85e3, /// 0x10
			 0xc573299c, /// 0x14
			 0xdd44aba3, /// 0x18
			 0x5ae5e56b, /// 0x1c
			 0xfc685842, /// 0x20
			 0x90948d67, /// 0x24
			 0x83d753f3, /// 0x28
			 0xa4a6eced, /// 0x2c
			 0x7013bcf1, /// 0x30
			 0xcdd2b41b, /// 0x34
			 0x7a3faf61, /// 0x38
			 0x6cb6b13f, /// 0x3c
			 0xdb643a16, /// 0x40
			 0x04052a22, /// 0x44
			 0x70921c0b, /// 0x48
			 0x1434c68f, /// 0x4c
			 0xf5ad060e, /// 0x50
			 0x10cb3e48, /// 0x54
			 0x0cba0721, /// 0x58
			 0x1b8fe0b2, /// 0x5c
			 0x092cadf1, /// 0x60
			 0xfc621094, /// 0x64
			 0x4bd4bc55, /// 0x68
			 0x1ae1d47e, /// 0x6c
			 0x7266df8b, /// 0x70
			 0x65a0c01c, /// 0x74
			 0x9fd611e4, /// 0x78
			 0xe3e815ee, /// 0x7c
			 0xb752db2c, /// 0x80
			 0x0abb257a, /// 0x84
			 0xbc45ac45, /// 0x88
			 0x842f7081, /// 0x8c
			 0xc36440a4, /// 0x90
			 0xc60b2450, /// 0x94
			 0x830cccc2, /// 0x98
			 0x3d439baf, /// 0x9c
			 0x1215d6fe, /// 0xa0
			 0x98fda104, /// 0xa4
			 0x1d91bd17, /// 0xa8
			 0x0f2c1dd4, /// 0xac
			 0xc5882710, /// 0xb0
			 0x70b988aa, /// 0xb4
			 0x770ae53b, /// 0xb8
			 0xd9aa492d, /// 0xbc
			 0xb6cd022e, /// 0xc0
			 0xffe8de9f, /// 0xc4
			 0x8d0136a5, /// 0xc8
			 0x353f8bab, /// 0xcc
			 0x94de7235, /// 0xd0
			 0x70118408, /// 0xd4
			 0x2fcd1786, /// 0xd8
			 0xc98a94e5, /// 0xdc
			 0x28ae42a6, /// 0xe0
			 0x1152d391, /// 0xe4
			 0xf1f49b8d, /// 0xe8
			 0xb7d8773f, /// 0xec
			 0x28d354c2, /// 0xf0
			 0xabe92730, /// 0xf4
			 0x880267f5, /// 0xf8
			 0xd4b3ffb4, /// 0xfc
			 0xd76f13bc, /// 0x100
			 0x41c96280, /// 0x104
			 0x87cd4a20, /// 0x108
			 0x63b322c3, /// 0x10c
			 0x576b75bc, /// 0x110
			 0x284a6809, /// 0x114
			 0x5b8de1c7, /// 0x118
			 0x271d7309, /// 0x11c
			 0xd6857e44, /// 0x120
			 0x6a724287, /// 0x124
			 0x830d2e79, /// 0x128
			 0xc96e1715, /// 0x12c
			 0xbefd67a4, /// 0x130
			 0x6a435bf9, /// 0x134
			 0x5ce5b76d, /// 0x138
			 0x9419d0bf, /// 0x13c
			 0xb827436f, /// 0x140
			 0xa337e374, /// 0x144
			 0xac4c47d9, /// 0x148
			 0x631c9bce, /// 0x14c
			 0x26a36a64, /// 0x150
			 0x8d844148, /// 0x154
			 0x50c27e0f, /// 0x158
			 0xc8450df0, /// 0x15c
			 0x7f2aa4bf, /// 0x160
			 0x2dc862bb, /// 0x164
			 0x94bbf21d, /// 0x168
			 0x11c136e3, /// 0x16c
			 0x57bda2f5, /// 0x170
			 0x39052ceb, /// 0x174
			 0x2b94894d, /// 0x178
			 0xd23c18e1, /// 0x17c
			 0xe6170029, /// 0x180
			 0x4459615f, /// 0x184
			 0xe973fa48, /// 0x188
			 0x4ab40b1f, /// 0x18c
			 0x78704fa6, /// 0x190
			 0xe60eddee, /// 0x194
			 0xbffad134, /// 0x198
			 0xc3791f62, /// 0x19c
			 0x9152c64d, /// 0x1a0
			 0xf9af6a5e, /// 0x1a4
			 0x5b3e4a5a, /// 0x1a8
			 0xfa135ecc, /// 0x1ac
			 0xd119dacb, /// 0x1b0
			 0x9f794b54, /// 0x1b4
			 0x730b7c50, /// 0x1b8
			 0x6a44e49e, /// 0x1bc
			 0x781ec126, /// 0x1c0
			 0x0a47247d, /// 0x1c4
			 0x65113a7a, /// 0x1c8
			 0xe3832da9, /// 0x1cc
			 0x3e06e84a, /// 0x1d0
			 0x30a27bb3, /// 0x1d4
			 0x77411294, /// 0x1d8
			 0x9f7efcc8, /// 0x1dc
			 0x8f31c396, /// 0x1e0
			 0xf8ed6ff2, /// 0x1e4
			 0xe4b75d98, /// 0x1e8
			 0x87a6a203, /// 0x1ec
			 0x03d19f18, /// 0x1f0
			 0x710f6717, /// 0x1f4
			 0xa9467573, /// 0x1f8
			 0xb52afdbd, /// 0x1fc
			 0xf3e69f4e, /// 0x200
			 0x5b944c28, /// 0x204
			 0x7130e15a, /// 0x208
			 0xfac8a71e, /// 0x20c
			 0x15f1a531, /// 0x210
			 0x523e2381, /// 0x214
			 0xc5d48afb, /// 0x218
			 0xc4a469b1, /// 0x21c
			 0x79de36f5, /// 0x220
			 0x4ed33b68, /// 0x224
			 0xd59c8aba, /// 0x228
			 0x605ce453, /// 0x22c
			 0x0da858ed, /// 0x230
			 0xe7074f74, /// 0x234
			 0x7e60c1f3, /// 0x238
			 0x3ed7f5a3, /// 0x23c
			 0x3de4c39a, /// 0x240
			 0x6654ad4d, /// 0x244
			 0x6d4c9cfd, /// 0x248
			 0x53874417, /// 0x24c
			 0x6bb81b17, /// 0x250
			 0x12b74ab4, /// 0x254
			 0x0625a312, /// 0x258
			 0xf21eecb5, /// 0x25c
			 0x748c6700, /// 0x260
			 0x7f3a96df, /// 0x264
			 0x7c7fdbef, /// 0x268
			 0x904622e4, /// 0x26c
			 0x59e4dfa5, /// 0x270
			 0xa8207022, /// 0x274
			 0x42e1ded5, /// 0x278
			 0x4e8f0f13, /// 0x27c
			 0xcf57a19b, /// 0x280
			 0xbbd0ef61, /// 0x284
			 0x1acc3cb3, /// 0x288
			 0x3fc437e6, /// 0x28c
			 0x66bb922b, /// 0x290
			 0x16442a61, /// 0x294
			 0x68a8c7cd, /// 0x298
			 0xd7d6bed3, /// 0x29c
			 0x20e0abfb, /// 0x2a0
			 0xf33ac32e, /// 0x2a4
			 0xfbfd54bf, /// 0x2a8
			 0xe252cca4, /// 0x2ac
			 0x6843a1fe, /// 0x2b0
			 0xe99f04a2, /// 0x2b4
			 0x88b5aa45, /// 0x2b8
			 0x6daa17ab, /// 0x2bc
			 0xdbcae2da, /// 0x2c0
			 0x69b7e37d, /// 0x2c4
			 0x1a0cd65f, /// 0x2c8
			 0x5daab18c, /// 0x2cc
			 0x74b0c476, /// 0x2d0
			 0xeec3b7b1, /// 0x2d4
			 0xd2cde4c4, /// 0x2d8
			 0xb233d340, /// 0x2dc
			 0xe5eb97a1, /// 0x2e0
			 0x089ffe0d, /// 0x2e4
			 0xec6fb621, /// 0x2e8
			 0x4fbf0ddf, /// 0x2ec
			 0xbaf5083a, /// 0x2f0
			 0x1b261492, /// 0x2f4
			 0x092a9218, /// 0x2f8
			 0x855e0ae0, /// 0x2fc
			 0x1c3ce7b0, /// 0x300
			 0x6130c036, /// 0x304
			 0x4898f747, /// 0x308
			 0x8b5ebb51, /// 0x30c
			 0x5b2dc0a4, /// 0x310
			 0x561aff11, /// 0x314
			 0xd51b58b4, /// 0x318
			 0xb876b90b, /// 0x31c
			 0x2872c1aa, /// 0x320
			 0xa50becf2, /// 0x324
			 0xd2d51e50, /// 0x328
			 0xa86efd8d, /// 0x32c
			 0x4ec4f1ee, /// 0x330
			 0xe91c9eb4, /// 0x334
			 0x9081abe8, /// 0x338
			 0x20f07403, /// 0x33c
			 0xc7f2318d, /// 0x340
			 0x1124cf2a, /// 0x344
			 0xd7496ce4, /// 0x348
			 0xfb1d1463, /// 0x34c
			 0xa226ef4d, /// 0x350
			 0xe01e6624, /// 0x354
			 0xece8126f, /// 0x358
			 0xfaf971d2, /// 0x35c
			 0x4e5adaa9, /// 0x360
			 0xedd2be01, /// 0x364
			 0xdeb1216e, /// 0x368
			 0x26e9aad9, /// 0x36c
			 0x7242560d, /// 0x370
			 0xe08c7d4c, /// 0x374
			 0xf064c66b, /// 0x378
			 0x27acd8ae, /// 0x37c
			 0xd88cb232, /// 0x380
			 0x28849757, /// 0x384
			 0xc0b26329, /// 0x388
			 0x02937b16, /// 0x38c
			 0x053c7eff, /// 0x390
			 0xf563e17f, /// 0x394
			 0xdb5d0bfe, /// 0x398
			 0x4b266153, /// 0x39c
			 0x9395cce9, /// 0x3a0
			 0x62569878, /// 0x3a4
			 0x30fa88a3, /// 0x3a8
			 0x92f77155, /// 0x3ac
			 0xaeac1acc, /// 0x3b0
			 0x637af2bb, /// 0x3b4
			 0x0d20973b, /// 0x3b8
			 0x9e7a516b, /// 0x3bc
			 0x592e23a3, /// 0x3c0
			 0x5cc72eca, /// 0x3c4
			 0xf0abfba7, /// 0x3c8
			 0xb38c88b6, /// 0x3cc
			 0xddb44fe4, /// 0x3d0
			 0xafe1cc51, /// 0x3d4
			 0x7c2f205e, /// 0x3d8
			 0x0964bee8, /// 0x3dc
			 0x643119d5, /// 0x3e0
			 0xf102e573, /// 0x3e4
			 0x780e1d64, /// 0x3e8
			 0x61017a28, /// 0x3ec
			 0x9d695545, /// 0x3f0
			 0xc8c7d5c1, /// 0x3f4
			 0x821b8685, /// 0x3f8
			 0xc3b0a9ae, /// 0x3fc
			 0x405104e5, /// 0x400
			 0xfa45f1fd, /// 0x404
			 0x1066a75c, /// 0x408
			 0xa1e532f7, /// 0x40c
			 0x905e718e, /// 0x410
			 0x571837e1, /// 0x414
			 0xc0acb5e1, /// 0x418
			 0x495c6607, /// 0x41c
			 0xbb5b5461, /// 0x420
			 0xbf0c2191, /// 0x424
			 0xa87505a4, /// 0x428
			 0xc9f58681, /// 0x42c
			 0x728ef8d7, /// 0x430
			 0xfae3730a, /// 0x434
			 0x8d972956, /// 0x438
			 0x134f9278, /// 0x43c
			 0xbe9a00d2, /// 0x440
			 0x96a1adfb, /// 0x444
			 0xce6d5071, /// 0x448
			 0xaeb805f2, /// 0x44c
			 0x830925ef, /// 0x450
			 0xcbe3c68b, /// 0x454
			 0x8f3b1f4d, /// 0x458
			 0x49360ae8, /// 0x45c
			 0x3265674a, /// 0x460
			 0x05a047b4, /// 0x464
			 0x6f310434, /// 0x468
			 0x361a8928, /// 0x46c
			 0x7d769430, /// 0x470
			 0xdc99d836, /// 0x474
			 0x383d36c1, /// 0x478
			 0x6ff603fd, /// 0x47c
			 0xdcb12cd9, /// 0x480
			 0xb63181ab, /// 0x484
			 0x80f29bc9, /// 0x488
			 0x82c663eb, /// 0x48c
			 0x794cde46, /// 0x490
			 0xbe3ff99d, /// 0x494
			 0xf158d426, /// 0x498
			 0xe696618e, /// 0x49c
			 0xee153521, /// 0x4a0
			 0xb82f2acc, /// 0x4a4
			 0x9f00a5b0, /// 0x4a8
			 0x59e2381f, /// 0x4ac
			 0xdc76cdf5, /// 0x4b0
			 0x0d5a4f7c, /// 0x4b4
			 0xdf144052, /// 0x4b8
			 0xc9dfdf2d, /// 0x4bc
			 0x4cc1f413, /// 0x4c0
			 0xbf9847af, /// 0x4c4
			 0xd25ead56, /// 0x4c8
			 0x45aa082c, /// 0x4cc
			 0x93a4e184, /// 0x4d0
			 0x66e27790, /// 0x4d4
			 0x5dafe738, /// 0x4d8
			 0xe8977f9f, /// 0x4dc
			 0x0abc9efe, /// 0x4e0
			 0xfb5640c9, /// 0x4e4
			 0x8b4dea3f, /// 0x4e8
			 0xe05f72a0, /// 0x4ec
			 0x79acf5f7, /// 0x4f0
			 0xe35d67e2, /// 0x4f4
			 0x0d847f15, /// 0x4f8
			 0xc677d781, /// 0x4fc
			 0x4e748410, /// 0x500
			 0x10c5ffbe, /// 0x504
			 0xfae4090c, /// 0x508
			 0x7842311e, /// 0x50c
			 0x76b494f6, /// 0x510
			 0xa8bb6915, /// 0x514
			 0x8c797490, /// 0x518
			 0xf76e675d, /// 0x51c
			 0xd4e5a1a1, /// 0x520
			 0xc5f0bb9e, /// 0x524
			 0xdac11b78, /// 0x528
			 0xb1e8efda, /// 0x52c
			 0x9b110d90, /// 0x530
			 0x43ff5351, /// 0x534
			 0x99a34cc6, /// 0x538
			 0xd185510f, /// 0x53c
			 0x52137158, /// 0x540
			 0x67e778ad, /// 0x544
			 0xe3ca2830, /// 0x548
			 0xb4c324b3, /// 0x54c
			 0xfa3ef9b5, /// 0x550
			 0xda0c8d76, /// 0x554
			 0xf27258d3, /// 0x558
			 0x6a4fc00e, /// 0x55c
			 0x44a9b279, /// 0x560
			 0x73408697, /// 0x564
			 0x43446110, /// 0x568
			 0x235eff66, /// 0x56c
			 0xc68b62cb, /// 0x570
			 0xe572d0a6, /// 0x574
			 0xde9c80dd, /// 0x578
			 0x95e93735, /// 0x57c
			 0xe75dac06, /// 0x580
			 0x688c89f3, /// 0x584
			 0x08d16666, /// 0x588
			 0xde96a3ae, /// 0x58c
			 0xb4c7097c, /// 0x590
			 0xf941f8a8, /// 0x594
			 0xdd6111e3, /// 0x598
			 0x9bac4e73, /// 0x59c
			 0xc7ac9d1d, /// 0x5a0
			 0xa6efcbe6, /// 0x5a4
			 0x2ad55fa6, /// 0x5a8
			 0x192a8882, /// 0x5ac
			 0x213e1f68, /// 0x5b0
			 0xa401369f, /// 0x5b4
			 0xd7f4baf7, /// 0x5b8
			 0x3825e037, /// 0x5bc
			 0xbb77b4b1, /// 0x5c0
			 0x54de1993, /// 0x5c4
			 0xfe2f2fad, /// 0x5c8
			 0x30288e2f, /// 0x5cc
			 0x56cc62b9, /// 0x5d0
			 0x10f3b48e, /// 0x5d4
			 0x05b79c22, /// 0x5d8
			 0x32db6304, /// 0x5dc
			 0xb610b0ec, /// 0x5e0
			 0x1e17fde0, /// 0x5e4
			 0x4df9fde2, /// 0x5e8
			 0x69a1d0e1, /// 0x5ec
			 0x8ee089d7, /// 0x5f0
			 0x85df5769, /// 0x5f4
			 0x59be0e49, /// 0x5f8
			 0xacb07795, /// 0x5fc
			 0xd0a35861, /// 0x600
			 0x41497f85, /// 0x604
			 0x4c2a2bbc, /// 0x608
			 0xf6205d4d, /// 0x60c
			 0x604c20f5, /// 0x610
			 0x1ab98104, /// 0x614
			 0x1c6c7dc0, /// 0x618
			 0xaa2e9d4b, /// 0x61c
			 0xbe1650a4, /// 0x620
			 0x5b6213c2, /// 0x624
			 0xe0340156, /// 0x628
			 0xadbba7b2, /// 0x62c
			 0x1f0e5eff, /// 0x630
			 0x3965c3ae, /// 0x634
			 0xc19589fc, /// 0x638
			 0x471d4ebb, /// 0x63c
			 0x8d54c810, /// 0x640
			 0xbf184949, /// 0x644
			 0x8c736e05, /// 0x648
			 0x07d76e60, /// 0x64c
			 0x2dfe14d5, /// 0x650
			 0x3e745cd6, /// 0x654
			 0x1b58d18d, /// 0x658
			 0x3c4561c4, /// 0x65c
			 0x1147c0c7, /// 0x660
			 0xad83106c, /// 0x664
			 0x71e9ea69, /// 0x668
			 0xcc835855, /// 0x66c
			 0xec2c8392, /// 0x670
			 0x298b4e55, /// 0x674
			 0xdf70e33c, /// 0x678
			 0x9b78b968, /// 0x67c
			 0x15caab8f, /// 0x680
			 0x9090d20d, /// 0x684
			 0x3707cdae, /// 0x688
			 0xc2d8dee8, /// 0x68c
			 0x3f0b5ce6, /// 0x690
			 0x5e464143, /// 0x694
			 0x173cf00c, /// 0x698
			 0xfefcfac6, /// 0x69c
			 0xe81b5dbc, /// 0x6a0
			 0x9cf716dc, /// 0x6a4
			 0x72a2ae52, /// 0x6a8
			 0x733ecc41, /// 0x6ac
			 0x360d1416, /// 0x6b0
			 0xc3452790, /// 0x6b4
			 0x9e8330c2, /// 0x6b8
			 0x8ce7fbd5, /// 0x6bc
			 0x5ac5fcb3, /// 0x6c0
			 0x652fb65b, /// 0x6c4
			 0x8f7bec18, /// 0x6c8
			 0xd6fcc6eb, /// 0x6cc
			 0xa7220d70, /// 0x6d0
			 0xc5fd6747, /// 0x6d4
			 0xc2c733d7, /// 0x6d8
			 0x8c211e3c, /// 0x6dc
			 0x7672622f, /// 0x6e0
			 0x20bc64af, /// 0x6e4
			 0x5b4cbb86, /// 0x6e8
			 0xb7c6bb86, /// 0x6ec
			 0xae007bc2, /// 0x6f0
			 0xbe141b57, /// 0x6f4
			 0xc47b03db, /// 0x6f8
			 0x22111920, /// 0x6fc
			 0xa159b9f9, /// 0x700
			 0x1b34c048, /// 0x704
			 0xdc5533a4, /// 0x708
			 0xd9ad1454, /// 0x70c
			 0x8d4f5d1f, /// 0x710
			 0x475afb11, /// 0x714
			 0xa97e7e36, /// 0x718
			 0xc4533809, /// 0x71c
			 0xdbc70c51, /// 0x720
			 0xd0f4eb9d, /// 0x724
			 0x4ef5db8a, /// 0x728
			 0xc8e379b3, /// 0x72c
			 0x23382784, /// 0x730
			 0x50ae1998, /// 0x734
			 0x9539cee8, /// 0x738
			 0x35061a9f, /// 0x73c
			 0xfcd5bfbf, /// 0x740
			 0x2c39344e, /// 0x744
			 0xfeb218b7, /// 0x748
			 0x11c6c278, /// 0x74c
			 0xfe17ca8a, /// 0x750
			 0x3ac64263, /// 0x754
			 0x157ce7ee, /// 0x758
			 0x6f65904f, /// 0x75c
			 0xdfed6ca0, /// 0x760
			 0x5a29e286, /// 0x764
			 0xf1b48d07, /// 0x768
			 0x8d9846cf, /// 0x76c
			 0x04d70c34, /// 0x770
			 0x4383afdb, /// 0x774
			 0x3054f26f, /// 0x778
			 0xf2ed94d2, /// 0x77c
			 0x894017e2, /// 0x780
			 0x73a7d86e, /// 0x784
			 0x61049e48, /// 0x788
			 0xecf41e37, /// 0x78c
			 0x4497cbad, /// 0x790
			 0x53cfb1ff, /// 0x794
			 0x5198eb83, /// 0x798
			 0x0c043248, /// 0x79c
			 0x27167c92, /// 0x7a0
			 0x0148614a, /// 0x7a4
			 0x27fa2a48, /// 0x7a8
			 0x600ba212, /// 0x7ac
			 0x942de0fd, /// 0x7b0
			 0xd94d77a6, /// 0x7b4
			 0x925b308e, /// 0x7b8
			 0xd6b45766, /// 0x7bc
			 0xdfef1754, /// 0x7c0
			 0x7d4d85cb, /// 0x7c4
			 0xe6fbbd22, /// 0x7c8
			 0x69f929eb, /// 0x7cc
			 0x097df47e, /// 0x7d0
			 0x0574dfb3, /// 0x7d4
			 0x42398553, /// 0x7d8
			 0x89f22c6f, /// 0x7dc
			 0x92cd00fd, /// 0x7e0
			 0x3225900f, /// 0x7e4
			 0xad6c7b53, /// 0x7e8
			 0x4ac69c8f, /// 0x7ec
			 0xf0489796, /// 0x7f0
			 0xc3d4e31c, /// 0x7f4
			 0xe5958835, /// 0x7f8
			 0xf9f91b71, /// 0x7fc
			 0xf03896b5, /// 0x800
			 0x8cef761e, /// 0x804
			 0x92453faa, /// 0x808
			 0x24c049f4, /// 0x80c
			 0x120b42d2, /// 0x810
			 0xc8d4d827, /// 0x814
			 0x7d2c063e, /// 0x818
			 0xacc865ce, /// 0x81c
			 0xeb42b8a4, /// 0x820
			 0x988fe701, /// 0x824
			 0x38909f81, /// 0x828
			 0x91ea4f8c, /// 0x82c
			 0x69f5a1dd, /// 0x830
			 0x91e0a186, /// 0x834
			 0xfa6ecff6, /// 0x838
			 0x78406c97, /// 0x83c
			 0x08336c07, /// 0x840
			 0x6cf0fca5, /// 0x844
			 0x5db0270c, /// 0x848
			 0x7f4b84a9, /// 0x84c
			 0xf55da71f, /// 0x850
			 0xd156d27f, /// 0x854
			 0xcdc40f11, /// 0x858
			 0xcd9aa5d1, /// 0x85c
			 0xc7ea7439, /// 0x860
			 0xac8e9057, /// 0x864
			 0xa86aae0f, /// 0x868
			 0x895ad3c8, /// 0x86c
			 0xd550fa2c, /// 0x870
			 0xc3c63c7b, /// 0x874
			 0xb2e685f4, /// 0x878
			 0x0088a1da, /// 0x87c
			 0x16a58207, /// 0x880
			 0xf0b33856, /// 0x884
			 0x8ec3c312, /// 0x888
			 0x48f1b942, /// 0x88c
			 0x7e84eefe, /// 0x890
			 0x1a6c3c6e, /// 0x894
			 0xee893dda, /// 0x898
			 0xb8f1d525, /// 0x89c
			 0xf730a831, /// 0x8a0
			 0x584285e5, /// 0x8a4
			 0xdd014a05, /// 0x8a8
			 0xa1498f20, /// 0x8ac
			 0x33b0ab9d, /// 0x8b0
			 0x65886e2c, /// 0x8b4
			 0x24d3cfa8, /// 0x8b8
			 0x4a7a3b52, /// 0x8bc
			 0x794e6adc, /// 0x8c0
			 0x83557807, /// 0x8c4
			 0x38cf59cf, /// 0x8c8
			 0xe4e766eb, /// 0x8cc
			 0x523974d0, /// 0x8d0
			 0xdc280eee, /// 0x8d4
			 0x7fd9ad96, /// 0x8d8
			 0x0548c93c, /// 0x8dc
			 0x3053e62a, /// 0x8e0
			 0xf38c15d7, /// 0x8e4
			 0xfe31ec1c, /// 0x8e8
			 0x5b678f5f, /// 0x8ec
			 0xd508dced, /// 0x8f0
			 0xcea6f84c, /// 0x8f4
			 0x84303ee8, /// 0x8f8
			 0xceb549f8, /// 0x8fc
			 0x666fe5a3, /// 0x900
			 0xfd647b67, /// 0x904
			 0xb2fe7e81, /// 0x908
			 0x875864ec, /// 0x90c
			 0x11b0b599, /// 0x910
			 0x85b5f4ce, /// 0x914
			 0xec717c66, /// 0x918
			 0xfdc8f665, /// 0x91c
			 0xb9bf3dc6, /// 0x920
			 0x6a2928c8, /// 0x924
			 0x460483c2, /// 0x928
			 0x0f91366d, /// 0x92c
			 0x855808f9, /// 0x930
			 0xb9016221, /// 0x934
			 0x596e7e5f, /// 0x938
			 0xddd4aa01, /// 0x93c
			 0x9957fe0b, /// 0x940
			 0x5671d85b, /// 0x944
			 0xe69fdb98, /// 0x948
			 0x843c4d59, /// 0x94c
			 0x512819b0, /// 0x950
			 0xd7066014, /// 0x954
			 0x04b3373a, /// 0x958
			 0x9e26ed79, /// 0x95c
			 0x3a06f45c, /// 0x960
			 0x501b561c, /// 0x964
			 0xdb059d00, /// 0x968
			 0x96138ed2, /// 0x96c
			 0x7e07e0d8, /// 0x970
			 0x42336ae2, /// 0x974
			 0x7dbe2b5e, /// 0x978
			 0x7b87afc4, /// 0x97c
			 0x8d5ddb4e, /// 0x980
			 0x8f90ec61, /// 0x984
			 0x8348b327, /// 0x988
			 0xe28e19ca, /// 0x98c
			 0xceef17e7, /// 0x990
			 0xfe4ed926, /// 0x994
			 0x9ada1830, /// 0x998
			 0xc0d24850, /// 0x99c
			 0xce1a852d, /// 0x9a0
			 0xc5cff2b7, /// 0x9a4
			 0xfcab12aa, /// 0x9a8
			 0xef3c221b, /// 0x9ac
			 0x2bfaf597, /// 0x9b0
			 0x95a8ea7e, /// 0x9b4
			 0x0d5ce4bb, /// 0x9b8
			 0x56efc563, /// 0x9bc
			 0xd69475db, /// 0x9c0
			 0xefff853c, /// 0x9c4
			 0x4bc3f862, /// 0x9c8
			 0xdf572c40, /// 0x9cc
			 0xdb5862d5, /// 0x9d0
			 0x0c635020, /// 0x9d4
			 0x7d3f3753, /// 0x9d8
			 0x66b597a3, /// 0x9dc
			 0xce1d7e91, /// 0x9e0
			 0x7b5b0763, /// 0x9e4
			 0x0a2c5da1, /// 0x9e8
			 0xa93d6d0c, /// 0x9ec
			 0xdacb9804, /// 0x9f0
			 0x0273e54f, /// 0x9f4
			 0x8254ebe3, /// 0x9f8
			 0x2d70da56, /// 0x9fc
			 0x4426578b, /// 0xa00
			 0x25e00e20, /// 0xa04
			 0xbdb78d2b, /// 0xa08
			 0xe18d5271, /// 0xa0c
			 0xe51aca9a, /// 0xa10
			 0x72fb4647, /// 0xa14
			 0x5a04d6de, /// 0xa18
			 0xf47ffd74, /// 0xa1c
			 0x381277a9, /// 0xa20
			 0xbf5e0d90, /// 0xa24
			 0xbd5d8b9c, /// 0xa28
			 0x5844028e, /// 0xa2c
			 0x99f06eeb, /// 0xa30
			 0x91b60d1c, /// 0xa34
			 0x9f42dec5, /// 0xa38
			 0xf886b643, /// 0xa3c
			 0x25667473, /// 0xa40
			 0xef7020ac, /// 0xa44
			 0x2858b338, /// 0xa48
			 0xeae8162f, /// 0xa4c
			 0x04180ce7, /// 0xa50
			 0x4f6828a7, /// 0xa54
			 0x61bf8c94, /// 0xa58
			 0x4fa6d93a, /// 0xa5c
			 0x714bf7a0, /// 0xa60
			 0x7c2589e2, /// 0xa64
			 0x1a0b7cec, /// 0xa68
			 0xab791b0c, /// 0xa6c
			 0x239f2779, /// 0xa70
			 0x9af5462e, /// 0xa74
			 0xb3b6145f, /// 0xa78
			 0x3632bc61, /// 0xa7c
			 0x1c2e9df1, /// 0xa80
			 0x80e5a699, /// 0xa84
			 0x5fc610f2, /// 0xa88
			 0x159f9913, /// 0xa8c
			 0x16b88952, /// 0xa90
			 0xb5ced494, /// 0xa94
			 0x77d012de, /// 0xa98
			 0x123c5b66, /// 0xa9c
			 0x9c3f0172, /// 0xaa0
			 0x102d6cac, /// 0xaa4
			 0x6046fcfc, /// 0xaa8
			 0xabffa1c1, /// 0xaac
			 0x99f0abdc, /// 0xab0
			 0x11a1a3ce, /// 0xab4
			 0xc224512e, /// 0xab8
			 0x4fd1b679, /// 0xabc
			 0x1ba1f82e, /// 0xac0
			 0xb0d6cdf4, /// 0xac4
			 0xda9db36c, /// 0xac8
			 0x3badc31d, /// 0xacc
			 0xa068fd1e, /// 0xad0
			 0xffeb60f5, /// 0xad4
			 0x1ef6aa7b, /// 0xad8
			 0xaf3c3b10, /// 0xadc
			 0xe319ce37, /// 0xae0
			 0x78809978, /// 0xae4
			 0x01709baa, /// 0xae8
			 0x82c19b0e, /// 0xaec
			 0x16f8ada2, /// 0xaf0
			 0x7d72cc89, /// 0xaf4
			 0x208a1a54, /// 0xaf8
			 0x10494349, /// 0xafc
			 0x61b5846b, /// 0xb00
			 0xc9c96e3f, /// 0xb04
			 0x3c5bc0b9, /// 0xb08
			 0xb6d25ff0, /// 0xb0c
			 0x100e5766, /// 0xb10
			 0x5ea9079d, /// 0xb14
			 0x62ae329e, /// 0xb18
			 0x7a353d63, /// 0xb1c
			 0x8c96e9e3, /// 0xb20
			 0x8d3f7f8b, /// 0xb24
			 0x62d6c095, /// 0xb28
			 0x7028e186, /// 0xb2c
			 0x2511bdf9, /// 0xb30
			 0x896238c4, /// 0xb34
			 0x4827d8a6, /// 0xb38
			 0xf0f18c08, /// 0xb3c
			 0x0bbdd4af, /// 0xb40
			 0xaeb78c9c, /// 0xb44
			 0xdabca880, /// 0xb48
			 0x871e914f, /// 0xb4c
			 0x4a96df00, /// 0xb50
			 0x5aabafc6, /// 0xb54
			 0xd407f134, /// 0xb58
			 0x7858a251, /// 0xb5c
			 0xf5fc9a76, /// 0xb60
			 0x75d3fd1b, /// 0xb64
			 0x08f44195, /// 0xb68
			 0x2e7425b0, /// 0xb6c
			 0xfe97a092, /// 0xb70
			 0xac61c46c, /// 0xb74
			 0xecefc4df, /// 0xb78
			 0xe7f5fb4c, /// 0xb7c
			 0x4438e3db, /// 0xb80
			 0x203106c5, /// 0xb84
			 0x559e0d08, /// 0xb88
			 0xa21582f2, /// 0xb8c
			 0x4cfbe61a, /// 0xb90
			 0x32de52ac, /// 0xb94
			 0xb09ecd83, /// 0xb98
			 0x4f406bb4, /// 0xb9c
			 0xf72f5454, /// 0xba0
			 0x70e96e3b, /// 0xba4
			 0x7d9b99a4, /// 0xba8
			 0xf37d30e7, /// 0xbac
			 0xfc64a972, /// 0xbb0
			 0x2900999e, /// 0xbb4
			 0x2df73403, /// 0xbb8
			 0xc5272ed7, /// 0xbbc
			 0x0174fda8, /// 0xbc0
			 0x8eecd7f3, /// 0xbc4
			 0xfdeccb17, /// 0xbc8
			 0xb98b261f, /// 0xbcc
			 0x83a8078b, /// 0xbd0
			 0xd921fa8b, /// 0xbd4
			 0x6a6e80e7, /// 0xbd8
			 0xb8603fb7, /// 0xbdc
			 0xff95238c, /// 0xbe0
			 0x91d08b98, /// 0xbe4
			 0xe3df6326, /// 0xbe8
			 0x1a2ee15e, /// 0xbec
			 0xed46e65c, /// 0xbf0
			 0x03738692, /// 0xbf4
			 0x8105308d, /// 0xbf8
			 0x5120350c, /// 0xbfc
			 0x2b3c25fb, /// 0xc00
			 0xccaca8d6, /// 0xc04
			 0x7fe3a44a, /// 0xc08
			 0xd6632039, /// 0xc0c
			 0xf2b65ab1, /// 0xc10
			 0x7114d73c, /// 0xc14
			 0x965995f9, /// 0xc18
			 0x794b5178, /// 0xc1c
			 0x0e34d316, /// 0xc20
			 0xdfb4be96, /// 0xc24
			 0x724f9059, /// 0xc28
			 0x8d31381d, /// 0xc2c
			 0xcccd7ed3, /// 0xc30
			 0x43ea7079, /// 0xc34
			 0x709cb17b, /// 0xc38
			 0x35d76b6f, /// 0xc3c
			 0xfe3fb1fa, /// 0xc40
			 0x952d61e7, /// 0xc44
			 0x4b0cfd25, /// 0xc48
			 0x60de72e8, /// 0xc4c
			 0x21227736, /// 0xc50
			 0x2b302d3d, /// 0xc54
			 0x2a584dba, /// 0xc58
			 0x180c51d7, /// 0xc5c
			 0xeea2d582, /// 0xc60
			 0xd74f7595, /// 0xc64
			 0x81beb4d9, /// 0xc68
			 0x73d27903, /// 0xc6c
			 0x2c1536c1, /// 0xc70
			 0xaa68d25c, /// 0xc74
			 0x9ac3dd40, /// 0xc78
			 0xfb225421, /// 0xc7c
			 0xe133927b, /// 0xc80
			 0xcfb24dec, /// 0xc84
			 0xeb9c7acb, /// 0xc88
			 0x0f78c0ec, /// 0xc8c
			 0x164239f3, /// 0xc90
			 0xdaec9504, /// 0xc94
			 0xdec3cb65, /// 0xc98
			 0xbc7bfd5e, /// 0xc9c
			 0xdd7c3cde, /// 0xca0
			 0xe290b306, /// 0xca4
			 0x11d491e9, /// 0xca8
			 0xf173a8fd, /// 0xcac
			 0x3dda8978, /// 0xcb0
			 0x331a38d6, /// 0xcb4
			 0x89356ad8, /// 0xcb8
			 0xcefc62c0, /// 0xcbc
			 0xa8a9dec4, /// 0xcc0
			 0x43201825, /// 0xcc4
			 0xfc489d86, /// 0xcc8
			 0x47f02b85, /// 0xccc
			 0x7d7fa4ee, /// 0xcd0
			 0x06824c14, /// 0xcd4
			 0xc13c51a1, /// 0xcd8
			 0x21a498f7, /// 0xcdc
			 0x98f8268a, /// 0xce0
			 0xda23e078, /// 0xce4
			 0xff67d8df, /// 0xce8
			 0x46987855, /// 0xcec
			 0x7af07de6, /// 0xcf0
			 0x8754fe13, /// 0xcf4
			 0xa48b5327, /// 0xcf8
			 0x211c16ce, /// 0xcfc
			 0xa4f24080, /// 0xd00
			 0x434231d6, /// 0xd04
			 0xa05510fe, /// 0xd08
			 0xf8d75fc2, /// 0xd0c
			 0x84c3a6fc, /// 0xd10
			 0x81833dcf, /// 0xd14
			 0xb525d7b8, /// 0xd18
			 0xb11c9f0b, /// 0xd1c
			 0x235dabb0, /// 0xd20
			 0x79f0e2b0, /// 0xd24
			 0x4fa853e8, /// 0xd28
			 0x34090717, /// 0xd2c
			 0x5564099e, /// 0xd30
			 0x5884e844, /// 0xd34
			 0xfb5ba6f7, /// 0xd38
			 0xd66176f9, /// 0xd3c
			 0xc39ee992, /// 0xd40
			 0x1a41574f, /// 0xd44
			 0xc690db4b, /// 0xd48
			 0x538ac35d, /// 0xd4c
			 0x1ca0c2cd, /// 0xd50
			 0x2922a861, /// 0xd54
			 0x49501a9a, /// 0xd58
			 0x1fbc838a, /// 0xd5c
			 0xb159b0a2, /// 0xd60
			 0xc721e7eb, /// 0xd64
			 0x9c1d902f, /// 0xd68
			 0x710828cb, /// 0xd6c
			 0x6cd3ef00, /// 0xd70
			 0xa52d34af, /// 0xd74
			 0x00bf9ba6, /// 0xd78
			 0x045329f5, /// 0xd7c
			 0xb39e7dad, /// 0xd80
			 0xca42b786, /// 0xd84
			 0x23230e94, /// 0xd88
			 0xe7d43bf5, /// 0xd8c
			 0x3bb85e38, /// 0xd90
			 0x74a2ba77, /// 0xd94
			 0xaeb0d3e9, /// 0xd98
			 0xff325b01, /// 0xd9c
			 0x1090c033, /// 0xda0
			 0x55646060, /// 0xda4
			 0x850b561f, /// 0xda8
			 0x57f7cc46, /// 0xdac
			 0x197500e1, /// 0xdb0
			 0x8f3c1bd7, /// 0xdb4
			 0x11607b97, /// 0xdb8
			 0x3dff9aaf, /// 0xdbc
			 0xa78dd441, /// 0xdc0
			 0xd1224fae, /// 0xdc4
			 0x5fd345c3, /// 0xdc8
			 0xcc8073ff, /// 0xdcc
			 0xd3bf3c33, /// 0xdd0
			 0xe806521d, /// 0xdd4
			 0xa4835fd0, /// 0xdd8
			 0xc4cde3a5, /// 0xddc
			 0xcdbd3649, /// 0xde0
			 0x02e9d570, /// 0xde4
			 0xf12cf755, /// 0xde8
			 0x53b5aa1c, /// 0xdec
			 0xe97fe1af, /// 0xdf0
			 0xf52e14a2, /// 0xdf4
			 0x0b760585, /// 0xdf8
			 0xdc89f423, /// 0xdfc
			 0xddd075f0, /// 0xe00
			 0xee9b0dc0, /// 0xe04
			 0x6cd8e5e9, /// 0xe08
			 0xdfcf21ff, /// 0xe0c
			 0xcc7a6ab8, /// 0xe10
			 0x10903bd3, /// 0xe14
			 0x5abd7b1e, /// 0xe18
			 0x829eea94, /// 0xe1c
			 0x2d4cf7a1, /// 0xe20
			 0xee62a271, /// 0xe24
			 0x118c152c, /// 0xe28
			 0x710fa02e, /// 0xe2c
			 0x482d4255, /// 0xe30
			 0x45b557b4, /// 0xe34
			 0xdd44e6a1, /// 0xe38
			 0x8c897587, /// 0xe3c
			 0x7607a16b, /// 0xe40
			 0xd54af440, /// 0xe44
			 0x9ebc26bc, /// 0xe48
			 0x09640988, /// 0xe4c
			 0xe1afff59, /// 0xe50
			 0x28425c53, /// 0xe54
			 0x78a47c81, /// 0xe58
			 0xd32506ac, /// 0xe5c
			 0x24fabd78, /// 0xe60
			 0xbf69469d, /// 0xe64
			 0xbf65ad4e, /// 0xe68
			 0xf26c4676, /// 0xe6c
			 0x66cdaba2, /// 0xe70
			 0x1bc36336, /// 0xe74
			 0x98bdbe5d, /// 0xe78
			 0xbb862ba0, /// 0xe7c
			 0xe4b23c06, /// 0xe80
			 0x14ec850c, /// 0xe84
			 0x1ff35ded, /// 0xe88
			 0x6588b4a0, /// 0xe8c
			 0xdbb12361, /// 0xe90
			 0x16b2b335, /// 0xe94
			 0x5df54fb6, /// 0xe98
			 0x9d4317fe, /// 0xe9c
			 0x1b4ce917, /// 0xea0
			 0x2bedc89b, /// 0xea4
			 0xe75863bc, /// 0xea8
			 0x1a7383a0, /// 0xeac
			 0x7bbff3d6, /// 0xeb0
			 0x4e2b32ba, /// 0xeb4
			 0x55aba73b, /// 0xeb8
			 0xf60ef611, /// 0xebc
			 0xa38c3c55, /// 0xec0
			 0xec849ee3, /// 0xec4
			 0xb502383b, /// 0xec8
			 0xc800537d, /// 0xecc
			 0xa2ac2a63, /// 0xed0
			 0xf671d7b2, /// 0xed4
			 0x7bc0fe69, /// 0xed8
			 0xd698776d, /// 0xedc
			 0xb91abb70, /// 0xee0
			 0xc3d5c800, /// 0xee4
			 0x8b492218, /// 0xee8
			 0x240443bb, /// 0xeec
			 0x1081c65b, /// 0xef0
			 0x94d51b02, /// 0xef4
			 0x089d484a, /// 0xef8
			 0x2681cf07, /// 0xefc
			 0xc63ec03d, /// 0xf00
			 0xd37d322b, /// 0xf04
			 0xc58f33c3, /// 0xf08
			 0x8b9c7dda, /// 0xf0c
			 0xa26dc2fc, /// 0xf10
			 0xe8bde86a, /// 0xf14
			 0xd24c9584, /// 0xf18
			 0x70f3b0e7, /// 0xf1c
			 0xba2ec1c0, /// 0xf20
			 0x3765ec44, /// 0xf24
			 0x9095de7a, /// 0xf28
			 0xd5c175d8, /// 0xf2c
			 0x2525dd06, /// 0xf30
			 0xa4dd3a9f, /// 0xf34
			 0x17691216, /// 0xf38
			 0x9b9d24df, /// 0xf3c
			 0x3fc242d7, /// 0xf40
			 0x9248b071, /// 0xf44
			 0x189eecf9, /// 0xf48
			 0xcb6d899a, /// 0xf4c
			 0xb87339d4, /// 0xf50
			 0x108ced3e, /// 0xf54
			 0x5d9e46fa, /// 0xf58
			 0xd0db9977, /// 0xf5c
			 0x6c6a718b, /// 0xf60
			 0x5c8f81f7, /// 0xf64
			 0xd2703f9e, /// 0xf68
			 0x828a5c6f, /// 0xf6c
			 0x2fe06e70, /// 0xf70
			 0x990ea8f0, /// 0xf74
			 0x05a39ad9, /// 0xf78
			 0x8ba34c18, /// 0xf7c
			 0xd2490b14, /// 0xf80
			 0xb10d5c18, /// 0xf84
			 0x32b1ab13, /// 0xf88
			 0x6c391fe4, /// 0xf8c
			 0xa7bea4ab, /// 0xf90
			 0x8908beb8, /// 0xf94
			 0x150fb611, /// 0xf98
			 0x14638bec, /// 0xf9c
			 0x58af24f5, /// 0xfa0
			 0x2ef1057e, /// 0xfa4
			 0x6571f153, /// 0xfa8
			 0xa6c25220, /// 0xfac
			 0x50a15a2a, /// 0xfb0
			 0x67361307, /// 0xfb4
			 0xcb6b307d, /// 0xfb8
			 0x0558dbb2, /// 0xfbc
			 0x106befe9, /// 0xfc0
			 0xcaf0badc, /// 0xfc4
			 0x6d2d897f, /// 0xfc8
			 0xc1553ad2, /// 0xfcc
			 0x0bfc366e, /// 0xfd0
			 0x848293aa, /// 0xfd4
			 0xd3a89da5, /// 0xfd8
			 0x2dd56263, /// 0xfdc
			 0x83a1f8c0, /// 0xfe0
			 0x4c3da97a, /// 0xfe4
			 0x837fd925, /// 0xfe8
			 0xf8da2401, /// 0xfec
			 0xaa6888ba, /// 0xff0
			 0x849bfc7e, /// 0xff4
			 0x0c2e59c7, /// 0xff8
			 0x65529108 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00000
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0000c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00010
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00014
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00020
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00028
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0002c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0xff800000,                                                  // -inf                                        /// 00034
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x3f028f5c,                                                  // 0.51                                        /// 0003c
			 0x80000000,                                                  // -zero                                       /// 00040
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00048
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0004c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00050
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00054
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00060
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00064
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00068
			 0x4b000000,                                                  // 8388608.0                                   /// 0006c
			 0xff800000,                                                  // -inf                                        /// 00070
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00074
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00011111,                                                  // 9.7958E-41                                  /// 0007c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00080
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00084
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0008c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00090
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00094
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00098
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0009c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0xbf028f5c,                                                  // -0.51                                       /// 000a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x3f028f5c,                                                  // 0.51                                        /// 000ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00100
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00104
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00108
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0010c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00110
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00118
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0011c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00120
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00128
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00130
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00134
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00138
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0013c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00140
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00144
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00148
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00150
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00154
			 0x0e000003,                                                  // Trailing 1s:                                /// 00158
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0015c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00164
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0016c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00170
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00184
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00188
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00194
			 0x00011111,                                                  // 9.7958E-41                                  /// 00198
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0019c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x4b000000,                                                  // 8388608.0                                   /// 001c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001d0
			 0x80000000,                                                  // -zero                                       /// 001d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00200
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00204
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00208
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00210
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00214
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0021c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00230
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00238
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00240
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0024c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00254
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00258
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0025c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00260
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00264
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00268
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0026c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00270
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00274
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00280
			 0x33333333,                                                  // 4 random values                             /// 00284
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00288
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0028c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00290
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00294
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00298
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002b8
			 0x80000000,                                                  // -zero                                       /// 002bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00300
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00304
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00308
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0030c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00314
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00318
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00324
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00338
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0033c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00340
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00344
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00348
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00350
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00354
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00358
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00360
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00364
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00370
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00374
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0037c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00388
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0038c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0039c
			 0x00000000,                                                  // zero                                        /// 003a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003b4
			 0x7f800000,                                                  // inf                                         /// 003b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 003bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00400
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0c600000,                                                  // Leading 1s:                                 /// 00408
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0xffc00001,                                                  // -signaling NaN                              /// 00410
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00414
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00418
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0041c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00428
			 0x55555555,                                                  // 4 random values                             /// 0042c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00430
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00434
			 0x80011111,                                                  // -9.7958E-41                                 /// 00438
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0045c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00464
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00468
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00478
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00480
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00484
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0048c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00490
			 0x55555555,                                                  // 4 random values                             /// 00494
			 0x80000000,                                                  // -zero                                       /// 00498
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 004e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00500
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00504
			 0x4b000000,                                                  // 8388608.0                                   /// 00508
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00510
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00514
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00518
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0051c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00520
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00524
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00528
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0052c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00530
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00534
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00538
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0053c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00540
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00544
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00548
			 0x80011111,                                                  // -9.7958E-41                                 /// 0054c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00554
			 0x33333333,                                                  // 4 random values                             /// 00558
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0055c
			 0x33333333,                                                  // 4 random values                             /// 00560
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00564
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00568
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00574
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0057c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00584
			 0x0c780000,                                                  // Leading 1s:                                 /// 00588
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0058c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00590
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00594
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0059c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e0
			 0xbf028f5c,                                                  // -0.51                                       /// 005e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 005f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00600
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00608
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0060c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00610
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00618
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0061c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00624
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0062c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00634
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x33333333,                                                  // 4 random values                             /// 00640
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0xbf028f5c,                                                  // -0.51                                       /// 00648
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0064c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00650
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00654
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00658
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0065c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00660
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00664
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00668
			 0x55555555,                                                  // 4 random values                             /// 0066c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x80011111,                                                  // -9.7958E-41                                 /// 00674
			 0x0c400000,                                                  // Leading 1s:                                 /// 00678
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0067c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00680
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00688
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0068c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00698
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0069c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006f0
			 0xcb000000,                                                  // -8388608.0                                  /// 006f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00704
			 0xffc00001,                                                  // -signaling NaN                              /// 00708
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x33333333,                                                  // 4 random values                             /// 00710
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00714
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00718
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0071c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00724
			 0xff800000,                                                  // -inf                                        /// 00728
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0072c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00730
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00734
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00740
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00748
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0074c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00750
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00754
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x7f800000,                                                  // inf                                         /// 0075c
			 0x33333333,                                                  // 4 random values                             /// 00760
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00764
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00768
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0076c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00770
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00778
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0077c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00780
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0xffc00001,                                                  // -signaling NaN                              /// 00788
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00790
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x80000000,                                                  // -zero                                       /// 00798
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0c700000,                                                  // Leading 1s:                                 /// 007a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007f0
			 0xbf028f5c,                                                  // -0.51                                       /// 007f4
			 0x00000000,                                                  // zero                                        /// 007f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00800
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00804
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00808
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0080c
			 0xff800000,                                                  // -inf                                        /// 00810
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00814
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00818
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00820
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x80011111,                                                  // -9.7958E-41                                 /// 00828
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0082c
			 0x4b000000,                                                  // 8388608.0                                   /// 00830
			 0x0c600000,                                                  // Leading 1s:                                 /// 00834
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00838
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0084c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x7f800000,                                                  // inf                                         /// 0085c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00860
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0086c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00870
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0087c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00880
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00884
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0088c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00890
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00898
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008f0
			 0x00000000,                                                  // zero                                        /// 008f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00900
			 0x00011111,                                                  // 9.7958E-41                                  /// 00904
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00910
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0091c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00920
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00924
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00928
			 0x3f028f5c,                                                  // 0.51                                        /// 0092c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00930
			 0x0c700000,                                                  // Leading 1s:                                 /// 00934
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0093c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00948
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00958
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0095c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00970
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00974
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0097c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00980
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00984
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00988
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0098c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0099c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00a00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x55555555,                                                  // 4 random values                             /// 00a10
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a18
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a38
			 0x33333333,                                                  // 4 random values                             /// 00a3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a48
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a4c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a54
			 0x3f028f5c,                                                  // 0.51                                        /// 00a58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a60
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a9c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aa0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aa4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00aa8
			 0x55555555,                                                  // 4 random values                             /// 00aac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ab0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ab4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00abc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ac8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00acc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ad0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ae0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ae4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ae8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00af0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00af4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00afc
			 0x33333333,                                                  // 4 random values                             /// 00b00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b80
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b8c
			 0x7f800000,                                                  // inf                                         /// 00b90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ba0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ba4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x55555555,                                                  // 4 random values                             /// 00bac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bb0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bb4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bb8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bc8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0xffc00001,                                                  // -signaling NaN                              /// 00bd0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bdc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00be0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00be4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bf8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c00
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c14
			 0x7fc00001,                                                  // signaling NaN                               /// 00c18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c30
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c34
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c38
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c68
			 0x55555555,                                                  // 4 random values                             /// 00c6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ca0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cb0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cc0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ccc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cd4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cdc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ce0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cf0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cf4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cf8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cfc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d10
			 0xffc00001,                                                  // -signaling NaN                              /// 00d14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d20
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d30
			 0x7fc00001,                                                  // signaling NaN                               /// 00d34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d38
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d5c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d68
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d70
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00da0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0xff800000,                                                  // -inf                                        /// 00da8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00db0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00db4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dc4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dc8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dcc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dd8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00de0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00de4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00de8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00df0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00df4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dfc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e40
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e8c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ea0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ea4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ea8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00eb4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eb8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ec0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ec8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ed4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00edc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ef0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f34
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f64
			 0x4b000000,                                                  // 8388608.0                                   /// 00f68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f98
			 0x7f800000,                                                  // inf                                         /// 00f9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fa4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fa8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fb8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fbc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fc4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fcc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fd0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fd4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fd8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fe0
			 0x7f800000,                                                  // inf                                         /// 00fe4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x8f7ffffc                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00000
			 0x0c700000,                                                  // Leading 1s:                                 /// 00004
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0000c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00010
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00020
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00024
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0002c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00030
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00034
			 0x33333333,                                                  // 4 random values                             /// 00038
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0003c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00040
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00044
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00048
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0004c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00058
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0005c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00064
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80011111,                                                  // -9.7958E-41                                 /// 0006c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00078
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00080
			 0x55555555,                                                  // 4 random values                             /// 00084
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00088
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00090
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00094
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00098
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000e8
			 0x3f028f5c,                                                  // 0.51                                        /// 000ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x80000000,                                                  // -zero                                       /// 000fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00100
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00104
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0010c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00114
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0011c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00128
			 0x7f800000,                                                  // inf                                         /// 0012c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00130
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00134
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00138
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00140
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00150
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00154
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00160
			 0xff800000,                                                  // -inf                                        /// 00164
			 0x0e000003,                                                  // Trailing 1s:                                /// 00168
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0xbf028f5c,                                                  // -0.51                                       /// 00170
			 0x0c700000,                                                  // Leading 1s:                                 /// 00174
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00178
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0017c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00180
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00188
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00190
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00194
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001a8
			 0xffc00001,                                                  // -signaling NaN                              /// 001ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001c4
			 0x4b000000,                                                  // 8388608.0                                   /// 001c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00204
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00208
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0020c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x00011111,                                                  // 9.7958E-41                                  /// 00214
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0021c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00220
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00224
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0022c
			 0x80000000,                                                  // -zero                                       /// 00230
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00234
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00238
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00240
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0024c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00250
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00258
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0025c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00260
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00264
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00268
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00270
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00274
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00278
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0027c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00280
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00284
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00288
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00290
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00298
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00300
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00304
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00308
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0030c
			 0x80000000,                                                  // -zero                                       /// 00310
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00314
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00318
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00320
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00324
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00328
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00330
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00334
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00344
			 0x0c700000,                                                  // Leading 1s:                                 /// 00348
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x33333333,                                                  // 4 random values                             /// 00354
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00358
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0035c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00364
			 0x33333333,                                                  // 4 random values                             /// 00368
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0036c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00378
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x7f800000,                                                  // inf                                         /// 00380
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00384
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00388
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0038c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00394
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00398
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0039c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003c4
			 0x33333333,                                                  // 4 random values                             /// 003c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x33333333,                                                  // 4 random values                             /// 003fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00400
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00408
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0040c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00410
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00414
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00418
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0041c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00420
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00424
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00428
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0042c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00434
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00438
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0043c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00458
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0045c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0046c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00470
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00474
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00478
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00480
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00484
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00488
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00490
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00494
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00498
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00504
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0050c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00510
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00520
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00524
			 0x55555555,                                                  // 4 random values                             /// 00528
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0052c
			 0x7f800000,                                                  // inf                                         /// 00530
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00534
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00538
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x0e000001,                                                  // Trailing 1s:                                /// 0054c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00550
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00554
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00558
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0055c
			 0x80000000,                                                  // -zero                                       /// 00560
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00564
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00568
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00574
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0057c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00580
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00584
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80000000,                                                  // -zero                                       /// 0058c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00590
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0xff800000,                                                  // -inf                                        /// 00598
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00604
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00608
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0060c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00610
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00614
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00618
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0061c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00620
			 0x0e000007,                                                  // Trailing 1s:                                /// 00624
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00628
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0062c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00640
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00648
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00650
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00654
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0065c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00664
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00668
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0066c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00670
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00678
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0067c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00680
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00684
			 0x80011111,                                                  // -9.7958E-41                                 /// 00688
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00694
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00698
			 0x4b000000,                                                  // 8388608.0                                   /// 0069c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x7fc00001,                                                  // signaling NaN                               /// 006b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00700
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00708
			 0x80011111,                                                  // -9.7958E-41                                 /// 0070c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00710
			 0x7fc00001,                                                  // signaling NaN                               /// 00714
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00718
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00724
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00728
			 0xff800000,                                                  // -inf                                        /// 0072c
			 0x55555555,                                                  // 4 random values                             /// 00730
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00734
			 0x55555555,                                                  // 4 random values                             /// 00738
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0073c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00744
			 0x00011111,                                                  // 9.7958E-41                                  /// 00748
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0074c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00750
			 0x00000000,                                                  // zero                                        /// 00754
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00758
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00760
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00768
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0076c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0xff800000,                                                  // -inf                                        /// 00774
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00778
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0077c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00784
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00794
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0079c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c8
			 0xbf028f5c,                                                  // -0.51                                       /// 007cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007e8
			 0x7f800000,                                                  // inf                                         /// 007ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00804
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00808
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00810
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00814
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00818
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00824
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00828
			 0x0c600000,                                                  // Leading 1s:                                 /// 0082c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00830
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0083c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x4b000000,                                                  // 8388608.0                                   /// 00848
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0084c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00850
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00858
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0085c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00860
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00868
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0086c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00870
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00874
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x4b000000,                                                  // 8388608.0                                   /// 0087c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00888
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0088c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00890
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00894
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00898
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0xff800000,                                                  // -inf                                        /// 008bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008c0
			 0xbf028f5c,                                                  // -0.51                                       /// 008c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80000000,                                                  // -zero                                       /// 008cc
			 0x00000000,                                                  // zero                                        /// 008d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008e4
			 0xcb000000,                                                  // -8388608.0                                  /// 008e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00900
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00904
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x00011111,                                                  // 9.7958E-41                                  /// 0090c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00910
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0091c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00920
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x33333333,                                                  // 4 random values                             /// 00928
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0092c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00930
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0093c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00940
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00944
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00948
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00950
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00954
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00958
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00960
			 0x7fc00001,                                                  // signaling NaN                               /// 00964
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00968
			 0x33333333,                                                  // 4 random values                             /// 0096c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00970
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00974
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00978
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00980
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00988
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0098c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00990
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00994
			 0xffc00001,                                                  // -signaling NaN                              /// 00998
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0099c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0xff800000,                                                  // -inf                                        /// 009dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a1c
			 0x00000000,                                                  // zero                                        /// 00a20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a30
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a58
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a68
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a70
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a88
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aa0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00aa4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ab0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ab8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00abc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ac0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ac4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ac8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ad4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00adc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ae4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ae8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00af0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00af4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00af8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00afc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b0c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0xffc00001,                                                  // -signaling NaN                              /// 00b5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ba8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bb0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00be0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00be4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00be8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bf0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bf4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bf8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bfc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c00
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c1c
			 0x55555555,                                                  // 4 random values                             /// 00c20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c24
			 0xff800000,                                                  // -inf                                        /// 00c28
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c30
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c38
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c54
			 0x80000000,                                                  // -zero                                       /// 00c58
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c6c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c70
			 0x55555555,                                                  // 4 random values                             /// 00c74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c80
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c84
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ca0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ca8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cb0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cc4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x3f028f5c,                                                  // 0.51                                        /// 00ccc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cdc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ce0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ce4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cf0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cf4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cfc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0xcb000000,                                                  // -8388608.0                                  /// 00d28
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d40
			 0x80000000,                                                  // -zero                                       /// 00d44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d5c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d64
			 0x4b000000,                                                  // 8388608.0                                   /// 00d68
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d7c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d80
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d8c
			 0x00000000,                                                  // zero                                        /// 00d90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00da0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0xffc00001,                                                  // -signaling NaN                              /// 00db0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00db4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dbc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dc4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dcc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ddc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00de4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00de8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00df0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00df4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00df8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e08
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00000000,                                                  // zero                                        /// 00e3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e58
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e64
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x33333333,                                                  // 4 random values                             /// 00e78
			 0x4b000000,                                                  // 8388608.0                                   /// 00e7c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ea4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eb0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00eb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0x80000000,                                                  // -zero                                       /// 00ec8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ee0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ee4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ef0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ef4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00efc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f04
			 0xffc00001,                                                  // -signaling NaN                              /// 00f08
			 0xbf028f5c,                                                  // -0.51                                       /// 00f0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x7fc00001,                                                  // signaling NaN                               /// 00f54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f58
			 0xcb000000,                                                  // -8388608.0                                  /// 00f5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0xffc00001,                                                  // -signaling NaN                              /// 00f8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f90
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fa0
			 0x80000000,                                                  // -zero                                       /// 00fa4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fa8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fc4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fcc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fd8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fe0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fe4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fe8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ff0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ff8
			 0x0e000001                                                  // Trailing 1s:                                /// last
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
			 0x00000530,
			 0x00000118,
			 0x000003a0,
			 0x00000018,
			 0x000004a4,
			 0x000005e8,
			 0x000005c8,
			 0x00000260,

			 /// vpu register f2
			 0x41700000,
			 0x41500000,
			 0x41d80000,
			 0x41c80000,
			 0x41200000,
			 0x41e00000,
			 0x41b80000,
			 0x41f00000,

			 /// vpu register f3
			 0x40a00000,
			 0x41800000,
			 0x41900000,
			 0x41100000,
			 0x41600000,
			 0x42000000,
			 0x41e80000,
			 0x42000000,

			 /// vpu register f4
			 0x41c00000,
			 0x41d80000,
			 0x40c00000,
			 0x41d80000,
			 0x41e00000,
			 0x41600000,
			 0x41f00000,
			 0x41700000,

			 /// vpu register f5
			 0x41900000,
			 0x41100000,
			 0x41000000,
			 0x40800000,
			 0x41980000,
			 0x41e00000,
			 0x40800000,
			 0x41e80000,

			 /// vpu register f6
			 0x41c80000,
			 0x40a00000,
			 0x41e00000,
			 0x41b80000,
			 0x41400000,
			 0x40e00000,
			 0x41b00000,
			 0x41300000,

			 /// vpu register f7
			 0x41b80000,
			 0x41d80000,
			 0x40c00000,
			 0x41500000,
			 0x41100000,
			 0x41800000,
			 0x41700000,
			 0x40e00000,

			 /// vpu register f8
			 0x41600000,
			 0x41300000,
			 0x41500000,
			 0x41f80000,
			 0x41f00000,
			 0x41d80000,
			 0x41600000,
			 0x41980000,

			 /// vpu register f9
			 0x40000000,
			 0x41980000,
			 0x41b80000,
			 0x41500000,
			 0x41300000,
			 0x41700000,
			 0x41200000,
			 0x40c00000,

			 /// vpu register f10
			 0x41b00000,
			 0x41500000,
			 0x41880000,
			 0x41500000,
			 0x41a80000,
			 0x41c80000,
			 0x41c80000,
			 0x41b80000,

			 /// vpu register f11
			 0x40400000,
			 0x41100000,
			 0x41a00000,
			 0x41c00000,
			 0x40400000,
			 0x41500000,
			 0x41f80000,
			 0x41c00000,

			 /// vpu register f12
			 0x41880000,
			 0x41f80000,
			 0x41000000,
			 0x41a80000,
			 0x41a00000,
			 0x41e00000,
			 0x40e00000,
			 0x42000000,

			 /// vpu register f13
			 0x41900000,
			 0x41f80000,
			 0x41d80000,
			 0x41c00000,
			 0x40a00000,
			 0x41600000,
			 0x41500000,
			 0x41e00000,

			 /// vpu register f14
			 0x41800000,
			 0x40e00000,
			 0x41d80000,
			 0x41d80000,
			 0x41d00000,
			 0x40c00000,
			 0x41880000,
			 0x41880000,

			 /// vpu register f15
			 0x41800000,
			 0x41800000,
			 0x41980000,
			 0x40c00000,
			 0x41f80000,
			 0x41e00000,
			 0x41e00000,
			 0x40400000,

			 /// vpu register f16
			 0x41500000,
			 0x42000000,
			 0x41600000,
			 0x41c00000,
			 0x40a00000,
			 0x41d80000,
			 0x40a00000,
			 0x41e80000,

			 /// vpu register f17
			 0x3f800000,
			 0x41d80000,
			 0x41f00000,
			 0x40800000,
			 0x41600000,
			 0x41d80000,
			 0x41400000,
			 0x41e00000,

			 /// vpu register f18
			 0x41d00000,
			 0x41880000,
			 0x41900000,
			 0x41500000,
			 0x41600000,
			 0x41f80000,
			 0x41f80000,
			 0x41880000,

			 /// vpu register f19
			 0x40000000,
			 0x41a00000,
			 0x41880000,
			 0x41d00000,
			 0x41100000,
			 0x41f00000,
			 0x40a00000,
			 0x41000000,

			 /// vpu register f20
			 0x41c00000,
			 0x41f00000,
			 0x40400000,
			 0x41100000,
			 0x40e00000,
			 0x41200000,
			 0x40800000,
			 0x41980000,

			 /// vpu register f21
			 0x41f80000,
			 0x41c80000,
			 0x41f00000,
			 0x41100000,
			 0x41a80000,
			 0x41600000,
			 0x41400000,
			 0x41000000,

			 /// vpu register f22
			 0x41700000,
			 0x41300000,
			 0x41200000,
			 0x41b80000,
			 0x41b00000,
			 0x41e80000,
			 0x41880000,
			 0x41e80000,

			 /// vpu register f23
			 0x41600000,
			 0x41c80000,
			 0x41980000,
			 0x41000000,
			 0x41600000,
			 0x41200000,
			 0x41000000,
			 0x41700000,

			 /// vpu register f24
			 0x42000000,
			 0x40800000,
			 0x3f800000,
			 0x41600000,
			 0x41d80000,
			 0x42000000,
			 0x41800000,
			 0x41600000,

			 /// vpu register f25
			 0x41b80000,
			 0x41000000,
			 0x41c00000,
			 0x42000000,
			 0x41d80000,
			 0x41600000,
			 0x42000000,
			 0x41e80000,

			 /// vpu register f26
			 0x41a00000,
			 0x40800000,
			 0x40e00000,
			 0x41200000,
			 0x41f00000,
			 0x41c80000,
			 0x40400000,
			 0x41400000,

			 /// vpu register f27
			 0x42000000,
			 0x41c00000,
			 0x41300000,
			 0x41c80000,
			 0x41300000,
			 0x42000000,
			 0x41880000,
			 0x41e00000,

			 /// vpu register f28
			 0x41b80000,
			 0x41300000,
			 0x41f00000,
			 0x3f800000,
			 0x41b00000,
			 0x41c00000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f29
			 0x41b80000,
			 0x42000000,
			 0x41c00000,
			 0x41b00000,
			 0x41f80000,
			 0x41300000,
			 0x41b00000,
			 0x40800000,

			 /// vpu register f30
			 0x41400000,
			 0x41c00000,
			 0x41b00000,
			 0x41e80000,
			 0x3f800000,
			 0x41d00000,
			 0x40000000,
			 0x3f800000,

			 /// vpu register f31
			 0x41b80000,
			 0x41600000,
			 0x41200000,
			 0x41500000,
			 0x41b80000,
			 0x41700000,
			 0x41c00000,
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
		"flog.ps f21, f7\n"                                   ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f7, f10\n"                                   ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f26, f27\n"                                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f27, f9\n"                                   ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f15, f13\n"                                  ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f25, f1\n"                                   ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f6, f1\n"                                    ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f25, f18\n"                                  ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f28, f18\n"                                  ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f20, f25\n"                                  ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f6, f9\n"                                    ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f10, f4\n"                                   ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f19, f8\n"                                   ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f8, f19\n"                                   ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f22, f7\n"                                   ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f15, f10\n"                                  ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f9, f25\n"                                   ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f27, f27\n"                                  ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f6, f15\n"                                   ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f2, f17\n"                                   ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f8, f22\n"                                   ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f28, f24\n"                                  ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f22, f8\n"                                   ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f15, f25\n"                                  ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f13, f2\n"                                   ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f3, f13\n"                                   ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f19, f26\n"                                  ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f11, f16\n"                                  ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f16, f19\n"                                  ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f26, f29\n"                                  ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f14, f20\n"                                  ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f20, f16\n"                                  ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f22, f12\n"                                  ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f16, f2\n"                                   ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f17, f30\n"                                  ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f6, f24\n"                                   ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f9, f22\n"                                   ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f28, f6\n"                                   ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f20, f27\n"                                  ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f8, f23\n"                                   ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f3, f22\n"                                   ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f4, f27\n"                                   ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f20, f22\n"                                  ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f3, f4\n"                                    ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f21, f15\n"                                  ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f6, f21\n"                                   ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f18, f27\n"                                  ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f11, f10\n"                                  ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f23, f10\n"                                  ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f11, f8\n"                                   ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f16, f13\n"                                  ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f9, f5\n"                                    ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f22, f8\n"                                   ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f10, f13\n"                                  ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f7, f7\n"                                    ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f23, f24\n"                                  ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f22, f20\n"                                  ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f24, f15\n"                                  ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f17, f3\n"                                   ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f24, f9\n"                                   ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f25, f15\n"                                  ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f21, f24\n"                                  ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f6, f29\n"                                   ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f22, f24\n"                                  ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f8, f17\n"                                   ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f18, f16\n"                                  ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f3, f10\n"                                   ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f21, f13\n"                                  ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f19, f3\n"                                   ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f26, f24\n"                                  ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f8, f2\n"                                    ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f11, f23\n"                                  ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f17, f16\n"                                  ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f17, f29\n"                                  ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f30, f11\n"                                  ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f26, f5\n"                                   ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f5, f26\n"                                   ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f20, f29\n"                                  ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f19, f8\n"                                   ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f24, f6\n"                                   ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f20, f16\n"                                  ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f22, f13\n"                                  ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f24, f28\n"                                  ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f28, f20\n"                                  ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f17, f13\n"                                  ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f21, f12\n"                                  ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f27, f19\n"                                  ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f2, f11\n"                                   ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f14, f21\n"                                  ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f16, f29\n"                                  ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f15, f0\n"                                   ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f6, f23\n"                                   ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f20, f1\n"                                   ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f29, f6\n"                                   ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f25, f1\n"                                   ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f11, f29\n"                                  ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f21, f0\n"                                   ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f19, f2\n"                                   ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flog.ps f25, f2\n"                                   ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
