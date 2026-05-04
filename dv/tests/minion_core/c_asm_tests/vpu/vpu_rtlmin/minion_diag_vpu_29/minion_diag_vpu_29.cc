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
			 0x9fe7b5b5, /// 0x0
			 0xf053d210, /// 0x4
			 0xcf3e3231, /// 0x8
			 0xde3f09d7, /// 0xc
			 0xce638906, /// 0x10
			 0x01c51877, /// 0x14
			 0xf35e78d4, /// 0x18
			 0x52107d64, /// 0x1c
			 0x2070b51f, /// 0x20
			 0xf2676e8b, /// 0x24
			 0xc02f863b, /// 0x28
			 0xc430e967, /// 0x2c
			 0xf8571681, /// 0x30
			 0x98568407, /// 0x34
			 0x4d685580, /// 0x38
			 0x7f13a6f0, /// 0x3c
			 0x59142c0e, /// 0x40
			 0xaaa1e221, /// 0x44
			 0x0ae0e7be, /// 0x48
			 0x70fbf2c0, /// 0x4c
			 0x8f6fc97a, /// 0x50
			 0x1b8f3727, /// 0x54
			 0x25cfdef0, /// 0x58
			 0xc85b036f, /// 0x5c
			 0x64588440, /// 0x60
			 0xe7a420a1, /// 0x64
			 0xb2b77147, /// 0x68
			 0x6259756e, /// 0x6c
			 0x91cdab73, /// 0x70
			 0x9e75df59, /// 0x74
			 0x934a2b77, /// 0x78
			 0x1dbcbbac, /// 0x7c
			 0xe11c0fb6, /// 0x80
			 0xd21003e4, /// 0x84
			 0x1d48999f, /// 0x88
			 0x13a30548, /// 0x8c
			 0x3fd987b0, /// 0x90
			 0x6a35034f, /// 0x94
			 0x5ddc90d1, /// 0x98
			 0x8f007854, /// 0x9c
			 0x4d24ab25, /// 0xa0
			 0xc8adb1e2, /// 0xa4
			 0xdd37ba9a, /// 0xa8
			 0x712db12b, /// 0xac
			 0x2afe4d11, /// 0xb0
			 0xe40ab616, /// 0xb4
			 0xbf4ec871, /// 0xb8
			 0x86e0757c, /// 0xbc
			 0xc8d3c754, /// 0xc0
			 0xfb83b124, /// 0xc4
			 0xc59db7f7, /// 0xc8
			 0x7bf90455, /// 0xcc
			 0x470f7013, /// 0xd0
			 0x2349e116, /// 0xd4
			 0x2a06bda6, /// 0xd8
			 0x01fb00ef, /// 0xdc
			 0xcf89cbe4, /// 0xe0
			 0x64515a6c, /// 0xe4
			 0xc4f1806d, /// 0xe8
			 0x47914059, /// 0xec
			 0x43cdb154, /// 0xf0
			 0xab835a8f, /// 0xf4
			 0xf14b45e4, /// 0xf8
			 0x2e8eec0a, /// 0xfc
			 0x718282bc, /// 0x100
			 0x030fd706, /// 0x104
			 0xf1d12266, /// 0x108
			 0x42445a85, /// 0x10c
			 0x53e23eb1, /// 0x110
			 0x1fb787ee, /// 0x114
			 0x3693a002, /// 0x118
			 0xef25ddd5, /// 0x11c
			 0xa3a30ab9, /// 0x120
			 0x8059b799, /// 0x124
			 0x12f2c792, /// 0x128
			 0xdc7af2f2, /// 0x12c
			 0xa2b708cf, /// 0x130
			 0xf8f810a7, /// 0x134
			 0xaf784e06, /// 0x138
			 0x0c37b0a9, /// 0x13c
			 0xb6c49653, /// 0x140
			 0x66806c6e, /// 0x144
			 0x54d5f3bf, /// 0x148
			 0x7f82a130, /// 0x14c
			 0x09e6f3a8, /// 0x150
			 0xcdc5731e, /// 0x154
			 0xb3476701, /// 0x158
			 0x0316718d, /// 0x15c
			 0x70cd3ec8, /// 0x160
			 0x54a5f6cd, /// 0x164
			 0x5172f9ee, /// 0x168
			 0x03627293, /// 0x16c
			 0xabf0112b, /// 0x170
			 0x3a8764ce, /// 0x174
			 0x18607255, /// 0x178
			 0x7ddd59fe, /// 0x17c
			 0x22d790f1, /// 0x180
			 0x943306a6, /// 0x184
			 0x24c9e26e, /// 0x188
			 0x831b1805, /// 0x18c
			 0xdf913d71, /// 0x190
			 0x09997003, /// 0x194
			 0x7bec715e, /// 0x198
			 0xecc459d0, /// 0x19c
			 0x49371275, /// 0x1a0
			 0xf640f41b, /// 0x1a4
			 0xe7972124, /// 0x1a8
			 0xbc620cc4, /// 0x1ac
			 0x82b8458c, /// 0x1b0
			 0x22896f4a, /// 0x1b4
			 0x593b41e7, /// 0x1b8
			 0x99374545, /// 0x1bc
			 0x68862570, /// 0x1c0
			 0x73a9b4ac, /// 0x1c4
			 0x1409b553, /// 0x1c8
			 0xc8492f5a, /// 0x1cc
			 0xfe871dd4, /// 0x1d0
			 0x8d8fb409, /// 0x1d4
			 0x36b56266, /// 0x1d8
			 0x4330dba2, /// 0x1dc
			 0x0a4e66f8, /// 0x1e0
			 0x66a19601, /// 0x1e4
			 0xcdd90628, /// 0x1e8
			 0x83c48e0d, /// 0x1ec
			 0xc576abf5, /// 0x1f0
			 0x99c6da9d, /// 0x1f4
			 0xd20da0e8, /// 0x1f8
			 0x6a1b1a83, /// 0x1fc
			 0x89d76430, /// 0x200
			 0xdf779ea5, /// 0x204
			 0xea4a7fb7, /// 0x208
			 0x0a8d428d, /// 0x20c
			 0x2f798047, /// 0x210
			 0x5d135399, /// 0x214
			 0x431c4f72, /// 0x218
			 0x989a53b7, /// 0x21c
			 0xc23af650, /// 0x220
			 0xa58b6eca, /// 0x224
			 0x10178e81, /// 0x228
			 0xfd5387c7, /// 0x22c
			 0x267022b2, /// 0x230
			 0x37487383, /// 0x234
			 0x22a3ebb1, /// 0x238
			 0x0df097cc, /// 0x23c
			 0x114e97c9, /// 0x240
			 0x46c8c311, /// 0x244
			 0x86657155, /// 0x248
			 0x2bcc06a4, /// 0x24c
			 0x5ab1f6d4, /// 0x250
			 0x16382fb0, /// 0x254
			 0xb36c8db5, /// 0x258
			 0x5d6645a4, /// 0x25c
			 0xd0d51c80, /// 0x260
			 0x9ede63e5, /// 0x264
			 0x43619471, /// 0x268
			 0x0aa98466, /// 0x26c
			 0x1599d2fe, /// 0x270
			 0xeb2f7c50, /// 0x274
			 0x2e933fc9, /// 0x278
			 0x125edc1f, /// 0x27c
			 0xf96e74c3, /// 0x280
			 0x2aeb88d9, /// 0x284
			 0x1f416f40, /// 0x288
			 0x34232d88, /// 0x28c
			 0xbf835cc7, /// 0x290
			 0x139e49b1, /// 0x294
			 0x96bb95d9, /// 0x298
			 0xf09a375d, /// 0x29c
			 0xcfde0625, /// 0x2a0
			 0xd28829d1, /// 0x2a4
			 0x300ffe73, /// 0x2a8
			 0x4b221a19, /// 0x2ac
			 0x584ba55d, /// 0x2b0
			 0x057f323d, /// 0x2b4
			 0xb0f90246, /// 0x2b8
			 0x1a58dd81, /// 0x2bc
			 0xbfe77615, /// 0x2c0
			 0x5e071472, /// 0x2c4
			 0xc89f3291, /// 0x2c8
			 0x778ad2d8, /// 0x2cc
			 0xc3ffd9e2, /// 0x2d0
			 0x4e7dc460, /// 0x2d4
			 0x1902580f, /// 0x2d8
			 0x0968fefc, /// 0x2dc
			 0xa98c625d, /// 0x2e0
			 0x420e579a, /// 0x2e4
			 0xada4020e, /// 0x2e8
			 0x1c50ff72, /// 0x2ec
			 0x2c064d4e, /// 0x2f0
			 0x70763269, /// 0x2f4
			 0xd78516d3, /// 0x2f8
			 0xc3c105e6, /// 0x2fc
			 0x99d58933, /// 0x300
			 0xa62bebe7, /// 0x304
			 0x7c981252, /// 0x308
			 0x825a2dcb, /// 0x30c
			 0x2199a164, /// 0x310
			 0x8a6e945e, /// 0x314
			 0x0ba869bd, /// 0x318
			 0xfc183367, /// 0x31c
			 0xc334630e, /// 0x320
			 0xdc333091, /// 0x324
			 0x0a8dfee0, /// 0x328
			 0x4a862b54, /// 0x32c
			 0x93795a9d, /// 0x330
			 0x2938df63, /// 0x334
			 0x779514c2, /// 0x338
			 0x66c29a1b, /// 0x33c
			 0xdba011f8, /// 0x340
			 0x06701c66, /// 0x344
			 0x610c6b8f, /// 0x348
			 0x14b61cc6, /// 0x34c
			 0x49376a88, /// 0x350
			 0x8059217f, /// 0x354
			 0xb42660f2, /// 0x358
			 0xa0c55ff0, /// 0x35c
			 0x49c3f22f, /// 0x360
			 0x95a99003, /// 0x364
			 0xeefea8f6, /// 0x368
			 0x733f9c55, /// 0x36c
			 0xd6775253, /// 0x370
			 0x8ffdd7d8, /// 0x374
			 0x3aefdea8, /// 0x378
			 0x86c59a29, /// 0x37c
			 0x10b3f259, /// 0x380
			 0x4b46f0bf, /// 0x384
			 0xeddde02f, /// 0x388
			 0xb0281499, /// 0x38c
			 0x3de34339, /// 0x390
			 0x65070ba9, /// 0x394
			 0xc16d005b, /// 0x398
			 0x01414fb2, /// 0x39c
			 0xadd49f11, /// 0x3a0
			 0x3c709a27, /// 0x3a4
			 0xe464bce7, /// 0x3a8
			 0x2c09f2d6, /// 0x3ac
			 0x17c30fff, /// 0x3b0
			 0x509b6d8e, /// 0x3b4
			 0x88a3f1e2, /// 0x3b8
			 0x937ae21c, /// 0x3bc
			 0xe6b4aea0, /// 0x3c0
			 0xf4c1e671, /// 0x3c4
			 0x8ecb34ab, /// 0x3c8
			 0xb3aaafe2, /// 0x3cc
			 0xa62a3bc0, /// 0x3d0
			 0xefc5e4ab, /// 0x3d4
			 0x2d9b697f, /// 0x3d8
			 0x51665880, /// 0x3dc
			 0x7047c2c1, /// 0x3e0
			 0xb6beb340, /// 0x3e4
			 0x173d90e5, /// 0x3e8
			 0xa1baf4f9, /// 0x3ec
			 0x6ab54597, /// 0x3f0
			 0x10c03d52, /// 0x3f4
			 0x6256832e, /// 0x3f8
			 0x4f6f619e, /// 0x3fc
			 0xbec35684, /// 0x400
			 0xefbda2cd, /// 0x404
			 0xb0804267, /// 0x408
			 0x9827e9c7, /// 0x40c
			 0xdc26f8dd, /// 0x410
			 0x6cd5a736, /// 0x414
			 0xeb653c37, /// 0x418
			 0x5b6263b3, /// 0x41c
			 0x1dd09227, /// 0x420
			 0x3a768ab5, /// 0x424
			 0xca9199f9, /// 0x428
			 0x5069cc74, /// 0x42c
			 0x25ea5788, /// 0x430
			 0x96636502, /// 0x434
			 0x368de7f4, /// 0x438
			 0x52e5ea97, /// 0x43c
			 0xc3cc7973, /// 0x440
			 0xbc809f66, /// 0x444
			 0xf834c357, /// 0x448
			 0x41ee3d53, /// 0x44c
			 0x74bc6913, /// 0x450
			 0x05441e67, /// 0x454
			 0x454eaa39, /// 0x458
			 0x28244f84, /// 0x45c
			 0xdd33c574, /// 0x460
			 0x09b1afc0, /// 0x464
			 0xe42035cb, /// 0x468
			 0xa1311f5d, /// 0x46c
			 0xc5300d94, /// 0x470
			 0xd815ab82, /// 0x474
			 0x45bd7d76, /// 0x478
			 0x93e41422, /// 0x47c
			 0x0707a455, /// 0x480
			 0xe3724165, /// 0x484
			 0x7b7f6fff, /// 0x488
			 0xce45b0d0, /// 0x48c
			 0xc66482ec, /// 0x490
			 0xf2b832a4, /// 0x494
			 0x36bb3646, /// 0x498
			 0x33ec04a2, /// 0x49c
			 0x0911c589, /// 0x4a0
			 0xb9199d36, /// 0x4a4
			 0x9df6e74d, /// 0x4a8
			 0xe2f32a6b, /// 0x4ac
			 0x1134e3fd, /// 0x4b0
			 0xb1d27bd0, /// 0x4b4
			 0x3587a115, /// 0x4b8
			 0x8dee359d, /// 0x4bc
			 0x49877b02, /// 0x4c0
			 0xb7c4c5b2, /// 0x4c4
			 0x7c7c4e1a, /// 0x4c8
			 0xe8fa6583, /// 0x4cc
			 0xb024cdec, /// 0x4d0
			 0xdb5159c7, /// 0x4d4
			 0xff1d174a, /// 0x4d8
			 0x2b0cf9a6, /// 0x4dc
			 0xed3357d4, /// 0x4e0
			 0x38c2edb5, /// 0x4e4
			 0x12a98c08, /// 0x4e8
			 0x26fd8a36, /// 0x4ec
			 0xc1b5f0f3, /// 0x4f0
			 0x9bed7b2b, /// 0x4f4
			 0xd26ed569, /// 0x4f8
			 0x4532227c, /// 0x4fc
			 0x380baa16, /// 0x500
			 0x13202a3b, /// 0x504
			 0xd32a36ef, /// 0x508
			 0xeb1afd61, /// 0x50c
			 0x9e976ed0, /// 0x510
			 0x98801f38, /// 0x514
			 0x6415a7b6, /// 0x518
			 0xced20391, /// 0x51c
			 0x6ffa8059, /// 0x520
			 0x43bb11d9, /// 0x524
			 0x7ed91e3f, /// 0x528
			 0x9c925f04, /// 0x52c
			 0xee97f169, /// 0x530
			 0xecfed534, /// 0x534
			 0xaa81672b, /// 0x538
			 0xa98ed1e7, /// 0x53c
			 0x41958638, /// 0x540
			 0x36c8dc15, /// 0x544
			 0x8d51406f, /// 0x548
			 0xa99ce7c1, /// 0x54c
			 0xb3c16102, /// 0x550
			 0x5aa8ad0a, /// 0x554
			 0x44e1adfe, /// 0x558
			 0x3ea74d7f, /// 0x55c
			 0xd0b7d59e, /// 0x560
			 0x2c041aee, /// 0x564
			 0x5b0f5627, /// 0x568
			 0xd0e086ae, /// 0x56c
			 0x578a0915, /// 0x570
			 0xffc0d067, /// 0x574
			 0x0ce440f2, /// 0x578
			 0x0f7a074c, /// 0x57c
			 0x5008998f, /// 0x580
			 0x92c4de23, /// 0x584
			 0x76b541a4, /// 0x588
			 0xba662541, /// 0x58c
			 0xe49f6beb, /// 0x590
			 0x8a4267ac, /// 0x594
			 0x0abfa13f, /// 0x598
			 0x5a50c9bd, /// 0x59c
			 0x8b8d5548, /// 0x5a0
			 0x545bd231, /// 0x5a4
			 0xcaadb707, /// 0x5a8
			 0x1dac9d3e, /// 0x5ac
			 0xcedc866e, /// 0x5b0
			 0x654f8dd1, /// 0x5b4
			 0x263e8aca, /// 0x5b8
			 0x94a318da, /// 0x5bc
			 0x169eaffc, /// 0x5c0
			 0xb8f58f2c, /// 0x5c4
			 0xddfe038e, /// 0x5c8
			 0x93f41c21, /// 0x5cc
			 0x67751e06, /// 0x5d0
			 0xf4a71636, /// 0x5d4
			 0x1c8c37db, /// 0x5d8
			 0xe24a51d3, /// 0x5dc
			 0x0fe6b762, /// 0x5e0
			 0xacc44b9c, /// 0x5e4
			 0x9c1e8f9c, /// 0x5e8
			 0x615bb815, /// 0x5ec
			 0x8d5f80d6, /// 0x5f0
			 0x8fb7e2b2, /// 0x5f4
			 0x11a319b1, /// 0x5f8
			 0x78c16ea9, /// 0x5fc
			 0x660bdb33, /// 0x600
			 0xc6a4e13f, /// 0x604
			 0x96607652, /// 0x608
			 0x1702a908, /// 0x60c
			 0xd1121997, /// 0x610
			 0xb8eaf646, /// 0x614
			 0xc5c9730f, /// 0x618
			 0xac69a600, /// 0x61c
			 0x367f5db3, /// 0x620
			 0x8661af20, /// 0x624
			 0x0259fd22, /// 0x628
			 0xef65b1a4, /// 0x62c
			 0xf7fd0f34, /// 0x630
			 0xa6e17278, /// 0x634
			 0x7821c3ac, /// 0x638
			 0xe256d6d6, /// 0x63c
			 0x06b54e09, /// 0x640
			 0x00c2e31d, /// 0x644
			 0xd5a67ba5, /// 0x648
			 0x810c643f, /// 0x64c
			 0x0343aace, /// 0x650
			 0xff326ca9, /// 0x654
			 0xd9f75340, /// 0x658
			 0x03c1b977, /// 0x65c
			 0xaaabad74, /// 0x660
			 0x41ed92cd, /// 0x664
			 0xb6db0b5b, /// 0x668
			 0x311f73b4, /// 0x66c
			 0x5b2bd584, /// 0x670
			 0xaf0bb0cc, /// 0x674
			 0x6622efaf, /// 0x678
			 0xd4c59f1c, /// 0x67c
			 0xfadb64fd, /// 0x680
			 0x7c91d65c, /// 0x684
			 0x0cee2a64, /// 0x688
			 0xc00b9ece, /// 0x68c
			 0x7aeb3c68, /// 0x690
			 0x6fae4abe, /// 0x694
			 0x4d657be4, /// 0x698
			 0x29da29ee, /// 0x69c
			 0x03a97598, /// 0x6a0
			 0x34ea78d9, /// 0x6a4
			 0xd1461353, /// 0x6a8
			 0xe5d0f7a0, /// 0x6ac
			 0xd25067ae, /// 0x6b0
			 0x8b4455c7, /// 0x6b4
			 0x476982f3, /// 0x6b8
			 0xa13a6d56, /// 0x6bc
			 0xad3798ba, /// 0x6c0
			 0x09b69bec, /// 0x6c4
			 0x4b76ecce, /// 0x6c8
			 0x141f6c00, /// 0x6cc
			 0xefeaf8db, /// 0x6d0
			 0x7013bb44, /// 0x6d4
			 0x1ee34e0c, /// 0x6d8
			 0x00bfb58e, /// 0x6dc
			 0xefd7b326, /// 0x6e0
			 0xbad536d8, /// 0x6e4
			 0x1f985f9a, /// 0x6e8
			 0xe2d555ce, /// 0x6ec
			 0x8b5501e3, /// 0x6f0
			 0x22ffa1aa, /// 0x6f4
			 0x519593b6, /// 0x6f8
			 0x5128abc6, /// 0x6fc
			 0xd1912ffc, /// 0x700
			 0x0302ba84, /// 0x704
			 0x76c1e914, /// 0x708
			 0xd0bf7f00, /// 0x70c
			 0xc4d97c56, /// 0x710
			 0xb2625de2, /// 0x714
			 0xbce7064b, /// 0x718
			 0xc5620153, /// 0x71c
			 0x6754108c, /// 0x720
			 0x7caa791d, /// 0x724
			 0x63e443e2, /// 0x728
			 0x2f287660, /// 0x72c
			 0x41a1b590, /// 0x730
			 0xfbbadf72, /// 0x734
			 0x35e0a7bc, /// 0x738
			 0xa04a9c5d, /// 0x73c
			 0x742bcf47, /// 0x740
			 0xdbea3426, /// 0x744
			 0xc3d95271, /// 0x748
			 0xfc04ee09, /// 0x74c
			 0xe151de74, /// 0x750
			 0x43caa21d, /// 0x754
			 0xa9de4408, /// 0x758
			 0xc9861ab6, /// 0x75c
			 0xd95e4407, /// 0x760
			 0x1366e63f, /// 0x764
			 0x09a2cb8c, /// 0x768
			 0xbc6c527b, /// 0x76c
			 0xab1e70d0, /// 0x770
			 0x34177df3, /// 0x774
			 0x9e11252e, /// 0x778
			 0x498da66d, /// 0x77c
			 0xbe4f6595, /// 0x780
			 0x0ac92e91, /// 0x784
			 0xe8cf1acf, /// 0x788
			 0x8ec18522, /// 0x78c
			 0xda8c5749, /// 0x790
			 0x3226e8d5, /// 0x794
			 0xee92255b, /// 0x798
			 0xb43dfdeb, /// 0x79c
			 0x4f5e7f0c, /// 0x7a0
			 0x67935165, /// 0x7a4
			 0xb1dcbb64, /// 0x7a8
			 0x6f4f78a5, /// 0x7ac
			 0x9c5a2ebc, /// 0x7b0
			 0x1c7294a9, /// 0x7b4
			 0xb7984a3c, /// 0x7b8
			 0xf783dc99, /// 0x7bc
			 0x60543596, /// 0x7c0
			 0x121cef19, /// 0x7c4
			 0xa72557d6, /// 0x7c8
			 0x85870ac2, /// 0x7cc
			 0x9b4cc5ea, /// 0x7d0
			 0x02c401c9, /// 0x7d4
			 0x0c2ae414, /// 0x7d8
			 0xfc161c29, /// 0x7dc
			 0x0af5af8f, /// 0x7e0
			 0x21e53bcb, /// 0x7e4
			 0x440abaac, /// 0x7e8
			 0x142ba966, /// 0x7ec
			 0x99c4c5b2, /// 0x7f0
			 0x62470044, /// 0x7f4
			 0x7a324739, /// 0x7f8
			 0xea550f93, /// 0x7fc
			 0x909afe63, /// 0x800
			 0x50213c06, /// 0x804
			 0x855ba266, /// 0x808
			 0x1c0ee47f, /// 0x80c
			 0x2f438459, /// 0x810
			 0xf7d84c6c, /// 0x814
			 0x9230bdc0, /// 0x818
			 0xd54c78a0, /// 0x81c
			 0x1b4f4df7, /// 0x820
			 0x157094db, /// 0x824
			 0x16bb1dfe, /// 0x828
			 0x27fe928d, /// 0x82c
			 0xdf241ace, /// 0x830
			 0x4d51a8b1, /// 0x834
			 0xa35fbc92, /// 0x838
			 0xe188a7aa, /// 0x83c
			 0x17369e2d, /// 0x840
			 0xc8f43873, /// 0x844
			 0x3ed2c188, /// 0x848
			 0x1c7a86f7, /// 0x84c
			 0x239f1291, /// 0x850
			 0x3bbca2e0, /// 0x854
			 0x7accf595, /// 0x858
			 0x329eb086, /// 0x85c
			 0x264480f5, /// 0x860
			 0x796df7b9, /// 0x864
			 0xd7e8eb71, /// 0x868
			 0xc4cdc9ec, /// 0x86c
			 0x8bcb54fe, /// 0x870
			 0xb9816ba9, /// 0x874
			 0x32b294ab, /// 0x878
			 0xa8c13ae1, /// 0x87c
			 0x1d29ba88, /// 0x880
			 0x810b48b4, /// 0x884
			 0x012775f3, /// 0x888
			 0x5eba81c4, /// 0x88c
			 0x53db0402, /// 0x890
			 0x47b28d76, /// 0x894
			 0xe25b214a, /// 0x898
			 0x58e67ca7, /// 0x89c
			 0x7df641f3, /// 0x8a0
			 0xb4dabc64, /// 0x8a4
			 0x7291769d, /// 0x8a8
			 0x4a5fd8b4, /// 0x8ac
			 0x8b4192e4, /// 0x8b0
			 0xf102648d, /// 0x8b4
			 0x76d93562, /// 0x8b8
			 0xe0fe4f48, /// 0x8bc
			 0x83c94057, /// 0x8c0
			 0xff64b696, /// 0x8c4
			 0xf79fb7ac, /// 0x8c8
			 0x056fc59d, /// 0x8cc
			 0xaf69b89c, /// 0x8d0
			 0xee3378ba, /// 0x8d4
			 0x47d89e1f, /// 0x8d8
			 0xc500390b, /// 0x8dc
			 0x4f6efe52, /// 0x8e0
			 0x03118f5a, /// 0x8e4
			 0x44c7153f, /// 0x8e8
			 0xb2a522a1, /// 0x8ec
			 0xc51025da, /// 0x8f0
			 0x4c719f84, /// 0x8f4
			 0x1f7b9a11, /// 0x8f8
			 0x870e867d, /// 0x8fc
			 0x8d53ca9f, /// 0x900
			 0x0bf5d628, /// 0x904
			 0xb45241c4, /// 0x908
			 0xa9f2047a, /// 0x90c
			 0xf4228ba1, /// 0x910
			 0x77ed3823, /// 0x914
			 0xfec70c22, /// 0x918
			 0x004d2481, /// 0x91c
			 0xd4ce33d8, /// 0x920
			 0x6d448e85, /// 0x924
			 0xdcc82e5f, /// 0x928
			 0xa6211a55, /// 0x92c
			 0xc3d55deb, /// 0x930
			 0xb1fe6989, /// 0x934
			 0x092499ed, /// 0x938
			 0x10e2f733, /// 0x93c
			 0xf9a98084, /// 0x940
			 0x66f8a820, /// 0x944
			 0xc7351c08, /// 0x948
			 0x96d179f2, /// 0x94c
			 0xedcdfb7b, /// 0x950
			 0xac1ef35e, /// 0x954
			 0xfa1a992b, /// 0x958
			 0x1c042601, /// 0x95c
			 0x8bea6904, /// 0x960
			 0x956924cd, /// 0x964
			 0x969dcc19, /// 0x968
			 0x48e109bc, /// 0x96c
			 0xeea2cdbb, /// 0x970
			 0x2a231512, /// 0x974
			 0xf17e01dd, /// 0x978
			 0x303fb06c, /// 0x97c
			 0x31b113e2, /// 0x980
			 0x336e2b7f, /// 0x984
			 0xc4f2fa6a, /// 0x988
			 0xb842bc7f, /// 0x98c
			 0xd75de5d8, /// 0x990
			 0x77072af8, /// 0x994
			 0xe0552958, /// 0x998
			 0x64ce394c, /// 0x99c
			 0x6979a428, /// 0x9a0
			 0x31aa3186, /// 0x9a4
			 0x394be68a, /// 0x9a8
			 0xcdd71048, /// 0x9ac
			 0x14dc84f3, /// 0x9b0
			 0x98f65ee4, /// 0x9b4
			 0x60cd22b5, /// 0x9b8
			 0x48a2165a, /// 0x9bc
			 0x44b14204, /// 0x9c0
			 0x204af130, /// 0x9c4
			 0xc08b63c4, /// 0x9c8
			 0x19518466, /// 0x9cc
			 0xc7368dff, /// 0x9d0
			 0x1400729a, /// 0x9d4
			 0x77e4c1fc, /// 0x9d8
			 0x1db09cc5, /// 0x9dc
			 0x72f09612, /// 0x9e0
			 0xdef8e460, /// 0x9e4
			 0x0f4010cd, /// 0x9e8
			 0x9c6002c8, /// 0x9ec
			 0xeec4e9cb, /// 0x9f0
			 0x8a951dab, /// 0x9f4
			 0xda460d35, /// 0x9f8
			 0x36244b53, /// 0x9fc
			 0xda24a138, /// 0xa00
			 0xf50308f3, /// 0xa04
			 0x45c593d7, /// 0xa08
			 0xeaa70102, /// 0xa0c
			 0x487f0191, /// 0xa10
			 0xe6e3b5dc, /// 0xa14
			 0x8abddfeb, /// 0xa18
			 0x1a552108, /// 0xa1c
			 0x9433a29a, /// 0xa20
			 0xc0dfaeef, /// 0xa24
			 0xc8fe0d67, /// 0xa28
			 0xb7e89990, /// 0xa2c
			 0x3f397045, /// 0xa30
			 0x4327bd14, /// 0xa34
			 0x6b88f02e, /// 0xa38
			 0xf1bfdced, /// 0xa3c
			 0x65ee0d59, /// 0xa40
			 0x7b1b863f, /// 0xa44
			 0x58d585eb, /// 0xa48
			 0x1e7563ad, /// 0xa4c
			 0x0187d8eb, /// 0xa50
			 0xb5da36d4, /// 0xa54
			 0x6da5948d, /// 0xa58
			 0xc6d6e348, /// 0xa5c
			 0xabb29a24, /// 0xa60
			 0x6f7df2c7, /// 0xa64
			 0xc5208c7b, /// 0xa68
			 0xd06648e8, /// 0xa6c
			 0x04a5e077, /// 0xa70
			 0xde58ea10, /// 0xa74
			 0x88249b26, /// 0xa78
			 0x9f978e8a, /// 0xa7c
			 0x9780857f, /// 0xa80
			 0x62f14a90, /// 0xa84
			 0xab6a6860, /// 0xa88
			 0xe704173b, /// 0xa8c
			 0xea22ee43, /// 0xa90
			 0x4f14162d, /// 0xa94
			 0x3c8af000, /// 0xa98
			 0x67704d7c, /// 0xa9c
			 0xad608938, /// 0xaa0
			 0xdab9af84, /// 0xaa4
			 0x25a1e1df, /// 0xaa8
			 0x73b27d1b, /// 0xaac
			 0x5fe20978, /// 0xab0
			 0x08946e24, /// 0xab4
			 0xcfb5172d, /// 0xab8
			 0xcad1e636, /// 0xabc
			 0x3754d200, /// 0xac0
			 0x4b55c146, /// 0xac4
			 0x11336c97, /// 0xac8
			 0x410ff5d6, /// 0xacc
			 0xbd7705a5, /// 0xad0
			 0xbcf6c971, /// 0xad4
			 0xe887afff, /// 0xad8
			 0x472fffdc, /// 0xadc
			 0x5a2fbacf, /// 0xae0
			 0x59d64c8b, /// 0xae4
			 0x7bd8566c, /// 0xae8
			 0xb956e37e, /// 0xaec
			 0x0113b537, /// 0xaf0
			 0x0092736e, /// 0xaf4
			 0x65a69765, /// 0xaf8
			 0x4451deb5, /// 0xafc
			 0x614785ae, /// 0xb00
			 0x8448d786, /// 0xb04
			 0x52fd92ba, /// 0xb08
			 0x1290afba, /// 0xb0c
			 0xaa5e8b28, /// 0xb10
			 0x6e24b8aa, /// 0xb14
			 0x4004c582, /// 0xb18
			 0xc533eaea, /// 0xb1c
			 0x5ff7c92a, /// 0xb20
			 0xcc476ea8, /// 0xb24
			 0xb01ee5b3, /// 0xb28
			 0x7b80b2e6, /// 0xb2c
			 0x3e23a61a, /// 0xb30
			 0xbbe1d24b, /// 0xb34
			 0x6b4edf04, /// 0xb38
			 0x28b11477, /// 0xb3c
			 0xe0bfc0aa, /// 0xb40
			 0xb5b2c757, /// 0xb44
			 0x3c3d292a, /// 0xb48
			 0x8f7cba23, /// 0xb4c
			 0xf83a87ea, /// 0xb50
			 0x1e3d617f, /// 0xb54
			 0x71864180, /// 0xb58
			 0xca99f902, /// 0xb5c
			 0xc90e4724, /// 0xb60
			 0x1ef05e7c, /// 0xb64
			 0xe1321d78, /// 0xb68
			 0x3455cd7b, /// 0xb6c
			 0xb905f35a, /// 0xb70
			 0xf5603e72, /// 0xb74
			 0xbf089383, /// 0xb78
			 0x9f5c7390, /// 0xb7c
			 0x8535f90f, /// 0xb80
			 0x7cb6f495, /// 0xb84
			 0x1b60c153, /// 0xb88
			 0x5a92ce82, /// 0xb8c
			 0x1c881215, /// 0xb90
			 0xb73f0839, /// 0xb94
			 0x971ba6ea, /// 0xb98
			 0x096702ab, /// 0xb9c
			 0xfd2acd38, /// 0xba0
			 0x8c00f51f, /// 0xba4
			 0x796c1d6f, /// 0xba8
			 0xa0702738, /// 0xbac
			 0x1416fa6f, /// 0xbb0
			 0x58aa9122, /// 0xbb4
			 0x9c7bc37a, /// 0xbb8
			 0x8392e1c2, /// 0xbbc
			 0x2bdc58fa, /// 0xbc0
			 0x707a2c66, /// 0xbc4
			 0x02c70415, /// 0xbc8
			 0xc0020997, /// 0xbcc
			 0x2ab0b3bb, /// 0xbd0
			 0x4e8b910c, /// 0xbd4
			 0xa1cc8c71, /// 0xbd8
			 0xbaad98f2, /// 0xbdc
			 0xc7c14755, /// 0xbe0
			 0x8ee39c5c, /// 0xbe4
			 0xf03d5497, /// 0xbe8
			 0x03c10024, /// 0xbec
			 0x53b978bc, /// 0xbf0
			 0x4bca0c8a, /// 0xbf4
			 0xe0afb83d, /// 0xbf8
			 0x8e3a7733, /// 0xbfc
			 0x1d1829ad, /// 0xc00
			 0x95a50f0f, /// 0xc04
			 0x1207eb46, /// 0xc08
			 0x58c71f0a, /// 0xc0c
			 0xf0342e47, /// 0xc10
			 0xe6e3d362, /// 0xc14
			 0xbb7ce1ce, /// 0xc18
			 0x3048d61e, /// 0xc1c
			 0x4725ce26, /// 0xc20
			 0xdbf7ea0b, /// 0xc24
			 0xf82b0656, /// 0xc28
			 0xf94833ca, /// 0xc2c
			 0x13ee2f44, /// 0xc30
			 0x9be8864a, /// 0xc34
			 0xf68d000e, /// 0xc38
			 0x4c11a4e4, /// 0xc3c
			 0x1dcb851c, /// 0xc40
			 0x6a76bc91, /// 0xc44
			 0x323fcea1, /// 0xc48
			 0xf4fb8150, /// 0xc4c
			 0xe6cd2332, /// 0xc50
			 0xcc5ebcb5, /// 0xc54
			 0x49928ed0, /// 0xc58
			 0x56af4de4, /// 0xc5c
			 0xb953b3ce, /// 0xc60
			 0x5da01ae5, /// 0xc64
			 0x35eece86, /// 0xc68
			 0x1eb4755f, /// 0xc6c
			 0xbad81240, /// 0xc70
			 0x825ce188, /// 0xc74
			 0x7bda25b5, /// 0xc78
			 0x0ce21580, /// 0xc7c
			 0x8742ac46, /// 0xc80
			 0x1e6ef1c8, /// 0xc84
			 0x818a625e, /// 0xc88
			 0x3f348668, /// 0xc8c
			 0xb3a9acf3, /// 0xc90
			 0x0d52ded7, /// 0xc94
			 0xf99128c4, /// 0xc98
			 0x845d5dae, /// 0xc9c
			 0x646381ee, /// 0xca0
			 0x85f8713e, /// 0xca4
			 0xa7af53ca, /// 0xca8
			 0x355fbe85, /// 0xcac
			 0x7600479d, /// 0xcb0
			 0xc77769e5, /// 0xcb4
			 0xd31f101e, /// 0xcb8
			 0x626158c5, /// 0xcbc
			 0x7453b08c, /// 0xcc0
			 0xad1370b6, /// 0xcc4
			 0x1fd93877, /// 0xcc8
			 0xa9d57200, /// 0xccc
			 0x3e65bca0, /// 0xcd0
			 0x91cf6a8d, /// 0xcd4
			 0xc02990f2, /// 0xcd8
			 0x7836a632, /// 0xcdc
			 0x84c3fc7f, /// 0xce0
			 0x68d3359a, /// 0xce4
			 0x7aed665e, /// 0xce8
			 0xd1fad14a, /// 0xcec
			 0x58944db4, /// 0xcf0
			 0x3c3879ea, /// 0xcf4
			 0x5949a209, /// 0xcf8
			 0xc16247e3, /// 0xcfc
			 0xf67ed84d, /// 0xd00
			 0xff54552c, /// 0xd04
			 0xee4eaaa3, /// 0xd08
			 0x6a9842bb, /// 0xd0c
			 0x9ac7549f, /// 0xd10
			 0x93964ce0, /// 0xd14
			 0x8b1fd61a, /// 0xd18
			 0xd95fc242, /// 0xd1c
			 0xaff559d0, /// 0xd20
			 0x7d68cdf9, /// 0xd24
			 0x7d102a8d, /// 0xd28
			 0x777f1be8, /// 0xd2c
			 0x28a2ddce, /// 0xd30
			 0x1f1fcb5b, /// 0xd34
			 0xdf3f9d37, /// 0xd38
			 0x933406b0, /// 0xd3c
			 0x7e04522e, /// 0xd40
			 0xc22f86dc, /// 0xd44
			 0xd04093d7, /// 0xd48
			 0x7bfd43c1, /// 0xd4c
			 0xf00df2c2, /// 0xd50
			 0x6818ae6c, /// 0xd54
			 0x24838de3, /// 0xd58
			 0xff39a34c, /// 0xd5c
			 0xd8afd906, /// 0xd60
			 0x3d5b8053, /// 0xd64
			 0xb1f51bc8, /// 0xd68
			 0x9a89b356, /// 0xd6c
			 0xe58f17f0, /// 0xd70
			 0xb6c653cd, /// 0xd74
			 0x90f24ddc, /// 0xd78
			 0x299d2c0b, /// 0xd7c
			 0x2fb1ab76, /// 0xd80
			 0x16fd761f, /// 0xd84
			 0xb8be7848, /// 0xd88
			 0xf8b46b6b, /// 0xd8c
			 0x97c2981b, /// 0xd90
			 0x9fc6d53f, /// 0xd94
			 0x45bcc699, /// 0xd98
			 0xf0fcf0b4, /// 0xd9c
			 0xb566c6da, /// 0xda0
			 0x1b6a0b1d, /// 0xda4
			 0x58267417, /// 0xda8
			 0xb589965e, /// 0xdac
			 0x3d02baf2, /// 0xdb0
			 0xc9411880, /// 0xdb4
			 0xda4cb5cf, /// 0xdb8
			 0x31b05c15, /// 0xdbc
			 0xe72ff95b, /// 0xdc0
			 0x7ca8f25e, /// 0xdc4
			 0xefdee339, /// 0xdc8
			 0x52770026, /// 0xdcc
			 0x6c0268a7, /// 0xdd0
			 0x9ef4de0e, /// 0xdd4
			 0xa8a450f0, /// 0xdd8
			 0x2a67195b, /// 0xddc
			 0xf72a0746, /// 0xde0
			 0xfdca9f5c, /// 0xde4
			 0x9a60d79f, /// 0xde8
			 0x7b8fab3a, /// 0xdec
			 0x1195ba35, /// 0xdf0
			 0xc3ff5139, /// 0xdf4
			 0x2b83dcd5, /// 0xdf8
			 0x61d335f1, /// 0xdfc
			 0x98bb8873, /// 0xe00
			 0xa1832407, /// 0xe04
			 0x1197fb6b, /// 0xe08
			 0x00d43c8f, /// 0xe0c
			 0x36238c1c, /// 0xe10
			 0x066dea48, /// 0xe14
			 0x1f2b1220, /// 0xe18
			 0x1796fbfa, /// 0xe1c
			 0x2ce9e785, /// 0xe20
			 0xd3774cf6, /// 0xe24
			 0x974c2bef, /// 0xe28
			 0x8c422290, /// 0xe2c
			 0xadbf0d8b, /// 0xe30
			 0x86b65699, /// 0xe34
			 0xb77e7764, /// 0xe38
			 0x9c07d14f, /// 0xe3c
			 0x0cf56e7f, /// 0xe40
			 0x8144edb5, /// 0xe44
			 0x99015b99, /// 0xe48
			 0xd7177adf, /// 0xe4c
			 0x0896b607, /// 0xe50
			 0x7fd81e32, /// 0xe54
			 0x7557813a, /// 0xe58
			 0x6dbb9799, /// 0xe5c
			 0xc7fad77d, /// 0xe60
			 0x6ebc8397, /// 0xe64
			 0xfcd7e986, /// 0xe68
			 0x1db6e0bc, /// 0xe6c
			 0xf8bc4027, /// 0xe70
			 0xb1e4df07, /// 0xe74
			 0xb0fbb426, /// 0xe78
			 0xacbc0a7c, /// 0xe7c
			 0x4a1fb353, /// 0xe80
			 0xb0287594, /// 0xe84
			 0x1e226b3a, /// 0xe88
			 0x45a4efb9, /// 0xe8c
			 0xd7697fa2, /// 0xe90
			 0x45f94153, /// 0xe94
			 0xf9221b8a, /// 0xe98
			 0x07ce65ad, /// 0xe9c
			 0x9ef84560, /// 0xea0
			 0x4d459930, /// 0xea4
			 0xd5d34edd, /// 0xea8
			 0xd18d729e, /// 0xeac
			 0x9366e19c, /// 0xeb0
			 0xc49dd55b, /// 0xeb4
			 0x52f97244, /// 0xeb8
			 0x686a985c, /// 0xebc
			 0x00cddef9, /// 0xec0
			 0x6df64011, /// 0xec4
			 0x89dbd2ee, /// 0xec8
			 0x01c3f71c, /// 0xecc
			 0x3e244794, /// 0xed0
			 0xbf300f06, /// 0xed4
			 0xb96efb82, /// 0xed8
			 0xb9b0354e, /// 0xedc
			 0xc3522917, /// 0xee0
			 0xb11ded41, /// 0xee4
			 0x0ca33277, /// 0xee8
			 0xa29daa21, /// 0xeec
			 0xddcf098c, /// 0xef0
			 0x024a89ad, /// 0xef4
			 0xc5410a84, /// 0xef8
			 0x349d35d5, /// 0xefc
			 0x52c455fb, /// 0xf00
			 0x9e426f4c, /// 0xf04
			 0xa9ca365f, /// 0xf08
			 0xf21312e8, /// 0xf0c
			 0x6e683edd, /// 0xf10
			 0xf65b1058, /// 0xf14
			 0x02cd1f3c, /// 0xf18
			 0x595a46d5, /// 0xf1c
			 0x61118c88, /// 0xf20
			 0x2ed54c34, /// 0xf24
			 0x78dcbbdf, /// 0xf28
			 0xebf415bf, /// 0xf2c
			 0x5c7beb50, /// 0xf30
			 0x533fdeba, /// 0xf34
			 0xe3dd18dd, /// 0xf38
			 0xd7463a2a, /// 0xf3c
			 0xe5bdf549, /// 0xf40
			 0xb20a4ff5, /// 0xf44
			 0xa7fa0c37, /// 0xf48
			 0x94d3ca1b, /// 0xf4c
			 0x6c3fd47b, /// 0xf50
			 0x64a1f203, /// 0xf54
			 0x8e1b48c2, /// 0xf58
			 0x82d2dacf, /// 0xf5c
			 0x2cb7795f, /// 0xf60
			 0x9066a3c3, /// 0xf64
			 0x33a80850, /// 0xf68
			 0x5e263b03, /// 0xf6c
			 0x483fba3d, /// 0xf70
			 0xcec1e736, /// 0xf74
			 0x337bdb9b, /// 0xf78
			 0x2d507be3, /// 0xf7c
			 0x1c3ee4c9, /// 0xf80
			 0x7a0d8df0, /// 0xf84
			 0x21e22c2a, /// 0xf88
			 0x659afe81, /// 0xf8c
			 0x5154cdf4, /// 0xf90
			 0xfa66ae68, /// 0xf94
			 0xc57b00b1, /// 0xf98
			 0x76350714, /// 0xf9c
			 0x761688c0, /// 0xfa0
			 0x01a88f20, /// 0xfa4
			 0xa4a6aec5, /// 0xfa8
			 0xa9e5f88a, /// 0xfac
			 0x04c6e072, /// 0xfb0
			 0xad0b55fc, /// 0xfb4
			 0x62f1e13c, /// 0xfb8
			 0xa14b03ca, /// 0xfbc
			 0x1abd64a5, /// 0xfc0
			 0x6375a322, /// 0xfc4
			 0xeed01528, /// 0xfc8
			 0x76d9ce2d, /// 0xfcc
			 0xe2804d79, /// 0xfd0
			 0xdd419121, /// 0xfd4
			 0x9a13bffd, /// 0xfd8
			 0xfa781df3, /// 0xfdc
			 0x55e1b765, /// 0xfe0
			 0x02be2edb, /// 0xfe4
			 0x91d23b2a, /// 0xfe8
			 0x2c226007, /// 0xfec
			 0x5c8d450d, /// 0xff0
			 0x26866e4f, /// 0xff4
			 0x475cc01c, /// 0xff8
			 0xebeb8a54 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00000
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c600000,                                                  // Leading 1s:                                 /// 0000c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00010
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00014
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00018
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00020
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00024
			 0xff800000,                                                  // -inf                                        /// 00028
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0002c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00030
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00038
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0003c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00048
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0004c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00050
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00054
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00060
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00064
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00068
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xbf028f5c,                                                  // -0.51                                       /// 00070
			 0x0e000003,                                                  // Trailing 1s:                                /// 00074
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00078
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0007c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00080
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00084
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00088
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0008c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00090
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00094
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0009c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000c0
			 0x55555555,                                                  // 4 random values                             /// 000c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00104
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00108
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0011c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00124
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00130
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00134
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0013c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00140
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00148
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0014c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00150
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x80011111,                                                  // -9.7958E-41                                 /// 00158
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00160
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00164
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0016c
			 0x80000000,                                                  // -zero                                       /// 00170
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00174
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00178
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0017c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00184
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00190
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00198
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001c8
			 0xbf028f5c,                                                  // -0.51                                       /// 001cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00200
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00204
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0020c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00210
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00214
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00218
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0021c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00224
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00228
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0022c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00230
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00234
			 0x80000000,                                                  // -zero                                       /// 00238
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0023c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00244
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0024c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00254
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00258
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00264
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00274
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00278
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80011111,                                                  // -9.7958E-41                                 /// 00284
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0028c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00294
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00298
			 0x0e000007,                                                  // Trailing 1s:                                /// 0029c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002a4
			 0x3f028f5c,                                                  // 0.51                                        /// 002a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 002d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002dc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002f0
			 0xcb000000,                                                  // -8388608.0                                  /// 002f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00304
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00308
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0xffc00001,                                                  // -signaling NaN                              /// 00310
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00314
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x0c400000,                                                  // Leading 1s:                                 /// 0031c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00320
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x0c780000,                                                  // Leading 1s:                                 /// 0032c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00330
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00334
			 0x0e000007,                                                  // Trailing 1s:                                /// 00338
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0033c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00344
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00348
			 0x0c600000,                                                  // Leading 1s:                                 /// 0034c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00350
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00354
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00358
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00360
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00364
			 0x7fc00001,                                                  // signaling NaN                               /// 00368
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0036c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00370
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00374
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e000001,                                                  // Trailing 1s:                                /// 0037c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00380
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00388
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0xff800000,                                                  // -inf                                        /// 00390
			 0x0c700000,                                                  // Leading 1s:                                 /// 00394
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0039c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0xff800000,                                                  // -inf                                        /// 003ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00400
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00408
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0040c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00410
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00414
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00418
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0041c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00424
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00000000,                                                  // zero                                        /// 0042c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00438
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x7f800000,                                                  // inf                                         /// 00444
			 0x4b000000,                                                  // 8388608.0                                   /// 00448
			 0x55555555,                                                  // 4 random values                             /// 0044c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00450
			 0xff800000,                                                  // -inf                                        /// 00454
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0045c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00460
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00464
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00468
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0046c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00470
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x33333333,                                                  // 4 random values                             /// 0048c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00490
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0049c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004a8
			 0x7f800000,                                                  // inf                                         /// 004ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004c4
			 0x55555555,                                                  // 4 random values                             /// 004c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00500
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0050c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00510
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00514
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00518
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0051c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00520
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00528
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0052c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00530
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00534
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00538
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x55555555,                                                  // 4 random values                             /// 00540
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00548
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00550
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00558
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0055c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00560
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0xbf028f5c,                                                  // -0.51                                       /// 00568
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00570
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00574
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0057c
			 0x7fc00001,                                                  // signaling NaN                               /// 00580
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00584
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00588
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0058c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x0c700000,                                                  // Leading 1s:                                 /// 00594
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00598
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00600
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00608
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00610
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00614
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00618
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0061c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00620
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00624
			 0x7f800000,                                                  // inf                                         /// 00628
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0062c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00630
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00634
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00638
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00640
			 0x4b000000,                                                  // 8388608.0                                   /// 00644
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00648
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0064c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00650
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00654
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0065c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00660
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00664
			 0xcb000000,                                                  // -8388608.0                                  /// 00668
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00674
			 0xffc00001,                                                  // -signaling NaN                              /// 00678
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00684
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00690
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00694
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00698
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0069c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006b4
			 0xbf028f5c,                                                  // -0.51                                       /// 006b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0xff800000,                                                  // -inf                                        /// 006cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 006d0
			 0x7fc00001,                                                  // signaling NaN                               /// 006d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00700
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00708
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00710
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00718
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00724
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00728
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00730
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00734
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0073c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00740
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00744
			 0x4b000000,                                                  // 8388608.0                                   /// 00748
			 0x0c400000,                                                  // Leading 1s:                                 /// 0074c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00750
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00754
			 0x7fc00001,                                                  // signaling NaN                               /// 00758
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00760
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00764
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0076c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00770
			 0xffc00001,                                                  // -signaling NaN                              /// 00774
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00784
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00788
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00790
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00794
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0079c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d8
			 0x7fc00001,                                                  // signaling NaN                               /// 007dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 007e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0xffc00001,                                                  // -signaling NaN                              /// 007f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00800
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00804
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00808
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00810
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0081c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00820
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00824
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0082c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00830
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00840
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00844
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00848
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0084c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00850
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0e000001,                                                  // Trailing 1s:                                /// 00858
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0085c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00860
			 0x7fc00001,                                                  // signaling NaN                               /// 00864
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00870
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00874
			 0x33333333,                                                  // 4 random values                             /// 00878
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00880
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x3f028f5c,                                                  // 0.51                                        /// 00888
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0088c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00890
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00894
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00898
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00011111,                                                  // 9.7958E-41                                  /// 00904
			 0x0e000003,                                                  // Trailing 1s:                                /// 00908
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00918
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0091c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00920
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0092c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x33333333,                                                  // 4 random values                             /// 00940
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00000000,                                                  // zero                                        /// 00948
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0094c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00954
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00958
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0095c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00964
			 0x80000000,                                                  // -zero                                       /// 00968
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0096c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00970
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0097c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00980
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00988
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00994
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00998
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x3f028f5c,                                                  // 0.51                                        /// 009b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009bc
			 0x4b000000,                                                  // 8388608.0                                   /// 009c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a04
			 0x3f028f5c,                                                  // 0.51                                        /// 00a08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a18
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00000000,                                                  // zero                                        /// 00a28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a30
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x00000000,                                                  // zero                                        /// 00a94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a9c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00aa0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ab8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ac4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ac8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00acc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ad4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ae0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ae4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00af0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00af8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b10
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b2c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b64
			 0xcb000000,                                                  // -8388608.0                                  /// 00b68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ba0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ba4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bb4
			 0x7f800000,                                                  // inf                                         /// 00bb8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bbc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bc0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bc4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bd0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bd8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bdc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00be0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00be8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bf0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x4b000000,                                                  // 8388608.0                                   /// 00c08
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c0c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c34
			 0x80000000,                                                  // -zero                                       /// 00c38
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c40
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c6c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x7f800000,                                                  // inf                                         /// 00c78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c7c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x33333333,                                                  // 4 random values                             /// 00ca8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cb4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cbc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cc4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ccc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cd4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ce0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ce4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x55555555,                                                  // 4 random values                             /// 00cf4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d28
			 0x80000000,                                                  // -zero                                       /// 00d2c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d48
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d60
			 0x3f028f5c,                                                  // 0.51                                        /// 00d64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d90
			 0x4b000000,                                                  // 8388608.0                                   /// 00d94
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00da8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00db0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00db4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00db8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dbc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dc8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dcc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x55555555,                                                  // 4 random values                             /// 00de0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dec
			 0x33333333,                                                  // 4 random values                             /// 00df0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00df8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dfc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e04
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0xbf028f5c,                                                  // -0.51                                       /// 00e0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e14
			 0xffc00001,                                                  // -signaling NaN                              /// 00e18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x7fc00001,                                                  // signaling NaN                               /// 00e48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e5c
			 0x33333333,                                                  // 4 random values                             /// 00e60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e6c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e70
			 0x80000000,                                                  // -zero                                       /// 00e74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ea0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00eac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00eb4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eb8
			 0x33333333,                                                  // 4 random values                             /// 00ebc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ec0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ec4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ee0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ee4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0x80000000,                                                  // -zero                                       /// 00ef8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00efc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f00
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f1c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x80000000,                                                  // -zero                                       /// 00f2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f30
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f8c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80000000,                                                  // -zero                                       /// 00f9c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fa0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fb4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fb8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fc4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fcc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fd4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fd8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fe0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fe8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ff0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ff4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ff8
			 0x8f7ffffc                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0x0e000001,                                                  // Trailing 1s:                                /// 00004
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00008
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00014
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00018
			 0x0e000003,                                                  // Trailing 1s:                                /// 0001c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00024
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00028
			 0x3f028f5c,                                                  // 0.51                                        /// 0002c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00030
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00038
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0003c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00048
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0004c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00050
			 0x0e000007,                                                  // Trailing 1s:                                /// 00054
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00064
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00068
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0006c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00074
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00078
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0007c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00088
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00090
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00094
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0009c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000a8
			 0x7fc00001,                                                  // signaling NaN                               /// 000ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x7f800000,                                                  // inf                                         /// 000d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0xff800000,                                                  // -inf                                        /// 000e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00100
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00114
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00118
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00120
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00128
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00130
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x4b000000,                                                  // 8388608.0                                   /// 00138
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0013c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00140
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00148
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0014c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00154
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00158
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0015c
			 0xff800000,                                                  // -inf                                        /// 00160
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00168
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0016c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00170
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00174
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x0e000003,                                                  // Trailing 1s:                                /// 0017c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00180
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0018c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00190
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0019c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x4b000000,                                                  // 8388608.0                                   /// 001dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00200
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00204
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00208
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00210
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00214
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00218
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0021c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00220
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00224
			 0x0c400000,                                                  // Leading 1s:                                 /// 00228
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0022c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00230
			 0x0e000001,                                                  // Trailing 1s:                                /// 00234
			 0x0c780000,                                                  // Leading 1s:                                 /// 00238
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x7f800000,                                                  // inf                                         /// 00244
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00250
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00254
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00260
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00268
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00274
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00278
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0027c
			 0xff800000,                                                  // -inf                                        /// 00280
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00284
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00290
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00294
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x7fc00001,                                                  // signaling NaN                               /// 0029c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0xbf028f5c,                                                  // -0.51                                       /// 002f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00300
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00308
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00314
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00318
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0031c
			 0x7fc00001,                                                  // signaling NaN                               /// 00320
			 0x0c400000,                                                  // Leading 1s:                                 /// 00324
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00330
			 0x33333333,                                                  // 4 random values                             /// 00334
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00338
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0033c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00344
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00348
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0034c
			 0x55555555,                                                  // 4 random values                             /// 00350
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00354
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00358
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0035c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00360
			 0x0e000001,                                                  // Trailing 1s:                                /// 00364
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0036c
			 0x33333333,                                                  // 4 random values                             /// 00370
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x7fc00001,                                                  // signaling NaN                               /// 00378
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0037c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00380
			 0x0e000001,                                                  // Trailing 1s:                                /// 00384
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0038c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00390
			 0x3f028f5c,                                                  // 0.51                                        /// 00394
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x80000000,                                                  // -zero                                       /// 003a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003f8
			 0x80000000,                                                  // -zero                                       /// 003fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00400
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0040c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x7fc00001,                                                  // signaling NaN                               /// 00414
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0041c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00420
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00424
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00428
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0042c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00434
			 0x0e000003,                                                  // Trailing 1s:                                /// 00438
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00440
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00444
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0044c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00450
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00454
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0045c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00464
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00468
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0046c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00474
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00480
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x55555555,                                                  // 4 random values                             /// 00488
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0048c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00494
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0049c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00500
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00504
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00510
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00518
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00520
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00524
			 0x80011111,                                                  // -9.7958E-41                                 /// 00528
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0052c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00530
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00534
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00540
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00544
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00554
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00558
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00560
			 0x80011111,                                                  // -9.7958E-41                                 /// 00564
			 0x55555555,                                                  // 4 random values                             /// 00568
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0056c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00570
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00574
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00578
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0057c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00580
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00584
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00598
			 0x0c780000,                                                  // Leading 1s:                                 /// 0059c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005bc
			 0x3f028f5c,                                                  // 0.51                                        /// 005c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00600
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00604
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0060c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00610
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00614
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00618
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0061c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00620
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00624
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x3f028f5c,                                                  // 0.51                                        /// 0062c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00630
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00638
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0063c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00640
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00644
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00648
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00650
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00654
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00660
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00668
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0066c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00674
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00678
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0067c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0068c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00690
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00694
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00698
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0069c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x33333333,                                                  // 4 random values                             /// 006a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006d0
			 0x80000000,                                                  // -zero                                       /// 006d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0e000001,                                                  // Trailing 1s:                                /// 00704
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00708
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0070c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00710
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00714
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0071c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00720
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00724
			 0x00000000,                                                  // zero                                        /// 00728
			 0x3f028f5c,                                                  // 0.51                                        /// 0072c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x7f800000,                                                  // inf                                         /// 00744
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0074c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00750
			 0x80011111,                                                  // -9.7958E-41                                 /// 00754
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00758
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0075c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00760
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00768
			 0x0c780000,                                                  // Leading 1s:                                 /// 0076c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00770
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00778
			 0x7fc00001,                                                  // signaling NaN                               /// 0077c
			 0x80000000,                                                  // -zero                                       /// 00780
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00788
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00790
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x3f028f5c,                                                  // 0.51                                        /// 007bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00800
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00804
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00810
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00818
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0c400000,                                                  // Leading 1s:                                 /// 00824
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00834
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00838
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00840
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00844
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00848
			 0x3f028f5c,                                                  // 0.51                                        /// 0084c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00850
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00854
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00858
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00860
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0086c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00870
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00874
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00878
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00880
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0088c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00894
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00898
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0089c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008b8
			 0x4b000000,                                                  // 8388608.0                                   /// 008bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00900
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00904
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00908
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0090c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00910
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00914
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00920
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00930
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0093c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00940
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00944
			 0x0c700000,                                                  // Leading 1s:                                 /// 00948
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0094c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00954
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00958
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0095c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00964
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00968
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0096c
			 0x55555555,                                                  // 4 random values                             /// 00970
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xff800000,                                                  // -inf                                        /// 00978
			 0x00011111,                                                  // 9.7958E-41                                  /// 0097c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00980
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00984
			 0x33333333,                                                  // 4 random values                             /// 00988
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0098c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00990
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00994
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0099c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x4b000000,                                                  // 8388608.0                                   /// 009fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a08
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a14
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a18
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a30
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a34
			 0xff800000,                                                  // -inf                                        /// 00a38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a3c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a44
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x00000000,                                                  // zero                                        /// 00a4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a50
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a54
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x4b000000,                                                  // 8388608.0                                   /// 00a5c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00aa4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00aa8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ab0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ab4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00abc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ac0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ac4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ac8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00acc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ad4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00adc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ae0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ae8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00af0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00af8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b0c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b10
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b24
			 0x7fc00001,                                                  // signaling NaN                               /// 00b28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0xbf028f5c,                                                  // -0.51                                       /// 00b34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xffc00001,                                                  // -signaling NaN                              /// 00b9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ba0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bbc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bc0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00be4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bf0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bf4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bfc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c18
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c30
			 0x3f028f5c,                                                  // 0.51                                        /// 00c34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c60
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c68
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c78
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x7fc00001,                                                  // signaling NaN                               /// 00c88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ca4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ca8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cb4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cb8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cc0
			 0x3f028f5c,                                                  // 0.51                                        /// 00cc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cc8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x55555555,                                                  // 4 random values                             /// 00cd0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cd4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ce8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cf0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cf8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cfc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d18
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d1c
			 0xff800000,                                                  // -inf                                        /// 00d20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d38
			 0x80000000,                                                  // -zero                                       /// 00d3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d64
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00da0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00db0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00db4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00000000,                                                  // zero                                        /// 00dc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dc4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dc8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dcc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dd0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ddc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00de8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00df0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00df4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e0c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e54
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e80
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ea0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ea8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00eac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eb0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ebc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0xff800000,                                                  // -inf                                        /// 00ec4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ed4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ed8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ee4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ee8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00efc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f34
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f60
			 0xff800000,                                                  // -inf                                        /// 00f64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f78
			 0x3f028f5c,                                                  // 0.51                                        /// 00f7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f88
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fa0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fa4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fc0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fc4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fc8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fe0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fe4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fe8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ff0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ff8
			 0x00000800                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00008
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00010
			 0x7f800000,                                                  // inf                                         /// 00014
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00018
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0001c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00020
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00024
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00028
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0002c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00030
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00034
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00038
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00040
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00048
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0004c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00050
			 0xcb000000,                                                  // -8388608.0                                  /// 00054
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00058
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0005c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x4b000000,                                                  // 8388608.0                                   /// 00064
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00068
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00070
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00074
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00078
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0007c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00084
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00088
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0008c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00090
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00098
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00104
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00108
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0010c
			 0xff800000,                                                  // -inf                                        /// 00110
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00114
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00118
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0011c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00120
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00124
			 0xffc00001,                                                  // -signaling NaN                              /// 00128
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0012c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00130
			 0x7fc00001,                                                  // signaling NaN                               /// 00134
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00138
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00148
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0014c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00150
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00154
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0015c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x00000000,                                                  // zero                                        /// 00164
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00168
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00170
			 0x7fc00001,                                                  // signaling NaN                               /// 00174
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00180
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00188
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00198
			 0x80000000,                                                  // -zero                                       /// 0019c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001b8
			 0x55555555,                                                  // 4 random values                             /// 001bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001dc
			 0x55555555,                                                  // 4 random values                             /// 001e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00200
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00204
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00208
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00210
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00214
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0021c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00220
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00228
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00230
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00234
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x80000000,                                                  // -zero                                       /// 0023c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00240
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0024c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00254
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00258
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00260
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00268
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0026c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00270
			 0xffc00001,                                                  // -signaling NaN                              /// 00274
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00288
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00290
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00294
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0029c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x33333333,                                                  // 4 random values                             /// 002b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x4b000000,                                                  // 8388608.0                                   /// 002c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002d4
			 0xffc00001,                                                  // -signaling NaN                              /// 002d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f4
			 0x7fc00001,                                                  // signaling NaN                               /// 002f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00304
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00308
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0030c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00314
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0031c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00320
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0032c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00330
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00334
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00340
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00344
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00348
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0034c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00354
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00360
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00364
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00368
			 0x0c780000,                                                  // Leading 1s:                                 /// 0036c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00370
			 0x3f028f5c,                                                  // 0.51                                        /// 00374
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00378
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0038c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00394
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0039c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0xcb000000,                                                  // -8388608.0                                  /// 003ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00400
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00408
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0040c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00410
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00418
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0041c
			 0xbf028f5c,                                                  // -0.51                                       /// 00420
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0042c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00434
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00438
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00440
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00444
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00448
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0044c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00450
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00454
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00458
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0045c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00460
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0046c
			 0x33333333,                                                  // 4 random values                             /// 00470
			 0x80000000,                                                  // -zero                                       /// 00474
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00478
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0047c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00480
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00484
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00488
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00490
			 0x7f800000,                                                  // inf                                         /// 00494
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0049c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004ac
			 0x3f028f5c,                                                  // 0.51                                        /// 004b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004b4
			 0xcb000000,                                                  // -8388608.0                                  /// 004b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004d4
			 0x33333333,                                                  // 4 random values                             /// 004d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004f8
			 0x3f028f5c,                                                  // 0.51                                        /// 004fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00500
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00504
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00508
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0050c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00510
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00514
			 0x33333333,                                                  // 4 random values                             /// 00518
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00524
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00528
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0052c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00530
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00534
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x4b000000,                                                  // 8388608.0                                   /// 00548
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00554
			 0x80011111,                                                  // -9.7958E-41                                 /// 00558
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00560
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00568
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0056c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00578
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0057c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00584
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0058c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00590
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00598
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0059c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 005cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005e0
			 0x4b000000,                                                  // 8388608.0                                   /// 005e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005ec
			 0x3f028f5c,                                                  // 0.51                                        /// 005f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00604
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0060c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00610
			 0xcb000000,                                                  // -8388608.0                                  /// 00614
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00618
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0061c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00624
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0062c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00634
			 0x0c400000,                                                  // Leading 1s:                                 /// 00638
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0063c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00644
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00648
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00650
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00654
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0065c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00660
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00664
			 0x0e000007,                                                  // Trailing 1s:                                /// 00668
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0066c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00670
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00678
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0067c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00680
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00684
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00690
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00698
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0069c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 006b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006d4
			 0x4b000000,                                                  // 8388608.0                                   /// 006d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006f0
			 0xffc00001,                                                  // -signaling NaN                              /// 006f4
			 0x55555555,                                                  // 4 random values                             /// 006f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00704
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0070c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00710
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0e000001,                                                  // Trailing 1s:                                /// 00718
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00720
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00724
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00728
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00730
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00738
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0073c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00740
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00744
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00748
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0074c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00750
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00754
			 0xffc00001,                                                  // -signaling NaN                              /// 00758
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00760
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00764
			 0x80011111,                                                  // -9.7958E-41                                 /// 00768
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0076c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00784
			 0x0c400000,                                                  // Leading 1s:                                 /// 00788
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00794
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0079c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 007d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00800
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00808
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0080c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00810
			 0xff800000,                                                  // -inf                                        /// 00814
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00818
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0081c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00820
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00824
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00838
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80000000,                                                  // -zero                                       /// 00840
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00848
			 0xffc00001,                                                  // -signaling NaN                              /// 0084c
			 0xff800000,                                                  // -inf                                        /// 00850
			 0xbf028f5c,                                                  // -0.51                                       /// 00854
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0085c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00860
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00874
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x0e000003,                                                  // Trailing 1s:                                /// 0087c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00880
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00888
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0088c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00890
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00894
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0089c
			 0x0c700000,                                                  // Leading 1s:                                 /// 008a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008b0
			 0x80000000,                                                  // -zero                                       /// 008b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008c4
			 0x7fc00001,                                                  // signaling NaN                               /// 008c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008e8
			 0x4b000000,                                                  // 8388608.0                                   /// 008ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00900
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00904
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0090c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00910
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0091c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00920
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00924
			 0x0c400000,                                                  // Leading 1s:                                 /// 00928
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0092c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00938
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00940
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00944
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00948
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0094c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00950
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00958
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0095c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00960
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00964
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0096c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00974
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00980
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00984
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000000,                                                  // -zero                                       /// 00990
			 0x80011111,                                                  // -9.7958E-41                                 /// 00994
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00998
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009a4
			 0x4b000000,                                                  // 8388608.0                                   /// 009a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009b0
			 0xff800000,                                                  // -inf                                        /// 009b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x7fc00001,                                                  // signaling NaN                               /// 009d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f0
			 0xff800000,                                                  // -inf                                        /// 009f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a10
			 0x00000000,                                                  // zero                                        /// 00a14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a20
			 0x3f028f5c,                                                  // 0.51                                        /// 00a24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a34
			 0x80000000,                                                  // -zero                                       /// 00a38
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a48
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a4c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a50
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a54
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a7c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a84
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a94
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aa4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00aa8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00aac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ab8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00abc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ac0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ad4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ae8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00af0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00af4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00af8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00afc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b14
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b18
			 0xbf028f5c,                                                  // -0.51                                       /// 00b1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b28
			 0x4b000000,                                                  // 8388608.0                                   /// 00b2c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b4c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0xffc00001,                                                  // -signaling NaN                              /// 00b64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0x7fc00001,                                                  // signaling NaN                               /// 00b8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ba0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ba8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bb4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0xbf028f5c,                                                  // -0.51                                       /// 00bd0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bd8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bdc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00be0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bf0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bf4
			 0xff800000,                                                  // -inf                                        /// 00bf8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c04
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c08
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c34
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c4c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c6c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c90
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cb8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cbc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc0
			 0x7f800000,                                                  // inf                                         /// 00cc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cc8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cd0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cdc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ce0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x7f800000,                                                  // inf                                         /// 00cec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cf0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cf4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cf8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cfc
			 0x55555555,                                                  // 4 random values                             /// 00d00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d10
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d14
			 0x7f800000,                                                  // inf                                         /// 00d18
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d6c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d80
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00da0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00da4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da8
			 0x80000000,                                                  // -zero                                       /// 00dac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00db0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00db8
			 0xcb000000,                                                  // -8388608.0                                  /// 00dbc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dc0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dc8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dd4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ddc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00de8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00df0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00df4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00df8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dfc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e38
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e44
			 0xff800000,                                                  // -inf                                        /// 00e48
			 0x80000000,                                                  // -zero                                       /// 00e4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e68
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e94
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ea4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00eb0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00eb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eb8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ebc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ec4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ec8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ed4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ed8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ee4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ef0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ef4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f14
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x7f800000,                                                  // inf                                         /// 00f2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f30
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f44
			 0x7fc00001,                                                  // signaling NaN                               /// 00f48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f4c
			 0x33333333,                                                  // 4 random values                             /// 00f50
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f70
			 0x3f028f5c,                                                  // 0.51                                        /// 00f74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x3f028f5c,                                                  // 0.51                                        /// 00fa4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fb0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fb8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fc4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fcc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fdc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x3f028f5c,                                                  // 0.51                                        /// 00fe4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fe8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ff0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ff4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ff8
			 0x00000400                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00000
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00004
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0000c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00010
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00014
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00018
			 0xffc00001,                                                  // -signaling NaN                              /// 0001c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00020
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00028
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0002c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00034
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00040
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00044
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0004c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00050
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0xff800000,                                                  // -inf                                        /// 00058
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0005c
			 0x80000000,                                                  // -zero                                       /// 00060
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00064
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00068
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0006c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00070
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00074
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00078
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0007c
			 0x7f800000,                                                  // inf                                         /// 00080
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00084
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00088
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0008c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00094
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00098
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0009c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00100
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00104
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00108
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0010c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0011c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00120
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00124
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0012c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00130
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0013c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00140
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x80011111,                                                  // -9.7958E-41                                 /// 00148
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0014c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00150
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00164
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00168
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0016c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00170
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00178
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00180
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00184
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0018c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00190
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0019c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a0
			 0xcb000000,                                                  // -8388608.0                                  /// 001a4
			 0x33333333,                                                  // 4 random values                             /// 001a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 001b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00200
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0e000001,                                                  // Trailing 1s:                                /// 00208
			 0x33333333,                                                  // 4 random values                             /// 0020c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00210
			 0xbf028f5c,                                                  // -0.51                                       /// 00214
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00220
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00224
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0022c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00230
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0023c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00240
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x80000000,                                                  // -zero                                       /// 00248
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0024c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00250
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00254
			 0x0e000003,                                                  // Trailing 1s:                                /// 00258
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00260
			 0xffc00001,                                                  // -signaling NaN                              /// 00264
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00268
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0026c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00270
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00274
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x80011111,                                                  // -9.7958E-41                                 /// 0027c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00284
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00288
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00290
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00298
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002b4
			 0x7f800000,                                                  // inf                                         /// 002b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x55555555,                                                  // 4 random values                             /// 002cc
			 0xffc00001,                                                  // -signaling NaN                              /// 002d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002f0
			 0x4b000000,                                                  // 8388608.0                                   /// 002f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0030c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00310
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x4b000000,                                                  // 8388608.0                                   /// 00318
			 0x0e000007,                                                  // Trailing 1s:                                /// 0031c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00320
			 0xffc00001,                                                  // -signaling NaN                              /// 00324
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x3f028f5c,                                                  // 0.51                                        /// 0032c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x80011111,                                                  // -9.7958E-41                                 /// 00334
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00338
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0033c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00348
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0034c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00350
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00358
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0035c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00364
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00368
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00370
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0037c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00380
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00384
			 0x7f800000,                                                  // inf                                         /// 00388
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0038c
			 0x33333333,                                                  // 4 random values                             /// 00390
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00394
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x7f800000,                                                  // inf                                         /// 0039c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00000000,                                                  // zero                                        /// 003b4
			 0xff800000,                                                  // -inf                                        /// 003b8
			 0x55555555,                                                  // 4 random values                             /// 003bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0040c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00410
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00414
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0041c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00420
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00424
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0042c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00438
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0c400000,                                                  // Leading 1s:                                 /// 00444
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00448
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00450
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00458
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0045c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00460
			 0x3f028f5c,                                                  // 0.51                                        /// 00464
			 0x0c600000,                                                  // Leading 1s:                                 /// 00468
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0046c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00474
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00478
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0047c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00490
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0049c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004bc
			 0x4b000000,                                                  // 8388608.0                                   /// 004c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x7f800000,                                                  // inf                                         /// 004d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004fc
			 0x55555555,                                                  // 4 random values                             /// 00500
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x80000000,                                                  // -zero                                       /// 00508
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00510
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00514
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00518
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00520
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00524
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00528
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0052c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00530
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00534
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0c400000,                                                  // Leading 1s:                                 /// 0053c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00548
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0054c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00550
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0055c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00560
			 0x0e000007,                                                  // Trailing 1s:                                /// 00564
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0056c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00570
			 0x00011111,                                                  // 9.7958E-41                                  /// 00574
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00578
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0057c
			 0xbf028f5c,                                                  // -0.51                                       /// 00580
			 0xbf028f5c,                                                  // -0.51                                       /// 00584
			 0x0c400000,                                                  // Leading 1s:                                 /// 00588
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0058c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x80000000,                                                  // -zero                                       /// 00594
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0059c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0xcb000000,                                                  // -8388608.0                                  /// 005a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x33333333,                                                  // 4 random values                             /// 005c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005d8
			 0x3f028f5c,                                                  // 0.51                                        /// 005dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x3f028f5c,                                                  // 0.51                                        /// 00604
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x33333333,                                                  // 4 random values                             /// 0060c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00618
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0061c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00620
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00624
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00628
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00630
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00634
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0e000003,                                                  // Trailing 1s:                                /// 0063c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00640
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00644
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00648
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00650
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00654
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0x00000000,                                                  // zero                                        /// 00664
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00670
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00678
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0067c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00684
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00688
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00690
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00698
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006c4
			 0xffc00001,                                                  // -signaling NaN                              /// 006c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006d4
			 0xffc00001,                                                  // -signaling NaN                              /// 006d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00700
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00708
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0070c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0xff800000,                                                  // -inf                                        /// 00714
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00718
			 0x80011111,                                                  // -9.7958E-41                                 /// 0071c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00720
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00724
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0073c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00740
			 0x0c400000,                                                  // Leading 1s:                                 /// 00744
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00748
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00750
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00758
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00764
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00768
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0076c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00770
			 0x0e000003,                                                  // Trailing 1s:                                /// 00774
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0077c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00780
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00784
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00788
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00794
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00798
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0079c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007a8
			 0x00000000,                                                  // zero                                        /// 007ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 007b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00804
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00810
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00814
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00818
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0081c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00820
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00828
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00830
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x80000000,                                                  // -zero                                       /// 00838
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00840
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00844
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00848
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0084c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00850
			 0x3f028f5c,                                                  // 0.51                                        /// 00854
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00858
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0085c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00860
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00864
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00868
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0086c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00870
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0087c
			 0x7fc00001,                                                  // signaling NaN                               /// 00880
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00884
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00888
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00890
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00894
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0089c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008ac
			 0xbf028f5c,                                                  // -0.51                                       /// 008b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 008c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x3f028f5c,                                                  // 0.51                                        /// 008d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x80000000,                                                  // -zero                                       /// 00900
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00904
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00908
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0090c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00910
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00914
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0091c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00920
			 0x0e000001,                                                  // Trailing 1s:                                /// 00924
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00928
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0092c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00930
			 0xff800000,                                                  // -inf                                        /// 00934
			 0xff800000,                                                  // -inf                                        /// 00938
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0093c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00944
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x80000000,                                                  // -zero                                       /// 00958
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0095c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00960
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00968
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0096c
			 0x00000000,                                                  // zero                                        /// 00970
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00974
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00978
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00980
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00984
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00988
			 0x0c780000,                                                  // Leading 1s:                                 /// 0098c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00990
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00994
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0099c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x33333333,                                                  // 4 random values                             /// 009ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x7fc00001,                                                  // signaling NaN                               /// 009e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 009f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a08
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a38
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a40
			 0xcb000000,                                                  // -8388608.0                                  /// 00a44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a50
			 0xcb000000,                                                  // -8388608.0                                  /// 00a54
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a58
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0xbf028f5c,                                                  // -0.51                                       /// 00a84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a88
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00abc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ac0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ad8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00adc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ae0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ae8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x3f028f5c,                                                  // 0.51                                        /// 00af0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00afc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b28
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b44
			 0xbf028f5c,                                                  // -0.51                                       /// 00b48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bbc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bc0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x00000000,                                                  // zero                                        /// 00bcc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bdc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00be8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bec
			 0xcb000000,                                                  // -8388608.0                                  /// 00bf0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bf8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bfc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c04
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c10
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c18
			 0xbf028f5c,                                                  // -0.51                                       /// 00c1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c20
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c50
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c70
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c78
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c88
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c90
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ca4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cb0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cb8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ccc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ce4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ce8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cf4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cf8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d10
			 0xffc00001,                                                  // -signaling NaN                              /// 00d14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0xbf028f5c,                                                  // -0.51                                       /// 00d34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d38
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d44
			 0xffc00001,                                                  // -signaling NaN                              /// 00d48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d6c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d88
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d94
			 0x7fc00001,                                                  // signaling NaN                               /// 00d98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00da0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00da8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dac
			 0x80000000,                                                  // -zero                                       /// 00db0
			 0x55555555,                                                  // 4 random values                             /// 00db4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00db8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dc4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dc8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dd8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ddc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00df0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00df4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e38
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e4c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e54
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e78
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e84
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e90
			 0xbf028f5c,                                                  // -0.51                                       /// 00e94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ea0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00eb4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00ebc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ec0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ee0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ee4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ee8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ef0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ef4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00efc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f34
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x7fc00001,                                                  // signaling NaN                               /// 00f4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x7f800000,                                                  // inf                                         /// 00f68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f8c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f94
			 0x00000000,                                                  // zero                                        /// 00f98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fa0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fa4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fa8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fc4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fd4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fe0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fe4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ff0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ff4
			 0x7f800000,                                                  // inf                                         /// 00ff8
			 0x00800001                                                  // none of the mantissa set to +3ulp           /// last
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
			 0x0000078c,
			 0x0000079c,
			 0x00000580,
			 0x000004b4,
			 0x00000618,
			 0x00000658,
			 0x00000224,
			 0x0000067c,

			 /// vpu register f2
			 0x40c00000,
			 0x41400000,
			 0x41880000,
			 0x41000000,
			 0x41f80000,
			 0x41980000,
			 0x41e80000,
			 0x41c00000,

			 /// vpu register f3
			 0x41f00000,
			 0x41a00000,
			 0x40a00000,
			 0x41a80000,
			 0x41a00000,
			 0x41000000,
			 0x40000000,
			 0x41200000,

			 /// vpu register f4
			 0x41900000,
			 0x41700000,
			 0x3f800000,
			 0x41a80000,
			 0x41e00000,
			 0x40000000,
			 0x3f800000,
			 0x41400000,

			 /// vpu register f5
			 0x41f80000,
			 0x41a80000,
			 0x41f00000,
			 0x41c80000,
			 0x42000000,
			 0x41980000,
			 0x40e00000,
			 0x41a00000,

			 /// vpu register f6
			 0x42000000,
			 0x41200000,
			 0x41400000,
			 0x42000000,
			 0x41200000,
			 0x41800000,
			 0x41f80000,
			 0x40400000,

			 /// vpu register f7
			 0x40000000,
			 0x41200000,
			 0x41900000,
			 0x41900000,
			 0x40e00000,
			 0x41100000,
			 0x41880000,
			 0x41f80000,

			 /// vpu register f8
			 0x41200000,
			 0x41c00000,
			 0x40000000,
			 0x41400000,
			 0x41600000,
			 0x40400000,
			 0x41d80000,
			 0x40e00000,

			 /// vpu register f9
			 0x41800000,
			 0x41880000,
			 0x40800000,
			 0x41200000,
			 0x41900000,
			 0x40000000,
			 0x41a80000,
			 0x41a00000,

			 /// vpu register f10
			 0x41700000,
			 0x3f800000,
			 0x40e00000,
			 0x41d80000,
			 0x3f800000,
			 0x41400000,
			 0x41700000,
			 0x41d80000,

			 /// vpu register f11
			 0x41200000,
			 0x41d00000,
			 0x40400000,
			 0x41f80000,
			 0x41c80000,
			 0x41200000,
			 0x41900000,
			 0x41100000,

			 /// vpu register f12
			 0x40e00000,
			 0x41700000,
			 0x41b80000,
			 0x41d80000,
			 0x41e00000,
			 0x41e00000,
			 0x40400000,
			 0x40a00000,

			 /// vpu register f13
			 0x41d00000,
			 0x41f80000,
			 0x41900000,
			 0x3f800000,
			 0x40800000,
			 0x41f80000,
			 0x41880000,
			 0x40000000,

			 /// vpu register f14
			 0x41c80000,
			 0x41900000,
			 0x40c00000,
			 0x41a80000,
			 0x41e80000,
			 0x41900000,
			 0x41b80000,
			 0x40000000,

			 /// vpu register f15
			 0x41c00000,
			 0x41100000,
			 0x41c00000,
			 0x41c80000,
			 0x41d00000,
			 0x41200000,
			 0x41e00000,
			 0x41700000,

			 /// vpu register f16
			 0x3f800000,
			 0x40e00000,
			 0x41200000,
			 0x41200000,
			 0x41d00000,
			 0x41700000,
			 0x41900000,
			 0x41d80000,

			 /// vpu register f17
			 0x41400000,
			 0x41e80000,
			 0x41900000,
			 0x41500000,
			 0x41c80000,
			 0x41e80000,
			 0x41f00000,
			 0x41980000,

			 /// vpu register f18
			 0x40e00000,
			 0x41200000,
			 0x41d00000,
			 0x41d00000,
			 0x40e00000,
			 0x40800000,
			 0x41300000,
			 0x41100000,

			 /// vpu register f19
			 0x41500000,
			 0x41100000,
			 0x41600000,
			 0x40a00000,
			 0x41880000,
			 0x41700000,
			 0x40800000,
			 0x41f00000,

			 /// vpu register f20
			 0x40e00000,
			 0x41800000,
			 0x41880000,
			 0x41900000,
			 0x41e00000,
			 0x40800000,
			 0x42000000,
			 0x41200000,

			 /// vpu register f21
			 0x41a00000,
			 0x41d00000,
			 0x41980000,
			 0x40000000,
			 0x40c00000,
			 0x41300000,
			 0x41100000,
			 0x41c80000,

			 /// vpu register f22
			 0x3f800000,
			 0x41e80000,
			 0x40800000,
			 0x41900000,
			 0x41000000,
			 0x41100000,
			 0x40800000,
			 0x41c80000,

			 /// vpu register f23
			 0x41e80000,
			 0x41c00000,
			 0x41000000,
			 0x41880000,
			 0x41e00000,
			 0x41800000,
			 0x40000000,
			 0x41100000,

			 /// vpu register f24
			 0x3f800000,
			 0x40400000,
			 0x3f800000,
			 0x41f80000,
			 0x40800000,
			 0x40e00000,
			 0x41100000,
			 0x41a00000,

			 /// vpu register f25
			 0x41000000,
			 0x41100000,
			 0x40000000,
			 0x41d00000,
			 0x41f00000,
			 0x42000000,
			 0x41d00000,
			 0x41400000,

			 /// vpu register f26
			 0x41c80000,
			 0x3f800000,
			 0x40400000,
			 0x41d80000,
			 0x41c00000,
			 0x3f800000,
			 0x41800000,
			 0x41f00000,

			 /// vpu register f27
			 0x41e00000,
			 0x41b80000,
			 0x41100000,
			 0x41500000,
			 0x41100000,
			 0x41000000,
			 0x40800000,
			 0x41e80000,

			 /// vpu register f28
			 0x41c80000,
			 0x41d80000,
			 0x41800000,
			 0x41a00000,
			 0x41200000,
			 0x3f800000,
			 0x41d80000,
			 0x40c00000,

			 /// vpu register f29
			 0x41c80000,
			 0x40000000,
			 0x41000000,
			 0x40c00000,
			 0x41100000,
			 0x41300000,
			 0x41880000,
			 0x41200000,

			 /// vpu register f30
			 0x3f800000,
			 0x41a00000,
			 0x40a00000,
			 0x40c00000,
			 0x41c80000,
			 0x40400000,
			 0x41600000,
			 0x41100000,

			 /// vpu register f31
			 0x41d80000,
			 0x41d00000,
			 0x40000000,
			 0x41600000,
			 0x41e00000,
			 0x41400000,
			 0x40c00000,
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
		"fg32w.ps f17, x14 (x13)\n"                           ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f27, x14 (x14)\n"                           ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f17, x12 (x15)\n"                           ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f30, x14 (x15)\n"                           ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f22, x13 (x11)\n"                           ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f14, x14 (x15)\n"                           ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f5, x15 (x13)\n"                            ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f16, x14 (x14)\n"                           ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f4, x13 (x15)\n"                            ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f4, x15 (x14)\n"                            ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f22, x12 (x12)\n"                           ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f5, x15 (x11)\n"                            ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f6, x13 (x15)\n"                            ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f29, x12 (x11)\n"                           ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f23, x14 (x12)\n"                           ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f30, x13 (x14)\n"                           ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f3, x14 (x15)\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f5, x11 (x11)\n"                            ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f14, x13 (x12)\n"                           ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f20, x15 (x15)\n"                           ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f20, x15 (x14)\n"                           ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f6, x11 (x14)\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f27, x11 (x11)\n"                           ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f22, x13 (x13)\n"                           ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f3, x15 (x15)\n"                            ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f11, x14 (x12)\n"                           ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f27, x11 (x14)\n"                           ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f15, x13 (x14)\n"                           ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f11, x13 (x13)\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f25, x12 (x12)\n"                           ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f16, x13 (x14)\n"                           ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f29, x15 (x15)\n"                           ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f14, x12 (x11)\n"                           ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f14, x12 (x11)\n"                           ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f16, x11 (x11)\n"                           ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f20, x13 (x12)\n"                           ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f26, x11 (x15)\n"                           ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f15, x13 (x13)\n"                           ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f25, x14 (x15)\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f8, x12 (x14)\n"                            ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f22, x12 (x15)\n"                           ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f18, x15 (x11)\n"                           ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f11, x12 (x13)\n"                           ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f13, x11 (x15)\n"                           ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f24, x14 (x14)\n"                           ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f8, x13 (x14)\n"                            ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f15, x11 (x13)\n"                           ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f4, x14 (x11)\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f9, x15 (x15)\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f12, x15 (x12)\n"                           ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f23, x15 (x12)\n"                           ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f18, x12 (x11)\n"                           ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f14, x15 (x11)\n"                           ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f28, x15 (x15)\n"                           ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f7, x14 (x13)\n"                            ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f20, x14 (x11)\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f26, x14 (x12)\n"                           ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f30, x14 (x12)\n"                           ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f4, x12 (x11)\n"                            ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f29, x13 (x11)\n"                           ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f11, x13 (x11)\n"                           ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f4, x14 (x12)\n"                            ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f17, x12 (x11)\n"                           ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f9, x13 (x13)\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f10, x12 (x11)\n"                           ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f16, x11 (x15)\n"                           ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f5, x13 (x11)\n"                            ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f8, x13 (x14)\n"                            ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f11, x15 (x12)\n"                           ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f27, x12 (x12)\n"                           ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f20, x13 (x13)\n"                           ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f19, x13 (x12)\n"                           ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f27, x13 (x15)\n"                           ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f20, x13 (x13)\n"                           ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f24, x14 (x14)\n"                           ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f24, x13 (x11)\n"                           ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f2, x14 (x13)\n"                            ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f27, x12 (x11)\n"                           ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f22, x15 (x15)\n"                           ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f27, x15 (x13)\n"                           ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f26, x14 (x13)\n"                           ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f18, x12 (x13)\n"                           ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f18, x15 (x13)\n"                           ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f25, x14 (x12)\n"                           ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f10, x11 (x15)\n"                           ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f7, x13 (x12)\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f22, x13 (x12)\n"                           ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f29, x11 (x11)\n"                           ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f23, x13 (x15)\n"                           ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f29, x13 (x13)\n"                           ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f5, x11 (x12)\n"                            ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f2, x11 (x15)\n"                            ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f27, x15 (x14)\n"                           ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f22, x13 (x13)\n"                           ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f24, x14 (x12)\n"                           ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f19, x11 (x15)\n"                           ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f6, x11 (x15)\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f12, x12 (x12)\n"                           ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32w.ps f19, x11 (x11)\n"                           ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
