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
			 0x279a2f58, /// 0x0
			 0x9d815ef0, /// 0x4
			 0xde064222, /// 0x8
			 0x176bd831, /// 0xc
			 0x338d4bd7, /// 0x10
			 0x31f93831, /// 0x14
			 0xc6e71527, /// 0x18
			 0x69d19fc9, /// 0x1c
			 0xde1bf356, /// 0x20
			 0x3913006f, /// 0x24
			 0x3e8bad5a, /// 0x28
			 0xef1b0c4c, /// 0x2c
			 0xc56543e1, /// 0x30
			 0xce5a3119, /// 0x34
			 0x95bff3d7, /// 0x38
			 0xa7c6d118, /// 0x3c
			 0x36cad868, /// 0x40
			 0xaadb08fa, /// 0x44
			 0x14296e13, /// 0x48
			 0x5304baf3, /// 0x4c
			 0xbef09294, /// 0x50
			 0xc0e45397, /// 0x54
			 0xa86cf93d, /// 0x58
			 0x2b208811, /// 0x5c
			 0x24c77a60, /// 0x60
			 0xa65ab829, /// 0x64
			 0x359b279d, /// 0x68
			 0xaea5fc68, /// 0x6c
			 0xb30180b1, /// 0x70
			 0x46d8c691, /// 0x74
			 0xc9f6ac4f, /// 0x78
			 0xa921b0f6, /// 0x7c
			 0xd2943ece, /// 0x80
			 0x90e9d8b9, /// 0x84
			 0x59f25a78, /// 0x88
			 0xd4828f41, /// 0x8c
			 0x2a56f15a, /// 0x90
			 0x1b55ae31, /// 0x94
			 0x752721a2, /// 0x98
			 0x238bf9a4, /// 0x9c
			 0xe31d5635, /// 0xa0
			 0x557c35b2, /// 0xa4
			 0xff67055b, /// 0xa8
			 0xa013a152, /// 0xac
			 0x1e69238f, /// 0xb0
			 0x7af5eaa5, /// 0xb4
			 0xd6547aaf, /// 0xb8
			 0x9e8e7253, /// 0xbc
			 0x55c4bfee, /// 0xc0
			 0x92d141aa, /// 0xc4
			 0xec2846f6, /// 0xc8
			 0x913b87b0, /// 0xcc
			 0x1ac127b4, /// 0xd0
			 0x1747a8b3, /// 0xd4
			 0x6cfecb21, /// 0xd8
			 0x8ad28b05, /// 0xdc
			 0x58c44d52, /// 0xe0
			 0x2161b8db, /// 0xe4
			 0xa24248e8, /// 0xe8
			 0xb6820114, /// 0xec
			 0xb25c662a, /// 0xf0
			 0x83f143d1, /// 0xf4
			 0x53e5dac0, /// 0xf8
			 0x87bbf1f1, /// 0xfc
			 0xf2e1dfd7, /// 0x100
			 0xbb8b0122, /// 0x104
			 0x0080d699, /// 0x108
			 0xe947c3be, /// 0x10c
			 0x6ecbddf7, /// 0x110
			 0x1347272d, /// 0x114
			 0xa68b0c9a, /// 0x118
			 0x378218e4, /// 0x11c
			 0x5df6eb4a, /// 0x120
			 0x487e5f68, /// 0x124
			 0x053a4256, /// 0x128
			 0x66841fed, /// 0x12c
			 0xcb9b85fe, /// 0x130
			 0x77070e31, /// 0x134
			 0xe0adf4be, /// 0x138
			 0x5f6de64c, /// 0x13c
			 0x713511af, /// 0x140
			 0xf55d89d0, /// 0x144
			 0x9e3ea202, /// 0x148
			 0x29218c99, /// 0x14c
			 0x7f909058, /// 0x150
			 0xfcbf8e53, /// 0x154
			 0x533519e2, /// 0x158
			 0x738fb216, /// 0x15c
			 0x3851de0b, /// 0x160
			 0x47a84ef5, /// 0x164
			 0x1f0483da, /// 0x168
			 0x2886d70d, /// 0x16c
			 0x87afbf97, /// 0x170
			 0x4bc55d96, /// 0x174
			 0xcab47357, /// 0x178
			 0x0f045c16, /// 0x17c
			 0x6e6991ec, /// 0x180
			 0x6d74be25, /// 0x184
			 0x9e6244b9, /// 0x188
			 0x694a3e78, /// 0x18c
			 0x6ef0191c, /// 0x190
			 0xf54d5b0f, /// 0x194
			 0xc95d0faa, /// 0x198
			 0x3a712097, /// 0x19c
			 0xf7b0c8cc, /// 0x1a0
			 0x04ca70f1, /// 0x1a4
			 0x6c017bae, /// 0x1a8
			 0xaf62aaac, /// 0x1ac
			 0xec2bf06d, /// 0x1b0
			 0xd14c3838, /// 0x1b4
			 0x5eacc24f, /// 0x1b8
			 0xd107885c, /// 0x1bc
			 0x08ef070b, /// 0x1c0
			 0xb3d5f106, /// 0x1c4
			 0xa423e75e, /// 0x1c8
			 0x74a86a3e, /// 0x1cc
			 0xd1c3630d, /// 0x1d0
			 0xa7ecbec6, /// 0x1d4
			 0xb1fcdaff, /// 0x1d8
			 0x8c4dffa9, /// 0x1dc
			 0x4bba367f, /// 0x1e0
			 0x4012b648, /// 0x1e4
			 0x6eabc1f5, /// 0x1e8
			 0xa4ebcadb, /// 0x1ec
			 0x98ab6cc7, /// 0x1f0
			 0x7f767e8d, /// 0x1f4
			 0x6547b763, /// 0x1f8
			 0x13d2b4f8, /// 0x1fc
			 0x2bf90a7a, /// 0x200
			 0x89bb2376, /// 0x204
			 0x82ea4cae, /// 0x208
			 0x2c78ee11, /// 0x20c
			 0xee659da1, /// 0x210
			 0x292d4424, /// 0x214
			 0x29d04211, /// 0x218
			 0x81828447, /// 0x21c
			 0x34212079, /// 0x220
			 0x71d6e66e, /// 0x224
			 0x58e8c22d, /// 0x228
			 0xa102c691, /// 0x22c
			 0xcd21cd58, /// 0x230
			 0xf228521b, /// 0x234
			 0x83f5a621, /// 0x238
			 0x9d75fbf1, /// 0x23c
			 0x136478e9, /// 0x240
			 0xf9ef7e00, /// 0x244
			 0x7f3f1e70, /// 0x248
			 0x95c2764e, /// 0x24c
			 0xe42cb3a8, /// 0x250
			 0xf6a3d6d5, /// 0x254
			 0x7d4dd73c, /// 0x258
			 0x4f18808c, /// 0x25c
			 0xf246b82c, /// 0x260
			 0xeef37c1a, /// 0x264
			 0x45eb9284, /// 0x268
			 0x9b90e3ab, /// 0x26c
			 0x06b6be13, /// 0x270
			 0xf58eb8dc, /// 0x274
			 0x299a7abf, /// 0x278
			 0x48b52d1b, /// 0x27c
			 0x3f9d33b2, /// 0x280
			 0xdccedb28, /// 0x284
			 0x0ef40c1f, /// 0x288
			 0xeda45067, /// 0x28c
			 0x3c2dbed8, /// 0x290
			 0x651c36c5, /// 0x294
			 0x2455e480, /// 0x298
			 0x6a050edd, /// 0x29c
			 0x68dca3ab, /// 0x2a0
			 0x8d322348, /// 0x2a4
			 0xc924eb95, /// 0x2a8
			 0x94c22829, /// 0x2ac
			 0xfccbdc08, /// 0x2b0
			 0x543575c5, /// 0x2b4
			 0xc4ceac05, /// 0x2b8
			 0x9fb61986, /// 0x2bc
			 0x3e4fcb91, /// 0x2c0
			 0x64dc656b, /// 0x2c4
			 0xf1e86942, /// 0x2c8
			 0x933e9469, /// 0x2cc
			 0x1a95d184, /// 0x2d0
			 0x40ef6047, /// 0x2d4
			 0x8e107111, /// 0x2d8
			 0xfdaca5f6, /// 0x2dc
			 0x411104b3, /// 0x2e0
			 0x22c7786e, /// 0x2e4
			 0x404283d2, /// 0x2e8
			 0xbac31c81, /// 0x2ec
			 0x3a69fd74, /// 0x2f0
			 0x5b8e3111, /// 0x2f4
			 0x44bdfaa3, /// 0x2f8
			 0xe1770535, /// 0x2fc
			 0x5063d884, /// 0x300
			 0xde9d1c07, /// 0x304
			 0x5bc920e7, /// 0x308
			 0xdaf01cb7, /// 0x30c
			 0x5a076cf6, /// 0x310
			 0x010c8f83, /// 0x314
			 0x34046b2b, /// 0x318
			 0xfbbc7553, /// 0x31c
			 0x29c087ec, /// 0x320
			 0xd5815ed1, /// 0x324
			 0x821b27e9, /// 0x328
			 0x825dc1ea, /// 0x32c
			 0x9bf387d5, /// 0x330
			 0xd59c08af, /// 0x334
			 0x9d8681f0, /// 0x338
			 0xa9a8ac3e, /// 0x33c
			 0xedeca817, /// 0x340
			 0xcd5612df, /// 0x344
			 0xddc2b994, /// 0x348
			 0x1a32849d, /// 0x34c
			 0xaff1b64b, /// 0x350
			 0xe2031013, /// 0x354
			 0x5984f3fe, /// 0x358
			 0x2f8d5c0f, /// 0x35c
			 0x03d71187, /// 0x360
			 0xe9cac55b, /// 0x364
			 0x97c3fdaf, /// 0x368
			 0x76f2dd05, /// 0x36c
			 0xd6fdf81a, /// 0x370
			 0x03e4217a, /// 0x374
			 0xe6075d26, /// 0x378
			 0xb5d57767, /// 0x37c
			 0xb7dad19b, /// 0x380
			 0x46dceeca, /// 0x384
			 0x20cdc1e7, /// 0x388
			 0x3e69639f, /// 0x38c
			 0xb06e4de7, /// 0x390
			 0x6e32f8a7, /// 0x394
			 0xe9d86d3f, /// 0x398
			 0xb2aed213, /// 0x39c
			 0x6d221329, /// 0x3a0
			 0x6309a736, /// 0x3a4
			 0xb3443c66, /// 0x3a8
			 0x6479bb93, /// 0x3ac
			 0x3a3a5dae, /// 0x3b0
			 0x4886a30a, /// 0x3b4
			 0x79515e29, /// 0x3b8
			 0x85391f86, /// 0x3bc
			 0xbec76a6c, /// 0x3c0
			 0x64199271, /// 0x3c4
			 0xbff8d229, /// 0x3c8
			 0x044328a6, /// 0x3cc
			 0x7786e522, /// 0x3d0
			 0x00891edf, /// 0x3d4
			 0x77972a03, /// 0x3d8
			 0xdf1ea596, /// 0x3dc
			 0x571a717a, /// 0x3e0
			 0x10a11011, /// 0x3e4
			 0x60d1bea2, /// 0x3e8
			 0x2ca731bf, /// 0x3ec
			 0xa83f49f3, /// 0x3f0
			 0x46ce656e, /// 0x3f4
			 0xd23fb3d9, /// 0x3f8
			 0x066bb166, /// 0x3fc
			 0x93b159d7, /// 0x400
			 0xc4661432, /// 0x404
			 0x11972c1f, /// 0x408
			 0x77de1a79, /// 0x40c
			 0xb7c74311, /// 0x410
			 0x4e35c077, /// 0x414
			 0xe6622efe, /// 0x418
			 0x02015391, /// 0x41c
			 0x461efeca, /// 0x420
			 0xfa0e55df, /// 0x424
			 0x4ee142ad, /// 0x428
			 0x49b82093, /// 0x42c
			 0xa6c65138, /// 0x430
			 0x80680ed3, /// 0x434
			 0x46a77d2d, /// 0x438
			 0x75805172, /// 0x43c
			 0x49e4b188, /// 0x440
			 0x7c276d83, /// 0x444
			 0x1c9ee52c, /// 0x448
			 0x16462018, /// 0x44c
			 0x1cf16dd1, /// 0x450
			 0x35cb6d9e, /// 0x454
			 0x46a03fdc, /// 0x458
			 0xc9f8ece8, /// 0x45c
			 0x76cd8c0a, /// 0x460
			 0x1214d059, /// 0x464
			 0x0c75b049, /// 0x468
			 0xb1e0c671, /// 0x46c
			 0xfd109731, /// 0x470
			 0x3d778d9e, /// 0x474
			 0xfcde9e5c, /// 0x478
			 0x711ccf04, /// 0x47c
			 0x75557e26, /// 0x480
			 0x65af0798, /// 0x484
			 0xaa39f144, /// 0x488
			 0xc3686c38, /// 0x48c
			 0xd3aa1910, /// 0x490
			 0xc4859dad, /// 0x494
			 0xa00fbffa, /// 0x498
			 0x5dc8becd, /// 0x49c
			 0xf919d584, /// 0x4a0
			 0x7193dd61, /// 0x4a4
			 0x081f467f, /// 0x4a8
			 0xdf4c55d1, /// 0x4ac
			 0x21d6760e, /// 0x4b0
			 0x9cc67c15, /// 0x4b4
			 0xe24f4a26, /// 0x4b8
			 0xeb694bfb, /// 0x4bc
			 0xe06a491c, /// 0x4c0
			 0x585043c4, /// 0x4c4
			 0xb288637b, /// 0x4c8
			 0x7feb5541, /// 0x4cc
			 0xd95fcfea, /// 0x4d0
			 0x384b0d17, /// 0x4d4
			 0xb11d8530, /// 0x4d8
			 0x0c5b1d1b, /// 0x4dc
			 0x5da9f75e, /// 0x4e0
			 0x12bf35c0, /// 0x4e4
			 0xa39c6db5, /// 0x4e8
			 0x6649df5d, /// 0x4ec
			 0x6d002918, /// 0x4f0
			 0x34339878, /// 0x4f4
			 0xca40d3f1, /// 0x4f8
			 0xa28b10c0, /// 0x4fc
			 0xee1a40a1, /// 0x500
			 0xdad9ea11, /// 0x504
			 0x21b8e969, /// 0x508
			 0xbe9b5e7e, /// 0x50c
			 0x37600d41, /// 0x510
			 0x56c1e180, /// 0x514
			 0x836890eb, /// 0x518
			 0xbf37d67e, /// 0x51c
			 0x3a0db66b, /// 0x520
			 0x83322771, /// 0x524
			 0x88927a94, /// 0x528
			 0x5888893d, /// 0x52c
			 0x3eb40196, /// 0x530
			 0x56cc2725, /// 0x534
			 0x72fdcbfd, /// 0x538
			 0xe8d95b2e, /// 0x53c
			 0xe93d34d6, /// 0x540
			 0x5574024a, /// 0x544
			 0x191291a3, /// 0x548
			 0xa2020dd7, /// 0x54c
			 0xd80376b5, /// 0x550
			 0x2667946a, /// 0x554
			 0xa6dbd9e3, /// 0x558
			 0xef90a5d2, /// 0x55c
			 0xcd5f84dd, /// 0x560
			 0x2d326130, /// 0x564
			 0x0c94c516, /// 0x568
			 0x4cb19fdd, /// 0x56c
			 0x1689e578, /// 0x570
			 0x1f8cacfa, /// 0x574
			 0x72bf194d, /// 0x578
			 0xa969bff9, /// 0x57c
			 0xe75ec416, /// 0x580
			 0xa6ef908d, /// 0x584
			 0x0545916a, /// 0x588
			 0x6be70888, /// 0x58c
			 0x6af44e1a, /// 0x590
			 0x217dede6, /// 0x594
			 0x94bd1d01, /// 0x598
			 0x55b7d20e, /// 0x59c
			 0xaf35530f, /// 0x5a0
			 0x3d37acdd, /// 0x5a4
			 0x8fd11cb6, /// 0x5a8
			 0xabe6c5b5, /// 0x5ac
			 0xdd13c2ff, /// 0x5b0
			 0x26dae695, /// 0x5b4
			 0x22f4e4b9, /// 0x5b8
			 0x0beb0cc0, /// 0x5bc
			 0xe62f6942, /// 0x5c0
			 0x7cdcb9eb, /// 0x5c4
			 0x9dcf942c, /// 0x5c8
			 0xfd1e12c3, /// 0x5cc
			 0xc4745fe8, /// 0x5d0
			 0x268c389f, /// 0x5d4
			 0x6f84edcf, /// 0x5d8
			 0xd0ef464c, /// 0x5dc
			 0x670144cb, /// 0x5e0
			 0xb7e8d79a, /// 0x5e4
			 0x3c555212, /// 0x5e8
			 0x1003a548, /// 0x5ec
			 0x8d24a68a, /// 0x5f0
			 0xfca96d9d, /// 0x5f4
			 0x0ff14261, /// 0x5f8
			 0x1c02efb1, /// 0x5fc
			 0x9e833fd6, /// 0x600
			 0xec4fc82b, /// 0x604
			 0x8b24c9cc, /// 0x608
			 0x145890fe, /// 0x60c
			 0xed1c89c3, /// 0x610
			 0xb70bd47b, /// 0x614
			 0x9d6f2520, /// 0x618
			 0x14c75fc0, /// 0x61c
			 0x1fdda79b, /// 0x620
			 0xce93a035, /// 0x624
			 0x5bf7d3f9, /// 0x628
			 0xecd83303, /// 0x62c
			 0x603349a8, /// 0x630
			 0xd642eedf, /// 0x634
			 0x4fe1d1fc, /// 0x638
			 0xe59036bc, /// 0x63c
			 0x76713e9f, /// 0x640
			 0x9adef49e, /// 0x644
			 0xa8ec2cd0, /// 0x648
			 0x1219888f, /// 0x64c
			 0xa8dcfbf5, /// 0x650
			 0xdc6f2837, /// 0x654
			 0x5911668a, /// 0x658
			 0x19a508b2, /// 0x65c
			 0xc01096c3, /// 0x660
			 0x0903d333, /// 0x664
			 0x41f2084c, /// 0x668
			 0xbb204cd9, /// 0x66c
			 0x5c5d03fb, /// 0x670
			 0x03bbe8f4, /// 0x674
			 0x9ad3807b, /// 0x678
			 0xd915a69a, /// 0x67c
			 0xa86e59e7, /// 0x680
			 0xd14d5cc7, /// 0x684
			 0x383e0996, /// 0x688
			 0xfb05db33, /// 0x68c
			 0x15e0fd6c, /// 0x690
			 0xc88f07d1, /// 0x694
			 0xd2aca3f8, /// 0x698
			 0xb647a515, /// 0x69c
			 0xbe1d6b4d, /// 0x6a0
			 0x22a799b8, /// 0x6a4
			 0x57880912, /// 0x6a8
			 0xe9b4a566, /// 0x6ac
			 0xa7144350, /// 0x6b0
			 0x92aeb663, /// 0x6b4
			 0x297f1391, /// 0x6b8
			 0x30d0eb24, /// 0x6bc
			 0x1e72a579, /// 0x6c0
			 0x13abbabb, /// 0x6c4
			 0xc5966d3c, /// 0x6c8
			 0x4dc80ced, /// 0x6cc
			 0xc48a8099, /// 0x6d0
			 0x4cc1d405, /// 0x6d4
			 0xd0c6449b, /// 0x6d8
			 0x635ba7eb, /// 0x6dc
			 0xa68a7cca, /// 0x6e0
			 0xb450b6a6, /// 0x6e4
			 0xa2920173, /// 0x6e8
			 0xd94dc7a9, /// 0x6ec
			 0xb90eaf39, /// 0x6f0
			 0xa9e360e0, /// 0x6f4
			 0x8c5cd4fa, /// 0x6f8
			 0x89c6ab56, /// 0x6fc
			 0x0e690f0e, /// 0x700
			 0x1500c9a9, /// 0x704
			 0xa3216ec2, /// 0x708
			 0xc40b1f8d, /// 0x70c
			 0x1b2bcc09, /// 0x710
			 0x6bbffffe, /// 0x714
			 0xbbb74815, /// 0x718
			 0xffe07fd6, /// 0x71c
			 0xbdb7cd7c, /// 0x720
			 0x02c642ef, /// 0x724
			 0xd399db56, /// 0x728
			 0xf3a7671b, /// 0x72c
			 0xb4b36860, /// 0x730
			 0xa7f41d72, /// 0x734
			 0xb4d652f7, /// 0x738
			 0x32553c83, /// 0x73c
			 0x047a7408, /// 0x740
			 0x29f68f6e, /// 0x744
			 0xfa76ecf7, /// 0x748
			 0x344004bf, /// 0x74c
			 0x891e5afe, /// 0x750
			 0xe5ffca1f, /// 0x754
			 0x5da62f18, /// 0x758
			 0xb033a119, /// 0x75c
			 0xcd6632f6, /// 0x760
			 0x9d05e293, /// 0x764
			 0x5354da2e, /// 0x768
			 0xa32064b5, /// 0x76c
			 0x0d2215de, /// 0x770
			 0x34b6dffe, /// 0x774
			 0x87adb184, /// 0x778
			 0x2628ca73, /// 0x77c
			 0x28c6227b, /// 0x780
			 0x64fb81da, /// 0x784
			 0x7da5a431, /// 0x788
			 0xa8a32d67, /// 0x78c
			 0x9be5299d, /// 0x790
			 0xc068a5fc, /// 0x794
			 0x33375b33, /// 0x798
			 0xd7f40281, /// 0x79c
			 0x94bd0188, /// 0x7a0
			 0x66db7c22, /// 0x7a4
			 0x307b104c, /// 0x7a8
			 0xc68af664, /// 0x7ac
			 0xcd14733d, /// 0x7b0
			 0xdbfd6083, /// 0x7b4
			 0x9fcdeddb, /// 0x7b8
			 0x1300c695, /// 0x7bc
			 0x4d0b8d0b, /// 0x7c0
			 0xf564d239, /// 0x7c4
			 0xd369fbfa, /// 0x7c8
			 0x20689e8f, /// 0x7cc
			 0x5dc2396b, /// 0x7d0
			 0x1fc003ad, /// 0x7d4
			 0xe858d088, /// 0x7d8
			 0x5ab97824, /// 0x7dc
			 0xb33fc775, /// 0x7e0
			 0x7e9f632f, /// 0x7e4
			 0xf0b3e143, /// 0x7e8
			 0xfd8e078b, /// 0x7ec
			 0x33a32c92, /// 0x7f0
			 0x589b4864, /// 0x7f4
			 0xbe7984a5, /// 0x7f8
			 0xe094e529, /// 0x7fc
			 0x5b3dfd18, /// 0x800
			 0x031ab3ea, /// 0x804
			 0x3c8752e8, /// 0x808
			 0x1801c1f9, /// 0x80c
			 0x006a5fb4, /// 0x810
			 0x80006ba1, /// 0x814
			 0x9aa1d9a5, /// 0x818
			 0x3cd1fd1d, /// 0x81c
			 0xe1e1c0e9, /// 0x820
			 0x0f5df409, /// 0x824
			 0xe624a6b7, /// 0x828
			 0x97cc74be, /// 0x82c
			 0x8bb65101, /// 0x830
			 0x941b683e, /// 0x834
			 0x5b02306d, /// 0x838
			 0x07a64e29, /// 0x83c
			 0x86f5700e, /// 0x840
			 0x1487f14c, /// 0x844
			 0x91f5fa50, /// 0x848
			 0x6351953b, /// 0x84c
			 0x9981facf, /// 0x850
			 0x385958a2, /// 0x854
			 0xebdb3697, /// 0x858
			 0x8bacf440, /// 0x85c
			 0x8b1886f2, /// 0x860
			 0xdbbbe8f8, /// 0x864
			 0x71bd1ee8, /// 0x868
			 0xf2d17b3b, /// 0x86c
			 0x2d45d99d, /// 0x870
			 0x960a1698, /// 0x874
			 0x415d05f7, /// 0x878
			 0x2a786457, /// 0x87c
			 0x602f8bf2, /// 0x880
			 0x0450b98d, /// 0x884
			 0x1e075b37, /// 0x888
			 0xd39b886e, /// 0x88c
			 0x1aaf3c3f, /// 0x890
			 0x2a4d050d, /// 0x894
			 0xe12d96a5, /// 0x898
			 0x66853d7b, /// 0x89c
			 0x508a2664, /// 0x8a0
			 0xef57c07d, /// 0x8a4
			 0x793e946e, /// 0x8a8
			 0x7e857db0, /// 0x8ac
			 0x1ff60a70, /// 0x8b0
			 0x35dafa0f, /// 0x8b4
			 0x46280d84, /// 0x8b8
			 0x773a22fc, /// 0x8bc
			 0x3a192c61, /// 0x8c0
			 0x6be5a242, /// 0x8c4
			 0x3c5ce9aa, /// 0x8c8
			 0xa7bca405, /// 0x8cc
			 0xc9a2a151, /// 0x8d0
			 0xe12525a3, /// 0x8d4
			 0x60e62051, /// 0x8d8
			 0x2130bfdc, /// 0x8dc
			 0xab0b5bd0, /// 0x8e0
			 0x9b475450, /// 0x8e4
			 0x0f4bec97, /// 0x8e8
			 0x2aa15986, /// 0x8ec
			 0x769f017a, /// 0x8f0
			 0x4de82c63, /// 0x8f4
			 0x50a0b0ea, /// 0x8f8
			 0x9e84a560, /// 0x8fc
			 0xf9b45913, /// 0x900
			 0x268d5784, /// 0x904
			 0x62d68b32, /// 0x908
			 0x40ffdbbe, /// 0x90c
			 0xb9b3fc8c, /// 0x910
			 0xd7bf2705, /// 0x914
			 0xb42f7212, /// 0x918
			 0xf991fb9f, /// 0x91c
			 0xd670e9b1, /// 0x920
			 0x35b246b7, /// 0x924
			 0x23bd8d94, /// 0x928
			 0x7420e95d, /// 0x92c
			 0x24b49c77, /// 0x930
			 0x468c6593, /// 0x934
			 0x25b5761d, /// 0x938
			 0x834da35a, /// 0x93c
			 0x60e8458a, /// 0x940
			 0x638248d9, /// 0x944
			 0x29cd74ce, /// 0x948
			 0x2476e59f, /// 0x94c
			 0x3fbab7d9, /// 0x950
			 0x647c6c61, /// 0x954
			 0x09982e49, /// 0x958
			 0xd2d1ebb3, /// 0x95c
			 0xc272ba19, /// 0x960
			 0x8d950f95, /// 0x964
			 0x5648684b, /// 0x968
			 0x2c6c3351, /// 0x96c
			 0xd7109bf1, /// 0x970
			 0xce0769f8, /// 0x974
			 0x37752565, /// 0x978
			 0xaa81a7fa, /// 0x97c
			 0xd2b62a63, /// 0x980
			 0x50fa0e0c, /// 0x984
			 0x9fff2e19, /// 0x988
			 0x2c4a887f, /// 0x98c
			 0xbfbd9b1e, /// 0x990
			 0x9c4d9efb, /// 0x994
			 0xaece08b1, /// 0x998
			 0x0bed83f5, /// 0x99c
			 0x38b0013b, /// 0x9a0
			 0x98bc7da5, /// 0x9a4
			 0xda466984, /// 0x9a8
			 0x09e39c80, /// 0x9ac
			 0xbbea5a02, /// 0x9b0
			 0x11b969f7, /// 0x9b4
			 0x186ec3e4, /// 0x9b8
			 0x53eef002, /// 0x9bc
			 0x5f91d82b, /// 0x9c0
			 0xdcd0d694, /// 0x9c4
			 0x9c0dc589, /// 0x9c8
			 0x7b666b38, /// 0x9cc
			 0x77776570, /// 0x9d0
			 0xa0e0c4c1, /// 0x9d4
			 0x4f581f77, /// 0x9d8
			 0xc5ef2728, /// 0x9dc
			 0xecae95ab, /// 0x9e0
			 0x5fd9c647, /// 0x9e4
			 0xfa892cdd, /// 0x9e8
			 0xe3571234, /// 0x9ec
			 0x7d98cc9d, /// 0x9f0
			 0x89279ad7, /// 0x9f4
			 0x56b7eb06, /// 0x9f8
			 0x17720de8, /// 0x9fc
			 0x267c83ae, /// 0xa00
			 0x0bb88459, /// 0xa04
			 0xc3ba0a61, /// 0xa08
			 0xc3ce6db2, /// 0xa0c
			 0xc9ace550, /// 0xa10
			 0x4f4ac5e2, /// 0xa14
			 0x6e48b017, /// 0xa18
			 0x60b4b6cd, /// 0xa1c
			 0x8a9250bd, /// 0xa20
			 0x52582722, /// 0xa24
			 0xee876783, /// 0xa28
			 0xa8155639, /// 0xa2c
			 0xdb3af64c, /// 0xa30
			 0xbc96c25f, /// 0xa34
			 0xf6d9044f, /// 0xa38
			 0xfc793564, /// 0xa3c
			 0x58a19e4d, /// 0xa40
			 0x094c0d6d, /// 0xa44
			 0xcd595040, /// 0xa48
			 0x8951e913, /// 0xa4c
			 0x3dfc10d3, /// 0xa50
			 0x2944181c, /// 0xa54
			 0x9cf90ecc, /// 0xa58
			 0x9b83d85f, /// 0xa5c
			 0xca66d098, /// 0xa60
			 0x0be52470, /// 0xa64
			 0xb64fafb8, /// 0xa68
			 0xde6fb6ee, /// 0xa6c
			 0xb947c39a, /// 0xa70
			 0x71829461, /// 0xa74
			 0xfb8e7aeb, /// 0xa78
			 0xd1d13052, /// 0xa7c
			 0x383127f4, /// 0xa80
			 0x3154b5e5, /// 0xa84
			 0xb916287c, /// 0xa88
			 0x0f33e434, /// 0xa8c
			 0x5a4b9a09, /// 0xa90
			 0xb121051a, /// 0xa94
			 0xfe1ec27c, /// 0xa98
			 0xfb37aefb, /// 0xa9c
			 0xc1ec6263, /// 0xaa0
			 0x86ca8ec0, /// 0xaa4
			 0xcbb01405, /// 0xaa8
			 0x6f3db83e, /// 0xaac
			 0x41407f23, /// 0xab0
			 0xc86ccd8e, /// 0xab4
			 0xd99f09bc, /// 0xab8
			 0x91a68dc7, /// 0xabc
			 0xef58ca07, /// 0xac0
			 0x12539cac, /// 0xac4
			 0x76a5f8d9, /// 0xac8
			 0xbe1d2eff, /// 0xacc
			 0xc172d22f, /// 0xad0
			 0x868756bc, /// 0xad4
			 0xeecaae9e, /// 0xad8
			 0x31e954f6, /// 0xadc
			 0xfcb5f412, /// 0xae0
			 0x61e32ec4, /// 0xae4
			 0x546277e6, /// 0xae8
			 0xb8094d2a, /// 0xaec
			 0x613d19a3, /// 0xaf0
			 0x79362d14, /// 0xaf4
			 0xc38053a6, /// 0xaf8
			 0xffea3a17, /// 0xafc
			 0xd32b645a, /// 0xb00
			 0x54bc3549, /// 0xb04
			 0xfc830b31, /// 0xb08
			 0xbec8f530, /// 0xb0c
			 0x267bf20f, /// 0xb10
			 0x154cf76b, /// 0xb14
			 0x8a7c2f1d, /// 0xb18
			 0x9a6aea84, /// 0xb1c
			 0x81c7c609, /// 0xb20
			 0xf67767a6, /// 0xb24
			 0x920aad71, /// 0xb28
			 0x315df21a, /// 0xb2c
			 0x96144909, /// 0xb30
			 0xa178f621, /// 0xb34
			 0xe5557a12, /// 0xb38
			 0x6f6e2007, /// 0xb3c
			 0x578f49dd, /// 0xb40
			 0x7d3cf011, /// 0xb44
			 0x91f0386e, /// 0xb48
			 0x64dedfc7, /// 0xb4c
			 0x0680ca58, /// 0xb50
			 0xe573abd0, /// 0xb54
			 0x6762f381, /// 0xb58
			 0x61969838, /// 0xb5c
			 0x88cbc6d1, /// 0xb60
			 0x3482567f, /// 0xb64
			 0x43437554, /// 0xb68
			 0x36e55b45, /// 0xb6c
			 0xfa2d623a, /// 0xb70
			 0x980ccbd4, /// 0xb74
			 0x54229d3e, /// 0xb78
			 0xc8c28f2e, /// 0xb7c
			 0x22a462dc, /// 0xb80
			 0xf0ceea5c, /// 0xb84
			 0x0eb5f1cd, /// 0xb88
			 0xa5eeafc3, /// 0xb8c
			 0x7344486e, /// 0xb90
			 0x0b535d55, /// 0xb94
			 0x920ffc2d, /// 0xb98
			 0x769be3e0, /// 0xb9c
			 0xcfeb9d14, /// 0xba0
			 0x29085907, /// 0xba4
			 0xe5264b02, /// 0xba8
			 0xf0037f2f, /// 0xbac
			 0xa9c38ddf, /// 0xbb0
			 0x61e2fe91, /// 0xbb4
			 0x99a94673, /// 0xbb8
			 0x8056fb67, /// 0xbbc
			 0x49921fdd, /// 0xbc0
			 0x25575d3c, /// 0xbc4
			 0x4629d084, /// 0xbc8
			 0x7c18e2f1, /// 0xbcc
			 0xd2eafe9f, /// 0xbd0
			 0x55ab98b1, /// 0xbd4
			 0xcd6dea7f, /// 0xbd8
			 0x202a3e10, /// 0xbdc
			 0xf981ce3e, /// 0xbe0
			 0x00c1937c, /// 0xbe4
			 0xae7e8c7c, /// 0xbe8
			 0xabcc658a, /// 0xbec
			 0xb5f5a688, /// 0xbf0
			 0xe0da9636, /// 0xbf4
			 0xe1508920, /// 0xbf8
			 0x27aae24e, /// 0xbfc
			 0x3cc2505f, /// 0xc00
			 0x3c812c1f, /// 0xc04
			 0xe65c327b, /// 0xc08
			 0x7d248edc, /// 0xc0c
			 0xac92191f, /// 0xc10
			 0x644187c8, /// 0xc14
			 0xf56c8141, /// 0xc18
			 0x75768ab4, /// 0xc1c
			 0x7b374b46, /// 0xc20
			 0xcce4edca, /// 0xc24
			 0x98174586, /// 0xc28
			 0x3c3184ff, /// 0xc2c
			 0xd2db47fc, /// 0xc30
			 0xfbe4a46c, /// 0xc34
			 0x64c54bbd, /// 0xc38
			 0x7cabdaa2, /// 0xc3c
			 0xe9271fbb, /// 0xc40
			 0x4b78c834, /// 0xc44
			 0x03251973, /// 0xc48
			 0xa41b0424, /// 0xc4c
			 0x0c27a4de, /// 0xc50
			 0xf32720d8, /// 0xc54
			 0xe95f30f6, /// 0xc58
			 0x22e13aee, /// 0xc5c
			 0x1318eb11, /// 0xc60
			 0x759a5e59, /// 0xc64
			 0x19291a6d, /// 0xc68
			 0xf60c27d6, /// 0xc6c
			 0x44c5f1b4, /// 0xc70
			 0x9840c071, /// 0xc74
			 0x837fb1f7, /// 0xc78
			 0x69d0301f, /// 0xc7c
			 0x48048f03, /// 0xc80
			 0x1fd03005, /// 0xc84
			 0x798a29ca, /// 0xc88
			 0xa1e54048, /// 0xc8c
			 0xaca9663f, /// 0xc90
			 0x2b103419, /// 0xc94
			 0xb7f52d0d, /// 0xc98
			 0x9206eb2f, /// 0xc9c
			 0xaa41da3d, /// 0xca0
			 0xc1728d39, /// 0xca4
			 0x5e9dfa99, /// 0xca8
			 0xdd10feff, /// 0xcac
			 0xc6ea523a, /// 0xcb0
			 0x74075225, /// 0xcb4
			 0xd74fa288, /// 0xcb8
			 0x8d25ef75, /// 0xcbc
			 0xfec970eb, /// 0xcc0
			 0xf4616e81, /// 0xcc4
			 0x3e173a38, /// 0xcc8
			 0xe938f696, /// 0xccc
			 0x5809f004, /// 0xcd0
			 0x2f6ed7f7, /// 0xcd4
			 0x396b45d0, /// 0xcd8
			 0xf55f780e, /// 0xcdc
			 0xe2b2608d, /// 0xce0
			 0x4562f2dd, /// 0xce4
			 0x5d7ecc81, /// 0xce8
			 0x3a9b6fd1, /// 0xcec
			 0x31435e8c, /// 0xcf0
			 0x0f83da97, /// 0xcf4
			 0x7658ef17, /// 0xcf8
			 0x38ef61bc, /// 0xcfc
			 0xc3570ceb, /// 0xd00
			 0xd71b2b7a, /// 0xd04
			 0x1d988624, /// 0xd08
			 0x01384bcb, /// 0xd0c
			 0x5c1ba3d6, /// 0xd10
			 0x81627858, /// 0xd14
			 0x4d132665, /// 0xd18
			 0x0517462b, /// 0xd1c
			 0x893b1bc5, /// 0xd20
			 0x6aa809d1, /// 0xd24
			 0xa8f52e66, /// 0xd28
			 0xfab8e5a8, /// 0xd2c
			 0x0b210ecf, /// 0xd30
			 0xd0058f37, /// 0xd34
			 0x0d2cdc39, /// 0xd38
			 0xfc10e11e, /// 0xd3c
			 0xbb906b21, /// 0xd40
			 0x8cbbafaa, /// 0xd44
			 0x8736b408, /// 0xd48
			 0x25d27a44, /// 0xd4c
			 0xf006f51a, /// 0xd50
			 0x96d8cbf8, /// 0xd54
			 0xfd1daa0f, /// 0xd58
			 0x9137346a, /// 0xd5c
			 0x9e1df240, /// 0xd60
			 0xafc8f19c, /// 0xd64
			 0xea3ff68d, /// 0xd68
			 0x5b4adfd4, /// 0xd6c
			 0xd4a7884e, /// 0xd70
			 0x9bd73059, /// 0xd74
			 0x7c3135c6, /// 0xd78
			 0x5fc9c936, /// 0xd7c
			 0x52b1b963, /// 0xd80
			 0x9b30b3c4, /// 0xd84
			 0x5fcf3eed, /// 0xd88
			 0xa9c4f99d, /// 0xd8c
			 0x20e8d464, /// 0xd90
			 0x4c2802f1, /// 0xd94
			 0xe21d4768, /// 0xd98
			 0xbd9fb7df, /// 0xd9c
			 0xebd03382, /// 0xda0
			 0x98995d0a, /// 0xda4
			 0x173e0169, /// 0xda8
			 0xe0bb74e0, /// 0xdac
			 0x05c14a46, /// 0xdb0
			 0x609a5e5d, /// 0xdb4
			 0x63b28126, /// 0xdb8
			 0xa422b5d8, /// 0xdbc
			 0x4b641151, /// 0xdc0
			 0x5a46cc53, /// 0xdc4
			 0xe482a00f, /// 0xdc8
			 0x4b11d3e6, /// 0xdcc
			 0xe3ed6a3d, /// 0xdd0
			 0xacbcc0b9, /// 0xdd4
			 0xfe923f21, /// 0xdd8
			 0x10efd85c, /// 0xddc
			 0x247ccfe9, /// 0xde0
			 0xabf4dc14, /// 0xde4
			 0xae80f1d2, /// 0xde8
			 0x6d0ee58e, /// 0xdec
			 0xdf22c4cb, /// 0xdf0
			 0xe5b58a5e, /// 0xdf4
			 0x68c5890e, /// 0xdf8
			 0x970127f7, /// 0xdfc
			 0xaa4b7856, /// 0xe00
			 0xa3e0f196, /// 0xe04
			 0x25d17700, /// 0xe08
			 0xe91468a9, /// 0xe0c
			 0x35dab5a3, /// 0xe10
			 0xf97e0bbf, /// 0xe14
			 0x96477f9d, /// 0xe18
			 0xea6014a3, /// 0xe1c
			 0x9d90a30f, /// 0xe20
			 0xbefe7073, /// 0xe24
			 0x90f395d5, /// 0xe28
			 0xdbf8fe95, /// 0xe2c
			 0x4f588f6f, /// 0xe30
			 0x40504287, /// 0xe34
			 0x10fd8bda, /// 0xe38
			 0xd9391266, /// 0xe3c
			 0xbdc97d93, /// 0xe40
			 0x505b3cb9, /// 0xe44
			 0xd729631f, /// 0xe48
			 0x7421e8bb, /// 0xe4c
			 0x6083b5e4, /// 0xe50
			 0x9de2d9f5, /// 0xe54
			 0xcf9ce943, /// 0xe58
			 0x0b0e2993, /// 0xe5c
			 0x9701688f, /// 0xe60
			 0xd23d7132, /// 0xe64
			 0xf13674cb, /// 0xe68
			 0x497a6930, /// 0xe6c
			 0x8917255d, /// 0xe70
			 0x4b5afe20, /// 0xe74
			 0xa2d24117, /// 0xe78
			 0x631ed5c1, /// 0xe7c
			 0xaf40e702, /// 0xe80
			 0xd0a8e59d, /// 0xe84
			 0x6994a0cc, /// 0xe88
			 0x12d48748, /// 0xe8c
			 0x2f8901fc, /// 0xe90
			 0xf9e38c63, /// 0xe94
			 0xb95854df, /// 0xe98
			 0x21ecef98, /// 0xe9c
			 0x2580f5f6, /// 0xea0
			 0xcf0e85ac, /// 0xea4
			 0xb03959fc, /// 0xea8
			 0xfb69c87b, /// 0xeac
			 0x892b58e0, /// 0xeb0
			 0xf8e8556d, /// 0xeb4
			 0xcf166c8c, /// 0xeb8
			 0x40615803, /// 0xebc
			 0xb742865a, /// 0xec0
			 0x8abaa2ab, /// 0xec4
			 0xa09283ed, /// 0xec8
			 0xb125ff63, /// 0xecc
			 0xb142b296, /// 0xed0
			 0x88a75f5e, /// 0xed4
			 0x6ee2c28f, /// 0xed8
			 0x23403454, /// 0xedc
			 0x525dec4a, /// 0xee0
			 0x6b0fcc2a, /// 0xee4
			 0xcb2f3106, /// 0xee8
			 0x89d19b38, /// 0xeec
			 0xd90a038c, /// 0xef0
			 0xfc2053b2, /// 0xef4
			 0xbfca3c7f, /// 0xef8
			 0x21c8f102, /// 0xefc
			 0x26238a47, /// 0xf00
			 0x8928db1c, /// 0xf04
			 0xd02ba2d9, /// 0xf08
			 0xb9f70249, /// 0xf0c
			 0x9c2333e8, /// 0xf10
			 0xbc03824a, /// 0xf14
			 0x04f5d36d, /// 0xf18
			 0xc83322e9, /// 0xf1c
			 0xe5e3c31d, /// 0xf20
			 0xf6fc1f02, /// 0xf24
			 0xbaaa5436, /// 0xf28
			 0xe5f23e94, /// 0xf2c
			 0x68c2edc8, /// 0xf30
			 0x1c22cc2b, /// 0xf34
			 0x8a9ee074, /// 0xf38
			 0x11ade780, /// 0xf3c
			 0x5485e459, /// 0xf40
			 0x68178f1c, /// 0xf44
			 0xcdbc1d6c, /// 0xf48
			 0xf229e550, /// 0xf4c
			 0x5533bf93, /// 0xf50
			 0x1aa06792, /// 0xf54
			 0x9a81adf1, /// 0xf58
			 0x8c8b2f7a, /// 0xf5c
			 0xb56dc4ac, /// 0xf60
			 0x9e11c985, /// 0xf64
			 0x0081c290, /// 0xf68
			 0xb7304284, /// 0xf6c
			 0x76c134d6, /// 0xf70
			 0xd79bf682, /// 0xf74
			 0x977f9612, /// 0xf78
			 0xa6a51d2a, /// 0xf7c
			 0x1c32b893, /// 0xf80
			 0xf623a938, /// 0xf84
			 0xe052974f, /// 0xf88
			 0x0087cbd8, /// 0xf8c
			 0xb1bfe788, /// 0xf90
			 0x04a38b29, /// 0xf94
			 0x3822a4f4, /// 0xf98
			 0xad0206e0, /// 0xf9c
			 0x006a88fa, /// 0xfa0
			 0x1f9195e4, /// 0xfa4
			 0x97ebd92b, /// 0xfa8
			 0xf77443df, /// 0xfac
			 0xaf0b98d8, /// 0xfb0
			 0x6227ec7a, /// 0xfb4
			 0xdfb9438c, /// 0xfb8
			 0x132c1cf3, /// 0xfbc
			 0x9c5d18c2, /// 0xfc0
			 0x57c5b7ce, /// 0xfc4
			 0xbd9fab15, /// 0xfc8
			 0xc10253f0, /// 0xfcc
			 0xb6920c3a, /// 0xfd0
			 0xe24c63a2, /// 0xfd4
			 0xe6275736, /// 0xfd8
			 0x3478d3fd, /// 0xfdc
			 0x77bed6a6, /// 0xfe0
			 0x8ec1a81f, /// 0xfe4
			 0xc921bca7, /// 0xfe8
			 0x0d6c4e69, /// 0xfec
			 0x01c55cb7, /// 0xff0
			 0xbcd2ff3b, /// 0xff4
			 0x9dd5beb7, /// 0xff8
			 0xe0e2ca6e /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00004
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0000c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00014
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0c400000,                                                  // Leading 1s:                                 /// 0001c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00028
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0002c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00034
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00038
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00044
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00048
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0xffc00001,                                                  // -signaling NaN                              /// 00050
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00058
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0005c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00064
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00068
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0006c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00070
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00074
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00078
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0007c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00080
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00090
			 0x80011111,                                                  // -9.7958E-41                                 /// 00094
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00098
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0009c
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0xffc00001,                                                  // -signaling NaN                              /// 000c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000c8
			 0x33333333,                                                  // 4 random values                             /// 000cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000d4
			 0x3f028f5c,                                                  // 0.51                                        /// 000d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00100
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00108
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00110
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00114
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00118
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00120
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00128
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0012c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00130
			 0x0c700000,                                                  // Leading 1s:                                 /// 00134
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00138
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0013c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00140
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00148
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0014c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00150
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00158
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00160
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00164
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00168
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00170
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00174
			 0x0e000003,                                                  // Trailing 1s:                                /// 00178
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0017c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00180
			 0x80000000,                                                  // -zero                                       /// 00184
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00188
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0018c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00190
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00194
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00198
			 0xcb000000,                                                  // -8388608.0                                  /// 0019c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001a0
			 0xff800000,                                                  // -inf                                        /// 001a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001dc
			 0x3f028f5c,                                                  // 0.51                                        /// 001e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x3f028f5c,                                                  // 0.51                                        /// 00204
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0020c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00214
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00218
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00224
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00228
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0022c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00230
			 0x0e000003,                                                  // Trailing 1s:                                /// 00234
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00238
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0023c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00240
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00250
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00258
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00260
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00264
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00270
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00274
			 0x0e000001,                                                  // Trailing 1s:                                /// 00278
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00284
			 0x55555555,                                                  // 4 random values                             /// 00288
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0028c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00294
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00298
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x4b000000,                                                  // 8388608.0                                   /// 002a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002b0
			 0x80000000,                                                  // -zero                                       /// 002b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 002e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0e000007,                                                  // Trailing 1s:                                /// 0030c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00314
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00318
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0031c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00320
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00324
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00328
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0032c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00330
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0033c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00344
			 0x7fc00001,                                                  // signaling NaN                               /// 00348
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00350
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00354
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00358
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00360
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00364
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00368
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0036c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00370
			 0xff800000,                                                  // -inf                                        /// 00374
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00378
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0037c
			 0xff800000,                                                  // -inf                                        /// 00380
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00388
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0038c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00390
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00394
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0039c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00400
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0c780000,                                                  // Leading 1s:                                 /// 00408
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0040c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00410
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0e000003,                                                  // Trailing 1s:                                /// 0041c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00420
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00424
			 0x0c700000,                                                  // Leading 1s:                                 /// 00428
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0042c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00430
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0e000001,                                                  // Trailing 1s:                                /// 00438
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0043c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0044c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00450
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00454
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0c400000,                                                  // Leading 1s:                                 /// 0045c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00460
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00464
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00468
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0046c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00474
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00478
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00484
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0048c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00498
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0xff800000,                                                  // -inf                                        /// 004ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004b4
			 0xffc00001,                                                  // -signaling NaN                              /// 004b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c8
			 0x7f800000,                                                  // inf                                         /// 004cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f0
			 0x33333333,                                                  // 4 random values                             /// 004f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00508
			 0x0c700000,                                                  // Leading 1s:                                 /// 0050c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00510
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00514
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0051c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x4b000000,                                                  // 8388608.0                                   /// 00524
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0052c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00530
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0053c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00544
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00554
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00558
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00560
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00568
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0056c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00570
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00578
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0057c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00580
			 0x80011111,                                                  // -9.7958E-41                                 /// 00584
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0058c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00590
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00594
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0059c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0xcb000000,                                                  // -8388608.0                                  /// 005e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00600
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00604
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00608
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0060c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00610
			 0x80011111,                                                  // -9.7958E-41                                 /// 00614
			 0x0c400000,                                                  // Leading 1s:                                 /// 00618
			 0x4b000000,                                                  // 8388608.0                                   /// 0061c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00628
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0062c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00630
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00638
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x3f028f5c,                                                  // 0.51                                        /// 00640
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00644
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00648
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0064c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00650
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00654
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00658
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0065c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00664
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x3f028f5c,                                                  // 0.51                                        /// 0066c
			 0xff800000,                                                  // -inf                                        /// 00670
			 0x0e000003,                                                  // Trailing 1s:                                /// 00674
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0067c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00680
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00688
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x3f028f5c,                                                  // 0.51                                        /// 00690
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00694
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00698
			 0x33333333,                                                  // 4 random values                             /// 0069c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x33333333,                                                  // 4 random values                             /// 00700
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00704
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00708
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0070c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00710
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00714
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00718
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00720
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00724
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00728
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00730
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00738
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0073c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00740
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00744
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0074c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00758
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0075c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00760
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00764
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00768
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0076c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00778
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0077c
			 0x4b000000,                                                  // 8388608.0                                   /// 00780
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00790
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00794
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00798
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0079c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x80000000,                                                  // -zero                                       /// 007b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007cc
			 0xcb000000,                                                  // -8388608.0                                  /// 007d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 007e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00808
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00810
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00814
			 0x0c700000,                                                  // Leading 1s:                                 /// 00818
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00820
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0xbf028f5c,                                                  // -0.51                                       /// 00828
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00834
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00840
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00844
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00848
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80000000,                                                  // -zero                                       /// 00850
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00854
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00858
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0085c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00860
			 0x0e000001,                                                  // Trailing 1s:                                /// 00864
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0086c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00874
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c600000,                                                  // Leading 1s:                                 /// 00888
			 0x55555555,                                                  // 4 random values                             /// 0088c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00890
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00894
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00898
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0089c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 008c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x3f028f5c,                                                  // 0.51                                        /// 008e0
			 0xcb000000,                                                  // -8388608.0                                  /// 008e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00900
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0090c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00910
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00914
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00920
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00924
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00928
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0092c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00930
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00934
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00938
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00940
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00944
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0094c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00950
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00954
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00958
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0095c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x33333333,                                                  // 4 random values                             /// 00964
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00968
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0xbf028f5c,                                                  // -0.51                                       /// 00970
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00978
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00984
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0c700000,                                                  // Leading 1s:                                 /// 0098c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00990
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00994
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0099c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0xbf028f5c,                                                  // -0.51                                       /// 009d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a04
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a10
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a2c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a34
			 0x4b000000,                                                  // 8388608.0                                   /// 00a38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a5c
			 0x00000000,                                                  // zero                                        /// 00a60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a6c
			 0x33333333,                                                  // 4 random values                             /// 00a70
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a84
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00aa4
			 0xff800000,                                                  // -inf                                        /// 00aa8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ab4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ac0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ac8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ad0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ad8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00adc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ae0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ae4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ae8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00af0
			 0xbf028f5c,                                                  // -0.51                                       /// 00af4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b48
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b58
			 0x7fc00001,                                                  // signaling NaN                               /// 00b5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b74
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b88
			 0x3f028f5c,                                                  // 0.51                                        /// 00b8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b98
			 0x33333333,                                                  // 4 random values                             /// 00b9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ba0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ba4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bb0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bb4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bb8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bcc
			 0x00000000,                                                  // zero                                        /// 00bd0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bd4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bdc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00be0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00be4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bf0
			 0x3f028f5c,                                                  // 0.51                                        /// 00bf4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf8
			 0x00000000,                                                  // zero                                        /// 00bfc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c14
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x7f800000,                                                  // inf                                         /// 00c2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c34
			 0x33333333,                                                  // 4 random values                             /// 00c38
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c48
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c4c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c60
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c6c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c70
			 0x7f800000,                                                  // inf                                         /// 00c74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cb4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cbc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cd0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cdc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ce0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ce4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cf4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cf8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cfc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x00000000,                                                  // zero                                        /// 00d08
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d10
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d20
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d34
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x33333333,                                                  // 4 random values                             /// 00d40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d78
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d84
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d8c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00da0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00db0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dcc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0xbf028f5c,                                                  // -0.51                                       /// 00dd4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00de4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00df0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00df4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00df8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dfc
			 0xcb000000,                                                  // -8388608.0                                  /// 00e00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e30
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e40
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0xcb000000,                                                  // -8388608.0                                  /// 00e58
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e60
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e88
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e9c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ea4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eb0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eb4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00eb8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ebc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ed0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00edc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ee0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ee4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ee8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ef8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f24
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f80
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fa0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fa4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fb0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fc4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fe0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fe4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fe8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fec
			 0x33333333,                                                  // 4 random values                             /// 00ff0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ff4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ff8
			 0x00400000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x70da6d07, /// 0x0
			 0xe27cb179, /// 0x4
			 0x9a3ef4c8, /// 0x8
			 0xf3127233, /// 0xc
			 0x3425f06e, /// 0x10
			 0x50c4f2dd, /// 0x14
			 0x7b883978, /// 0x18
			 0x292248d3, /// 0x1c
			 0x78e9373e, /// 0x20
			 0x08b5ad76, /// 0x24
			 0xcabf5031, /// 0x28
			 0xa12ef623, /// 0x2c
			 0x8586ac70, /// 0x30
			 0xc66e8ea9, /// 0x34
			 0x051570bb, /// 0x38
			 0x5d9e0824, /// 0x3c
			 0x0ce87daf, /// 0x40
			 0xfe1063d7, /// 0x44
			 0xaf91fd35, /// 0x48
			 0x0c9f72bb, /// 0x4c
			 0xe9aa192f, /// 0x50
			 0x0d09861a, /// 0x54
			 0xd781964c, /// 0x58
			 0x5377ed4d, /// 0x5c
			 0x4dfab484, /// 0x60
			 0x6866b2c7, /// 0x64
			 0x9dfd083d, /// 0x68
			 0x95533b66, /// 0x6c
			 0x296d7790, /// 0x70
			 0x28dff88c, /// 0x74
			 0x8e80ad01, /// 0x78
			 0x2412c5a8, /// 0x7c
			 0x347fa2e0, /// 0x80
			 0x63eb6aea, /// 0x84
			 0x7c4f7f28, /// 0x88
			 0x7a4ce3b0, /// 0x8c
			 0x0b5ef483, /// 0x90
			 0x0da04baf, /// 0x94
			 0x3d7f5ae2, /// 0x98
			 0x8cc892b6, /// 0x9c
			 0x1a7eaaca, /// 0xa0
			 0x1dba02e9, /// 0xa4
			 0x3805a0db, /// 0xa8
			 0x61e4d057, /// 0xac
			 0xa883f65e, /// 0xb0
			 0x8c30278b, /// 0xb4
			 0x7d185e51, /// 0xb8
			 0x07c42872, /// 0xbc
			 0x361fc2b4, /// 0xc0
			 0x3ce0b682, /// 0xc4
			 0x46bd1696, /// 0xc8
			 0x242e4505, /// 0xcc
			 0xc75f440f, /// 0xd0
			 0x8bf2deee, /// 0xd4
			 0x27f6d9d2, /// 0xd8
			 0xdbac5bed, /// 0xdc
			 0x6b86902c, /// 0xe0
			 0x6c5b1908, /// 0xe4
			 0x2a1b3a70, /// 0xe8
			 0xcff27480, /// 0xec
			 0xf9337dd3, /// 0xf0
			 0x98a51462, /// 0xf4
			 0x0e20978f, /// 0xf8
			 0xc700979e, /// 0xfc
			 0x197d0318, /// 0x100
			 0x3f6b6420, /// 0x104
			 0x70bbb4be, /// 0x108
			 0xf13eeed0, /// 0x10c
			 0x8d23f372, /// 0x110
			 0x5347b5f0, /// 0x114
			 0xce37fe38, /// 0x118
			 0xefc2e30d, /// 0x11c
			 0xe8ebded0, /// 0x120
			 0x2e95f781, /// 0x124
			 0xed9ca995, /// 0x128
			 0xfb8660b6, /// 0x12c
			 0x77e3dd70, /// 0x130
			 0x7ba6f026, /// 0x134
			 0x0c385c92, /// 0x138
			 0x8233c0eb, /// 0x13c
			 0x72a3f73b, /// 0x140
			 0x734f9498, /// 0x144
			 0x3470b554, /// 0x148
			 0xeccc7952, /// 0x14c
			 0x9b12a07c, /// 0x150
			 0xfb09e558, /// 0x154
			 0x3d5c1783, /// 0x158
			 0xe1036dde, /// 0x15c
			 0x83c3d19e, /// 0x160
			 0x47686cf4, /// 0x164
			 0xd11524d9, /// 0x168
			 0x54acf53d, /// 0x16c
			 0x891bbc25, /// 0x170
			 0x941651e6, /// 0x174
			 0x80071dff, /// 0x178
			 0x0a9593fa, /// 0x17c
			 0x734fd6a9, /// 0x180
			 0x9342007a, /// 0x184
			 0x88b31664, /// 0x188
			 0xeb83ae9a, /// 0x18c
			 0x4a313a47, /// 0x190
			 0x9e102a28, /// 0x194
			 0x4806c051, /// 0x198
			 0x0a8a24b3, /// 0x19c
			 0x158c785e, /// 0x1a0
			 0x52b5be56, /// 0x1a4
			 0x16f6982e, /// 0x1a8
			 0x1f2c40ee, /// 0x1ac
			 0x6a4c625b, /// 0x1b0
			 0xd7a80c17, /// 0x1b4
			 0x9c129dce, /// 0x1b8
			 0x18008ba7, /// 0x1bc
			 0xc0d56c24, /// 0x1c0
			 0xde3f15f9, /// 0x1c4
			 0xe8922913, /// 0x1c8
			 0xc1712fe6, /// 0x1cc
			 0x393a6054, /// 0x1d0
			 0x0cd1633d, /// 0x1d4
			 0x22a72ae7, /// 0x1d8
			 0xdcd2a763, /// 0x1dc
			 0x07625bc0, /// 0x1e0
			 0x16e84693, /// 0x1e4
			 0xf1c248b6, /// 0x1e8
			 0x8e151fa7, /// 0x1ec
			 0x4e8e2809, /// 0x1f0
			 0x57d6d678, /// 0x1f4
			 0xbe8344e2, /// 0x1f8
			 0x3c061019, /// 0x1fc
			 0x6ba02e1c, /// 0x200
			 0x54b30071, /// 0x204
			 0xc40583c2, /// 0x208
			 0xc1e97d89, /// 0x20c
			 0xb61b64b3, /// 0x210
			 0x17662509, /// 0x214
			 0x35b72924, /// 0x218
			 0xf7afa258, /// 0x21c
			 0x4357c376, /// 0x220
			 0x766211e7, /// 0x224
			 0x45416bb0, /// 0x228
			 0x1f041da0, /// 0x22c
			 0xfd5615d1, /// 0x230
			 0x2c6f8c1e, /// 0x234
			 0xc165c720, /// 0x238
			 0x0f598357, /// 0x23c
			 0xfaa8966e, /// 0x240
			 0xe94891ac, /// 0x244
			 0x6a08cbde, /// 0x248
			 0x7b6a0297, /// 0x24c
			 0x7756c735, /// 0x250
			 0x7c61e5ee, /// 0x254
			 0xd232b6ef, /// 0x258
			 0xc4a5da14, /// 0x25c
			 0x11ad79d9, /// 0x260
			 0xf9c234a1, /// 0x264
			 0xbcba23af, /// 0x268
			 0x976ab50e, /// 0x26c
			 0xf5111a3d, /// 0x270
			 0xefa0bf3e, /// 0x274
			 0x56a9358e, /// 0x278
			 0x13df17bd, /// 0x27c
			 0x3b99eb9e, /// 0x280
			 0x14c20642, /// 0x284
			 0x32eb6cec, /// 0x288
			 0x4ef5cd8a, /// 0x28c
			 0x5eaf5a5a, /// 0x290
			 0xedcec703, /// 0x294
			 0xd163f07b, /// 0x298
			 0xe12f7841, /// 0x29c
			 0xd4990b25, /// 0x2a0
			 0xadd91b94, /// 0x2a4
			 0xdcbda864, /// 0x2a8
			 0x7fcd2da9, /// 0x2ac
			 0x8ca41a44, /// 0x2b0
			 0xdd543c00, /// 0x2b4
			 0x952700c2, /// 0x2b8
			 0x68dac8f5, /// 0x2bc
			 0xfa6e22b1, /// 0x2c0
			 0x8b3e970b, /// 0x2c4
			 0xd34edf5b, /// 0x2c8
			 0x10ca0f89, /// 0x2cc
			 0xd534c72f, /// 0x2d0
			 0x287cdac5, /// 0x2d4
			 0x768a69b7, /// 0x2d8
			 0x7695a177, /// 0x2dc
			 0x395edd1b, /// 0x2e0
			 0x54f3eeee, /// 0x2e4
			 0xdcfff505, /// 0x2e8
			 0xd9403c6a, /// 0x2ec
			 0x27a5617c, /// 0x2f0
			 0x3fee77db, /// 0x2f4
			 0x8200c874, /// 0x2f8
			 0x242bbfb9, /// 0x2fc
			 0x3f34aac0, /// 0x300
			 0x14d5de41, /// 0x304
			 0x88bca4ee, /// 0x308
			 0x0c70c308, /// 0x30c
			 0xdbc8ea2f, /// 0x310
			 0x2546f4f5, /// 0x314
			 0x8d89ddc9, /// 0x318
			 0xf3c480f3, /// 0x31c
			 0xf54d484f, /// 0x320
			 0xa4e4b500, /// 0x324
			 0xca059f5a, /// 0x328
			 0x6499c55c, /// 0x32c
			 0x4374e9aa, /// 0x330
			 0xb4818545, /// 0x334
			 0x0c414793, /// 0x338
			 0x8d8784ed, /// 0x33c
			 0x0b4a4fb5, /// 0x340
			 0x06bd11ba, /// 0x344
			 0xab9f4bc2, /// 0x348
			 0x23e364ad, /// 0x34c
			 0xcc4bd5ce, /// 0x350
			 0x875196c8, /// 0x354
			 0xdff106bc, /// 0x358
			 0xcc738c65, /// 0x35c
			 0x4a3221fc, /// 0x360
			 0x3068d695, /// 0x364
			 0x851cf7d6, /// 0x368
			 0x0838ca10, /// 0x36c
			 0xff2926d8, /// 0x370
			 0x85a64710, /// 0x374
			 0x64f74523, /// 0x378
			 0x7907108b, /// 0x37c
			 0x5e015356, /// 0x380
			 0x1f48a3a4, /// 0x384
			 0x4fb93c35, /// 0x388
			 0x6de0cbde, /// 0x38c
			 0x5390558b, /// 0x390
			 0x434a297e, /// 0x394
			 0xeda4837a, /// 0x398
			 0x2c8fff2b, /// 0x39c
			 0x35bcffca, /// 0x3a0
			 0xe0fde0b3, /// 0x3a4
			 0x1a1c342a, /// 0x3a8
			 0xc1aad639, /// 0x3ac
			 0x8eae4373, /// 0x3b0
			 0x4ca753ee, /// 0x3b4
			 0x356d5da4, /// 0x3b8
			 0x61d68167, /// 0x3bc
			 0xbd53f421, /// 0x3c0
			 0x6555cc8c, /// 0x3c4
			 0x478c8f10, /// 0x3c8
			 0xfa415677, /// 0x3cc
			 0x3c862c8a, /// 0x3d0
			 0xb0c8c898, /// 0x3d4
			 0xc310b83a, /// 0x3d8
			 0xebcf460f, /// 0x3dc
			 0xeaa808ac, /// 0x3e0
			 0x9f2327f9, /// 0x3e4
			 0x753688af, /// 0x3e8
			 0xee3f2881, /// 0x3ec
			 0x46c00a38, /// 0x3f0
			 0xb5c90992, /// 0x3f4
			 0x646199b5, /// 0x3f8
			 0x5ba4bd68, /// 0x3fc
			 0x3036576e, /// 0x400
			 0xe1def50d, /// 0x404
			 0x22fa8be0, /// 0x408
			 0x962b19c9, /// 0x40c
			 0xbe17c21a, /// 0x410
			 0xd93fe51d, /// 0x414
			 0xfa7ba3a9, /// 0x418
			 0x30a277d1, /// 0x41c
			 0x7813e4d3, /// 0x420
			 0xb24bf1cf, /// 0x424
			 0x300f084c, /// 0x428
			 0x07d4a265, /// 0x42c
			 0x28188ae0, /// 0x430
			 0x6ad83028, /// 0x434
			 0x3cd8d5f1, /// 0x438
			 0x0da47602, /// 0x43c
			 0x3592ea06, /// 0x440
			 0xf77bed2d, /// 0x444
			 0x156e5328, /// 0x448
			 0x664693f5, /// 0x44c
			 0x23869b8d, /// 0x450
			 0xa1500ee4, /// 0x454
			 0x57840ac4, /// 0x458
			 0x2fd4c5da, /// 0x45c
			 0x0f05397b, /// 0x460
			 0xccc4ad01, /// 0x464
			 0x1ac5bac3, /// 0x468
			 0x2ea6b8b3, /// 0x46c
			 0xffd068e5, /// 0x470
			 0x0627e6df, /// 0x474
			 0xa9d6ed10, /// 0x478
			 0x8cf66d96, /// 0x47c
			 0x8e79132c, /// 0x480
			 0x2d972a89, /// 0x484
			 0x9ba4fd15, /// 0x488
			 0x84791c61, /// 0x48c
			 0x0075476d, /// 0x490
			 0xae0a3bfc, /// 0x494
			 0x5a34aa27, /// 0x498
			 0x30f1cb13, /// 0x49c
			 0x38c608f0, /// 0x4a0
			 0x6f3e800e, /// 0x4a4
			 0xde4e82ff, /// 0x4a8
			 0x343084e0, /// 0x4ac
			 0xeb9d6a6f, /// 0x4b0
			 0xa4502c27, /// 0x4b4
			 0xeb3ab319, /// 0x4b8
			 0xf6a30e41, /// 0x4bc
			 0x4c90053a, /// 0x4c0
			 0xfe41cc42, /// 0x4c4
			 0xc2f756b4, /// 0x4c8
			 0x79acd41d, /// 0x4cc
			 0x4084472c, /// 0x4d0
			 0x52d9bba3, /// 0x4d4
			 0xf3e0373d, /// 0x4d8
			 0xe4cf1df1, /// 0x4dc
			 0x4578d58a, /// 0x4e0
			 0xa5197104, /// 0x4e4
			 0x875de62f, /// 0x4e8
			 0x5798e662, /// 0x4ec
			 0x855d7871, /// 0x4f0
			 0xefd76dd3, /// 0x4f4
			 0x4bef5256, /// 0x4f8
			 0x778e5e2d, /// 0x4fc
			 0x4ae6c7d6, /// 0x500
			 0x59ebe1d6, /// 0x504
			 0x6c7002f2, /// 0x508
			 0x6e1c6efd, /// 0x50c
			 0x2d6ba5d3, /// 0x510
			 0x1804cd81, /// 0x514
			 0x467ce234, /// 0x518
			 0x0509db9f, /// 0x51c
			 0xe1da638d, /// 0x520
			 0xc981ecc1, /// 0x524
			 0x3930d536, /// 0x528
			 0x70af36bc, /// 0x52c
			 0x62a7bd39, /// 0x530
			 0x8ee072b9, /// 0x534
			 0x9448f49e, /// 0x538
			 0x64e1bbbe, /// 0x53c
			 0xcaaecae5, /// 0x540
			 0x574b93cd, /// 0x544
			 0x0d116d20, /// 0x548
			 0xa56c423d, /// 0x54c
			 0x45af8aeb, /// 0x550
			 0xf160b575, /// 0x554
			 0x186e24bc, /// 0x558
			 0xbc258149, /// 0x55c
			 0x3726399c, /// 0x560
			 0xa8e29de9, /// 0x564
			 0x6160fa59, /// 0x568
			 0xd4e61a61, /// 0x56c
			 0xbecf82f7, /// 0x570
			 0xe1eaac90, /// 0x574
			 0xde2b4e2d, /// 0x578
			 0x971456d2, /// 0x57c
			 0x2c75fc86, /// 0x580
			 0x3fcd4c5f, /// 0x584
			 0xe0499898, /// 0x588
			 0x0f0d450e, /// 0x58c
			 0x116693eb, /// 0x590
			 0x2eba4663, /// 0x594
			 0xdda94491, /// 0x598
			 0x8160e57f, /// 0x59c
			 0x748477ef, /// 0x5a0
			 0x726eea71, /// 0x5a4
			 0xd9bad388, /// 0x5a8
			 0x4b9751fd, /// 0x5ac
			 0x2c002604, /// 0x5b0
			 0xab192936, /// 0x5b4
			 0x72e78e94, /// 0x5b8
			 0x7aff6f4a, /// 0x5bc
			 0xe4114abc, /// 0x5c0
			 0x4599ca86, /// 0x5c4
			 0x0e04d9b7, /// 0x5c8
			 0x72ea7113, /// 0x5cc
			 0xf32982f7, /// 0x5d0
			 0xdc3d13ce, /// 0x5d4
			 0x52b44568, /// 0x5d8
			 0xdf635aed, /// 0x5dc
			 0xf97c8601, /// 0x5e0
			 0xb8492ba8, /// 0x5e4
			 0x4ad06c7d, /// 0x5e8
			 0x2ced4333, /// 0x5ec
			 0xa13b2593, /// 0x5f0
			 0x4542f432, /// 0x5f4
			 0xcdbf920b, /// 0x5f8
			 0xc3391fcc, /// 0x5fc
			 0xde2d1ded, /// 0x600
			 0x7c5d87aa, /// 0x604
			 0x33d9e675, /// 0x608
			 0xa4e765ca, /// 0x60c
			 0x5e6f3e9c, /// 0x610
			 0x49b945f2, /// 0x614
			 0x978bdc7b, /// 0x618
			 0x028187e5, /// 0x61c
			 0x555b049e, /// 0x620
			 0x3381e66b, /// 0x624
			 0x2b1adbd3, /// 0x628
			 0xff5ea7a6, /// 0x62c
			 0x83416494, /// 0x630
			 0x5c0c563d, /// 0x634
			 0x86084451, /// 0x638
			 0x9447a0e9, /// 0x63c
			 0x6b6c368c, /// 0x640
			 0xf56f163a, /// 0x644
			 0xf998180d, /// 0x648
			 0xcc20fd00, /// 0x64c
			 0x017c1c96, /// 0x650
			 0x5e20fb3c, /// 0x654
			 0xd459a8cd, /// 0x658
			 0xcb8313cc, /// 0x65c
			 0xbc16e8a7, /// 0x660
			 0x7b5c7fd6, /// 0x664
			 0x38163aa0, /// 0x668
			 0x1077efb7, /// 0x66c
			 0x0d1193c7, /// 0x670
			 0xb0b67854, /// 0x674
			 0x4db45727, /// 0x678
			 0x617b9093, /// 0x67c
			 0x2703107f, /// 0x680
			 0xa685a377, /// 0x684
			 0x9208b7f5, /// 0x688
			 0x8b03b5f0, /// 0x68c
			 0xe29e38eb, /// 0x690
			 0x1f242ecf, /// 0x694
			 0x9f003a66, /// 0x698
			 0x0724f8aa, /// 0x69c
			 0x4d2f271f, /// 0x6a0
			 0x8eede71f, /// 0x6a4
			 0xa731149c, /// 0x6a8
			 0x79eebb45, /// 0x6ac
			 0x7a8d765f, /// 0x6b0
			 0x4ec68731, /// 0x6b4
			 0xd051234a, /// 0x6b8
			 0x651c83d4, /// 0x6bc
			 0xcfdac6b0, /// 0x6c0
			 0xb9a40787, /// 0x6c4
			 0x6ef25dc1, /// 0x6c8
			 0x10509d9e, /// 0x6cc
			 0xcb6c45b2, /// 0x6d0
			 0xd7f2de14, /// 0x6d4
			 0x1124bb11, /// 0x6d8
			 0xf129d5c3, /// 0x6dc
			 0x8f469d10, /// 0x6e0
			 0x99763ec8, /// 0x6e4
			 0x52436b87, /// 0x6e8
			 0xccf37bef, /// 0x6ec
			 0xdd52bea0, /// 0x6f0
			 0x72cbcaf6, /// 0x6f4
			 0x7fa03635, /// 0x6f8
			 0xa70bf0f4, /// 0x6fc
			 0xc39cc442, /// 0x700
			 0xb4ded828, /// 0x704
			 0x7d816a31, /// 0x708
			 0xb46b5f6f, /// 0x70c
			 0xe658b14c, /// 0x710
			 0xa1e54981, /// 0x714
			 0xa7202937, /// 0x718
			 0x66b60c25, /// 0x71c
			 0x846bce91, /// 0x720
			 0xf6d5f7df, /// 0x724
			 0xee9e52dd, /// 0x728
			 0xb017997f, /// 0x72c
			 0x00e2c28a, /// 0x730
			 0x3df2cc6c, /// 0x734
			 0xb0e17e9f, /// 0x738
			 0xe1f7b2c3, /// 0x73c
			 0xd2c837b5, /// 0x740
			 0x380d3421, /// 0x744
			 0xee290721, /// 0x748
			 0x535e4319, /// 0x74c
			 0xc16a030a, /// 0x750
			 0x7ad54d0a, /// 0x754
			 0xe53f963a, /// 0x758
			 0xa168c53d, /// 0x75c
			 0xce4ba6b2, /// 0x760
			 0x0a65ca81, /// 0x764
			 0x0c61b672, /// 0x768
			 0x0264202c, /// 0x76c
			 0x8416c313, /// 0x770
			 0x6eda8ba7, /// 0x774
			 0xe807ea06, /// 0x778
			 0x0a074af1, /// 0x77c
			 0xb518568f, /// 0x780
			 0x06d259ba, /// 0x784
			 0x05104d95, /// 0x788
			 0xec03887f, /// 0x78c
			 0xc97b936a, /// 0x790
			 0x42269998, /// 0x794
			 0xb019164c, /// 0x798
			 0xd5c12d6e, /// 0x79c
			 0x79cd137b, /// 0x7a0
			 0xf92fbef5, /// 0x7a4
			 0xab9f05c5, /// 0x7a8
			 0x7377d8c6, /// 0x7ac
			 0x117c5ccb, /// 0x7b0
			 0x2a1d6452, /// 0x7b4
			 0xb68f2e86, /// 0x7b8
			 0xd684a461, /// 0x7bc
			 0x0025be36, /// 0x7c0
			 0xe2fe99c6, /// 0x7c4
			 0x9b196019, /// 0x7c8
			 0x7683391d, /// 0x7cc
			 0x48e4ade0, /// 0x7d0
			 0xb534307a, /// 0x7d4
			 0xfb22ba28, /// 0x7d8
			 0xfc02d1bc, /// 0x7dc
			 0xd14c6260, /// 0x7e0
			 0xdd0402bb, /// 0x7e4
			 0x7a73a840, /// 0x7e8
			 0x45f08059, /// 0x7ec
			 0x60b41cf1, /// 0x7f0
			 0x37bfbe73, /// 0x7f4
			 0xa47db9ae, /// 0x7f8
			 0xebc4d019, /// 0x7fc
			 0xac8eee2a, /// 0x800
			 0xe3a671f4, /// 0x804
			 0x7b340772, /// 0x808
			 0x1118d065, /// 0x80c
			 0x33b3a17b, /// 0x810
			 0xefd99cf1, /// 0x814
			 0x56506583, /// 0x818
			 0x7b61f00b, /// 0x81c
			 0x03519b44, /// 0x820
			 0xc7063963, /// 0x824
			 0x1bf629c4, /// 0x828
			 0xb381d6f3, /// 0x82c
			 0xf352b964, /// 0x830
			 0x89be5ca0, /// 0x834
			 0x8337869d, /// 0x838
			 0x1bbad408, /// 0x83c
			 0x386db16d, /// 0x840
			 0x772df4e0, /// 0x844
			 0x5f5a355e, /// 0x848
			 0x31b57ded, /// 0x84c
			 0x9c6b8882, /// 0x850
			 0x468f35f7, /// 0x854
			 0x8bc76f59, /// 0x858
			 0x78c0eb17, /// 0x85c
			 0x8e85c73c, /// 0x860
			 0x15b26381, /// 0x864
			 0xe6d0742a, /// 0x868
			 0x4f32baa3, /// 0x86c
			 0xf9170d98, /// 0x870
			 0xbfa19627, /// 0x874
			 0xd21dcbb3, /// 0x878
			 0x7f920ebe, /// 0x87c
			 0xd28d6b11, /// 0x880
			 0xeb350d86, /// 0x884
			 0xcfdead06, /// 0x888
			 0xf3e7d9ba, /// 0x88c
			 0x076078d1, /// 0x890
			 0x992a9453, /// 0x894
			 0xce9becfb, /// 0x898
			 0x38c32887, /// 0x89c
			 0xcd013520, /// 0x8a0
			 0xdf2510b6, /// 0x8a4
			 0xbc4fcd3f, /// 0x8a8
			 0xccf63477, /// 0x8ac
			 0x86b39c92, /// 0x8b0
			 0x13dc9571, /// 0x8b4
			 0xcf79479d, /// 0x8b8
			 0x3ff15d1a, /// 0x8bc
			 0xa9d5af6b, /// 0x8c0
			 0x1d473628, /// 0x8c4
			 0xf4e25a57, /// 0x8c8
			 0x3162a955, /// 0x8cc
			 0xee92ab78, /// 0x8d0
			 0x667a7f87, /// 0x8d4
			 0x6270559f, /// 0x8d8
			 0x5e397a65, /// 0x8dc
			 0x28ee884a, /// 0x8e0
			 0x91459783, /// 0x8e4
			 0xd0950c4e, /// 0x8e8
			 0xe960deb1, /// 0x8ec
			 0x76766148, /// 0x8f0
			 0x3c7940d4, /// 0x8f4
			 0xf2230fe9, /// 0x8f8
			 0xc8b859fb, /// 0x8fc
			 0xdcf1a551, /// 0x900
			 0xca3a65df, /// 0x904
			 0xf54756ce, /// 0x908
			 0x3abe0e22, /// 0x90c
			 0xefd401b7, /// 0x910
			 0xc408bc96, /// 0x914
			 0x62402c6f, /// 0x918
			 0x8aa37461, /// 0x91c
			 0x4268638a, /// 0x920
			 0x906dcacb, /// 0x924
			 0x83b9d899, /// 0x928
			 0x818df777, /// 0x92c
			 0xa5b64ce1, /// 0x930
			 0x06b5e7ae, /// 0x934
			 0x016d74b2, /// 0x938
			 0x60bdb2c8, /// 0x93c
			 0x889fcf7e, /// 0x940
			 0x83616181, /// 0x944
			 0x5d1c1a90, /// 0x948
			 0x96a1b266, /// 0x94c
			 0xd85b2b88, /// 0x950
			 0x361cc83b, /// 0x954
			 0x9af577c7, /// 0x958
			 0xee197613, /// 0x95c
			 0x837044c9, /// 0x960
			 0xf80fbdba, /// 0x964
			 0x556d1efc, /// 0x968
			 0xfbf583c7, /// 0x96c
			 0xae7510f5, /// 0x970
			 0x6c7b4dd3, /// 0x974
			 0xa1962031, /// 0x978
			 0x45e3e88a, /// 0x97c
			 0x31d89751, /// 0x980
			 0x91f8a767, /// 0x984
			 0xef2df087, /// 0x988
			 0x3e8903c4, /// 0x98c
			 0x4596bf84, /// 0x990
			 0x0c35e298, /// 0x994
			 0x86abebfb, /// 0x998
			 0x8c387eb0, /// 0x99c
			 0x446a406e, /// 0x9a0
			 0xa2bd5f0f, /// 0x9a4
			 0x480ca0ef, /// 0x9a8
			 0x10bae6d1, /// 0x9ac
			 0x8ea6e26d, /// 0x9b0
			 0x3da305c2, /// 0x9b4
			 0xe617c991, /// 0x9b8
			 0xfb1aae24, /// 0x9bc
			 0x1a24f50b, /// 0x9c0
			 0x8a679352, /// 0x9c4
			 0x56051b4a, /// 0x9c8
			 0x9b54d00d, /// 0x9cc
			 0x977d7bf4, /// 0x9d0
			 0x6e037771, /// 0x9d4
			 0x6422e0a0, /// 0x9d8
			 0x0935af4c, /// 0x9dc
			 0x524a95ec, /// 0x9e0
			 0x0fdbce47, /// 0x9e4
			 0xba191753, /// 0x9e8
			 0x370673c7, /// 0x9ec
			 0xcab035f7, /// 0x9f0
			 0x09e8470a, /// 0x9f4
			 0xec238c36, /// 0x9f8
			 0xa520dd18, /// 0x9fc
			 0x6e2244cb, /// 0xa00
			 0xb351c3fc, /// 0xa04
			 0x9502647b, /// 0xa08
			 0x1e8183e0, /// 0xa0c
			 0xbc0bad34, /// 0xa10
			 0xd9fc9f4c, /// 0xa14
			 0xc2ca24c7, /// 0xa18
			 0xaf72e6e6, /// 0xa1c
			 0x0163b03d, /// 0xa20
			 0xbf5f75df, /// 0xa24
			 0x3cd93164, /// 0xa28
			 0x7187129e, /// 0xa2c
			 0xe3a30448, /// 0xa30
			 0x214b9853, /// 0xa34
			 0xa1a09370, /// 0xa38
			 0xd7bd0a5b, /// 0xa3c
			 0x700fede3, /// 0xa40
			 0xf248ca83, /// 0xa44
			 0x7a724a4e, /// 0xa48
			 0x1e51e1dc, /// 0xa4c
			 0x1a2eb4d2, /// 0xa50
			 0x299435eb, /// 0xa54
			 0xbdc7baca, /// 0xa58
			 0xee398ef9, /// 0xa5c
			 0x5631b63d, /// 0xa60
			 0xa552ad85, /// 0xa64
			 0xe2f9e8d8, /// 0xa68
			 0x31215c13, /// 0xa6c
			 0x933a2883, /// 0xa70
			 0xf115ce4d, /// 0xa74
			 0xeb4f30f7, /// 0xa78
			 0x7d1c7bfe, /// 0xa7c
			 0x5f2dd339, /// 0xa80
			 0xcda6a0aa, /// 0xa84
			 0x203872cb, /// 0xa88
			 0xa23acc43, /// 0xa8c
			 0xb51c6156, /// 0xa90
			 0x42f773d2, /// 0xa94
			 0xe7f37793, /// 0xa98
			 0x8ebf3d6d, /// 0xa9c
			 0xbb89c964, /// 0xaa0
			 0xaef9217f, /// 0xaa4
			 0x6d86944e, /// 0xaa8
			 0x890e25cc, /// 0xaac
			 0x2adfd05c, /// 0xab0
			 0x91b215a3, /// 0xab4
			 0xaefd6d93, /// 0xab8
			 0xcb0e761c, /// 0xabc
			 0xc7383fd7, /// 0xac0
			 0xaaa6faaf, /// 0xac4
			 0xa8105594, /// 0xac8
			 0x1382d553, /// 0xacc
			 0xa1d3da24, /// 0xad0
			 0x325484c3, /// 0xad4
			 0xb6ba8774, /// 0xad8
			 0x7b8af0ae, /// 0xadc
			 0x9f9ca4a3, /// 0xae0
			 0x803d7c4c, /// 0xae4
			 0xdfce6315, /// 0xae8
			 0x3685ef3d, /// 0xaec
			 0x842dba99, /// 0xaf0
			 0xebcec690, /// 0xaf4
			 0xded84beb, /// 0xaf8
			 0x5cb583a3, /// 0xafc
			 0x78374aaf, /// 0xb00
			 0x189ba8a0, /// 0xb04
			 0x4d61179a, /// 0xb08
			 0x79a97d65, /// 0xb0c
			 0x1019d4da, /// 0xb10
			 0x3c6b245a, /// 0xb14
			 0x3ccdccf5, /// 0xb18
			 0x42613ffb, /// 0xb1c
			 0xe5661407, /// 0xb20
			 0x8d7f0985, /// 0xb24
			 0x692421c1, /// 0xb28
			 0x5552fa10, /// 0xb2c
			 0x605235f2, /// 0xb30
			 0x2af46ade, /// 0xb34
			 0x0cca7551, /// 0xb38
			 0xaa02c4f0, /// 0xb3c
			 0x39098640, /// 0xb40
			 0xa675257d, /// 0xb44
			 0x826a89ef, /// 0xb48
			 0xaa71ba8b, /// 0xb4c
			 0x99206619, /// 0xb50
			 0x8b4c4fdc, /// 0xb54
			 0x0a7016f2, /// 0xb58
			 0xf0dac40e, /// 0xb5c
			 0x571679df, /// 0xb60
			 0xfd1a7841, /// 0xb64
			 0x8bd83399, /// 0xb68
			 0x0f95f647, /// 0xb6c
			 0xd55057c6, /// 0xb70
			 0xf7afc6f7, /// 0xb74
			 0xdfcb4428, /// 0xb78
			 0xb7f86a04, /// 0xb7c
			 0x21b513ab, /// 0xb80
			 0xcb076afa, /// 0xb84
			 0x56ca6694, /// 0xb88
			 0x89043e83, /// 0xb8c
			 0x2f92f694, /// 0xb90
			 0x70dfdeab, /// 0xb94
			 0xf6222de8, /// 0xb98
			 0xd31bd738, /// 0xb9c
			 0x25c5525e, /// 0xba0
			 0x374887cb, /// 0xba4
			 0xa829ec70, /// 0xba8
			 0x1fb855f3, /// 0xbac
			 0x2cbb0843, /// 0xbb0
			 0x79a80893, /// 0xbb4
			 0x50f866a1, /// 0xbb8
			 0x789f8c77, /// 0xbbc
			 0xf9fb8bdd, /// 0xbc0
			 0x3374f028, /// 0xbc4
			 0xe39eda2d, /// 0xbc8
			 0x64679afe, /// 0xbcc
			 0xdbc03f1d, /// 0xbd0
			 0xbf3de504, /// 0xbd4
			 0x0abfee2e, /// 0xbd8
			 0x077d139f, /// 0xbdc
			 0x3a18759e, /// 0xbe0
			 0x341393be, /// 0xbe4
			 0x6e1361c8, /// 0xbe8
			 0x85b07cf5, /// 0xbec
			 0x38928633, /// 0xbf0
			 0x17ae1b40, /// 0xbf4
			 0x701fc41b, /// 0xbf8
			 0xa41ba3cf, /// 0xbfc
			 0x194cac4b, /// 0xc00
			 0xe6bdcf9a, /// 0xc04
			 0x4e5fa433, /// 0xc08
			 0x9d35e4d4, /// 0xc0c
			 0xf45d2246, /// 0xc10
			 0x5c9aa673, /// 0xc14
			 0x3b186d22, /// 0xc18
			 0x6c8e8e0c, /// 0xc1c
			 0x9c8b8e99, /// 0xc20
			 0x7acaaef7, /// 0xc24
			 0x2e71d806, /// 0xc28
			 0x9a6f1490, /// 0xc2c
			 0xa935c2e5, /// 0xc30
			 0x0ab41c3e, /// 0xc34
			 0xc74d49ff, /// 0xc38
			 0x48ccc85f, /// 0xc3c
			 0xcdcaeb7c, /// 0xc40
			 0x26a0c5b0, /// 0xc44
			 0x10d2acd9, /// 0xc48
			 0x374223ed, /// 0xc4c
			 0x4fe5aa24, /// 0xc50
			 0x41ab41b8, /// 0xc54
			 0xe35d16c8, /// 0xc58
			 0xbff54ae6, /// 0xc5c
			 0xf4c6e2ab, /// 0xc60
			 0xe9f033d2, /// 0xc64
			 0x078eff52, /// 0xc68
			 0x31e2fd17, /// 0xc6c
			 0x1dc13782, /// 0xc70
			 0xab553f3c, /// 0xc74
			 0xab56d095, /// 0xc78
			 0x30871519, /// 0xc7c
			 0x5d7d6a20, /// 0xc80
			 0xf8cf3ca4, /// 0xc84
			 0xf24c040e, /// 0xc88
			 0x452e8452, /// 0xc8c
			 0xa3b0da03, /// 0xc90
			 0x190bf1f5, /// 0xc94
			 0x17103fea, /// 0xc98
			 0xf14e0263, /// 0xc9c
			 0x20a9e66b, /// 0xca0
			 0x2973c20b, /// 0xca4
			 0x1b1c1b46, /// 0xca8
			 0xff91f284, /// 0xcac
			 0x1263522f, /// 0xcb0
			 0x305f3d18, /// 0xcb4
			 0x11691d62, /// 0xcb8
			 0x4298da32, /// 0xcbc
			 0xd1f2ea8d, /// 0xcc0
			 0xd090ac65, /// 0xcc4
			 0xafa4d425, /// 0xcc8
			 0xdac0cfe9, /// 0xccc
			 0x9b212642, /// 0xcd0
			 0x9c4f7172, /// 0xcd4
			 0x4e6dcdc7, /// 0xcd8
			 0x2baae8a8, /// 0xcdc
			 0xb7a78f31, /// 0xce0
			 0x1d1e438c, /// 0xce4
			 0xf474a37c, /// 0xce8
			 0x8bcb08c9, /// 0xcec
			 0xdfbfdadd, /// 0xcf0
			 0xd603c846, /// 0xcf4
			 0x1f01ff16, /// 0xcf8
			 0x9d93a77c, /// 0xcfc
			 0xf461b38c, /// 0xd00
			 0x8d6e56e9, /// 0xd04
			 0xd34efaf9, /// 0xd08
			 0xea935361, /// 0xd0c
			 0xdd96cef7, /// 0xd10
			 0x5b6b4d25, /// 0xd14
			 0x82710ba1, /// 0xd18
			 0x0ea8243d, /// 0xd1c
			 0xc54ceb0c, /// 0xd20
			 0x6fb514d3, /// 0xd24
			 0xa5db91b9, /// 0xd28
			 0x8837fc4b, /// 0xd2c
			 0x9feb1f06, /// 0xd30
			 0x25abe0da, /// 0xd34
			 0xbb6fbf02, /// 0xd38
			 0x5d56095c, /// 0xd3c
			 0xae56a15d, /// 0xd40
			 0x032b751c, /// 0xd44
			 0xfeaf17c5, /// 0xd48
			 0x0f133de7, /// 0xd4c
			 0x2b6583b6, /// 0xd50
			 0xd32e4520, /// 0xd54
			 0x2ba4d459, /// 0xd58
			 0xd9eabf92, /// 0xd5c
			 0x01b40b57, /// 0xd60
			 0x85e760f9, /// 0xd64
			 0x32e32f95, /// 0xd68
			 0x6aa07e62, /// 0xd6c
			 0xf177363f, /// 0xd70
			 0xb159b990, /// 0xd74
			 0x7d436b6d, /// 0xd78
			 0x72224fda, /// 0xd7c
			 0xbd958858, /// 0xd80
			 0xa157eed1, /// 0xd84
			 0xa7a50f7c, /// 0xd88
			 0x6217e945, /// 0xd8c
			 0x7393e82c, /// 0xd90
			 0x70286f07, /// 0xd94
			 0x959b5c6c, /// 0xd98
			 0x91aee032, /// 0xd9c
			 0x63ebeb8f, /// 0xda0
			 0x68d9c84c, /// 0xda4
			 0x344a8cf6, /// 0xda8
			 0x41e8194c, /// 0xdac
			 0x454b2f55, /// 0xdb0
			 0xb40e90fd, /// 0xdb4
			 0x7e807a41, /// 0xdb8
			 0x6080d9f5, /// 0xdbc
			 0xe56fa03f, /// 0xdc0
			 0xed5a85dd, /// 0xdc4
			 0x601d0be2, /// 0xdc8
			 0xc357b9c3, /// 0xdcc
			 0xa285d7ae, /// 0xdd0
			 0x5311fbbf, /// 0xdd4
			 0xb6118e8f, /// 0xdd8
			 0x5f516c61, /// 0xddc
			 0xa2b35051, /// 0xde0
			 0x2f44241a, /// 0xde4
			 0x42197d19, /// 0xde8
			 0x050e1473, /// 0xdec
			 0xb57a90d7, /// 0xdf0
			 0x91ec0b8e, /// 0xdf4
			 0x410b8239, /// 0xdf8
			 0x48d3ac66, /// 0xdfc
			 0x1e248a33, /// 0xe00
			 0x01354fde, /// 0xe04
			 0xaf9f2754, /// 0xe08
			 0x680ab55d, /// 0xe0c
			 0x61c1c582, /// 0xe10
			 0x204c2937, /// 0xe14
			 0xbdab98fb, /// 0xe18
			 0x1a951800, /// 0xe1c
			 0x30d1d65f, /// 0xe20
			 0xdbca7bbc, /// 0xe24
			 0x08776f96, /// 0xe28
			 0x4b1dfdd1, /// 0xe2c
			 0x72bf7bbb, /// 0xe30
			 0x472846a6, /// 0xe34
			 0xd7ed8464, /// 0xe38
			 0x5036f591, /// 0xe3c
			 0xc078ff4a, /// 0xe40
			 0x4d6c9a6b, /// 0xe44
			 0x800897e6, /// 0xe48
			 0xac1b0c88, /// 0xe4c
			 0x34778b36, /// 0xe50
			 0xae2df0e1, /// 0xe54
			 0xe9d40e65, /// 0xe58
			 0x7b79b6c1, /// 0xe5c
			 0x720b3094, /// 0xe60
			 0xef0ff4b6, /// 0xe64
			 0x3b4ead30, /// 0xe68
			 0xb3bb3abc, /// 0xe6c
			 0x3218dd9a, /// 0xe70
			 0x0ac93df5, /// 0xe74
			 0x9662d981, /// 0xe78
			 0x91accf1c, /// 0xe7c
			 0x029c41f5, /// 0xe80
			 0xe1fbd9a5, /// 0xe84
			 0x00b0842f, /// 0xe88
			 0x98014363, /// 0xe8c
			 0xeca09d17, /// 0xe90
			 0xa0550a1a, /// 0xe94
			 0x78f2f451, /// 0xe98
			 0xf69fcbff, /// 0xe9c
			 0xfd482095, /// 0xea0
			 0xc1de3437, /// 0xea4
			 0x414ffa54, /// 0xea8
			 0xd4d52a4f, /// 0xeac
			 0x335a5b04, /// 0xeb0
			 0x66c23170, /// 0xeb4
			 0xc0021805, /// 0xeb8
			 0xe4f2c2fe, /// 0xebc
			 0xa584d3df, /// 0xec0
			 0xf6259087, /// 0xec4
			 0x1361af53, /// 0xec8
			 0x22c392d2, /// 0xecc
			 0x32412511, /// 0xed0
			 0xbedc5e11, /// 0xed4
			 0x45ef662a, /// 0xed8
			 0xd7b9c180, /// 0xedc
			 0x70c7906b, /// 0xee0
			 0xc1e81f4a, /// 0xee4
			 0xd262893d, /// 0xee8
			 0x4d18cbc6, /// 0xeec
			 0xaf8f965f, /// 0xef0
			 0x95612c38, /// 0xef4
			 0xfc82ca81, /// 0xef8
			 0x72cb2f5d, /// 0xefc
			 0x6dec8b57, /// 0xf00
			 0x563500ba, /// 0xf04
			 0x1cf6427f, /// 0xf08
			 0xc500235d, /// 0xf0c
			 0x86ff5189, /// 0xf10
			 0xfed439b3, /// 0xf14
			 0x1cb8fa74, /// 0xf18
			 0xfd09135f, /// 0xf1c
			 0x78df58c9, /// 0xf20
			 0xbce2fc14, /// 0xf24
			 0x90deec4e, /// 0xf28
			 0x9ebd14b0, /// 0xf2c
			 0xad911994, /// 0xf30
			 0x2e0f3a3f, /// 0xf34
			 0xeaad476a, /// 0xf38
			 0x1bc58612, /// 0xf3c
			 0x55c64d87, /// 0xf40
			 0xf068b381, /// 0xf44
			 0x7647ba6c, /// 0xf48
			 0x1e035cd7, /// 0xf4c
			 0x026d1151, /// 0xf50
			 0xf94ef595, /// 0xf54
			 0xdb10e364, /// 0xf58
			 0xe7ac69c5, /// 0xf5c
			 0x321ecbdc, /// 0xf60
			 0x384bd6cd, /// 0xf64
			 0x24f05374, /// 0xf68
			 0x215679b9, /// 0xf6c
			 0xb97837cf, /// 0xf70
			 0xf27151ba, /// 0xf74
			 0x3f890d8e, /// 0xf78
			 0x5843dcd6, /// 0xf7c
			 0xb29c8640, /// 0xf80
			 0x609a65b9, /// 0xf84
			 0x82644fb8, /// 0xf88
			 0x6c67f0d7, /// 0xf8c
			 0x29b562f6, /// 0xf90
			 0x0fae5ace, /// 0xf94
			 0xee2ec23f, /// 0xf98
			 0xf9f77f6b, /// 0xf9c
			 0x826c544a, /// 0xfa0
			 0xec087b07, /// 0xfa4
			 0x989e5590, /// 0xfa8
			 0x8b820b62, /// 0xfac
			 0x165c84e1, /// 0xfb0
			 0xaddcb402, /// 0xfb4
			 0xc2a93b76, /// 0xfb8
			 0x1ed2dee5, /// 0xfbc
			 0xed073dc3, /// 0xfc0
			 0x5f25bc7b, /// 0xfc4
			 0xdfc245cc, /// 0xfc8
			 0x2a7db66c, /// 0xfcc
			 0x3d762117, /// 0xfd0
			 0xc993de52, /// 0xfd4
			 0xdef923d1, /// 0xfd8
			 0x755eb995, /// 0xfdc
			 0x69a2f68c, /// 0xfe0
			 0x369d6ca8, /// 0xfe4
			 0x674306e2, /// 0xfe8
			 0x0cda8f9a, /// 0xfec
			 0xc578c263, /// 0xff0
			 0x6b607f2c, /// 0xff4
			 0xe0fd67f4, /// 0xff8
			 0x4cdd14ac /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xd2ae5a5a, /// 0x0
			 0x58576a73, /// 0x4
			 0xd6c5d0a5, /// 0x8
			 0xa0706713, /// 0xc
			 0x8be9bcfa, /// 0x10
			 0xc609f9e3, /// 0x14
			 0x3ea7d301, /// 0x18
			 0xf987d716, /// 0x1c
			 0xb42e183e, /// 0x20
			 0xbfbf1ccb, /// 0x24
			 0xc56b8111, /// 0x28
			 0xad639eb4, /// 0x2c
			 0x4a7a12d7, /// 0x30
			 0x48b1739e, /// 0x34
			 0xb0376b3c, /// 0x38
			 0x71b84994, /// 0x3c
			 0x47499469, /// 0x40
			 0x05281816, /// 0x44
			 0xa5892806, /// 0x48
			 0x6b0e37b4, /// 0x4c
			 0x5dc38981, /// 0x50
			 0xcf047031, /// 0x54
			 0x8fee0d8b, /// 0x58
			 0xce205518, /// 0x5c
			 0x56898e78, /// 0x60
			 0x914171e8, /// 0x64
			 0x15503e9d, /// 0x68
			 0x7c746576, /// 0x6c
			 0x4f35a13c, /// 0x70
			 0x2328f233, /// 0x74
			 0x646cd1a9, /// 0x78
			 0x365daded, /// 0x7c
			 0x96a3c0ee, /// 0x80
			 0x745011ae, /// 0x84
			 0xe6965e76, /// 0x88
			 0xbe9ea0df, /// 0x8c
			 0x67c2d763, /// 0x90
			 0xe913a231, /// 0x94
			 0xdedaf18d, /// 0x98
			 0xe67db8a2, /// 0x9c
			 0x06a3e976, /// 0xa0
			 0xe0cb35b8, /// 0xa4
			 0x6b887b31, /// 0xa8
			 0xeb4ac5e5, /// 0xac
			 0x03f17197, /// 0xb0
			 0xa682e8b4, /// 0xb4
			 0x54e9b488, /// 0xb8
			 0xbb357a0c, /// 0xbc
			 0x8611fae9, /// 0xc0
			 0x56cfd418, /// 0xc4
			 0x41c46483, /// 0xc8
			 0x8cc56323, /// 0xcc
			 0xcc1dba77, /// 0xd0
			 0x2675f6eb, /// 0xd4
			 0x373da44d, /// 0xd8
			 0xa2ab6926, /// 0xdc
			 0xd702053a, /// 0xe0
			 0x866e3287, /// 0xe4
			 0xe1e2aea0, /// 0xe8
			 0x1e2c47e3, /// 0xec
			 0x969dd273, /// 0xf0
			 0x70e4527b, /// 0xf4
			 0x533c9a78, /// 0xf8
			 0xc4728cee, /// 0xfc
			 0x607f7217, /// 0x100
			 0x72663cc1, /// 0x104
			 0xa8c2151d, /// 0x108
			 0x841ab37d, /// 0x10c
			 0xa591681a, /// 0x110
			 0x7aa15c4d, /// 0x114
			 0xa61a4741, /// 0x118
			 0xf563f481, /// 0x11c
			 0x8139a2f0, /// 0x120
			 0xa06e4760, /// 0x124
			 0x03c6fd1a, /// 0x128
			 0x73bbe6ad, /// 0x12c
			 0xda0dd00d, /// 0x130
			 0x62670f06, /// 0x134
			 0xdc5c4694, /// 0x138
			 0x0c717546, /// 0x13c
			 0xa6afee16, /// 0x140
			 0x364f2dfe, /// 0x144
			 0x9b0159ab, /// 0x148
			 0xa21dbe15, /// 0x14c
			 0xa68ac81c, /// 0x150
			 0x992429b0, /// 0x154
			 0xd172ed60, /// 0x158
			 0xdd807066, /// 0x15c
			 0x3cba9e66, /// 0x160
			 0xc7b1dc73, /// 0x164
			 0x322f4010, /// 0x168
			 0x25bb19d7, /// 0x16c
			 0x83b691be, /// 0x170
			 0xaef2a019, /// 0x174
			 0xe954d15e, /// 0x178
			 0x622030a2, /// 0x17c
			 0x0ca65d9f, /// 0x180
			 0x39fbb7cc, /// 0x184
			 0x8ac1044b, /// 0x188
			 0x0e197e7a, /// 0x18c
			 0x0e77d402, /// 0x190
			 0xf097cc41, /// 0x194
			 0xe19f5bb1, /// 0x198
			 0xc0a2016c, /// 0x19c
			 0xbf4ae6a7, /// 0x1a0
			 0x386e2b7b, /// 0x1a4
			 0x5467790c, /// 0x1a8
			 0xfdaa00a8, /// 0x1ac
			 0x0cfe70b3, /// 0x1b0
			 0xaa3db5fd, /// 0x1b4
			 0x595409a7, /// 0x1b8
			 0xab43859e, /// 0x1bc
			 0x0a3b6e85, /// 0x1c0
			 0x5af6025e, /// 0x1c4
			 0xf3b1704d, /// 0x1c8
			 0x8bd043f7, /// 0x1cc
			 0xe560f9d9, /// 0x1d0
			 0xb8e985e1, /// 0x1d4
			 0x058acdac, /// 0x1d8
			 0xc6dbd9f7, /// 0x1dc
			 0x3369715e, /// 0x1e0
			 0xb6d849af, /// 0x1e4
			 0xbf7795e2, /// 0x1e8
			 0x1dbbb173, /// 0x1ec
			 0x24afca98, /// 0x1f0
			 0x42246ce6, /// 0x1f4
			 0x70ad6b8b, /// 0x1f8
			 0x620dbfde, /// 0x1fc
			 0x8dcb38ff, /// 0x200
			 0x2fe22e49, /// 0x204
			 0x24fa67f6, /// 0x208
			 0xfcb7db0a, /// 0x20c
			 0x071b663b, /// 0x210
			 0x88274dc3, /// 0x214
			 0x32de1c1d, /// 0x218
			 0x504bac38, /// 0x21c
			 0xd3b5394d, /// 0x220
			 0xf5b7dae7, /// 0x224
			 0x2cccec6c, /// 0x228
			 0x8b2a3198, /// 0x22c
			 0x2a9af32a, /// 0x230
			 0xab9a8fbb, /// 0x234
			 0x0d83c3f1, /// 0x238
			 0x84baed04, /// 0x23c
			 0x1c79e01a, /// 0x240
			 0xb5b5180b, /// 0x244
			 0x58f14d95, /// 0x248
			 0x06eaa8d3, /// 0x24c
			 0xdab39b4f, /// 0x250
			 0x42ce311e, /// 0x254
			 0x9ba41aed, /// 0x258
			 0xb2e69a84, /// 0x25c
			 0xe788ca14, /// 0x260
			 0xb59cc217, /// 0x264
			 0x04934cd1, /// 0x268
			 0x5cb369c5, /// 0x26c
			 0xa0a7fc38, /// 0x270
			 0x8e49912b, /// 0x274
			 0xa5159cc7, /// 0x278
			 0x00094a5d, /// 0x27c
			 0xd7c3c994, /// 0x280
			 0x332fa130, /// 0x284
			 0x187bdd61, /// 0x288
			 0x219501e5, /// 0x28c
			 0xfdddae67, /// 0x290
			 0xeca90635, /// 0x294
			 0x0f9bd648, /// 0x298
			 0xc9fb5745, /// 0x29c
			 0x7b577a04, /// 0x2a0
			 0x0cfa5050, /// 0x2a4
			 0x45b43560, /// 0x2a8
			 0x68b7f1b5, /// 0x2ac
			 0xfd2d11ba, /// 0x2b0
			 0x1cd25bdc, /// 0x2b4
			 0x9bed0742, /// 0x2b8
			 0xf99a34b1, /// 0x2bc
			 0xde1552e9, /// 0x2c0
			 0xbe69550b, /// 0x2c4
			 0x42ad73ae, /// 0x2c8
			 0x1354b44c, /// 0x2cc
			 0x97d929c2, /// 0x2d0
			 0x5bd64c4a, /// 0x2d4
			 0x3709a4d1, /// 0x2d8
			 0xc8501b20, /// 0x2dc
			 0x1f994290, /// 0x2e0
			 0x90343e5b, /// 0x2e4
			 0x8219a8ef, /// 0x2e8
			 0x207d5433, /// 0x2ec
			 0xe78d70c7, /// 0x2f0
			 0xae03abcd, /// 0x2f4
			 0x62027bc9, /// 0x2f8
			 0x7d62cad8, /// 0x2fc
			 0xaaeccbb5, /// 0x300
			 0x6f44b106, /// 0x304
			 0xecc45786, /// 0x308
			 0x0f109458, /// 0x30c
			 0x5d462560, /// 0x310
			 0x858a67ee, /// 0x314
			 0x3e3cf044, /// 0x318
			 0x1f4a84d6, /// 0x31c
			 0xd310e983, /// 0x320
			 0x87844a5b, /// 0x324
			 0x8e2f313f, /// 0x328
			 0x9a0fe8a8, /// 0x32c
			 0x49867794, /// 0x330
			 0x6ff5a63f, /// 0x334
			 0x260e0a26, /// 0x338
			 0x70c93f1f, /// 0x33c
			 0xa7f0984e, /// 0x340
			 0x162525c7, /// 0x344
			 0xc6916fd9, /// 0x348
			 0xb1273ec0, /// 0x34c
			 0x401f062f, /// 0x350
			 0xeb040d50, /// 0x354
			 0x7a74629a, /// 0x358
			 0x0e55b5b5, /// 0x35c
			 0x6f9a1016, /// 0x360
			 0x0068c272, /// 0x364
			 0x74055aad, /// 0x368
			 0xeb194e0e, /// 0x36c
			 0x4a3d20e0, /// 0x370
			 0xbf7ee9d9, /// 0x374
			 0xea128476, /// 0x378
			 0x2dabcfdd, /// 0x37c
			 0x66fb33ba, /// 0x380
			 0x84e7d2cc, /// 0x384
			 0xd8b1cd4f, /// 0x388
			 0x08f692d7, /// 0x38c
			 0x2002bf55, /// 0x390
			 0x2eaabe18, /// 0x394
			 0xb51d0dc8, /// 0x398
			 0x073b782d, /// 0x39c
			 0x98c9a5b3, /// 0x3a0
			 0x13736f24, /// 0x3a4
			 0x2264d112, /// 0x3a8
			 0x9409d676, /// 0x3ac
			 0x6c4d374d, /// 0x3b0
			 0xcffe1cf1, /// 0x3b4
			 0x9ef935ba, /// 0x3b8
			 0x656db6fb, /// 0x3bc
			 0xadfbab52, /// 0x3c0
			 0xa4d36777, /// 0x3c4
			 0x0acf8ad0, /// 0x3c8
			 0x705daf91, /// 0x3cc
			 0x479422a0, /// 0x3d0
			 0xdad9a939, /// 0x3d4
			 0xe892c69b, /// 0x3d8
			 0xd0082245, /// 0x3dc
			 0x142e499d, /// 0x3e0
			 0x899a3083, /// 0x3e4
			 0xce86a29b, /// 0x3e8
			 0x3c40f8ec, /// 0x3ec
			 0x9ac526e7, /// 0x3f0
			 0xd1271b27, /// 0x3f4
			 0x987a56bc, /// 0x3f8
			 0x96023f9d, /// 0x3fc
			 0xe8e1332f, /// 0x400
			 0x5045583d, /// 0x404
			 0xd38c9725, /// 0x408
			 0x61d43928, /// 0x40c
			 0x95f616d6, /// 0x410
			 0xcbd1f91b, /// 0x414
			 0xd72d483a, /// 0x418
			 0xb0e40982, /// 0x41c
			 0xc65b6574, /// 0x420
			 0x615302b5, /// 0x424
			 0x097c6be6, /// 0x428
			 0xe6a2adf7, /// 0x42c
			 0x89ab21de, /// 0x430
			 0xbe31540b, /// 0x434
			 0x4383019f, /// 0x438
			 0x7fa1a04c, /// 0x43c
			 0x42d6f20d, /// 0x440
			 0x79f40db2, /// 0x444
			 0x963ca828, /// 0x448
			 0xc577ec5b, /// 0x44c
			 0x9b020b2f, /// 0x450
			 0x258ee291, /// 0x454
			 0xf45178eb, /// 0x458
			 0x5546a446, /// 0x45c
			 0xac4119f6, /// 0x460
			 0x51352fd3, /// 0x464
			 0x5e4a88a5, /// 0x468
			 0xd6637a0b, /// 0x46c
			 0x7bf81d2f, /// 0x470
			 0x0ee0e176, /// 0x474
			 0xfb3aabbf, /// 0x478
			 0x2f0c5b2c, /// 0x47c
			 0x7c3a3e20, /// 0x480
			 0x9a4aac52, /// 0x484
			 0x19b4e874, /// 0x488
			 0x176708ec, /// 0x48c
			 0xccb679d0, /// 0x490
			 0x2d54a7c3, /// 0x494
			 0x230715c1, /// 0x498
			 0xc9b4e73f, /// 0x49c
			 0x1ca32c03, /// 0x4a0
			 0x8a55a544, /// 0x4a4
			 0xc7209da9, /// 0x4a8
			 0x78ea77d4, /// 0x4ac
			 0x702f6fd6, /// 0x4b0
			 0x6079cefc, /// 0x4b4
			 0xab518c44, /// 0x4b8
			 0xbd4723f5, /// 0x4bc
			 0x15990963, /// 0x4c0
			 0x69b1921b, /// 0x4c4
			 0x5423abbd, /// 0x4c8
			 0xe7aae1fb, /// 0x4cc
			 0x1e00d4e7, /// 0x4d0
			 0xa8bae1c6, /// 0x4d4
			 0x70f2b97c, /// 0x4d8
			 0x9070233f, /// 0x4dc
			 0x814cb769, /// 0x4e0
			 0xe442c86f, /// 0x4e4
			 0xe97ea7f3, /// 0x4e8
			 0x03ceb7c6, /// 0x4ec
			 0x62cd2557, /// 0x4f0
			 0x5c47b71c, /// 0x4f4
			 0x6c02feb6, /// 0x4f8
			 0x169c6c3b, /// 0x4fc
			 0xa150458a, /// 0x500
			 0xbd7340ca, /// 0x504
			 0x5372244d, /// 0x508
			 0x7c111dee, /// 0x50c
			 0x0386a67c, /// 0x510
			 0x59eacaff, /// 0x514
			 0xd0d479e3, /// 0x518
			 0x9b56a869, /// 0x51c
			 0xe22fb126, /// 0x520
			 0xce81cbc3, /// 0x524
			 0x82e7720a, /// 0x528
			 0x134de272, /// 0x52c
			 0xa2683e5e, /// 0x530
			 0x4ecea60e, /// 0x534
			 0x3250ac5e, /// 0x538
			 0x4aa45e09, /// 0x53c
			 0x64a8ffc9, /// 0x540
			 0xcf113f15, /// 0x544
			 0x4e7215e8, /// 0x548
			 0x459110e0, /// 0x54c
			 0x635ba9fb, /// 0x550
			 0x4a33f766, /// 0x554
			 0x1cc44443, /// 0x558
			 0x1e46a18d, /// 0x55c
			 0xda734d56, /// 0x560
			 0x690a4208, /// 0x564
			 0x3488b216, /// 0x568
			 0xcb0ca6bb, /// 0x56c
			 0x6eafb37c, /// 0x570
			 0x93e9b10b, /// 0x574
			 0x8a7904bf, /// 0x578
			 0x33d721b1, /// 0x57c
			 0x0b6edc66, /// 0x580
			 0xcd7119fb, /// 0x584
			 0xb2e9c64a, /// 0x588
			 0x0396679c, /// 0x58c
			 0x74582e9c, /// 0x590
			 0x1a04322a, /// 0x594
			 0x478dee18, /// 0x598
			 0x95e801d9, /// 0x59c
			 0xf9601a10, /// 0x5a0
			 0x6c6af702, /// 0x5a4
			 0x06f99e7a, /// 0x5a8
			 0x979b8997, /// 0x5ac
			 0xccea5541, /// 0x5b0
			 0xdfc5042e, /// 0x5b4
			 0xf7f96628, /// 0x5b8
			 0x6912eebc, /// 0x5bc
			 0x7fed7434, /// 0x5c0
			 0x00e96fcd, /// 0x5c4
			 0x7f390ec0, /// 0x5c8
			 0x9e3d721e, /// 0x5cc
			 0xd3efc8da, /// 0x5d0
			 0x4476be1e, /// 0x5d4
			 0x1fd5871f, /// 0x5d8
			 0x0be588ff, /// 0x5dc
			 0x44f1317d, /// 0x5e0
			 0x3cf9e587, /// 0x5e4
			 0xf2ee0cc6, /// 0x5e8
			 0xc72b7873, /// 0x5ec
			 0x6c648361, /// 0x5f0
			 0x3ef90ada, /// 0x5f4
			 0xa30fad60, /// 0x5f8
			 0xb65170b3, /// 0x5fc
			 0xf77bc987, /// 0x600
			 0xe4924e29, /// 0x604
			 0x5e616d34, /// 0x608
			 0x93359ffd, /// 0x60c
			 0x51b042a1, /// 0x610
			 0x40ce3d62, /// 0x614
			 0x8c9fd3a2, /// 0x618
			 0xd974f4c0, /// 0x61c
			 0x55cf4a8f, /// 0x620
			 0x72e225bc, /// 0x624
			 0x1d636c38, /// 0x628
			 0x6c70e002, /// 0x62c
			 0x00e0b1d6, /// 0x630
			 0xa2231d00, /// 0x634
			 0xb112e667, /// 0x638
			 0xc64fe712, /// 0x63c
			 0x8aa9a856, /// 0x640
			 0xf830505f, /// 0x644
			 0x49f2b3c0, /// 0x648
			 0x29b04137, /// 0x64c
			 0x6bb31a96, /// 0x650
			 0x26111a02, /// 0x654
			 0xa84e3f6d, /// 0x658
			 0x00ffad2b, /// 0x65c
			 0x7f2b7d35, /// 0x660
			 0x34d2f178, /// 0x664
			 0x560079f3, /// 0x668
			 0x8e4023d3, /// 0x66c
			 0xf9a43f4b, /// 0x670
			 0x0f2d2324, /// 0x674
			 0x3d4dfb07, /// 0x678
			 0x4b7a834a, /// 0x67c
			 0x3fb9c879, /// 0x680
			 0x68e3ab88, /// 0x684
			 0x64669a75, /// 0x688
			 0x000472ff, /// 0x68c
			 0x848e8521, /// 0x690
			 0x365f816f, /// 0x694
			 0xb2c98a87, /// 0x698
			 0xa97c07af, /// 0x69c
			 0x552735d2, /// 0x6a0
			 0x78818255, /// 0x6a4
			 0xab637071, /// 0x6a8
			 0x4730de32, /// 0x6ac
			 0x18aab954, /// 0x6b0
			 0xfdf2732c, /// 0x6b4
			 0x96fee09f, /// 0x6b8
			 0x78f29c99, /// 0x6bc
			 0x63286432, /// 0x6c0
			 0x0d76ff8c, /// 0x6c4
			 0x214be345, /// 0x6c8
			 0xe0e3e68f, /// 0x6cc
			 0x1bb9183a, /// 0x6d0
			 0x4adff206, /// 0x6d4
			 0x07b74a1c, /// 0x6d8
			 0x14095840, /// 0x6dc
			 0x29a979fe, /// 0x6e0
			 0xb56f941b, /// 0x6e4
			 0x3c41bd4b, /// 0x6e8
			 0xf0369ca6, /// 0x6ec
			 0x16ed35e2, /// 0x6f0
			 0xc6681f7a, /// 0x6f4
			 0x4ea66ebb, /// 0x6f8
			 0xfdfeca59, /// 0x6fc
			 0x07cc4f92, /// 0x700
			 0x2970d4ba, /// 0x704
			 0x78446f49, /// 0x708
			 0x6265f76b, /// 0x70c
			 0x7e1d4251, /// 0x710
			 0x01594e98, /// 0x714
			 0xb9d44979, /// 0x718
			 0xe74ba52c, /// 0x71c
			 0x67ed1e69, /// 0x720
			 0x706c4e91, /// 0x724
			 0xb47910e8, /// 0x728
			 0x5c2e5048, /// 0x72c
			 0x75a871fb, /// 0x730
			 0x7fd80596, /// 0x734
			 0x4fcf48d3, /// 0x738
			 0xb5ac4409, /// 0x73c
			 0xc170442d, /// 0x740
			 0x414b6519, /// 0x744
			 0x80c1ded5, /// 0x748
			 0x79e878a2, /// 0x74c
			 0xb51f34b0, /// 0x750
			 0xd2ca8f55, /// 0x754
			 0x4e8ea0fe, /// 0x758
			 0xd244ee85, /// 0x75c
			 0x2c8b08b5, /// 0x760
			 0x58de3cdb, /// 0x764
			 0x417ec922, /// 0x768
			 0x9869deee, /// 0x76c
			 0x16c10491, /// 0x770
			 0x370151bc, /// 0x774
			 0x9f5db073, /// 0x778
			 0xca2588c4, /// 0x77c
			 0xb154377c, /// 0x780
			 0x0d53efd2, /// 0x784
			 0x8044e630, /// 0x788
			 0x612635b1, /// 0x78c
			 0xf68fdc10, /// 0x790
			 0xc15fdd2b, /// 0x794
			 0x3cead1e3, /// 0x798
			 0x8c95c06c, /// 0x79c
			 0xd8a8a31e, /// 0x7a0
			 0xec9fe064, /// 0x7a4
			 0x0c81f0b5, /// 0x7a8
			 0xe87f738d, /// 0x7ac
			 0x30cf0ea3, /// 0x7b0
			 0x8399e110, /// 0x7b4
			 0xd78bdc87, /// 0x7b8
			 0x57deeb8f, /// 0x7bc
			 0xabf7da1a, /// 0x7c0
			 0x0eeae161, /// 0x7c4
			 0xb4b8ccc9, /// 0x7c8
			 0x7a583a6d, /// 0x7cc
			 0x8b88b271, /// 0x7d0
			 0x526c2850, /// 0x7d4
			 0x6243da80, /// 0x7d8
			 0xabf3511c, /// 0x7dc
			 0x6069afa9, /// 0x7e0
			 0x2c2d8887, /// 0x7e4
			 0xb3ecf02b, /// 0x7e8
			 0xb8c121e4, /// 0x7ec
			 0x79692828, /// 0x7f0
			 0xf4b023b5, /// 0x7f4
			 0x58b3aad2, /// 0x7f8
			 0x65fd8b25, /// 0x7fc
			 0x3b120cc4, /// 0x800
			 0x42860954, /// 0x804
			 0x0ab387f7, /// 0x808
			 0xee64bd9c, /// 0x80c
			 0x9acc0a1b, /// 0x810
			 0x8cd5ae0f, /// 0x814
			 0xbbdd937b, /// 0x818
			 0x965b2e3c, /// 0x81c
			 0xeb7cce32, /// 0x820
			 0x04f0bb12, /// 0x824
			 0x49ed839a, /// 0x828
			 0xb09b6c7d, /// 0x82c
			 0x56a5d309, /// 0x830
			 0x4599e626, /// 0x834
			 0x023a9b92, /// 0x838
			 0x3932b4bb, /// 0x83c
			 0x90c52200, /// 0x840
			 0x2b1ea4a3, /// 0x844
			 0xad2e474a, /// 0x848
			 0x1c16a6d1, /// 0x84c
			 0x9c5333c1, /// 0x850
			 0xb66102aa, /// 0x854
			 0xeecdac81, /// 0x858
			 0xe5b8c761, /// 0x85c
			 0x807bd2ec, /// 0x860
			 0x43f049b0, /// 0x864
			 0xf768cfb1, /// 0x868
			 0xb63e561c, /// 0x86c
			 0x3fe4b27b, /// 0x870
			 0xb5d5fe56, /// 0x874
			 0x03cf69cc, /// 0x878
			 0xfdf5a023, /// 0x87c
			 0x557d8ce8, /// 0x880
			 0xf9be42af, /// 0x884
			 0x935049e4, /// 0x888
			 0x1ae6b3bf, /// 0x88c
			 0x2ef246c7, /// 0x890
			 0x7cc19870, /// 0x894
			 0x5535df68, /// 0x898
			 0x84a9e523, /// 0x89c
			 0x955a9d27, /// 0x8a0
			 0xdb1665f4, /// 0x8a4
			 0x4ca1303f, /// 0x8a8
			 0x1cd3e76c, /// 0x8ac
			 0x9c4aa915, /// 0x8b0
			 0x154bc1e7, /// 0x8b4
			 0x6c9d1702, /// 0x8b8
			 0x69349c2a, /// 0x8bc
			 0xa375a70b, /// 0x8c0
			 0xbac70fb7, /// 0x8c4
			 0x77d36409, /// 0x8c8
			 0x7e87ce57, /// 0x8cc
			 0x8c92e582, /// 0x8d0
			 0x7e035917, /// 0x8d4
			 0x2af2d1b6, /// 0x8d8
			 0x252b590b, /// 0x8dc
			 0x2bacbb76, /// 0x8e0
			 0xc52bab6e, /// 0x8e4
			 0xaaf4cc46, /// 0x8e8
			 0xee7ba815, /// 0x8ec
			 0x20eb3b41, /// 0x8f0
			 0x61b594d8, /// 0x8f4
			 0xb5a8e7c1, /// 0x8f8
			 0xe824e637, /// 0x8fc
			 0x510b3589, /// 0x900
			 0x39cb82bb, /// 0x904
			 0x795a3663, /// 0x908
			 0x5fdac47b, /// 0x90c
			 0x765efcb7, /// 0x910
			 0x481a2b68, /// 0x914
			 0x6aaa5e6c, /// 0x918
			 0x5e1f5ffa, /// 0x91c
			 0xfe05f5aa, /// 0x920
			 0x04d262c3, /// 0x924
			 0x5edb659c, /// 0x928
			 0x07c537ee, /// 0x92c
			 0x817b0df4, /// 0x930
			 0x1ac0832f, /// 0x934
			 0xa60c2b5d, /// 0x938
			 0xb68142d0, /// 0x93c
			 0x2725508b, /// 0x940
			 0x2beee791, /// 0x944
			 0x4bfde852, /// 0x948
			 0x7449dfcf, /// 0x94c
			 0x1f6afd3d, /// 0x950
			 0x5504fbb2, /// 0x954
			 0x92565e55, /// 0x958
			 0x858fcddd, /// 0x95c
			 0xe894ced6, /// 0x960
			 0x6f983709, /// 0x964
			 0x6676a814, /// 0x968
			 0x594d63e8, /// 0x96c
			 0x8cd3dc6e, /// 0x970
			 0x8aebae87, /// 0x974
			 0x2d2b384e, /// 0x978
			 0x38b17af2, /// 0x97c
			 0xfde23a06, /// 0x980
			 0x03024e05, /// 0x984
			 0x7a44738c, /// 0x988
			 0x3bd631b3, /// 0x98c
			 0xf7775a07, /// 0x990
			 0xf9e8661b, /// 0x994
			 0xc6275be3, /// 0x998
			 0x8a908846, /// 0x99c
			 0xff20e4b4, /// 0x9a0
			 0x7f643e47, /// 0x9a4
			 0x5f42038d, /// 0x9a8
			 0x274a70d2, /// 0x9ac
			 0xe6765e85, /// 0x9b0
			 0x37adf553, /// 0x9b4
			 0x5ab79d69, /// 0x9b8
			 0x0b091816, /// 0x9bc
			 0xc5583be3, /// 0x9c0
			 0xba0e0f0f, /// 0x9c4
			 0x41aa5c34, /// 0x9c8
			 0x49b1d0bd, /// 0x9cc
			 0x32861636, /// 0x9d0
			 0xc33d0c2e, /// 0x9d4
			 0x4031dc11, /// 0x9d8
			 0x00c218e7, /// 0x9dc
			 0x5d7e4f6d, /// 0x9e0
			 0x3a390168, /// 0x9e4
			 0x01faa80f, /// 0x9e8
			 0x6869e42d, /// 0x9ec
			 0xec8c0fe2, /// 0x9f0
			 0x2818aa33, /// 0x9f4
			 0x290e3bdb, /// 0x9f8
			 0x0132c4af, /// 0x9fc
			 0xab6871b4, /// 0xa00
			 0xd3399c06, /// 0xa04
			 0x0710752a, /// 0xa08
			 0x65554b13, /// 0xa0c
			 0x79c6a790, /// 0xa10
			 0xfefaacb4, /// 0xa14
			 0xb61a2d18, /// 0xa18
			 0x6d26d21a, /// 0xa1c
			 0x05042096, /// 0xa20
			 0x642b7613, /// 0xa24
			 0x3a6563f0, /// 0xa28
			 0xcbba9e57, /// 0xa2c
			 0x59bf2adf, /// 0xa30
			 0x4cc1e9c7, /// 0xa34
			 0x26412c81, /// 0xa38
			 0x651a1de7, /// 0xa3c
			 0x93f89a4b, /// 0xa40
			 0xb3a946bb, /// 0xa44
			 0x5e436880, /// 0xa48
			 0xdaa95a64, /// 0xa4c
			 0x0f827790, /// 0xa50
			 0x5083edce, /// 0xa54
			 0x68c9cd90, /// 0xa58
			 0x53e9a927, /// 0xa5c
			 0x8761f38e, /// 0xa60
			 0x4c837548, /// 0xa64
			 0xe26822a4, /// 0xa68
			 0xfa1387e8, /// 0xa6c
			 0x864291ef, /// 0xa70
			 0x8df5baaf, /// 0xa74
			 0xba61d62f, /// 0xa78
			 0x7752ca44, /// 0xa7c
			 0x4923ac61, /// 0xa80
			 0x9f45bede, /// 0xa84
			 0xe7f9dd0f, /// 0xa88
			 0x044d4fc3, /// 0xa8c
			 0x708621eb, /// 0xa90
			 0xf7645322, /// 0xa94
			 0x5080ad58, /// 0xa98
			 0xde761e95, /// 0xa9c
			 0xd011be4f, /// 0xaa0
			 0xd1054ed5, /// 0xaa4
			 0x625bb88c, /// 0xaa8
			 0xa31139b9, /// 0xaac
			 0x0eb37b3e, /// 0xab0
			 0x2ffe3d89, /// 0xab4
			 0x256f195f, /// 0xab8
			 0x5f0cdb09, /// 0xabc
			 0x8609376b, /// 0xac0
			 0x217f1aa2, /// 0xac4
			 0x753eebfd, /// 0xac8
			 0x73022a4a, /// 0xacc
			 0xe1b2ea24, /// 0xad0
			 0xd7f21b59, /// 0xad4
			 0x363ab161, /// 0xad8
			 0xed2fed12, /// 0xadc
			 0xb2d017e0, /// 0xae0
			 0xd30f5e83, /// 0xae4
			 0xec1dd58b, /// 0xae8
			 0x5c73fc95, /// 0xaec
			 0xe9fee674, /// 0xaf0
			 0x6993de28, /// 0xaf4
			 0xd3403a14, /// 0xaf8
			 0xa1363154, /// 0xafc
			 0x9d93cc73, /// 0xb00
			 0xf5e47429, /// 0xb04
			 0x0ab0de45, /// 0xb08
			 0x5ab5736a, /// 0xb0c
			 0x9d8010fe, /// 0xb10
			 0x80e9c67d, /// 0xb14
			 0x94d684ef, /// 0xb18
			 0xdbcf5836, /// 0xb1c
			 0xe2c076b8, /// 0xb20
			 0x938dac73, /// 0xb24
			 0x49e8b2e6, /// 0xb28
			 0x1babd0c2, /// 0xb2c
			 0xed37c857, /// 0xb30
			 0x2a820343, /// 0xb34
			 0x6fbd21f6, /// 0xb38
			 0xd7e616bd, /// 0xb3c
			 0x440d05d2, /// 0xb40
			 0x246f02bd, /// 0xb44
			 0x91b0f07d, /// 0xb48
			 0x966656f3, /// 0xb4c
			 0x60e156fd, /// 0xb50
			 0x99bdde74, /// 0xb54
			 0xa903e89a, /// 0xb58
			 0x82b56eff, /// 0xb5c
			 0x01b664cc, /// 0xb60
			 0x18b6d5b4, /// 0xb64
			 0xe4202426, /// 0xb68
			 0x754195bd, /// 0xb6c
			 0x77f8c2c1, /// 0xb70
			 0x93aa9efc, /// 0xb74
			 0xf509a544, /// 0xb78
			 0xcfe224c0, /// 0xb7c
			 0xc1a0be4b, /// 0xb80
			 0xbb472240, /// 0xb84
			 0x92f53bbd, /// 0xb88
			 0x97bc4880, /// 0xb8c
			 0x8477bbd9, /// 0xb90
			 0x5ac8711e, /// 0xb94
			 0xea0ad974, /// 0xb98
			 0x12b8ec6c, /// 0xb9c
			 0xf9c1236a, /// 0xba0
			 0x0101c0bb, /// 0xba4
			 0x890358cf, /// 0xba8
			 0x144ba898, /// 0xbac
			 0xb765600b, /// 0xbb0
			 0xe9ecd0ca, /// 0xbb4
			 0xb897028b, /// 0xbb8
			 0x4a5b6407, /// 0xbbc
			 0x4aace0b0, /// 0xbc0
			 0x358d022f, /// 0xbc4
			 0xec76831e, /// 0xbc8
			 0xc72cfcfd, /// 0xbcc
			 0x9be504de, /// 0xbd0
			 0x6b8bddd6, /// 0xbd4
			 0x53ccfd18, /// 0xbd8
			 0x5c65ad69, /// 0xbdc
			 0xef90eea0, /// 0xbe0
			 0xf48baac4, /// 0xbe4
			 0x147b6fef, /// 0xbe8
			 0x4f42770d, /// 0xbec
			 0x4d2050e5, /// 0xbf0
			 0x7a37de8a, /// 0xbf4
			 0x43093148, /// 0xbf8
			 0xa7db8bff, /// 0xbfc
			 0x1bd340b3, /// 0xc00
			 0xa161cf88, /// 0xc04
			 0x8637305e, /// 0xc08
			 0x66ce5c31, /// 0xc0c
			 0xfbe94bdd, /// 0xc10
			 0x485ae50b, /// 0xc14
			 0x19ae4a4c, /// 0xc18
			 0x247e56e5, /// 0xc1c
			 0xdbe71011, /// 0xc20
			 0x0833ac6e, /// 0xc24
			 0xfb912696, /// 0xc28
			 0xf2ed254a, /// 0xc2c
			 0xe12bce2c, /// 0xc30
			 0xe08f22ca, /// 0xc34
			 0x3a0e362c, /// 0xc38
			 0x846da154, /// 0xc3c
			 0xa9aac3fa, /// 0xc40
			 0xbe8f3718, /// 0xc44
			 0xa6c6731e, /// 0xc48
			 0x979fd138, /// 0xc4c
			 0x985f193d, /// 0xc50
			 0x669b0b13, /// 0xc54
			 0xb57423d3, /// 0xc58
			 0xbcb18175, /// 0xc5c
			 0x5692f50b, /// 0xc60
			 0x48e9f773, /// 0xc64
			 0xd46a3f38, /// 0xc68
			 0x0912f367, /// 0xc6c
			 0x96709376, /// 0xc70
			 0x797f32a0, /// 0xc74
			 0xfe9e0687, /// 0xc78
			 0x3c32432f, /// 0xc7c
			 0xb80fa246, /// 0xc80
			 0x6b5973f9, /// 0xc84
			 0x24492009, /// 0xc88
			 0x3f1ebf10, /// 0xc8c
			 0x3a604332, /// 0xc90
			 0x8083b8c4, /// 0xc94
			 0xfebb365d, /// 0xc98
			 0xf1a3fe8b, /// 0xc9c
			 0xbac4a67c, /// 0xca0
			 0x04a7ada8, /// 0xca4
			 0xc511dbdf, /// 0xca8
			 0xc1a7bba3, /// 0xcac
			 0x2f564957, /// 0xcb0
			 0x197123a2, /// 0xcb4
			 0x3a15c1d5, /// 0xcb8
			 0x02acd0b1, /// 0xcbc
			 0xa8f3d874, /// 0xcc0
			 0xb2f6fee0, /// 0xcc4
			 0xe6d4c125, /// 0xcc8
			 0x134acf42, /// 0xccc
			 0x2fb56832, /// 0xcd0
			 0xb5f44165, /// 0xcd4
			 0x6aa1d2c7, /// 0xcd8
			 0xe52b75f1, /// 0xcdc
			 0x73f99d50, /// 0xce0
			 0x0c248842, /// 0xce4
			 0xc469564a, /// 0xce8
			 0x3baffe1d, /// 0xcec
			 0x3eac48d2, /// 0xcf0
			 0x9e80ce72, /// 0xcf4
			 0xd9bbc660, /// 0xcf8
			 0x2bb48c01, /// 0xcfc
			 0x96f85995, /// 0xd00
			 0x75a0b278, /// 0xd04
			 0x1a59f7a7, /// 0xd08
			 0x94fc8d0a, /// 0xd0c
			 0x72236af9, /// 0xd10
			 0x50bcfe85, /// 0xd14
			 0x3013623a, /// 0xd18
			 0xda6a4f93, /// 0xd1c
			 0x9f42a96e, /// 0xd20
			 0x8a5c9789, /// 0xd24
			 0xb39b957f, /// 0xd28
			 0x3a22b175, /// 0xd2c
			 0x9634fe68, /// 0xd30
			 0x840c359d, /// 0xd34
			 0x793fc7c8, /// 0xd38
			 0xf825a8e5, /// 0xd3c
			 0x21a8efea, /// 0xd40
			 0x34828aed, /// 0xd44
			 0x80717864, /// 0xd48
			 0xcf7b1d57, /// 0xd4c
			 0x1dfc7875, /// 0xd50
			 0x187229eb, /// 0xd54
			 0x9284713f, /// 0xd58
			 0x5f1f4b1d, /// 0xd5c
			 0xab3549ed, /// 0xd60
			 0x501fe5bd, /// 0xd64
			 0x1c56f8f4, /// 0xd68
			 0x466b88a2, /// 0xd6c
			 0xe79c9804, /// 0xd70
			 0x94714db7, /// 0xd74
			 0x525dc7a1, /// 0xd78
			 0x6667e76b, /// 0xd7c
			 0xe8c89ab6, /// 0xd80
			 0x50d0b31d, /// 0xd84
			 0x061f4ed1, /// 0xd88
			 0x76659af9, /// 0xd8c
			 0x7eb6c28c, /// 0xd90
			 0xe8fea25b, /// 0xd94
			 0x71c37d96, /// 0xd98
			 0x38846c3a, /// 0xd9c
			 0xe313fe56, /// 0xda0
			 0xbd0d5b5f, /// 0xda4
			 0x7c7a57bf, /// 0xda8
			 0xf29652ca, /// 0xdac
			 0x9eb3ae6e, /// 0xdb0
			 0x934279a9, /// 0xdb4
			 0xa9440981, /// 0xdb8
			 0x0ec02032, /// 0xdbc
			 0xe75ed4af, /// 0xdc0
			 0x8a0d1bc9, /// 0xdc4
			 0x8e176747, /// 0xdc8
			 0x5817692f, /// 0xdcc
			 0x43f140ff, /// 0xdd0
			 0xb8fda7a3, /// 0xdd4
			 0x32ecc789, /// 0xdd8
			 0xddf7355a, /// 0xddc
			 0x4877602e, /// 0xde0
			 0xb7a05ca2, /// 0xde4
			 0x7a78f64f, /// 0xde8
			 0xdc0d9689, /// 0xdec
			 0x97e7023e, /// 0xdf0
			 0xb8c60fc0, /// 0xdf4
			 0xe8964f09, /// 0xdf8
			 0xc5deabad, /// 0xdfc
			 0x43599eb1, /// 0xe00
			 0x9c8184c7, /// 0xe04
			 0x47f02ddc, /// 0xe08
			 0x0287fde6, /// 0xe0c
			 0xa97fb8da, /// 0xe10
			 0xca8b30e3, /// 0xe14
			 0x64e4b2bb, /// 0xe18
			 0xef1433c4, /// 0xe1c
			 0x0ccc5d09, /// 0xe20
			 0xab8252a8, /// 0xe24
			 0xb25fbfd3, /// 0xe28
			 0x70f9201c, /// 0xe2c
			 0x4e90c25a, /// 0xe30
			 0x43193383, /// 0xe34
			 0xdb67bbf8, /// 0xe38
			 0x5190913d, /// 0xe3c
			 0xa3d3de0c, /// 0xe40
			 0x10fe61cd, /// 0xe44
			 0x9a874e44, /// 0xe48
			 0x56a192f0, /// 0xe4c
			 0x82dedd24, /// 0xe50
			 0x5cdceb30, /// 0xe54
			 0x3d264c7d, /// 0xe58
			 0xc65ff120, /// 0xe5c
			 0xa803cba2, /// 0xe60
			 0xca3cfe86, /// 0xe64
			 0x6926d7ce, /// 0xe68
			 0x13b21d2b, /// 0xe6c
			 0x1f850c6f, /// 0xe70
			 0xaa6a22b9, /// 0xe74
			 0x2d7f4518, /// 0xe78
			 0x32677e3d, /// 0xe7c
			 0x007013a1, /// 0xe80
			 0x2d24fb98, /// 0xe84
			 0xd94a8d6c, /// 0xe88
			 0x0028f3a9, /// 0xe8c
			 0xee151cca, /// 0xe90
			 0x7d13e5cd, /// 0xe94
			 0xdd9fb2b8, /// 0xe98
			 0x4a4ec803, /// 0xe9c
			 0x45602cb4, /// 0xea0
			 0x067489b4, /// 0xea4
			 0xa730d491, /// 0xea8
			 0x464a6a0b, /// 0xeac
			 0xb149c0f0, /// 0xeb0
			 0xa3f0e7ac, /// 0xeb4
			 0xf8b6a643, /// 0xeb8
			 0xe124a64f, /// 0xebc
			 0x91087fb7, /// 0xec0
			 0x7c525774, /// 0xec4
			 0x129c947f, /// 0xec8
			 0x71dc40fc, /// 0xecc
			 0xbbefbb25, /// 0xed0
			 0xff75bc6f, /// 0xed4
			 0x61ea18d2, /// 0xed8
			 0xabc8c952, /// 0xedc
			 0xdb3c9372, /// 0xee0
			 0x1ffaf3e7, /// 0xee4
			 0xaf1fae85, /// 0xee8
			 0x11a25c4c, /// 0xeec
			 0x68a0045a, /// 0xef0
			 0x8bcb5110, /// 0xef4
			 0xf7225776, /// 0xef8
			 0xb6dfc259, /// 0xefc
			 0x8e155f2b, /// 0xf00
			 0xda95a279, /// 0xf04
			 0x11e3381b, /// 0xf08
			 0x344f005f, /// 0xf0c
			 0xc780eeee, /// 0xf10
			 0x9eaab954, /// 0xf14
			 0xa699fbc9, /// 0xf18
			 0x0ea0b659, /// 0xf1c
			 0xe6807608, /// 0xf20
			 0x24a866bc, /// 0xf24
			 0x1d62e634, /// 0xf28
			 0xb01f18eb, /// 0xf2c
			 0x87d51347, /// 0xf30
			 0x255b086a, /// 0xf34
			 0xd5a028c3, /// 0xf38
			 0x75d9b27a, /// 0xf3c
			 0xc3ce656f, /// 0xf40
			 0x611b11ad, /// 0xf44
			 0x1968da78, /// 0xf48
			 0x2d7285f5, /// 0xf4c
			 0xd82fc6e5, /// 0xf50
			 0x95d62c76, /// 0xf54
			 0x2dd4b4e7, /// 0xf58
			 0xad772df2, /// 0xf5c
			 0x2eb60615, /// 0xf60
			 0x915eb261, /// 0xf64
			 0x1c64331f, /// 0xf68
			 0xf84c89fa, /// 0xf6c
			 0x41182b69, /// 0xf70
			 0xdbb4d33c, /// 0xf74
			 0x386fae14, /// 0xf78
			 0x95ce4fd9, /// 0xf7c
			 0xf1b72519, /// 0xf80
			 0xd9236668, /// 0xf84
			 0x7e167b10, /// 0xf88
			 0x9fa70b32, /// 0xf8c
			 0x5c48e61d, /// 0xf90
			 0xd29944aa, /// 0xf94
			 0xfc1eff6b, /// 0xf98
			 0x12e91c31, /// 0xf9c
			 0x15a1404f, /// 0xfa0
			 0xe73ac225, /// 0xfa4
			 0xd292c018, /// 0xfa8
			 0xd206709c, /// 0xfac
			 0xdc5f6214, /// 0xfb0
			 0x4d7f9fe6, /// 0xfb4
			 0x1a9d3e6b, /// 0xfb8
			 0xa7678f7e, /// 0xfbc
			 0xa1ae5bdc, /// 0xfc0
			 0x8e106903, /// 0xfc4
			 0x0e7bab0c, /// 0xfc8
			 0x03dd6bed, /// 0xfcc
			 0x7719e2f9, /// 0xfd0
			 0x9a7d3403, /// 0xfd4
			 0x2a4e5a3f, /// 0xfd8
			 0x41a75d56, /// 0xfdc
			 0x96889692, /// 0xfe0
			 0x563618ff, /// 0xfe4
			 0x5c662419, /// 0xfe8
			 0x6fe7086b, /// 0xfec
			 0x370491ae, /// 0xff0
			 0x25a49233, /// 0xff4
			 0xc883f565, /// 0xff8
			 0x8e52a746 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00000
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00004
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00008
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0000c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00010
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00014
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00018
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0001c
			 0x00000000,                                                  // zero                                        /// 00020
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00028
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0002c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00030
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00034
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00038
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00040
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00044
			 0x0e000007,                                                  // Trailing 1s:                                /// 00048
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0004c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00050
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00054
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00058
			 0x0c400000,                                                  // Leading 1s:                                 /// 0005c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00064
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0006c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00070
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00074
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00078
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0007c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00080
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00084
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00088
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0008c
			 0x3f028f5c,                                                  // 0.51                                        /// 00090
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x7fc00001,                                                  // signaling NaN                               /// 00098
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0009c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000e0
			 0xcb000000,                                                  // -8388608.0                                  /// 000e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f8
			 0x80000000,                                                  // -zero                                       /// 000fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00104
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0010c
			 0x7f800000,                                                  // inf                                         /// 00110
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00118
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00120
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0012c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00130
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00134
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00138
			 0xffc00001,                                                  // -signaling NaN                              /// 0013c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00140
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00144
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00148
			 0x80000000,                                                  // -zero                                       /// 0014c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00154
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00158
			 0x0c780000,                                                  // Leading 1s:                                 /// 0015c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00160
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00168
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0016c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00170
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00178
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0017c
			 0x3f028f5c,                                                  // 0.51                                        /// 00180
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00184
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00188
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0018c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0e000003,                                                  // Trailing 1s:                                /// 00194
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00198
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0019c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x0c400000,                                                  // Leading 1s:                                 /// 00204
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00210
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00214
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00218
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0e000001,                                                  // Trailing 1s:                                /// 00224
			 0x0c700000,                                                  // Leading 1s:                                 /// 00228
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00230
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00238
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0023c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00240
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00244
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0024c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00254
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00258
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0025c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00260
			 0x7f800000,                                                  // inf                                         /// 00264
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00268
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0026c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00270
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00278
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00280
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00284
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0028c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00290
			 0x3f028f5c,                                                  // 0.51                                        /// 00294
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00298
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0029c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002d4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002e4
			 0x7fc00001,                                                  // signaling NaN                               /// 002e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00300
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00304
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00308
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0030c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00314
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00318
			 0x3f028f5c,                                                  // 0.51                                        /// 0031c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00320
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00324
			 0x80011111,                                                  // -9.7958E-41                                 /// 00328
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0032c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00330
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00338
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0033c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00340
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0034c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00350
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00354
			 0x0c780000,                                                  // Leading 1s:                                 /// 00358
			 0x0e000007,                                                  // Trailing 1s:                                /// 0035c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00360
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00364
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0037c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00380
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00384
			 0xffc00001,                                                  // -signaling NaN                              /// 00388
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00390
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00394
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x3f028f5c,                                                  // 0.51                                        /// 003b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 003d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00400
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00404
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0040c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00410
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00424
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00428
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0043c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00440
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00444
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00448
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00450
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00454
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00458
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0045c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00460
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00468
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0046c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00470
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00474
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00484
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00488
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00490
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00498
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0049c
			 0xcb000000,                                                  // -8388608.0                                  /// 004a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004a8
			 0x7f800000,                                                  // inf                                         /// 004ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x7f800000,                                                  // inf                                         /// 004b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004c4
			 0x55555555,                                                  // 4 random values                             /// 004c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00504
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00508
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0050c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00514
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00518
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0051c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00520
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0052c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00530
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00534
			 0x4b000000,                                                  // 8388608.0                                   /// 00538
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00544
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00548
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0054c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00550
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00554
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00558
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0055c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00560
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00564
			 0x55555555,                                                  // 4 random values                             /// 00568
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00570
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00574
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00578
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0057c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00580
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00584
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0058c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00590
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00594
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0059c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x55555555,                                                  // 4 random values                             /// 005c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005d8
			 0xff800000,                                                  // -inf                                        /// 005dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005fc
			 0x00000000,                                                  // zero                                        /// 00600
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00604
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00608
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0060c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00610
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00614
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00620
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00624
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00628
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0062c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00638
			 0x55555555,                                                  // 4 random values                             /// 0063c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00640
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x3f028f5c,                                                  // 0.51                                        /// 0064c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00660
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00668
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00674
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00680
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0068c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0c400000,                                                  // Leading 1s:                                 /// 00694
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0069c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00700
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00710
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0071c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00720
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00724
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00730
			 0x3f028f5c,                                                  // 0.51                                        /// 00734
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00738
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0073c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00740
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0074c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00750
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00754
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00758
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0075c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00760
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00764
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00770
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x33333333,                                                  // 4 random values                             /// 0077c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00784
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0078c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00790
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00798
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x80000000,                                                  // -zero                                       /// 007c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00800
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00808
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0080c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00818
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0081c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00820
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00824
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00828
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0082c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00830
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0xcb000000,                                                  // -8388608.0                                  /// 00844
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0084c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00850
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00858
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0085c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00868
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00874
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00880
			 0xcb000000,                                                  // -8388608.0                                  /// 00884
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00888
			 0x0e000003,                                                  // Trailing 1s:                                /// 0088c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0c780000,                                                  // Leading 1s:                                 /// 00894
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0089c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 008c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 008c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008e4
			 0x33333333,                                                  // 4 random values                             /// 008e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0xbf028f5c,                                                  // -0.51                                       /// 008f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00900
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00904
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0090c
			 0x00000000,                                                  // zero                                        /// 00910
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00914
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0091c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0092c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00930
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00934
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00938
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0093c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00940
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0094c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00950
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00954
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00958
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00960
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00964
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00968
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00970
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00978
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0097c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00980
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x0e000007,                                                  // Trailing 1s:                                /// 00994
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00998
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 009c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009dc
			 0x7fc00001,                                                  // signaling NaN                               /// 009e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x55555555,                                                  // 4 random values                             /// 00a00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a04
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a10
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a24
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a64
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a8c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aa0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00aa4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aa8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00aac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ab0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ab4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ab8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ac0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ad0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ad4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00adc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ae4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x55555555,                                                  // 4 random values                             /// 00aec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00af0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00af4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00af8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00afc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b24
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b40
			 0x7fc00001,                                                  // signaling NaN                               /// 00b44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b48
			 0xffc00001,                                                  // -signaling NaN                              /// 00b4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b5c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ba4
			 0x33333333,                                                  // 4 random values                             /// 00ba8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x7f800000,                                                  // inf                                         /// 00bb0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bb4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bb8
			 0x55555555,                                                  // 4 random values                             /// 00bbc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bc4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bc8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bd0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bd4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00be8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bf0
			 0x3f028f5c,                                                  // 0.51                                        /// 00bf4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bf8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bfc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c04
			 0x33333333,                                                  // 4 random values                             /// 00c08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c1c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c28
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x00000000,                                                  // zero                                        /// 00c5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xbf028f5c,                                                  // -0.51                                       /// 00c64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c80
			 0x7fc00001,                                                  // signaling NaN                               /// 00c84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c8c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c98
			 0x7f800000,                                                  // inf                                         /// 00c9c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ca4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cb0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cb4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cb8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cc8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ccc
			 0x80000000,                                                  // -zero                                       /// 00cd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cd8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ce4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x55555555,                                                  // 4 random values                             /// 00cec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cf4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cf8
			 0x33333333,                                                  // 4 random values                             /// 00cfc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d08
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d34
			 0x00000000,                                                  // zero                                        /// 00d38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0xffc00001,                                                  // -signaling NaN                              /// 00d4c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d9c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00da0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00da4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00db4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dc0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dc4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dd8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00de0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00de4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00df0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00df4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00df8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dfc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e08
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e30
			 0x80000000,                                                  // -zero                                       /// 00e34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e44
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x55555555,                                                  // 4 random values                             /// 00e4c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e64
			 0xcb000000,                                                  // -8388608.0                                  /// 00e68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e74
			 0x80000000,                                                  // -zero                                       /// 00e78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e90
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ea4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ea8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ebc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ec8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ed8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x55555555,                                                  // 4 random values                             /// 00ee0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x33333333,                                                  // 4 random values                             /// 00ee8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00eec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ef4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ef8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00efc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f34
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f44
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f50
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f70
			 0x4b000000,                                                  // 8388608.0                                   /// 00f74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f98
			 0x80000000,                                                  // -zero                                       /// 00f9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x80000000,                                                  // -zero                                       /// 00fb4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fc0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fc4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fd8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fe0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fe4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fe8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ff0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ff4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00100000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x000004a8,
			 0x00000060,
			 0x00000490,
			 0x00000034,
			 0x0000018c,
			 0x000006dc,
			 0x00000708,
			 0x0000000c,

			 /// vpu register f2
			 0x41a80000,
			 0x40800000,
			 0x41980000,
			 0x41600000,
			 0x40400000,
			 0x41200000,
			 0x41c00000,
			 0x41300000,

			 /// vpu register f3
			 0x41d80000,
			 0x40a00000,
			 0x41b80000,
			 0x41100000,
			 0x41b00000,
			 0x41100000,
			 0x41b80000,
			 0x42000000,

			 /// vpu register f4
			 0x41d80000,
			 0x41000000,
			 0x41c80000,
			 0x41d00000,
			 0x41200000,
			 0x41200000,
			 0x40a00000,
			 0x41c80000,

			 /// vpu register f5
			 0x40e00000,
			 0x41800000,
			 0x40e00000,
			 0x41900000,
			 0x41900000,
			 0x41100000,
			 0x41700000,
			 0x41100000,

			 /// vpu register f6
			 0x41a80000,
			 0x41600000,
			 0x41e80000,
			 0x41300000,
			 0x41f80000,
			 0x41a80000,
			 0x40a00000,
			 0x40c00000,

			 /// vpu register f7
			 0x41200000,
			 0x41f80000,
			 0x41900000,
			 0x41a80000,
			 0x3f800000,
			 0x40000000,
			 0x41600000,
			 0x41800000,

			 /// vpu register f8
			 0x41d00000,
			 0x40e00000,
			 0x40c00000,
			 0x40400000,
			 0x40a00000,
			 0x41c00000,
			 0x41100000,
			 0x41200000,

			 /// vpu register f9
			 0x41e00000,
			 0x41c00000,
			 0x41a80000,
			 0x40e00000,
			 0x41100000,
			 0x40e00000,
			 0x41700000,
			 0x41980000,

			 /// vpu register f10
			 0x40000000,
			 0x3f800000,
			 0x41600000,
			 0x41f00000,
			 0x40400000,
			 0x41a80000,
			 0x41e80000,
			 0x41100000,

			 /// vpu register f11
			 0x41d00000,
			 0x41f00000,
			 0x40a00000,
			 0x41500000,
			 0x41880000,
			 0x41400000,
			 0x41880000,
			 0x41b80000,

			 /// vpu register f12
			 0x41a80000,
			 0x41b80000,
			 0x40000000,
			 0x40000000,
			 0x41f00000,
			 0x41600000,
			 0x41300000,
			 0x41a80000,

			 /// vpu register f13
			 0x41c00000,
			 0x41500000,
			 0x40a00000,
			 0x41500000,
			 0x41a80000,
			 0x41500000,
			 0x41c80000,
			 0x41500000,

			 /// vpu register f14
			 0x41d00000,
			 0x41c80000,
			 0x41a80000,
			 0x41100000,
			 0x41d00000,
			 0x40a00000,
			 0x41400000,
			 0x41b80000,

			 /// vpu register f15
			 0x40e00000,
			 0x41f00000,
			 0x40e00000,
			 0x42000000,
			 0x41600000,
			 0x40400000,
			 0x41100000,
			 0x41700000,

			 /// vpu register f16
			 0x41a80000,
			 0x41200000,
			 0x40000000,
			 0x41200000,
			 0x41f80000,
			 0x41900000,
			 0x41200000,
			 0x41400000,

			 /// vpu register f17
			 0x41d00000,
			 0x41400000,
			 0x41d00000,
			 0x41100000,
			 0x41700000,
			 0x41e80000,
			 0x41b00000,
			 0x41600000,

			 /// vpu register f18
			 0x41a00000,
			 0x3f800000,
			 0x41100000,
			 0x40000000,
			 0x41200000,
			 0x41d80000,
			 0x41d00000,
			 0x40800000,

			 /// vpu register f19
			 0x41100000,
			 0x40a00000,
			 0x41b00000,
			 0x41600000,
			 0x41700000,
			 0x40c00000,
			 0x40a00000,
			 0x41f00000,

			 /// vpu register f20
			 0x41f80000,
			 0x40e00000,
			 0x41e00000,
			 0x41f00000,
			 0x41200000,
			 0x41d00000,
			 0x41f80000,
			 0x41c80000,

			 /// vpu register f21
			 0x40400000,
			 0x40400000,
			 0x41980000,
			 0x41b80000,
			 0x40800000,
			 0x41700000,
			 0x41e80000,
			 0x41980000,

			 /// vpu register f22
			 0x41d80000,
			 0x41c80000,
			 0x41400000,
			 0x41400000,
			 0x41d00000,
			 0x41200000,
			 0x40400000,
			 0x41980000,

			 /// vpu register f23
			 0x41880000,
			 0x40000000,
			 0x41b80000,
			 0x41900000,
			 0x40c00000,
			 0x41d00000,
			 0x41c80000,
			 0x41400000,

			 /// vpu register f24
			 0x41100000,
			 0x41200000,
			 0x41200000,
			 0x42000000,
			 0x41200000,
			 0x41b80000,
			 0x41800000,
			 0x41a80000,

			 /// vpu register f25
			 0x41f80000,
			 0x40000000,
			 0x41d80000,
			 0x41c80000,
			 0x41000000,
			 0x3f800000,
			 0x41f80000,
			 0x40c00000,

			 /// vpu register f26
			 0x40000000,
			 0x40e00000,
			 0x41000000,
			 0x40c00000,
			 0x41e00000,
			 0x41a00000,
			 0x41a80000,
			 0x41600000,

			 /// vpu register f27
			 0x41800000,
			 0x41000000,
			 0x41e00000,
			 0x41800000,
			 0x41b80000,
			 0x40c00000,
			 0x41980000,
			 0x40c00000,

			 /// vpu register f28
			 0x41300000,
			 0x40e00000,
			 0x41880000,
			 0x41400000,
			 0x41100000,
			 0x41900000,
			 0x41880000,
			 0x41d80000,

			 /// vpu register f29
			 0x41700000,
			 0x41880000,
			 0x41f80000,
			 0x41400000,
			 0x41e00000,
			 0x41000000,
			 0x40800000,
			 0x41980000,

			 /// vpu register f30
			 0x41300000,
			 0x41c00000,
			 0x40800000,
			 0x41000000,
			 0x41800000,
			 0x41b00000,
			 0x41b80000,
			 0x40000000,

			 /// vpu register f31
			 0x41b00000,
			 0x41880000,
			 0x41e80000,
			 0x41b80000,
			 0x41c00000,
			 0x41700000,
			 0x40400000,
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
		"masknot m3, m3\n"                                    ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m3\n"                                    ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m4\n"                                    ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m7\n"                                    ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m4\n"                                    ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m4, m0\n"                                    ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m7\n"                                    ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m1\n"                                    ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m5\n"                                    ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m3\n"                                    ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m0\n"                                    ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m3\n"                                    ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m5\n"                                    ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m3\n"                                    ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m2\n"                                    ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m3\n"                                    ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m5\n"                                    ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m1\n"                                    ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m4\n"                                    ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m6, m3\n"                                    ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m0\n"                                    ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m6, m5\n"                                    ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m5\n"                                    ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m2\n"                                    ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m7\n"                                    ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m4, m1\n"                                    ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m1\n"                                    ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m6\n"                                    ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m7\n"                                    ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m4, m4\n"                                    ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m0\n"                                    ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m4\n"                                    ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m4, m4\n"                                    ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m6, m6\n"                                    ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m0\n"                                    ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m4, m2\n"                                    ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m1\n"                                    ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m6, m5\n"                                    ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m6, m1\n"                                    ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m4\n"                                    ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m4\n"                                    ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m2\n"                                    ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m2, m0\n"                                    ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m3\n"                                    ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m7\n"                                    ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m0\n"                                    ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m6, m3\n"                                    ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m6\n"                                    ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m6\n"                                    ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m4, m0\n"                                    ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m4, m3\n"                                    ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m5\n"                                    ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m2, m1\n"                                    ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m7\n"                                    ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m5\n"                                    ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m2\n"                                    ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m2\n"                                    ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m4, m0\n"                                    ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m5\n"                                    ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m3\n"                                    ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m4\n"                                    ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m6, m6\n"                                    ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m5\n"                                    ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m6\n"                                    ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m3\n"                                    ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m2\n"                                    ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m7\n"                                    ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m6, m6\n"                                    ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m2, m7\n"                                    ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m4\n"                                    ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m3\n"                                    ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m5\n"                                    ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m6\n"                                    ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m3\n"                                    ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m6\n"                                    ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m2, m6\n"                                    ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m6, m1\n"                                    ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m1\n"                                    ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m7\n"                                    ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m2, m3\n"                                    ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m0, m4\n"                                    ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m7\n"                                    ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m7\n"                                    ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m2\n"                                    ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m3\n"                                    ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m3, m6\n"                                    ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m2, m6\n"                                    ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m7\n"                                    ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m4, m2\n"                                    ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m6\n"                                    ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m5, m2\n"                                    ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m4, m7\n"                                    ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m5\n"                                    ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m5\n"                                    ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m6\n"                                    ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m7\n"                                    ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m6, m5\n"                                    ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m1, m3\n"                                    ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"masknot m7, m3\n"                                    ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
