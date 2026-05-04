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
			 0xf018a528, /// 0x0
			 0xae1054ce, /// 0x4
			 0x52d226f7, /// 0x8
			 0x0a44cc81, /// 0xc
			 0xf3714714, /// 0x10
			 0x4345ec91, /// 0x14
			 0xcfae0044, /// 0x18
			 0x1dff4bd7, /// 0x1c
			 0x0637014f, /// 0x20
			 0x8ff162c3, /// 0x24
			 0xfc0d8bc9, /// 0x28
			 0x623250a7, /// 0x2c
			 0xa10190b0, /// 0x30
			 0xe2c05c02, /// 0x34
			 0x1fa2c6d3, /// 0x38
			 0x51271479, /// 0x3c
			 0x22ff3685, /// 0x40
			 0x376c9d6d, /// 0x44
			 0xedd4d8d1, /// 0x48
			 0x6892fc99, /// 0x4c
			 0x6315c2b7, /// 0x50
			 0x2d1c4fc2, /// 0x54
			 0xb05f2552, /// 0x58
			 0xa3c1dd39, /// 0x5c
			 0x1ce8306f, /// 0x60
			 0x815fb012, /// 0x64
			 0x39b3a937, /// 0x68
			 0x11d71a0a, /// 0x6c
			 0x50467810, /// 0x70
			 0xdd53ded8, /// 0x74
			 0x7c8223ad, /// 0x78
			 0xe8b0dec6, /// 0x7c
			 0xc94f77ab, /// 0x80
			 0xccdeb21c, /// 0x84
			 0x6f9c3038, /// 0x88
			 0x42544d02, /// 0x8c
			 0xd25724ca, /// 0x90
			 0x8a7b3ea0, /// 0x94
			 0x32a43e5a, /// 0x98
			 0x2b9b831d, /// 0x9c
			 0x5501da12, /// 0xa0
			 0xc452b093, /// 0xa4
			 0x54623fb4, /// 0xa8
			 0xe7dee903, /// 0xac
			 0x63c45b16, /// 0xb0
			 0xd4e470e6, /// 0xb4
			 0xddb4bfcb, /// 0xb8
			 0xb61eca69, /// 0xbc
			 0x07358d6b, /// 0xc0
			 0xf70f5767, /// 0xc4
			 0xbb97fd2e, /// 0xc8
			 0xc9a6f5c1, /// 0xcc
			 0xd826bbd6, /// 0xd0
			 0x6e6e69ea, /// 0xd4
			 0x3e5e2317, /// 0xd8
			 0x19548254, /// 0xdc
			 0x72f49936, /// 0xe0
			 0x7442a836, /// 0xe4
			 0x1fb3e091, /// 0xe8
			 0x7a02a23a, /// 0xec
			 0x819623da, /// 0xf0
			 0x7ca578e1, /// 0xf4
			 0x646728b1, /// 0xf8
			 0xfb03e063, /// 0xfc
			 0x95a04d5d, /// 0x100
			 0xe7674379, /// 0x104
			 0xd42bb58d, /// 0x108
			 0x18dc0b61, /// 0x10c
			 0xc31af4f5, /// 0x110
			 0xe2e5fa69, /// 0x114
			 0x10be8c4d, /// 0x118
			 0xcaa5b883, /// 0x11c
			 0x9c8d3c01, /// 0x120
			 0xea4316c9, /// 0x124
			 0x88de1665, /// 0x128
			 0x2cd08563, /// 0x12c
			 0x017193ae, /// 0x130
			 0xecb78da6, /// 0x134
			 0x14e72677, /// 0x138
			 0x2e152330, /// 0x13c
			 0x8221083d, /// 0x140
			 0x6a8f1217, /// 0x144
			 0x99b458c4, /// 0x148
			 0x708030e9, /// 0x14c
			 0x3db3f4d2, /// 0x150
			 0x9e8df229, /// 0x154
			 0xf91067de, /// 0x158
			 0xe0e821e2, /// 0x15c
			 0x4a971a2d, /// 0x160
			 0xc146b318, /// 0x164
			 0xbbd0fb02, /// 0x168
			 0x3d6ccfc1, /// 0x16c
			 0xb5b8b811, /// 0x170
			 0x37189b25, /// 0x174
			 0x5bc82a36, /// 0x178
			 0xf7ff48b3, /// 0x17c
			 0x41cd096a, /// 0x180
			 0x6a22939b, /// 0x184
			 0xc6665896, /// 0x188
			 0x8b13c201, /// 0x18c
			 0xcec16841, /// 0x190
			 0xaeba4062, /// 0x194
			 0xe9ea4ce1, /// 0x198
			 0x0e6589e6, /// 0x19c
			 0xe7a69a38, /// 0x1a0
			 0x4956fd11, /// 0x1a4
			 0x00c3c919, /// 0x1a8
			 0x02c2c2a1, /// 0x1ac
			 0x987f3252, /// 0x1b0
			 0x40376adc, /// 0x1b4
			 0xf7bd472e, /// 0x1b8
			 0x195bf262, /// 0x1bc
			 0xcd079f7f, /// 0x1c0
			 0x0f28e23e, /// 0x1c4
			 0xc992daec, /// 0x1c8
			 0xdca64230, /// 0x1cc
			 0x7cfe0877, /// 0x1d0
			 0xb9c991c0, /// 0x1d4
			 0x150a1ed3, /// 0x1d8
			 0x00dcae73, /// 0x1dc
			 0xc3aeb349, /// 0x1e0
			 0x012e3aa1, /// 0x1e4
			 0x185ea4c5, /// 0x1e8
			 0x7249722b, /// 0x1ec
			 0x8333c830, /// 0x1f0
			 0x885ad0fe, /// 0x1f4
			 0xb7990ba5, /// 0x1f8
			 0x5112beea, /// 0x1fc
			 0xb9cffb80, /// 0x200
			 0x4760276e, /// 0x204
			 0x549900ad, /// 0x208
			 0xd36f7683, /// 0x20c
			 0x73bcae77, /// 0x210
			 0x3d7a6408, /// 0x214
			 0x3f23473a, /// 0x218
			 0xa7cc9c3a, /// 0x21c
			 0x37a78198, /// 0x220
			 0xfd41bdcd, /// 0x224
			 0x2559dc39, /// 0x228
			 0x5418ffc7, /// 0x22c
			 0xcc118218, /// 0x230
			 0xda2c4b2d, /// 0x234
			 0xbd528d32, /// 0x238
			 0xdf7dde4c, /// 0x23c
			 0x45dd709b, /// 0x240
			 0x22b45ab5, /// 0x244
			 0x3bee3891, /// 0x248
			 0x90094a47, /// 0x24c
			 0x5e9b0bfd, /// 0x250
			 0xb6450b51, /// 0x254
			 0x47d7dff9, /// 0x258
			 0x2dab29f9, /// 0x25c
			 0xcdadb088, /// 0x260
			 0xf734f97d, /// 0x264
			 0xae93fa42, /// 0x268
			 0x34a5a708, /// 0x26c
			 0x3111fbf5, /// 0x270
			 0xbc0fcb4b, /// 0x274
			 0x4e134ad2, /// 0x278
			 0x64df020b, /// 0x27c
			 0x2efa43af, /// 0x280
			 0xa1ac4af3, /// 0x284
			 0x54eacc7d, /// 0x288
			 0x2baf0b3e, /// 0x28c
			 0x8fbb846d, /// 0x290
			 0xfccf218d, /// 0x294
			 0x0ed8afa3, /// 0x298
			 0x644c382d, /// 0x29c
			 0x9db9a4cb, /// 0x2a0
			 0xac77f96e, /// 0x2a4
			 0xf3fe3f48, /// 0x2a8
			 0x67c79a01, /// 0x2ac
			 0x1e3ca5b4, /// 0x2b0
			 0x302107fb, /// 0x2b4
			 0x6e62deb0, /// 0x2b8
			 0x373fa652, /// 0x2bc
			 0xc627d567, /// 0x2c0
			 0xc95a773f, /// 0x2c4
			 0xb7b3fff5, /// 0x2c8
			 0xaaa8a94f, /// 0x2cc
			 0xcb3983fd, /// 0x2d0
			 0x2ab9222d, /// 0x2d4
			 0xc07972e4, /// 0x2d8
			 0x5b7d276e, /// 0x2dc
			 0x52f0985a, /// 0x2e0
			 0x2b8e3025, /// 0x2e4
			 0x9aeaade7, /// 0x2e8
			 0x57d4b85e, /// 0x2ec
			 0xd638c7b4, /// 0x2f0
			 0x75a5ab5e, /// 0x2f4
			 0xa9d9c8b1, /// 0x2f8
			 0x67eb20ce, /// 0x2fc
			 0x184d81cb, /// 0x300
			 0x6e1cb7fb, /// 0x304
			 0x4fa8d7f0, /// 0x308
			 0xee9fb2a2, /// 0x30c
			 0x3f36c06e, /// 0x310
			 0xd2e66b50, /// 0x314
			 0xf1243dd0, /// 0x318
			 0xd9eb2a23, /// 0x31c
			 0x0f816122, /// 0x320
			 0xd521397c, /// 0x324
			 0xa37c0699, /// 0x328
			 0x63bfa689, /// 0x32c
			 0x8e32edf6, /// 0x330
			 0x984de1f3, /// 0x334
			 0xde6820d3, /// 0x338
			 0x32babee3, /// 0x33c
			 0x7b843316, /// 0x340
			 0x72a59a68, /// 0x344
			 0xa5571663, /// 0x348
			 0x3ea78cb8, /// 0x34c
			 0x16d31c97, /// 0x350
			 0x45a57d53, /// 0x354
			 0x082011a5, /// 0x358
			 0x8013209e, /// 0x35c
			 0x9343e27a, /// 0x360
			 0xea7327ce, /// 0x364
			 0x386d191e, /// 0x368
			 0x1e8e6152, /// 0x36c
			 0xfdb55961, /// 0x370
			 0x67f3e976, /// 0x374
			 0x1e1967ea, /// 0x378
			 0x51ce25b4, /// 0x37c
			 0xa2551a12, /// 0x380
			 0x3d6c5fe4, /// 0x384
			 0x6b79c469, /// 0x388
			 0xcf607c07, /// 0x38c
			 0xdece4fdc, /// 0x390
			 0x3fabd158, /// 0x394
			 0xfaa4ca6a, /// 0x398
			 0xea795067, /// 0x39c
			 0x3da18e7c, /// 0x3a0
			 0x50bc6341, /// 0x3a4
			 0x49864052, /// 0x3a8
			 0x1ebbcc2c, /// 0x3ac
			 0x7bfda8cf, /// 0x3b0
			 0x7ada9d18, /// 0x3b4
			 0x38331f40, /// 0x3b8
			 0xa214bce8, /// 0x3bc
			 0x8a2322d3, /// 0x3c0
			 0x68c63741, /// 0x3c4
			 0x20ec8f88, /// 0x3c8
			 0xdd6c975b, /// 0x3cc
			 0x636a0e9d, /// 0x3d0
			 0xc3afef50, /// 0x3d4
			 0x8f51277a, /// 0x3d8
			 0x33b0ec8a, /// 0x3dc
			 0x58695715, /// 0x3e0
			 0x1ce4d38a, /// 0x3e4
			 0xba432adb, /// 0x3e8
			 0x796eeaf3, /// 0x3ec
			 0xeea5d24d, /// 0x3f0
			 0xebc05310, /// 0x3f4
			 0x520a9026, /// 0x3f8
			 0x9cb5955c, /// 0x3fc
			 0xee1d4802, /// 0x400
			 0xe83693da, /// 0x404
			 0x89935b88, /// 0x408
			 0x24e8b1fc, /// 0x40c
			 0xd8656d0c, /// 0x410
			 0x10cb650b, /// 0x414
			 0x1d73118b, /// 0x418
			 0x341087e4, /// 0x41c
			 0xabbada4b, /// 0x420
			 0xbfdc09ec, /// 0x424
			 0xacea708d, /// 0x428
			 0xd7c6bf0b, /// 0x42c
			 0x51fcbec1, /// 0x430
			 0x9eb9da1b, /// 0x434
			 0xd470f3f0, /// 0x438
			 0xec08a03a, /// 0x43c
			 0x1637ac54, /// 0x440
			 0x03ae7c59, /// 0x444
			 0x4756af99, /// 0x448
			 0x9b6b73c2, /// 0x44c
			 0xd184d4c8, /// 0x450
			 0x5200af5e, /// 0x454
			 0x70c5e886, /// 0x458
			 0xac9c2fd5, /// 0x45c
			 0xea2f8b4e, /// 0x460
			 0xaab41059, /// 0x464
			 0x141d3d43, /// 0x468
			 0x8fdbfd7d, /// 0x46c
			 0xbf6095c7, /// 0x470
			 0xfdd18c43, /// 0x474
			 0x25e06ce5, /// 0x478
			 0xe8d968c1, /// 0x47c
			 0xca10b0bb, /// 0x480
			 0xf9974cbb, /// 0x484
			 0x1a24742d, /// 0x488
			 0x2144e939, /// 0x48c
			 0xd7465951, /// 0x490
			 0xbce9946a, /// 0x494
			 0xb6bb7e90, /// 0x498
			 0xf5648d1d, /// 0x49c
			 0x102efbe9, /// 0x4a0
			 0xdd600d62, /// 0x4a4
			 0x262d30cc, /// 0x4a8
			 0xb45cff09, /// 0x4ac
			 0xd345d6f0, /// 0x4b0
			 0x51240f87, /// 0x4b4
			 0x28018b04, /// 0x4b8
			 0xc5129a61, /// 0x4bc
			 0x5d85952b, /// 0x4c0
			 0x79f70ea3, /// 0x4c4
			 0x24f0e315, /// 0x4c8
			 0x54eeed52, /// 0x4cc
			 0xe6442fc3, /// 0x4d0
			 0x17e2c131, /// 0x4d4
			 0x93ac24a6, /// 0x4d8
			 0x3afdc7b3, /// 0x4dc
			 0xb0801df6, /// 0x4e0
			 0x596a9aa9, /// 0x4e4
			 0x092ba2b1, /// 0x4e8
			 0x838eac96, /// 0x4ec
			 0xbfdeefc1, /// 0x4f0
			 0xb19e26db, /// 0x4f4
			 0xd4e0412f, /// 0x4f8
			 0x02935865, /// 0x4fc
			 0x075401fd, /// 0x500
			 0xd058b6f0, /// 0x504
			 0x4ad4fcf3, /// 0x508
			 0x2b53deac, /// 0x50c
			 0xad3e7e06, /// 0x510
			 0x8d75ff16, /// 0x514
			 0x891e4e59, /// 0x518
			 0x853a0bf4, /// 0x51c
			 0x16fb7da4, /// 0x520
			 0x4fbea295, /// 0x524
			 0x30f55073, /// 0x528
			 0xfe7dccc2, /// 0x52c
			 0xa815cf8a, /// 0x530
			 0x86117a7d, /// 0x534
			 0xbdc7ce72, /// 0x538
			 0xebf1f76e, /// 0x53c
			 0xfbdc44d6, /// 0x540
			 0xac76afe9, /// 0x544
			 0x381cf63b, /// 0x548
			 0x403259b0, /// 0x54c
			 0xf13a1059, /// 0x550
			 0x27ab1529, /// 0x554
			 0x65c9b210, /// 0x558
			 0xb9c8ef77, /// 0x55c
			 0xc5a451ac, /// 0x560
			 0xa347b972, /// 0x564
			 0xfe5daa86, /// 0x568
			 0xe3d8a70e, /// 0x56c
			 0xa2973f75, /// 0x570
			 0x570919d7, /// 0x574
			 0xec251dc0, /// 0x578
			 0x43eebb12, /// 0x57c
			 0xb0d2cfcf, /// 0x580
			 0x35612273, /// 0x584
			 0x9fc468f2, /// 0x588
			 0xfd603547, /// 0x58c
			 0x536470c6, /// 0x590
			 0x57e262f7, /// 0x594
			 0x2a8fd7ba, /// 0x598
			 0x8aaae410, /// 0x59c
			 0x4c3c8a0e, /// 0x5a0
			 0xc76350f8, /// 0x5a4
			 0xfb0f4369, /// 0x5a8
			 0x7fbfec26, /// 0x5ac
			 0xae6358ad, /// 0x5b0
			 0xcc6fdfb0, /// 0x5b4
			 0xbb70143e, /// 0x5b8
			 0x4fc3080f, /// 0x5bc
			 0xb603ed8d, /// 0x5c0
			 0x4d13e6fa, /// 0x5c4
			 0x589969b1, /// 0x5c8
			 0x036a679a, /// 0x5cc
			 0x3054c0b8, /// 0x5d0
			 0xc07223c0, /// 0x5d4
			 0xe81ff52c, /// 0x5d8
			 0xd22d082d, /// 0x5dc
			 0xa42c1512, /// 0x5e0
			 0x3871bd1e, /// 0x5e4
			 0x50ca2082, /// 0x5e8
			 0xf7248579, /// 0x5ec
			 0x14372186, /// 0x5f0
			 0x63577a90, /// 0x5f4
			 0x51fdead5, /// 0x5f8
			 0xa19d3d31, /// 0x5fc
			 0x6011a58b, /// 0x600
			 0x06619b2b, /// 0x604
			 0x2d8e84ce, /// 0x608
			 0x40fd4d08, /// 0x60c
			 0x5ae693cb, /// 0x610
			 0xa26b4750, /// 0x614
			 0x60c139ac, /// 0x618
			 0xd9f548cb, /// 0x61c
			 0x109b0490, /// 0x620
			 0xeb449a91, /// 0x624
			 0xe2ce00fa, /// 0x628
			 0xe7716a21, /// 0x62c
			 0x628a844d, /// 0x630
			 0x09c62581, /// 0x634
			 0x49ee90e6, /// 0x638
			 0x0154b0b0, /// 0x63c
			 0xfcd45a80, /// 0x640
			 0x2e2b6921, /// 0x644
			 0x79fa17fb, /// 0x648
			 0x95cd6085, /// 0x64c
			 0x7d3307b2, /// 0x650
			 0xaa4240ac, /// 0x654
			 0xf09c8cb7, /// 0x658
			 0x1dbbdec6, /// 0x65c
			 0x14c84863, /// 0x660
			 0x77077bcf, /// 0x664
			 0xb5abd8ae, /// 0x668
			 0xcaf0b5f3, /// 0x66c
			 0xd555a84f, /// 0x670
			 0x59c34226, /// 0x674
			 0xb749647a, /// 0x678
			 0x387c6de9, /// 0x67c
			 0x3772b12b, /// 0x680
			 0x99f79cab, /// 0x684
			 0x89c0c246, /// 0x688
			 0x5839ca97, /// 0x68c
			 0xcce9f8e6, /// 0x690
			 0xb2add736, /// 0x694
			 0x7336d3ae, /// 0x698
			 0x03b109f8, /// 0x69c
			 0xedc0d9be, /// 0x6a0
			 0xa1958131, /// 0x6a4
			 0x0f843ccc, /// 0x6a8
			 0x511ff285, /// 0x6ac
			 0xdfd0b23e, /// 0x6b0
			 0x903ff067, /// 0x6b4
			 0x7d16a977, /// 0x6b8
			 0xd2b954e8, /// 0x6bc
			 0xcf137706, /// 0x6c0
			 0x0b2c44e3, /// 0x6c4
			 0x0bcde6ef, /// 0x6c8
			 0x93e8a9a9, /// 0x6cc
			 0xad604888, /// 0x6d0
			 0x9c0d5f52, /// 0x6d4
			 0x02ff06ee, /// 0x6d8
			 0x7f7af3c0, /// 0x6dc
			 0xf5fbb328, /// 0x6e0
			 0x4a8b6c97, /// 0x6e4
			 0x545f4ae8, /// 0x6e8
			 0x6af9e23d, /// 0x6ec
			 0xdfe77da5, /// 0x6f0
			 0xe5ae4ed4, /// 0x6f4
			 0x9cf14241, /// 0x6f8
			 0xb81bd044, /// 0x6fc
			 0x3b87c1ca, /// 0x700
			 0xc9d1ffb5, /// 0x704
			 0xd10bfd6c, /// 0x708
			 0xea6ceae1, /// 0x70c
			 0xee4d384f, /// 0x710
			 0x2d1d5ec6, /// 0x714
			 0xd4939b28, /// 0x718
			 0x489bf894, /// 0x71c
			 0x1264d09d, /// 0x720
			 0xab710a98, /// 0x724
			 0x098cca84, /// 0x728
			 0x69e4e883, /// 0x72c
			 0x3bd54ef6, /// 0x730
			 0x8b8ef1d8, /// 0x734
			 0xaf16bd07, /// 0x738
			 0x90c9e756, /// 0x73c
			 0x99bcc25f, /// 0x740
			 0x7e87ef83, /// 0x744
			 0x4ff7db2a, /// 0x748
			 0x715490fb, /// 0x74c
			 0x2d157bf7, /// 0x750
			 0x434fd286, /// 0x754
			 0xde1a2073, /// 0x758
			 0xfaacbf99, /// 0x75c
			 0xbf8ec312, /// 0x760
			 0x39ae00cd, /// 0x764
			 0x2afdcbaf, /// 0x768
			 0x6707ed91, /// 0x76c
			 0x676fb804, /// 0x770
			 0xe9ed86c6, /// 0x774
			 0x4f4b2243, /// 0x778
			 0x81cc7d25, /// 0x77c
			 0x1ff0c58d, /// 0x780
			 0x20f8918f, /// 0x784
			 0xd968c0b7, /// 0x788
			 0xb66dad77, /// 0x78c
			 0xd9b6836a, /// 0x790
			 0xc75487d2, /// 0x794
			 0x5451dccd, /// 0x798
			 0xba19cb5f, /// 0x79c
			 0xa9873d4a, /// 0x7a0
			 0x2690add5, /// 0x7a4
			 0x2127353a, /// 0x7a8
			 0x4333e749, /// 0x7ac
			 0x258a613b, /// 0x7b0
			 0xd0c1b7d0, /// 0x7b4
			 0xc9f3c29f, /// 0x7b8
			 0x42dc7578, /// 0x7bc
			 0x3319f20b, /// 0x7c0
			 0x59dad0e6, /// 0x7c4
			 0xce33fef8, /// 0x7c8
			 0x33fd6611, /// 0x7cc
			 0x6a4d50f3, /// 0x7d0
			 0xfa447617, /// 0x7d4
			 0x8984f902, /// 0x7d8
			 0x3a00fff4, /// 0x7dc
			 0x6d564e1a, /// 0x7e0
			 0xb0693be2, /// 0x7e4
			 0xfb66faa0, /// 0x7e8
			 0x1c85e4a8, /// 0x7ec
			 0xa8dfb25c, /// 0x7f0
			 0x713a9bda, /// 0x7f4
			 0x7bd99531, /// 0x7f8
			 0x41ce528d, /// 0x7fc
			 0x12095008, /// 0x800
			 0xfef79302, /// 0x804
			 0x6e00b9f2, /// 0x808
			 0x20d049ed, /// 0x80c
			 0x7b7279e8, /// 0x810
			 0xe8ec4197, /// 0x814
			 0x648bf427, /// 0x818
			 0x57d69a80, /// 0x81c
			 0xc3f8a374, /// 0x820
			 0x338eb86f, /// 0x824
			 0x1a39aafd, /// 0x828
			 0xf26fa762, /// 0x82c
			 0x9427d98e, /// 0x830
			 0x5c211e3e, /// 0x834
			 0xa38ab010, /// 0x838
			 0x8ec65c72, /// 0x83c
			 0x5f2eb110, /// 0x840
			 0x98a7b87d, /// 0x844
			 0xc211e25c, /// 0x848
			 0x642cc96e, /// 0x84c
			 0xdf2a85ad, /// 0x850
			 0x0f306876, /// 0x854
			 0x3a657397, /// 0x858
			 0x6e965d65, /// 0x85c
			 0x83a42745, /// 0x860
			 0xd06aa65c, /// 0x864
			 0xcdbe97d0, /// 0x868
			 0xc5bdb424, /// 0x86c
			 0x10346c1b, /// 0x870
			 0x952d0a84, /// 0x874
			 0x191a11de, /// 0x878
			 0x9d98fc64, /// 0x87c
			 0x292a7291, /// 0x880
			 0x307bb3be, /// 0x884
			 0xc63368a1, /// 0x888
			 0xdb928bc0, /// 0x88c
			 0x70deab90, /// 0x890
			 0xb447cb67, /// 0x894
			 0xd34f953c, /// 0x898
			 0x46975b36, /// 0x89c
			 0xdb4cc8b9, /// 0x8a0
			 0x63d1870d, /// 0x8a4
			 0x32bdc519, /// 0x8a8
			 0x666be17e, /// 0x8ac
			 0x76d9af71, /// 0x8b0
			 0x1d230b7d, /// 0x8b4
			 0x1f2cfb07, /// 0x8b8
			 0xf594056e, /// 0x8bc
			 0xf850cc21, /// 0x8c0
			 0x62eac888, /// 0x8c4
			 0xd3ae2e8a, /// 0x8c8
			 0xd2bfae30, /// 0x8cc
			 0xc934b4c9, /// 0x8d0
			 0x27159279, /// 0x8d4
			 0xdec86c6d, /// 0x8d8
			 0xccdc451e, /// 0x8dc
			 0xcd1b597a, /// 0x8e0
			 0xfb05e24b, /// 0x8e4
			 0x411bcd2c, /// 0x8e8
			 0xeb100d7e, /// 0x8ec
			 0x050562b2, /// 0x8f0
			 0xc4df33b1, /// 0x8f4
			 0x7c1224ab, /// 0x8f8
			 0x49539e0c, /// 0x8fc
			 0x9c12f96f, /// 0x900
			 0x24f599ed, /// 0x904
			 0x9147aea8, /// 0x908
			 0xf4dd2e03, /// 0x90c
			 0xbee5c66f, /// 0x910
			 0x9a986821, /// 0x914
			 0xec66a50e, /// 0x918
			 0x968e1c97, /// 0x91c
			 0x1eb757f1, /// 0x920
			 0xe8e0c878, /// 0x924
			 0x893d46d2, /// 0x928
			 0xe7c3e7e0, /// 0x92c
			 0xc4d52ebe, /// 0x930
			 0x65069be7, /// 0x934
			 0xca0fe8e8, /// 0x938
			 0x8c3395a4, /// 0x93c
			 0xb9a3e2d6, /// 0x940
			 0xe5c66408, /// 0x944
			 0xf8f50a9c, /// 0x948
			 0x9c347f6d, /// 0x94c
			 0x4a625dcb, /// 0x950
			 0x50c409fb, /// 0x954
			 0x75b9de5c, /// 0x958
			 0xd5c00085, /// 0x95c
			 0x9d3f8a25, /// 0x960
			 0x76e09c2f, /// 0x964
			 0x16dbad32, /// 0x968
			 0x4e485381, /// 0x96c
			 0xc51d37ca, /// 0x970
			 0xf9bdb1a2, /// 0x974
			 0x6948b21d, /// 0x978
			 0xb7bb147e, /// 0x97c
			 0x7c43d7bf, /// 0x980
			 0xf9c4bdcf, /// 0x984
			 0x3176d8a3, /// 0x988
			 0x22bc78f6, /// 0x98c
			 0xac2e51db, /// 0x990
			 0x974d3af4, /// 0x994
			 0x57de4048, /// 0x998
			 0x08f46db6, /// 0x99c
			 0x8d7d4c17, /// 0x9a0
			 0x21cc107a, /// 0x9a4
			 0xb0106eec, /// 0x9a8
			 0x745f4254, /// 0x9ac
			 0x0d39c047, /// 0x9b0
			 0xff136737, /// 0x9b4
			 0xe042f109, /// 0x9b8
			 0x32d7b0e7, /// 0x9bc
			 0xb009be2a, /// 0x9c0
			 0x965c4e4c, /// 0x9c4
			 0xa02a91c6, /// 0x9c8
			 0x022ab910, /// 0x9cc
			 0xbb74a55e, /// 0x9d0
			 0xa4d74cac, /// 0x9d4
			 0xdaa693e6, /// 0x9d8
			 0xad851ab0, /// 0x9dc
			 0x800502be, /// 0x9e0
			 0x1f13de08, /// 0x9e4
			 0xcfee308d, /// 0x9e8
			 0x2a31cef3, /// 0x9ec
			 0x9a416d88, /// 0x9f0
			 0x49bb3853, /// 0x9f4
			 0x8d09d87b, /// 0x9f8
			 0x958c229b, /// 0x9fc
			 0xf65b575d, /// 0xa00
			 0x04206bce, /// 0xa04
			 0x945457f6, /// 0xa08
			 0xf87be028, /// 0xa0c
			 0x5e4b48e0, /// 0xa10
			 0x29f5a134, /// 0xa14
			 0x18fff70b, /// 0xa18
			 0x17745e6c, /// 0xa1c
			 0x1e55d919, /// 0xa20
			 0xb35536cb, /// 0xa24
			 0x6983674f, /// 0xa28
			 0x718012e7, /// 0xa2c
			 0x0047a597, /// 0xa30
			 0x5f6514cc, /// 0xa34
			 0x8b7e2d90, /// 0xa38
			 0x740c2746, /// 0xa3c
			 0x5bdd127f, /// 0xa40
			 0xd9e9c893, /// 0xa44
			 0x81796bf8, /// 0xa48
			 0x77f3f61c, /// 0xa4c
			 0xa125e6f2, /// 0xa50
			 0x71d2e71f, /// 0xa54
			 0xf0fd5dc8, /// 0xa58
			 0x2baddad8, /// 0xa5c
			 0x0512dfba, /// 0xa60
			 0x5676f4dd, /// 0xa64
			 0x5417b206, /// 0xa68
			 0xf283eda3, /// 0xa6c
			 0x6b16c634, /// 0xa70
			 0xf7cdd18e, /// 0xa74
			 0x5bc82e03, /// 0xa78
			 0x381b30d9, /// 0xa7c
			 0x9311a986, /// 0xa80
			 0x5d031f57, /// 0xa84
			 0x1349d844, /// 0xa88
			 0x425e3f84, /// 0xa8c
			 0xcfaa8ea1, /// 0xa90
			 0x165f88ff, /// 0xa94
			 0x8959910f, /// 0xa98
			 0xc1bca133, /// 0xa9c
			 0x51b75195, /// 0xaa0
			 0x5bb4ebc9, /// 0xaa4
			 0xd43906f5, /// 0xaa8
			 0x405c55df, /// 0xaac
			 0x928b38ae, /// 0xab0
			 0xc57fb5d4, /// 0xab4
			 0xd926249b, /// 0xab8
			 0xb5866154, /// 0xabc
			 0xc6dc170b, /// 0xac0
			 0xd9520806, /// 0xac4
			 0x73d5845e, /// 0xac8
			 0x3245a823, /// 0xacc
			 0x767df32a, /// 0xad0
			 0xa0d5b59f, /// 0xad4
			 0x260a3bab, /// 0xad8
			 0xbf886b97, /// 0xadc
			 0xaff2a77e, /// 0xae0
			 0x51cf6d17, /// 0xae4
			 0x651eab21, /// 0xae8
			 0x8fb012a9, /// 0xaec
			 0x6222b424, /// 0xaf0
			 0x603c3407, /// 0xaf4
			 0xa3e9beec, /// 0xaf8
			 0x1505ca28, /// 0xafc
			 0x8555cad1, /// 0xb00
			 0x54db6228, /// 0xb04
			 0x29d8b71d, /// 0xb08
			 0x3feb5aa2, /// 0xb0c
			 0x5aa4a6b2, /// 0xb10
			 0x909573b7, /// 0xb14
			 0x9124c721, /// 0xb18
			 0xffa99ce6, /// 0xb1c
			 0xed37e965, /// 0xb20
			 0xfb169433, /// 0xb24
			 0x0c5f3020, /// 0xb28
			 0x033e3092, /// 0xb2c
			 0x84c2545d, /// 0xb30
			 0xb4313ba6, /// 0xb34
			 0x3ca5d4bb, /// 0xb38
			 0xd34ebc68, /// 0xb3c
			 0xd8462b3f, /// 0xb40
			 0xb0e79bae, /// 0xb44
			 0xc567cba2, /// 0xb48
			 0x9ebf6481, /// 0xb4c
			 0x6b01eb57, /// 0xb50
			 0xda19c39a, /// 0xb54
			 0x8b86fc0b, /// 0xb58
			 0x55b10bd4, /// 0xb5c
			 0x5194f5d6, /// 0xb60
			 0x968ee5ba, /// 0xb64
			 0x3425debb, /// 0xb68
			 0x2fa29b92, /// 0xb6c
			 0x9acb10a3, /// 0xb70
			 0x73879b73, /// 0xb74
			 0x10e87f1b, /// 0xb78
			 0x00783f5d, /// 0xb7c
			 0xdca5a766, /// 0xb80
			 0x06940406, /// 0xb84
			 0x00528fe2, /// 0xb88
			 0x4bf4d044, /// 0xb8c
			 0x9b9f4adf, /// 0xb90
			 0x945d126b, /// 0xb94
			 0xd86400ff, /// 0xb98
			 0xdd38c92c, /// 0xb9c
			 0xe9c183d4, /// 0xba0
			 0x07266bda, /// 0xba4
			 0xf8f24797, /// 0xba8
			 0xe7e2d74f, /// 0xbac
			 0x5948ff87, /// 0xbb0
			 0xe8f0c2b9, /// 0xbb4
			 0xe945cecb, /// 0xbb8
			 0xdf40eaaf, /// 0xbbc
			 0x29a52670, /// 0xbc0
			 0x80b28fd5, /// 0xbc4
			 0x25ff4231, /// 0xbc8
			 0xeda4ee55, /// 0xbcc
			 0x1f520c9c, /// 0xbd0
			 0xab90fe99, /// 0xbd4
			 0xd020b071, /// 0xbd8
			 0x0fef005d, /// 0xbdc
			 0x080839ef, /// 0xbe0
			 0xd1307f2b, /// 0xbe4
			 0xb4ad1025, /// 0xbe8
			 0x953bd3ce, /// 0xbec
			 0xbe8ce681, /// 0xbf0
			 0x3b20be9e, /// 0xbf4
			 0xf9859b5d, /// 0xbf8
			 0x593a0705, /// 0xbfc
			 0x93b33068, /// 0xc00
			 0x9520ae4d, /// 0xc04
			 0xab216de1, /// 0xc08
			 0x0c771666, /// 0xc0c
			 0x2aaf7279, /// 0xc10
			 0x9fb551fa, /// 0xc14
			 0x5e9a7b1f, /// 0xc18
			 0x4cd8f756, /// 0xc1c
			 0xf3c2d90f, /// 0xc20
			 0x05bf4831, /// 0xc24
			 0x4b0e2181, /// 0xc28
			 0xf4993bd6, /// 0xc2c
			 0x273ed959, /// 0xc30
			 0xc6a9b4bf, /// 0xc34
			 0x2d676cdc, /// 0xc38
			 0x7ad470c3, /// 0xc3c
			 0x1cbc84db, /// 0xc40
			 0xdbb2f2d8, /// 0xc44
			 0x9fe2d712, /// 0xc48
			 0x8f012944, /// 0xc4c
			 0x583dee3e, /// 0xc50
			 0xd6ed2a26, /// 0xc54
			 0x3c53f1ab, /// 0xc58
			 0x9d892e8f, /// 0xc5c
			 0x42edbe19, /// 0xc60
			 0x5dc1f2c8, /// 0xc64
			 0xa498bbcf, /// 0xc68
			 0xa7a60dfa, /// 0xc6c
			 0xb527cfa1, /// 0xc70
			 0x10cd21eb, /// 0xc74
			 0xacc410d8, /// 0xc78
			 0x08d00f5b, /// 0xc7c
			 0xa0ac5c0d, /// 0xc80
			 0xadd58741, /// 0xc84
			 0x2621c961, /// 0xc88
			 0x9615d4ea, /// 0xc8c
			 0x9b804b67, /// 0xc90
			 0x39ea74d1, /// 0xc94
			 0xffb1a6c1, /// 0xc98
			 0x5c9489ec, /// 0xc9c
			 0xb5a2ab4e, /// 0xca0
			 0xa036a31e, /// 0xca4
			 0xf93b5ed1, /// 0xca8
			 0x84a01964, /// 0xcac
			 0xff8bd871, /// 0xcb0
			 0x071da278, /// 0xcb4
			 0xf963bf1c, /// 0xcb8
			 0x38045c94, /// 0xcbc
			 0xaa89f035, /// 0xcc0
			 0xf52be08c, /// 0xcc4
			 0x02ec3f02, /// 0xcc8
			 0x6ed84b8a, /// 0xccc
			 0x00846b29, /// 0xcd0
			 0x42ba49d2, /// 0xcd4
			 0x46ce3cd7, /// 0xcd8
			 0x73f86bef, /// 0xcdc
			 0x38854120, /// 0xce0
			 0x2e869a73, /// 0xce4
			 0xd52d8b31, /// 0xce8
			 0x4c986573, /// 0xcec
			 0x234517dd, /// 0xcf0
			 0x85c5fe77, /// 0xcf4
			 0x45a64dd1, /// 0xcf8
			 0x433b82a0, /// 0xcfc
			 0x3484c771, /// 0xd00
			 0x995c2f88, /// 0xd04
			 0x32777a99, /// 0xd08
			 0xc3b9f876, /// 0xd0c
			 0x87e68e35, /// 0xd10
			 0x36ad3f0c, /// 0xd14
			 0x8ac9259f, /// 0xd18
			 0x3a05f454, /// 0xd1c
			 0x77e77288, /// 0xd20
			 0x912eedb1, /// 0xd24
			 0xb4921451, /// 0xd28
			 0x5e3ddffd, /// 0xd2c
			 0x53d16dfe, /// 0xd30
			 0xed06ae11, /// 0xd34
			 0xe1eb74fa, /// 0xd38
			 0x55160f4a, /// 0xd3c
			 0x525f9474, /// 0xd40
			 0xcd83847d, /// 0xd44
			 0xeb064214, /// 0xd48
			 0x0fb8fb53, /// 0xd4c
			 0xffdef04b, /// 0xd50
			 0x625a8c78, /// 0xd54
			 0xa65e0904, /// 0xd58
			 0x81a34fa5, /// 0xd5c
			 0xd4f46574, /// 0xd60
			 0xaca7b54a, /// 0xd64
			 0x65031da8, /// 0xd68
			 0x6686e307, /// 0xd6c
			 0x3c4e9d65, /// 0xd70
			 0x472f9f4f, /// 0xd74
			 0x6a68158c, /// 0xd78
			 0x01955e0b, /// 0xd7c
			 0xc2a08183, /// 0xd80
			 0xf5794024, /// 0xd84
			 0x11343049, /// 0xd88
			 0x8716b368, /// 0xd8c
			 0xd59ed7da, /// 0xd90
			 0x29936abf, /// 0xd94
			 0x4e06e504, /// 0xd98
			 0xab4e050d, /// 0xd9c
			 0xd72c37eb, /// 0xda0
			 0xed7c3e20, /// 0xda4
			 0x1c81c041, /// 0xda8
			 0x3f343e32, /// 0xdac
			 0x88150c17, /// 0xdb0
			 0x1ab0a041, /// 0xdb4
			 0xfd18b038, /// 0xdb8
			 0xa89c5329, /// 0xdbc
			 0xc7afaafa, /// 0xdc0
			 0xfee25b43, /// 0xdc4
			 0x8c81c15d, /// 0xdc8
			 0x0e612309, /// 0xdcc
			 0x3cffaaba, /// 0xdd0
			 0xfdfe87ca, /// 0xdd4
			 0x1a23e2e5, /// 0xdd8
			 0x48f28caa, /// 0xddc
			 0xbc25b283, /// 0xde0
			 0x4daf0a40, /// 0xde4
			 0x3a82d7be, /// 0xde8
			 0x13975065, /// 0xdec
			 0x76cc68ba, /// 0xdf0
			 0x0c04929a, /// 0xdf4
			 0xa3d9c67c, /// 0xdf8
			 0xf26af6e0, /// 0xdfc
			 0xb9ed1741, /// 0xe00
			 0x231b60f2, /// 0xe04
			 0xdbe3575e, /// 0xe08
			 0x12128ade, /// 0xe0c
			 0xf0dda3c2, /// 0xe10
			 0xc021041f, /// 0xe14
			 0x9b969450, /// 0xe18
			 0x5bfc57d8, /// 0xe1c
			 0xa8067a30, /// 0xe20
			 0x6fa01aa9, /// 0xe24
			 0xdac1c51b, /// 0xe28
			 0x7dc0c371, /// 0xe2c
			 0xaebac88a, /// 0xe30
			 0xcc435ad0, /// 0xe34
			 0x9934ef7e, /// 0xe38
			 0xd9bac2ff, /// 0xe3c
			 0xbf2e3762, /// 0xe40
			 0x71d42897, /// 0xe44
			 0x7617641a, /// 0xe48
			 0x05e7fa7e, /// 0xe4c
			 0xf9674be8, /// 0xe50
			 0x07cde1ff, /// 0xe54
			 0xc1a61d93, /// 0xe58
			 0x1f495d7f, /// 0xe5c
			 0x2d8a3461, /// 0xe60
			 0x0af8d859, /// 0xe64
			 0x9e7cf1cb, /// 0xe68
			 0x6fcae108, /// 0xe6c
			 0xffb2a75d, /// 0xe70
			 0xcb8a08bb, /// 0xe74
			 0x8b032407, /// 0xe78
			 0xf773002a, /// 0xe7c
			 0x043fef98, /// 0xe80
			 0x4faf7a75, /// 0xe84
			 0xc02bd8f0, /// 0xe88
			 0x3f35ebd1, /// 0xe8c
			 0x3177134d, /// 0xe90
			 0x8365f5a6, /// 0xe94
			 0x387b28b9, /// 0xe98
			 0x0783e2a7, /// 0xe9c
			 0x88a7167b, /// 0xea0
			 0x83329bc8, /// 0xea4
			 0x3f298cbb, /// 0xea8
			 0x8eb2e572, /// 0xeac
			 0x9929ecf1, /// 0xeb0
			 0x7a7c2393, /// 0xeb4
			 0x078d1cbe, /// 0xeb8
			 0xb27adfc7, /// 0xebc
			 0xd77e1ff3, /// 0xec0
			 0xda87d2a3, /// 0xec4
			 0x853bbcc9, /// 0xec8
			 0xb2dd3e37, /// 0xecc
			 0xa7edef36, /// 0xed0
			 0x90c08c5c, /// 0xed4
			 0x88aea7f4, /// 0xed8
			 0xef0c399b, /// 0xedc
			 0x547c43c1, /// 0xee0
			 0x765b84b4, /// 0xee4
			 0x0c0ed305, /// 0xee8
			 0x5e005877, /// 0xeec
			 0x3ac3e8ae, /// 0xef0
			 0x8674ef2e, /// 0xef4
			 0x54986aaf, /// 0xef8
			 0xcef8f664, /// 0xefc
			 0xeeb3a3d5, /// 0xf00
			 0xd8ef6399, /// 0xf04
			 0xdb7f1b24, /// 0xf08
			 0x8e6b43d8, /// 0xf0c
			 0x174708c9, /// 0xf10
			 0x5c0d8f96, /// 0xf14
			 0x496e0446, /// 0xf18
			 0x6aef2cad, /// 0xf1c
			 0xa839468e, /// 0xf20
			 0x226139f1, /// 0xf24
			 0x6bf646f3, /// 0xf28
			 0x7fbea959, /// 0xf2c
			 0x73f1629c, /// 0xf30
			 0x70685906, /// 0xf34
			 0x7277bd7e, /// 0xf38
			 0x3d6d46e4, /// 0xf3c
			 0x00ccf6b8, /// 0xf40
			 0x41f2f383, /// 0xf44
			 0xeb016f57, /// 0xf48
			 0x8600c2cf, /// 0xf4c
			 0xa2c8351d, /// 0xf50
			 0x722d8601, /// 0xf54
			 0x5f3e8df3, /// 0xf58
			 0x79cc0e9f, /// 0xf5c
			 0x4f5ce60e, /// 0xf60
			 0xb52aac50, /// 0xf64
			 0x29210080, /// 0xf68
			 0x32ef3911, /// 0xf6c
			 0xc9faa956, /// 0xf70
			 0xe470a1f4, /// 0xf74
			 0xbe40889f, /// 0xf78
			 0x7aad3758, /// 0xf7c
			 0x65456758, /// 0xf80
			 0xce6dae25, /// 0xf84
			 0xe5b3ea9b, /// 0xf88
			 0xcd3d974d, /// 0xf8c
			 0x5b3cc1b3, /// 0xf90
			 0xbd4a2dd6, /// 0xf94
			 0x811eb76d, /// 0xf98
			 0xc98c84e5, /// 0xf9c
			 0xf4649136, /// 0xfa0
			 0x0fc7c53f, /// 0xfa4
			 0x1612abf7, /// 0xfa8
			 0x1c86dd57, /// 0xfac
			 0x4b88d3ff, /// 0xfb0
			 0x7e86b913, /// 0xfb4
			 0x64f79e8a, /// 0xfb8
			 0x5a0dcacc, /// 0xfbc
			 0xe8143d27, /// 0xfc0
			 0xe4041df6, /// 0xfc4
			 0x616a282e, /// 0xfc8
			 0xbe444a2c, /// 0xfcc
			 0x834b4ffe, /// 0xfd0
			 0xd0fa8e09, /// 0xfd4
			 0x1fabca19, /// 0xfd8
			 0x00fabc32, /// 0xfdc
			 0x61b958b1, /// 0xfe0
			 0xa5e7b302, /// 0xfe4
			 0x234d6635, /// 0xfe8
			 0x4efa45c3, /// 0xfec
			 0x51de3773, /// 0xff0
			 0xba0d2aa6, /// 0xff4
			 0xc70c432e, /// 0xff8
			 0x481b81ad /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00000
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00004
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00008
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0000c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00000000,                                                  // zero                                        /// 00014
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80000000,                                                  // -zero                                       /// 0001c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00020
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00028
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0002c
			 0x55555555,                                                  // 4 random values                             /// 00030
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00034
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00038
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00040
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00044
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00048
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00050
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00054
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00058
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00060
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00064
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00068
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0006c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00070
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00078
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00080
			 0x7f800000,                                                  // inf                                         /// 00084
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00088
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00094
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00098
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0009c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000a0
			 0x33333333,                                                  // 4 random values                             /// 000a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0xcb000000,                                                  // -8388608.0                                  /// 000bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00108
			 0x33333333,                                                  // 4 random values                             /// 0010c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00110
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00114
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00118
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0011c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00120
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00124
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00130
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00138
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0013c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00140
			 0x00011111,                                                  // 9.7958E-41                                  /// 00144
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0014c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00150
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00154
			 0x0e000007,                                                  // Trailing 1s:                                /// 00158
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0015c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00164
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0016c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00174
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0017c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00180
			 0x0c700000,                                                  // Leading 1s:                                 /// 00184
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00188
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00190
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00198
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0019c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001b8
			 0x33333333,                                                  // 4 random values                             /// 001bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x80000000,                                                  // -zero                                       /// 001cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00200
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00214
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00218
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00220
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00224
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0022c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00234
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00238
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00240
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00244
			 0x00000000,                                                  // zero                                        /// 00248
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00250
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00254
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00260
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00264
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0026c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00270
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00278
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x0c400000,                                                  // Leading 1s:                                 /// 00284
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0028c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00290
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0029c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002a0
			 0x80000000,                                                  // -zero                                       /// 002a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002cc
			 0x55555555,                                                  // 4 random values                             /// 002d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002d8
			 0xffc00001,                                                  // -signaling NaN                              /// 002dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00300
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00304
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00308
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0030c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0xffc00001,                                                  // -signaling NaN                              /// 00314
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00318
			 0x0c700000,                                                  // Leading 1s:                                 /// 0031c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00320
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00324
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00328
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0032c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00330
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00338
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0033c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00340
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00348
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0034c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00350
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00354
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00358
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00364
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00368
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0036c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00374
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00378
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00384
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00388
			 0x00011111,                                                  // 9.7958E-41                                  /// 0038c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00390
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00394
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00398
			 0x33333333,                                                  // 4 random values                             /// 0039c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x00000000,                                                  // zero                                        /// 003bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003c8
			 0x3f028f5c,                                                  // 0.51                                        /// 003cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 003dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003ec
			 0x4b000000,                                                  // 8388608.0                                   /// 003f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00400
			 0x0e000001,                                                  // Trailing 1s:                                /// 00404
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x7f800000,                                                  // inf                                         /// 0040c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00410
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00414
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x4b000000,                                                  // 8388608.0                                   /// 00420
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00424
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00428
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0042c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00430
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00434
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00438
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0043c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00458
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00460
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00464
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0046c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00470
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00474
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00480
			 0x80011111,                                                  // -9.7958E-41                                 /// 00484
			 0x0c400000,                                                  // Leading 1s:                                 /// 00488
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0048c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00490
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0049c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004bc
			 0x55555555,                                                  // 4 random values                             /// 004c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 004f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00500
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00508
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0050c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00510
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00518
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0051c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00520
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00528
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0052c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00530
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00538
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0054c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00550
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0c600000,                                                  // Leading 1s:                                 /// 00558
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0055c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00560
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00564
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00568
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0056c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00570
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0057c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00580
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00584
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00588
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x7fc00001,                                                  // signaling NaN                               /// 0059c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 005c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00600
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00604
			 0x4b000000,                                                  // 8388608.0                                   /// 00608
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0060c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00614
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00620
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00628
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0063c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00640
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x0e000003,                                                  // Trailing 1s:                                /// 00648
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0064c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00650
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00654
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00658
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0065c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00660
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00664
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0066c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00670
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00674
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0067c
			 0xbf028f5c,                                                  // -0.51                                       /// 00680
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00684
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00694
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00698
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0069c
			 0xff800000,                                                  // -inf                                        /// 006a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006b0
			 0x4b000000,                                                  // 8388608.0                                   /// 006b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d4
			 0xff800000,                                                  // -inf                                        /// 006d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 006e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00704
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e000007,                                                  // Trailing 1s:                                /// 0070c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00710
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00718
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00720
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00724
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0072c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00734
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00738
			 0x7fc00001,                                                  // signaling NaN                               /// 0073c
			 0xffc00001,                                                  // -signaling NaN                              /// 00740
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0c600000,                                                  // Leading 1s:                                 /// 00748
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0074c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00750
			 0x80011111,                                                  // -9.7958E-41                                 /// 00754
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00768
			 0xff800000,                                                  // -inf                                        /// 0076c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00770
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00778
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0077c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00784
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00788
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00790
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00794
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00798
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00800
			 0x33333333,                                                  // 4 random values                             /// 00804
			 0xffc00001,                                                  // -signaling NaN                              /// 00808
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00810
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00814
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00818
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0081c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00828
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0082c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00830
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00838
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0083c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00840
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x33333333,                                                  // 4 random values                             /// 00848
			 0x00011111,                                                  // 9.7958E-41                                  /// 0084c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00854
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00858
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0085c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0xcb000000,                                                  // -8388608.0                                  /// 00878
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00880
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00888
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0088c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00890
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00894
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00898
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0089c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008c4
			 0xbf028f5c,                                                  // -0.51                                       /// 008c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008d8
			 0x4b000000,                                                  // 8388608.0                                   /// 008dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008e0
			 0x33333333,                                                  // 4 random values                             /// 008e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00900
			 0x00000000,                                                  // zero                                        /// 00904
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00908
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0090c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00910
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00914
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00918
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0091c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00920
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00924
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00934
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00940
			 0x7fc00001,                                                  // signaling NaN                               /// 00944
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00948
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0094c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0095c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00960
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00964
			 0x7f800000,                                                  // inf                                         /// 00968
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00970
			 0x80000000,                                                  // -zero                                       /// 00974
			 0x4b000000,                                                  // 8388608.0                                   /// 00978
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0097c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0xbf028f5c,                                                  // -0.51                                       /// 00988
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0098c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00990
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00994
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0099c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 009d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x3f028f5c,                                                  // 0.51                                        /// 00a08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a10
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a1c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a2c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a30
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a94
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aa4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00aa8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00aac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ab4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00abc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ac0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ad4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00adc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ae0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00af0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00af4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00af8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00afc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b58
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b64
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b68
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b70
			 0x7fc00001,                                                  // signaling NaN                               /// 00b74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b90
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x7f800000,                                                  // inf                                         /// 00ba0
			 0x7f800000,                                                  // inf                                         /// 00ba4
			 0x7f800000,                                                  // inf                                         /// 00ba8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bb8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bcc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bd0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bf8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c14
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c2c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c58
			 0xffc00001,                                                  // -signaling NaN                              /// 00c5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c64
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c88
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c90
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c94
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ca4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ca8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cb4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cb8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cc4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ccc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cdc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ce0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ce8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cf0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cfc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d08
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d0c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d50
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d78
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d80
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0xff800000,                                                  // -inf                                        /// 00dac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00db0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00db4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00db8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dc4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ddc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00de0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00de4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00de8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00df0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dfc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e08
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0xcb000000,                                                  // -8388608.0                                  /// 00e18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e54
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e58
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e6c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e74
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e78
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ea8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ebc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ec0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ecc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ed4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ed8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ee0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ee4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ee8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ef0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ef4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00efc
			 0xbf028f5c,                                                  // -0.51                                       /// 00f00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f48
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f54
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00000000,                                                  // zero                                        /// 00f74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f80
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x80000000,                                                  // -zero                                       /// 00f90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fa4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fa8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fb8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fd0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fdc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0xffc00001,                                                  // -signaling NaN                              /// 00fe8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ff8
			 0x0c7ff800                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80000000,                                                  // -zero                                       /// 00000
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00004
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0000c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00010
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00014
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00018
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0001c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00020
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00024
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00028
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0002c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x7fc00001,                                                  // signaling NaN                               /// 00034
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00038
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0003c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00040
			 0xffc00001,                                                  // -signaling NaN                              /// 00044
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00048
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00050
			 0x7f800000,                                                  // inf                                         /// 00054
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00058
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00060
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00068
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0006c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00074
			 0x7fc00001,                                                  // signaling NaN                               /// 00078
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0007c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00080
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x33333333,                                                  // 4 random values                             /// 00088
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0008c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00090
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00094
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00098
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0009c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x7f800000,                                                  // inf                                         /// 000a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000b4
			 0x7fc00001,                                                  // signaling NaN                               /// 000b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000e4
			 0x3f028f5c,                                                  // 0.51                                        /// 000e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00100
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00104
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00108
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00110
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00114
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00118
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0011c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00120
			 0x7f800000,                                                  // inf                                         /// 00124
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00128
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00130
			 0x7f800000,                                                  // inf                                         /// 00134
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00138
			 0x3f028f5c,                                                  // 0.51                                        /// 0013c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00140
			 0xbf028f5c,                                                  // -0.51                                       /// 00144
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0014c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00150
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00158
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0015c
			 0x80000000,                                                  // -zero                                       /// 00160
			 0x0e000001,                                                  // Trailing 1s:                                /// 00164
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00168
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0016c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00174
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00178
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0017c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00180
			 0x00000000,                                                  // zero                                        /// 00184
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00194
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00198
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001a0
			 0x00000000,                                                  // zero                                        /// 001a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001b0
			 0xbf028f5c,                                                  // -0.51                                       /// 001b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001cc
			 0x4b000000,                                                  // 8388608.0                                   /// 001d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001d4
			 0x3f028f5c,                                                  // 0.51                                        /// 001d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00200
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00208
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0020c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00210
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00214
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00218
			 0x0e000007,                                                  // Trailing 1s:                                /// 0021c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00220
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00224
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00228
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0022c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00230
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00234
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00238
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00240
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00248
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00254
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00258
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0025c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00264
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00268
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0026c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00274
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00278
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00280
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00284
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00290
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00294
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0029c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00300
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0030c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00314
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00318
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0031c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00324
			 0x0c400000,                                                  // Leading 1s:                                 /// 00328
			 0xff800000,                                                  // -inf                                        /// 0032c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00330
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00338
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00340
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00344
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0034c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00350
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00354
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00358
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00360
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00364
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00370
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00374
			 0x0e000001,                                                  // Trailing 1s:                                /// 00378
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0037c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00380
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00384
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00388
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0038c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00390
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00394
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0039c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003f0
			 0xff800000,                                                  // -inf                                        /// 003f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00400
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00404
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00410
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0041c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00420
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00424
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00434
			 0x33333333,                                                  // 4 random values                             /// 00438
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00440
			 0x33333333,                                                  // 4 random values                             /// 00444
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00448
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0044c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00450
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00458
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00460
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00464
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00468
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00474
			 0x7fc00001,                                                  // signaling NaN                               /// 00478
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00484
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00490
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00494
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0049c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0xffc00001,                                                  // -signaling NaN                              /// 004ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00500
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0050c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00510
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00518
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00524
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00530
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00534
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0053c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00540
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00544
			 0x00000000,                                                  // zero                                        /// 00548
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0054c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00550
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00554
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00558
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00564
			 0x0c780000,                                                  // Leading 1s:                                 /// 00568
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0056c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00570
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00574
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00578
			 0x4b000000,                                                  // 8388608.0                                   /// 0057c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c780000,                                                  // Leading 1s:                                 /// 00584
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00594
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00598
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00600
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00604
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0c780000,                                                  // Leading 1s:                                 /// 0060c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00614
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00618
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0061c
			 0x33333333,                                                  // 4 random values                             /// 00620
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00624
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80011111,                                                  // -9.7958E-41                                 /// 0062c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0063c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00640
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00644
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00648
			 0x00011111,                                                  // 9.7958E-41                                  /// 0064c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0xff800000,                                                  // -inf                                        /// 00654
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00658
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x00000000,                                                  // zero                                        /// 00660
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00664
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00668
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0066c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00670
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0067c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00680
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00684
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00688
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00690
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00698
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0069c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x33333333,                                                  // 4 random values                             /// 006a4
			 0xff800000,                                                  // -inf                                        /// 006a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x4b000000,                                                  // 8388608.0                                   /// 006dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00700
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0c700000,                                                  // Leading 1s:                                 /// 00708
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0070c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0071c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00720
			 0xcb000000,                                                  // -8388608.0                                  /// 00724
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0072c
			 0x33333333,                                                  // 4 random values                             /// 00730
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0073c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00744
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0074c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00750
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00758
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0075c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00760
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00764
			 0x55555555,                                                  // 4 random values                             /// 00768
			 0x55555555,                                                  // 4 random values                             /// 0076c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00774
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0077c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00780
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00784
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00788
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00798
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0079c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x00000000,                                                  // zero                                        /// 007d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00800
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00804
			 0x0e000007,                                                  // Trailing 1s:                                /// 00808
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0080c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00810
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00820
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00828
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0082c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00834
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0083c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00844
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00848
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0084c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00850
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00854
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0085c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0086c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00874
			 0xbf028f5c,                                                  // -0.51                                       /// 00878
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0087c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00880
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00884
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00888
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0088c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00890
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00894
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008b0
			 0x3f028f5c,                                                  // 0.51                                        /// 008b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00904
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00908
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00910
			 0xff800000,                                                  // -inf                                        /// 00914
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00918
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0091c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00924
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0e000007,                                                  // Trailing 1s:                                /// 0092c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00930
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00934
			 0x4b000000,                                                  // 8388608.0                                   /// 00938
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x4b000000,                                                  // 8388608.0                                   /// 00944
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00948
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0094c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00954
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0095c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00960
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00964
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00968
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0096c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00970
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00978
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0097c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00980
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00984
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00988
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00990
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00998
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0099c
			 0xff800000,                                                  // -inf                                        /// 009a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009cc
			 0xff800000,                                                  // -inf                                        /// 009d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 009d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a14
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a44
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a74
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aa4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aa8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ab0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ab4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ab8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00abc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ac0
			 0x33333333,                                                  // 4 random values                             /// 00ac4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ad0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ad4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ad8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00adc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ae4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ae8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00af0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00af4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00af8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b20
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0xffc00001,                                                  // -signaling NaN                              /// 00b8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b90
			 0x00000000,                                                  // zero                                        /// 00b94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ba0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ba4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ba8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bbc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc8
			 0xff800000,                                                  // -inf                                        /// 00bcc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bd4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bd8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bdc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00be4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bf0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bf8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bfc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c00
			 0x7f800000,                                                  // inf                                         /// 00c04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c0c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c20
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c38
			 0x7f800000,                                                  // inf                                         /// 00c3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c48
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c58
			 0x4b000000,                                                  // 8388608.0                                   /// 00c5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c68
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c6c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c70
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c80
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ca0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ca4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cb0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cb4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cb8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cc4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cc8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ccc
			 0xbf028f5c,                                                  // -0.51                                       /// 00cd0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cd4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cd8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ce4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ce8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cec
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cf8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cfc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d00
			 0xffc00001,                                                  // -signaling NaN                              /// 00d04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d08
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x7fc00001,                                                  // signaling NaN                               /// 00d28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d38
			 0xbf028f5c,                                                  // -0.51                                       /// 00d3c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d44
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x55555555,                                                  // 4 random values                             /// 00d4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d58
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d68
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d70
			 0xffc00001,                                                  // -signaling NaN                              /// 00d74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d80
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d84
			 0x7fc00001,                                                  // signaling NaN                               /// 00d88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d90
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00da0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00db0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00db4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00db8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dbc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dc0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dc8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dd4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dd8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00de4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e28
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e30
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e44
			 0x80000000,                                                  // -zero                                       /// 00e48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e60
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80000000,                                                  // -zero                                       /// 00e7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e88
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ea0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ea4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00eb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eb8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ec0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ec4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ec8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ecc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ed8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00edc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ee0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ee4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ee8
			 0x7f800000,                                                  // inf                                         /// 00eec
			 0xff800000,                                                  // -inf                                        /// 00ef0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00efc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f14
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f4c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f54
			 0xcb000000,                                                  // -8388608.0                                  /// 00f58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f84
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f9c
			 0xff800000,                                                  // -inf                                        /// 00fa0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fbc
			 0xff800000,                                                  // -inf                                        /// 00fc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fc4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fcc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fdc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fe0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ff8
			 0x00000001                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xffc00001,                                                  // -signaling NaN                              /// 00000
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00004
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0000c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00014
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0x0c400000,                                                  // Leading 1s:                                 /// 0001c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00020
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00024
			 0x0c780000,                                                  // Leading 1s:                                 /// 00028
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0xcb000000,                                                  // -8388608.0                                  /// 00040
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00044
			 0x0c400000,                                                  // Leading 1s:                                 /// 00048
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0004c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00054
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0005c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00060
			 0x0c700000,                                                  // Leading 1s:                                 /// 00064
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00070
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00074
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00080
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00088
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0008c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00098
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0009c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000c8
			 0x55555555,                                                  // 4 random values                             /// 000cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000ec
			 0x3f028f5c,                                                  // 0.51                                        /// 000f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00100
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00104
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00108
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0010c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00110
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00114
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00118
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0011c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00120
			 0x3f028f5c,                                                  // 0.51                                        /// 00124
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00128
			 0x0e000003,                                                  // Trailing 1s:                                /// 0012c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00130
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0c780000,                                                  // Leading 1s:                                 /// 00138
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0013c
			 0x33333333,                                                  // 4 random values                             /// 00140
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00148
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0xff800000,                                                  // -inf                                        /// 00150
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00154
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00158
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0015c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x7f800000,                                                  // inf                                         /// 00164
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00168
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00170
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00174
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00178
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00198
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0xffc00001,                                                  // -signaling NaN                              /// 001ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0x55555555,                                                  // 4 random values                             /// 001e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001ec
			 0x3f028f5c,                                                  // 0.51                                        /// 001f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00200
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00204
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0020c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00210
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00218
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0021c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00220
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00224
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00228
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0022c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00230
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00234
			 0x0c780000,                                                  // Leading 1s:                                 /// 00238
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00240
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00248
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0024c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00250
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0xcb000000,                                                  // -8388608.0                                  /// 00258
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0025c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00260
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00264
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00268
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0026c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00270
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00274
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00278
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0027c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00280
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00284
			 0x4b000000,                                                  // 8388608.0                                   /// 00288
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0028c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00294
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00298
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0029c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0xbf028f5c,                                                  // -0.51                                       /// 002d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00304
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00314
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00318
			 0x7f800000,                                                  // inf                                         /// 0031c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00320
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00328
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00334
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00340
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00348
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00350
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00354
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00358
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0035c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00364
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00368
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0036c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00370
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00374
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00378
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0037c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00380
			 0x0e000003,                                                  // Trailing 1s:                                /// 00384
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00388
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0038c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00394
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0039c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003c4
			 0x55555555,                                                  // 4 random values                             /// 003c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 003d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 003dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003e8
			 0xbf028f5c,                                                  // -0.51                                       /// 003ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00400
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00404
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00408
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00414
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0xbf028f5c,                                                  // -0.51                                       /// 0041c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00420
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00424
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00430
			 0x80011111,                                                  // -9.7958E-41                                 /// 00434
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00438
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00440
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00444
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00448
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0044c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00450
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00454
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0045c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00460
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0046c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00470
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00474
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00478
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0xff800000,                                                  // -inf                                        /// 00480
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00484
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00488
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0048c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00490
			 0x0e000007,                                                  // Trailing 1s:                                /// 00494
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00498
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004e4
			 0x3f028f5c,                                                  // 0.51                                        /// 004e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00504
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00508
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0050c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00510
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00514
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00518
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00520
			 0x4b000000,                                                  // 8388608.0                                   /// 00524
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00530
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00534
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00538
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0053c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00540
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00544
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00548
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0054c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00550
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0055c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00560
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00564
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00568
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0056c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0057c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00580
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00584
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00588
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0058c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00590
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00594
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0059c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0xffc00001,                                                  // -signaling NaN                              /// 005f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00600
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00608
			 0x7fc00001,                                                  // signaling NaN                               /// 0060c
			 0x80000000,                                                  // -zero                                       /// 00610
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00614
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00618
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0061c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00620
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x4b000000,                                                  // 8388608.0                                   /// 00628
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0062c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00634
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0063c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00640
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00644
			 0xff800000,                                                  // -inf                                        /// 00648
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0064c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00654
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00658
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00660
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00664
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00670
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00674
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00678
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0067c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00680
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00684
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00688
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0068c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00690
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00694
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0069c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 006d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006e0
			 0x4b000000,                                                  // 8388608.0                                   /// 006e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00700
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00704
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00708
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00710
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00718
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x7fc00001,                                                  // signaling NaN                               /// 00720
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00724
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00728
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0072c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00730
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00734
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00738
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00740
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00748
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0074c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00750
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00758
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0075c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00764
			 0x0c780000,                                                  // Leading 1s:                                 /// 00768
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00774
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0077c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00780
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0c600000,                                                  // Leading 1s:                                 /// 0078c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00794
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00798
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00804
			 0x0e000007,                                                  // Trailing 1s:                                /// 00808
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0080c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00011111,                                                  // 9.7958E-41                                  /// 00818
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0081c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00824
			 0x0e000003,                                                  // Trailing 1s:                                /// 00828
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00830
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00834
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00000000,                                                  // zero                                        /// 0083c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00840
			 0x80000000,                                                  // -zero                                       /// 00844
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00848
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0084c
			 0x4b000000,                                                  // 8388608.0                                   /// 00850
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00858
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00860
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00864
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0086c
			 0x00000000,                                                  // zero                                        /// 00870
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00874
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00878
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00880
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0088c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00890
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00894
			 0x0c600000,                                                  // Leading 1s:                                 /// 00898
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0089c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008cc
			 0xbf028f5c,                                                  // -0.51                                       /// 008d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00908
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0091c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00920
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00924
			 0x0c400000,                                                  // Leading 1s:                                 /// 00928
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00000000,                                                  // zero                                        /// 00930
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00940
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00948
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00958
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0095c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00964
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00968
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00970
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00974
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0097c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00980
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00984
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00988
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0098c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0e000003,                                                  // Trailing 1s:                                /// 00998
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0099c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0xbf028f5c,                                                  // -0.51                                       /// 009c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 009cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 009f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a00
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a04
			 0x3f028f5c,                                                  // 0.51                                        /// 00a08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a0c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a34
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a38
			 0xbf028f5c,                                                  // -0.51                                       /// 00a3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a48
			 0x3f028f5c,                                                  // 0.51                                        /// 00a4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x55555555,                                                  // 4 random values                             /// 00a54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a98
			 0x00000000,                                                  // zero                                        /// 00a9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aa8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ab4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ac0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00acc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ad8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00adc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00af0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00af4
			 0x00000000,                                                  // zero                                        /// 00af8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00afc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b28
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b38
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b44
			 0xbf028f5c,                                                  // -0.51                                       /// 00b48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b50
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b78
			 0x80000000,                                                  // -zero                                       /// 00b7c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b9c
			 0x00000000,                                                  // zero                                        /// 00ba0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ba8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bb8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bc0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bc8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bcc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bd4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bd8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00be0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bf0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bf4
			 0xff800000,                                                  // -inf                                        /// 00bf8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bfc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c18
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c48
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c5c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c60
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x80000000,                                                  // -zero                                       /// 00c6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c94
			 0x3f028f5c,                                                  // 0.51                                        /// 00c98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ca0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ca4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ca8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cb8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x55555555,                                                  // 4 random values                             /// 00cc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cc8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cd0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00000000,                                                  // zero                                        /// 00cdc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ce8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cf4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cf8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cfc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d40
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d4c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d58
			 0x00000000,                                                  // zero                                        /// 00d5c
			 0x7f800000,                                                  // inf                                         /// 00d60
			 0x55555555,                                                  // 4 random values                             /// 00d64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00da4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00db8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x3f028f5c,                                                  // 0.51                                        /// 00dc0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dd0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dd4
			 0xcb000000,                                                  // -8388608.0                                  /// 00dd8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ddc
			 0x4b000000,                                                  // 8388608.0                                   /// 00de0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00df0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00df4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dfc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e2c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e30
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e3c
			 0x55555555,                                                  // 4 random values                             /// 00e40
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e48
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e68
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ea0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ea4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00eac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eb8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ebc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ec0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ec4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ecc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ed4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ed8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00edc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ee0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ee8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ef0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ef8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f00
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f1c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f20
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0xffc00001,                                                  // -signaling NaN                              /// 00f2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f48
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f64
			 0x4b000000,                                                  // 8388608.0                                   /// 00f68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f94
			 0x00000000,                                                  // zero                                        /// 00f98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fa4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fa8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fb4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fc4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fe8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ff0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ff4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ff8
			 0x00000002                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x043c1c7d, /// 0x0
			 0x02ce6943, /// 0x4
			 0x51f5a1a7, /// 0x8
			 0x8aadaee0, /// 0xc
			 0x4d08bb72, /// 0x10
			 0x0ec24a8c, /// 0x14
			 0xb773cf20, /// 0x18
			 0xd645c08b, /// 0x1c
			 0xc9fcf90a, /// 0x20
			 0xaa51e5de, /// 0x24
			 0xa1d285ea, /// 0x28
			 0x31303655, /// 0x2c
			 0x99001085, /// 0x30
			 0xc4c119a6, /// 0x34
			 0xe3e132cf, /// 0x38
			 0x9738b627, /// 0x3c
			 0xf35f9adf, /// 0x40
			 0xb6aa2edc, /// 0x44
			 0x038e1eec, /// 0x48
			 0xe84a09f9, /// 0x4c
			 0x601767e4, /// 0x50
			 0x05e4753a, /// 0x54
			 0x876b419e, /// 0x58
			 0x96f72342, /// 0x5c
			 0xf2ba6713, /// 0x60
			 0x6e977020, /// 0x64
			 0x573eed54, /// 0x68
			 0xb4810c96, /// 0x6c
			 0x163a870b, /// 0x70
			 0xbf5fa354, /// 0x74
			 0x7de6e4e0, /// 0x78
			 0xc13f4c08, /// 0x7c
			 0xcac59459, /// 0x80
			 0x15c19711, /// 0x84
			 0xd16d0c0c, /// 0x88
			 0x0b7b45c5, /// 0x8c
			 0xe3d9eacf, /// 0x90
			 0xb084d2f9, /// 0x94
			 0x507037ce, /// 0x98
			 0x2d183aa5, /// 0x9c
			 0xf22c9f49, /// 0xa0
			 0x45439295, /// 0xa4
			 0xa76ffcbe, /// 0xa8
			 0xa883dbbe, /// 0xac
			 0xda5d6598, /// 0xb0
			 0x77ebf9d1, /// 0xb4
			 0x534a101b, /// 0xb8
			 0xe46f6c53, /// 0xbc
			 0xa4d166d2, /// 0xc0
			 0x0bca838a, /// 0xc4
			 0x1962f7c7, /// 0xc8
			 0x64ec80c3, /// 0xcc
			 0xa889b669, /// 0xd0
			 0xf11f5a68, /// 0xd4
			 0xea57710e, /// 0xd8
			 0xa694a0b5, /// 0xdc
			 0x1e93ce05, /// 0xe0
			 0x46c6deab, /// 0xe4
			 0x75f3e39d, /// 0xe8
			 0x7def6978, /// 0xec
			 0x08a71a81, /// 0xf0
			 0x1e6001e0, /// 0xf4
			 0x19c6af70, /// 0xf8
			 0xa3653a5f, /// 0xfc
			 0x81998e35, /// 0x100
			 0x9c17a5e3, /// 0x104
			 0x31bd0689, /// 0x108
			 0xd25c8a79, /// 0x10c
			 0x3c601aea, /// 0x110
			 0x4f7d2152, /// 0x114
			 0xb6b4732e, /// 0x118
			 0x5d0e4b44, /// 0x11c
			 0xbbaf1a55, /// 0x120
			 0xe33a8d54, /// 0x124
			 0x10c04253, /// 0x128
			 0x0066fed5, /// 0x12c
			 0x04b78eba, /// 0x130
			 0x1bcebc21, /// 0x134
			 0xdbca115a, /// 0x138
			 0xba6b0023, /// 0x13c
			 0xf1c86d1e, /// 0x140
			 0x50e12d57, /// 0x144
			 0x439e1fcb, /// 0x148
			 0xbdf34f33, /// 0x14c
			 0xd6b48e01, /// 0x150
			 0xd0279cf7, /// 0x154
			 0xc5cef31e, /// 0x158
			 0xbacd7742, /// 0x15c
			 0xa5298ec2, /// 0x160
			 0x2662b429, /// 0x164
			 0xc5140a0a, /// 0x168
			 0xe4790165, /// 0x16c
			 0x2364a10c, /// 0x170
			 0xd19a3bc5, /// 0x174
			 0x231a93e2, /// 0x178
			 0x5440be4d, /// 0x17c
			 0x2f6fefd4, /// 0x180
			 0xab36ece4, /// 0x184
			 0x623d3c9d, /// 0x188
			 0xf309c940, /// 0x18c
			 0xfb181fac, /// 0x190
			 0x05a314b5, /// 0x194
			 0xedad5825, /// 0x198
			 0x827d99e6, /// 0x19c
			 0x9c287b78, /// 0x1a0
			 0x85f91fea, /// 0x1a4
			 0x640e0a77, /// 0x1a8
			 0x68c63949, /// 0x1ac
			 0x2792092c, /// 0x1b0
			 0xc680dd9f, /// 0x1b4
			 0x002af704, /// 0x1b8
			 0x2880dce6, /// 0x1bc
			 0x16731cf9, /// 0x1c0
			 0x43da05ac, /// 0x1c4
			 0xe968bc76, /// 0x1c8
			 0x0e75e9b5, /// 0x1cc
			 0xcc50bf6c, /// 0x1d0
			 0x5fc8bccd, /// 0x1d4
			 0x6aa4ca71, /// 0x1d8
			 0xe217ce66, /// 0x1dc
			 0x105c4631, /// 0x1e0
			 0xb02b9934, /// 0x1e4
			 0x358a3596, /// 0x1e8
			 0x662380f4, /// 0x1ec
			 0x10bc4647, /// 0x1f0
			 0x8d93b614, /// 0x1f4
			 0xd6a671ef, /// 0x1f8
			 0xeb7c979b, /// 0x1fc
			 0x2286b98d, /// 0x200
			 0x4e82e2a3, /// 0x204
			 0xfc087708, /// 0x208
			 0x0472bb15, /// 0x20c
			 0x0221aa6a, /// 0x210
			 0x3360665f, /// 0x214
			 0x36c3d94e, /// 0x218
			 0x38e88237, /// 0x21c
			 0x463c26b8, /// 0x220
			 0x25423f78, /// 0x224
			 0x47a1eaa3, /// 0x228
			 0x653561de, /// 0x22c
			 0x2d45f11f, /// 0x230
			 0x9ab338cc, /// 0x234
			 0x954c0dc8, /// 0x238
			 0x94f46d0a, /// 0x23c
			 0xe3c3b0ba, /// 0x240
			 0x692035f1, /// 0x244
			 0x4333878f, /// 0x248
			 0xabfbdc75, /// 0x24c
			 0x1d00e4d1, /// 0x250
			 0xd6d7336b, /// 0x254
			 0x63b79d21, /// 0x258
			 0xc7018882, /// 0x25c
			 0xc188ce3e, /// 0x260
			 0x297c1da0, /// 0x264
			 0x126c6134, /// 0x268
			 0x5304e379, /// 0x26c
			 0xd2f42b4d, /// 0x270
			 0xcd913dee, /// 0x274
			 0x5639f544, /// 0x278
			 0xf786e13f, /// 0x27c
			 0x6fa34df8, /// 0x280
			 0x262a44c4, /// 0x284
			 0x308600bf, /// 0x288
			 0x161f172f, /// 0x28c
			 0xb67046d0, /// 0x290
			 0xf9f47e3f, /// 0x294
			 0x5da190da, /// 0x298
			 0xa38dd375, /// 0x29c
			 0x4fa33f49, /// 0x2a0
			 0x8c471548, /// 0x2a4
			 0xdc31cd32, /// 0x2a8
			 0x09ba07b9, /// 0x2ac
			 0x38fe16d2, /// 0x2b0
			 0xf0deaf49, /// 0x2b4
			 0x77752a9a, /// 0x2b8
			 0x4c962b94, /// 0x2bc
			 0x8939d517, /// 0x2c0
			 0x85b3da94, /// 0x2c4
			 0xcdfe4120, /// 0x2c8
			 0x6e8a8bcd, /// 0x2cc
			 0x5c016645, /// 0x2d0
			 0xe25648d6, /// 0x2d4
			 0x5b1e1e31, /// 0x2d8
			 0xb02a76cb, /// 0x2dc
			 0xb7b10b84, /// 0x2e0
			 0xc669ff50, /// 0x2e4
			 0xa9770a77, /// 0x2e8
			 0x49ec1b54, /// 0x2ec
			 0x39f0e3f8, /// 0x2f0
			 0xfca7903d, /// 0x2f4
			 0xdc7a85ce, /// 0x2f8
			 0xe874e783, /// 0x2fc
			 0x88fb6fef, /// 0x300
			 0xe90cde2f, /// 0x304
			 0x14cb5132, /// 0x308
			 0xeda354bd, /// 0x30c
			 0x6de54c01, /// 0x310
			 0xe74f715b, /// 0x314
			 0x390ba02f, /// 0x318
			 0x90e61fda, /// 0x31c
			 0x879b7a1f, /// 0x320
			 0x7409adce, /// 0x324
			 0xd946806d, /// 0x328
			 0xed0b38f9, /// 0x32c
			 0x9437659f, /// 0x330
			 0xac5813b1, /// 0x334
			 0xf006eb82, /// 0x338
			 0xee161ffa, /// 0x33c
			 0x17a0f363, /// 0x340
			 0x86646eac, /// 0x344
			 0x91c4b5ac, /// 0x348
			 0x14152a56, /// 0x34c
			 0x7d17719f, /// 0x350
			 0x59b849bb, /// 0x354
			 0x1bbfef49, /// 0x358
			 0x4052c30c, /// 0x35c
			 0x6b7c008c, /// 0x360
			 0xdb76a3b2, /// 0x364
			 0x70d12d2a, /// 0x368
			 0x6169c3f7, /// 0x36c
			 0x4974f756, /// 0x370
			 0x5932c341, /// 0x374
			 0x8fb68e30, /// 0x378
			 0xa7251a52, /// 0x37c
			 0xbcb2dc1b, /// 0x380
			 0x3da49ad2, /// 0x384
			 0xfd6afd9e, /// 0x388
			 0xae965b4e, /// 0x38c
			 0xc3c5a095, /// 0x390
			 0xc421121d, /// 0x394
			 0x27003e5a, /// 0x398
			 0x0d3f81b7, /// 0x39c
			 0x63a7ba66, /// 0x3a0
			 0x325da807, /// 0x3a4
			 0xa71a872f, /// 0x3a8
			 0x261d387d, /// 0x3ac
			 0x2b235893, /// 0x3b0
			 0xd732abb9, /// 0x3b4
			 0x26c631ac, /// 0x3b8
			 0x7e65a42d, /// 0x3bc
			 0x9561a6c4, /// 0x3c0
			 0x71e824e1, /// 0x3c4
			 0x86d956e5, /// 0x3c8
			 0x103a79b3, /// 0x3cc
			 0x27545a22, /// 0x3d0
			 0x0523f79a, /// 0x3d4
			 0x495d8eeb, /// 0x3d8
			 0x6c2a5677, /// 0x3dc
			 0xbdc3d8dc, /// 0x3e0
			 0x02e4585c, /// 0x3e4
			 0xf732ed58, /// 0x3e8
			 0x64799e82, /// 0x3ec
			 0xabe0619d, /// 0x3f0
			 0x5812930c, /// 0x3f4
			 0x4d3e8a9c, /// 0x3f8
			 0x441b6260, /// 0x3fc
			 0x8d65d07a, /// 0x400
			 0x8c96f4ae, /// 0x404
			 0x876e890a, /// 0x408
			 0xecdca564, /// 0x40c
			 0x296114d4, /// 0x410
			 0xfa1779fd, /// 0x414
			 0x89209e78, /// 0x418
			 0xc90ed4ce, /// 0x41c
			 0x9d135cac, /// 0x420
			 0x6f9572f2, /// 0x424
			 0xd3bf31ac, /// 0x428
			 0x2d80da40, /// 0x42c
			 0x9b4932a1, /// 0x430
			 0xe852a395, /// 0x434
			 0x1418384b, /// 0x438
			 0x1d24b3b3, /// 0x43c
			 0xea621ea5, /// 0x440
			 0xfd5bc0df, /// 0x444
			 0x3dead653, /// 0x448
			 0x0e8bd551, /// 0x44c
			 0x328d5dee, /// 0x450
			 0x968546ca, /// 0x454
			 0x131251e1, /// 0x458
			 0xb1a3a2f6, /// 0x45c
			 0x289ba27b, /// 0x460
			 0x4d8e266a, /// 0x464
			 0xd154efa4, /// 0x468
			 0xa9f9acf5, /// 0x46c
			 0x720284b0, /// 0x470
			 0xe1ad8dda, /// 0x474
			 0xf3780f4b, /// 0x478
			 0xfe8f54ec, /// 0x47c
			 0xbfb87c28, /// 0x480
			 0x68f1b4f7, /// 0x484
			 0x642dd74c, /// 0x488
			 0xdecc8631, /// 0x48c
			 0x54f879d3, /// 0x490
			 0x8218d834, /// 0x494
			 0x002a5472, /// 0x498
			 0x9ea4d707, /// 0x49c
			 0xbd5ac996, /// 0x4a0
			 0xaf275c5f, /// 0x4a4
			 0xb6803f3e, /// 0x4a8
			 0x68ce5477, /// 0x4ac
			 0xbe7808db, /// 0x4b0
			 0x1baf9c3e, /// 0x4b4
			 0x5beeec9f, /// 0x4b8
			 0x03e39ffe, /// 0x4bc
			 0xf90d3bde, /// 0x4c0
			 0x023b1856, /// 0x4c4
			 0x7378ab73, /// 0x4c8
			 0x62cdcb2d, /// 0x4cc
			 0x0cdca5b4, /// 0x4d0
			 0x1c086ddf, /// 0x4d4
			 0x653dc8bc, /// 0x4d8
			 0x58f60db7, /// 0x4dc
			 0x3375d3c1, /// 0x4e0
			 0x272468c3, /// 0x4e4
			 0x9b7c55d6, /// 0x4e8
			 0xfc184ea8, /// 0x4ec
			 0x54b88960, /// 0x4f0
			 0x62e9de75, /// 0x4f4
			 0x60a50db7, /// 0x4f8
			 0x0de80fc3, /// 0x4fc
			 0xb1b058f0, /// 0x500
			 0xbce813c6, /// 0x504
			 0x02250a7b, /// 0x508
			 0x5f273c99, /// 0x50c
			 0x7aa7cf43, /// 0x510
			 0xe7974429, /// 0x514
			 0x434a2be0, /// 0x518
			 0x2a77f337, /// 0x51c
			 0x0679e967, /// 0x520
			 0x2b280266, /// 0x524
			 0xfa7fe214, /// 0x528
			 0xc699cb77, /// 0x52c
			 0xd18ab05f, /// 0x530
			 0x7675923d, /// 0x534
			 0xc79e40a1, /// 0x538
			 0x9df4d05a, /// 0x53c
			 0x6c56fc75, /// 0x540
			 0xf8af4545, /// 0x544
			 0x956f7b69, /// 0x548
			 0x58b51f91, /// 0x54c
			 0x1d5958e4, /// 0x550
			 0x60c80801, /// 0x554
			 0x83e1cf4d, /// 0x558
			 0x09ef3acf, /// 0x55c
			 0xa7c038f1, /// 0x560
			 0x04ea366f, /// 0x564
			 0x6fa9f56c, /// 0x568
			 0x23672344, /// 0x56c
			 0xea86d7bf, /// 0x570
			 0x3d92428a, /// 0x574
			 0xf70d7098, /// 0x578
			 0xd372c1bb, /// 0x57c
			 0x9384ee39, /// 0x580
			 0xb061eec9, /// 0x584
			 0x0026a982, /// 0x588
			 0x0e70f742, /// 0x58c
			 0xc54636dd, /// 0x590
			 0x177720d0, /// 0x594
			 0x3d0f5210, /// 0x598
			 0xa8b21ed6, /// 0x59c
			 0xcd7f9168, /// 0x5a0
			 0xdf5b996e, /// 0x5a4
			 0x314505dd, /// 0x5a8
			 0x6e82f585, /// 0x5ac
			 0x3bb06a97, /// 0x5b0
			 0x0dc743cc, /// 0x5b4
			 0x1a44de6d, /// 0x5b8
			 0xe4874d59, /// 0x5bc
			 0xdfef92c4, /// 0x5c0
			 0xf29f7864, /// 0x5c4
			 0x2edc0d3a, /// 0x5c8
			 0x043d5e29, /// 0x5cc
			 0xa887db64, /// 0x5d0
			 0x6fd10b92, /// 0x5d4
			 0xa86cbc76, /// 0x5d8
			 0x28c2822d, /// 0x5dc
			 0xb12c956b, /// 0x5e0
			 0x8e685361, /// 0x5e4
			 0xda0f4696, /// 0x5e8
			 0x07a92cbe, /// 0x5ec
			 0xca30e976, /// 0x5f0
			 0x8f6f239e, /// 0x5f4
			 0xab9c17dc, /// 0x5f8
			 0x1ab8f71e, /// 0x5fc
			 0x5bceced5, /// 0x600
			 0xafb8a1c2, /// 0x604
			 0xf6635a62, /// 0x608
			 0x817494b7, /// 0x60c
			 0x4aa0d06e, /// 0x610
			 0x60d4127a, /// 0x614
			 0xbe014a2f, /// 0x618
			 0xc68c76b5, /// 0x61c
			 0x7f11f2d6, /// 0x620
			 0xf016122a, /// 0x624
			 0x291b94c7, /// 0x628
			 0xa5352db6, /// 0x62c
			 0x315ac351, /// 0x630
			 0x00158221, /// 0x634
			 0x07c18c11, /// 0x638
			 0x26953ed1, /// 0x63c
			 0xb7b328df, /// 0x640
			 0xd343f66e, /// 0x644
			 0xf7056bdd, /// 0x648
			 0x2e99718b, /// 0x64c
			 0xb0fb9718, /// 0x650
			 0x38df8811, /// 0x654
			 0xbf1b4819, /// 0x658
			 0xfc98aafb, /// 0x65c
			 0xfde2482b, /// 0x660
			 0x3889d25f, /// 0x664
			 0xc592cea7, /// 0x668
			 0xe532a9ae, /// 0x66c
			 0x85c4c87f, /// 0x670
			 0x726d3850, /// 0x674
			 0x22ab569f, /// 0x678
			 0x114f3386, /// 0x67c
			 0xc19f4ab2, /// 0x680
			 0x0cf427db, /// 0x684
			 0x94de059b, /// 0x688
			 0x30623ac5, /// 0x68c
			 0x2986bfbd, /// 0x690
			 0x6278f084, /// 0x694
			 0x2b04d125, /// 0x698
			 0xddee7e9a, /// 0x69c
			 0xd90a87e3, /// 0x6a0
			 0x5e8a6b77, /// 0x6a4
			 0x4a33a575, /// 0x6a8
			 0x64352d04, /// 0x6ac
			 0xc3c3b676, /// 0x6b0
			 0x38aa4910, /// 0x6b4
			 0x273e7d26, /// 0x6b8
			 0xe8616d32, /// 0x6bc
			 0x51e01ac8, /// 0x6c0
			 0xeb63bb53, /// 0x6c4
			 0xab2aa87a, /// 0x6c8
			 0xf425209b, /// 0x6cc
			 0x5b122b1a, /// 0x6d0
			 0xe30365ed, /// 0x6d4
			 0x54836ef3, /// 0x6d8
			 0x1c7ec7fa, /// 0x6dc
			 0x88a7403f, /// 0x6e0
			 0x9991d6a2, /// 0x6e4
			 0xb627d001, /// 0x6e8
			 0xfd37bdbc, /// 0x6ec
			 0x96e928de, /// 0x6f0
			 0xcb42593c, /// 0x6f4
			 0x01db5259, /// 0x6f8
			 0xdce1a766, /// 0x6fc
			 0xdd69dd1d, /// 0x700
			 0x59529803, /// 0x704
			 0x676552de, /// 0x708
			 0xaa670653, /// 0x70c
			 0x0e1506b8, /// 0x710
			 0x4acc46e2, /// 0x714
			 0xf638e3e5, /// 0x718
			 0x432262e8, /// 0x71c
			 0x0bb03e3b, /// 0x720
			 0x606ca3dc, /// 0x724
			 0xf862bd8c, /// 0x728
			 0x6fd74752, /// 0x72c
			 0x3499111b, /// 0x730
			 0x3c64c372, /// 0x734
			 0x6969e5ba, /// 0x738
			 0x42e24f5b, /// 0x73c
			 0xab2d2f5c, /// 0x740
			 0x0a184f21, /// 0x744
			 0xa3e1be42, /// 0x748
			 0xeaad6600, /// 0x74c
			 0x7089bc55, /// 0x750
			 0xd73a2be8, /// 0x754
			 0x54ee9c06, /// 0x758
			 0x2ed7cc39, /// 0x75c
			 0x7488f299, /// 0x760
			 0xc75dbb8e, /// 0x764
			 0x3fc24696, /// 0x768
			 0xb41b093e, /// 0x76c
			 0xb8cc6632, /// 0x770
			 0x1f1fe560, /// 0x774
			 0xca42fc50, /// 0x778
			 0x56649f98, /// 0x77c
			 0x537932ad, /// 0x780
			 0x61d6be9f, /// 0x784
			 0x9ffbbc64, /// 0x788
			 0xfaaf352a, /// 0x78c
			 0xd13edb09, /// 0x790
			 0x800d230a, /// 0x794
			 0x427cc779, /// 0x798
			 0xbb5c4f32, /// 0x79c
			 0xd2eb80c6, /// 0x7a0
			 0x621e58c9, /// 0x7a4
			 0xfe7bfa53, /// 0x7a8
			 0xde0740d4, /// 0x7ac
			 0x500f4bc1, /// 0x7b0
			 0x67a1c0c7, /// 0x7b4
			 0x74401153, /// 0x7b8
			 0x1b6ac612, /// 0x7bc
			 0xe14ea767, /// 0x7c0
			 0xd79cc7c1, /// 0x7c4
			 0xc30f0d5f, /// 0x7c8
			 0x61e7cced, /// 0x7cc
			 0x8c93ba03, /// 0x7d0
			 0x5a2307e0, /// 0x7d4
			 0xeedb0904, /// 0x7d8
			 0x8758fa04, /// 0x7dc
			 0x02be9182, /// 0x7e0
			 0x52871044, /// 0x7e4
			 0xf5e83294, /// 0x7e8
			 0xc862aec6, /// 0x7ec
			 0xaa544574, /// 0x7f0
			 0xecb2fda9, /// 0x7f4
			 0x8894b43e, /// 0x7f8
			 0x623138a4, /// 0x7fc
			 0x908b4405, /// 0x800
			 0xc1ae764b, /// 0x804
			 0x21e03b8d, /// 0x808
			 0x873d33e9, /// 0x80c
			 0x613a6a9b, /// 0x810
			 0x154c5d8b, /// 0x814
			 0x2a9a76b8, /// 0x818
			 0xc4ab9861, /// 0x81c
			 0x6a53ab13, /// 0x820
			 0xd234db58, /// 0x824
			 0x3276d729, /// 0x828
			 0xf8e2e7bf, /// 0x82c
			 0x8ca1f8f0, /// 0x830
			 0x5254c1e4, /// 0x834
			 0xdaaeed9c, /// 0x838
			 0x5cf76fa8, /// 0x83c
			 0x86774e9a, /// 0x840
			 0xb0c353d4, /// 0x844
			 0x2b777d84, /// 0x848
			 0x8fb0e8c3, /// 0x84c
			 0x66c97f38, /// 0x850
			 0x5660e439, /// 0x854
			 0xfd87be1f, /// 0x858
			 0xf6eb220a, /// 0x85c
			 0xe2a3cf08, /// 0x860
			 0x2da907f4, /// 0x864
			 0x7e1b1775, /// 0x868
			 0x7265f5be, /// 0x86c
			 0xce86f65f, /// 0x870
			 0x7576140a, /// 0x874
			 0xf9635484, /// 0x878
			 0x65ec2b23, /// 0x87c
			 0x68e8fe1f, /// 0x880
			 0xc8fcedc7, /// 0x884
			 0x4a9cade6, /// 0x888
			 0x8901eecf, /// 0x88c
			 0xe999567a, /// 0x890
			 0x9dfe93bc, /// 0x894
			 0x5a7f1024, /// 0x898
			 0xe4093d9b, /// 0x89c
			 0x456857d4, /// 0x8a0
			 0x44a88bc6, /// 0x8a4
			 0xb13e062f, /// 0x8a8
			 0x4e12e6f4, /// 0x8ac
			 0xe89c7408, /// 0x8b0
			 0x4d549ad0, /// 0x8b4
			 0x855f5714, /// 0x8b8
			 0xb13a05d3, /// 0x8bc
			 0x3ee77961, /// 0x8c0
			 0x8245da91, /// 0x8c4
			 0x19b6040c, /// 0x8c8
			 0x5e23458e, /// 0x8cc
			 0xdeb5b6f4, /// 0x8d0
			 0x88748a2c, /// 0x8d4
			 0xbe35275a, /// 0x8d8
			 0x2f5ce1d6, /// 0x8dc
			 0x76fc15ae, /// 0x8e0
			 0xcc238d78, /// 0x8e4
			 0x07dc213f, /// 0x8e8
			 0x6496b346, /// 0x8ec
			 0xe7f99fbf, /// 0x8f0
			 0xcc9ec0f4, /// 0x8f4
			 0x3173f30a, /// 0x8f8
			 0xae07277e, /// 0x8fc
			 0x8bc40fd4, /// 0x900
			 0x6381a47e, /// 0x904
			 0x71540f58, /// 0x908
			 0x3b05797d, /// 0x90c
			 0x6545d034, /// 0x910
			 0x73535496, /// 0x914
			 0x46ad4fd3, /// 0x918
			 0xd9a0617d, /// 0x91c
			 0x1fac7ebc, /// 0x920
			 0x8c46c2d1, /// 0x924
			 0x47d83c2f, /// 0x928
			 0xbab8b7ae, /// 0x92c
			 0x2564aa39, /// 0x930
			 0xfd2ea515, /// 0x934
			 0x2be798a8, /// 0x938
			 0xb6de2876, /// 0x93c
			 0x3ffe5944, /// 0x940
			 0x99d2f2a7, /// 0x944
			 0xcc3d52e0, /// 0x948
			 0xae877e7f, /// 0x94c
			 0xbfec8b71, /// 0x950
			 0x4417bed5, /// 0x954
			 0xb1e67615, /// 0x958
			 0x4f9fd17d, /// 0x95c
			 0x88af8eb6, /// 0x960
			 0xa0cacc8b, /// 0x964
			 0xaf360adb, /// 0x968
			 0x6c799175, /// 0x96c
			 0xcea440d6, /// 0x970
			 0xda51f8c0, /// 0x974
			 0x30fc9c83, /// 0x978
			 0xd028d374, /// 0x97c
			 0xaed7a994, /// 0x980
			 0x7bf9cb1c, /// 0x984
			 0x3181da7b, /// 0x988
			 0x42c29dd8, /// 0x98c
			 0xa62469d8, /// 0x990
			 0xb20c05ce, /// 0x994
			 0x322bf12b, /// 0x998
			 0x5292acd5, /// 0x99c
			 0x379e035a, /// 0x9a0
			 0x30440666, /// 0x9a4
			 0x3b839cf9, /// 0x9a8
			 0xb29c355c, /// 0x9ac
			 0xfb63fb24, /// 0x9b0
			 0xd58b8c50, /// 0x9b4
			 0xc1594983, /// 0x9b8
			 0x1cc44074, /// 0x9bc
			 0x7d67e64a, /// 0x9c0
			 0xa3f5b113, /// 0x9c4
			 0x0c47f4c4, /// 0x9c8
			 0x8062df0e, /// 0x9cc
			 0x95ad09d6, /// 0x9d0
			 0x6939edf9, /// 0x9d4
			 0x79d82b63, /// 0x9d8
			 0x5cd1fe5d, /// 0x9dc
			 0x3e1b2060, /// 0x9e0
			 0x251175b4, /// 0x9e4
			 0xc1bf53be, /// 0x9e8
			 0xcfc99bc0, /// 0x9ec
			 0x1abdd386, /// 0x9f0
			 0x3dc8ebe8, /// 0x9f4
			 0x77fa1b0b, /// 0x9f8
			 0x841bcf0b, /// 0x9fc
			 0x9961b8ef, /// 0xa00
			 0xce10a747, /// 0xa04
			 0x6be9b374, /// 0xa08
			 0x97f052c7, /// 0xa0c
			 0x71dec3c7, /// 0xa10
			 0x6be737de, /// 0xa14
			 0xe92954e2, /// 0xa18
			 0x3be3a88f, /// 0xa1c
			 0x99483acc, /// 0xa20
			 0xa969d847, /// 0xa24
			 0x7bf771d7, /// 0xa28
			 0x0282a1f1, /// 0xa2c
			 0xbad79003, /// 0xa30
			 0xc714c63d, /// 0xa34
			 0x8f37b30a, /// 0xa38
			 0x8aedd486, /// 0xa3c
			 0x6866d6a8, /// 0xa40
			 0x9cd90bd0, /// 0xa44
			 0x6223a227, /// 0xa48
			 0xa145cac1, /// 0xa4c
			 0xb3457a80, /// 0xa50
			 0xe1b56d64, /// 0xa54
			 0x7baa0f72, /// 0xa58
			 0x3cfcc522, /// 0xa5c
			 0x06673c4a, /// 0xa60
			 0xfe593b96, /// 0xa64
			 0x4d4728c8, /// 0xa68
			 0x4120d17e, /// 0xa6c
			 0x1a96bcaa, /// 0xa70
			 0x877e7c06, /// 0xa74
			 0x3843ce63, /// 0xa78
			 0x727dc435, /// 0xa7c
			 0xd3dac700, /// 0xa80
			 0x26e2d378, /// 0xa84
			 0xbcaf60de, /// 0xa88
			 0xfc0a23a1, /// 0xa8c
			 0x3635f616, /// 0xa90
			 0x73c70f02, /// 0xa94
			 0xda846fd8, /// 0xa98
			 0x806b7e05, /// 0xa9c
			 0x4e15732c, /// 0xaa0
			 0xe844a6b3, /// 0xaa4
			 0x38752644, /// 0xaa8
			 0x11310e7c, /// 0xaac
			 0x262b1b21, /// 0xab0
			 0x49e6a8de, /// 0xab4
			 0x989ae7f1, /// 0xab8
			 0xd58776a5, /// 0xabc
			 0xb7a873c5, /// 0xac0
			 0xf8ae14f5, /// 0xac4
			 0x5388da57, /// 0xac8
			 0x55ea0d1f, /// 0xacc
			 0x153b6c7f, /// 0xad0
			 0xa49a485b, /// 0xad4
			 0x94978e92, /// 0xad8
			 0xa04e0ea5, /// 0xadc
			 0x5160a1b4, /// 0xae0
			 0x78ae2dcc, /// 0xae4
			 0x16675545, /// 0xae8
			 0x2facbab8, /// 0xaec
			 0xc9478e16, /// 0xaf0
			 0x055a9890, /// 0xaf4
			 0xf2619ac9, /// 0xaf8
			 0xb68c37d6, /// 0xafc
			 0x3d773c45, /// 0xb00
			 0x1e097eee, /// 0xb04
			 0xc21f81b5, /// 0xb08
			 0x2a1d2651, /// 0xb0c
			 0x65583423, /// 0xb10
			 0x5940518a, /// 0xb14
			 0x1d081fa8, /// 0xb18
			 0x0493f7eb, /// 0xb1c
			 0xc7376625, /// 0xb20
			 0xf5c0a684, /// 0xb24
			 0xbbf67362, /// 0xb28
			 0xfc517340, /// 0xb2c
			 0xd5382e38, /// 0xb30
			 0x8af45a04, /// 0xb34
			 0x0e82707d, /// 0xb38
			 0x9c4b0b85, /// 0xb3c
			 0x00f4648c, /// 0xb40
			 0xb5da6b0c, /// 0xb44
			 0xda3647c5, /// 0xb48
			 0xdcf783b9, /// 0xb4c
			 0x865ca31b, /// 0xb50
			 0x2f5abaf2, /// 0xb54
			 0x1753c733, /// 0xb58
			 0x4fc02539, /// 0xb5c
			 0xb9d78080, /// 0xb60
			 0x6df197e4, /// 0xb64
			 0xdaee3303, /// 0xb68
			 0x789cdd3e, /// 0xb6c
			 0xe33cc98e, /// 0xb70
			 0x768e0634, /// 0xb74
			 0xcab98741, /// 0xb78
			 0x7120bb15, /// 0xb7c
			 0xfade6357, /// 0xb80
			 0xade6c132, /// 0xb84
			 0xcfaaa181, /// 0xb88
			 0x938b1a76, /// 0xb8c
			 0x8606c22c, /// 0xb90
			 0x5320c012, /// 0xb94
			 0x13740cc2, /// 0xb98
			 0x86843124, /// 0xb9c
			 0x950fae46, /// 0xba0
			 0x83ee9186, /// 0xba4
			 0xb2d22b51, /// 0xba8
			 0x9707b793, /// 0xbac
			 0x2085d4a7, /// 0xbb0
			 0xb3d81920, /// 0xbb4
			 0xb82d2c2f, /// 0xbb8
			 0xb6587122, /// 0xbbc
			 0x3d149b74, /// 0xbc0
			 0x4d9b5029, /// 0xbc4
			 0x7206557c, /// 0xbc8
			 0x27a68cca, /// 0xbcc
			 0x7a4c9cfd, /// 0xbd0
			 0x7a7f0285, /// 0xbd4
			 0x9c12e7e8, /// 0xbd8
			 0xfa26d249, /// 0xbdc
			 0xf072bc34, /// 0xbe0
			 0xc27ad761, /// 0xbe4
			 0x9e1da352, /// 0xbe8
			 0x39b69e8c, /// 0xbec
			 0xd37804ed, /// 0xbf0
			 0x94c9bbe4, /// 0xbf4
			 0x623e70ea, /// 0xbf8
			 0x63b59613, /// 0xbfc
			 0x1701e6a0, /// 0xc00
			 0x62a06f72, /// 0xc04
			 0x93bb6218, /// 0xc08
			 0x6280cb24, /// 0xc0c
			 0x3b771abb, /// 0xc10
			 0x7ab90b9e, /// 0xc14
			 0xb7cb6851, /// 0xc18
			 0x3056f67c, /// 0xc1c
			 0xbef0fcd0, /// 0xc20
			 0x024d8d2b, /// 0xc24
			 0xe781187a, /// 0xc28
			 0x8dc64985, /// 0xc2c
			 0xb18abe65, /// 0xc30
			 0xd23a6570, /// 0xc34
			 0xab2e7645, /// 0xc38
			 0x1f3fd687, /// 0xc3c
			 0xb8803daa, /// 0xc40
			 0x0aa3a732, /// 0xc44
			 0x9851927c, /// 0xc48
			 0xc9b1fa98, /// 0xc4c
			 0x119e8f06, /// 0xc50
			 0xb429789a, /// 0xc54
			 0x7b1b51ed, /// 0xc58
			 0xf93f5898, /// 0xc5c
			 0x2c299953, /// 0xc60
			 0x287a4fd0, /// 0xc64
			 0x84c1766e, /// 0xc68
			 0xc342065b, /// 0xc6c
			 0xc80ef838, /// 0xc70
			 0xe3e824df, /// 0xc74
			 0x4e14aed4, /// 0xc78
			 0xc460ef18, /// 0xc7c
			 0x632f18ba, /// 0xc80
			 0xa49fba1c, /// 0xc84
			 0xdea06b60, /// 0xc88
			 0xa0538526, /// 0xc8c
			 0x7fc29478, /// 0xc90
			 0xa0a68c94, /// 0xc94
			 0x3d5a2375, /// 0xc98
			 0x6d11eaef, /// 0xc9c
			 0x55d59452, /// 0xca0
			 0xd0500f83, /// 0xca4
			 0x4fd23159, /// 0xca8
			 0x18eb1820, /// 0xcac
			 0xc54dd0f5, /// 0xcb0
			 0x2ca52578, /// 0xcb4
			 0x36c2344b, /// 0xcb8
			 0x4744eb51, /// 0xcbc
			 0x9227a5e0, /// 0xcc0
			 0xf599a789, /// 0xcc4
			 0x11c52c7a, /// 0xcc8
			 0xb23b9451, /// 0xccc
			 0x86d2480e, /// 0xcd0
			 0xb4c852e1, /// 0xcd4
			 0x5eea26e5, /// 0xcd8
			 0x355bfb6f, /// 0xcdc
			 0x74fe1996, /// 0xce0
			 0x89f76726, /// 0xce4
			 0xa2067fd4, /// 0xce8
			 0x3e0eabb5, /// 0xcec
			 0x58089f43, /// 0xcf0
			 0x0d672bcb, /// 0xcf4
			 0x58dcf9e5, /// 0xcf8
			 0x7824aefe, /// 0xcfc
			 0x1566b7ca, /// 0xd00
			 0xe774c93d, /// 0xd04
			 0xacfcc55f, /// 0xd08
			 0x327ac84b, /// 0xd0c
			 0x0520a11c, /// 0xd10
			 0x0193f7ac, /// 0xd14
			 0x84842077, /// 0xd18
			 0x64dfb565, /// 0xd1c
			 0xc89ee859, /// 0xd20
			 0x6edaaa89, /// 0xd24
			 0x3669ffca, /// 0xd28
			 0x6cb43a7b, /// 0xd2c
			 0xd77c62e5, /// 0xd30
			 0x1b5e8fbe, /// 0xd34
			 0xa1d6a42c, /// 0xd38
			 0xa7195176, /// 0xd3c
			 0x07e98bc8, /// 0xd40
			 0x70fa7511, /// 0xd44
			 0x0cac0b68, /// 0xd48
			 0x395d8ec8, /// 0xd4c
			 0x872eff86, /// 0xd50
			 0xf4587404, /// 0xd54
			 0x5e33f698, /// 0xd58
			 0xae458fae, /// 0xd5c
			 0x300cbbef, /// 0xd60
			 0x4be22e6e, /// 0xd64
			 0xa421425f, /// 0xd68
			 0x694b4129, /// 0xd6c
			 0x2ccba644, /// 0xd70
			 0x14d4f260, /// 0xd74
			 0x7d52e0bf, /// 0xd78
			 0xa1bc91d8, /// 0xd7c
			 0xd808e12e, /// 0xd80
			 0x05237156, /// 0xd84
			 0x8c3b08c5, /// 0xd88
			 0xac573f15, /// 0xd8c
			 0x454e6377, /// 0xd90
			 0xd9555f28, /// 0xd94
			 0x137d141a, /// 0xd98
			 0xabf3fc4a, /// 0xd9c
			 0x7a2e2f67, /// 0xda0
			 0x822018e9, /// 0xda4
			 0xef26714a, /// 0xda8
			 0xd7c981d1, /// 0xdac
			 0x0a15ce52, /// 0xdb0
			 0x75a75608, /// 0xdb4
			 0x12aa7cac, /// 0xdb8
			 0xddad139f, /// 0xdbc
			 0xd4f42ca4, /// 0xdc0
			 0xf9afaa8b, /// 0xdc4
			 0xcdc40323, /// 0xdc8
			 0xfeb2a5b0, /// 0xdcc
			 0x84dda325, /// 0xdd0
			 0x9367cd37, /// 0xdd4
			 0xe3afb84a, /// 0xdd8
			 0x042192a6, /// 0xddc
			 0xdf72847b, /// 0xde0
			 0x0f73538a, /// 0xde4
			 0xf020809d, /// 0xde8
			 0xfdc1a3dc, /// 0xdec
			 0xcc2855bb, /// 0xdf0
			 0xdef1a3b4, /// 0xdf4
			 0xb49b37ae, /// 0xdf8
			 0x67366da3, /// 0xdfc
			 0xb3d8b947, /// 0xe00
			 0x65fb429c, /// 0xe04
			 0x9e53c886, /// 0xe08
			 0xd4cc3b7d, /// 0xe0c
			 0xdc7635c9, /// 0xe10
			 0xdad1233a, /// 0xe14
			 0x32c88364, /// 0xe18
			 0xc7915651, /// 0xe1c
			 0x4b6c1185, /// 0xe20
			 0x2c2c4ae3, /// 0xe24
			 0x2e66c47f, /// 0xe28
			 0x18ea6aa2, /// 0xe2c
			 0x867a879e, /// 0xe30
			 0x0f38a6f2, /// 0xe34
			 0xdc322482, /// 0xe38
			 0xca8b90d8, /// 0xe3c
			 0x38c96584, /// 0xe40
			 0xee51eea5, /// 0xe44
			 0x9b887946, /// 0xe48
			 0x7e50217f, /// 0xe4c
			 0x0653b769, /// 0xe50
			 0x4fbed766, /// 0xe54
			 0x5c67737b, /// 0xe58
			 0x4b285c3a, /// 0xe5c
			 0xeb994623, /// 0xe60
			 0xf8979700, /// 0xe64
			 0xcda212f8, /// 0xe68
			 0x5ecb0595, /// 0xe6c
			 0xfc271160, /// 0xe70
			 0x76585558, /// 0xe74
			 0xeba6eed1, /// 0xe78
			 0x0b53dccc, /// 0xe7c
			 0xd875d709, /// 0xe80
			 0x33ecaa90, /// 0xe84
			 0xf7bb905d, /// 0xe88
			 0x8545155d, /// 0xe8c
			 0x5d91e2be, /// 0xe90
			 0xdc153964, /// 0xe94
			 0x5e1b024f, /// 0xe98
			 0x517989b4, /// 0xe9c
			 0x6267df06, /// 0xea0
			 0xb959d71f, /// 0xea4
			 0x22892092, /// 0xea8
			 0x7dd2472c, /// 0xeac
			 0xd04684ea, /// 0xeb0
			 0x9c12443b, /// 0xeb4
			 0xf7bedf17, /// 0xeb8
			 0xcd4616b7, /// 0xebc
			 0x5aa53fd6, /// 0xec0
			 0xd691a1b6, /// 0xec4
			 0x05d36bb8, /// 0xec8
			 0x3781d41d, /// 0xecc
			 0xf0cb0471, /// 0xed0
			 0xb2c9ed8a, /// 0xed4
			 0xa7bda759, /// 0xed8
			 0xfaa8c647, /// 0xedc
			 0x3822493c, /// 0xee0
			 0xb2aaa6f3, /// 0xee4
			 0x72fb9946, /// 0xee8
			 0xb7da9ae9, /// 0xeec
			 0x50654dcf, /// 0xef0
			 0xb00469c5, /// 0xef4
			 0x6c9a0657, /// 0xef8
			 0xa3a87151, /// 0xefc
			 0x570a8526, /// 0xf00
			 0xe3769624, /// 0xf04
			 0x7d527352, /// 0xf08
			 0x65955f75, /// 0xf0c
			 0xb5308501, /// 0xf10
			 0xdf25c366, /// 0xf14
			 0x1596ca47, /// 0xf18
			 0xc845c5fa, /// 0xf1c
			 0x8eb07d10, /// 0xf20
			 0x1925d958, /// 0xf24
			 0xbe274146, /// 0xf28
			 0x7d403aaf, /// 0xf2c
			 0xe775fb73, /// 0xf30
			 0x529bd903, /// 0xf34
			 0x2a68f921, /// 0xf38
			 0x3b290166, /// 0xf3c
			 0xfdbd27f1, /// 0xf40
			 0x60b18750, /// 0xf44
			 0xd7b217c5, /// 0xf48
			 0x0e1bf211, /// 0xf4c
			 0xe54f2174, /// 0xf50
			 0x9acad34e, /// 0xf54
			 0xd628f9e0, /// 0xf58
			 0xa9ac1075, /// 0xf5c
			 0xa914dbfe, /// 0xf60
			 0xbe2064fc, /// 0xf64
			 0xb9cde8a5, /// 0xf68
			 0x698ff785, /// 0xf6c
			 0x62a373de, /// 0xf70
			 0x21b15b0a, /// 0xf74
			 0x648b42e3, /// 0xf78
			 0x76c74cb3, /// 0xf7c
			 0xc68187df, /// 0xf80
			 0x1ba601ed, /// 0xf84
			 0x1f11f173, /// 0xf88
			 0x74b44ab2, /// 0xf8c
			 0xc78c40e0, /// 0xf90
			 0x0f361099, /// 0xf94
			 0x6ccbe7ab, /// 0xf98
			 0xca9aeacd, /// 0xf9c
			 0x975be98c, /// 0xfa0
			 0x894c835d, /// 0xfa4
			 0x30205ec4, /// 0xfa8
			 0x4797ebc0, /// 0xfac
			 0x52aa39a3, /// 0xfb0
			 0xc269ebd6, /// 0xfb4
			 0xb1f40e75, /// 0xfb8
			 0xb2391794, /// 0xfbc
			 0x6afbe40f, /// 0xfc0
			 0xf2cb5072, /// 0xfc4
			 0x06b1b589, /// 0xfc8
			 0xe9651305, /// 0xfcc
			 0x2a7b7334, /// 0xfd0
			 0x2fc8fb6e, /// 0xfd4
			 0xa27b705a, /// 0xfd8
			 0x7672089c, /// 0xfdc
			 0xfa8e9451, /// 0xfe0
			 0x467773dd, /// 0xfe4
			 0xdfab5e7e, /// 0xfe8
			 0x7f78b817, /// 0xfec
			 0x2738db35, /// 0xff0
			 0x1d62660b, /// 0xff4
			 0xdfb943da, /// 0xff8
			 0x45db8229 /// last
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
			 0x000002d4,
			 0x000006c0,
			 0x000001ac,
			 0x0000041c,
			 0x00000670,
			 0x000003a4,
			 0x00000020,
			 0x00000444,

			 /// vpu register f2
			 0x41e00000,
			 0x42000000,
			 0x41b00000,
			 0x41e00000,
			 0x41800000,
			 0x40000000,
			 0x41e80000,
			 0x3f800000,

			 /// vpu register f3
			 0x41600000,
			 0x41880000,
			 0x41300000,
			 0x41880000,
			 0x41d80000,
			 0x40c00000,
			 0x41d00000,
			 0x40e00000,

			 /// vpu register f4
			 0x41200000,
			 0x41000000,
			 0x40400000,
			 0x41100000,
			 0x41d00000,
			 0x41300000,
			 0x41f80000,
			 0x41e80000,

			 /// vpu register f5
			 0x41200000,
			 0x41000000,
			 0x41b00000,
			 0x40800000,
			 0x41c80000,
			 0x41000000,
			 0x40400000,
			 0x41d80000,

			 /// vpu register f6
			 0x41d80000,
			 0x3f800000,
			 0x41b80000,
			 0x41b00000,
			 0x41a80000,
			 0x41500000,
			 0x41880000,
			 0x41e80000,

			 /// vpu register f7
			 0x40400000,
			 0x41800000,
			 0x41d00000,
			 0x41c80000,
			 0x41800000,
			 0x41400000,
			 0x41c00000,
			 0x41c80000,

			 /// vpu register f8
			 0x41b80000,
			 0x41d00000,
			 0x41300000,
			 0x41700000,
			 0x40800000,
			 0x41e00000,
			 0x40a00000,
			 0x41100000,

			 /// vpu register f9
			 0x41f80000,
			 0x40000000,
			 0x40400000,
			 0x40c00000,
			 0x41400000,
			 0x41100000,
			 0x41300000,
			 0x41300000,

			 /// vpu register f10
			 0x41900000,
			 0x41a00000,
			 0x41500000,
			 0x40400000,
			 0x41f80000,
			 0x41f00000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f11
			 0x40800000,
			 0x41200000,
			 0x41d00000,
			 0x41c00000,
			 0x41f00000,
			 0x42000000,
			 0x41d80000,
			 0x40c00000,

			 /// vpu register f12
			 0x41c80000,
			 0x41980000,
			 0x41500000,
			 0x41e80000,
			 0x41880000,
			 0x41e80000,
			 0x41c00000,
			 0x41600000,

			 /// vpu register f13
			 0x40800000,
			 0x41b80000,
			 0x41880000,
			 0x41e80000,
			 0x41a00000,
			 0x41100000,
			 0x41600000,
			 0x41900000,

			 /// vpu register f14
			 0x41b80000,
			 0x41f80000,
			 0x41d00000,
			 0x41980000,
			 0x41900000,
			 0x41400000,
			 0x41100000,
			 0x41c00000,

			 /// vpu register f15
			 0x41b00000,
			 0x41d80000,
			 0x40800000,
			 0x41000000,
			 0x41500000,
			 0x41800000,
			 0x40800000,
			 0x42000000,

			 /// vpu register f16
			 0x41c00000,
			 0x3f800000,
			 0x41e00000,
			 0x41500000,
			 0x41f80000,
			 0x41c80000,
			 0x41a80000,
			 0x40e00000,

			 /// vpu register f17
			 0x41500000,
			 0x41700000,
			 0x41b00000,
			 0x41f00000,
			 0x41f80000,
			 0x41e80000,
			 0x41880000,
			 0x41b80000,

			 /// vpu register f18
			 0x41000000,
			 0x41200000,
			 0x41b00000,
			 0x41b80000,
			 0x41b80000,
			 0x41e80000,
			 0x40000000,
			 0x41d80000,

			 /// vpu register f19
			 0x41d00000,
			 0x41b80000,
			 0x41e80000,
			 0x40a00000,
			 0x41c80000,
			 0x41f80000,
			 0x41980000,
			 0x41880000,

			 /// vpu register f20
			 0x41f80000,
			 0x41c80000,
			 0x41000000,
			 0x41800000,
			 0x40000000,
			 0x41300000,
			 0x3f800000,
			 0x41d00000,

			 /// vpu register f21
			 0x40c00000,
			 0x41800000,
			 0x41e80000,
			 0x41e00000,
			 0x41600000,
			 0x40c00000,
			 0x40800000,
			 0x41c80000,

			 /// vpu register f22
			 0x41500000,
			 0x41700000,
			 0x41100000,
			 0x41c80000,
			 0x41400000,
			 0x41e00000,
			 0x40400000,
			 0x41200000,

			 /// vpu register f23
			 0x41600000,
			 0x40000000,
			 0x40800000,
			 0x41d00000,
			 0x40400000,
			 0x41500000,
			 0x3f800000,
			 0x41200000,

			 /// vpu register f24
			 0x41e00000,
			 0x41b80000,
			 0x41200000,
			 0x40000000,
			 0x40e00000,
			 0x41600000,
			 0x41c00000,
			 0x3f800000,

			 /// vpu register f25
			 0x41c00000,
			 0x41e00000,
			 0x41900000,
			 0x41980000,
			 0x41e00000,
			 0x41b00000,
			 0x40a00000,
			 0x40a00000,

			 /// vpu register f26
			 0x41f00000,
			 0x40e00000,
			 0x41f80000,
			 0x42000000,
			 0x41700000,
			 0x41400000,
			 0x41900000,
			 0x41e00000,

			 /// vpu register f27
			 0x41e00000,
			 0x41b80000,
			 0x40c00000,
			 0x41c80000,
			 0x41c80000,
			 0x41000000,
			 0x41000000,
			 0x41980000,

			 /// vpu register f28
			 0x41100000,
			 0x41800000,
			 0x41a00000,
			 0x41600000,
			 0x40400000,
			 0x41700000,
			 0x41c00000,
			 0x41c80000,

			 /// vpu register f29
			 0x41f00000,
			 0x41f80000,
			 0x41d00000,
			 0x41400000,
			 0x40800000,
			 0x41500000,
			 0x41b80000,
			 0x41d00000,

			 /// vpu register f30
			 0x40c00000,
			 0x41b80000,
			 0x41880000,
			 0x41a00000,
			 0x41300000,
			 0x40400000,
			 0x41b00000,
			 0x41e80000,

			 /// vpu register f31
			 0x41f00000,
			 0x41100000,
			 0x41e00000,
			 0x41700000,
			 0x41b80000,
			 0x41100000,
			 0x41200000,
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
		"famomaxl.pi f11, f1 (x13)\n"                         ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f4, f1 (x14)\n"                          ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f16, f1 (x12)\n"                         ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f12, f1 (x13)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f18, f1 (x14)\n"                         ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f9, f1 (x11)\n"                          ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f2, f1 (x14)\n"                          ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f18, f1 (x15)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f16, f1 (x15)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f30, f1 (x14)\n"                         ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f2, f1 (x12)\n"                          ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f22, f1 (x11)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f20, f1 (x11)\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f29, f1 (x15)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f16, f1 (x14)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f21, f1 (x15)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f27, f1 (x11)\n"                         ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f23, f1 (x11)\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f8, f1 (x13)\n"                          ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f26, f1 (x15)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f30, f1 (x12)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f18, f1 (x13)\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f23, f1 (x13)\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f17, f1 (x15)\n"                         ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f28, f1 (x14)\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f20, f1 (x12)\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f24, f1 (x13)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f2, f1 (x15)\n"                          ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f4, f1 (x12)\n"                          ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f27, f1 (x11)\n"                         ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f6, f1 (x12)\n"                          ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f16, f1 (x13)\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f22, f1 (x12)\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f10, f1 (x15)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f11, f1 (x11)\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f9, f1 (x13)\n"                          ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f22, f1 (x13)\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f23, f1 (x12)\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f10, f1 (x13)\n"                         ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f26, f1 (x11)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f19, f1 (x14)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f16, f1 (x12)\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f6, f1 (x14)\n"                          ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f27, f1 (x13)\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f23, f1 (x12)\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f30, f1 (x13)\n"                         ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f30, f1 (x14)\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f10, f1 (x11)\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f8, f1 (x13)\n"                          ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f7, f1 (x15)\n"                          ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f29, f1 (x15)\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f26, f1 (x12)\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f14, f1 (x13)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f18, f1 (x14)\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f23, f1 (x11)\n"                         ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f12, f1 (x14)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f18, f1 (x11)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f12, f1 (x12)\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f9, f1 (x15)\n"                          ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f28, f1 (x14)\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f14, f1 (x11)\n"                         ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f25, f1 (x14)\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f9, f1 (x14)\n"                          ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f4, f1 (x14)\n"                          ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f19, f1 (x13)\n"                         ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f28, f1 (x12)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f2, f1 (x13)\n"                          ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f7, f1 (x12)\n"                          ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f5, f1 (x12)\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f16, f1 (x15)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f18, f1 (x13)\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f9, f1 (x12)\n"                          ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f10, f1 (x15)\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f15, f1 (x15)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f14, f1 (x12)\n"                         ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f16, f1 (x11)\n"                         ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f10, f1 (x15)\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f17, f1 (x15)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f11, f1 (x14)\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f14, f1 (x11)\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f11, f1 (x11)\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f6, f1 (x15)\n"                          ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f5, f1 (x12)\n"                          ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f17, f1 (x13)\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f6, f1 (x12)\n"                          ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f22, f1 (x14)\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f28, f1 (x13)\n"                         ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f12, f1 (x15)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f12, f1 (x13)\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f29, f1 (x14)\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f28, f1 (x14)\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f15, f1 (x13)\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f10, f1 (x13)\n"                         ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f17, f1 (x11)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f6, f1 (x14)\n"                          ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f8, f1 (x14)\n"                          ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f24, f1 (x15)\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f23, f1 (x13)\n"                         ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxl.pi f9, f1 (x12)\n"                          ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
