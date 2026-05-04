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
			 0xd851f75f, /// 0x0
			 0xe8c40107, /// 0x4
			 0xa9ee1082, /// 0x8
			 0x9a9a80aa, /// 0xc
			 0x8abcc37a, /// 0x10
			 0x15b502fb, /// 0x14
			 0x9f0ae8f2, /// 0x18
			 0x5c8bd646, /// 0x1c
			 0xb24cd2a5, /// 0x20
			 0x0d2cc525, /// 0x24
			 0x43199ee4, /// 0x28
			 0x3cca44de, /// 0x2c
			 0x2787afff, /// 0x30
			 0xd0d9aa76, /// 0x34
			 0x62f05ef4, /// 0x38
			 0xd3be14c4, /// 0x3c
			 0x4226ae0f, /// 0x40
			 0xb5048c69, /// 0x44
			 0xf994caf6, /// 0x48
			 0xb704b2e3, /// 0x4c
			 0x6f618dca, /// 0x50
			 0xa6155c7f, /// 0x54
			 0xab2ec0a9, /// 0x58
			 0xd60df81b, /// 0x5c
			 0xf5574ee1, /// 0x60
			 0xce89f720, /// 0x64
			 0xd36c7c0b, /// 0x68
			 0x619ca695, /// 0x6c
			 0x32f3c628, /// 0x70
			 0xc146fdfc, /// 0x74
			 0xfc6eb95c, /// 0x78
			 0x9b1041dc, /// 0x7c
			 0xcc29bf22, /// 0x80
			 0x31308026, /// 0x84
			 0xf6b69bbe, /// 0x88
			 0x005266fc, /// 0x8c
			 0xa078e87a, /// 0x90
			 0xec952c4e, /// 0x94
			 0x94740c83, /// 0x98
			 0xe90ecef7, /// 0x9c
			 0x2e6de785, /// 0xa0
			 0x07164744, /// 0xa4
			 0x44b30be8, /// 0xa8
			 0x711a731b, /// 0xac
			 0x4a034423, /// 0xb0
			 0x2044f90d, /// 0xb4
			 0x727a5ad5, /// 0xb8
			 0xb45fa27c, /// 0xbc
			 0xb253f334, /// 0xc0
			 0x5a8c6cb9, /// 0xc4
			 0xa5fefe19, /// 0xc8
			 0x5834bb9e, /// 0xcc
			 0x0cb01962, /// 0xd0
			 0x1058b9e4, /// 0xd4
			 0x33413d2f, /// 0xd8
			 0x435be01a, /// 0xdc
			 0xf9ac06be, /// 0xe0
			 0x087b35a1, /// 0xe4
			 0x3033cbb2, /// 0xe8
			 0x33d5ac37, /// 0xec
			 0xb43b4f14, /// 0xf0
			 0x135f4e9b, /// 0xf4
			 0x579dd431, /// 0xf8
			 0x683e8245, /// 0xfc
			 0xf18468ea, /// 0x100
			 0xa1eb21fd, /// 0x104
			 0xd1a88ccf, /// 0x108
			 0x62f3554f, /// 0x10c
			 0x379a6034, /// 0x110
			 0x5e75f553, /// 0x114
			 0x0bf52339, /// 0x118
			 0x996cba14, /// 0x11c
			 0x09d377e4, /// 0x120
			 0x308249ae, /// 0x124
			 0x911b40f7, /// 0x128
			 0xd7e1b1b0, /// 0x12c
			 0x39da54ac, /// 0x130
			 0xde946b9c, /// 0x134
			 0x5fe2aebc, /// 0x138
			 0x04c83ab3, /// 0x13c
			 0xd2be660a, /// 0x140
			 0x09dbcd51, /// 0x144
			 0x7304de71, /// 0x148
			 0x88d34f90, /// 0x14c
			 0xd382778b, /// 0x150
			 0xbd8ba6c5, /// 0x154
			 0xf88e8770, /// 0x158
			 0x3c669ec2, /// 0x15c
			 0x04cf2c73, /// 0x160
			 0xae5fae27, /// 0x164
			 0x07727fe9, /// 0x168
			 0x00dbfcaa, /// 0x16c
			 0x121381fb, /// 0x170
			 0xc9ce754a, /// 0x174
			 0xc4a5c3b0, /// 0x178
			 0xb4644702, /// 0x17c
			 0xd494b320, /// 0x180
			 0x5ab483c3, /// 0x184
			 0xc195bdde, /// 0x188
			 0x77055d63, /// 0x18c
			 0x8d3f6a2b, /// 0x190
			 0x89345d73, /// 0x194
			 0xc88c6685, /// 0x198
			 0x89aa3355, /// 0x19c
			 0xb5e54677, /// 0x1a0
			 0x4ad63306, /// 0x1a4
			 0xabb01968, /// 0x1a8
			 0x9703b347, /// 0x1ac
			 0x948b435e, /// 0x1b0
			 0x162616b3, /// 0x1b4
			 0x2f64cbfc, /// 0x1b8
			 0xfe27b69d, /// 0x1bc
			 0x76040e43, /// 0x1c0
			 0x3f9cb7de, /// 0x1c4
			 0xc32efb92, /// 0x1c8
			 0xddd68db7, /// 0x1cc
			 0x8489660c, /// 0x1d0
			 0x52fb6f5a, /// 0x1d4
			 0xac30229c, /// 0x1d8
			 0x66b2843e, /// 0x1dc
			 0x47513ca8, /// 0x1e0
			 0xc57fa54d, /// 0x1e4
			 0xaa7eda90, /// 0x1e8
			 0x34d78491, /// 0x1ec
			 0xa14f5714, /// 0x1f0
			 0x458dd623, /// 0x1f4
			 0x281d1d49, /// 0x1f8
			 0x4494f87a, /// 0x1fc
			 0xc00d180c, /// 0x200
			 0x4caba3df, /// 0x204
			 0x6bcc2801, /// 0x208
			 0x120ede29, /// 0x20c
			 0xadb7bc0d, /// 0x210
			 0x3094d23a, /// 0x214
			 0x7c4dd43a, /// 0x218
			 0xadf173e1, /// 0x21c
			 0xfaca3dd1, /// 0x220
			 0xd8ff3688, /// 0x224
			 0x2a9afaba, /// 0x228
			 0x29b87051, /// 0x22c
			 0xd998f6d8, /// 0x230
			 0xad371461, /// 0x234
			 0x760037ea, /// 0x238
			 0x9e28c38d, /// 0x23c
			 0x7b9569dc, /// 0x240
			 0x9e9e12a2, /// 0x244
			 0x29b6d06d, /// 0x248
			 0xb033210f, /// 0x24c
			 0xb10057b2, /// 0x250
			 0x7a90f835, /// 0x254
			 0xe0a589f8, /// 0x258
			 0xf9657ae7, /// 0x25c
			 0xb64c05b0, /// 0x260
			 0xbabb78d8, /// 0x264
			 0x4a415a1a, /// 0x268
			 0xe68c61b5, /// 0x26c
			 0xda267066, /// 0x270
			 0xe5a67569, /// 0x274
			 0x8528bc32, /// 0x278
			 0x8374ef1f, /// 0x27c
			 0x329e5c54, /// 0x280
			 0x54f2aef1, /// 0x284
			 0x29c6f22c, /// 0x288
			 0x69e1a36e, /// 0x28c
			 0x5d5eafed, /// 0x290
			 0xfcfb0034, /// 0x294
			 0x45563df0, /// 0x298
			 0x80a8d664, /// 0x29c
			 0x22a5e156, /// 0x2a0
			 0xfc7c5836, /// 0x2a4
			 0xb7fb5752, /// 0x2a8
			 0x76e18859, /// 0x2ac
			 0xf0546da7, /// 0x2b0
			 0x0ef92867, /// 0x2b4
			 0xb5a65efe, /// 0x2b8
			 0xc951a25e, /// 0x2bc
			 0xf4e02002, /// 0x2c0
			 0x0d7e6d8b, /// 0x2c4
			 0xb1edd2dd, /// 0x2c8
			 0x72618af3, /// 0x2cc
			 0xd3dd70e8, /// 0x2d0
			 0xdfb97437, /// 0x2d4
			 0x193d0701, /// 0x2d8
			 0x857bf973, /// 0x2dc
			 0x29bc2a49, /// 0x2e0
			 0x0c55524e, /// 0x2e4
			 0x8541b933, /// 0x2e8
			 0x18b6e9ea, /// 0x2ec
			 0x93e4a7d4, /// 0x2f0
			 0x153b1eb9, /// 0x2f4
			 0x4f5206a9, /// 0x2f8
			 0xfc4ebf1f, /// 0x2fc
			 0x8d0a8a64, /// 0x300
			 0xf5af2eb6, /// 0x304
			 0x51339997, /// 0x308
			 0x3578e05e, /// 0x30c
			 0xf9e45fca, /// 0x310
			 0xaa751896, /// 0x314
			 0x035e2daa, /// 0x318
			 0xd220aaad, /// 0x31c
			 0x6b20d0a0, /// 0x320
			 0xa5c96259, /// 0x324
			 0x0f43f281, /// 0x328
			 0x82d3bd18, /// 0x32c
			 0xfcc6348c, /// 0x330
			 0xe299e36a, /// 0x334
			 0x68ebedf3, /// 0x338
			 0xe9aad4f4, /// 0x33c
			 0x8778e47a, /// 0x340
			 0x547a3198, /// 0x344
			 0x0631a97d, /// 0x348
			 0x691ef4e4, /// 0x34c
			 0x70616926, /// 0x350
			 0x0a5fe33e, /// 0x354
			 0x56dcdf52, /// 0x358
			 0xaa44d9cd, /// 0x35c
			 0x11ad3d27, /// 0x360
			 0xe0ee3f93, /// 0x364
			 0x201f3141, /// 0x368
			 0x0c906939, /// 0x36c
			 0x19273151, /// 0x370
			 0x507de24b, /// 0x374
			 0xb7fa63a7, /// 0x378
			 0x64f1ae6d, /// 0x37c
			 0xfcd55484, /// 0x380
			 0xc1e2f92b, /// 0x384
			 0xac5c4eaa, /// 0x388
			 0xae0a04db, /// 0x38c
			 0xbb8b94ad, /// 0x390
			 0x48dba2ee, /// 0x394
			 0xef0036ef, /// 0x398
			 0xc6d31b74, /// 0x39c
			 0x4ba115a0, /// 0x3a0
			 0xf97eaf87, /// 0x3a4
			 0x7c4bf2ba, /// 0x3a8
			 0xc4007f46, /// 0x3ac
			 0x56b77c37, /// 0x3b0
			 0x95cc8101, /// 0x3b4
			 0x60ada1fd, /// 0x3b8
			 0x33490c94, /// 0x3bc
			 0x59951e42, /// 0x3c0
			 0xe0a9ab74, /// 0x3c4
			 0xabd5242b, /// 0x3c8
			 0x4e452007, /// 0x3cc
			 0xab9e6236, /// 0x3d0
			 0xe5d8bed9, /// 0x3d4
			 0x3d370033, /// 0x3d8
			 0x53525c0f, /// 0x3dc
			 0x3f21a602, /// 0x3e0
			 0x2c0b315e, /// 0x3e4
			 0x56670dd5, /// 0x3e8
			 0xe396d81b, /// 0x3ec
			 0x42cacc7c, /// 0x3f0
			 0xe2d5cd71, /// 0x3f4
			 0xc58f603b, /// 0x3f8
			 0xbc734504, /// 0x3fc
			 0x830b5087, /// 0x400
			 0x0bf5668c, /// 0x404
			 0x10efde22, /// 0x408
			 0xbcd57f52, /// 0x40c
			 0x91d4b91a, /// 0x410
			 0xaf18ae91, /// 0x414
			 0x9b12d2b0, /// 0x418
			 0xc5c1f341, /// 0x41c
			 0x9775a3c2, /// 0x420
			 0x9d77d7f6, /// 0x424
			 0xb315e485, /// 0x428
			 0x187e0f28, /// 0x42c
			 0xf40d47a3, /// 0x430
			 0xe6127d2c, /// 0x434
			 0x33067925, /// 0x438
			 0xe4c5dfd5, /// 0x43c
			 0x1b63cd72, /// 0x440
			 0x9c00401c, /// 0x444
			 0xb9dca4df, /// 0x448
			 0xa91f958d, /// 0x44c
			 0x1d495278, /// 0x450
			 0xb06cde61, /// 0x454
			 0x2579319c, /// 0x458
			 0x2cf262ea, /// 0x45c
			 0xb99ef40b, /// 0x460
			 0x7c1d5407, /// 0x464
			 0x0a6a5ced, /// 0x468
			 0x0d0b589a, /// 0x46c
			 0x130c33ff, /// 0x470
			 0xd8465059, /// 0x474
			 0xfc39bb37, /// 0x478
			 0x98d120b0, /// 0x47c
			 0x40fb24cf, /// 0x480
			 0x5e705101, /// 0x484
			 0xeb6afc25, /// 0x488
			 0xac09b62c, /// 0x48c
			 0x934e9e40, /// 0x490
			 0xd87c4ca8, /// 0x494
			 0x5aad45d0, /// 0x498
			 0xf2f16a2b, /// 0x49c
			 0xe2bc5323, /// 0x4a0
			 0x3689af27, /// 0x4a4
			 0x5c399132, /// 0x4a8
			 0x5d7d76cd, /// 0x4ac
			 0xe219e638, /// 0x4b0
			 0x0ce96312, /// 0x4b4
			 0xe8febf6e, /// 0x4b8
			 0xe9b09523, /// 0x4bc
			 0xc8d71485, /// 0x4c0
			 0x8d650a04, /// 0x4c4
			 0xa2d76cf3, /// 0x4c8
			 0xace40425, /// 0x4cc
			 0x5b7c70d3, /// 0x4d0
			 0x43a41213, /// 0x4d4
			 0x1f54103d, /// 0x4d8
			 0xe16559a9, /// 0x4dc
			 0x93e155e4, /// 0x4e0
			 0x76dd048d, /// 0x4e4
			 0x76e84648, /// 0x4e8
			 0xbd4ca45b, /// 0x4ec
			 0xa481ace9, /// 0x4f0
			 0xf9c8c9e7, /// 0x4f4
			 0x4ab6fa12, /// 0x4f8
			 0x8c74243b, /// 0x4fc
			 0x00c4e040, /// 0x500
			 0x538fbb52, /// 0x504
			 0x8ac043a7, /// 0x508
			 0x8929d4c2, /// 0x50c
			 0xad4f45f8, /// 0x510
			 0xcb035ef7, /// 0x514
			 0x186a82be, /// 0x518
			 0xb15fd102, /// 0x51c
			 0x4ece8485, /// 0x520
			 0xc49993cb, /// 0x524
			 0x58bf4e45, /// 0x528
			 0x33510f29, /// 0x52c
			 0x7c695be0, /// 0x530
			 0x2d7590eb, /// 0x534
			 0x71fe2391, /// 0x538
			 0xb906f690, /// 0x53c
			 0x11b4c22c, /// 0x540
			 0xb1815e72, /// 0x544
			 0xcaafb31a, /// 0x548
			 0x08224f76, /// 0x54c
			 0x56148434, /// 0x550
			 0xb60f1896, /// 0x554
			 0xc6a99abb, /// 0x558
			 0x8d2ae4d6, /// 0x55c
			 0x49ee420d, /// 0x560
			 0x008b23a9, /// 0x564
			 0x2d936113, /// 0x568
			 0x48e37d12, /// 0x56c
			 0x5014d889, /// 0x570
			 0x39c317e4, /// 0x574
			 0x1dc6bb1a, /// 0x578
			 0x9ebaa710, /// 0x57c
			 0xe31a127b, /// 0x580
			 0x59363dac, /// 0x584
			 0x6a24f379, /// 0x588
			 0x8ed62147, /// 0x58c
			 0x2356cd44, /// 0x590
			 0x74c136f1, /// 0x594
			 0xe6ee5d54, /// 0x598
			 0x49f9ac9a, /// 0x59c
			 0xf8b18534, /// 0x5a0
			 0xf03f024e, /// 0x5a4
			 0xdc8aa693, /// 0x5a8
			 0x978f1e2c, /// 0x5ac
			 0x6ab03ad5, /// 0x5b0
			 0x21759ba4, /// 0x5b4
			 0xf43be3ce, /// 0x5b8
			 0x68a58f13, /// 0x5bc
			 0xc290a0ce, /// 0x5c0
			 0xee89f7f5, /// 0x5c4
			 0x5a131a85, /// 0x5c8
			 0xfa9bed14, /// 0x5cc
			 0x9d66df1b, /// 0x5d0
			 0x08d029e8, /// 0x5d4
			 0x64c3a06d, /// 0x5d8
			 0xb47fc171, /// 0x5dc
			 0xf7ed0c8d, /// 0x5e0
			 0x51cbc7bc, /// 0x5e4
			 0x2b15b8f9, /// 0x5e8
			 0x18b396d2, /// 0x5ec
			 0xa41e125f, /// 0x5f0
			 0x5fdd7bd9, /// 0x5f4
			 0x94d22a67, /// 0x5f8
			 0x83b774c5, /// 0x5fc
			 0x11e5c3e3, /// 0x600
			 0x26732ab0, /// 0x604
			 0x28c60991, /// 0x608
			 0x160e9359, /// 0x60c
			 0xcd8a64a8, /// 0x610
			 0xb9cb5f90, /// 0x614
			 0x202fbe74, /// 0x618
			 0xd06c0911, /// 0x61c
			 0xb16cbb11, /// 0x620
			 0x73eb74c0, /// 0x624
			 0x5e664fe4, /// 0x628
			 0xe1599a21, /// 0x62c
			 0x61bd30f7, /// 0x630
			 0x4f8a8331, /// 0x634
			 0x783bef56, /// 0x638
			 0x6be601bd, /// 0x63c
			 0x918c8726, /// 0x640
			 0x159450fe, /// 0x644
			 0x745a9f66, /// 0x648
			 0xb3d1c60d, /// 0x64c
			 0x61157680, /// 0x650
			 0x26984599, /// 0x654
			 0x747c0067, /// 0x658
			 0x8cc0f636, /// 0x65c
			 0x34e9581c, /// 0x660
			 0x703264e7, /// 0x664
			 0x44213977, /// 0x668
			 0xf647037d, /// 0x66c
			 0xb2c9ccb1, /// 0x670
			 0xabc61c26, /// 0x674
			 0x91673efc, /// 0x678
			 0x7e60bf1e, /// 0x67c
			 0x01d5189b, /// 0x680
			 0xdb7d515a, /// 0x684
			 0x193195d7, /// 0x688
			 0xb759a145, /// 0x68c
			 0xa09ec4c8, /// 0x690
			 0x77a3978a, /// 0x694
			 0x4d133165, /// 0x698
			 0xb7c8f905, /// 0x69c
			 0xf9ed8314, /// 0x6a0
			 0x806bda23, /// 0x6a4
			 0xc55d4a4d, /// 0x6a8
			 0x1f8e8ab1, /// 0x6ac
			 0xcf85802d, /// 0x6b0
			 0x45045cc4, /// 0x6b4
			 0xe03c7328, /// 0x6b8
			 0x5187d887, /// 0x6bc
			 0x60ff3f53, /// 0x6c0
			 0xf0ac6476, /// 0x6c4
			 0xf40146e4, /// 0x6c8
			 0x62823846, /// 0x6cc
			 0xadfa3f48, /// 0x6d0
			 0xa75e5614, /// 0x6d4
			 0xd967690c, /// 0x6d8
			 0xadacead4, /// 0x6dc
			 0x366fa6bd, /// 0x6e0
			 0xb2e634e6, /// 0x6e4
			 0xa060dc8b, /// 0x6e8
			 0x4effe976, /// 0x6ec
			 0x3a5e38ff, /// 0x6f0
			 0xa05451e4, /// 0x6f4
			 0x01a6d3c9, /// 0x6f8
			 0x98329fd9, /// 0x6fc
			 0xb1eb5fec, /// 0x700
			 0x7b661038, /// 0x704
			 0x6adaf846, /// 0x708
			 0xc8158be0, /// 0x70c
			 0x0638d29c, /// 0x710
			 0xf4f95690, /// 0x714
			 0x1b7067c5, /// 0x718
			 0x0c465c44, /// 0x71c
			 0x443e0034, /// 0x720
			 0x4a064a50, /// 0x724
			 0xe325cf14, /// 0x728
			 0x669f24d6, /// 0x72c
			 0xd10c4bc9, /// 0x730
			 0x9e111087, /// 0x734
			 0xebea905e, /// 0x738
			 0x0ac908c4, /// 0x73c
			 0x97d525fc, /// 0x740
			 0x7e65026c, /// 0x744
			 0x9b36dc5f, /// 0x748
			 0x6f8460a1, /// 0x74c
			 0x12056603, /// 0x750
			 0x31677cb2, /// 0x754
			 0xc8143c4c, /// 0x758
			 0xd3763277, /// 0x75c
			 0xc5f342f6, /// 0x760
			 0x911e01e1, /// 0x764
			 0xcb20b3bc, /// 0x768
			 0x22d1e026, /// 0x76c
			 0xe6b9538a, /// 0x770
			 0x37654015, /// 0x774
			 0xe5255333, /// 0x778
			 0x7133b54f, /// 0x77c
			 0xf1b9bb97, /// 0x780
			 0x3848dd77, /// 0x784
			 0x0952d9c1, /// 0x788
			 0x84b5e884, /// 0x78c
			 0xa9b417f0, /// 0x790
			 0xfeccda1c, /// 0x794
			 0xc7619c00, /// 0x798
			 0x2b427dd3, /// 0x79c
			 0x005577fe, /// 0x7a0
			 0x40ade003, /// 0x7a4
			 0xe5508575, /// 0x7a8
			 0x5aba3e28, /// 0x7ac
			 0xb959b232, /// 0x7b0
			 0x6bafc7bb, /// 0x7b4
			 0xfd081a19, /// 0x7b8
			 0xaa7c5a52, /// 0x7bc
			 0x29a6f1f7, /// 0x7c0
			 0xae93974a, /// 0x7c4
			 0xe5781488, /// 0x7c8
			 0x7e1375e3, /// 0x7cc
			 0xa37afd31, /// 0x7d0
			 0x25372431, /// 0x7d4
			 0xced10d8b, /// 0x7d8
			 0x75ff2226, /// 0x7dc
			 0x0fcc0b3b, /// 0x7e0
			 0xa8d6395d, /// 0x7e4
			 0xba97a0d3, /// 0x7e8
			 0x70c8e093, /// 0x7ec
			 0x72c44374, /// 0x7f0
			 0x76fbf1f8, /// 0x7f4
			 0xe04b573a, /// 0x7f8
			 0xf982b5e9, /// 0x7fc
			 0xd6d15b22, /// 0x800
			 0x07aece1e, /// 0x804
			 0x30b7b6fe, /// 0x808
			 0x3643d592, /// 0x80c
			 0x02997ebc, /// 0x810
			 0xb607552c, /// 0x814
			 0xb620cdce, /// 0x818
			 0x454e36b8, /// 0x81c
			 0xbcea3e8c, /// 0x820
			 0xb794b913, /// 0x824
			 0x3e99226f, /// 0x828
			 0x214e0383, /// 0x82c
			 0x80d19261, /// 0x830
			 0x843d8a8b, /// 0x834
			 0xd1bb9f3a, /// 0x838
			 0x6c5578ee, /// 0x83c
			 0x2e6c55c7, /// 0x840
			 0xfa08e44e, /// 0x844
			 0xdff776de, /// 0x848
			 0xddb5ff73, /// 0x84c
			 0x7a3ce4cd, /// 0x850
			 0xd192c0f8, /// 0x854
			 0x8a5ef558, /// 0x858
			 0x27c50ab9, /// 0x85c
			 0x263fe3f6, /// 0x860
			 0xc44a6d2c, /// 0x864
			 0x397d9cd3, /// 0x868
			 0x64fec299, /// 0x86c
			 0x30512eea, /// 0x870
			 0xd64e1053, /// 0x874
			 0x831cf64e, /// 0x878
			 0x118f36a2, /// 0x87c
			 0xbde8664d, /// 0x880
			 0xc8092a35, /// 0x884
			 0xb0e87fcd, /// 0x888
			 0x1679a6fe, /// 0x88c
			 0xf1eb1a4e, /// 0x890
			 0xfa68139b, /// 0x894
			 0xc93e5e2a, /// 0x898
			 0xc6114dff, /// 0x89c
			 0x87a96e61, /// 0x8a0
			 0xebba7c9e, /// 0x8a4
			 0x61428c64, /// 0x8a8
			 0x0bd741e2, /// 0x8ac
			 0x94c26a51, /// 0x8b0
			 0x0c1fd141, /// 0x8b4
			 0x7a8288b6, /// 0x8b8
			 0xc4de62ec, /// 0x8bc
			 0x1327493a, /// 0x8c0
			 0x4ea3a209, /// 0x8c4
			 0xb7725e69, /// 0x8c8
			 0xfb82f473, /// 0x8cc
			 0xc49bf67c, /// 0x8d0
			 0x195785d2, /// 0x8d4
			 0xc2ce4f31, /// 0x8d8
			 0x6c08ee34, /// 0x8dc
			 0xee545c30, /// 0x8e0
			 0x63bd9eaf, /// 0x8e4
			 0x7e344945, /// 0x8e8
			 0x1613f450, /// 0x8ec
			 0x8677941a, /// 0x8f0
			 0x1d6928a7, /// 0x8f4
			 0x04d8a9fe, /// 0x8f8
			 0x9f9dfc67, /// 0x8fc
			 0x95f6a5ad, /// 0x900
			 0xc1272dbc, /// 0x904
			 0x3275db7d, /// 0x908
			 0x229830ee, /// 0x90c
			 0xfcb1efce, /// 0x910
			 0xd44d09cc, /// 0x914
			 0x632c8f75, /// 0x918
			 0x89f99378, /// 0x91c
			 0x7efe45ea, /// 0x920
			 0x659776fa, /// 0x924
			 0xbefed557, /// 0x928
			 0x14fa8358, /// 0x92c
			 0x941d01e5, /// 0x930
			 0xf910eda8, /// 0x934
			 0x73acc394, /// 0x938
			 0x47556908, /// 0x93c
			 0x6c166a44, /// 0x940
			 0x1d7e7cde, /// 0x944
			 0xe2d7d6b7, /// 0x948
			 0xb255c07e, /// 0x94c
			 0xfea3460c, /// 0x950
			 0x4cfc426a, /// 0x954
			 0x5c5a2bb9, /// 0x958
			 0x628aac8d, /// 0x95c
			 0x87dbb5ad, /// 0x960
			 0xdef6e85c, /// 0x964
			 0x7eb19aa5, /// 0x968
			 0x6d9c79ea, /// 0x96c
			 0x09254fa2, /// 0x970
			 0xd26d7f7a, /// 0x974
			 0xa2c0f9f6, /// 0x978
			 0x650f1f25, /// 0x97c
			 0x313f9a21, /// 0x980
			 0xe9f48d0f, /// 0x984
			 0xeff5d0e4, /// 0x988
			 0x973c41fe, /// 0x98c
			 0xdf7099c7, /// 0x990
			 0x3a9b81ca, /// 0x994
			 0x4072e5cb, /// 0x998
			 0x15f5778a, /// 0x99c
			 0x19ae4930, /// 0x9a0
			 0x848ab8ab, /// 0x9a4
			 0xdaffb76b, /// 0x9a8
			 0x81c718e5, /// 0x9ac
			 0xceb88d71, /// 0x9b0
			 0x41e4cbeb, /// 0x9b4
			 0xae6c8250, /// 0x9b8
			 0xe2479357, /// 0x9bc
			 0x737b85af, /// 0x9c0
			 0xcf5804fc, /// 0x9c4
			 0x98253984, /// 0x9c8
			 0x2d2c655c, /// 0x9cc
			 0x56816435, /// 0x9d0
			 0xde0dbcaa, /// 0x9d4
			 0xa0934501, /// 0x9d8
			 0xa9ce17a6, /// 0x9dc
			 0x647c8a1e, /// 0x9e0
			 0x84d7eb58, /// 0x9e4
			 0x68e5ba26, /// 0x9e8
			 0x522a1979, /// 0x9ec
			 0x5c7016de, /// 0x9f0
			 0x7d03c21b, /// 0x9f4
			 0x06298646, /// 0x9f8
			 0xc08efdd5, /// 0x9fc
			 0x1d652830, /// 0xa00
			 0x3eaed17e, /// 0xa04
			 0xc71a445f, /// 0xa08
			 0x01d87cc1, /// 0xa0c
			 0x4dcdc6f9, /// 0xa10
			 0x53e6a542, /// 0xa14
			 0xcb4d46fd, /// 0xa18
			 0x90915280, /// 0xa1c
			 0xfc8cb83f, /// 0xa20
			 0x42243e43, /// 0xa24
			 0xb92310a8, /// 0xa28
			 0xaec9cd1c, /// 0xa2c
			 0xf2631321, /// 0xa30
			 0xf8e4f664, /// 0xa34
			 0xe4039530, /// 0xa38
			 0x4d0661ec, /// 0xa3c
			 0x710bad76, /// 0xa40
			 0xa8c7852d, /// 0xa44
			 0x604ef47f, /// 0xa48
			 0xa48ac687, /// 0xa4c
			 0x81563e49, /// 0xa50
			 0x3603955b, /// 0xa54
			 0xde3a9181, /// 0xa58
			 0xc47f2a91, /// 0xa5c
			 0x5b3107f5, /// 0xa60
			 0x60507a80, /// 0xa64
			 0xc657631a, /// 0xa68
			 0x68791561, /// 0xa6c
			 0x46e41539, /// 0xa70
			 0x814da4b4, /// 0xa74
			 0x29732e63, /// 0xa78
			 0xd25b7438, /// 0xa7c
			 0xa7e9b660, /// 0xa80
			 0x62f900cc, /// 0xa84
			 0xd102c605, /// 0xa88
			 0x488dd612, /// 0xa8c
			 0x518e7d74, /// 0xa90
			 0x35b8613f, /// 0xa94
			 0xad2d222f, /// 0xa98
			 0xf973d7e0, /// 0xa9c
			 0x27d3764b, /// 0xaa0
			 0x4703a1d0, /// 0xaa4
			 0x690d0d54, /// 0xaa8
			 0xfffa7a02, /// 0xaac
			 0xfdc8cb1d, /// 0xab0
			 0x2689a4f8, /// 0xab4
			 0x9f278f99, /// 0xab8
			 0x3ca66c32, /// 0xabc
			 0x085b8f0f, /// 0xac0
			 0x2b480587, /// 0xac4
			 0xb5a04e9a, /// 0xac8
			 0x0925ec08, /// 0xacc
			 0xcef985cf, /// 0xad0
			 0x989cd1ae, /// 0xad4
			 0xf9de9f4a, /// 0xad8
			 0x439cf252, /// 0xadc
			 0xafa1c637, /// 0xae0
			 0x64b7e23f, /// 0xae4
			 0xf4a07d50, /// 0xae8
			 0x337e80b6, /// 0xaec
			 0xaaacf8ca, /// 0xaf0
			 0x64ec741f, /// 0xaf4
			 0x9fc586d0, /// 0xaf8
			 0x9b5861a1, /// 0xafc
			 0x0b90a203, /// 0xb00
			 0xb1313f6a, /// 0xb04
			 0xaa00b27a, /// 0xb08
			 0xc4170b6e, /// 0xb0c
			 0xedc0ffb8, /// 0xb10
			 0x2ced1200, /// 0xb14
			 0xaf51f164, /// 0xb18
			 0x2f7ad87a, /// 0xb1c
			 0xe722dfe9, /// 0xb20
			 0xf3f9608d, /// 0xb24
			 0x34b24246, /// 0xb28
			 0xa40af6fd, /// 0xb2c
			 0x186d13a3, /// 0xb30
			 0x785193dc, /// 0xb34
			 0x56bcbf90, /// 0xb38
			 0x71046992, /// 0xb3c
			 0x3cb6d113, /// 0xb40
			 0x092db4bf, /// 0xb44
			 0xc0a019e2, /// 0xb48
			 0x36634076, /// 0xb4c
			 0x2e6bde4b, /// 0xb50
			 0x36d3f0a8, /// 0xb54
			 0x8da9ef36, /// 0xb58
			 0x2006a56d, /// 0xb5c
			 0xbe6df0f1, /// 0xb60
			 0xc867c51f, /// 0xb64
			 0x856b85f3, /// 0xb68
			 0x7f33fb60, /// 0xb6c
			 0x1d3da493, /// 0xb70
			 0x9d17db57, /// 0xb74
			 0x07d86d13, /// 0xb78
			 0xcb57fccf, /// 0xb7c
			 0x2a4cab35, /// 0xb80
			 0xbe4feef8, /// 0xb84
			 0xefc6d234, /// 0xb88
			 0xbb03e0a5, /// 0xb8c
			 0xe91945a1, /// 0xb90
			 0xeea9a627, /// 0xb94
			 0x664856e9, /// 0xb98
			 0xb2c18d4e, /// 0xb9c
			 0xd77d749c, /// 0xba0
			 0x2b3b730d, /// 0xba4
			 0x1ca9ca4f, /// 0xba8
			 0xf9b97836, /// 0xbac
			 0x073c3e5a, /// 0xbb0
			 0xa08b3205, /// 0xbb4
			 0xd636025c, /// 0xbb8
			 0x5ab28101, /// 0xbbc
			 0x6c11ea26, /// 0xbc0
			 0x9e79a106, /// 0xbc4
			 0x3aeed000, /// 0xbc8
			 0xbb268f58, /// 0xbcc
			 0x05b8eb41, /// 0xbd0
			 0x5facebff, /// 0xbd4
			 0x69c3adfe, /// 0xbd8
			 0x93d2d1cf, /// 0xbdc
			 0x7606abeb, /// 0xbe0
			 0xd9a0ac21, /// 0xbe4
			 0xc884af3f, /// 0xbe8
			 0x95f78cb1, /// 0xbec
			 0x56d1f7f4, /// 0xbf0
			 0xfb0ad10b, /// 0xbf4
			 0xab67fd35, /// 0xbf8
			 0xe571fc53, /// 0xbfc
			 0xb5677d14, /// 0xc00
			 0x979f3d97, /// 0xc04
			 0x5a1ebfe7, /// 0xc08
			 0xfa31a882, /// 0xc0c
			 0x80da9eb2, /// 0xc10
			 0xc4d9f4a0, /// 0xc14
			 0x6003efbc, /// 0xc18
			 0xff7343d3, /// 0xc1c
			 0x8db347fe, /// 0xc20
			 0x38d937c2, /// 0xc24
			 0x5cec203d, /// 0xc28
			 0xe631d6ff, /// 0xc2c
			 0x0033a01e, /// 0xc30
			 0xe8135767, /// 0xc34
			 0xa53eb4bd, /// 0xc38
			 0x2bc73961, /// 0xc3c
			 0x36dde80a, /// 0xc40
			 0x082e778c, /// 0xc44
			 0xb19da0f0, /// 0xc48
			 0xcc84876b, /// 0xc4c
			 0xc834dc80, /// 0xc50
			 0xce73f0bb, /// 0xc54
			 0x2cc246fa, /// 0xc58
			 0x0e92034f, /// 0xc5c
			 0x4ea938fd, /// 0xc60
			 0xf8c6783f, /// 0xc64
			 0xb3448b8f, /// 0xc68
			 0xe4601512, /// 0xc6c
			 0x34a8c697, /// 0xc70
			 0x6a357b3c, /// 0xc74
			 0x270bb0a2, /// 0xc78
			 0xf4c75f78, /// 0xc7c
			 0x77fd884b, /// 0xc80
			 0x0c58cf5c, /// 0xc84
			 0x7bb12cb6, /// 0xc88
			 0x3dd46eb1, /// 0xc8c
			 0x6bf03026, /// 0xc90
			 0xce95dc5d, /// 0xc94
			 0x4e29632d, /// 0xc98
			 0x972c1fdf, /// 0xc9c
			 0xa864934d, /// 0xca0
			 0x31c0560b, /// 0xca4
			 0x2bee6af4, /// 0xca8
			 0x4e8b666c, /// 0xcac
			 0x9c29e1b4, /// 0xcb0
			 0x13e90d31, /// 0xcb4
			 0x267dcccf, /// 0xcb8
			 0x278bb9ad, /// 0xcbc
			 0x4ce12d1b, /// 0xcc0
			 0xf8ddb144, /// 0xcc4
			 0x0f5dfe57, /// 0xcc8
			 0x2f5d009b, /// 0xccc
			 0x97dfd899, /// 0xcd0
			 0x72abd0b3, /// 0xcd4
			 0x695d4f81, /// 0xcd8
			 0xf801cebd, /// 0xcdc
			 0xc83e1582, /// 0xce0
			 0x993662b3, /// 0xce4
			 0x409ba039, /// 0xce8
			 0x7d46ab6d, /// 0xcec
			 0xc6a305a6, /// 0xcf0
			 0xd7b9fdc4, /// 0xcf4
			 0x40f5f182, /// 0xcf8
			 0x33792001, /// 0xcfc
			 0x0cdb2589, /// 0xd00
			 0x016eb4ad, /// 0xd04
			 0x8dc86a9e, /// 0xd08
			 0x86834459, /// 0xd0c
			 0x819e61f0, /// 0xd10
			 0x5b15fc58, /// 0xd14
			 0x7efb25ff, /// 0xd18
			 0xf638841a, /// 0xd1c
			 0xd286b74d, /// 0xd20
			 0xf9f8525b, /// 0xd24
			 0x84b7efd2, /// 0xd28
			 0x74aea090, /// 0xd2c
			 0x3298c127, /// 0xd30
			 0x73ae20e0, /// 0xd34
			 0x3027558c, /// 0xd38
			 0xb6b3e776, /// 0xd3c
			 0x10e1e016, /// 0xd40
			 0x6bcb1b9b, /// 0xd44
			 0xe9f64916, /// 0xd48
			 0x19c21285, /// 0xd4c
			 0x2dc52110, /// 0xd50
			 0x1b2bbb2e, /// 0xd54
			 0xd7f178d5, /// 0xd58
			 0x87e2ea76, /// 0xd5c
			 0xb3e72730, /// 0xd60
			 0x7a725d99, /// 0xd64
			 0x24b42218, /// 0xd68
			 0x113821a3, /// 0xd6c
			 0x5701a6e7, /// 0xd70
			 0xf79710fe, /// 0xd74
			 0xd6febd4d, /// 0xd78
			 0x56936594, /// 0xd7c
			 0x3e1e4901, /// 0xd80
			 0xc7f7aff9, /// 0xd84
			 0xcb6f931b, /// 0xd88
			 0xe0a2b988, /// 0xd8c
			 0x2b4fcab9, /// 0xd90
			 0x954be3fc, /// 0xd94
			 0x6b8e312b, /// 0xd98
			 0xde198dd7, /// 0xd9c
			 0x6f92594e, /// 0xda0
			 0x71b3bb25, /// 0xda4
			 0x2e17b574, /// 0xda8
			 0xd6651f27, /// 0xdac
			 0xd6baf6d4, /// 0xdb0
			 0xadd9d010, /// 0xdb4
			 0xad054316, /// 0xdb8
			 0x0f9b33da, /// 0xdbc
			 0x8e192294, /// 0xdc0
			 0x3e602cb5, /// 0xdc4
			 0x7e25fa0e, /// 0xdc8
			 0xf2fcb716, /// 0xdcc
			 0xd1404b86, /// 0xdd0
			 0xfcce2cb7, /// 0xdd4
			 0xf2876590, /// 0xdd8
			 0xa6e557a7, /// 0xddc
			 0x5e68030f, /// 0xde0
			 0xcfb2a5c8, /// 0xde4
			 0x4e1ed2c9, /// 0xde8
			 0x38b405c0, /// 0xdec
			 0x585f90c9, /// 0xdf0
			 0x55126f7d, /// 0xdf4
			 0xc79299b9, /// 0xdf8
			 0x850e18de, /// 0xdfc
			 0xf30293e8, /// 0xe00
			 0xc326fa47, /// 0xe04
			 0x318a1ad9, /// 0xe08
			 0xa9599af5, /// 0xe0c
			 0x40f8aa32, /// 0xe10
			 0x291481af, /// 0xe14
			 0x057471bd, /// 0xe18
			 0xe2a998bf, /// 0xe1c
			 0x48f59368, /// 0xe20
			 0x282791aa, /// 0xe24
			 0xd416d455, /// 0xe28
			 0x3414b883, /// 0xe2c
			 0x6b89c7eb, /// 0xe30
			 0xc05d76eb, /// 0xe34
			 0x35e32539, /// 0xe38
			 0xae6298cc, /// 0xe3c
			 0xb915b4c0, /// 0xe40
			 0x98f87db6, /// 0xe44
			 0x14d5a9d7, /// 0xe48
			 0xefa0c202, /// 0xe4c
			 0xdc79a913, /// 0xe50
			 0x624935ae, /// 0xe54
			 0x85278365, /// 0xe58
			 0xf4e8c852, /// 0xe5c
			 0x3b2d1b0a, /// 0xe60
			 0xd2e9f420, /// 0xe64
			 0xeba0bb80, /// 0xe68
			 0x8c42ac7d, /// 0xe6c
			 0x25e86e10, /// 0xe70
			 0x2828405a, /// 0xe74
			 0x474c7bcb, /// 0xe78
			 0x3527f2ae, /// 0xe7c
			 0xbb07a86a, /// 0xe80
			 0x214d6af7, /// 0xe84
			 0xb4c06a28, /// 0xe88
			 0x48cfb3c7, /// 0xe8c
			 0x33e726f5, /// 0xe90
			 0xa16f62b3, /// 0xe94
			 0x67d3eafd, /// 0xe98
			 0xab19cd1d, /// 0xe9c
			 0xe5fd956f, /// 0xea0
			 0x6155c178, /// 0xea4
			 0x3b80dee2, /// 0xea8
			 0x22fcc063, /// 0xeac
			 0x71a994d1, /// 0xeb0
			 0x9361a128, /// 0xeb4
			 0x6b46481b, /// 0xeb8
			 0xb98364ff, /// 0xebc
			 0xb80316ef, /// 0xec0
			 0x9ce63f6a, /// 0xec4
			 0x22fa52ac, /// 0xec8
			 0x773a9708, /// 0xecc
			 0xef6814fc, /// 0xed0
			 0xd6534e5c, /// 0xed4
			 0x59990b89, /// 0xed8
			 0x39726541, /// 0xedc
			 0x974f1d65, /// 0xee0
			 0xeb34e3a8, /// 0xee4
			 0x93f471e6, /// 0xee8
			 0x7a66a9a8, /// 0xeec
			 0x66c17500, /// 0xef0
			 0x44d98be4, /// 0xef4
			 0xcba999aa, /// 0xef8
			 0xd18efaaf, /// 0xefc
			 0x455b2733, /// 0xf00
			 0xb36ebeca, /// 0xf04
			 0x7202ab7a, /// 0xf08
			 0xf2701644, /// 0xf0c
			 0xed7bc83b, /// 0xf10
			 0x65c52b5b, /// 0xf14
			 0xb833e333, /// 0xf18
			 0x5863b766, /// 0xf1c
			 0x77d8e2bf, /// 0xf20
			 0x80c9c70b, /// 0xf24
			 0xe911098e, /// 0xf28
			 0x7947a685, /// 0xf2c
			 0xc0e93156, /// 0xf30
			 0x5f8c1d7c, /// 0xf34
			 0x1af5bde9, /// 0xf38
			 0xb018b99a, /// 0xf3c
			 0x295c9c2c, /// 0xf40
			 0x3f06029d, /// 0xf44
			 0xf8c865b3, /// 0xf48
			 0x062970a0, /// 0xf4c
			 0x3dc2067a, /// 0xf50
			 0xe31842d0, /// 0xf54
			 0xefa5c681, /// 0xf58
			 0xe58ebbb5, /// 0xf5c
			 0xfbb944e3, /// 0xf60
			 0xa753f94f, /// 0xf64
			 0x29d7c1fe, /// 0xf68
			 0xa38978c3, /// 0xf6c
			 0x0fdc3172, /// 0xf70
			 0x8ee29697, /// 0xf74
			 0xb03d945d, /// 0xf78
			 0xa68a948f, /// 0xf7c
			 0xd718df90, /// 0xf80
			 0x70e95baa, /// 0xf84
			 0xf810ab0b, /// 0xf88
			 0xc16381f7, /// 0xf8c
			 0x02b59b1f, /// 0xf90
			 0x9d5323b9, /// 0xf94
			 0x3973c613, /// 0xf98
			 0x919c5c16, /// 0xf9c
			 0xbd603d1d, /// 0xfa0
			 0x848ae38b, /// 0xfa4
			 0xf590d6f5, /// 0xfa8
			 0x9b2e95f4, /// 0xfac
			 0x028e0f0d, /// 0xfb0
			 0x6cc84e11, /// 0xfb4
			 0xe05558e4, /// 0xfb8
			 0xd2af8a68, /// 0xfbc
			 0x5105dc0c, /// 0xfc0
			 0x8f926cc8, /// 0xfc4
			 0x9394998b, /// 0xfc8
			 0x5659af47, /// 0xfcc
			 0xf0ef1f62, /// 0xfd0
			 0xd445d6ed, /// 0xfd4
			 0xdaffdab7, /// 0xfd8
			 0x6caed9e2, /// 0xfdc
			 0xc5ef0ed0, /// 0xfe0
			 0x4be6aa89, /// 0xfe4
			 0xb6597344, /// 0xfe8
			 0x3e0b2488, /// 0xfec
			 0x7ced7371, /// 0xff0
			 0xc7c41576, /// 0xff4
			 0x74dc9837, /// 0xff8
			 0xbb46cc8a /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00000
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00008
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0000c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00018
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0001c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00034
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00038
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0003c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00044
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00054
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00058
			 0x0e000001,                                                  // Trailing 1s:                                /// 0005c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00060
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0006c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00070
			 0x0c600000,                                                  // Leading 1s:                                 /// 00074
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00078
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00080
			 0x0c400000,                                                  // Leading 1s:                                 /// 00084
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00088
			 0x80011111,                                                  // -9.7958E-41                                 /// 0008c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00090
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00094
			 0x0e000001,                                                  // Trailing 1s:                                /// 00098
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000e0
			 0x33333333,                                                  // 4 random values                             /// 000e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000f8
			 0x7f800000,                                                  // inf                                         /// 000fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00100
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00104
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00108
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0010c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00110
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00114
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0011c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00120
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00124
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0012c
			 0xffc00001,                                                  // -signaling NaN                              /// 00130
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00134
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0013c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00144
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00148
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00160
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00164
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0016c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00170
			 0x80011111,                                                  // -9.7958E-41                                 /// 00174
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0017c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00184
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00188
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0018c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00190
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00198
			 0x0e000007,                                                  // Trailing 1s:                                /// 0019c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001a8
			 0x7f800000,                                                  // inf                                         /// 001ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001e4
			 0xff800000,                                                  // -inf                                        /// 001e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00200
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00208
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00210
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00218
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0021c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00220
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00224
			 0x0e000001,                                                  // Trailing 1s:                                /// 00228
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0022c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00230
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00234
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00238
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0023c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00240
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00244
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00248
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00250
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00254
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0025c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00264
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0026c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00270
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00274
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00280
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00284
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00290
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00298
			 0x55555555,                                                  // 4 random values                             /// 0029c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00300
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00304
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00308
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0030c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00310
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00314
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00318
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0031c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00320
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00324
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00328
			 0x0e000007,                                                  // Trailing 1s:                                /// 0032c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00330
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00334
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0033c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00340
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00344
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0034c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00354
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00358
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0035c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00368
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0036c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00370
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00374
			 0x4b000000,                                                  // 8388608.0                                   /// 00378
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0037c
			 0x55555555,                                                  // 4 random values                             /// 00380
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00388
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0038c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00398
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x7fc00001,                                                  // signaling NaN                               /// 003a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003ac
			 0xffc00001,                                                  // -signaling NaN                              /// 003b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80000000,                                                  // -zero                                       /// 00400
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0040c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00414
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00418
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00420
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00424
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00428
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00438
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00444
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0044c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00450
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00011111,                                                  // 9.7958E-41                                  /// 00458
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0045c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00460
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00470
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00478
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00490
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00494
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00498
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004ac
			 0x55555555,                                                  // 4 random values                             /// 004b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004fc
			 0xff800000,                                                  // -inf                                        /// 00500
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00508
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00510
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00514
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0051c
			 0x7f800000,                                                  // inf                                         /// 00520
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00524
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00528
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0052c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00530
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0xcb000000,                                                  // -8388608.0                                  /// 0053c
			 0xffc00001,                                                  // -signaling NaN                              /// 00540
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00548
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00554
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00558
			 0x33333333,                                                  // 4 random values                             /// 0055c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00560
			 0xff800000,                                                  // -inf                                        /// 00564
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00568
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00570
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00574
			 0x7f800000,                                                  // inf                                         /// 00578
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80000000,                                                  // -zero                                       /// 00580
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00590
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00594
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00598
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0059c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0xffc00001,                                                  // -signaling NaN                              /// 005d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00604
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00608
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0060c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00610
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00614
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0061c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00620
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00624
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x33333333,                                                  // 4 random values                             /// 00630
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00638
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0063c
			 0x7fc00001,                                                  // signaling NaN                               /// 00640
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00648
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0064c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00650
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00654
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0065c
			 0xffc00001,                                                  // -signaling NaN                              /// 00660
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00668
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00670
			 0x0c780000,                                                  // Leading 1s:                                 /// 00674
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00678
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0067c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00680
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00690
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00698
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0069c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006a4
			 0xff800000,                                                  // -inf                                        /// 006a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x33333333,                                                  // 4 random values                             /// 006c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006e8
			 0x80000000,                                                  // -zero                                       /// 006ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x7f800000,                                                  // inf                                         /// 006f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00700
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00704
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0070c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00714
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00720
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0072c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00730
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00734
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00738
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00740
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xcb000000,                                                  // -8388608.0                                  /// 00750
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00754
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00768
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00770
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00774
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00778
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0077c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00780
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00784
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00788
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00790
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00798
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0079c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 007a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x0e000003,                                                  // Trailing 1s:                                /// 00808
			 0xbf028f5c,                                                  // -0.51                                       /// 0080c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00810
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00814
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0081c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00820
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00824
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00828
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00830
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00834
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00838
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x00011111,                                                  // 9.7958E-41                                  /// 00844
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x80000000,                                                  // -zero                                       /// 0084c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00854
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00858
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00860
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80000000,                                                  // -zero                                       /// 00868
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0086c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00870
			 0x0e000001,                                                  // Trailing 1s:                                /// 00874
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00878
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00880
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00894
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x0e000001,                                                  // Trailing 1s:                                /// 0089c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008dc
			 0xcb000000,                                                  // -8388608.0                                  /// 008e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0xbf028f5c,                                                  // -0.51                                       /// 008f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008f4
			 0x55555555,                                                  // 4 random values                             /// 008f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00900
			 0x0e000003,                                                  // Trailing 1s:                                /// 00904
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00908
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00910
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0091c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00920
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0092c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0xff800000,                                                  // -inf                                        /// 0093c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00948
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00954
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00958
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0095c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00960
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00964
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00968
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00970
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00974
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0097c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00980
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00984
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0098c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00998
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0099c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009d8
			 0x55555555,                                                  // 4 random values                             /// 009dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0xffc00001,                                                  // -signaling NaN                              /// 009ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a28
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a38
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a50
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a6c
			 0x7f800000,                                                  // inf                                         /// 00a70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a84
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x7f800000,                                                  // inf                                         /// 00a90
			 0x00000000,                                                  // zero                                        /// 00a94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00aa0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aa8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ab0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00abc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ac0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ac4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ac8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00acc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ad8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00adc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ae0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ae4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ae8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00af0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00af4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00af8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00afc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b50
			 0x4b000000,                                                  // 8388608.0                                   /// 00b54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b74
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b78
			 0xff800000,                                                  // -inf                                        /// 00b7c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b80
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x33333333,                                                  // 4 random values                             /// 00ba4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ba8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bbc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bcc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bd0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bd4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bdc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00be0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bf0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bfc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0xff800000,                                                  // -inf                                        /// 00c0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c10
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c24
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x00000000,                                                  // zero                                        /// 00c30
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c34
			 0x80000000,                                                  // -zero                                       /// 00c38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c40
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c4c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c5c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c70
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c78
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c7c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c90
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ca4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ca8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cb0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cb4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cb8
			 0x33333333,                                                  // 4 random values                             /// 00cbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cc4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cc8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ccc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cd0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ce0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ce4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ce8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cf0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cf8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cfc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d30
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0xff800000,                                                  // -inf                                        /// 00d40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x33333333,                                                  // 4 random values                             /// 00d48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d54
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d70
			 0x33333333,                                                  // 4 random values                             /// 00d74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00da0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00da4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00da8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00db0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00db8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dc8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dd4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dd8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00de0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00de4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00df4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dfc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e60
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00ea0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ea8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eb4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ec0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ec4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ecc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ed0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ed4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ed8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00edc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ee0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ee4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ee8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ef4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ef8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00efc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f0c
			 0x00000000,                                                  // zero                                        /// 00f10
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x7f800000,                                                  // inf                                         /// 00f34
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fa0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fb4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fc0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fcc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fd8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fdc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff8
			 0x0c7ffe00                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00000
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00004
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00008
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0000c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00014
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00018
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00020
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00024
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00038
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0003c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00040
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00044
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0004c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00058
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0005c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00060
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00064
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00068
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0006c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00070
			 0x0e000003,                                                  // Trailing 1s:                                /// 00074
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00078
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0007c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00080
			 0x0c700000,                                                  // Leading 1s:                                 /// 00084
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00088
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0008c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00090
			 0x00011111,                                                  // 9.7958E-41                                  /// 00094
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00098
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0009c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00100
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00108
			 0x0e000001,                                                  // Trailing 1s:                                /// 0010c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00110
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00118
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0011c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00120
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00124
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00128
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00130
			 0x00000000,                                                  // zero                                        /// 00134
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00138
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0013c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00140
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00144
			 0x7f800000,                                                  // inf                                         /// 00148
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0014c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00154
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0015c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00160
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00164
			 0x0e000003,                                                  // Trailing 1s:                                /// 00168
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00170
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00174
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00178
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80000000,                                                  // -zero                                       /// 00180
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00184
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00188
			 0x0c400000,                                                  // Leading 1s:                                 /// 0018c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00190
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00194
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00198
			 0x80011111,                                                  // -9.7958E-41                                 /// 0019c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00200
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00208
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0020c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00218
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0021c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00220
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00224
			 0x0c700000,                                                  // Leading 1s:                                 /// 00228
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0022c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00230
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00234
			 0x7fc00001,                                                  // signaling NaN                               /// 00238
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0023c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00248
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0024c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00250
			 0x0c400000,                                                  // Leading 1s:                                 /// 00254
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00258
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00260
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0026c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x00000000,                                                  // zero                                        /// 00274
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00280
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00288
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0028c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00290
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00298
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x55555555,                                                  // 4 random values                             /// 002a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 002b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x3f028f5c,                                                  // 0.51                                        /// 002d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002dc
			 0xff800000,                                                  // -inf                                        /// 002e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002fc
			 0x7f800000,                                                  // inf                                         /// 00300
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00304
			 0x00011111,                                                  // 9.7958E-41                                  /// 00308
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00310
			 0x0c780000,                                                  // Leading 1s:                                 /// 00314
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00318
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0031c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00324
			 0x0e000001,                                                  // Trailing 1s:                                /// 00328
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0032c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00330
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00334
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0033c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00344
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00348
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0034c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0xff800000,                                                  // -inf                                        /// 00358
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00360
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0036c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00374
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00378
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00380
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0038c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00390
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00394
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0039c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 003dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 003e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003e4
			 0x4b000000,                                                  // 8388608.0                                   /// 003e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x00000000,                                                  // zero                                        /// 003f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003f8
			 0x7fc00001,                                                  // signaling NaN                               /// 003fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00400
			 0x0e000003,                                                  // Trailing 1s:                                /// 00404
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00408
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0040c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00410
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00414
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0041c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x7f800000,                                                  // inf                                         /// 00424
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00428
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0042c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00430
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0c600000,                                                  // Leading 1s:                                 /// 00438
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00444
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00448
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0044c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00450
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00454
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00458
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00464
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00470
			 0xcb000000,                                                  // -8388608.0                                  /// 00474
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0047c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00480
			 0xffc00001,                                                  // -signaling NaN                              /// 00484
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0x80000000,                                                  // -zero                                       /// 00490
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00494
			 0x33333333,                                                  // 4 random values                             /// 00498
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0049c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004d0
			 0x7fc00001,                                                  // signaling NaN                               /// 004d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00500
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00504
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00508
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0050c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00510
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00518
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00530
			 0x55555555,                                                  // 4 random values                             /// 00534
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00538
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0053c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0054c
			 0x7f800000,                                                  // inf                                         /// 00550
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00558
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0055c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00560
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00564
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00568
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0056c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00570
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0057c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00588
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00590
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00598
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00600
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00604
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0060c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00614
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00618
			 0x33333333,                                                  // 4 random values                             /// 0061c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00620
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00624
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00628
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0062c
			 0x80000000,                                                  // -zero                                       /// 00630
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00634
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0063c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00640
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0064c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00650
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00654
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00658
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0065c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00660
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00664
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00668
			 0x0c780000,                                                  // Leading 1s:                                 /// 0066c
			 0x4b000000,                                                  // 8388608.0                                   /// 00670
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00674
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0067c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00680
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00688
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00694
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00698
			 0x0e000003,                                                  // Trailing 1s:                                /// 0069c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006bc
			 0x80000000,                                                  // -zero                                       /// 006c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 006e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00700
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00708
			 0x00000000,                                                  // zero                                        /// 0070c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00710
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00714
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0071c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00720
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00724
			 0x4b000000,                                                  // 8388608.0                                   /// 00728
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0073c
			 0x7fc00001,                                                  // signaling NaN                               /// 00740
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00744
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00748
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00750
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0075c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00760
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00764
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0076c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00770
			 0x0c600000,                                                  // Leading 1s:                                 /// 00774
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0077c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00780
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x00011111,                                                  // 9.7958E-41                                  /// 00788
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0078c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00794
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0e000001,                                                  // Trailing 1s:                                /// 0079c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c0
			 0xff800000,                                                  // -inf                                        /// 007c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x3f028f5c,                                                  // 0.51                                        /// 007e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00800
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00804
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00808
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0080c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00810
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00814
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00818
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0081c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00820
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00828
			 0x80011111,                                                  // -9.7958E-41                                 /// 0082c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00844
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00848
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00850
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00858
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0085c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00860
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00864
			 0x00011111,                                                  // 9.7958E-41                                  /// 00868
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00870
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00874
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00878
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0087c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0088c
			 0xffc00001,                                                  // -signaling NaN                              /// 00890
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00894
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x3f028f5c,                                                  // 0.51                                        /// 008a0
			 0xbf028f5c,                                                  // -0.51                                       /// 008a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0xcb000000,                                                  // -8388608.0                                  /// 008e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00900
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00908
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0090c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00910
			 0x0c780000,                                                  // Leading 1s:                                 /// 00914
			 0x00011111,                                                  // 9.7958E-41                                  /// 00918
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0091c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00920
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00928
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0092c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00930
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00938
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00948
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0094c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00950
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0xff800000,                                                  // -inf                                        /// 0095c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00960
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00968
			 0x0c780000,                                                  // Leading 1s:                                 /// 0096c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00970
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0097c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00980
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00988
			 0x80000000,                                                  // -zero                                       /// 0098c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00990
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00994
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00998
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b0
			 0x7fc00001,                                                  // signaling NaN                               /// 009b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009d4
			 0x33333333,                                                  // 4 random values                             /// 009d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a34
			 0x7fc00001,                                                  // signaling NaN                               /// 00a38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a40
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a58
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a60
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x33333333,                                                  // 4 random values                             /// 00a9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00aa4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ab4
			 0xff800000,                                                  // -inf                                        /// 00ab8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00abc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ac0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x80000000,                                                  // -zero                                       /// 00ac8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ad4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00adc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ae0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ae4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ae8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00af8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00afc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b08
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b10
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b14
			 0x3f028f5c,                                                  // 0.51                                        /// 00b18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b20
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b24
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b40
			 0x80000000,                                                  // -zero                                       /// 00b44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b48
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b4c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80000000,                                                  // -zero                                       /// 00b74
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b84
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bb0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bb4
			 0x80000000,                                                  // -zero                                       /// 00bb8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bcc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00be0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00be8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bf0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bf4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bfc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c18
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c1c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0xff800000,                                                  // -inf                                        /// 00c28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c40
			 0x7f800000,                                                  // inf                                         /// 00c44
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c5c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c68
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c74
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c84
			 0xffc00001,                                                  // -signaling NaN                              /// 00c88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c8c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c90
			 0x33333333,                                                  // 4 random values                             /// 00c94
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ca0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ca4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ca8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cb0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cb4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ccc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cd8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cdc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cf0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cf4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cf8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d04
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d38
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d3c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d60
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d7c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d94
			 0xffc00001,                                                  // -signaling NaN                              /// 00d98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00da0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00db0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00db8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dbc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dc0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dc8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dd4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ddc
			 0x00000000,                                                  // zero                                        /// 00de0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00de8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00df0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00df8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dfc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x3f028f5c,                                                  // 0.51                                        /// 00e0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e14
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e18
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e20
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x4b000000,                                                  // 8388608.0                                   /// 00e58
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ea0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ea8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00eac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ebc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ec4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ecc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ed8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ee8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ef0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ef8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f2c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f30
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fa0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fc0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fcc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fd0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fdc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ff8
			 0x0c7ffff8                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xc2cc273d, /// 0x0
			 0x631a295b, /// 0x4
			 0x7db85df0, /// 0x8
			 0x980a893f, /// 0xc
			 0xde207a93, /// 0x10
			 0x1c0d0635, /// 0x14
			 0x88462ef1, /// 0x18
			 0x819897bc, /// 0x1c
			 0x1f7d8c86, /// 0x20
			 0xdd0137f3, /// 0x24
			 0xcff65fe8, /// 0x28
			 0xda094a86, /// 0x2c
			 0xe5df0f0c, /// 0x30
			 0x0bdd3b48, /// 0x34
			 0x540c30b4, /// 0x38
			 0x2f797120, /// 0x3c
			 0xab4138cc, /// 0x40
			 0xa14a3652, /// 0x44
			 0x636f8ff8, /// 0x48
			 0x413c6f27, /// 0x4c
			 0x8aca4534, /// 0x50
			 0x0e383ab9, /// 0x54
			 0x3d0c213c, /// 0x58
			 0x973894e9, /// 0x5c
			 0xf22c9119, /// 0x60
			 0x2886d4f3, /// 0x64
			 0xa32edb8c, /// 0x68
			 0x260e38fb, /// 0x6c
			 0x6b5eed53, /// 0x70
			 0x4a1e593d, /// 0x74
			 0xa6a24aac, /// 0x78
			 0x5c1f7ab2, /// 0x7c
			 0xca5e9fe3, /// 0x80
			 0xc9502adf, /// 0x84
			 0x90368983, /// 0x88
			 0x9dd7387e, /// 0x8c
			 0x0129c5a8, /// 0x90
			 0x486c342b, /// 0x94
			 0x8611415b, /// 0x98
			 0xf1adcc1f, /// 0x9c
			 0xf0fe0d54, /// 0xa0
			 0x53e78057, /// 0xa4
			 0xd29a2612, /// 0xa8
			 0x9f634f84, /// 0xac
			 0xadbfccb6, /// 0xb0
			 0x0268b717, /// 0xb4
			 0x3c5727a8, /// 0xb8
			 0x17d09781, /// 0xbc
			 0xea551700, /// 0xc0
			 0xd5b6ede1, /// 0xc4
			 0x37154665, /// 0xc8
			 0x9a7550be, /// 0xcc
			 0xc1aaf94a, /// 0xd0
			 0xe1a658df, /// 0xd4
			 0x0601300f, /// 0xd8
			 0xc25d2e72, /// 0xdc
			 0x133bdfc6, /// 0xe0
			 0x6057da0b, /// 0xe4
			 0x428ae4f6, /// 0xe8
			 0xea7025fe, /// 0xec
			 0x7e71553e, /// 0xf0
			 0x8d24fe7e, /// 0xf4
			 0x38000474, /// 0xf8
			 0x201cbc8a, /// 0xfc
			 0x3ed624ed, /// 0x100
			 0xacbbc5bb, /// 0x104
			 0x3a66eb59, /// 0x108
			 0x38384c18, /// 0x10c
			 0x453dd112, /// 0x110
			 0xfc406d17, /// 0x114
			 0x7b2732ab, /// 0x118
			 0xd910d2dd, /// 0x11c
			 0x273478c6, /// 0x120
			 0x33f9935f, /// 0x124
			 0x14365e89, /// 0x128
			 0x57ee52bf, /// 0x12c
			 0x811dab34, /// 0x130
			 0x688e3a92, /// 0x134
			 0xc9e3dc3c, /// 0x138
			 0xcbb478d9, /// 0x13c
			 0x4f25f724, /// 0x140
			 0x93116fba, /// 0x144
			 0x0aa68251, /// 0x148
			 0xabcccbd7, /// 0x14c
			 0xe48115da, /// 0x150
			 0x35eec4eb, /// 0x154
			 0x962c49dc, /// 0x158
			 0x3672342d, /// 0x15c
			 0xaf5b3e72, /// 0x160
			 0x7d973e27, /// 0x164
			 0x87b5e83d, /// 0x168
			 0x810a164d, /// 0x16c
			 0xcc42216d, /// 0x170
			 0x3e3294db, /// 0x174
			 0x82294111, /// 0x178
			 0xd5aa47f8, /// 0x17c
			 0x31f49755, /// 0x180
			 0xd29b5424, /// 0x184
			 0xdb933737, /// 0x188
			 0x263b524d, /// 0x18c
			 0x03263aa5, /// 0x190
			 0x6beb36e4, /// 0x194
			 0x775093cd, /// 0x198
			 0x284fd360, /// 0x19c
			 0xc86c9657, /// 0x1a0
			 0xb107308f, /// 0x1a4
			 0xfe02f7d1, /// 0x1a8
			 0x45974e1a, /// 0x1ac
			 0x36cf4a5f, /// 0x1b0
			 0xe1c2cb62, /// 0x1b4
			 0x2790fef9, /// 0x1b8
			 0xf9f7cdef, /// 0x1bc
			 0xf511e0ab, /// 0x1c0
			 0x267bffb9, /// 0x1c4
			 0xee7d2d7b, /// 0x1c8
			 0xbe8e834d, /// 0x1cc
			 0x2a10a631, /// 0x1d0
			 0x1eaae888, /// 0x1d4
			 0xc590ca77, /// 0x1d8
			 0x57048331, /// 0x1dc
			 0xd0eed80b, /// 0x1e0
			 0x05a597b6, /// 0x1e4
			 0x8cc3673d, /// 0x1e8
			 0x137bf669, /// 0x1ec
			 0x518e406f, /// 0x1f0
			 0x30730572, /// 0x1f4
			 0xdc80e638, /// 0x1f8
			 0x90d246b1, /// 0x1fc
			 0x72c50520, /// 0x200
			 0x805fff67, /// 0x204
			 0xfa5e1c39, /// 0x208
			 0x21f25ef8, /// 0x20c
			 0x3118ab7e, /// 0x210
			 0x7a9c1b77, /// 0x214
			 0x09c6bcb7, /// 0x218
			 0xdc81e507, /// 0x21c
			 0x0bedde05, /// 0x220
			 0xb4d1abef, /// 0x224
			 0x7df92a8b, /// 0x228
			 0xa1348031, /// 0x22c
			 0xd34aef3c, /// 0x230
			 0xa87c5297, /// 0x234
			 0x65bb1471, /// 0x238
			 0x0dbb10d2, /// 0x23c
			 0x304edca0, /// 0x240
			 0x7822ed94, /// 0x244
			 0x6940aee0, /// 0x248
			 0xd3b99562, /// 0x24c
			 0x197d7f13, /// 0x250
			 0x2a06817b, /// 0x254
			 0xde07b659, /// 0x258
			 0x9659557f, /// 0x25c
			 0xb940889a, /// 0x260
			 0xbd2c19f6, /// 0x264
			 0x5edeec90, /// 0x268
			 0xabffcceb, /// 0x26c
			 0x35bcfff2, /// 0x270
			 0x92c226ff, /// 0x274
			 0x9bc28349, /// 0x278
			 0x20586b54, /// 0x27c
			 0x488cc768, /// 0x280
			 0xdf16f915, /// 0x284
			 0x6b43e312, /// 0x288
			 0xe5ccc68c, /// 0x28c
			 0x6c0a94f3, /// 0x290
			 0x34c3f886, /// 0x294
			 0x7a765b6c, /// 0x298
			 0x26d14033, /// 0x29c
			 0x0fe6817f, /// 0x2a0
			 0xa857bb88, /// 0x2a4
			 0xcb2993af, /// 0x2a8
			 0x9c091e15, /// 0x2ac
			 0x396f0215, /// 0x2b0
			 0x29067d37, /// 0x2b4
			 0x3bf10ffb, /// 0x2b8
			 0xd0ac10c8, /// 0x2bc
			 0x361890c8, /// 0x2c0
			 0xbe996145, /// 0x2c4
			 0xe066654b, /// 0x2c8
			 0xa55c0d98, /// 0x2cc
			 0x95ab2d08, /// 0x2d0
			 0x11999566, /// 0x2d4
			 0x17777df0, /// 0x2d8
			 0xbaae0e9c, /// 0x2dc
			 0x807c57ea, /// 0x2e0
			 0xec9356a0, /// 0x2e4
			 0x54129c01, /// 0x2e8
			 0x3fe95741, /// 0x2ec
			 0xd8035484, /// 0x2f0
			 0x1da46cb4, /// 0x2f4
			 0x3a78283a, /// 0x2f8
			 0xa70a5c40, /// 0x2fc
			 0x4b8cfdcc, /// 0x300
			 0x97c7bd61, /// 0x304
			 0x8960d014, /// 0x308
			 0xa7639f42, /// 0x30c
			 0x3535cf9a, /// 0x310
			 0xe71be925, /// 0x314
			 0x29b05267, /// 0x318
			 0xb52bf6c0, /// 0x31c
			 0x2ad1865d, /// 0x320
			 0xad5d41de, /// 0x324
			 0x8e609be3, /// 0x328
			 0xf9dc9f8b, /// 0x32c
			 0x9979bc1c, /// 0x330
			 0x2dcba2f0, /// 0x334
			 0xeb21f1f9, /// 0x338
			 0x872f8ce3, /// 0x33c
			 0x5ddefae4, /// 0x340
			 0x337b9a65, /// 0x344
			 0x0c9448d6, /// 0x348
			 0xa3c19eab, /// 0x34c
			 0xb4d8a21d, /// 0x350
			 0x50aeb64b, /// 0x354
			 0x7d2f7414, /// 0x358
			 0x2f0ff8ba, /// 0x35c
			 0x1164d791, /// 0x360
			 0x4a4a0fca, /// 0x364
			 0xe35f588f, /// 0x368
			 0xd2f76802, /// 0x36c
			 0xde95c303, /// 0x370
			 0xd94f84e7, /// 0x374
			 0x7bfc34c6, /// 0x378
			 0xda365350, /// 0x37c
			 0xf91f09e8, /// 0x380
			 0x0e076b9c, /// 0x384
			 0xc6069446, /// 0x388
			 0x7cbf1e78, /// 0x38c
			 0xb24aaa90, /// 0x390
			 0x79ddc0e9, /// 0x394
			 0x7657db82, /// 0x398
			 0xccf3e018, /// 0x39c
			 0x1eda0e1b, /// 0x3a0
			 0x713122aa, /// 0x3a4
			 0xfdf7d4ca, /// 0x3a8
			 0x15776810, /// 0x3ac
			 0x278a88fa, /// 0x3b0
			 0x51ab94c2, /// 0x3b4
			 0xaf93bbcc, /// 0x3b8
			 0x8f532d5a, /// 0x3bc
			 0x92c9ede5, /// 0x3c0
			 0x36a5c4ba, /// 0x3c4
			 0x22564298, /// 0x3c8
			 0xf5d4be6a, /// 0x3cc
			 0x31e0e563, /// 0x3d0
			 0x69e6d504, /// 0x3d4
			 0xab9e6db3, /// 0x3d8
			 0xad9eee0a, /// 0x3dc
			 0xd15e8c56, /// 0x3e0
			 0xddc9c98c, /// 0x3e4
			 0x032de1cc, /// 0x3e8
			 0xdc54acef, /// 0x3ec
			 0x913b4df3, /// 0x3f0
			 0x888a3e98, /// 0x3f4
			 0x73d0621e, /// 0x3f8
			 0x97f915be, /// 0x3fc
			 0x445cfe4d, /// 0x400
			 0x73c38be2, /// 0x404
			 0x877a4e1e, /// 0x408
			 0x0a3f5d77, /// 0x40c
			 0xca7c0803, /// 0x410
			 0x04d71095, /// 0x414
			 0x8b1a53ff, /// 0x418
			 0x533bf768, /// 0x41c
			 0xffc3fe33, /// 0x420
			 0x850d5b6d, /// 0x424
			 0xd94fe71d, /// 0x428
			 0x328c4fb1, /// 0x42c
			 0x99afb244, /// 0x430
			 0x641e6549, /// 0x434
			 0x8a609737, /// 0x438
			 0x26e12fde, /// 0x43c
			 0x0c5e90e8, /// 0x440
			 0x5a908d3f, /// 0x444
			 0x42a9dfdb, /// 0x448
			 0xd49300f9, /// 0x44c
			 0xe87afe54, /// 0x450
			 0xc7321907, /// 0x454
			 0x358c30d6, /// 0x458
			 0xbe1113f8, /// 0x45c
			 0xbc056e89, /// 0x460
			 0x178761ca, /// 0x464
			 0x1d9f96e4, /// 0x468
			 0x5dcf3ed4, /// 0x46c
			 0xd13c9b6e, /// 0x470
			 0xb60b4bbb, /// 0x474
			 0x0f8d004d, /// 0x478
			 0x9f9c4714, /// 0x47c
			 0xd13ccd17, /// 0x480
			 0x9626b2a4, /// 0x484
			 0xe51e641a, /// 0x488
			 0x2329e9f7, /// 0x48c
			 0x8961dc60, /// 0x490
			 0x222ccd23, /// 0x494
			 0x258385de, /// 0x498
			 0x847a7d29, /// 0x49c
			 0x95a7990b, /// 0x4a0
			 0x898f00b6, /// 0x4a4
			 0x7a355985, /// 0x4a8
			 0xbd7adff0, /// 0x4ac
			 0x18481f39, /// 0x4b0
			 0x0df56122, /// 0x4b4
			 0xc6d7ae68, /// 0x4b8
			 0x6be50a0c, /// 0x4bc
			 0x6e18f25c, /// 0x4c0
			 0x34390664, /// 0x4c4
			 0x8df05f86, /// 0x4c8
			 0x99b54083, /// 0x4cc
			 0x26857388, /// 0x4d0
			 0x13bd5857, /// 0x4d4
			 0xb6e6dd59, /// 0x4d8
			 0xe2946411, /// 0x4dc
			 0xb2282f34, /// 0x4e0
			 0x36bb20ab, /// 0x4e4
			 0x66a2e35c, /// 0x4e8
			 0x20e12d4e, /// 0x4ec
			 0xf87c20c8, /// 0x4f0
			 0x334ca55e, /// 0x4f4
			 0x75cafd4a, /// 0x4f8
			 0x03cf4fa7, /// 0x4fc
			 0x3b572da7, /// 0x500
			 0x45101f4c, /// 0x504
			 0x1bddd890, /// 0x508
			 0x2ec5e29a, /// 0x50c
			 0xae2737e2, /// 0x510
			 0x2d46c8be, /// 0x514
			 0xe2305470, /// 0x518
			 0x5047c029, /// 0x51c
			 0xaaa188b5, /// 0x520
			 0x4f8c8d38, /// 0x524
			 0xcc281b17, /// 0x528
			 0x7dacba7b, /// 0x52c
			 0xe744d119, /// 0x530
			 0x7d315f5c, /// 0x534
			 0x14f51eea, /// 0x538
			 0x4542dfda, /// 0x53c
			 0x352d5bc8, /// 0x540
			 0x81ef0703, /// 0x544
			 0xe701cea3, /// 0x548
			 0x1ffa6dec, /// 0x54c
			 0xbfb528df, /// 0x550
			 0x8566c62f, /// 0x554
			 0xdc8feb3f, /// 0x558
			 0x545aba81, /// 0x55c
			 0x8a1ef9e7, /// 0x560
			 0xae34530a, /// 0x564
			 0xabce4ba3, /// 0x568
			 0x0c65518e, /// 0x56c
			 0x2df55d25, /// 0x570
			 0xd7425fef, /// 0x574
			 0x7b29e822, /// 0x578
			 0x04a44dbd, /// 0x57c
			 0xb2fddec3, /// 0x580
			 0x0e96a63d, /// 0x584
			 0x7d20ab5b, /// 0x588
			 0xc0ca4a18, /// 0x58c
			 0xa9904666, /// 0x590
			 0xa554a382, /// 0x594
			 0x6b5849ed, /// 0x598
			 0x19c643f2, /// 0x59c
			 0xa88d2fd8, /// 0x5a0
			 0x89e31122, /// 0x5a4
			 0x61c5d51e, /// 0x5a8
			 0x966aa986, /// 0x5ac
			 0xb2f2a8ae, /// 0x5b0
			 0xe3e352e1, /// 0x5b4
			 0x65a81e8d, /// 0x5b8
			 0xc4c17eb1, /// 0x5bc
			 0x648fa100, /// 0x5c0
			 0x60a251ba, /// 0x5c4
			 0xf030c16f, /// 0x5c8
			 0x7cbbbc35, /// 0x5cc
			 0x5d27e228, /// 0x5d0
			 0xcfadc4b7, /// 0x5d4
			 0x89caa924, /// 0x5d8
			 0xf08c00fd, /// 0x5dc
			 0x449b97ac, /// 0x5e0
			 0x85051f9f, /// 0x5e4
			 0x3db910b1, /// 0x5e8
			 0x428eb335, /// 0x5ec
			 0x24006efc, /// 0x5f0
			 0xcf0a35ac, /// 0x5f4
			 0xd4fbd405, /// 0x5f8
			 0x9dbcb05e, /// 0x5fc
			 0x6839cbdd, /// 0x600
			 0x61155d1f, /// 0x604
			 0xe0fd5b04, /// 0x608
			 0x05aa4100, /// 0x60c
			 0x25d1fa6b, /// 0x610
			 0xde4e1e41, /// 0x614
			 0x55f93999, /// 0x618
			 0x5b15454a, /// 0x61c
			 0xdb4ad4de, /// 0x620
			 0x07305c8b, /// 0x624
			 0xb72faba3, /// 0x628
			 0xc3507e29, /// 0x62c
			 0x00075bb6, /// 0x630
			 0x8f451d7f, /// 0x634
			 0x78a4ec8a, /// 0x638
			 0x2ce59ba3, /// 0x63c
			 0xb3efbe8f, /// 0x640
			 0xdd046ada, /// 0x644
			 0x65180f97, /// 0x648
			 0x53d39d1b, /// 0x64c
			 0x753f1263, /// 0x650
			 0x549ad916, /// 0x654
			 0x08e1902d, /// 0x658
			 0xffd170a4, /// 0x65c
			 0x9fcc56a4, /// 0x660
			 0x7fc2b1b6, /// 0x664
			 0x194adbae, /// 0x668
			 0x069e5318, /// 0x66c
			 0x9c8897d7, /// 0x670
			 0x11a35560, /// 0x674
			 0x52a20e0c, /// 0x678
			 0x55b82c84, /// 0x67c
			 0x2f5809bf, /// 0x680
			 0x5fcd458a, /// 0x684
			 0xf894fee4, /// 0x688
			 0x1c8dffb0, /// 0x68c
			 0xeddbd095, /// 0x690
			 0x419729a0, /// 0x694
			 0xe7a96793, /// 0x698
			 0xa6d848af, /// 0x69c
			 0xbe2247af, /// 0x6a0
			 0x63edcf7a, /// 0x6a4
			 0x395562a5, /// 0x6a8
			 0x9590e719, /// 0x6ac
			 0x9315d443, /// 0x6b0
			 0x79f79324, /// 0x6b4
			 0x65cfad02, /// 0x6b8
			 0xc8725e74, /// 0x6bc
			 0x7b06d54f, /// 0x6c0
			 0xa1c56a1b, /// 0x6c4
			 0x5e6d3c4d, /// 0x6c8
			 0x4311d815, /// 0x6cc
			 0xf581fd19, /// 0x6d0
			 0xc8ee097a, /// 0x6d4
			 0x11b419de, /// 0x6d8
			 0x50a82c02, /// 0x6dc
			 0x8679e5a2, /// 0x6e0
			 0xa68ea2ea, /// 0x6e4
			 0x98d76db5, /// 0x6e8
			 0x61f45283, /// 0x6ec
			 0x0dcb6491, /// 0x6f0
			 0x89ed5b87, /// 0x6f4
			 0xc8f5ba21, /// 0x6f8
			 0x4adc1032, /// 0x6fc
			 0xb8c5c814, /// 0x700
			 0x350eae3b, /// 0x704
			 0x5e86d6f5, /// 0x708
			 0xe3b48d9c, /// 0x70c
			 0x75f47303, /// 0x710
			 0xa5b13cea, /// 0x714
			 0x38568a09, /// 0x718
			 0x1db79565, /// 0x71c
			 0x36f41a71, /// 0x720
			 0x5ddac4a1, /// 0x724
			 0xf0a25f2e, /// 0x728
			 0x4849f957, /// 0x72c
			 0x41c76395, /// 0x730
			 0x14f6aa1f, /// 0x734
			 0xf0357bc5, /// 0x738
			 0x057dd366, /// 0x73c
			 0x3ac66262, /// 0x740
			 0x9f756369, /// 0x744
			 0x84862ae1, /// 0x748
			 0xf49117fc, /// 0x74c
			 0xca089643, /// 0x750
			 0x8be1e41c, /// 0x754
			 0x2c1e5c8a, /// 0x758
			 0x09ac2b4f, /// 0x75c
			 0x9e69db63, /// 0x760
			 0x460dda6b, /// 0x764
			 0x6f22b1b4, /// 0x768
			 0x3eec5c23, /// 0x76c
			 0x944f8067, /// 0x770
			 0xf862c650, /// 0x774
			 0x21785920, /// 0x778
			 0x61d17173, /// 0x77c
			 0x38d9bda8, /// 0x780
			 0x9fc4bfbd, /// 0x784
			 0xa5ff6618, /// 0x788
			 0x19eac510, /// 0x78c
			 0x0cc261ed, /// 0x790
			 0x8e62221b, /// 0x794
			 0xc82f8a7b, /// 0x798
			 0x73f09b94, /// 0x79c
			 0x13af4a61, /// 0x7a0
			 0xf7bf4a82, /// 0x7a4
			 0xeaca4018, /// 0x7a8
			 0xa80569ce, /// 0x7ac
			 0x42d9f7f3, /// 0x7b0
			 0x9407ebfb, /// 0x7b4
			 0x1d786e37, /// 0x7b8
			 0x8745429e, /// 0x7bc
			 0x26e7f9d1, /// 0x7c0
			 0x5b26c5ac, /// 0x7c4
			 0xbc93aaa4, /// 0x7c8
			 0x943868e9, /// 0x7cc
			 0xadbc2e42, /// 0x7d0
			 0x17bbe8ad, /// 0x7d4
			 0xa6092ad4, /// 0x7d8
			 0xafe4f590, /// 0x7dc
			 0xd620c8a7, /// 0x7e0
			 0x25785ec7, /// 0x7e4
			 0x45047598, /// 0x7e8
			 0x5671f389, /// 0x7ec
			 0x6c5b8822, /// 0x7f0
			 0x631ae096, /// 0x7f4
			 0x2edce451, /// 0x7f8
			 0x805fefa1, /// 0x7fc
			 0xf5121668, /// 0x800
			 0xcafb3928, /// 0x804
			 0x866b859b, /// 0x808
			 0xf285b885, /// 0x80c
			 0x1baf7803, /// 0x810
			 0x5fb719b9, /// 0x814
			 0x30c23239, /// 0x818
			 0xc04789b4, /// 0x81c
			 0x32ff4a1f, /// 0x820
			 0x664f2bfc, /// 0x824
			 0x28c12d1f, /// 0x828
			 0x2e35db23, /// 0x82c
			 0x801bf4ff, /// 0x830
			 0xdc709c71, /// 0x834
			 0xe000bf4b, /// 0x838
			 0x77069017, /// 0x83c
			 0x3d86ed4d, /// 0x840
			 0x5d70684f, /// 0x844
			 0x2422ab23, /// 0x848
			 0xb03d04a5, /// 0x84c
			 0xb3463d7e, /// 0x850
			 0x2b4068bf, /// 0x854
			 0xbbed2ee5, /// 0x858
			 0xc2dabfb5, /// 0x85c
			 0x6e330fd6, /// 0x860
			 0x7d7f8c50, /// 0x864
			 0x5e35f6a3, /// 0x868
			 0x56f7ebd8, /// 0x86c
			 0xf4f54a1b, /// 0x870
			 0x5333caa7, /// 0x874
			 0x918e2bcf, /// 0x878
			 0x6ff7f993, /// 0x87c
			 0x792a9af5, /// 0x880
			 0x9944fab1, /// 0x884
			 0x8f1e482f, /// 0x888
			 0x606ab321, /// 0x88c
			 0xd5a7e938, /// 0x890
			 0xaad0675c, /// 0x894
			 0x6218c7e2, /// 0x898
			 0xd52e22dc, /// 0x89c
			 0x8e749524, /// 0x8a0
			 0x28083b5a, /// 0x8a4
			 0xf053310a, /// 0x8a8
			 0xfe963285, /// 0x8ac
			 0x2d9ec546, /// 0x8b0
			 0x1733ad47, /// 0x8b4
			 0x2ecd87f6, /// 0x8b8
			 0x5e91166e, /// 0x8bc
			 0x1ca57603, /// 0x8c0
			 0x4ca9d6a0, /// 0x8c4
			 0x55b3d282, /// 0x8c8
			 0xbd31d01c, /// 0x8cc
			 0xcf7ff073, /// 0x8d0
			 0x369b487c, /// 0x8d4
			 0x13581150, /// 0x8d8
			 0x5132678e, /// 0x8dc
			 0x0819db74, /// 0x8e0
			 0xdfdfbab1, /// 0x8e4
			 0x78b68b8a, /// 0x8e8
			 0xfdea0799, /// 0x8ec
			 0xd1bc2ecc, /// 0x8f0
			 0xb34f2d40, /// 0x8f4
			 0x03468993, /// 0x8f8
			 0xe43643d2, /// 0x8fc
			 0x8bda2fc4, /// 0x900
			 0x569389b2, /// 0x904
			 0xfb23f026, /// 0x908
			 0xed08bd2f, /// 0x90c
			 0xb87b9f93, /// 0x910
			 0x2d9b0ace, /// 0x914
			 0xbfe3629c, /// 0x918
			 0x099b0123, /// 0x91c
			 0x34eab2e7, /// 0x920
			 0xd42c436b, /// 0x924
			 0x507c3ad9, /// 0x928
			 0x9a32bcdc, /// 0x92c
			 0x068eda74, /// 0x930
			 0xa4cab49d, /// 0x934
			 0x59cb5e9e, /// 0x938
			 0x836bec39, /// 0x93c
			 0xad029622, /// 0x940
			 0xe6de88ed, /// 0x944
			 0x8a87d16b, /// 0x948
			 0x83b383e6, /// 0x94c
			 0x6d0d3468, /// 0x950
			 0xa934ba72, /// 0x954
			 0xacf4fdd7, /// 0x958
			 0xbc6b04b4, /// 0x95c
			 0x8fcea0b1, /// 0x960
			 0xb9eb6a92, /// 0x964
			 0xff3e2376, /// 0x968
			 0x3c5c606d, /// 0x96c
			 0xc4253cbe, /// 0x970
			 0x66bf7035, /// 0x974
			 0x1a7051fb, /// 0x978
			 0x02e816da, /// 0x97c
			 0xd2bf7efc, /// 0x980
			 0xb5ddb55a, /// 0x984
			 0x274c2fe7, /// 0x988
			 0x857e0068, /// 0x98c
			 0x1571a4ab, /// 0x990
			 0xf7ad0e27, /// 0x994
			 0xdc3af73e, /// 0x998
			 0xecea946b, /// 0x99c
			 0xaaa15287, /// 0x9a0
			 0x314d905e, /// 0x9a4
			 0xbe4c1705, /// 0x9a8
			 0xa5a5bfa5, /// 0x9ac
			 0x2831c1b9, /// 0x9b0
			 0xec448a8f, /// 0x9b4
			 0x4d9bdc64, /// 0x9b8
			 0x8cacb7a3, /// 0x9bc
			 0x9a047468, /// 0x9c0
			 0x6a255128, /// 0x9c4
			 0xebfa7919, /// 0x9c8
			 0x96e91662, /// 0x9cc
			 0x1675f250, /// 0x9d0
			 0x016970cc, /// 0x9d4
			 0x9b2455fd, /// 0x9d8
			 0x594f16ae, /// 0x9dc
			 0x10757530, /// 0x9e0
			 0xafd63b44, /// 0x9e4
			 0xe378f754, /// 0x9e8
			 0x59f5ce10, /// 0x9ec
			 0x5a728ca0, /// 0x9f0
			 0x669531d7, /// 0x9f4
			 0xc380ffd2, /// 0x9f8
			 0x37d4689a, /// 0x9fc
			 0x53b9946c, /// 0xa00
			 0xbb5bb6b7, /// 0xa04
			 0x96c084fd, /// 0xa08
			 0x8bc0aa9b, /// 0xa0c
			 0xc511ab70, /// 0xa10
			 0xbac0d2b7, /// 0xa14
			 0x00cee968, /// 0xa18
			 0x8a02dc71, /// 0xa1c
			 0xdb09033c, /// 0xa20
			 0xf19ba22a, /// 0xa24
			 0x77d19944, /// 0xa28
			 0xf5dfd238, /// 0xa2c
			 0x1319cc35, /// 0xa30
			 0x829a5cf1, /// 0xa34
			 0x48aa6923, /// 0xa38
			 0x9d980906, /// 0xa3c
			 0x7d2de6ec, /// 0xa40
			 0xa3369d7e, /// 0xa44
			 0x3ada25d3, /// 0xa48
			 0xf76d50b2, /// 0xa4c
			 0x0154d446, /// 0xa50
			 0x2b472c1b, /// 0xa54
			 0x775c784e, /// 0xa58
			 0x2fd4d93a, /// 0xa5c
			 0x670bb1d3, /// 0xa60
			 0x5448988d, /// 0xa64
			 0x3c110945, /// 0xa68
			 0xb59aba14, /// 0xa6c
			 0x645d5249, /// 0xa70
			 0x2f9c7882, /// 0xa74
			 0x2230e9e3, /// 0xa78
			 0xe636d7a3, /// 0xa7c
			 0xa1de39c1, /// 0xa80
			 0x80739685, /// 0xa84
			 0x0c0bb2a3, /// 0xa88
			 0xdf169f04, /// 0xa8c
			 0xdaf92aa9, /// 0xa90
			 0xc36bcb43, /// 0xa94
			 0xfa91cb00, /// 0xa98
			 0x9032af8d, /// 0xa9c
			 0xdd0a6a1f, /// 0xaa0
			 0x21f5d8d3, /// 0xaa4
			 0xb74355e3, /// 0xaa8
			 0xff28679d, /// 0xaac
			 0xbb696b7a, /// 0xab0
			 0xc8395d1d, /// 0xab4
			 0x44648f99, /// 0xab8
			 0x6920ee1c, /// 0xabc
			 0x3ac9433a, /// 0xac0
			 0x6af814af, /// 0xac4
			 0x53936452, /// 0xac8
			 0xa94a5d04, /// 0xacc
			 0x98894241, /// 0xad0
			 0x8073284b, /// 0xad4
			 0xf3c6dd97, /// 0xad8
			 0xd3a7c751, /// 0xadc
			 0x82acd378, /// 0xae0
			 0x85c475c2, /// 0xae4
			 0x52360c75, /// 0xae8
			 0x76306ea3, /// 0xaec
			 0xe92fc95f, /// 0xaf0
			 0x225bc494, /// 0xaf4
			 0xd1badc0d, /// 0xaf8
			 0x58407338, /// 0xafc
			 0x830f29f0, /// 0xb00
			 0x40c444e8, /// 0xb04
			 0xfea13bae, /// 0xb08
			 0x57b6dba8, /// 0xb0c
			 0x061135dd, /// 0xb10
			 0x559c483a, /// 0xb14
			 0x0cb2a300, /// 0xb18
			 0x9a7fa5e6, /// 0xb1c
			 0x30cc6525, /// 0xb20
			 0x91227de4, /// 0xb24
			 0x56b2410d, /// 0xb28
			 0xc2eaac7d, /// 0xb2c
			 0x21f65527, /// 0xb30
			 0xd27bc5c9, /// 0xb34
			 0xd934213f, /// 0xb38
			 0x80f04640, /// 0xb3c
			 0x259a64af, /// 0xb40
			 0xf1a65348, /// 0xb44
			 0xdb7c55c6, /// 0xb48
			 0x556de952, /// 0xb4c
			 0x271c0393, /// 0xb50
			 0x8762262b, /// 0xb54
			 0xf238d33c, /// 0xb58
			 0x7aac30be, /// 0xb5c
			 0x79296a24, /// 0xb60
			 0x2baea1b6, /// 0xb64
			 0x332fb5fa, /// 0xb68
			 0x05697175, /// 0xb6c
			 0xc809e6c5, /// 0xb70
			 0x71bbd4dd, /// 0xb74
			 0x130c1c91, /// 0xb78
			 0x5ecab36a, /// 0xb7c
			 0x5b97fae1, /// 0xb80
			 0x939ee647, /// 0xb84
			 0x663bd12f, /// 0xb88
			 0x271d4d26, /// 0xb8c
			 0xf786f44b, /// 0xb90
			 0x1a73c4b6, /// 0xb94
			 0xe50a7255, /// 0xb98
			 0x31dcbea3, /// 0xb9c
			 0xfcc13595, /// 0xba0
			 0x51949fbf, /// 0xba4
			 0xa858f23f, /// 0xba8
			 0xe4bbe6ca, /// 0xbac
			 0x7233b0b3, /// 0xbb0
			 0xdd482049, /// 0xbb4
			 0xe4c81506, /// 0xbb8
			 0x459a4412, /// 0xbbc
			 0xb98324e3, /// 0xbc0
			 0x37349d68, /// 0xbc4
			 0x15fbaca8, /// 0xbc8
			 0xf1a1873e, /// 0xbcc
			 0x4cf5af5e, /// 0xbd0
			 0xf76012ba, /// 0xbd4
			 0x44b0a1c3, /// 0xbd8
			 0x5edc4716, /// 0xbdc
			 0x59cbbd3b, /// 0xbe0
			 0x6084d04b, /// 0xbe4
			 0xe85a5257, /// 0xbe8
			 0xd23915c4, /// 0xbec
			 0xd9ba2885, /// 0xbf0
			 0xc080e1a6, /// 0xbf4
			 0xd68545b8, /// 0xbf8
			 0x3a3cb512, /// 0xbfc
			 0xde7b44d4, /// 0xc00
			 0x3ea40beb, /// 0xc04
			 0x8fa9c1df, /// 0xc08
			 0xa81746d8, /// 0xc0c
			 0xb126d6b5, /// 0xc10
			 0xa3f3e574, /// 0xc14
			 0x04f4d4a5, /// 0xc18
			 0x4eed4572, /// 0xc1c
			 0xb8ed06cd, /// 0xc20
			 0x1eb5acfe, /// 0xc24
			 0x79bc0c2d, /// 0xc28
			 0x3bcdb056, /// 0xc2c
			 0xd1d7ca24, /// 0xc30
			 0x448b5334, /// 0xc34
			 0x3f8c5d0e, /// 0xc38
			 0xf1db8cdc, /// 0xc3c
			 0x7ce3562e, /// 0xc40
			 0x0535cd4e, /// 0xc44
			 0xc2a2b648, /// 0xc48
			 0xd7c6e08f, /// 0xc4c
			 0x10ffdf6e, /// 0xc50
			 0x88963ead, /// 0xc54
			 0xb19686f7, /// 0xc58
			 0xc9b75905, /// 0xc5c
			 0x6d0c0468, /// 0xc60
			 0x49e58939, /// 0xc64
			 0x545df08a, /// 0xc68
			 0x47bb5cda, /// 0xc6c
			 0xf122e65b, /// 0xc70
			 0x308aa321, /// 0xc74
			 0x60030681, /// 0xc78
			 0x392e20dd, /// 0xc7c
			 0x185c939c, /// 0xc80
			 0xad97eafa, /// 0xc84
			 0x14bc3439, /// 0xc88
			 0x8f5a2bbf, /// 0xc8c
			 0xa6a0848d, /// 0xc90
			 0xd964f570, /// 0xc94
			 0xa80812aa, /// 0xc98
			 0xcc9bb299, /// 0xc9c
			 0xeae47e25, /// 0xca0
			 0x6d0260ba, /// 0xca4
			 0xa5383599, /// 0xca8
			 0x60e573ba, /// 0xcac
			 0xf7535122, /// 0xcb0
			 0x77b8d314, /// 0xcb4
			 0xee1acdfd, /// 0xcb8
			 0x5c4c14bd, /// 0xcbc
			 0xa9e86805, /// 0xcc0
			 0x2432bdf2, /// 0xcc4
			 0x0779b43e, /// 0xcc8
			 0xf42d8b8d, /// 0xccc
			 0x100c5386, /// 0xcd0
			 0x3bea38ea, /// 0xcd4
			 0xbfebddd6, /// 0xcd8
			 0x5ee31dec, /// 0xcdc
			 0x45ad7339, /// 0xce0
			 0x92b4038a, /// 0xce4
			 0xfc306204, /// 0xce8
			 0xc169ca32, /// 0xcec
			 0x279fb0f1, /// 0xcf0
			 0xde86115e, /// 0xcf4
			 0xdadf58b1, /// 0xcf8
			 0xe513fe51, /// 0xcfc
			 0x7e202ed6, /// 0xd00
			 0xd61ad808, /// 0xd04
			 0xe609e138, /// 0xd08
			 0xcf937d5e, /// 0xd0c
			 0x0de1c554, /// 0xd10
			 0x846f773f, /// 0xd14
			 0x1502edc5, /// 0xd18
			 0x990a0795, /// 0xd1c
			 0xd42ef11a, /// 0xd20
			 0xb89750aa, /// 0xd24
			 0x2644e2ff, /// 0xd28
			 0x43857e63, /// 0xd2c
			 0xd59d0774, /// 0xd30
			 0x3a35e70f, /// 0xd34
			 0x96652fe9, /// 0xd38
			 0x36f22633, /// 0xd3c
			 0x142fda62, /// 0xd40
			 0xb36fd830, /// 0xd44
			 0x61bfebfd, /// 0xd48
			 0x17b6bb83, /// 0xd4c
			 0x4ea8ab64, /// 0xd50
			 0x2136f661, /// 0xd54
			 0x04b9bd9e, /// 0xd58
			 0x0ea41fd7, /// 0xd5c
			 0xb8543d06, /// 0xd60
			 0x0af80f13, /// 0xd64
			 0x4b8041f6, /// 0xd68
			 0xb11b7e0d, /// 0xd6c
			 0xc340dcab, /// 0xd70
			 0x7a0f116a, /// 0xd74
			 0x1416c593, /// 0xd78
			 0xdf7d4daf, /// 0xd7c
			 0x3378847a, /// 0xd80
			 0x196e3e6b, /// 0xd84
			 0xf1380373, /// 0xd88
			 0x1f8c7ed3, /// 0xd8c
			 0x3283e2de, /// 0xd90
			 0xcd0603fd, /// 0xd94
			 0x0a388111, /// 0xd98
			 0x7063a7e4, /// 0xd9c
			 0x1b6e0570, /// 0xda0
			 0xb5927209, /// 0xda4
			 0x8521122d, /// 0xda8
			 0xb36f2a82, /// 0xdac
			 0xb9aa9bf4, /// 0xdb0
			 0x253c453b, /// 0xdb4
			 0x156edf14, /// 0xdb8
			 0xa10872e5, /// 0xdbc
			 0x5750f72a, /// 0xdc0
			 0x64299e14, /// 0xdc4
			 0xa7f90c0b, /// 0xdc8
			 0x49c77716, /// 0xdcc
			 0x8cdea948, /// 0xdd0
			 0xe0b504da, /// 0xdd4
			 0x83169671, /// 0xdd8
			 0xa02f507b, /// 0xddc
			 0x85d0cd84, /// 0xde0
			 0x71851b8e, /// 0xde4
			 0x1f2f07d7, /// 0xde8
			 0x1a5c7e17, /// 0xdec
			 0xb9e46844, /// 0xdf0
			 0x7399893c, /// 0xdf4
			 0xf6b09ee7, /// 0xdf8
			 0x20903ab6, /// 0xdfc
			 0xf1faadc5, /// 0xe00
			 0x103fd5e6, /// 0xe04
			 0x96aef6ab, /// 0xe08
			 0x5b03b2f3, /// 0xe0c
			 0x771465da, /// 0xe10
			 0x79388458, /// 0xe14
			 0x7a371886, /// 0xe18
			 0xb5fbddef, /// 0xe1c
			 0x143653be, /// 0xe20
			 0x92eac4c6, /// 0xe24
			 0xec15fa77, /// 0xe28
			 0xa0e259b5, /// 0xe2c
			 0x2f1c9231, /// 0xe30
			 0x1fe668f0, /// 0xe34
			 0x3b6cc7b4, /// 0xe38
			 0xdf31ef9b, /// 0xe3c
			 0x87d44b1d, /// 0xe40
			 0x95b3aff3, /// 0xe44
			 0x4a696f2a, /// 0xe48
			 0x972d94f9, /// 0xe4c
			 0xa859a2e0, /// 0xe50
			 0x06234972, /// 0xe54
			 0x5afb8358, /// 0xe58
			 0x2ab52f21, /// 0xe5c
			 0x565c7bcf, /// 0xe60
			 0xdc1c1042, /// 0xe64
			 0x96c2ca29, /// 0xe68
			 0x68b5c631, /// 0xe6c
			 0x96a6a786, /// 0xe70
			 0xcf9b3880, /// 0xe74
			 0xf1277536, /// 0xe78
			 0xd30bf04c, /// 0xe7c
			 0xa19b0070, /// 0xe80
			 0x25a3c837, /// 0xe84
			 0x8d845c74, /// 0xe88
			 0xc3b1f456, /// 0xe8c
			 0x7ebed541, /// 0xe90
			 0x4e3e6f4c, /// 0xe94
			 0x99f4b2d7, /// 0xe98
			 0x2c2b0867, /// 0xe9c
			 0x65e569ac, /// 0xea0
			 0x3ae02463, /// 0xea4
			 0x844a8404, /// 0xea8
			 0x3a44cf80, /// 0xeac
			 0x54cf7e46, /// 0xeb0
			 0x78f5ab58, /// 0xeb4
			 0x7a75a09f, /// 0xeb8
			 0x723ecf26, /// 0xebc
			 0x35aa32f7, /// 0xec0
			 0xe44cca72, /// 0xec4
			 0x3cf2ae75, /// 0xec8
			 0xf4bc1197, /// 0xecc
			 0x48c33764, /// 0xed0
			 0x15d349fc, /// 0xed4
			 0x0dbf6d13, /// 0xed8
			 0x6a044adb, /// 0xedc
			 0xac23b10b, /// 0xee0
			 0x3ac6eb65, /// 0xee4
			 0x96c731af, /// 0xee8
			 0x120813f5, /// 0xeec
			 0xc4741797, /// 0xef0
			 0x1f6005f3, /// 0xef4
			 0xefac5cde, /// 0xef8
			 0x1ad03ac2, /// 0xefc
			 0x0241b8e8, /// 0xf00
			 0xf7a65565, /// 0xf04
			 0x824505e5, /// 0xf08
			 0x514f39c4, /// 0xf0c
			 0x647d28cd, /// 0xf10
			 0x642d7f84, /// 0xf14
			 0xba94d990, /// 0xf18
			 0xffe91a17, /// 0xf1c
			 0x5d8d5dc2, /// 0xf20
			 0xf906d15c, /// 0xf24
			 0x91ad62bd, /// 0xf28
			 0xc3d7b454, /// 0xf2c
			 0x9caf806c, /// 0xf30
			 0x01e99845, /// 0xf34
			 0x2c48bfea, /// 0xf38
			 0xc35e3f13, /// 0xf3c
			 0x64407794, /// 0xf40
			 0xb395ec37, /// 0xf44
			 0xcd3d7264, /// 0xf48
			 0xa943c2cc, /// 0xf4c
			 0xa31547c0, /// 0xf50
			 0x30466ad7, /// 0xf54
			 0x92e83547, /// 0xf58
			 0x52d42566, /// 0xf5c
			 0x15a89c36, /// 0xf60
			 0xda9a8fb1, /// 0xf64
			 0x1d006081, /// 0xf68
			 0x4e380728, /// 0xf6c
			 0xc1667dcd, /// 0xf70
			 0x8d928e1c, /// 0xf74
			 0x7d76deb2, /// 0xf78
			 0xc46fd6e5, /// 0xf7c
			 0x98ddf871, /// 0xf80
			 0xb46eb24e, /// 0xf84
			 0x73ee4014, /// 0xf88
			 0xfb7f41b8, /// 0xf8c
			 0x9cf5bdbc, /// 0xf90
			 0x27bbdfdc, /// 0xf94
			 0x7a4d8623, /// 0xf98
			 0x83cf268e, /// 0xf9c
			 0xd0fa2198, /// 0xfa0
			 0x00484423, /// 0xfa4
			 0x16d85ca9, /// 0xfa8
			 0x26e88332, /// 0xfac
			 0xfc3d02f7, /// 0xfb0
			 0x6c0c5454, /// 0xfb4
			 0xe6ac3f3a, /// 0xfb8
			 0xd36ce33f, /// 0xfbc
			 0x342ac889, /// 0xfc0
			 0x17a145bf, /// 0xfc4
			 0x90c119b6, /// 0xfc8
			 0xd71d1ccb, /// 0xfcc
			 0xa1799fa2, /// 0xfd0
			 0x786a33d5, /// 0xfd4
			 0x534adef9, /// 0xfd8
			 0xc7094e66, /// 0xfdc
			 0xd3a75b8f, /// 0xfe0
			 0x51eaadf4, /// 0xfe4
			 0x95b9f1e1, /// 0xfe8
			 0xe687d660, /// 0xfec
			 0xb242fda5, /// 0xff0
			 0x9cf6663d, /// 0xff4
			 0x952f3b10, /// 0xff8
			 0x1411b4fd /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00000
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00008
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0000c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00010
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0c400000,                                                  // Leading 1s:                                 /// 0001c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00024
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00028
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00030
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00034
			 0x0e000003,                                                  // Trailing 1s:                                /// 00038
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0003c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00040
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00044
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00048
			 0x80011111,                                                  // -9.7958E-41                                 /// 0004c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00050
			 0x55555555,                                                  // 4 random values                             /// 00054
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00068
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0006c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00070
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00078
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0007c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00080
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00084
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00088
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0008c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00090
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00094
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00098
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0009c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0xbf028f5c,                                                  // -0.51                                       /// 000ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 000b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00100
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00104
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00108
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00110
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00114
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0011c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00120
			 0xffc00001,                                                  // -signaling NaN                              /// 00124
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00128
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0012c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00130
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0013c
			 0x3f028f5c,                                                  // 0.51                                        /// 00140
			 0xffc00001,                                                  // -signaling NaN                              /// 00144
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00148
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00154
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0015c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00160
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00164
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0016c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00170
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00174
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00178
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0xffc00001,                                                  // -signaling NaN                              /// 00180
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00188
			 0x0c780000,                                                  // Leading 1s:                                 /// 0018c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0xcb000000,                                                  // -8388608.0                                  /// 00198
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b4
			 0x7fc00001,                                                  // signaling NaN                               /// 001b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00200
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00204
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00210
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00220
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00224
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0022c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00230
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00234
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x80000000,                                                  // -zero                                       /// 00244
			 0x0c600000,                                                  // Leading 1s:                                 /// 00248
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0024c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00250
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00254
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00258
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0025c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00260
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00264
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00268
			 0x33333333,                                                  // 4 random values                             /// 0026c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00270
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00278
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0029c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002a4
			 0x00000000,                                                  // zero                                        /// 002a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xbf028f5c,                                                  // -0.51                                       /// 002b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00300
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00304
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00308
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0030c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x80011111,                                                  // -9.7958E-41                                 /// 00314
			 0x55555555,                                                  // 4 random values                             /// 00318
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0031c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00320
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00328
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00334
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00338
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00340
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00348
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00350
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00354
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00360
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00364
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0036c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00370
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00374
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0037c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0x0e000003,                                                  // Trailing 1s:                                /// 00384
			 0x0c780000,                                                  // Leading 1s:                                 /// 00388
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00390
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00398
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 003bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00400
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00404
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0040c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00410
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00414
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00418
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00420
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00424
			 0x00000000,                                                  // zero                                        /// 00428
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0042c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00430
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0043c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00448
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0044c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0045c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00460
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00468
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0046c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00470
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0047c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0048c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00490
			 0x0e000001,                                                  // Trailing 1s:                                /// 00494
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00498
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0049c
			 0x00000000,                                                  // zero                                        /// 004a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00500
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00504
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0050c
			 0x80000000,                                                  // -zero                                       /// 00510
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00518
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0051c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00524
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00528
			 0x0c600000,                                                  // Leading 1s:                                 /// 0052c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0053c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00540
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00544
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00548
			 0x0e000001,                                                  // Trailing 1s:                                /// 0054c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00550
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00554
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00558
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0055c
			 0x7f800000,                                                  // inf                                         /// 00560
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00564
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00568
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0056c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00570
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00574
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0057c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00580
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00584
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00588
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0058c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00594
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x4b000000,                                                  // 8388608.0                                   /// 005b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x4b000000,                                                  // 8388608.0                                   /// 005d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 005f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00604
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00608
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0060c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00610
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00614
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00618
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0061c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0xff800000,                                                  // -inf                                        /// 00624
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00628
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00630
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00634
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00640
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00644
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00648
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0064c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00654
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00658
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0065c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00660
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0066c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00670
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00678
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00680
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00688
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0068c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00690
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00698
			 0x0e000003,                                                  // Trailing 1s:                                /// 0069c
			 0xbf028f5c,                                                  // -0.51                                       /// 006a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006b4
			 0x33333333,                                                  // 4 random values                             /// 006b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00704
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0070c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00710
			 0x00011111,                                                  // 9.7958E-41                                  /// 00714
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0071c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00720
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0072c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00730
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00740
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00744
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00748
			 0x7fc00001,                                                  // signaling NaN                               /// 0074c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00750
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00760
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00768
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0076c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00770
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00774
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00778
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00784
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00788
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00790
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00794
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00798
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x7f800000,                                                  // inf                                         /// 007a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007f0
			 0x00000000,                                                  // zero                                        /// 007f4
			 0x4b000000,                                                  // 8388608.0                                   /// 007f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00804
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0080c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00810
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00818
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00820
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00824
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0082c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00830
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x55555555,                                                  // 4 random values                             /// 0083c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0xff800000,                                                  // -inf                                        /// 00844
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00848
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00854
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0085c
			 0x4b000000,                                                  // 8388608.0                                   /// 00860
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00864
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0c700000,                                                  // Leading 1s:                                 /// 0086c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00870
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00874
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0087c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x7f800000,                                                  // inf                                         /// 00884
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c600000,                                                  // Leading 1s:                                 /// 0088c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00890
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00894
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00898
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0089c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80000000,                                                  // -zero                                       /// 008b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008f8
			 0x00000000,                                                  // zero                                        /// 008fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00900
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00904
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0090c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00918
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0091c
			 0xff800000,                                                  // -inf                                        /// 00920
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00924
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00928
			 0x7f800000,                                                  // inf                                         /// 0092c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x00000000,                                                  // zero                                        /// 00938
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00940
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00944
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0094c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00954
			 0xbf028f5c,                                                  // -0.51                                       /// 00958
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0xffc00001,                                                  // -signaling NaN                              /// 00968
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0096c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00970
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00974
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00978
			 0x55555555,                                                  // 4 random values                             /// 0097c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00980
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00984
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00994
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00998
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0099c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 009b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009ec
			 0xffc00001,                                                  // -signaling NaN                              /// 009f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x4b000000,                                                  // 8388608.0                                   /// 009fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a2c
			 0xff800000,                                                  // -inf                                        /// 00a30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x7f800000,                                                  // inf                                         /// 00aa0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aa4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00aa8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ac0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ac8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00acc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ad0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ad8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ae0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00af0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00af8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00afc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b08
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00000000,                                                  // zero                                        /// 00b10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b14
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b28
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b30
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b3c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b44
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b64
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b78
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ba4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bc4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bcc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bd4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bdc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00be0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00be8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bf0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bf4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c08
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c20
			 0x33333333,                                                  // 4 random values                             /// 00c24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c2c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c54
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c78
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ca0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0xbf028f5c,                                                  // -0.51                                       /// 00cc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cc8
			 0xffc00001,                                                  // -signaling NaN                              /// 00ccc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cd0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cd4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ce0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ce4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ce8
			 0x7f800000,                                                  // inf                                         /// 00cec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cf0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d08
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d58
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d80
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d9c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00da8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00db0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00db8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dc4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dc8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dcc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ddc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00de0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00de8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00df4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00df8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dfc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e00
			 0xcb000000,                                                  // -8388608.0                                  /// 00e04
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0xbf028f5c,                                                  // -0.51                                       /// 00e28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e30
			 0xbf028f5c,                                                  // -0.51                                       /// 00e34
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e48
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e5c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e60
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e7c
			 0x7f800000,                                                  // inf                                         /// 00e80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ec4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ecc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ed0
			 0x00000000,                                                  // zero                                        /// 00ed4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ed8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00edc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ee4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ef0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ef4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f04
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f14
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f18
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f34
			 0xcb000000,                                                  // -8388608.0                                  /// 00f38
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f4c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f68
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x33333333,                                                  // 4 random values                             /// 00f74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x55555555,                                                  // 4 random values                             /// 00f98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fa8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fac
			 0xcb000000,                                                  // -8388608.0                                  /// 00fb0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fb8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fd0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fd8
			 0xff800000,                                                  // -inf                                        /// 00fdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fe8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ff0
			 0x80000000,                                                  // -zero                                       /// 00ff4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x00400000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x00000634,
			 0x00000198,
			 0x00000310,
			 0x000004c0,
			 0x0000014c,
			 0x00000570,
			 0x0000034c,
			 0x00000724,

			 /// vpu register f2
			 0x41f80000,
			 0x41d80000,
			 0x41b00000,
			 0x40800000,
			 0x40e00000,
			 0x42000000,
			 0x41800000,
			 0x41e00000,

			 /// vpu register f3
			 0x41700000,
			 0x41000000,
			 0x40e00000,
			 0x41f00000,
			 0x41f80000,
			 0x41d00000,
			 0x41d80000,
			 0x41000000,

			 /// vpu register f4
			 0x41300000,
			 0x40400000,
			 0x41700000,
			 0x40e00000,
			 0x40800000,
			 0x41a00000,
			 0x41d80000,
			 0x41800000,

			 /// vpu register f5
			 0x40800000,
			 0x41000000,
			 0x41e80000,
			 0x40000000,
			 0x41800000,
			 0x40e00000,
			 0x41f80000,
			 0x40c00000,

			 /// vpu register f6
			 0x41200000,
			 0x41b00000,
			 0x41b00000,
			 0x40400000,
			 0x41b80000,
			 0x40400000,
			 0x41c00000,
			 0x41600000,

			 /// vpu register f7
			 0x41a80000,
			 0x41d00000,
			 0x41000000,
			 0x41c00000,
			 0x40000000,
			 0x41880000,
			 0x41b80000,
			 0x41f80000,

			 /// vpu register f8
			 0x41000000,
			 0x41500000,
			 0x41600000,
			 0x41500000,
			 0x41d80000,
			 0x41c00000,
			 0x41a00000,
			 0x41600000,

			 /// vpu register f9
			 0x42000000,
			 0x40a00000,
			 0x42000000,
			 0x40800000,
			 0x41e80000,
			 0x40400000,
			 0x41a00000,
			 0x41f00000,

			 /// vpu register f10
			 0x40400000,
			 0x40e00000,
			 0x40400000,
			 0x41a00000,
			 0x41800000,
			 0x3f800000,
			 0x41e00000,
			 0x41a80000,

			 /// vpu register f11
			 0x40400000,
			 0x40c00000,
			 0x40400000,
			 0x41300000,
			 0x41500000,
			 0x41c80000,
			 0x41800000,
			 0x41880000,

			 /// vpu register f12
			 0x40c00000,
			 0x41c80000,
			 0x41300000,
			 0x41300000,
			 0x41a00000,
			 0x40c00000,
			 0x42000000,
			 0x41400000,

			 /// vpu register f13
			 0x41800000,
			 0x41500000,
			 0x41c00000,
			 0x41900000,
			 0x40000000,
			 0x41400000,
			 0x41d80000,
			 0x41b00000,

			 /// vpu register f14
			 0x41d00000,
			 0x41800000,
			 0x41980000,
			 0x41300000,
			 0x41e80000,
			 0x41b00000,
			 0x40000000,
			 0x41900000,

			 /// vpu register f15
			 0x40400000,
			 0x41500000,
			 0x41d00000,
			 0x40e00000,
			 0x41900000,
			 0x41d00000,
			 0x41a80000,
			 0x41b00000,

			 /// vpu register f16
			 0x41880000,
			 0x3f800000,
			 0x41000000,
			 0x41200000,
			 0x40400000,
			 0x41500000,
			 0x40c00000,
			 0x41b80000,

			 /// vpu register f17
			 0x41800000,
			 0x41d00000,
			 0x40e00000,
			 0x41f80000,
			 0x41c00000,
			 0x41d80000,
			 0x41900000,
			 0x41b80000,

			 /// vpu register f18
			 0x41980000,
			 0x41b80000,
			 0x41700000,
			 0x40a00000,
			 0x41f00000,
			 0x41800000,
			 0x41600000,
			 0x40400000,

			 /// vpu register f19
			 0x40e00000,
			 0x41900000,
			 0x41100000,
			 0x41400000,
			 0x41a00000,
			 0x41f80000,
			 0x41f80000,
			 0x41d80000,

			 /// vpu register f20
			 0x41900000,
			 0x41300000,
			 0x40400000,
			 0x41900000,
			 0x41e80000,
			 0x41b80000,
			 0x41880000,
			 0x41100000,

			 /// vpu register f21
			 0x41700000,
			 0x42000000,
			 0x41d00000,
			 0x40800000,
			 0x41980000,
			 0x40a00000,
			 0x41400000,
			 0x40800000,

			 /// vpu register f22
			 0x40800000,
			 0x41f00000,
			 0x42000000,
			 0x41980000,
			 0x41200000,
			 0x41100000,
			 0x42000000,
			 0x41d80000,

			 /// vpu register f23
			 0x40a00000,
			 0x41d00000,
			 0x40000000,
			 0x41f00000,
			 0x41e00000,
			 0x41600000,
			 0x41f00000,
			 0x41880000,

			 /// vpu register f24
			 0x41c00000,
			 0x41100000,
			 0x41200000,
			 0x40c00000,
			 0x42000000,
			 0x41300000,
			 0x40e00000,
			 0x41c80000,

			 /// vpu register f25
			 0x41100000,
			 0x41f80000,
			 0x41200000,
			 0x41b00000,
			 0x41b00000,
			 0x41500000,
			 0x40000000,
			 0x40e00000,

			 /// vpu register f26
			 0x40000000,
			 0x41a00000,
			 0x41900000,
			 0x40800000,
			 0x41d00000,
			 0x41e80000,
			 0x41b00000,
			 0x41300000,

			 /// vpu register f27
			 0x41d80000,
			 0x41f80000,
			 0x41d80000,
			 0x41a00000,
			 0x41500000,
			 0x41e00000,
			 0x40a00000,
			 0x41700000,

			 /// vpu register f28
			 0x41f00000,
			 0x41600000,
			 0x41980000,
			 0x41a00000,
			 0x41200000,
			 0x41b00000,
			 0x41880000,
			 0x41000000,

			 /// vpu register f29
			 0x41800000,
			 0x41500000,
			 0x41880000,
			 0x40000000,
			 0x41500000,
			 0x42000000,
			 0x41000000,
			 0x41e00000,

			 /// vpu register f30
			 0x41300000,
			 0x42000000,
			 0x41980000,
			 0x40c00000,
			 0x41800000,
			 0x40e00000,
			 0x41880000,
			 0x41c00000,

			 /// vpu register f31
			 0x41700000,
			 0x41200000,
			 0x41200000,
			 0x41900000,
			 0x41900000,
			 0x41500000,
			 0x40000000,
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
		"fxor.pi f2, f16, f23\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f14, f24, f19\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f24, f12, f24\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f25, f28, f19\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f29, f16, f13\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f29, f26, f6\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f23, f4, f3\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f5, f2, f14\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f10, f19, f22\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f16, f4, f10\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f28, f12, f27\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f6, f0, f21\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f22, f2, f5\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f22, f2, f0\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f27, f26, f5\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f17, f25, f13\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f3, f11, f18\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f15, f17, f1\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f15, f3, f10\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f11, f14, f29\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f12, f6, f29\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f21, f7, f15\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f30, f11, f14\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f27, f22, f22\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f12, f3, f28\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f13, f10, f19\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f28, f18, f11\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f5, f2, f1\n"                                ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f30, f29, f29\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f30, f15, f26\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f24, f22, f27\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f27, f22, f9\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f17, f5, f1\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f29, f29, f29\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f13, f7, f19\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f22, f21, f2\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f21, f7, f6\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f4, f2, f16\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f30, f18, f30\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f27, f6, f23\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f14, f13, f19\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f30, f29, f21\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f13, f14, f29\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f23, f30, f20\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f20, f16, f18\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f3, f6, f8\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f11, f30, f13\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f13, f27, f8\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f2, f11, f23\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f16, f7, f5\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f2, f27, f3\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f11, f29, f1\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f9, f1, f7\n"                                ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f5, f15, f13\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f18, f0, f8\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f2, f13, f7\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f20, f21, f25\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f9, f27, f29\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f19, f24, f27\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f18, f21, f27\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f24, f8, f27\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f7, f18, f7\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f23, f2, f3\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f2, f2, f28\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f27, f7, f3\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f13, f30, f27\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f26, f4, f18\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f28, f8, f27\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f27, f11, f7\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f26, f14, f3\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f23, f11, f15\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f26, f28, f3\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f27, f14, f30\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f25, f22, f4\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f2, f26, f11\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f13, f8, f6\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f11, f21, f10\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f12, f22, f30\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f18, f14, f20\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f10, f13, f12\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f6, f6, f3\n"                                ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f27, f8, f22\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f28, f3, f1\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f13, f5, f3\n"                               ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f28, f21, f3\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f8, f18, f15\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f30, f19, f9\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f2, f13, f14\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f9, f7, f26\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f11, f0, f2\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f9, f26, f1\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f2, f18, f14\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f14, f26, f10\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f24, f16, f16\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f4, f17, f14\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f18, f4, f1\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f25, f0, f24\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f9, f22, f27\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fxor.pi f11, f16, f12\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
