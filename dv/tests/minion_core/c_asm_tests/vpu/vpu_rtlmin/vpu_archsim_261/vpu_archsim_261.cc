/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//////////////////////////////////////////////////////////////////////// 
////// VPU FOCUS TEST: seed = 0x4976d1c  = 77032732
////// VPU FOCUS TEST: isa_focus = vpu_non_tensor
////// VPU FOCUS TEST: uarch_focus = uarch_default
////// VPU FOCUS TEST: cvg_choice = 0
//////////////////////////////////////////////////////////////////////// 
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
			 0x2d6c1e45, /// 0x0
			 0x835bc725, /// 0x4
			 0x3ebef8c8, /// 0x8
			 0xfa89c995, /// 0xc
			 0x753b99f3, /// 0x10
			 0x8d7a46cd, /// 0x14
			 0x8fd86786, /// 0x18
			 0x59d687e6, /// 0x1c
			 0x8f78fa72, /// 0x20
			 0x5e31e0af, /// 0x24
			 0x0b355263, /// 0x28
			 0x4a94a8c2, /// 0x2c
			 0xb927f1e5, /// 0x30
			 0x2440f863, /// 0x34
			 0xeeaa092b, /// 0x38
			 0x93cfe32b, /// 0x3c
			 0x629f1826, /// 0x40
			 0x532bd9ea, /// 0x44
			 0x8ed6df24, /// 0x48
			 0x59f8ccf6, /// 0x4c
			 0xd179c614, /// 0x50
			 0x79721518, /// 0x54
			 0x95aea774, /// 0x58
			 0xcec62d5a, /// 0x5c
			 0x670c9bd0, /// 0x60
			 0x1773212c, /// 0x64
			 0x36a7c922, /// 0x68
			 0x5c4d3fde, /// 0x6c
			 0xd3c84163, /// 0x70
			 0xa1f66551, /// 0x74
			 0x68bb7a34, /// 0x78
			 0xa46814d0, /// 0x7c
			 0x39bebd4f, /// 0x80
			 0x0c38aa1a, /// 0x84
			 0x2200057b, /// 0x88
			 0xbc77cbb3, /// 0x8c
			 0x28bd3528, /// 0x90
			 0x9d7914b1, /// 0x94
			 0xf44b8330, /// 0x98
			 0xdb7b5d0d, /// 0x9c
			 0xfbbee971, /// 0xa0
			 0x0d72b2de, /// 0xa4
			 0x27e5e452, /// 0xa8
			 0x5d43653e, /// 0xac
			 0xdc5233cf, /// 0xb0
			 0xd2136ba0, /// 0xb4
			 0x5123dda4, /// 0xb8
			 0xe4a1f2e8, /// 0xbc
			 0x17c19107, /// 0xc0
			 0x42ef206d, /// 0xc4
			 0x2812e64b, /// 0xc8
			 0xb6c28296, /// 0xcc
			 0x3918bd60, /// 0xd0
			 0x794f0a87, /// 0xd4
			 0xd34c0bb4, /// 0xd8
			 0x1e85ced8, /// 0xdc
			 0x4a21c787, /// 0xe0
			 0xd83f313f, /// 0xe4
			 0x688479e6, /// 0xe8
			 0x00933869, /// 0xec
			 0x05916de7, /// 0xf0
			 0x41197fd3, /// 0xf4
			 0xe7959d71, /// 0xf8
			 0x9c1ffc39, /// 0xfc
			 0x3279de72, /// 0x100
			 0x1c7c98d9, /// 0x104
			 0x5f0c4468, /// 0x108
			 0x8d023923, /// 0x10c
			 0x063f1afe, /// 0x110
			 0x205f919c, /// 0x114
			 0xaa6b8869, /// 0x118
			 0x31fe0f3d, /// 0x11c
			 0x520cd677, /// 0x120
			 0x38955fe5, /// 0x124
			 0x4bb0c491, /// 0x128
			 0xc97fccda, /// 0x12c
			 0xd6630d0d, /// 0x130
			 0x5c1c455b, /// 0x134
			 0x990821a2, /// 0x138
			 0xbcbe88d7, /// 0x13c
			 0x260a41ad, /// 0x140
			 0xb41c1b80, /// 0x144
			 0x25dd1fee, /// 0x148
			 0x8296b2f1, /// 0x14c
			 0x8fad79a5, /// 0x150
			 0xb9b6c8e6, /// 0x154
			 0x025385c3, /// 0x158
			 0x3091dece, /// 0x15c
			 0xb7ccb313, /// 0x160
			 0x469d0477, /// 0x164
			 0x77608568, /// 0x168
			 0xd70373d5, /// 0x16c
			 0x7ac5d6d4, /// 0x170
			 0xa8972c11, /// 0x174
			 0x76256ad5, /// 0x178
			 0xb1fe7ded, /// 0x17c
			 0xb1bffde0, /// 0x180
			 0x2f84da37, /// 0x184
			 0x7888d1ac, /// 0x188
			 0xb5f0c558, /// 0x18c
			 0xe0d1c828, /// 0x190
			 0x50b3414f, /// 0x194
			 0x5693d52c, /// 0x198
			 0x74400fb4, /// 0x19c
			 0x574c741a, /// 0x1a0
			 0x5c4c846f, /// 0x1a4
			 0xc01dc072, /// 0x1a8
			 0x4262317a, /// 0x1ac
			 0x65682262, /// 0x1b0
			 0xf9ecb866, /// 0x1b4
			 0x5d1aecd7, /// 0x1b8
			 0x368ffe02, /// 0x1bc
			 0x657a99a4, /// 0x1c0
			 0x73ea8683, /// 0x1c4
			 0x273a6a02, /// 0x1c8
			 0x0362cdbd, /// 0x1cc
			 0xa4bc247a, /// 0x1d0
			 0xd86c69d1, /// 0x1d4
			 0x2d06410a, /// 0x1d8
			 0x20505f9e, /// 0x1dc
			 0xd8f41df2, /// 0x1e0
			 0xb6447977, /// 0x1e4
			 0x415a26a7, /// 0x1e8
			 0x3844bff2, /// 0x1ec
			 0x8848a1e2, /// 0x1f0
			 0x62a6ffbe, /// 0x1f4
			 0xbbc62bf8, /// 0x1f8
			 0xf40d5245, /// 0x1fc
			 0xf0e9858a, /// 0x200
			 0x18709c1d, /// 0x204
			 0xc64d9a1c, /// 0x208
			 0xe605b2f7, /// 0x20c
			 0xe22d7992, /// 0x210
			 0x78ec99d9, /// 0x214
			 0x6c625255, /// 0x218
			 0x5cbd11cf, /// 0x21c
			 0x9938ad7f, /// 0x220
			 0xf1d46298, /// 0x224
			 0x264a020a, /// 0x228
			 0x7204389c, /// 0x22c
			 0x913cf763, /// 0x230
			 0x9c5b0dc6, /// 0x234
			 0xf2f25232, /// 0x238
			 0xe57fbeda, /// 0x23c
			 0x027bedd9, /// 0x240
			 0xd83b221a, /// 0x244
			 0xdc15d7a3, /// 0x248
			 0xad3316e3, /// 0x24c
			 0x401614f9, /// 0x250
			 0xc6b222c0, /// 0x254
			 0x6e8dfe2f, /// 0x258
			 0x5e1a1feb, /// 0x25c
			 0x8d975e24, /// 0x260
			 0xa1612e19, /// 0x264
			 0x6777d666, /// 0x268
			 0x87e6f42d, /// 0x26c
			 0x89afdee7, /// 0x270
			 0x0001a83e, /// 0x274
			 0xa4a9ba40, /// 0x278
			 0x0fd7e4a2, /// 0x27c
			 0xe5df1fa4, /// 0x280
			 0xcb840e5e, /// 0x284
			 0x598dbd73, /// 0x288
			 0xe9c1318a, /// 0x28c
			 0xaad8aab4, /// 0x290
			 0xe208aa11, /// 0x294
			 0xc0023cd3, /// 0x298
			 0x97f814db, /// 0x29c
			 0x891e0381, /// 0x2a0
			 0x45d848b2, /// 0x2a4
			 0x378a9776, /// 0x2a8
			 0x2603ebb5, /// 0x2ac
			 0x5470b3d0, /// 0x2b0
			 0x76dbf266, /// 0x2b4
			 0x88445791, /// 0x2b8
			 0xaa68c318, /// 0x2bc
			 0xfc3b7fe1, /// 0x2c0
			 0xf026ff92, /// 0x2c4
			 0x9b3ee71b, /// 0x2c8
			 0x31e9bf85, /// 0x2cc
			 0x64f4ba10, /// 0x2d0
			 0xccd90280, /// 0x2d4
			 0x18c3d96d, /// 0x2d8
			 0x266545c0, /// 0x2dc
			 0x6b2abdf2, /// 0x2e0
			 0x6b85ab1e, /// 0x2e4
			 0xb57c3d6e, /// 0x2e8
			 0xd016d6a3, /// 0x2ec
			 0xeb2b8f0c, /// 0x2f0
			 0xd02fdf01, /// 0x2f4
			 0x8d3e441d, /// 0x2f8
			 0x26fe58a1, /// 0x2fc
			 0xc05cb05f, /// 0x300
			 0x545d1e5f, /// 0x304
			 0xc3b13d97, /// 0x308
			 0xc9a57622, /// 0x30c
			 0x6ba345f4, /// 0x310
			 0xafaaa30a, /// 0x314
			 0x363ecc0b, /// 0x318
			 0x78085b55, /// 0x31c
			 0x4c90b03f, /// 0x320
			 0xa7fd2551, /// 0x324
			 0x65910473, /// 0x328
			 0x53c82962, /// 0x32c
			 0xbf8b72a1, /// 0x330
			 0x7f3a7ba5, /// 0x334
			 0xaab3f6ce, /// 0x338
			 0x18092247, /// 0x33c
			 0x7fe1de0e, /// 0x340
			 0x0522e7b8, /// 0x344
			 0x6fcc55f1, /// 0x348
			 0x317d8a75, /// 0x34c
			 0x908bc7b6, /// 0x350
			 0x341d29c0, /// 0x354
			 0xdcd10fcb, /// 0x358
			 0x149d7485, /// 0x35c
			 0x0af7bb94, /// 0x360
			 0xa9be8213, /// 0x364
			 0x9f889e47, /// 0x368
			 0x556e8b69, /// 0x36c
			 0x2ce25a82, /// 0x370
			 0x70f855ee, /// 0x374
			 0xd11c8080, /// 0x378
			 0xd4f6fb9e, /// 0x37c
			 0x610776b4, /// 0x380
			 0x659bc05b, /// 0x384
			 0xf97f29e1, /// 0x388
			 0x260b4a72, /// 0x38c
			 0x656c7fc5, /// 0x390
			 0x347e68ee, /// 0x394
			 0x547c16ae, /// 0x398
			 0x3e8bf923, /// 0x39c
			 0xf99dafc4, /// 0x3a0
			 0xe167f2c7, /// 0x3a4
			 0xef3d5ef5, /// 0x3a8
			 0x9786bf8e, /// 0x3ac
			 0xbeac1b34, /// 0x3b0
			 0xcb0f0d0e, /// 0x3b4
			 0x2624a37e, /// 0x3b8
			 0xc5a98127, /// 0x3bc
			 0xe138e5e7, /// 0x3c0
			 0xea701a6f, /// 0x3c4
			 0x81442ec0, /// 0x3c8
			 0xb61586b7, /// 0x3cc
			 0x513e44db, /// 0x3d0
			 0x98f9bc2e, /// 0x3d4
			 0x054eaf6a, /// 0x3d8
			 0x95ad2e99, /// 0x3dc
			 0xa9fd0dfb, /// 0x3e0
			 0xb5ed71f8, /// 0x3e4
			 0x63bd6fc7, /// 0x3e8
			 0x6d13f0cc, /// 0x3ec
			 0xd636fbce, /// 0x3f0
			 0xb57cd139, /// 0x3f4
			 0xad98b604, /// 0x3f8
			 0x4cc063bd, /// 0x3fc
			 0xf16bf7ac, /// 0x400
			 0x48ce9dd8, /// 0x404
			 0xb4b8d0f1, /// 0x408
			 0x96df4923, /// 0x40c
			 0x9b265380, /// 0x410
			 0xd2cc545a, /// 0x414
			 0x9195556c, /// 0x418
			 0x1a5e0489, /// 0x41c
			 0x14323250, /// 0x420
			 0x7ab1f5f4, /// 0x424
			 0xa15d4e14, /// 0x428
			 0x6a802fe2, /// 0x42c
			 0x2c7f69ad, /// 0x430
			 0xc43693fe, /// 0x434
			 0x4b4ebc2d, /// 0x438
			 0x9e739e3f, /// 0x43c
			 0x8474e826, /// 0x440
			 0xbf1275a4, /// 0x444
			 0x036b90de, /// 0x448
			 0x6cf3c728, /// 0x44c
			 0xcb62fe47, /// 0x450
			 0xd2a4427d, /// 0x454
			 0xcc275fb2, /// 0x458
			 0x25398e01, /// 0x45c
			 0xec24f71c, /// 0x460
			 0xfb827dc8, /// 0x464
			 0xd793b5b2, /// 0x468
			 0xfc73aedc, /// 0x46c
			 0x5ead1b6c, /// 0x470
			 0x90733d47, /// 0x474
			 0x9b2cdf79, /// 0x478
			 0x9c52345d, /// 0x47c
			 0xa65c73ea, /// 0x480
			 0xad52223d, /// 0x484
			 0x539b80f5, /// 0x488
			 0x5dd83e67, /// 0x48c
			 0xd66cea6e, /// 0x490
			 0x0e8f9f08, /// 0x494
			 0xe8a922ec, /// 0x498
			 0xae0a128c, /// 0x49c
			 0x0e6488d7, /// 0x4a0
			 0x10290883, /// 0x4a4
			 0x44aa559a, /// 0x4a8
			 0xc55e3669, /// 0x4ac
			 0x4cd4774b, /// 0x4b0
			 0x1bbf42e1, /// 0x4b4
			 0xc382a87e, /// 0x4b8
			 0x0ecc2f38, /// 0x4bc
			 0x794de6ef, /// 0x4c0
			 0x6370b5f1, /// 0x4c4
			 0x45abdba8, /// 0x4c8
			 0x4115c2c0, /// 0x4cc
			 0xbe2677b5, /// 0x4d0
			 0x27936e30, /// 0x4d4
			 0x6f80a346, /// 0x4d8
			 0x3bb40ef5, /// 0x4dc
			 0x371a0bba, /// 0x4e0
			 0x2b96f6ec, /// 0x4e4
			 0x5029a8c5, /// 0x4e8
			 0xde64a23f, /// 0x4ec
			 0x4d118e9c, /// 0x4f0
			 0x14abbf10, /// 0x4f4
			 0xa35f947d, /// 0x4f8
			 0x1843d88b, /// 0x4fc
			 0x52a03599, /// 0x500
			 0x1c2342e9, /// 0x504
			 0xcfa071bb, /// 0x508
			 0xd466796c, /// 0x50c
			 0xb9d04494, /// 0x510
			 0x6599903f, /// 0x514
			 0xd3e1d4e9, /// 0x518
			 0x4f6d48c6, /// 0x51c
			 0xd2c60a42, /// 0x520
			 0x25cc5e39, /// 0x524
			 0xc192a5c9, /// 0x528
			 0xf57fe5d2, /// 0x52c
			 0xdc872b5a, /// 0x530
			 0xca27d0e9, /// 0x534
			 0x9b13dba7, /// 0x538
			 0x34a352dd, /// 0x53c
			 0x3731bb16, /// 0x540
			 0x32052a61, /// 0x544
			 0x24ff7f7c, /// 0x548
			 0xaaadf89f, /// 0x54c
			 0x77c33615, /// 0x550
			 0x1a026360, /// 0x554
			 0xa451ff90, /// 0x558
			 0x4f3715bd, /// 0x55c
			 0x05a821f0, /// 0x560
			 0x129b2036, /// 0x564
			 0x2b37e1f9, /// 0x568
			 0xa508f894, /// 0x56c
			 0x3d086de0, /// 0x570
			 0xac5120b7, /// 0x574
			 0x43a4f9dd, /// 0x578
			 0xf46ca28d, /// 0x57c
			 0x408e8c88, /// 0x580
			 0xd32db7a0, /// 0x584
			 0xd0c4226d, /// 0x588
			 0x14d52ffe, /// 0x58c
			 0xeb34747c, /// 0x590
			 0xca81af65, /// 0x594
			 0x54b74c45, /// 0x598
			 0x3d74d2b6, /// 0x59c
			 0xfec2faf7, /// 0x5a0
			 0x79b92f58, /// 0x5a4
			 0xfdfdfcd2, /// 0x5a8
			 0x14e45b70, /// 0x5ac
			 0x13b9f963, /// 0x5b0
			 0xdda24638, /// 0x5b4
			 0xec9fdb04, /// 0x5b8
			 0x204c3735, /// 0x5bc
			 0xc9c3dbf7, /// 0x5c0
			 0xa9629f68, /// 0x5c4
			 0x4da97406, /// 0x5c8
			 0xb0d986b7, /// 0x5cc
			 0xfef38dab, /// 0x5d0
			 0x42735506, /// 0x5d4
			 0x128023b0, /// 0x5d8
			 0xeeddf602, /// 0x5dc
			 0xd4865ced, /// 0x5e0
			 0xe1e17ba3, /// 0x5e4
			 0x01479f5b, /// 0x5e8
			 0x8c9506e1, /// 0x5ec
			 0x2b972ac6, /// 0x5f0
			 0x150e6c84, /// 0x5f4
			 0x682b39b2, /// 0x5f8
			 0x2376eff3, /// 0x5fc
			 0x513a03c2, /// 0x600
			 0xab8d3bfc, /// 0x604
			 0x1e9e03c9, /// 0x608
			 0xa2696da9, /// 0x60c
			 0xa0d66856, /// 0x610
			 0x326d4a47, /// 0x614
			 0x9c309053, /// 0x618
			 0x3ad4138e, /// 0x61c
			 0x58a0b619, /// 0x620
			 0xaaa7e81c, /// 0x624
			 0x9863a5a2, /// 0x628
			 0xf9395987, /// 0x62c
			 0x28e8c89b, /// 0x630
			 0x62f61e73, /// 0x634
			 0x292715e9, /// 0x638
			 0x2ffbd258, /// 0x63c
			 0x16e5f5d1, /// 0x640
			 0xb913a84a, /// 0x644
			 0x7c3a67dd, /// 0x648
			 0x0e2c6225, /// 0x64c
			 0xe0eb5e79, /// 0x650
			 0x852f6ed7, /// 0x654
			 0xede08643, /// 0x658
			 0x95dd9673, /// 0x65c
			 0xdb789630, /// 0x660
			 0x6292e5a5, /// 0x664
			 0x12c291ce, /// 0x668
			 0x2cdd185a, /// 0x66c
			 0xde273277, /// 0x670
			 0x2944eab0, /// 0x674
			 0x4ee1ed63, /// 0x678
			 0xdb0a9c29, /// 0x67c
			 0x1012437e, /// 0x680
			 0xcde3230e, /// 0x684
			 0xbe829d96, /// 0x688
			 0xd92649b2, /// 0x68c
			 0x6f3eb3da, /// 0x690
			 0x6e58378d, /// 0x694
			 0x9542704a, /// 0x698
			 0xf1531d26, /// 0x69c
			 0x7985036a, /// 0x6a0
			 0x165f7a39, /// 0x6a4
			 0x41b3a577, /// 0x6a8
			 0x7b183f2e, /// 0x6ac
			 0x56b3419f, /// 0x6b0
			 0x717c2bf1, /// 0x6b4
			 0xa18f2ace, /// 0x6b8
			 0x27f267ad, /// 0x6bc
			 0x6c8368cc, /// 0x6c0
			 0x17cfa8ee, /// 0x6c4
			 0x6d2a6432, /// 0x6c8
			 0xcaae38cc, /// 0x6cc
			 0x1009cfb6, /// 0x6d0
			 0x42c26e8d, /// 0x6d4
			 0xe197af4f, /// 0x6d8
			 0xc62913ab, /// 0x6dc
			 0x36c83bad, /// 0x6e0
			 0x7b158e37, /// 0x6e4
			 0x3cf94e0a, /// 0x6e8
			 0x93205eba, /// 0x6ec
			 0xa634bf0e, /// 0x6f0
			 0x9845c2dc, /// 0x6f4
			 0x0d3a66c6, /// 0x6f8
			 0xdde55e62, /// 0x6fc
			 0xfbdc9ade, /// 0x700
			 0xf4c8e0d4, /// 0x704
			 0x5d892e84, /// 0x708
			 0xff88ef09, /// 0x70c
			 0x5de7703b, /// 0x710
			 0x7ed1691b, /// 0x714
			 0x1803aa8f, /// 0x718
			 0x25a1d2df, /// 0x71c
			 0xab52e605, /// 0x720
			 0x159d7f28, /// 0x724
			 0x1e376120, /// 0x728
			 0x11321a6f, /// 0x72c
			 0xf7bddc74, /// 0x730
			 0x99e4407e, /// 0x734
			 0xb8813f06, /// 0x738
			 0xddf64fd1, /// 0x73c
			 0xe67b7b32, /// 0x740
			 0xc6a15950, /// 0x744
			 0x46d98711, /// 0x748
			 0xf03f662e, /// 0x74c
			 0xa1c41260, /// 0x750
			 0xedaed2c5, /// 0x754
			 0xab984692, /// 0x758
			 0xe6dd190e, /// 0x75c
			 0x2338651c, /// 0x760
			 0x2809b03b, /// 0x764
			 0x2dd1e94b, /// 0x768
			 0x4fcd16c1, /// 0x76c
			 0x1f4d27ea, /// 0x770
			 0x2c6af35e, /// 0x774
			 0xa9807366, /// 0x778
			 0xc9fb99eb, /// 0x77c
			 0x06a13dec, /// 0x780
			 0x12f81d91, /// 0x784
			 0x5a21cc37, /// 0x788
			 0x209a34c5, /// 0x78c
			 0xc975f4c8, /// 0x790
			 0x08265a49, /// 0x794
			 0x91ad4b46, /// 0x798
			 0xf23f9535, /// 0x79c
			 0x3710248c, /// 0x7a0
			 0x3c55a24a, /// 0x7a4
			 0x1b4eb8b7, /// 0x7a8
			 0x974d215b, /// 0x7ac
			 0x80c8f0c1, /// 0x7b0
			 0x93ef1f46, /// 0x7b4
			 0xf6acf0f3, /// 0x7b8
			 0xae5dc038, /// 0x7bc
			 0x37b0666c, /// 0x7c0
			 0x3abc7d45, /// 0x7c4
			 0x63867441, /// 0x7c8
			 0x22b1de35, /// 0x7cc
			 0x9eda6599, /// 0x7d0
			 0x9c37772a, /// 0x7d4
			 0xa56209eb, /// 0x7d8
			 0xbb616c7a, /// 0x7dc
			 0xd8636d7b, /// 0x7e0
			 0x53cd5a2d, /// 0x7e4
			 0xa32116a4, /// 0x7e8
			 0xe0d0cdc9, /// 0x7ec
			 0xfc03341c, /// 0x7f0
			 0xd4085159, /// 0x7f4
			 0xab66b251, /// 0x7f8
			 0xf50f4672, /// 0x7fc
			 0xc71d9548, /// 0x800
			 0x9f8f50b9, /// 0x804
			 0x120c4a94, /// 0x808
			 0x6c0734c4, /// 0x80c
			 0x98c22f1a, /// 0x810
			 0x111aae2b, /// 0x814
			 0x738dcde0, /// 0x818
			 0x99ea938b, /// 0x81c
			 0xabb2f7e4, /// 0x820
			 0xf68974ee, /// 0x824
			 0x91c3d7ac, /// 0x828
			 0x9804ba70, /// 0x82c
			 0x562488e0, /// 0x830
			 0x34e58493, /// 0x834
			 0xee4820af, /// 0x838
			 0x9cb06496, /// 0x83c
			 0x852ca292, /// 0x840
			 0xa7c48176, /// 0x844
			 0xbda994b6, /// 0x848
			 0x751e4511, /// 0x84c
			 0x07d8a677, /// 0x850
			 0x6a9203ef, /// 0x854
			 0xd29344cd, /// 0x858
			 0xce7dc1be, /// 0x85c
			 0xa1913c12, /// 0x860
			 0x9814c340, /// 0x864
			 0x80f1c88c, /// 0x868
			 0x063e2c01, /// 0x86c
			 0x9096c34b, /// 0x870
			 0x81d107fe, /// 0x874
			 0x83f203df, /// 0x878
			 0x0cc6d05b, /// 0x87c
			 0x1c115506, /// 0x880
			 0xae26921a, /// 0x884
			 0x754acd47, /// 0x888
			 0x96f5538f, /// 0x88c
			 0xdf66e78c, /// 0x890
			 0x33e446fc, /// 0x894
			 0xbb078f44, /// 0x898
			 0xb82f2469, /// 0x89c
			 0x16989fb1, /// 0x8a0
			 0x67feca51, /// 0x8a4
			 0x1b9eb8dc, /// 0x8a8
			 0x5aceadf1, /// 0x8ac
			 0xb4a021e4, /// 0x8b0
			 0xd5f251cc, /// 0x8b4
			 0xf4eb8518, /// 0x8b8
			 0xfb2e6462, /// 0x8bc
			 0xcf97c7b6, /// 0x8c0
			 0x54878fc5, /// 0x8c4
			 0x51af2bb7, /// 0x8c8
			 0x339e6ad1, /// 0x8cc
			 0xff391bc7, /// 0x8d0
			 0xaeb97878, /// 0x8d4
			 0xdf24826f, /// 0x8d8
			 0xe2677727, /// 0x8dc
			 0xa2e1261e, /// 0x8e0
			 0xae139672, /// 0x8e4
			 0x8c5b86dd, /// 0x8e8
			 0xe0422d7e, /// 0x8ec
			 0xb7950916, /// 0x8f0
			 0x9924d8df, /// 0x8f4
			 0xc5218c1e, /// 0x8f8
			 0xb3e0fb9f, /// 0x8fc
			 0x4a89ec13, /// 0x900
			 0x96be732f, /// 0x904
			 0x64963baa, /// 0x908
			 0x64d67f28, /// 0x90c
			 0x52770fbf, /// 0x910
			 0x171585a7, /// 0x914
			 0xbf0fd2e2, /// 0x918
			 0x38d5c0ba, /// 0x91c
			 0x0c3f6d1e, /// 0x920
			 0x8c13d0b7, /// 0x924
			 0x326b8f58, /// 0x928
			 0xe638b852, /// 0x92c
			 0x3774c0ae, /// 0x930
			 0x85af5466, /// 0x934
			 0x39c90e17, /// 0x938
			 0xee93d8cf, /// 0x93c
			 0x78fc9e00, /// 0x940
			 0xdc759c65, /// 0x944
			 0xfff2a8aa, /// 0x948
			 0x3ce9cfb0, /// 0x94c
			 0xe81363f2, /// 0x950
			 0x2ee8bd9c, /// 0x954
			 0xd9551ad6, /// 0x958
			 0x666a787c, /// 0x95c
			 0xc9a5b857, /// 0x960
			 0xc5b5caad, /// 0x964
			 0x444a2a3f, /// 0x968
			 0xd36fd4fb, /// 0x96c
			 0xf0c35585, /// 0x970
			 0xc869bdc1, /// 0x974
			 0xa6294330, /// 0x978
			 0x9aa6f924, /// 0x97c
			 0x8676163e, /// 0x980
			 0xad86f9a6, /// 0x984
			 0xc6c9b615, /// 0x988
			 0xf4f3ec43, /// 0x98c
			 0xc5d617a5, /// 0x990
			 0x0218d377, /// 0x994
			 0x8e9e3e0e, /// 0x998
			 0x132ab46e, /// 0x99c
			 0x15dad038, /// 0x9a0
			 0x986d3ea2, /// 0x9a4
			 0xfe4cf9f1, /// 0x9a8
			 0x24a0419b, /// 0x9ac
			 0xfbe5ac95, /// 0x9b0
			 0x058b0513, /// 0x9b4
			 0x4b8e53f2, /// 0x9b8
			 0xe7b00d05, /// 0x9bc
			 0x93e6998d, /// 0x9c0
			 0x5b9e8e73, /// 0x9c4
			 0xd8145257, /// 0x9c8
			 0x8855bebf, /// 0x9cc
			 0xb131dd04, /// 0x9d0
			 0x91e21b3b, /// 0x9d4
			 0xbc856e0e, /// 0x9d8
			 0x952a4579, /// 0x9dc
			 0xcdb9f8a0, /// 0x9e0
			 0x0799d2bf, /// 0x9e4
			 0x929335f4, /// 0x9e8
			 0x2be9e6ba, /// 0x9ec
			 0xcce07f5b, /// 0x9f0
			 0x7c7f6649, /// 0x9f4
			 0xaa27c1ed, /// 0x9f8
			 0xc87cd51c, /// 0x9fc
			 0xf341e109, /// 0xa00
			 0xf9ff136a, /// 0xa04
			 0x5d165ddc, /// 0xa08
			 0xf8f9ab65, /// 0xa0c
			 0x04631e0a, /// 0xa10
			 0x1d590367, /// 0xa14
			 0x3f2154a2, /// 0xa18
			 0x7dd94241, /// 0xa1c
			 0xcc464b1a, /// 0xa20
			 0x330cfc7f, /// 0xa24
			 0x0811119e, /// 0xa28
			 0x000d994f, /// 0xa2c
			 0x30d8039f, /// 0xa30
			 0xef8ba432, /// 0xa34
			 0x4a13525a, /// 0xa38
			 0x36766fa0, /// 0xa3c
			 0xbede630e, /// 0xa40
			 0x229ffacd, /// 0xa44
			 0x025c64b7, /// 0xa48
			 0xb546f737, /// 0xa4c
			 0xb5a102a7, /// 0xa50
			 0x21f47551, /// 0xa54
			 0x385f619e, /// 0xa58
			 0xf52e1240, /// 0xa5c
			 0xa0d28597, /// 0xa60
			 0x409e7a14, /// 0xa64
			 0xd15ba27d, /// 0xa68
			 0x0ad5a9c8, /// 0xa6c
			 0x95cda168, /// 0xa70
			 0xfc0d0770, /// 0xa74
			 0x14080cde, /// 0xa78
			 0x3f8ae1a3, /// 0xa7c
			 0xf3f54c88, /// 0xa80
			 0x764c5f85, /// 0xa84
			 0x83fd3754, /// 0xa88
			 0x5f952ff0, /// 0xa8c
			 0x38118592, /// 0xa90
			 0xb4dcc775, /// 0xa94
			 0x6d0b55ba, /// 0xa98
			 0x21bb79ff, /// 0xa9c
			 0xc2535c05, /// 0xaa0
			 0x6ea45d20, /// 0xaa4
			 0xbfe1d90d, /// 0xaa8
			 0x82f1c1ff, /// 0xaac
			 0xd84479c3, /// 0xab0
			 0x659586ff, /// 0xab4
			 0xbb2dfa85, /// 0xab8
			 0x4555e2c0, /// 0xabc
			 0x5abe65b6, /// 0xac0
			 0xb2429b77, /// 0xac4
			 0xf8441571, /// 0xac8
			 0x2e798af1, /// 0xacc
			 0x60665845, /// 0xad0
			 0xed431d55, /// 0xad4
			 0x4c065a12, /// 0xad8
			 0xe6d1a50a, /// 0xadc
			 0xec2b06c5, /// 0xae0
			 0x85c77fb1, /// 0xae4
			 0xff654d9c, /// 0xae8
			 0x428ff062, /// 0xaec
			 0x3ddbfd73, /// 0xaf0
			 0x5eca8b40, /// 0xaf4
			 0x5771819c, /// 0xaf8
			 0x45938b2f, /// 0xafc
			 0xf1cb650c, /// 0xb00
			 0xd750db29, /// 0xb04
			 0x73637c59, /// 0xb08
			 0x69f6853b, /// 0xb0c
			 0xd539b5ba, /// 0xb10
			 0xf36af903, /// 0xb14
			 0xd8a27ea4, /// 0xb18
			 0xce406395, /// 0xb1c
			 0xfbd58808, /// 0xb20
			 0xba84acd8, /// 0xb24
			 0xf21784ae, /// 0xb28
			 0xf3e897d9, /// 0xb2c
			 0x93e2bdd9, /// 0xb30
			 0xdaa7d8af, /// 0xb34
			 0x5415dd47, /// 0xb38
			 0x2ff665c7, /// 0xb3c
			 0x2a757029, /// 0xb40
			 0x5e1efcd9, /// 0xb44
			 0x724910a3, /// 0xb48
			 0x09a83ab0, /// 0xb4c
			 0x3635e11c, /// 0xb50
			 0xcbec003e, /// 0xb54
			 0xfdaf533e, /// 0xb58
			 0x8d5223d9, /// 0xb5c
			 0x8c7fa5f7, /// 0xb60
			 0x9bac5459, /// 0xb64
			 0xbdb47230, /// 0xb68
			 0x01e6f636, /// 0xb6c
			 0x729bb820, /// 0xb70
			 0x7671c739, /// 0xb74
			 0x1f228f5c, /// 0xb78
			 0x08898e30, /// 0xb7c
			 0xdd720dd3, /// 0xb80
			 0x8fad977e, /// 0xb84
			 0xdd6ad3b5, /// 0xb88
			 0xcb475a95, /// 0xb8c
			 0x0619cecc, /// 0xb90
			 0xca4b9c39, /// 0xb94
			 0x1b3b69a5, /// 0xb98
			 0xdf9d20fb, /// 0xb9c
			 0x3aff14f0, /// 0xba0
			 0xf1a2ded2, /// 0xba4
			 0xbf0aa965, /// 0xba8
			 0xea9e3926, /// 0xbac
			 0xc151402a, /// 0xbb0
			 0xaab408b4, /// 0xbb4
			 0xfb1cf1a1, /// 0xbb8
			 0xba99c340, /// 0xbbc
			 0x3a4dcfd4, /// 0xbc0
			 0x43794120, /// 0xbc4
			 0x1efdc3cb, /// 0xbc8
			 0xfe4f541b, /// 0xbcc
			 0x7c10c405, /// 0xbd0
			 0x980fdef3, /// 0xbd4
			 0x1014e9c9, /// 0xbd8
			 0x5e16e409, /// 0xbdc
			 0xfa9d16d2, /// 0xbe0
			 0x02acdf3e, /// 0xbe4
			 0xb38630f7, /// 0xbe8
			 0x9e8ae7a0, /// 0xbec
			 0x7f2ac724, /// 0xbf0
			 0x34872af0, /// 0xbf4
			 0x21aacd95, /// 0xbf8
			 0xa942d005, /// 0xbfc
			 0x28d08aed, /// 0xc00
			 0xe5e4de56, /// 0xc04
			 0x8df22de2, /// 0xc08
			 0x27e68636, /// 0xc0c
			 0xf4270bcd, /// 0xc10
			 0x70411897, /// 0xc14
			 0x0f6a5c03, /// 0xc18
			 0xec07bc31, /// 0xc1c
			 0x36620b60, /// 0xc20
			 0x0c9e165b, /// 0xc24
			 0x1b7d640f, /// 0xc28
			 0xd4dbebc8, /// 0xc2c
			 0xe471e80a, /// 0xc30
			 0x4ee80e57, /// 0xc34
			 0x01610f47, /// 0xc38
			 0x4b26ed46, /// 0xc3c
			 0x5eeb3e6b, /// 0xc40
			 0xc76f9cd0, /// 0xc44
			 0x976f8bfd, /// 0xc48
			 0xc851a063, /// 0xc4c
			 0xd12ee8c9, /// 0xc50
			 0xcbb61793, /// 0xc54
			 0x60e957a4, /// 0xc58
			 0x3c6dc60c, /// 0xc5c
			 0x085bea65, /// 0xc60
			 0x7df18af2, /// 0xc64
			 0xf9d14a8e, /// 0xc68
			 0xa806a922, /// 0xc6c
			 0x0296a1bf, /// 0xc70
			 0x63c92ff6, /// 0xc74
			 0x0e386500, /// 0xc78
			 0x93c60680, /// 0xc7c
			 0x6f91a78f, /// 0xc80
			 0xa9fda4ef, /// 0xc84
			 0x9c449494, /// 0xc88
			 0x3e3d42a6, /// 0xc8c
			 0xaa2d2664, /// 0xc90
			 0xd2fe0367, /// 0xc94
			 0xd7bf38bb, /// 0xc98
			 0x7f4b70c4, /// 0xc9c
			 0x07822e80, /// 0xca0
			 0x02b38ca1, /// 0xca4
			 0x67148c17, /// 0xca8
			 0x8037ce8e, /// 0xcac
			 0x04aed7e5, /// 0xcb0
			 0x6ba825a6, /// 0xcb4
			 0x69f784d7, /// 0xcb8
			 0xf5b8b81d, /// 0xcbc
			 0x05655c1c, /// 0xcc0
			 0x47f492da, /// 0xcc4
			 0x099a7653, /// 0xcc8
			 0x01e9cc30, /// 0xccc
			 0xc000fa27, /// 0xcd0
			 0xf49840b9, /// 0xcd4
			 0x563016f5, /// 0xcd8
			 0x8430c66c, /// 0xcdc
			 0x42a4baaf, /// 0xce0
			 0x42e2977a, /// 0xce4
			 0xcc327d57, /// 0xce8
			 0x8a5c5bf2, /// 0xcec
			 0xe2aa2962, /// 0xcf0
			 0xdf65fb42, /// 0xcf4
			 0x0ac35f5e, /// 0xcf8
			 0x70895720, /// 0xcfc
			 0x240e477e, /// 0xd00
			 0x04f923e8, /// 0xd04
			 0x63e597a1, /// 0xd08
			 0xc9c3909f, /// 0xd0c
			 0xb2c50cd0, /// 0xd10
			 0x0ad2fb94, /// 0xd14
			 0xaa8501aa, /// 0xd18
			 0xc1b4b99c, /// 0xd1c
			 0x2f12f0b3, /// 0xd20
			 0x7963341d, /// 0xd24
			 0xe88b2782, /// 0xd28
			 0x7b6dca89, /// 0xd2c
			 0x1a440651, /// 0xd30
			 0xff99d2b9, /// 0xd34
			 0xb361251b, /// 0xd38
			 0xaaa5e20c, /// 0xd3c
			 0xaf5ad3db, /// 0xd40
			 0xe5ca86a2, /// 0xd44
			 0x73cc0f26, /// 0xd48
			 0x66f2acf1, /// 0xd4c
			 0x43a6f233, /// 0xd50
			 0xf162ba43, /// 0xd54
			 0x742e3963, /// 0xd58
			 0x57d41b09, /// 0xd5c
			 0x5a27979b, /// 0xd60
			 0xe7f97d1c, /// 0xd64
			 0x41fc9ca5, /// 0xd68
			 0xd989a968, /// 0xd6c
			 0x4434788a, /// 0xd70
			 0xd8e43dd5, /// 0xd74
			 0x6bb71901, /// 0xd78
			 0x7120a046, /// 0xd7c
			 0xa4f98f9c, /// 0xd80
			 0x11b246d8, /// 0xd84
			 0x4e31fdd4, /// 0xd88
			 0xc4d0c851, /// 0xd8c
			 0xe0b991c9, /// 0xd90
			 0x902e5fad, /// 0xd94
			 0x6c6396d2, /// 0xd98
			 0xcdb47b1e, /// 0xd9c
			 0x26909604, /// 0xda0
			 0xce2563ef, /// 0xda4
			 0x98a32a54, /// 0xda8
			 0x6561fcbd, /// 0xdac
			 0x5a6f58b2, /// 0xdb0
			 0xd53c3d8c, /// 0xdb4
			 0xfec297f5, /// 0xdb8
			 0xd613233e, /// 0xdbc
			 0x66f86e1f, /// 0xdc0
			 0xfe7270a6, /// 0xdc4
			 0xf3a5eaf4, /// 0xdc8
			 0x78ce6a26, /// 0xdcc
			 0x059839d5, /// 0xdd0
			 0x11ebf3ff, /// 0xdd4
			 0x533200a0, /// 0xdd8
			 0xee9be776, /// 0xddc
			 0x57c947c9, /// 0xde0
			 0xe791ae45, /// 0xde4
			 0x8f465749, /// 0xde8
			 0x8994949d, /// 0xdec
			 0xf8545f78, /// 0xdf0
			 0x82cf5e80, /// 0xdf4
			 0xecb8aad6, /// 0xdf8
			 0xd2622f0b, /// 0xdfc
			 0x0c6b429f, /// 0xe00
			 0xf508384e, /// 0xe04
			 0x99b65565, /// 0xe08
			 0xc1f47b8e, /// 0xe0c
			 0xfe2bf77d, /// 0xe10
			 0x030b8724, /// 0xe14
			 0x66dc6259, /// 0xe18
			 0xcd2b3944, /// 0xe1c
			 0x497a5f1e, /// 0xe20
			 0xd635cf3d, /// 0xe24
			 0x5fbc3dd5, /// 0xe28
			 0x4a7809d2, /// 0xe2c
			 0x17591b12, /// 0xe30
			 0x7d002959, /// 0xe34
			 0xaaed795d, /// 0xe38
			 0x53d04bf0, /// 0xe3c
			 0xf0e5a065, /// 0xe40
			 0x8214afba, /// 0xe44
			 0xd79d5611, /// 0xe48
			 0x233ec0c9, /// 0xe4c
			 0x99673ca5, /// 0xe50
			 0x2227ff1e, /// 0xe54
			 0x8f7e4fea, /// 0xe58
			 0x53ed1a6d, /// 0xe5c
			 0x4e7fc64e, /// 0xe60
			 0x05d30a33, /// 0xe64
			 0x21cfa617, /// 0xe68
			 0x408a36ac, /// 0xe6c
			 0xe88364b7, /// 0xe70
			 0xda50bd6c, /// 0xe74
			 0xe7e6b33b, /// 0xe78
			 0x3b9a071a, /// 0xe7c
			 0xa75db118, /// 0xe80
			 0xd7c5c34c, /// 0xe84
			 0x71593c77, /// 0xe88
			 0xeb06b4c9, /// 0xe8c
			 0x3649348d, /// 0xe90
			 0xb6302db2, /// 0xe94
			 0x57cc0a85, /// 0xe98
			 0xa57d6209, /// 0xe9c
			 0xe152ab36, /// 0xea0
			 0x7d576cc8, /// 0xea4
			 0xdd5ecfbd, /// 0xea8
			 0xa3f5959f, /// 0xeac
			 0x5d502fd6, /// 0xeb0
			 0x2aeea346, /// 0xeb4
			 0x318f7666, /// 0xeb8
			 0x91d53d00, /// 0xebc
			 0xb3c52f2a, /// 0xec0
			 0xf2f7003b, /// 0xec4
			 0xc6587da4, /// 0xec8
			 0x0a88ec60, /// 0xecc
			 0x6a39832d, /// 0xed0
			 0x9eb508c1, /// 0xed4
			 0x38ee6b81, /// 0xed8
			 0xd209321b, /// 0xedc
			 0xae5e74e3, /// 0xee0
			 0x18dff61c, /// 0xee4
			 0x6e46d7ca, /// 0xee8
			 0xfef759f5, /// 0xeec
			 0x26c65c4e, /// 0xef0
			 0x1c6d4f2e, /// 0xef4
			 0xde5192cc, /// 0xef8
			 0x596a7de6, /// 0xefc
			 0x3933ffbc, /// 0xf00
			 0x8b636ef6, /// 0xf04
			 0x84d135d8, /// 0xf08
			 0xe0927ba6, /// 0xf0c
			 0x328cd2db, /// 0xf10
			 0x681b345b, /// 0xf14
			 0x06598685, /// 0xf18
			 0x02547eae, /// 0xf1c
			 0x06c7f996, /// 0xf20
			 0xeab9186e, /// 0xf24
			 0x740f9031, /// 0xf28
			 0xc1d2ea8a, /// 0xf2c
			 0x19adadf7, /// 0xf30
			 0x8b9e9213, /// 0xf34
			 0x04b099fa, /// 0xf38
			 0x6608a0ec, /// 0xf3c
			 0x228cbdbc, /// 0xf40
			 0x4777b65c, /// 0xf44
			 0xb017a04b, /// 0xf48
			 0xac4e020a, /// 0xf4c
			 0x0dbf0650, /// 0xf50
			 0x9729c42a, /// 0xf54
			 0xd5624614, /// 0xf58
			 0xf9740705, /// 0xf5c
			 0x6d3db5e1, /// 0xf60
			 0xa9f5b4f0, /// 0xf64
			 0x385e8b24, /// 0xf68
			 0x517bccdf, /// 0xf6c
			 0x4a9c0fec, /// 0xf70
			 0xe61eebc1, /// 0xf74
			 0x6da669d0, /// 0xf78
			 0x0c46b160, /// 0xf7c
			 0x72add629, /// 0xf80
			 0x118c068e, /// 0xf84
			 0x9d4e184a, /// 0xf88
			 0x9ec42920, /// 0xf8c
			 0xa579b025, /// 0xf90
			 0x70196892, /// 0xf94
			 0x565c0296, /// 0xf98
			 0x28750ebb, /// 0xf9c
			 0x58ca6e91, /// 0xfa0
			 0x582157ac, /// 0xfa4
			 0x313ff574, /// 0xfa8
			 0x630e7485, /// 0xfac
			 0x4edf3e4b, /// 0xfb0
			 0x2eed29e8, /// 0xfb4
			 0xaf1d8eb8, /// 0xfb8
			 0x40399b2a, /// 0xfbc
			 0x141efd30, /// 0xfc0
			 0xe6799c48, /// 0xfc4
			 0x20aab413, /// 0xfc8
			 0x3532f5c9, /// 0xfcc
			 0xc5f9ff4f, /// 0xfd0
			 0x52c0b98b, /// 0xfd4
			 0x7d77e789, /// 0xfd8
			 0xc64e5cd0, /// 0xfdc
			 0xd076e1bc, /// 0xfe0
			 0x54071e88, /// 0xfe4
			 0xbb5e9360, /// 0xfe8
			 0xc482cc4b, /// 0xfec
			 0x007e2061, /// 0xff0
			 0xefad7016, /// 0xff4
			 0x027d48eb, /// 0xff8
			 0x37fc69f7 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00000
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00004
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0000c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00010
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x4b000000,                                                  // 8388608.0                                   /// 0001c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00020
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00024
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00028
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0002c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00030
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0003c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00044
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0004c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00050
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00054
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00058
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0005c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00068
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00074
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0007c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00080
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00088
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0008c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00090
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00098
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x7fc00001,                                                  // signaling NaN                               /// 000a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00104
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0010c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00110
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00118
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00120
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00124
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00128
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0012c
			 0xcb000000,                                                  // -8388608.0                                  /// 00130
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00134
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00138
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0013c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00140
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00144
			 0x00000000,                                                  // zero                                        /// 00148
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00158
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00160
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00164
			 0x80000000,                                                  // -zero                                       /// 00168
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0016c
			 0x7fc00001,                                                  // signaling NaN                               /// 00170
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00174
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0017c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00180
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00188
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00190
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00198
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0xbf028f5c,                                                  // -0.51                                       /// 001b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00204
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00208
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0020c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00210
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00218
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0021c
			 0xffc00001,                                                  // -signaling NaN                              /// 00220
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00224
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00228
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00230
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00234
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0023c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00244
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00248
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0025c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00260
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0026c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00270
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00280
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00284
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x3f028f5c,                                                  // 0.51                                        /// 0028c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00290
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00294
			 0x80011111,                                                  // -9.7958E-41                                 /// 00298
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0029c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002a4
			 0x7fc00001,                                                  // signaling NaN                               /// 002a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 002b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x4b000000,                                                  // 8388608.0                                   /// 002f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00304
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00308
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00314
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00318
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0031c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0032c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00334
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0033c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00344
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00348
			 0x80000000,                                                  // -zero                                       /// 0034c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00358
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x0e000001,                                                  // Trailing 1s:                                /// 00364
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00368
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0036c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00370
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00378
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0037c
			 0x7fc00001,                                                  // signaling NaN                               /// 00380
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00384
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0038c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00390
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00394
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00398
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0039c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x7f800000,                                                  // inf                                         /// 003bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003c0
			 0x00000000,                                                  // zero                                        /// 003c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 003d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003d8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00400
			 0x4b000000,                                                  // 8388608.0                                   /// 00404
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0040c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00410
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00418
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0041c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00420
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00424
			 0x0c400000,                                                  // Leading 1s:                                 /// 00428
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0042c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00430
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00438
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0043c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00440
			 0x00011111,                                                  // 9.7958E-41                                  /// 00444
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00448
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00450
			 0xbf028f5c,                                                  // -0.51                                       /// 00454
			 0x80000000,                                                  // -zero                                       /// 00458
			 0x33333333,                                                  // 4 random values                             /// 0045c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00460
			 0x33333333,                                                  // 4 random values                             /// 00464
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00468
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0046c
			 0xffc00001,                                                  // -signaling NaN                              /// 00470
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00474
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00478
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00480
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0048c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00490
			 0x0e000001,                                                  // Trailing 1s:                                /// 00494
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00498
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0049c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004dc
			 0x7fc00001,                                                  // signaling NaN                               /// 004e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00504
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0050c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00510
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00514
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00518
			 0x0c600000,                                                  // Leading 1s:                                 /// 0051c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00520
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00528
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0052c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00538
			 0x0c700000,                                                  // Leading 1s:                                 /// 0053c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00000000,                                                  // zero                                        /// 00548
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0054c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00550
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00558
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0055c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00564
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0056c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00574
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00578
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0057c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00588
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00590
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00594
			 0x55555555,                                                  // 4 random values                             /// 00598
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0059c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 005e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x55555555,                                                  // 4 random values                             /// 005ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005f8
			 0x33333333,                                                  // 4 random values                             /// 005fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00614
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00618
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00620
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00628
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0062c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00630
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00634
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0063c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00644
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0064c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00650
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0xcb000000,                                                  // -8388608.0                                  /// 00658
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0066c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00670
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00674
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00678
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0067c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00684
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00688
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0068c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00690
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00694
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00698
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0069c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00708
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00710
			 0x7fc00001,                                                  // signaling NaN                               /// 00714
			 0xffc00001,                                                  // -signaling NaN                              /// 00718
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0071c
			 0x00000000,                                                  // zero                                        /// 00720
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00724
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0072c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00730
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00734
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0073c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00740
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00744
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00748
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0074c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00750
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00754
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00760
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00764
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00768
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00770
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0077c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00784
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00788
			 0x80011111,                                                  // -9.7958E-41                                 /// 0078c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00790
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00794
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0xffc00001,                                                  // -signaling NaN                              /// 0079c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00800
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00804
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00808
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0080c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00810
			 0xcb000000,                                                  // -8388608.0                                  /// 00814
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00818
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00824
			 0x55555555,                                                  // 4 random values                             /// 00828
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0082c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00830
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00834
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00838
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00844
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x80011111,                                                  // -9.7958E-41                                 /// 0084c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00850
			 0x33333333,                                                  // 4 random values                             /// 00854
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00858
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0085c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00860
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00868
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00870
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00874
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0087c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00880
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00884
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0088c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00890
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00894
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00898
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0089c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 008ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00900
			 0x3f028f5c,                                                  // 0.51                                        /// 00904
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00908
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0090c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00910
			 0x55555555,                                                  // 4 random values                             /// 00914
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0091c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00920
			 0xff800000,                                                  // -inf                                        /// 00924
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00928
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0092c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00934
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0093c
			 0x3f028f5c,                                                  // 0.51                                        /// 00940
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00944
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0094c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00950
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00958
			 0x7fc00001,                                                  // signaling NaN                               /// 0095c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00960
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00968
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0096c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00974
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0097c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00980
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00984
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00988
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00990
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00998
			 0xff800000,                                                  // -inf                                        /// 0099c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a28
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a30
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a48
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a5c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a74
			 0x7fc00001,                                                  // signaling NaN                               /// 00a78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a90
			 0xbf028f5c,                                                  // -0.51                                       /// 00a94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00aa0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ab0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ab4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ab8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00abc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ad0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ad8
			 0x3f028f5c,                                                  // 0.51                                        /// 00adc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ae0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0xcb000000,                                                  // -8388608.0                                  /// 00aec
			 0x4b000000,                                                  // 8388608.0                                   /// 00af0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00af4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00af8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00afc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b18
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b48
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b4c
			 0x55555555,                                                  // 4 random values                             /// 00b50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b58
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b78
			 0xcb000000,                                                  // -8388608.0                                  /// 00b7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b80
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b90
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b98
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ba4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bc0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bd0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00be4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00be8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bf4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bf8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x7f800000,                                                  // inf                                         /// 00c1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c38
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c40
			 0x3f028f5c,                                                  // 0.51                                        /// 00c44
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c5c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c6c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80000000,                                                  // -zero                                       /// 00c94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ca0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ca4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cbc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cc4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cc8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cd0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cd8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ce8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cf0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cfc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d14
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d18
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d24
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d28
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d48
			 0x7fc00001,                                                  // signaling NaN                               /// 00d4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d60
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d64
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d84
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d90
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00db4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dc0
			 0x55555555,                                                  // 4 random values                             /// 00dc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dd0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x7f800000,                                                  // inf                                         /// 00dd8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ddc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00de0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00de4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00de8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00df8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dfc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e00
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x80000000,                                                  // -zero                                       /// 00e0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e14
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e20
			 0x00000000,                                                  // zero                                        /// 00e24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e38
			 0x4b000000,                                                  // 8388608.0                                   /// 00e3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e88
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ea4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0xffc00001,                                                  // -signaling NaN                              /// 00eb4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ec0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ec8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ed0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00edc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x7f800000,                                                  // inf                                         /// 00ee4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f24
			 0x7f800000,                                                  // inf                                         /// 00f28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f30
			 0x00000000,                                                  // zero                                        /// 00f34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f54
			 0xcb000000,                                                  // -8388608.0                                  /// 00f58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f64
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f68
			 0x3f028f5c,                                                  // 0.51                                        /// 00f6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x55555555,                                                  // 4 random values                             /// 00f7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fa4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fa8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fb8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fbc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fc0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fc4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fc8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fcc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ff0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ff8
			 0xAAAAAAAA                                                  // 4 random values                             /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00000
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00004
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0000c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00010
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00018
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0001c
			 0xcb000000,                                                  // -8388608.0                                  /// 00020
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0002c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00034
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00038
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0003c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00040
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00044
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0004c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00050
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00054
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00058
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0005c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00060
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00064
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00068
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0006c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00070
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00074
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00078
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0007c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00088
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0008c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00090
			 0x80011111,                                                  // -9.7958E-41                                 /// 00094
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00098
			 0x0c700000,                                                  // Leading 1s:                                 /// 0009c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00104
			 0x80011111,                                                  // -9.7958E-41                                 /// 00108
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0xbf028f5c,                                                  // -0.51                                       /// 00110
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00114
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00124
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00128
			 0x00000000,                                                  // zero                                        /// 0012c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00130
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00134
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00138
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00140
			 0x0c400000,                                                  // Leading 1s:                                 /// 00144
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0014c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00150
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00158
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0015c
			 0xffc00001,                                                  // -signaling NaN                              /// 00160
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00164
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0016c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00174
			 0x7fc00001,                                                  // signaling NaN                               /// 00178
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0017c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00184
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00190
			 0x3f028f5c,                                                  // 0.51                                        /// 00194
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00198
			 0x0e000003,                                                  // Trailing 1s:                                /// 0019c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001b0
			 0x00000000,                                                  // zero                                        /// 001b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001f4
			 0x3f028f5c,                                                  // 0.51                                        /// 001f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00200
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00204
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00208
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00214
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0021c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00220
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00224
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00228
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0022c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00230
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00234
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00244
			 0x0c600000,                                                  // Leading 1s:                                 /// 00248
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0024c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00250
			 0x0c600000,                                                  // Leading 1s:                                 /// 00254
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0025c
			 0x3f028f5c,                                                  // 0.51                                        /// 00260
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00264
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00270
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00274
			 0xff800000,                                                  // -inf                                        /// 00278
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0027c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00280
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00288
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0028c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00294
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00298
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0029c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002a0
			 0xbf028f5c,                                                  // -0.51                                       /// 002a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002c0
			 0x80000000,                                                  // -zero                                       /// 002c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 002e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x00000000,                                                  // zero                                        /// 002f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x7f800000,                                                  // inf                                         /// 002fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00300
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00308
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00314
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00318
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0031c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00320
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00328
			 0x80000000,                                                  // -zero                                       /// 0032c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00330
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00334
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00338
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0033c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00340
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00358
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0035c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00360
			 0x0c780000,                                                  // Leading 1s:                                 /// 00364
			 0x80000000,                                                  // -zero                                       /// 00368
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0036c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00378
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0037c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00384
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00388
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0038c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00390
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0039c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a8
			 0x00000000,                                                  // zero                                        /// 003ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00408
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00414
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00418
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0041c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00420
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x55555555,                                                  // 4 random values                             /// 00428
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00430
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00434
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00438
			 0xcb000000,                                                  // -8388608.0                                  /// 0043c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00440
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00444
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00448
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0044c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00454
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e000003,                                                  // Trailing 1s:                                /// 0045c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00464
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00468
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00474
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0xff800000,                                                  // -inf                                        /// 00480
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0xbf028f5c,                                                  // -0.51                                       /// 00490
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00494
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00498
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0049c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 004d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 004ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00500
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00504
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00508
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0050c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00514
			 0xff800000,                                                  // -inf                                        /// 00518
			 0x4b000000,                                                  // 8388608.0                                   /// 0051c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00520
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00528
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0052c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0xbf028f5c,                                                  // -0.51                                       /// 00538
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0053c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0c700000,                                                  // Leading 1s:                                 /// 00544
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00548
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0054c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00550
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00554
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0055c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00560
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00564
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00568
			 0xbf028f5c,                                                  // -0.51                                       /// 0056c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00578
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0057c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00580
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00584
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00588
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0058c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00590
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00594
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0059c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00604
			 0x33333333,                                                  // 4 random values                             /// 00608
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0060c
			 0xcb000000,                                                  // -8388608.0                                  /// 00610
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00614
			 0xff800000,                                                  // -inf                                        /// 00618
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0061c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00620
			 0x00000000,                                                  // zero                                        /// 00624
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00628
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0062c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00630
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00638
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0063c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00644
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00648
			 0x33333333,                                                  // 4 random values                             /// 0064c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00650
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00654
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0e000007,                                                  // Trailing 1s:                                /// 0065c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00664
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00668
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0066c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00670
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00674
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00678
			 0xff800000,                                                  // -inf                                        /// 0067c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00680
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00684
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00688
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0068c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00690
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x7f800000,                                                  // inf                                         /// 0069c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006b8
			 0xff800000,                                                  // -inf                                        /// 006bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c4
			 0xff800000,                                                  // -inf                                        /// 006c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00700
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00704
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00708
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0071c
			 0xffc00001,                                                  // -signaling NaN                              /// 00720
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00724
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0072c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00730
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00734
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x0c600000,                                                  // Leading 1s:                                 /// 0073c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00740
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00744
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0xffc00001,                                                  // -signaling NaN                              /// 0074c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00754
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0075c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x4b000000,                                                  // 8388608.0                                   /// 00768
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0076c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00770
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00774
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0077c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00780
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00788
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0078c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00790
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00794
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00798
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0079c
			 0x0e000007,                                                  // Trailing 1s:                                /// 007a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 007f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00804
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0080c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00814
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00818
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0081c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00820
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00828
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00830
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00834
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0083c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00844
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00848
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0084c
			 0x55555555,                                                  // 4 random values                             /// 00850
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00858
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00864
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00868
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0086c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00878
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00884
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00888
			 0x80000000,                                                  // -zero                                       /// 0088c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00890
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00894
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00898
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0089c
			 0x33333333,                                                  // 4 random values                             /// 008a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008c0
			 0x33333333,                                                  // 4 random values                             /// 008c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x7fc00001,                                                  // signaling NaN                               /// 008e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008f0
			 0x80000000,                                                  // -zero                                       /// 008f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00904
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00908
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00910
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00914
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00918
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00924
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00930
			 0x80011111,                                                  // -9.7958E-41                                 /// 00934
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0093c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00940
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00948
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00950
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00954
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00958
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0095c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00960
			 0x80011111,                                                  // -9.7958E-41                                 /// 00964
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00968
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00974
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0xff800000,                                                  // -inf                                        /// 0097c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00980
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0098c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00998
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0099c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009a0
			 0x3f028f5c,                                                  // 0.51                                        /// 009a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009b8
			 0xbf028f5c,                                                  // -0.51                                       /// 009bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009ec
			 0x33333333,                                                  // 4 random values                             /// 009f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a08
			 0x7f800000,                                                  // inf                                         /// 00a0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a1c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a54
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a64
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a94
			 0xbf028f5c,                                                  // -0.51                                       /// 00a98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00aa4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ab4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ab8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00abc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ac0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ac4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ac8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00acc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ad0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ad4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00adc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ae4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae8
			 0x4b000000,                                                  // 8388608.0                                   /// 00aec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00af0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00afc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b0c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b24
			 0x55555555,                                                  // 4 random values                             /// 00b28
			 0x7fc00001,                                                  // signaling NaN                               /// 00b2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b3c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b70
			 0xffc00001,                                                  // -signaling NaN                              /// 00b74
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b80
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b90
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ba4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bb4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bc0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bc4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bcc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bd4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bf4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c2c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x55555555,                                                  // 4 random values                             /// 00c50
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c6c
			 0x80000000,                                                  // -zero                                       /// 00c70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c80
			 0x7f800000,                                                  // inf                                         /// 00c84
			 0x4b000000,                                                  // 8388608.0                                   /// 00c88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cbc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cc0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cc4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ccc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x7f800000,                                                  // inf                                         /// 00ce4
			 0x00000000,                                                  // zero                                        /// 00ce8
			 0x55555555,                                                  // 4 random values                             /// 00cec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cf0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cf4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cfc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d24
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d30
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d34
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d44
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d48
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d58
			 0x00000000,                                                  // zero                                        /// 00d5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d70
			 0x00000000,                                                  // zero                                        /// 00d74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x7f800000,                                                  // inf                                         /// 00d84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d88
			 0xcb000000,                                                  // -8388608.0                                  /// 00d8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00da0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00da8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00db0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x55555555,                                                  // 4 random values                             /// 00db8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dc0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dd8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0xbf028f5c,                                                  // -0.51                                       /// 00de8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00df0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00df4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dfc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e38
			 0x33333333,                                                  // 4 random values                             /// 00e3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e40
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e44
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e50
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e68
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e6c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e74
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e7c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e94
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ea4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eb0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eb4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ebc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ec0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ecc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ed4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ed8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00edc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ee0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ee4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ee8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00eec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f18
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f24
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f50
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x33333333,                                                  // 4 random values                             /// 00f70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f80
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f98
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fa4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fcc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fd0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fd4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fe0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fe4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fe8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x807ffffe // max subnorm - 1ulp                            // SP - ve numbers                             /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x1b314ec1, /// 0x0
			 0x24fae5e5, /// 0x4
			 0x411f2a0e, /// 0x8
			 0xadba775c, /// 0xc
			 0xd1202336, /// 0x10
			 0x7cd23c09, /// 0x14
			 0xf008efa7, /// 0x18
			 0xc86f85f2, /// 0x1c
			 0x04ede20f, /// 0x20
			 0x3f7634d6, /// 0x24
			 0x94abc1d7, /// 0x28
			 0x50e316fc, /// 0x2c
			 0xd95a1f6d, /// 0x30
			 0x464804e0, /// 0x34
			 0x81379705, /// 0x38
			 0x6cf3d6ba, /// 0x3c
			 0x6696d2e8, /// 0x40
			 0x486fbce9, /// 0x44
			 0xee589afd, /// 0x48
			 0x6d91bb7a, /// 0x4c
			 0x0be05ed1, /// 0x50
			 0xe9233529, /// 0x54
			 0x535be963, /// 0x58
			 0x8178994e, /// 0x5c
			 0xc61a92f3, /// 0x60
			 0x15952c25, /// 0x64
			 0x339de5cf, /// 0x68
			 0xba22c619, /// 0x6c
			 0x0fdee543, /// 0x70
			 0xa22dc053, /// 0x74
			 0xac15bd9e, /// 0x78
			 0x9543f550, /// 0x7c
			 0xe49444da, /// 0x80
			 0x06f9ffb0, /// 0x84
			 0x14117bf5, /// 0x88
			 0xfb239902, /// 0x8c
			 0xb15b7acf, /// 0x90
			 0x26c274fc, /// 0x94
			 0xff726c9a, /// 0x98
			 0xb3369b33, /// 0x9c
			 0x8a3c3a65, /// 0xa0
			 0xf29d34fb, /// 0xa4
			 0xaa1b3388, /// 0xa8
			 0xcc90c591, /// 0xac
			 0x86533bf3, /// 0xb0
			 0x4689b6df, /// 0xb4
			 0xc3c5c07a, /// 0xb8
			 0xa64116bb, /// 0xbc
			 0x0ead8761, /// 0xc0
			 0x81f00b5c, /// 0xc4
			 0xc38fd2e7, /// 0xc8
			 0x0b1419a3, /// 0xcc
			 0xb51dca8a, /// 0xd0
			 0x41f65a08, /// 0xd4
			 0xa76d8b14, /// 0xd8
			 0x55269f5b, /// 0xdc
			 0xdcd5f6d5, /// 0xe0
			 0xd15131ef, /// 0xe4
			 0x1c2f4f79, /// 0xe8
			 0xe608c291, /// 0xec
			 0x00f8e9be, /// 0xf0
			 0x874a5367, /// 0xf4
			 0x44960fc2, /// 0xf8
			 0x6db7d998, /// 0xfc
			 0x1bfe5698, /// 0x100
			 0xacb206aa, /// 0x104
			 0x69f692b0, /// 0x108
			 0x9301902b, /// 0x10c
			 0x17de2298, /// 0x110
			 0x79c313f1, /// 0x114
			 0x3a7c3b2d, /// 0x118
			 0x3d26053a, /// 0x11c
			 0x01d42d92, /// 0x120
			 0x97b198fc, /// 0x124
			 0xe2990803, /// 0x128
			 0x3aed3882, /// 0x12c
			 0x3868c8c4, /// 0x130
			 0xce6a2269, /// 0x134
			 0x9eb88e35, /// 0x138
			 0xcb2bcecc, /// 0x13c
			 0x229f9eb7, /// 0x140
			 0x70289c2f, /// 0x144
			 0x73588af5, /// 0x148
			 0x326efcaa, /// 0x14c
			 0xc878d77b, /// 0x150
			 0x1a372490, /// 0x154
			 0x9cf83106, /// 0x158
			 0xe05bae74, /// 0x15c
			 0x70bf1759, /// 0x160
			 0xaef5c68a, /// 0x164
			 0xb835fe1f, /// 0x168
			 0xb88adcb2, /// 0x16c
			 0xbbb6cc96, /// 0x170
			 0x003a30ef, /// 0x174
			 0xab36ac5b, /// 0x178
			 0xc11390ff, /// 0x17c
			 0x413b632c, /// 0x180
			 0x583755e1, /// 0x184
			 0x1d256db2, /// 0x188
			 0xf78c3c05, /// 0x18c
			 0xfff21dc6, /// 0x190
			 0x23762e83, /// 0x194
			 0x7f647399, /// 0x198
			 0xa6db2290, /// 0x19c
			 0xba7af203, /// 0x1a0
			 0xa9910a82, /// 0x1a4
			 0x62d3b831, /// 0x1a8
			 0x1a3d7d5f, /// 0x1ac
			 0x04b98584, /// 0x1b0
			 0x6bd859fa, /// 0x1b4
			 0x444d4d0e, /// 0x1b8
			 0x7135a748, /// 0x1bc
			 0x54436afd, /// 0x1c0
			 0xc15685ba, /// 0x1c4
			 0x91f83970, /// 0x1c8
			 0x89ef66ec, /// 0x1cc
			 0x9b4bc7e8, /// 0x1d0
			 0x751f2c1f, /// 0x1d4
			 0xa1d0e1a2, /// 0x1d8
			 0xa62c5f65, /// 0x1dc
			 0x04c64a52, /// 0x1e0
			 0x002794a0, /// 0x1e4
			 0xfbc1cec5, /// 0x1e8
			 0xf440ef16, /// 0x1ec
			 0xbba5cc4d, /// 0x1f0
			 0x51798a0a, /// 0x1f4
			 0xbaaf12b2, /// 0x1f8
			 0xd663b92d, /// 0x1fc
			 0x6d62be89, /// 0x200
			 0xbe42365e, /// 0x204
			 0x3a161fb8, /// 0x208
			 0xcc927ac0, /// 0x20c
			 0xdb2191ec, /// 0x210
			 0xe7799e09, /// 0x214
			 0xed907294, /// 0x218
			 0xa7107215, /// 0x21c
			 0x49c2a773, /// 0x220
			 0x2f49a303, /// 0x224
			 0x6c8226e5, /// 0x228
			 0xa01c4723, /// 0x22c
			 0xd2a6dc2c, /// 0x230
			 0x9c5c2bf6, /// 0x234
			 0x439aeaa0, /// 0x238
			 0x00c178cf, /// 0x23c
			 0x2ecce60f, /// 0x240
			 0x1b8cf941, /// 0x244
			 0x8cc2810e, /// 0x248
			 0xc74914fc, /// 0x24c
			 0xb31a929a, /// 0x250
			 0x308565fb, /// 0x254
			 0x2d8974e9, /// 0x258
			 0x50a80338, /// 0x25c
			 0x33e3d533, /// 0x260
			 0xf524830f, /// 0x264
			 0x6418f865, /// 0x268
			 0x6770a674, /// 0x26c
			 0x6e09a15b, /// 0x270
			 0x99330988, /// 0x274
			 0xf26b9942, /// 0x278
			 0x1b4a75ab, /// 0x27c
			 0x48bcd2a0, /// 0x280
			 0xa72d92f3, /// 0x284
			 0x3f7b39a7, /// 0x288
			 0xc61bc141, /// 0x28c
			 0xbc06aedb, /// 0x290
			 0xcb22a014, /// 0x294
			 0x4a4e0728, /// 0x298
			 0x31ac1567, /// 0x29c
			 0xaa2b5229, /// 0x2a0
			 0x0410a55d, /// 0x2a4
			 0x5bbad2c1, /// 0x2a8
			 0x39c4bfd2, /// 0x2ac
			 0x4294a8e7, /// 0x2b0
			 0x08e10814, /// 0x2b4
			 0x8566cefd, /// 0x2b8
			 0x794f07c7, /// 0x2bc
			 0x1e2751ed, /// 0x2c0
			 0x57d1de37, /// 0x2c4
			 0x20b5bd6c, /// 0x2c8
			 0x9fcba74e, /// 0x2cc
			 0xe0da0151, /// 0x2d0
			 0xafad94e2, /// 0x2d4
			 0x8589e078, /// 0x2d8
			 0xad3df5b1, /// 0x2dc
			 0x4898e56c, /// 0x2e0
			 0xbeb79c04, /// 0x2e4
			 0x05014175, /// 0x2e8
			 0xf8ff7022, /// 0x2ec
			 0x26e861f3, /// 0x2f0
			 0x9330ee8d, /// 0x2f4
			 0x995d5c57, /// 0x2f8
			 0x7d699208, /// 0x2fc
			 0x7abffc7b, /// 0x300
			 0x66b52e8c, /// 0x304
			 0x592ef72c, /// 0x308
			 0x8e0517f0, /// 0x30c
			 0xa4d819a5, /// 0x310
			 0xc2f304b8, /// 0x314
			 0x065c6e55, /// 0x318
			 0xf33ebe94, /// 0x31c
			 0x84657240, /// 0x320
			 0x9c9c3841, /// 0x324
			 0xe7211358, /// 0x328
			 0xec0a126d, /// 0x32c
			 0x87893153, /// 0x330
			 0xbcf622b9, /// 0x334
			 0x96fd04c1, /// 0x338
			 0x4fccc3b1, /// 0x33c
			 0x91487001, /// 0x340
			 0x8ccfd6af, /// 0x344
			 0x497dc9db, /// 0x348
			 0x45891170, /// 0x34c
			 0x5d23fb1d, /// 0x350
			 0x8253f025, /// 0x354
			 0x622af6ca, /// 0x358
			 0x78852ab2, /// 0x35c
			 0x60c796ee, /// 0x360
			 0x764c4694, /// 0x364
			 0xe0641d53, /// 0x368
			 0x758dd752, /// 0x36c
			 0xd83d49be, /// 0x370
			 0x23ab3c40, /// 0x374
			 0x47569bee, /// 0x378
			 0xdf08b1cc, /// 0x37c
			 0x5558319b, /// 0x380
			 0xf24ea1e0, /// 0x384
			 0x91fadc24, /// 0x388
			 0xa90624bc, /// 0x38c
			 0x3f6f9cc6, /// 0x390
			 0xcdb900a4, /// 0x394
			 0x6fa7a487, /// 0x398
			 0xfe86f0e9, /// 0x39c
			 0x7ec8fc18, /// 0x3a0
			 0xc2ed5390, /// 0x3a4
			 0xb7114400, /// 0x3a8
			 0x4121d886, /// 0x3ac
			 0x4def50f8, /// 0x3b0
			 0xc0e15519, /// 0x3b4
			 0x9c8bdc1f, /// 0x3b8
			 0x024ed696, /// 0x3bc
			 0xb073e76d, /// 0x3c0
			 0xafbcc2b6, /// 0x3c4
			 0x9b4c0efc, /// 0x3c8
			 0x33cbf5c3, /// 0x3cc
			 0xc96a6c20, /// 0x3d0
			 0x34dfbfab, /// 0x3d4
			 0x0ab1434d, /// 0x3d8
			 0x4aa17143, /// 0x3dc
			 0xd594f47a, /// 0x3e0
			 0x9e96ee0c, /// 0x3e4
			 0x20dec7ec, /// 0x3e8
			 0x80173f36, /// 0x3ec
			 0x41c6f2b2, /// 0x3f0
			 0x7e797e73, /// 0x3f4
			 0x0e2d391a, /// 0x3f8
			 0x8ef62171, /// 0x3fc
			 0xd89651d3, /// 0x400
			 0x9ec22638, /// 0x404
			 0x8c2bf4d7, /// 0x408
			 0x790106f9, /// 0x40c
			 0x56dce5e7, /// 0x410
			 0xadf77f9e, /// 0x414
			 0x563cf1b3, /// 0x418
			 0x32def3bd, /// 0x41c
			 0x19845d4e, /// 0x420
			 0xfaeb15d7, /// 0x424
			 0x4c4ad5db, /// 0x428
			 0x553a7eaa, /// 0x42c
			 0x88948d4b, /// 0x430
			 0x905391f6, /// 0x434
			 0xb11c0c14, /// 0x438
			 0x4c76e83c, /// 0x43c
			 0x61037fce, /// 0x440
			 0xb9e6d105, /// 0x444
			 0x8c3419e4, /// 0x448
			 0xd103add8, /// 0x44c
			 0x9647c3f3, /// 0x450
			 0x76a47ec6, /// 0x454
			 0x3a9aa763, /// 0x458
			 0xc7236129, /// 0x45c
			 0x7c6a1d62, /// 0x460
			 0x634ec48e, /// 0x464
			 0x471519a1, /// 0x468
			 0xf037f995, /// 0x46c
			 0x990a391c, /// 0x470
			 0xff8b3500, /// 0x474
			 0x9e4ea6ba, /// 0x478
			 0x3b26b1f5, /// 0x47c
			 0x938b8695, /// 0x480
			 0x6718348d, /// 0x484
			 0x33b33e92, /// 0x488
			 0xd476fd90, /// 0x48c
			 0x0b284575, /// 0x490
			 0xa9fc3f17, /// 0x494
			 0x60939539, /// 0x498
			 0x9e33abf6, /// 0x49c
			 0x941f894e, /// 0x4a0
			 0xdbae7044, /// 0x4a4
			 0xf4fbdb00, /// 0x4a8
			 0x7ac004f2, /// 0x4ac
			 0x7f51b42d, /// 0x4b0
			 0xce931825, /// 0x4b4
			 0xa55ac58e, /// 0x4b8
			 0x7f6c17cf, /// 0x4bc
			 0x73c4763f, /// 0x4c0
			 0x7c7ef13f, /// 0x4c4
			 0x7b89c19b, /// 0x4c8
			 0xf32a8ea1, /// 0x4cc
			 0x7eb510e7, /// 0x4d0
			 0x9f272116, /// 0x4d4
			 0xfc03537b, /// 0x4d8
			 0xeda3a6af, /// 0x4dc
			 0x6a6a50e9, /// 0x4e0
			 0xb31f0d8d, /// 0x4e4
			 0xca3909fc, /// 0x4e8
			 0xff02a6a8, /// 0x4ec
			 0xe8390ed5, /// 0x4f0
			 0x987c8ef2, /// 0x4f4
			 0xcbf8aeb8, /// 0x4f8
			 0x810972f6, /// 0x4fc
			 0x2f1cc244, /// 0x500
			 0xd7695808, /// 0x504
			 0x6682a065, /// 0x508
			 0x2720e2ed, /// 0x50c
			 0xd02261ee, /// 0x510
			 0x673e786f, /// 0x514
			 0x2b869c49, /// 0x518
			 0x645de857, /// 0x51c
			 0x0d442380, /// 0x520
			 0x39684eb3, /// 0x524
			 0xbec98a4b, /// 0x528
			 0x47151006, /// 0x52c
			 0xddd75216, /// 0x530
			 0x00736fdd, /// 0x534
			 0x1bc9e409, /// 0x538
			 0x7dc69941, /// 0x53c
			 0x3ea8eb1b, /// 0x540
			 0x72bb7178, /// 0x544
			 0x9a503b2b, /// 0x548
			 0x5090effa, /// 0x54c
			 0x537c58dc, /// 0x550
			 0xeabc5587, /// 0x554
			 0x6524fa35, /// 0x558
			 0xb5f7df53, /// 0x55c
			 0xf6dd6a36, /// 0x560
			 0xb7199165, /// 0x564
			 0x6130d084, /// 0x568
			 0x87ded9f1, /// 0x56c
			 0x1cbad451, /// 0x570
			 0xb5620d6d, /// 0x574
			 0x4637645f, /// 0x578
			 0xb7b77321, /// 0x57c
			 0x6030feb1, /// 0x580
			 0xf42617c2, /// 0x584
			 0x1930f6ac, /// 0x588
			 0x677f88ec, /// 0x58c
			 0x731f2817, /// 0x590
			 0x2e5ee1f8, /// 0x594
			 0xeb5aa258, /// 0x598
			 0x556d5dc9, /// 0x59c
			 0xbe82e72f, /// 0x5a0
			 0xdf639008, /// 0x5a4
			 0x7b332165, /// 0x5a8
			 0xd760439c, /// 0x5ac
			 0x939a6207, /// 0x5b0
			 0xdeb70078, /// 0x5b4
			 0xa508e275, /// 0x5b8
			 0x398a9c95, /// 0x5bc
			 0x28a67ddc, /// 0x5c0
			 0xb6766a72, /// 0x5c4
			 0x3a48ce28, /// 0x5c8
			 0xfde0dbf9, /// 0x5cc
			 0x834582ad, /// 0x5d0
			 0x37271f49, /// 0x5d4
			 0x887211e0, /// 0x5d8
			 0x4667c2c3, /// 0x5dc
			 0x4a7562fd, /// 0x5e0
			 0x9bd61d6a, /// 0x5e4
			 0x45577250, /// 0x5e8
			 0x9fa09536, /// 0x5ec
			 0x1d70901f, /// 0x5f0
			 0x87f279cd, /// 0x5f4
			 0x68164b5f, /// 0x5f8
			 0x80402a38, /// 0x5fc
			 0x76e56dac, /// 0x600
			 0xe09ff5c2, /// 0x604
			 0x5a277a5a, /// 0x608
			 0x377e2276, /// 0x60c
			 0xb949acb6, /// 0x610
			 0xf66cfef6, /// 0x614
			 0xc2eeb653, /// 0x618
			 0x010dd514, /// 0x61c
			 0x01c64f2f, /// 0x620
			 0x869d5c64, /// 0x624
			 0x54b3baf9, /// 0x628
			 0x0e3e8f16, /// 0x62c
			 0xa7346ded, /// 0x630
			 0xea4e09c1, /// 0x634
			 0x1e7b213c, /// 0x638
			 0xeb072ff4, /// 0x63c
			 0x6a1089cf, /// 0x640
			 0x0be952a2, /// 0x644
			 0x8c2be571, /// 0x648
			 0xcb28fe6e, /// 0x64c
			 0x20fa3550, /// 0x650
			 0x97a3a743, /// 0x654
			 0xe4eb05a5, /// 0x658
			 0x9dfb164c, /// 0x65c
			 0x2ea3e5f3, /// 0x660
			 0x83f04cdc, /// 0x664
			 0xe9cf5f3f, /// 0x668
			 0x3ed8c6d9, /// 0x66c
			 0x3404a8d5, /// 0x670
			 0x9d9b4b9f, /// 0x674
			 0x4e19f4b6, /// 0x678
			 0x5c5e8503, /// 0x67c
			 0xa73d9c38, /// 0x680
			 0xf2420696, /// 0x684
			 0xcc3276f7, /// 0x688
			 0xfab3176b, /// 0x68c
			 0xe99aab11, /// 0x690
			 0x832d2deb, /// 0x694
			 0xe8062ad0, /// 0x698
			 0x835b77cc, /// 0x69c
			 0x47459fe6, /// 0x6a0
			 0xf0172e3f, /// 0x6a4
			 0x09648453, /// 0x6a8
			 0x7e63f539, /// 0x6ac
			 0xf149b603, /// 0x6b0
			 0xd86752ac, /// 0x6b4
			 0xcb1b0298, /// 0x6b8
			 0x53a52b0b, /// 0x6bc
			 0xc42496bd, /// 0x6c0
			 0xe325b554, /// 0x6c4
			 0xab278bfd, /// 0x6c8
			 0xe87bbcc7, /// 0x6cc
			 0x8d500a61, /// 0x6d0
			 0xb51d8bd0, /// 0x6d4
			 0x66cfd152, /// 0x6d8
			 0x91ce8812, /// 0x6dc
			 0xcd318893, /// 0x6e0
			 0x5cbea736, /// 0x6e4
			 0x1cf42ea8, /// 0x6e8
			 0x7784692e, /// 0x6ec
			 0x2839984e, /// 0x6f0
			 0x2bebeb87, /// 0x6f4
			 0x6693b32b, /// 0x6f8
			 0x79d242f4, /// 0x6fc
			 0x4de441ad, /// 0x700
			 0x4b324bb7, /// 0x704
			 0x18b7a702, /// 0x708
			 0x2f905b2c, /// 0x70c
			 0xb79109c1, /// 0x710
			 0xa1f85a55, /// 0x714
			 0x3fc04d91, /// 0x718
			 0xeda6c8a2, /// 0x71c
			 0x4eded989, /// 0x720
			 0xc1f5fb7a, /// 0x724
			 0x20c24bfe, /// 0x728
			 0x732fd119, /// 0x72c
			 0x832bdb46, /// 0x730
			 0xbcd0b67d, /// 0x734
			 0x6d9c1af5, /// 0x738
			 0x55c11ee4, /// 0x73c
			 0x927648ea, /// 0x740
			 0x35a79044, /// 0x744
			 0x0e7075e0, /// 0x748
			 0x1f468c64, /// 0x74c
			 0x29d6c492, /// 0x750
			 0x4d7caf2b, /// 0x754
			 0x40bde036, /// 0x758
			 0x9b73f133, /// 0x75c
			 0x274f836e, /// 0x760
			 0xf5e8f81a, /// 0x764
			 0xeaaddd21, /// 0x768
			 0xdc3c29f5, /// 0x76c
			 0x159f8af7, /// 0x770
			 0x16891a1c, /// 0x774
			 0x08fab1c3, /// 0x778
			 0x85660d56, /// 0x77c
			 0x607664fc, /// 0x780
			 0x1c9c1502, /// 0x784
			 0x63444371, /// 0x788
			 0x3fbd0a65, /// 0x78c
			 0xdb480d82, /// 0x790
			 0x520681a5, /// 0x794
			 0x4bdc89f4, /// 0x798
			 0x3410d7b8, /// 0x79c
			 0xc9b20374, /// 0x7a0
			 0x860ee588, /// 0x7a4
			 0x26a44355, /// 0x7a8
			 0x0b35104a, /// 0x7ac
			 0x1384c741, /// 0x7b0
			 0x2e7d2f93, /// 0x7b4
			 0x141f4fee, /// 0x7b8
			 0xc18086a6, /// 0x7bc
			 0xe81fa1bb, /// 0x7c0
			 0x04907efd, /// 0x7c4
			 0x5da794a0, /// 0x7c8
			 0xdbcc4daa, /// 0x7cc
			 0x6aaa144a, /// 0x7d0
			 0x622ea8b3, /// 0x7d4
			 0x36a8f0c5, /// 0x7d8
			 0xfb968a3f, /// 0x7dc
			 0xd08dd970, /// 0x7e0
			 0xd8d6cda7, /// 0x7e4
			 0xa77d1b16, /// 0x7e8
			 0x4b6421a9, /// 0x7ec
			 0x2891f93b, /// 0x7f0
			 0xf07080cd, /// 0x7f4
			 0x3ebc11ea, /// 0x7f8
			 0x2e7ec072, /// 0x7fc
			 0x29709b1a, /// 0x800
			 0xa4a5c9fe, /// 0x804
			 0x29fdc054, /// 0x808
			 0xa2fbd604, /// 0x80c
			 0x4be1ce61, /// 0x810
			 0xdb8b11cc, /// 0x814
			 0xa154b3b4, /// 0x818
			 0xb5d57149, /// 0x81c
			 0xb13254a2, /// 0x820
			 0x7e517899, /// 0x824
			 0x7d5f2792, /// 0x828
			 0xe387d4be, /// 0x82c
			 0x23a1772b, /// 0x830
			 0x73c63631, /// 0x834
			 0x3d133ebc, /// 0x838
			 0xb1afaf65, /// 0x83c
			 0xd4bad2fe, /// 0x840
			 0x38618685, /// 0x844
			 0x91139654, /// 0x848
			 0x7d1fda3b, /// 0x84c
			 0xf9c0ab81, /// 0x850
			 0x2af04a7f, /// 0x854
			 0x43551a06, /// 0x858
			 0xe1e622af, /// 0x85c
			 0x6e7f1d02, /// 0x860
			 0x5080c1cb, /// 0x864
			 0x8c408089, /// 0x868
			 0x6c7b4fec, /// 0x86c
			 0x8567d6e3, /// 0x870
			 0x87dfe195, /// 0x874
			 0xfba56664, /// 0x878
			 0xc20aed54, /// 0x87c
			 0x60858266, /// 0x880
			 0x93bf4181, /// 0x884
			 0x674b078f, /// 0x888
			 0x74b41984, /// 0x88c
			 0x3ea9c8fc, /// 0x890
			 0x618a5824, /// 0x894
			 0x73a1c60f, /// 0x898
			 0xe0d2f836, /// 0x89c
			 0xe401713c, /// 0x8a0
			 0x2b278c56, /// 0x8a4
			 0xc6b60698, /// 0x8a8
			 0xcb939920, /// 0x8ac
			 0x7eb8cfc9, /// 0x8b0
			 0x59abe3e6, /// 0x8b4
			 0x9e3ab06e, /// 0x8b8
			 0x338b61ad, /// 0x8bc
			 0xfb523ff8, /// 0x8c0
			 0x9672cd6e, /// 0x8c4
			 0xfe537b5e, /// 0x8c8
			 0xc7c05cfb, /// 0x8cc
			 0x305cded9, /// 0x8d0
			 0x8b977d30, /// 0x8d4
			 0x17b02e5c, /// 0x8d8
			 0x3f06dbc2, /// 0x8dc
			 0x714db951, /// 0x8e0
			 0x2378d4a5, /// 0x8e4
			 0xeabafc6f, /// 0x8e8
			 0x5d1bdce4, /// 0x8ec
			 0xc9bed538, /// 0x8f0
			 0xc57e38bf, /// 0x8f4
			 0x4df1ee71, /// 0x8f8
			 0x78809c0d, /// 0x8fc
			 0x7cba54e0, /// 0x900
			 0x0a53a390, /// 0x904
			 0x58971b66, /// 0x908
			 0x567ba084, /// 0x90c
			 0xbee6b9bd, /// 0x910
			 0xdc70dc05, /// 0x914
			 0x0d5c007f, /// 0x918
			 0x50965831, /// 0x91c
			 0x4ccd145c, /// 0x920
			 0x1409be25, /// 0x924
			 0xe7f2bff1, /// 0x928
			 0x8c1ac248, /// 0x92c
			 0xcf06c19a, /// 0x930
			 0x61b67732, /// 0x934
			 0x8edc6f9a, /// 0x938
			 0xb8b0de6d, /// 0x93c
			 0x40d78db1, /// 0x940
			 0x01c9ef48, /// 0x944
			 0x139a86db, /// 0x948
			 0x90064037, /// 0x94c
			 0x9ed2f997, /// 0x950
			 0x4bbba0cd, /// 0x954
			 0xf90fa634, /// 0x958
			 0x7de20533, /// 0x95c
			 0x7d44f185, /// 0x960
			 0xe9486502, /// 0x964
			 0xcf5254ff, /// 0x968
			 0x2d35dbff, /// 0x96c
			 0xdae9dbc4, /// 0x970
			 0x0be0f8a3, /// 0x974
			 0xe92e0be5, /// 0x978
			 0x9eca5c7d, /// 0x97c
			 0x6c65a659, /// 0x980
			 0x199e355e, /// 0x984
			 0x3a9acdc5, /// 0x988
			 0x72c3a955, /// 0x98c
			 0x365151d6, /// 0x990
			 0x8eb14e80, /// 0x994
			 0x36372712, /// 0x998
			 0x813b3a44, /// 0x99c
			 0x291ca080, /// 0x9a0
			 0x9a4165ab, /// 0x9a4
			 0xd1aa4b5e, /// 0x9a8
			 0xa1157322, /// 0x9ac
			 0xcc3d742b, /// 0x9b0
			 0xe1b9fb30, /// 0x9b4
			 0xdb8701ec, /// 0x9b8
			 0xe68444fb, /// 0x9bc
			 0x3630c2b5, /// 0x9c0
			 0x1c47b891, /// 0x9c4
			 0x2a248355, /// 0x9c8
			 0x4d7e3674, /// 0x9cc
			 0xb3b09559, /// 0x9d0
			 0x85476b7f, /// 0x9d4
			 0x1cd08220, /// 0x9d8
			 0xcf7b9cc7, /// 0x9dc
			 0x1fcf1442, /// 0x9e0
			 0x0ee1dd75, /// 0x9e4
			 0x2a55578c, /// 0x9e8
			 0x2203fc68, /// 0x9ec
			 0xaa207fd0, /// 0x9f0
			 0x584970cc, /// 0x9f4
			 0xd1618ea2, /// 0x9f8
			 0x30d3ffac, /// 0x9fc
			 0xd31520e2, /// 0xa00
			 0xe9977041, /// 0xa04
			 0xc3cfcfde, /// 0xa08
			 0xac260f76, /// 0xa0c
			 0x94a84d21, /// 0xa10
			 0x3f2bac3c, /// 0xa14
			 0x6a767694, /// 0xa18
			 0xdbd26910, /// 0xa1c
			 0xc7b394c3, /// 0xa20
			 0xe7d7a812, /// 0xa24
			 0x5b5d2b5c, /// 0xa28
			 0x423bbab5, /// 0xa2c
			 0x05277e3d, /// 0xa30
			 0x48ee1cc7, /// 0xa34
			 0x553599ef, /// 0xa38
			 0x8e55ad7b, /// 0xa3c
			 0xd2a12ff7, /// 0xa40
			 0x49ee02e6, /// 0xa44
			 0x57d108f2, /// 0xa48
			 0x3c797b20, /// 0xa4c
			 0xc6e1b7d3, /// 0xa50
			 0xb9423745, /// 0xa54
			 0x1e8880da, /// 0xa58
			 0x9f815230, /// 0xa5c
			 0x967ec14f, /// 0xa60
			 0xfd0293ff, /// 0xa64
			 0x544ac01b, /// 0xa68
			 0x3cb773e8, /// 0xa6c
			 0x0ab868bd, /// 0xa70
			 0x7a4954fd, /// 0xa74
			 0x8e741e4f, /// 0xa78
			 0x2f1b86c8, /// 0xa7c
			 0x3a7746fd, /// 0xa80
			 0xe34e1ff9, /// 0xa84
			 0x218eeece, /// 0xa88
			 0x768c51a3, /// 0xa8c
			 0x6854c5fd, /// 0xa90
			 0xddae7b87, /// 0xa94
			 0x9397919c, /// 0xa98
			 0xbb832109, /// 0xa9c
			 0x89b4da0f, /// 0xaa0
			 0xea9526bd, /// 0xaa4
			 0x6478927c, /// 0xaa8
			 0x0799a896, /// 0xaac
			 0xe46249b5, /// 0xab0
			 0x9845924c, /// 0xab4
			 0x69dae6eb, /// 0xab8
			 0x99c79dd7, /// 0xabc
			 0xc54e369c, /// 0xac0
			 0xed4be350, /// 0xac4
			 0x6ff2e151, /// 0xac8
			 0x94694741, /// 0xacc
			 0xf034114f, /// 0xad0
			 0x249e8438, /// 0xad4
			 0x4b9f5009, /// 0xad8
			 0x6fefb12a, /// 0xadc
			 0x001a1d0b, /// 0xae0
			 0xb84e6816, /// 0xae4
			 0x683cca4c, /// 0xae8
			 0x8394a678, /// 0xaec
			 0x6d214666, /// 0xaf0
			 0x1f8fb400, /// 0xaf4
			 0xc6380621, /// 0xaf8
			 0x591d98e1, /// 0xafc
			 0x5d4d0c0f, /// 0xb00
			 0x1b1d29a2, /// 0xb04
			 0x463aa055, /// 0xb08
			 0x86085f6b, /// 0xb0c
			 0x0ea6a295, /// 0xb10
			 0x829c2878, /// 0xb14
			 0xff74e9ce, /// 0xb18
			 0x89e16444, /// 0xb1c
			 0xcca42329, /// 0xb20
			 0x83e2efd0, /// 0xb24
			 0x9745bf79, /// 0xb28
			 0x1f8d5e85, /// 0xb2c
			 0x9c9fe92c, /// 0xb30
			 0xff4a84ea, /// 0xb34
			 0x4cdea8ca, /// 0xb38
			 0x3da6d5a3, /// 0xb3c
			 0xa4cc5244, /// 0xb40
			 0x99d3f7fe, /// 0xb44
			 0x8655a554, /// 0xb48
			 0x17269f22, /// 0xb4c
			 0x216f0793, /// 0xb50
			 0xc894c63f, /// 0xb54
			 0x2453fcbd, /// 0xb58
			 0xfac9e9c4, /// 0xb5c
			 0xa03d45c7, /// 0xb60
			 0x77991f27, /// 0xb64
			 0xa857c06a, /// 0xb68
			 0x27a48684, /// 0xb6c
			 0x24fc144d, /// 0xb70
			 0xcd1c3835, /// 0xb74
			 0xb699dfff, /// 0xb78
			 0xdc618315, /// 0xb7c
			 0x8531db2c, /// 0xb80
			 0x0f14dd98, /// 0xb84
			 0xd3edbd6c, /// 0xb88
			 0x4eb54c1c, /// 0xb8c
			 0x4e965875, /// 0xb90
			 0xf47917b2, /// 0xb94
			 0xf97d9b89, /// 0xb98
			 0xfb3d8b8a, /// 0xb9c
			 0x36b2eed6, /// 0xba0
			 0x7189de52, /// 0xba4
			 0x9b883a7e, /// 0xba8
			 0xa6511389, /// 0xbac
			 0x75dff1b3, /// 0xbb0
			 0x179fa883, /// 0xbb4
			 0x826d6802, /// 0xbb8
			 0x424b77cf, /// 0xbbc
			 0x6c7ff0b6, /// 0xbc0
			 0x491e60ae, /// 0xbc4
			 0xe8fee69e, /// 0xbc8
			 0xecbcee73, /// 0xbcc
			 0x7006fbbc, /// 0xbd0
			 0xa56cf20f, /// 0xbd4
			 0x8fbff06d, /// 0xbd8
			 0x6e734702, /// 0xbdc
			 0x723c32b1, /// 0xbe0
			 0xddace8ed, /// 0xbe4
			 0xbec9df48, /// 0xbe8
			 0x58a35f30, /// 0xbec
			 0xf2636ede, /// 0xbf0
			 0xe140505f, /// 0xbf4
			 0x25a56afb, /// 0xbf8
			 0xad617d52, /// 0xbfc
			 0x2af0e686, /// 0xc00
			 0x8d397315, /// 0xc04
			 0xd87e3e1c, /// 0xc08
			 0xd6dbb3e5, /// 0xc0c
			 0x957266e8, /// 0xc10
			 0xdef42dec, /// 0xc14
			 0x1b6bbc1d, /// 0xc18
			 0xb694747c, /// 0xc1c
			 0xfe15c2f1, /// 0xc20
			 0x89aeb5eb, /// 0xc24
			 0xeda3255f, /// 0xc28
			 0x10384663, /// 0xc2c
			 0x4b78fc0e, /// 0xc30
			 0xc8b6279f, /// 0xc34
			 0x53883697, /// 0xc38
			 0x3feaa60c, /// 0xc3c
			 0x747234ed, /// 0xc40
			 0x13021007, /// 0xc44
			 0x492597b0, /// 0xc48
			 0x48252492, /// 0xc4c
			 0x6578b025, /// 0xc50
			 0xa98990ad, /// 0xc54
			 0xf22eabf3, /// 0xc58
			 0x9afe24fd, /// 0xc5c
			 0x9a01156d, /// 0xc60
			 0x6672e993, /// 0xc64
			 0xe48f76a5, /// 0xc68
			 0x5d892db8, /// 0xc6c
			 0x7a65de5e, /// 0xc70
			 0x80aee638, /// 0xc74
			 0xe39e3d2e, /// 0xc78
			 0x91ecb489, /// 0xc7c
			 0x8be8d2db, /// 0xc80
			 0x420c0c26, /// 0xc84
			 0x028422de, /// 0xc88
			 0xb034afbb, /// 0xc8c
			 0xbb416bab, /// 0xc90
			 0xc24ab94e, /// 0xc94
			 0xb19226bb, /// 0xc98
			 0x6a0fb78b, /// 0xc9c
			 0x74b485c6, /// 0xca0
			 0x2fe38ff9, /// 0xca4
			 0xe50c3955, /// 0xca8
			 0xd332f55e, /// 0xcac
			 0x21a9ddee, /// 0xcb0
			 0x8b157645, /// 0xcb4
			 0x3b8ea707, /// 0xcb8
			 0xd86d0bf3, /// 0xcbc
			 0x7307601b, /// 0xcc0
			 0x0bef95d3, /// 0xcc4
			 0x83b2f25f, /// 0xcc8
			 0x42e93b5a, /// 0xccc
			 0xd20ddc30, /// 0xcd0
			 0x7fd52253, /// 0xcd4
			 0x695ba672, /// 0xcd8
			 0xd8329657, /// 0xcdc
			 0x28d5b02f, /// 0xce0
			 0x2d00ab6b, /// 0xce4
			 0x509e3947, /// 0xce8
			 0x8b580b98, /// 0xcec
			 0xf23fffaf, /// 0xcf0
			 0xa574924a, /// 0xcf4
			 0x32aab444, /// 0xcf8
			 0x34587b9c, /// 0xcfc
			 0xef35676a, /// 0xd00
			 0x46ee1c86, /// 0xd04
			 0xb2a9ed6a, /// 0xd08
			 0x883e436c, /// 0xd0c
			 0xc287fe77, /// 0xd10
			 0xed471d50, /// 0xd14
			 0x858ba14a, /// 0xd18
			 0xfee1b823, /// 0xd1c
			 0x91df10d9, /// 0xd20
			 0x8a2ed367, /// 0xd24
			 0x2aefc31a, /// 0xd28
			 0x1172da4c, /// 0xd2c
			 0xcd394fcf, /// 0xd30
			 0x95522e57, /// 0xd34
			 0x50a7cb9e, /// 0xd38
			 0xdbf15838, /// 0xd3c
			 0xeb7019a2, /// 0xd40
			 0x40f4916c, /// 0xd44
			 0x190d2b2d, /// 0xd48
			 0x073805b2, /// 0xd4c
			 0xeec070c8, /// 0xd50
			 0x9fdfc070, /// 0xd54
			 0x0760e2d3, /// 0xd58
			 0x4e89905d, /// 0xd5c
			 0xf6c80a9f, /// 0xd60
			 0xe5976e39, /// 0xd64
			 0xbc27636b, /// 0xd68
			 0x013842eb, /// 0xd6c
			 0xd42c4398, /// 0xd70
			 0x45a73287, /// 0xd74
			 0x0fd66667, /// 0xd78
			 0xed0bf6bf, /// 0xd7c
			 0x33955737, /// 0xd80
			 0x28a0e448, /// 0xd84
			 0x47b22bfd, /// 0xd88
			 0xd3ccc37d, /// 0xd8c
			 0xc854a248, /// 0xd90
			 0xaaa11ca9, /// 0xd94
			 0x8372c3a8, /// 0xd98
			 0x5ad76b4a, /// 0xd9c
			 0xae9e1fe2, /// 0xda0
			 0x03e73ed6, /// 0xda4
			 0x44674627, /// 0xda8
			 0xb8c228ea, /// 0xdac
			 0xb6bc661f, /// 0xdb0
			 0xee996054, /// 0xdb4
			 0x4713f111, /// 0xdb8
			 0xfb3e12de, /// 0xdbc
			 0xa91ff7b3, /// 0xdc0
			 0xb69704ac, /// 0xdc4
			 0x4d9d3ee3, /// 0xdc8
			 0x066cf989, /// 0xdcc
			 0x7ed877c8, /// 0xdd0
			 0x239411cb, /// 0xdd4
			 0x9063ef9f, /// 0xdd8
			 0xed30ab7e, /// 0xddc
			 0x160d3dbe, /// 0xde0
			 0x63303ea3, /// 0xde4
			 0x43d33c6e, /// 0xde8
			 0x5aced6f0, /// 0xdec
			 0xbcbf958d, /// 0xdf0
			 0x63eb54bb, /// 0xdf4
			 0x2b13c3a9, /// 0xdf8
			 0x9afa50ef, /// 0xdfc
			 0xaa2070b7, /// 0xe00
			 0x4c2fc8b6, /// 0xe04
			 0x934eed94, /// 0xe08
			 0x7ea3cdde, /// 0xe0c
			 0xf29361db, /// 0xe10
			 0xe0b9dc2c, /// 0xe14
			 0x353ddc93, /// 0xe18
			 0xd42e7993, /// 0xe1c
			 0xcdbcf1d8, /// 0xe20
			 0x729974b5, /// 0xe24
			 0xf07391bb, /// 0xe28
			 0x3fe9bd5f, /// 0xe2c
			 0x6e56af1c, /// 0xe30
			 0x7fb0b9a6, /// 0xe34
			 0xed69da30, /// 0xe38
			 0x3560eb36, /// 0xe3c
			 0xdb48c861, /// 0xe40
			 0x57906978, /// 0xe44
			 0xdda127a1, /// 0xe48
			 0xee547e83, /// 0xe4c
			 0x299c8e34, /// 0xe50
			 0xc99298b1, /// 0xe54
			 0x7bd80efd, /// 0xe58
			 0x635ffa79, /// 0xe5c
			 0x70a4c07e, /// 0xe60
			 0xf0a4ce75, /// 0xe64
			 0x2920a804, /// 0xe68
			 0x0c29e9fb, /// 0xe6c
			 0xa2f1aac7, /// 0xe70
			 0x4e8e92ae, /// 0xe74
			 0x87c211f5, /// 0xe78
			 0xf6972be8, /// 0xe7c
			 0x0084b0fd, /// 0xe80
			 0xbc22bab6, /// 0xe84
			 0x48cc37eb, /// 0xe88
			 0xce3e5dff, /// 0xe8c
			 0xfedbb2a0, /// 0xe90
			 0x5fdc691d, /// 0xe94
			 0x37977153, /// 0xe98
			 0xc176346d, /// 0xe9c
			 0xc0c86adc, /// 0xea0
			 0x89496dd3, /// 0xea4
			 0x2083c8a9, /// 0xea8
			 0xc0a796e0, /// 0xeac
			 0xa4c7a934, /// 0xeb0
			 0xe190d337, /// 0xeb4
			 0x98f15ed2, /// 0xeb8
			 0xa403dc7c, /// 0xebc
			 0xb242fa96, /// 0xec0
			 0x4407da04, /// 0xec4
			 0x5a20928f, /// 0xec8
			 0x9e269258, /// 0xecc
			 0xf4a6f071, /// 0xed0
			 0x11f3d080, /// 0xed4
			 0x099e2d77, /// 0xed8
			 0xde7fff7f, /// 0xedc
			 0x6093c533, /// 0xee0
			 0x1d442fc6, /// 0xee4
			 0xeb985d7f, /// 0xee8
			 0x7cfafa2a, /// 0xeec
			 0xc0e6905b, /// 0xef0
			 0xae8d8e06, /// 0xef4
			 0x762c8134, /// 0xef8
			 0x2fd1064e, /// 0xefc
			 0x57a5991c, /// 0xf00
			 0x280cda14, /// 0xf04
			 0x0d7c056a, /// 0xf08
			 0x52fcc064, /// 0xf0c
			 0x07278524, /// 0xf10
			 0xe4e8d487, /// 0xf14
			 0xc3cf7ed1, /// 0xf18
			 0x96205666, /// 0xf1c
			 0x76101831, /// 0xf20
			 0xe975e317, /// 0xf24
			 0x53f9c825, /// 0xf28
			 0xc343ac80, /// 0xf2c
			 0xceca9a6b, /// 0xf30
			 0x6b89a457, /// 0xf34
			 0x934f1225, /// 0xf38
			 0x2867eb5c, /// 0xf3c
			 0xe9104dd2, /// 0xf40
			 0xb17115f7, /// 0xf44
			 0xae5a8b70, /// 0xf48
			 0x118414b7, /// 0xf4c
			 0xbc83a6d8, /// 0xf50
			 0xe0055261, /// 0xf54
			 0x7019574b, /// 0xf58
			 0xc67cb1f0, /// 0xf5c
			 0x808d7138, /// 0xf60
			 0x686e42ac, /// 0xf64
			 0x551915d5, /// 0xf68
			 0xb83073ca, /// 0xf6c
			 0x6ac5f90f, /// 0xf70
			 0xe745573c, /// 0xf74
			 0x45028503, /// 0xf78
			 0x973cf64b, /// 0xf7c
			 0x08365505, /// 0xf80
			 0x9d9e40da, /// 0xf84
			 0xfa10d7a6, /// 0xf88
			 0xdcb811d2, /// 0xf8c
			 0xd1e4dd2d, /// 0xf90
			 0xd798883f, /// 0xf94
			 0xd6c42079, /// 0xf98
			 0xa0623231, /// 0xf9c
			 0xdde822a1, /// 0xfa0
			 0xd5a83ff3, /// 0xfa4
			 0xc8ab6749, /// 0xfa8
			 0xfa366128, /// 0xfac
			 0x1ac01ac4, /// 0xfb0
			 0x14caba6a, /// 0xfb4
			 0x7c502017, /// 0xfb8
			 0x467fea5a, /// 0xfbc
			 0x8459be83, /// 0xfc0
			 0x5d633989, /// 0xfc4
			 0x77f63484, /// 0xfc8
			 0x6b5d04bc, /// 0xfcc
			 0x78217edb, /// 0xfd0
			 0x9aefcc60, /// 0xfd4
			 0x29e96515, /// 0xfd8
			 0xc73460ad, /// 0xfdc
			 0xee25062e, /// 0xfe0
			 0xcb20babc, /// 0xfe4
			 0x2e205bbc, /// 0xfe8
			 0xa795f752, /// 0xfec
			 0x35a559f8, /// 0xff0
			 0xec632805, /// 0xff4
			 0x96bd8eff, /// 0xff8
			 0x9aaabd57 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0000c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00010
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00014
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00018
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0001c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00020
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00024
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00028
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0002c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0003c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0004c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0xbf028f5c,                                                  // -0.51                                       /// 00054
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00058
			 0x80011111,                                                  // -9.7958E-41                                 /// 0005c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00060
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0006c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00070
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00074
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0007c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00080
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00084
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00094
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00098
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00100
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0010c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0011c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00120
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00128
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00130
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0013c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00140
			 0x80011111,                                                  // -9.7958E-41                                 /// 00144
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00148
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0014c
			 0xff800000,                                                  // -inf                                        /// 00150
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00154
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x80000000,                                                  // -zero                                       /// 0015c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x33333333,                                                  // 4 random values                             /// 0016c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00170
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00178
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00180
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00188
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0018c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00190
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00194
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b8
			 0x33333333,                                                  // 4 random values                             /// 001bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0xff800000,                                                  // -inf                                        /// 001d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00200
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0020c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00210
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00214
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00218
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00220
			 0xff800000,                                                  // -inf                                        /// 00224
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0022c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00230
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00238
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0023c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00240
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00248
			 0x7fc00001,                                                  // signaling NaN                               /// 0024c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00254
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00258
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0025c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00264
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00268
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0026c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00270
			 0x0e000007,                                                  // Trailing 1s:                                /// 00274
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00278
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0027c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00280
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00288
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00294
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x7f800000,                                                  // inf                                         /// 002a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 002d0
			 0xbf028f5c,                                                  // -0.51                                       /// 002d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002e0
			 0xff800000,                                                  // -inf                                        /// 002e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00308
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0030c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00310
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00314
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x7fc00001,                                                  // signaling NaN                               /// 0031c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00330
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00334
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0033c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00340
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00348
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00350
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00354
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00358
			 0x0e000001,                                                  // Trailing 1s:                                /// 0035c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00360
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00364
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0036c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00370
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00374
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00380
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00388
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0038c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00390
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00394
			 0xffc00001,                                                  // -signaling NaN                              /// 00398
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0039c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x7fc00001,                                                  // signaling NaN                               /// 003ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 003e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00400
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00404
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00408
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0xcb000000,                                                  // -8388608.0                                  /// 00414
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00418
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0041c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x33333333,                                                  // 4 random values                             /// 00424
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0042c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00430
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x0c700000,                                                  // Leading 1s:                                 /// 0043c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xcb000000,                                                  // -8388608.0                                  /// 00448
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0044c
			 0x7f800000,                                                  // inf                                         /// 00450
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00454
			 0x7fc00001,                                                  // signaling NaN                               /// 00458
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00464
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0046c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0e000007,                                                  // Trailing 1s:                                /// 0047c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00490
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00494
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004f0
			 0x80000000,                                                  // -zero                                       /// 004f4
			 0xffc00001,                                                  // -signaling NaN                              /// 004f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00500
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0xcb000000,                                                  // -8388608.0                                  /// 00508
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0050c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00510
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00514
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00524
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00528
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00530
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00534
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0053c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00540
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00548
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0054c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0055c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00560
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00564
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00570
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00574
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0057c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00580
			 0x7f800000,                                                  // inf                                         /// 00584
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00588
			 0x0c400000,                                                  // Leading 1s:                                 /// 0058c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00590
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00598
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0059c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0xffc00001,                                                  // -signaling NaN                              /// 005b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x00000000,                                                  // zero                                        /// 005ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00600
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00604
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0060c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00610
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x7f800000,                                                  // inf                                         /// 00618
			 0x80011111,                                                  // -9.7958E-41                                 /// 0061c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00620
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0xff800000,                                                  // -inf                                        /// 00628
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00630
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00634
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00638
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0063c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00640
			 0x0c600000,                                                  // Leading 1s:                                 /// 00644
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0064c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00000000,                                                  // zero                                        /// 00654
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00658
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0065c
			 0x55555555,                                                  // 4 random values                             /// 00660
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00668
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0066c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00670
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x80000000,                                                  // -zero                                       /// 00678
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0067c
			 0xffc00001,                                                  // -signaling NaN                              /// 00680
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00688
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00694
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00698
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0069c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 006a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x3f028f5c,                                                  // 0.51                                        /// 006e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006e8
			 0xffc00001,                                                  // -signaling NaN                              /// 006ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00700
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00708
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0070c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x0c600000,                                                  // Leading 1s:                                 /// 00714
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00718
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0071c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00720
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00728
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00730
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00734
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0073c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00740
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00744
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00750
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00760
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00764
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00778
			 0x7fc00001,                                                  // signaling NaN                               /// 0077c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00780
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0078c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00794
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00798
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 007d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00800
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00808
			 0x0c780000,                                                  // Leading 1s:                                 /// 0080c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00810
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00818
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0081c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00820
			 0x0c600000,                                                  // Leading 1s:                                 /// 00824
			 0xbf028f5c,                                                  // -0.51                                       /// 00828
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00830
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00834
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00838
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0083c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00844
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0084c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00850
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00858
			 0x4b000000,                                                  // 8388608.0                                   /// 0085c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00860
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00868
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00870
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00878
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0087c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00880
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00884
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00890
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00894
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00898
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0089c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008d0
			 0x55555555,                                                  // 4 random values                             /// 008d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0xbf028f5c,                                                  // -0.51                                       /// 008dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 008e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00904
			 0x0c400000,                                                  // Leading 1s:                                 /// 00908
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0090c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00920
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00924
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00928
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0092c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00948
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0094c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00950
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00954
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00958
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0095c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00960
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00968
			 0xcb000000,                                                  // -8388608.0                                  /// 0096c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00970
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00974
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00978
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0097c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00980
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0098c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e000001,                                                  // Trailing 1s:                                /// 0099c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009f4
			 0x80000000,                                                  // -zero                                       /// 009f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a00
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a10
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a34
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x7f800000,                                                  // inf                                         /// 00a54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a84
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x4b000000,                                                  // 8388608.0                                   /// 00aa4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00aac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ab0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ab8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00abc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ac0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ac8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00acc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ad0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ad4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ad8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00adc
			 0x00000000,                                                  // zero                                        /// 00ae0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ae8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00aec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00af0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00af8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00afc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b38
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x7f800000,                                                  // inf                                         /// 00b40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b70
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x33333333,                                                  // 4 random values                             /// 00b84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ba0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bb0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bb8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bc8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x7f800000,                                                  // inf                                         /// 00bd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bd4
			 0x33333333,                                                  // 4 random values                             /// 00bd8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00be4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c00
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c14
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c24
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c6c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x55555555,                                                  // 4 random values                             /// 00c7c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c9c
			 0x33333333,                                                  // 4 random values                             /// 00ca0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cb0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cb4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cc4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ccc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cd8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ce8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cf4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cf8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cfc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d1c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d54
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0xffc00001,                                                  // -signaling NaN                              /// 00d5c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d80
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0xcb000000,                                                  // -8388608.0                                  /// 00d9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00da0
			 0x55555555,                                                  // 4 random values                             /// 00da4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00da8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00db4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dc4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x7f800000,                                                  // inf                                         /// 00dd8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00de4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00de8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00df4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00df8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dfc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e38
			 0xffc00001,                                                  // -signaling NaN                              /// 00e3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e4c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e54
			 0xbf028f5c,                                                  // -0.51                                       /// 00e58
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e60
			 0x4b000000,                                                  // 8388608.0                                   /// 00e64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e8c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ea0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ea4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eb0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ec0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ecc
			 0xff800000,                                                  // -inf                                        /// 00ed0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x55555555,                                                  // 4 random values                             /// 00ed8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00edc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ee0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ee4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ee8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ef4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ef8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f00
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f14
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f58
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f64
			 0x33333333,                                                  // 4 random values                             /// 00f68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f6c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0xffc00001,                                                  // -signaling NaN                              /// 00f78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f94
			 0xbf028f5c,                                                  // -0.51                                       /// 00f98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fa0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fa8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fb8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fbc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fc4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fc8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fcc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fd0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fdc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fe0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0xbf028f5c,                                                  // -0.51                                       /// 00fe8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ff4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ff8
			 0x80000800                                                  // SP - 1 bit alone set in mantissa -ve        /// last
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
			 0x00000590,
			 0x00000378,
			 0x00000118,
			 0x000005c0,
			 0x000004a8,
			 0x00000394,
			 0x00000484,
			 0x000007ac,

			 /// vpu register f2
			 0x41d00000,
			 0x41800000,
			 0x41800000,
			 0x42000000,
			 0x40400000,
			 0x41d00000,
			 0x41f80000,
			 0x41300000,

			 /// vpu register f3
			 0x40400000,
			 0x41500000,
			 0x40000000,
			 0x40000000,
			 0x41c80000,
			 0x40800000,
			 0x41a80000,
			 0x41880000,

			 /// vpu register f4
			 0x41000000,
			 0x41f80000,
			 0x41980000,
			 0x41980000,
			 0x41000000,
			 0x41600000,
			 0x41d80000,
			 0x41700000,

			 /// vpu register f5
			 0x41c80000,
			 0x3f800000,
			 0x41300000,
			 0x41c00000,
			 0x40e00000,
			 0x41900000,
			 0x40e00000,
			 0x40c00000,

			 /// vpu register f6
			 0x41100000,
			 0x42000000,
			 0x41a00000,
			 0x41700000,
			 0x41f00000,
			 0x41d80000,
			 0x40a00000,
			 0x41a00000,

			 /// vpu register f7
			 0x41900000,
			 0x41b80000,
			 0x41400000,
			 0x3f800000,
			 0x41300000,
			 0x41400000,
			 0x41c00000,
			 0x41e00000,

			 /// vpu register f8
			 0x41880000,
			 0x41e00000,
			 0x42000000,
			 0x40e00000,
			 0x40e00000,
			 0x40000000,
			 0x40800000,
			 0x41980000,

			 /// vpu register f9
			 0x41980000,
			 0x41e80000,
			 0x41b80000,
			 0x41f80000,
			 0x42000000,
			 0x41000000,
			 0x40800000,
			 0x41f00000,

			 /// vpu register f10
			 0x40a00000,
			 0x41600000,
			 0x40800000,
			 0x40c00000,
			 0x41880000,
			 0x41d00000,
			 0x41300000,
			 0x40e00000,

			 /// vpu register f11
			 0x41300000,
			 0x41000000,
			 0x41e80000,
			 0x40a00000,
			 0x41900000,
			 0x42000000,
			 0x41d00000,
			 0x41300000,

			 /// vpu register f12
			 0x41c00000,
			 0x40000000,
			 0x41800000,
			 0x3f800000,
			 0x41100000,
			 0x41700000,
			 0x41100000,
			 0x41a00000,

			 /// vpu register f13
			 0x41900000,
			 0x41e80000,
			 0x40800000,
			 0x41c00000,
			 0x41b80000,
			 0x41c80000,
			 0x41000000,
			 0x41a00000,

			 /// vpu register f14
			 0x41300000,
			 0x41880000,
			 0x41a80000,
			 0x40e00000,
			 0x40c00000,
			 0x41a80000,
			 0x41f00000,
			 0x41b00000,

			 /// vpu register f15
			 0x41800000,
			 0x41700000,
			 0x41a80000,
			 0x41d00000,
			 0x41500000,
			 0x40c00000,
			 0x40e00000,
			 0x42000000,

			 /// vpu register f16
			 0x41c00000,
			 0x41500000,
			 0x41980000,
			 0x41e80000,
			 0x41b80000,
			 0x41c80000,
			 0x41600000,
			 0x41100000,

			 /// vpu register f17
			 0x3f800000,
			 0x42000000,
			 0x41900000,
			 0x41700000,
			 0x41a80000,
			 0x41e80000,
			 0x41f00000,
			 0x41000000,

			 /// vpu register f18
			 0x41800000,
			 0x3f800000,
			 0x41400000,
			 0x41e00000,
			 0x40400000,
			 0x41f00000,
			 0x41e00000,
			 0x40e00000,

			 /// vpu register f19
			 0x41100000,
			 0x40c00000,
			 0x41500000,
			 0x41400000,
			 0x3f800000,
			 0x41b80000,
			 0x41700000,
			 0x40000000,

			 /// vpu register f20
			 0x41c00000,
			 0x41b00000,
			 0x40a00000,
			 0x41d00000,
			 0x3f800000,
			 0x41200000,
			 0x41c80000,
			 0x41c00000,

			 /// vpu register f21
			 0x41c00000,
			 0x41b00000,
			 0x41400000,
			 0x40000000,
			 0x41880000,
			 0x41f00000,
			 0x40800000,
			 0x41880000,

			 /// vpu register f22
			 0x41400000,
			 0x41880000,
			 0x41500000,
			 0x41a00000,
			 0x40e00000,
			 0x41a00000,
			 0x41800000,
			 0x41b80000,

			 /// vpu register f23
			 0x41b00000,
			 0x40800000,
			 0x41f00000,
			 0x40c00000,
			 0x41880000,
			 0x42000000,
			 0x41d00000,
			 0x41c80000,

			 /// vpu register f24
			 0x40e00000,
			 0x42000000,
			 0x41c80000,
			 0x41c80000,
			 0x41a80000,
			 0x3f800000,
			 0x3f800000,
			 0x41a80000,

			 /// vpu register f25
			 0x41a80000,
			 0x41d80000,
			 0x40a00000,
			 0x41880000,
			 0x41900000,
			 0x40800000,
			 0x41200000,
			 0x41500000,

			 /// vpu register f26
			 0x41000000,
			 0x40a00000,
			 0x41500000,
			 0x41f80000,
			 0x41900000,
			 0x41600000,
			 0x41e00000,
			 0x41e00000,

			 /// vpu register f27
			 0x41d00000,
			 0x41200000,
			 0x41600000,
			 0x41f00000,
			 0x41a00000,
			 0x41400000,
			 0x41800000,
			 0x41f80000,

			 /// vpu register f28
			 0x41a00000,
			 0x41100000,
			 0x41500000,
			 0x41c00000,
			 0x41980000,
			 0x41700000,
			 0x41c80000,
			 0x40a00000,

			 /// vpu register f29
			 0x41f00000,
			 0x41700000,
			 0x40400000,
			 0x40e00000,
			 0x41100000,
			 0x41f80000,
			 0x42000000,
			 0x41f80000,

			 /// vpu register f30
			 0x41600000,
			 0x3f800000,
			 0x40800000,
			 0x41100000,
			 0x40a00000,
			 0x40a00000,
			 0x41e80000,
			 0x41100000,

			 /// vpu register f31
			 0x40e00000,
			 0x41200000,
			 0x41700000,
			 0x41200000,
			 0x40000000,
			 0x41900000,
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

	"LBL_VPU_SEQ_START_0:\n"
		"li x14, 0x1\n"
		"flw f5, 0(x14)\n"                                  ///  0,    38


		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
