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
			 0x79bd5f4e, /// 0x0
			 0x111d8cc0, /// 0x4
			 0x70bec512, /// 0x8
			 0x86297573, /// 0xc
			 0xaf339124, /// 0x10
			 0x17ce326e, /// 0x14
			 0x0b332ee0, /// 0x18
			 0x9e1be046, /// 0x1c
			 0xd96cebaf, /// 0x20
			 0x7eb007e0, /// 0x24
			 0x1c28c8c0, /// 0x28
			 0xfe64f72a, /// 0x2c
			 0xc5f4bfe8, /// 0x30
			 0xcf840cf4, /// 0x34
			 0xe1ba281e, /// 0x38
			 0x96de78e2, /// 0x3c
			 0x0bdf5cdf, /// 0x40
			 0x8ec89f33, /// 0x44
			 0x47b9cfcd, /// 0x48
			 0x3084a3fc, /// 0x4c
			 0x311c3394, /// 0x50
			 0xb13dd325, /// 0x54
			 0x0f3e1a7d, /// 0x58
			 0x65b17794, /// 0x5c
			 0x4bf2b159, /// 0x60
			 0x9c15beab, /// 0x64
			 0xce1e0278, /// 0x68
			 0x154411f9, /// 0x6c
			 0xbe5026a1, /// 0x70
			 0xada81846, /// 0x74
			 0x04570ed5, /// 0x78
			 0x2393baca, /// 0x7c
			 0xbab59781, /// 0x80
			 0xf9445484, /// 0x84
			 0xac10b08f, /// 0x88
			 0x7f598f05, /// 0x8c
			 0xc85dd212, /// 0x90
			 0xaa723746, /// 0x94
			 0x8d94cccc, /// 0x98
			 0xb0cc2612, /// 0x9c
			 0x95e5b58b, /// 0xa0
			 0x550b5b19, /// 0xa4
			 0x555a599c, /// 0xa8
			 0x508b47f1, /// 0xac
			 0x1091ae17, /// 0xb0
			 0x5e64527f, /// 0xb4
			 0x5459cd7a, /// 0xb8
			 0xcbb38431, /// 0xbc
			 0x2e241199, /// 0xc0
			 0x136f41f7, /// 0xc4
			 0x86b06c68, /// 0xc8
			 0x6294ce5c, /// 0xcc
			 0x2ab58318, /// 0xd0
			 0x382bf15b, /// 0xd4
			 0x78e83bce, /// 0xd8
			 0x3c285573, /// 0xdc
			 0x3248d11a, /// 0xe0
			 0x32b838f2, /// 0xe4
			 0x5218d9a9, /// 0xe8
			 0x79d93e29, /// 0xec
			 0x32acca6b, /// 0xf0
			 0x932800f7, /// 0xf4
			 0x66981b19, /// 0xf8
			 0xf07a5a30, /// 0xfc
			 0xc80e804e, /// 0x100
			 0xea82b2cc, /// 0x104
			 0xf4e7680a, /// 0x108
			 0x42eba5f5, /// 0x10c
			 0x0507da1a, /// 0x110
			 0x86e5adde, /// 0x114
			 0x679691bb, /// 0x118
			 0x35918f6d, /// 0x11c
			 0x1554a0e4, /// 0x120
			 0xf6967cb6, /// 0x124
			 0x30297397, /// 0x128
			 0xbed814f9, /// 0x12c
			 0x3cb9b5ec, /// 0x130
			 0xf308f956, /// 0x134
			 0x349d2e0c, /// 0x138
			 0x34a1f95f, /// 0x13c
			 0x72f9d358, /// 0x140
			 0x2e3d747a, /// 0x144
			 0xf7417507, /// 0x148
			 0x5a9f2170, /// 0x14c
			 0xcd205aae, /// 0x150
			 0x8c62bdf0, /// 0x154
			 0xca8c2e51, /// 0x158
			 0x22e89e6b, /// 0x15c
			 0x3639396b, /// 0x160
			 0x5e1a0946, /// 0x164
			 0xe401ac4f, /// 0x168
			 0x81b8aad8, /// 0x16c
			 0x0c0716f8, /// 0x170
			 0x1db3dbf8, /// 0x174
			 0x3ef0c4f1, /// 0x178
			 0x08a52fe8, /// 0x17c
			 0x95f257b1, /// 0x180
			 0xf33ac0b5, /// 0x184
			 0x7f467fab, /// 0x188
			 0x25b71e4c, /// 0x18c
			 0x9980e22f, /// 0x190
			 0x8198a250, /// 0x194
			 0x7aac5a82, /// 0x198
			 0xbba3a7ca, /// 0x19c
			 0xff67b4e2, /// 0x1a0
			 0x7c1dbdfc, /// 0x1a4
			 0x62b67b50, /// 0x1a8
			 0x4443027b, /// 0x1ac
			 0xf05989c9, /// 0x1b0
			 0xaadec6b2, /// 0x1b4
			 0x2cbc275b, /// 0x1b8
			 0x0ba4104d, /// 0x1bc
			 0xcc678a40, /// 0x1c0
			 0xb210ed40, /// 0x1c4
			 0x4225c2ef, /// 0x1c8
			 0xb672f531, /// 0x1cc
			 0x421660aa, /// 0x1d0
			 0x9e8e722a, /// 0x1d4
			 0xed714755, /// 0x1d8
			 0x247af120, /// 0x1dc
			 0x1b4027e7, /// 0x1e0
			 0x0e9b1ec6, /// 0x1e4
			 0xfc22d1b2, /// 0x1e8
			 0x96cd30af, /// 0x1ec
			 0xa7fa0dcc, /// 0x1f0
			 0x9c51debc, /// 0x1f4
			 0x400460df, /// 0x1f8
			 0x8d4a3119, /// 0x1fc
			 0x3261656e, /// 0x200
			 0xc28b736a, /// 0x204
			 0x3621789c, /// 0x208
			 0x7458c7ff, /// 0x20c
			 0x62541219, /// 0x210
			 0xd59acf18, /// 0x214
			 0xec8b11b1, /// 0x218
			 0x07b82544, /// 0x21c
			 0x0003b473, /// 0x220
			 0xf723f3dd, /// 0x224
			 0xe7757dd3, /// 0x228
			 0x1d6be6f0, /// 0x22c
			 0x38f108a1, /// 0x230
			 0xeab7a18e, /// 0x234
			 0xf36525d2, /// 0x238
			 0x3d6a1717, /// 0x23c
			 0x7ea0c30d, /// 0x240
			 0x008d9a68, /// 0x244
			 0xe091cff4, /// 0x248
			 0x4d54eda1, /// 0x24c
			 0x0d70983b, /// 0x250
			 0xa7af8634, /// 0x254
			 0xaf1ad946, /// 0x258
			 0x4324bf6e, /// 0x25c
			 0xdcbacbc1, /// 0x260
			 0x84de74a8, /// 0x264
			 0x4c6587dd, /// 0x268
			 0x362e46d3, /// 0x26c
			 0x1830796b, /// 0x270
			 0x0e1ec2f9, /// 0x274
			 0x9a3c9f5a, /// 0x278
			 0xa1522268, /// 0x27c
			 0x744603b8, /// 0x280
			 0xd3b5674e, /// 0x284
			 0xaeb6b811, /// 0x288
			 0x25e4dda5, /// 0x28c
			 0x7651074c, /// 0x290
			 0xbd309681, /// 0x294
			 0x3bf45d63, /// 0x298
			 0x9c6d681f, /// 0x29c
			 0x1795757b, /// 0x2a0
			 0x7ba5ce02, /// 0x2a4
			 0x459ff919, /// 0x2a8
			 0x4b5df280, /// 0x2ac
			 0xb04b26ae, /// 0x2b0
			 0x43027f16, /// 0x2b4
			 0x3cabcaad, /// 0x2b8
			 0x2caf953d, /// 0x2bc
			 0xd632c8e3, /// 0x2c0
			 0x10f0ed5b, /// 0x2c4
			 0xd3fd527b, /// 0x2c8
			 0x423e9622, /// 0x2cc
			 0x9e9a5acc, /// 0x2d0
			 0xd117a67d, /// 0x2d4
			 0x8e32b6c6, /// 0x2d8
			 0x9d17e059, /// 0x2dc
			 0xa2f35c25, /// 0x2e0
			 0x4041f852, /// 0x2e4
			 0x0ffe0833, /// 0x2e8
			 0x413c5329, /// 0x2ec
			 0x49e5fa72, /// 0x2f0
			 0x70350914, /// 0x2f4
			 0x56b61793, /// 0x2f8
			 0xa78d66ba, /// 0x2fc
			 0x78cc593e, /// 0x300
			 0x617e5ca7, /// 0x304
			 0x723696b3, /// 0x308
			 0x4ec934e1, /// 0x30c
			 0xdf27561d, /// 0x310
			 0xf79d6c81, /// 0x314
			 0x505b5b31, /// 0x318
			 0x445124d1, /// 0x31c
			 0xd6e79749, /// 0x320
			 0xccc9a40e, /// 0x324
			 0x4d2c755f, /// 0x328
			 0x7810b2bb, /// 0x32c
			 0x2f5296cf, /// 0x330
			 0x211d7cd8, /// 0x334
			 0x374e4fbb, /// 0x338
			 0x79c2366a, /// 0x33c
			 0x88551415, /// 0x340
			 0xd880b8b1, /// 0x344
			 0x1fc9d62b, /// 0x348
			 0xd4fcea0d, /// 0x34c
			 0x0e11b719, /// 0x350
			 0xdeb40895, /// 0x354
			 0x89b465e3, /// 0x358
			 0x25958536, /// 0x35c
			 0xe46d7161, /// 0x360
			 0x55eeced3, /// 0x364
			 0xf599221d, /// 0x368
			 0x8a635e8b, /// 0x36c
			 0xed0a9643, /// 0x370
			 0x51570cdc, /// 0x374
			 0xbd7fce4a, /// 0x378
			 0x2a24c14a, /// 0x37c
			 0x5a3e869f, /// 0x380
			 0x8dbec384, /// 0x384
			 0xfd8498a6, /// 0x388
			 0x1bb13b1b, /// 0x38c
			 0x141a0db6, /// 0x390
			 0x1b3cb01e, /// 0x394
			 0xf1e6da42, /// 0x398
			 0xa56c80f2, /// 0x39c
			 0xd4a31182, /// 0x3a0
			 0x95384372, /// 0x3a4
			 0x1e4e0228, /// 0x3a8
			 0xec3e43e2, /// 0x3ac
			 0x3f072d28, /// 0x3b0
			 0xebd72194, /// 0x3b4
			 0x69510c3c, /// 0x3b8
			 0x80d15bd8, /// 0x3bc
			 0x20161bd4, /// 0x3c0
			 0xc488a66b, /// 0x3c4
			 0x42bb576b, /// 0x3c8
			 0x395a2c69, /// 0x3cc
			 0x0e8a1d00, /// 0x3d0
			 0xe384a86d, /// 0x3d4
			 0x022b3d53, /// 0x3d8
			 0x14b075bc, /// 0x3dc
			 0xb32ee5c1, /// 0x3e0
			 0x33fcf80e, /// 0x3e4
			 0x1e5da99a, /// 0x3e8
			 0xee5d1f85, /// 0x3ec
			 0x032c9911, /// 0x3f0
			 0x77079bfa, /// 0x3f4
			 0x061df498, /// 0x3f8
			 0xe87ddb37, /// 0x3fc
			 0x53aad77c, /// 0x400
			 0x6f40a628, /// 0x404
			 0x0747c8d3, /// 0x408
			 0xad50dc7c, /// 0x40c
			 0x9d11ba64, /// 0x410
			 0x2a7838e7, /// 0x414
			 0x738f0ac4, /// 0x418
			 0x34c146f9, /// 0x41c
			 0x121b69b7, /// 0x420
			 0xc47c928d, /// 0x424
			 0x51d94320, /// 0x428
			 0xf0fb25ce, /// 0x42c
			 0x47d0394f, /// 0x430
			 0x7039da79, /// 0x434
			 0xa45bdae5, /// 0x438
			 0x0fb67977, /// 0x43c
			 0xb56f3e02, /// 0x440
			 0x78b6e333, /// 0x444
			 0xb6a18b36, /// 0x448
			 0xb9a87cd0, /// 0x44c
			 0xfcd5ca26, /// 0x450
			 0xdb372380, /// 0x454
			 0xb2efe406, /// 0x458
			 0x242392b3, /// 0x45c
			 0x4b72ef61, /// 0x460
			 0x6a179f7a, /// 0x464
			 0x410c3066, /// 0x468
			 0xe0c12cd6, /// 0x46c
			 0xb9e7c467, /// 0x470
			 0x511d9b8f, /// 0x474
			 0xa19c0fa3, /// 0x478
			 0x4b4a389a, /// 0x47c
			 0x0af6c085, /// 0x480
			 0x8d6fe7e4, /// 0x484
			 0x34181305, /// 0x488
			 0xaec05231, /// 0x48c
			 0xa174b5fd, /// 0x490
			 0x9eded5f3, /// 0x494
			 0x4f8fc00f, /// 0x498
			 0xee0e81fc, /// 0x49c
			 0xef03cb5f, /// 0x4a0
			 0x72281d7a, /// 0x4a4
			 0x4126896a, /// 0x4a8
			 0x4389a7d0, /// 0x4ac
			 0x6abc20b1, /// 0x4b0
			 0x8b90a757, /// 0x4b4
			 0xe4616825, /// 0x4b8
			 0x723fcb25, /// 0x4bc
			 0x2a9779aa, /// 0x4c0
			 0x3e9cd054, /// 0x4c4
			 0x88ca373f, /// 0x4c8
			 0x24aeebf5, /// 0x4cc
			 0x9dc3aa0a, /// 0x4d0
			 0xd3a184f0, /// 0x4d4
			 0xbe3c4d3f, /// 0x4d8
			 0xa09f1120, /// 0x4dc
			 0x08f04408, /// 0x4e0
			 0x616bd4b2, /// 0x4e4
			 0xca7a9b4e, /// 0x4e8
			 0x8d63580e, /// 0x4ec
			 0xc38dc29d, /// 0x4f0
			 0x6a6f3239, /// 0x4f4
			 0x133e1033, /// 0x4f8
			 0xfdf6f965, /// 0x4fc
			 0x75f57508, /// 0x500
			 0xe7cab12d, /// 0x504
			 0xfcd83866, /// 0x508
			 0x5d7f5d1c, /// 0x50c
			 0xcb795e14, /// 0x510
			 0x3fcffdc6, /// 0x514
			 0xc6ff8704, /// 0x518
			 0xfddff2d3, /// 0x51c
			 0xd654c3ef, /// 0x520
			 0x3eae52f6, /// 0x524
			 0x8cacc0cf, /// 0x528
			 0x11ac375c, /// 0x52c
			 0x1281644b, /// 0x530
			 0x9018af03, /// 0x534
			 0x23481497, /// 0x538
			 0xa2e17210, /// 0x53c
			 0x08c8970e, /// 0x540
			 0x669ddff9, /// 0x544
			 0xe611b45f, /// 0x548
			 0xf787b3d2, /// 0x54c
			 0x2c47cc40, /// 0x550
			 0xcd97a5e0, /// 0x554
			 0x1d9d6925, /// 0x558
			 0xd31620d7, /// 0x55c
			 0xc08296c3, /// 0x560
			 0x83210bec, /// 0x564
			 0xeb9f37c4, /// 0x568
			 0xe0c4063a, /// 0x56c
			 0xa6126439, /// 0x570
			 0xea24bd23, /// 0x574
			 0xb8b1078d, /// 0x578
			 0xc8abea61, /// 0x57c
			 0x353fe12a, /// 0x580
			 0xbfca4979, /// 0x584
			 0xce70fd12, /// 0x588
			 0x5d5cbb12, /// 0x58c
			 0x1650f5b4, /// 0x590
			 0xf4bc3424, /// 0x594
			 0x68e5f40e, /// 0x598
			 0x18fd1dd3, /// 0x59c
			 0x1241b0cc, /// 0x5a0
			 0xbe2be544, /// 0x5a4
			 0x34ac00c2, /// 0x5a8
			 0x6506a2a7, /// 0x5ac
			 0x5b2c77b0, /// 0x5b0
			 0x82d0ce66, /// 0x5b4
			 0xb0166d49, /// 0x5b8
			 0x7acbf6fe, /// 0x5bc
			 0xc36077ac, /// 0x5c0
			 0xec6aa14c, /// 0x5c4
			 0xff89a35c, /// 0x5c8
			 0x33843647, /// 0x5cc
			 0x83e134c2, /// 0x5d0
			 0x4c6556b1, /// 0x5d4
			 0xbed6225c, /// 0x5d8
			 0x1b797598, /// 0x5dc
			 0x9d74e6b9, /// 0x5e0
			 0xdf4c845f, /// 0x5e4
			 0xbaf98a76, /// 0x5e8
			 0xd5d06b2f, /// 0x5ec
			 0xd64ba221, /// 0x5f0
			 0x9ee9bcd9, /// 0x5f4
			 0x2221f84d, /// 0x5f8
			 0xc5f30f7f, /// 0x5fc
			 0x1fcf257f, /// 0x600
			 0xc4d4ed33, /// 0x604
			 0xe3c5c45b, /// 0x608
			 0x90877199, /// 0x60c
			 0x8c0a542e, /// 0x610
			 0x15878bd9, /// 0x614
			 0x62c6d9ef, /// 0x618
			 0xc05f5f04, /// 0x61c
			 0x858c4fab, /// 0x620
			 0x3e016c25, /// 0x624
			 0x86539ab5, /// 0x628
			 0x78e8a2a4, /// 0x62c
			 0x9be0f073, /// 0x630
			 0xf2a1087a, /// 0x634
			 0x340a1172, /// 0x638
			 0x966d230d, /// 0x63c
			 0x742ba8a9, /// 0x640
			 0xddce3453, /// 0x644
			 0x7a32ea4b, /// 0x648
			 0xc8839cfd, /// 0x64c
			 0x29b2acce, /// 0x650
			 0x7db96a2b, /// 0x654
			 0x448c183e, /// 0x658
			 0x32a4e8d2, /// 0x65c
			 0x3a4c0a48, /// 0x660
			 0x71355f2b, /// 0x664
			 0xaf813051, /// 0x668
			 0xa7f219e0, /// 0x66c
			 0x3a222614, /// 0x670
			 0xeb130f07, /// 0x674
			 0x029097a6, /// 0x678
			 0x264c0cac, /// 0x67c
			 0xec0f0cf2, /// 0x680
			 0x040b63e5, /// 0x684
			 0x1751f0ec, /// 0x688
			 0x70d81d40, /// 0x68c
			 0x40895b3f, /// 0x690
			 0x09bf5a89, /// 0x694
			 0x5502cbf6, /// 0x698
			 0x1858bb54, /// 0x69c
			 0xd54d671e, /// 0x6a0
			 0x58735871, /// 0x6a4
			 0xeb348087, /// 0x6a8
			 0x06b74fe7, /// 0x6ac
			 0xdaa16d55, /// 0x6b0
			 0x3cff72b6, /// 0x6b4
			 0xb4a9f679, /// 0x6b8
			 0xc7cc8c2d, /// 0x6bc
			 0x1907ad2c, /// 0x6c0
			 0x8399b4e1, /// 0x6c4
			 0x2c2c2213, /// 0x6c8
			 0x18aca902, /// 0x6cc
			 0x3de9401c, /// 0x6d0
			 0x293b7bf7, /// 0x6d4
			 0xc3fe56b2, /// 0x6d8
			 0xd210058b, /// 0x6dc
			 0x46593277, /// 0x6e0
			 0x036b73f0, /// 0x6e4
			 0x42dbbefd, /// 0x6e8
			 0x9a40341c, /// 0x6ec
			 0x98165482, /// 0x6f0
			 0x75457174, /// 0x6f4
			 0x19b72479, /// 0x6f8
			 0x11239afe, /// 0x6fc
			 0x0fdd3e7f, /// 0x700
			 0x72a2755c, /// 0x704
			 0xad9ca344, /// 0x708
			 0x169792fe, /// 0x70c
			 0xf455a2d5, /// 0x710
			 0x56ea7352, /// 0x714
			 0x85166a74, /// 0x718
			 0x7068db75, /// 0x71c
			 0x5d7a9cf3, /// 0x720
			 0x3d4d35e8, /// 0x724
			 0x1c5d317a, /// 0x728
			 0x2a8e12a5, /// 0x72c
			 0xb5ef2b81, /// 0x730
			 0xfe0f893f, /// 0x734
			 0x72317619, /// 0x738
			 0xaa9dacbe, /// 0x73c
			 0xf2352dfa, /// 0x740
			 0x634a6efe, /// 0x744
			 0x7dbe34b2, /// 0x748
			 0x920eb398, /// 0x74c
			 0x7f5db8bd, /// 0x750
			 0x99c63815, /// 0x754
			 0xa3984133, /// 0x758
			 0xc737712c, /// 0x75c
			 0x065b73e8, /// 0x760
			 0x3af24c59, /// 0x764
			 0xb46b8fd9, /// 0x768
			 0xa117758a, /// 0x76c
			 0x01e3da60, /// 0x770
			 0xb30ca818, /// 0x774
			 0x8e94ea7e, /// 0x778
			 0xe2c47f90, /// 0x77c
			 0xa2539f61, /// 0x780
			 0x7e9e9f9e, /// 0x784
			 0x9b638964, /// 0x788
			 0xb76864c2, /// 0x78c
			 0x97a1f638, /// 0x790
			 0x0fafdbe6, /// 0x794
			 0x3fe48487, /// 0x798
			 0x252d1b19, /// 0x79c
			 0x4aa6aa61, /// 0x7a0
			 0x0f3ba1c5, /// 0x7a4
			 0x01224618, /// 0x7a8
			 0xa362aa6f, /// 0x7ac
			 0xc230d47c, /// 0x7b0
			 0xf088825a, /// 0x7b4
			 0xea130b95, /// 0x7b8
			 0x6cdb8995, /// 0x7bc
			 0x30efdcc8, /// 0x7c0
			 0xe529706d, /// 0x7c4
			 0x20234e56, /// 0x7c8
			 0xc90874ea, /// 0x7cc
			 0x0001d418, /// 0x7d0
			 0x842fc991, /// 0x7d4
			 0xfe0b1ee7, /// 0x7d8
			 0x2da3b564, /// 0x7dc
			 0x06b564d3, /// 0x7e0
			 0x5b3e0ebf, /// 0x7e4
			 0x2ddbd02e, /// 0x7e8
			 0x1f9a6b6a, /// 0x7ec
			 0x7fbafe06, /// 0x7f0
			 0xddd7fec6, /// 0x7f4
			 0xade3c5b5, /// 0x7f8
			 0x7a4bc482, /// 0x7fc
			 0xf3554663, /// 0x800
			 0x7a986468, /// 0x804
			 0xeae5e556, /// 0x808
			 0x9a7c2b4d, /// 0x80c
			 0x97104cf6, /// 0x810
			 0x00aece2f, /// 0x814
			 0xb6fb34e7, /// 0x818
			 0xb39c4da6, /// 0x81c
			 0x44eae21d, /// 0x820
			 0x82382d48, /// 0x824
			 0x7fed2907, /// 0x828
			 0xe9822b13, /// 0x82c
			 0x9b9e4825, /// 0x830
			 0xaa9127c2, /// 0x834
			 0x9e54952b, /// 0x838
			 0x3f390a87, /// 0x83c
			 0xddf89c6e, /// 0x840
			 0x96189d48, /// 0x844
			 0xc8672832, /// 0x848
			 0x0df50939, /// 0x84c
			 0x94cfd736, /// 0x850
			 0xe44b1544, /// 0x854
			 0x74919f46, /// 0x858
			 0xe07e660b, /// 0x85c
			 0x402e9d3e, /// 0x860
			 0xb5f7b0e6, /// 0x864
			 0x2ecb3d12, /// 0x868
			 0xb23c6ff8, /// 0x86c
			 0x71f73b34, /// 0x870
			 0x101d6a49, /// 0x874
			 0xc6b3e3bf, /// 0x878
			 0x8694ee58, /// 0x87c
			 0x40e4eafe, /// 0x880
			 0x67b76fc5, /// 0x884
			 0x864b6330, /// 0x888
			 0x52ba9210, /// 0x88c
			 0x079c3bc7, /// 0x890
			 0xe31e46a6, /// 0x894
			 0x1410e736, /// 0x898
			 0xdcd42297, /// 0x89c
			 0x9318d0f0, /// 0x8a0
			 0xcbd169de, /// 0x8a4
			 0xee0c693d, /// 0x8a8
			 0xde67cd0a, /// 0x8ac
			 0x85a7a81e, /// 0x8b0
			 0x6bb1ddba, /// 0x8b4
			 0x5138c83b, /// 0x8b8
			 0x41021d95, /// 0x8bc
			 0x6c5112d4, /// 0x8c0
			 0x9d0756ea, /// 0x8c4
			 0xecf23c1d, /// 0x8c8
			 0x6bf324e1, /// 0x8cc
			 0x5b05e6ae, /// 0x8d0
			 0x622fba6a, /// 0x8d4
			 0x6c65b473, /// 0x8d8
			 0x3b6eaf95, /// 0x8dc
			 0xb94101ee, /// 0x8e0
			 0x4da638d7, /// 0x8e4
			 0x5c9c0d11, /// 0x8e8
			 0xe393baed, /// 0x8ec
			 0x8628b5d4, /// 0x8f0
			 0xae42a318, /// 0x8f4
			 0xdab4b8ff, /// 0x8f8
			 0x17c62d58, /// 0x8fc
			 0x4e2daa16, /// 0x900
			 0x018c9793, /// 0x904
			 0x6f1e5f58, /// 0x908
			 0xeda8828c, /// 0x90c
			 0xfa4f08cc, /// 0x910
			 0x4a68cf85, /// 0x914
			 0x215c5d81, /// 0x918
			 0xdf7592f7, /// 0x91c
			 0x020b17eb, /// 0x920
			 0xc371baac, /// 0x924
			 0x09b3352e, /// 0x928
			 0x893325de, /// 0x92c
			 0xc937987e, /// 0x930
			 0xb5b415fe, /// 0x934
			 0xba53681c, /// 0x938
			 0xe5e72e77, /// 0x93c
			 0xcb3b1995, /// 0x940
			 0x55c1af1e, /// 0x944
			 0x7e502076, /// 0x948
			 0xe7f90b07, /// 0x94c
			 0x2976238f, /// 0x950
			 0x6affc60e, /// 0x954
			 0xbe53a0d7, /// 0x958
			 0x1727a97b, /// 0x95c
			 0x73894a3b, /// 0x960
			 0x1c48d682, /// 0x964
			 0xd0002b5f, /// 0x968
			 0x2b57ae99, /// 0x96c
			 0x2ddaad4c, /// 0x970
			 0x847a0753, /// 0x974
			 0x63a70ec7, /// 0x978
			 0xfafc919c, /// 0x97c
			 0x5e9d64f5, /// 0x980
			 0xd9779823, /// 0x984
			 0x3a3cc5e9, /// 0x988
			 0xa904613e, /// 0x98c
			 0x80109729, /// 0x990
			 0xe0a3ad3c, /// 0x994
			 0x4712b3fb, /// 0x998
			 0x28a27d74, /// 0x99c
			 0x14dc9d59, /// 0x9a0
			 0x70734d6f, /// 0x9a4
			 0xb88e9d76, /// 0x9a8
			 0x6be5a91e, /// 0x9ac
			 0x322bbb2a, /// 0x9b0
			 0x33162ffa, /// 0x9b4
			 0xcc3d2ee0, /// 0x9b8
			 0xf1d9b2ba, /// 0x9bc
			 0x918d2872, /// 0x9c0
			 0x2f3e2968, /// 0x9c4
			 0x4e7f0024, /// 0x9c8
			 0x935af71d, /// 0x9cc
			 0x07878417, /// 0x9d0
			 0xd28aa3ea, /// 0x9d4
			 0x8083f3ea, /// 0x9d8
			 0x6754ec37, /// 0x9dc
			 0x918ccabf, /// 0x9e0
			 0x5ddb0298, /// 0x9e4
			 0x6e570593, /// 0x9e8
			 0x1459240d, /// 0x9ec
			 0x8328a527, /// 0x9f0
			 0xdd13c938, /// 0x9f4
			 0x6ab3aad3, /// 0x9f8
			 0x28ef1ec2, /// 0x9fc
			 0x884765f1, /// 0xa00
			 0x320d267f, /// 0xa04
			 0xc4f472d2, /// 0xa08
			 0x8ea5985a, /// 0xa0c
			 0xeb6e8b0e, /// 0xa10
			 0x86c30b97, /// 0xa14
			 0x4e049ac0, /// 0xa18
			 0x2b8298cc, /// 0xa1c
			 0xe8462fde, /// 0xa20
			 0xdcf9a67c, /// 0xa24
			 0x6aecff80, /// 0xa28
			 0x182e193a, /// 0xa2c
			 0x22f895a2, /// 0xa30
			 0x43748966, /// 0xa34
			 0xfcdbb69c, /// 0xa38
			 0x4e517393, /// 0xa3c
			 0xec53035b, /// 0xa40
			 0x2d7471cd, /// 0xa44
			 0x1b918163, /// 0xa48
			 0xad2a0f4c, /// 0xa4c
			 0x30c89ff6, /// 0xa50
			 0xd817ea10, /// 0xa54
			 0x4c79a17b, /// 0xa58
			 0x5677c2ae, /// 0xa5c
			 0x704a4876, /// 0xa60
			 0xb35d48ba, /// 0xa64
			 0x75f79cf3, /// 0xa68
			 0xe5e9eafb, /// 0xa6c
			 0x86f5eaa0, /// 0xa70
			 0x674828ea, /// 0xa74
			 0xadf61ba7, /// 0xa78
			 0x98e50e33, /// 0xa7c
			 0xd6fd2b24, /// 0xa80
			 0x9d0eb24a, /// 0xa84
			 0x4a55bf02, /// 0xa88
			 0x81ce5a7d, /// 0xa8c
			 0xd1dce10c, /// 0xa90
			 0xc4b048cb, /// 0xa94
			 0x10f18fbf, /// 0xa98
			 0x8bf5075b, /// 0xa9c
			 0xe51d160b, /// 0xaa0
			 0x2418716d, /// 0xaa4
			 0xb1168d4d, /// 0xaa8
			 0xaf664228, /// 0xaac
			 0x4b9776f9, /// 0xab0
			 0xe8056803, /// 0xab4
			 0xf7eebfb4, /// 0xab8
			 0x507a1fd4, /// 0xabc
			 0x3a7e35be, /// 0xac0
			 0xc60a9411, /// 0xac4
			 0xa6fbd3b5, /// 0xac8
			 0x883870a6, /// 0xacc
			 0xce1b7196, /// 0xad0
			 0x75d17c58, /// 0xad4
			 0xb58e70cf, /// 0xad8
			 0xc0d43a31, /// 0xadc
			 0x84c9c5c8, /// 0xae0
			 0xcf471e76, /// 0xae4
			 0x239b307f, /// 0xae8
			 0x30bb93d6, /// 0xaec
			 0x2fee8717, /// 0xaf0
			 0x2d35af54, /// 0xaf4
			 0xf7eac00f, /// 0xaf8
			 0x65c4c13c, /// 0xafc
			 0x99e15ec6, /// 0xb00
			 0xd785d200, /// 0xb04
			 0xce6b9544, /// 0xb08
			 0x6928eb05, /// 0xb0c
			 0x0519b873, /// 0xb10
			 0x5481f279, /// 0xb14
			 0xad601fbf, /// 0xb18
			 0x0274d2eb, /// 0xb1c
			 0x5120af8f, /// 0xb20
			 0x99b42bea, /// 0xb24
			 0xd4aff93e, /// 0xb28
			 0xef7b0020, /// 0xb2c
			 0x979dd338, /// 0xb30
			 0x1c5eb223, /// 0xb34
			 0x3e2145d3, /// 0xb38
			 0x4465e6ab, /// 0xb3c
			 0xd3dd6ead, /// 0xb40
			 0xbc7f69d6, /// 0xb44
			 0xa857c67a, /// 0xb48
			 0x22894623, /// 0xb4c
			 0x0e51a71f, /// 0xb50
			 0xf2175cb2, /// 0xb54
			 0xf476779c, /// 0xb58
			 0x196969ae, /// 0xb5c
			 0x93076207, /// 0xb60
			 0x1607b226, /// 0xb64
			 0x13cb7f83, /// 0xb68
			 0x7656e3ae, /// 0xb6c
			 0x9508a7e4, /// 0xb70
			 0x6fd1b4be, /// 0xb74
			 0x8ce4e7c2, /// 0xb78
			 0x84a82010, /// 0xb7c
			 0xbdf2ff75, /// 0xb80
			 0x26bb6879, /// 0xb84
			 0x49f1c040, /// 0xb88
			 0x92dd2243, /// 0xb8c
			 0x7ef02d3f, /// 0xb90
			 0xf6436e41, /// 0xb94
			 0x4a4b4f2c, /// 0xb98
			 0xa4ae78a3, /// 0xb9c
			 0x749e59ac, /// 0xba0
			 0x68f0dcab, /// 0xba4
			 0x75341f4f, /// 0xba8
			 0x40cc624e, /// 0xbac
			 0x39c73256, /// 0xbb0
			 0x7c332bc2, /// 0xbb4
			 0x8a201153, /// 0xbb8
			 0x7ec333c9, /// 0xbbc
			 0xda356aef, /// 0xbc0
			 0x644f9060, /// 0xbc4
			 0x37cc907e, /// 0xbc8
			 0x82c2e7e1, /// 0xbcc
			 0x3fb6c4ba, /// 0xbd0
			 0xb125d4a6, /// 0xbd4
			 0x68377655, /// 0xbd8
			 0x0a9a9ce9, /// 0xbdc
			 0xbf699eed, /// 0xbe0
			 0xd6df463a, /// 0xbe4
			 0x45c3dd72, /// 0xbe8
			 0x57316c90, /// 0xbec
			 0x1cfa5ae1, /// 0xbf0
			 0x405e588d, /// 0xbf4
			 0xe4d7017a, /// 0xbf8
			 0x977508a7, /// 0xbfc
			 0xe24f53bc, /// 0xc00
			 0x7df789f6, /// 0xc04
			 0x80f6096d, /// 0xc08
			 0xd37ee921, /// 0xc0c
			 0x84ef1528, /// 0xc10
			 0xf22e63b3, /// 0xc14
			 0x8693b268, /// 0xc18
			 0x30a57a16, /// 0xc1c
			 0xa72598fa, /// 0xc20
			 0xb351ef3e, /// 0xc24
			 0xcdabbdda, /// 0xc28
			 0xaa2ac052, /// 0xc2c
			 0x8a039f5b, /// 0xc30
			 0x3c61b564, /// 0xc34
			 0xf72eb702, /// 0xc38
			 0x63f39dea, /// 0xc3c
			 0xcec41765, /// 0xc40
			 0xf27b50ec, /// 0xc44
			 0x413746c4, /// 0xc48
			 0x9b111450, /// 0xc4c
			 0xa279b222, /// 0xc50
			 0x013ed594, /// 0xc54
			 0xb9eb5538, /// 0xc58
			 0xd3acc8c0, /// 0xc5c
			 0xe491e19c, /// 0xc60
			 0xb87758d2, /// 0xc64
			 0x5e5edb29, /// 0xc68
			 0xa3f616f2, /// 0xc6c
			 0x8351123d, /// 0xc70
			 0xd3f38aaf, /// 0xc74
			 0xba35f100, /// 0xc78
			 0xda507748, /// 0xc7c
			 0x2ef75955, /// 0xc80
			 0x0af75031, /// 0xc84
			 0xe1213d86, /// 0xc88
			 0xcf6c164a, /// 0xc8c
			 0xdd107134, /// 0xc90
			 0xefc4a92f, /// 0xc94
			 0xed50edac, /// 0xc98
			 0x933a6710, /// 0xc9c
			 0x347627e9, /// 0xca0
			 0x5b987295, /// 0xca4
			 0x15ec5eda, /// 0xca8
			 0xa3b6ce9b, /// 0xcac
			 0x2bf9941d, /// 0xcb0
			 0xb0d51999, /// 0xcb4
			 0x485b09c4, /// 0xcb8
			 0xdd80fa07, /// 0xcbc
			 0x92c75b99, /// 0xcc0
			 0x52163d5a, /// 0xcc4
			 0x4a519978, /// 0xcc8
			 0xa169eb95, /// 0xccc
			 0xaf6f32ca, /// 0xcd0
			 0x8aa4c260, /// 0xcd4
			 0xd51a2796, /// 0xcd8
			 0xf5c1c9d3, /// 0xcdc
			 0xec2854ba, /// 0xce0
			 0xe05890ce, /// 0xce4
			 0x2306f8c3, /// 0xce8
			 0x48dcd03a, /// 0xcec
			 0xaeb96b95, /// 0xcf0
			 0xa4f7d389, /// 0xcf4
			 0x161facda, /// 0xcf8
			 0x1d0f97ae, /// 0xcfc
			 0x091da950, /// 0xd00
			 0x8fba631c, /// 0xd04
			 0x341f699b, /// 0xd08
			 0x1b1a163a, /// 0xd0c
			 0x14117f94, /// 0xd10
			 0x9b16a2d5, /// 0xd14
			 0x8da4f66f, /// 0xd18
			 0x7a32d36f, /// 0xd1c
			 0x469da5aa, /// 0xd20
			 0x22d98992, /// 0xd24
			 0xa6bb29c1, /// 0xd28
			 0x6524c13f, /// 0xd2c
			 0x560e5dcc, /// 0xd30
			 0xecf5486e, /// 0xd34
			 0x9bc63d6c, /// 0xd38
			 0xcd4b108f, /// 0xd3c
			 0x3b66c27e, /// 0xd40
			 0x2ca2bf7b, /// 0xd44
			 0x92c01b0b, /// 0xd48
			 0xf89024cb, /// 0xd4c
			 0xbb4dbbb6, /// 0xd50
			 0xeaa7290f, /// 0xd54
			 0xdd69dcba, /// 0xd58
			 0x33a0237f, /// 0xd5c
			 0x2cc18b47, /// 0xd60
			 0x8fca2ae7, /// 0xd64
			 0x8db8465d, /// 0xd68
			 0x34b12ca0, /// 0xd6c
			 0x0068674f, /// 0xd70
			 0xbe009412, /// 0xd74
			 0xdfce4408, /// 0xd78
			 0xc9a69cfe, /// 0xd7c
			 0xf128ddc4, /// 0xd80
			 0xcfe3da16, /// 0xd84
			 0x5e972eb0, /// 0xd88
			 0x7596923b, /// 0xd8c
			 0x3e8a03c7, /// 0xd90
			 0x7c33f5b5, /// 0xd94
			 0xfa6cc0c6, /// 0xd98
			 0x1d5abe43, /// 0xd9c
			 0x0fe68093, /// 0xda0
			 0x77c65adb, /// 0xda4
			 0xa55661f6, /// 0xda8
			 0xd28502e3, /// 0xdac
			 0xef5ce77e, /// 0xdb0
			 0x004102ca, /// 0xdb4
			 0xc3f672e6, /// 0xdb8
			 0x6dcac0d2, /// 0xdbc
			 0x103eb875, /// 0xdc0
			 0xdf385786, /// 0xdc4
			 0x8568e1e4, /// 0xdc8
			 0x7d50998e, /// 0xdcc
			 0x40003bc7, /// 0xdd0
			 0xba4e6946, /// 0xdd4
			 0x833948b7, /// 0xdd8
			 0xaf46a5f5, /// 0xddc
			 0x34127a57, /// 0xde0
			 0x62954338, /// 0xde4
			 0x3c0c0d8c, /// 0xde8
			 0xc736722f, /// 0xdec
			 0x71867e6a, /// 0xdf0
			 0xcb434f9e, /// 0xdf4
			 0x1d0b85a8, /// 0xdf8
			 0x27480a9b, /// 0xdfc
			 0x53e4fc13, /// 0xe00
			 0x97b0474f, /// 0xe04
			 0x193b0bce, /// 0xe08
			 0x77e9ef46, /// 0xe0c
			 0xa6a1d36d, /// 0xe10
			 0x8cd90ff1, /// 0xe14
			 0xfe130c63, /// 0xe18
			 0xda5e97ab, /// 0xe1c
			 0x6cc36825, /// 0xe20
			 0x1dbd8b5e, /// 0xe24
			 0x566f7986, /// 0xe28
			 0x79e353d1, /// 0xe2c
			 0xaebc89c6, /// 0xe30
			 0xe1319310, /// 0xe34
			 0x53cf3cba, /// 0xe38
			 0xf243d996, /// 0xe3c
			 0xe36e2abb, /// 0xe40
			 0xda78bc61, /// 0xe44
			 0x5714fbae, /// 0xe48
			 0x8906c565, /// 0xe4c
			 0x0fb3ff46, /// 0xe50
			 0x2ef9942d, /// 0xe54
			 0x74e9b836, /// 0xe58
			 0xc3ea1327, /// 0xe5c
			 0x296430e3, /// 0xe60
			 0x03960790, /// 0xe64
			 0xecb55622, /// 0xe68
			 0x640cfaf1, /// 0xe6c
			 0x385bb236, /// 0xe70
			 0xbdda8078, /// 0xe74
			 0x8e25f400, /// 0xe78
			 0x69360a9e, /// 0xe7c
			 0xbe0fb9c1, /// 0xe80
			 0x7007b04d, /// 0xe84
			 0x03688c44, /// 0xe88
			 0x210c77f8, /// 0xe8c
			 0x4d4eaa5c, /// 0xe90
			 0xcbff6c71, /// 0xe94
			 0xddde46c1, /// 0xe98
			 0x0f904830, /// 0xe9c
			 0x417d3444, /// 0xea0
			 0xfd3dff1c, /// 0xea4
			 0xb7216cdd, /// 0xea8
			 0x655bc41a, /// 0xeac
			 0xa2e8a3ad, /// 0xeb0
			 0xf38a041a, /// 0xeb4
			 0xb60f1285, /// 0xeb8
			 0x4a87492f, /// 0xebc
			 0xee4ce1e2, /// 0xec0
			 0x6e4d91d4, /// 0xec4
			 0x55a4e99b, /// 0xec8
			 0x6c3fdcdd, /// 0xecc
			 0xb86da703, /// 0xed0
			 0x08b62fcb, /// 0xed4
			 0xc8c94a27, /// 0xed8
			 0x0a43a5a6, /// 0xedc
			 0x61b54b09, /// 0xee0
			 0x9fc26326, /// 0xee4
			 0xd2f6340c, /// 0xee8
			 0x14750d59, /// 0xeec
			 0x89a662cd, /// 0xef0
			 0xf9d839fd, /// 0xef4
			 0x3456d37c, /// 0xef8
			 0x2d88c1cd, /// 0xefc
			 0xf9797438, /// 0xf00
			 0xe2c7ebc7, /// 0xf04
			 0x1ca14034, /// 0xf08
			 0x5cff9daa, /// 0xf0c
			 0x2b53ac36, /// 0xf10
			 0x7469329e, /// 0xf14
			 0x909178ae, /// 0xf18
			 0x1de5ad07, /// 0xf1c
			 0xe19c55ab, /// 0xf20
			 0x7defe410, /// 0xf24
			 0x4f83ccf5, /// 0xf28
			 0xe3fbde2b, /// 0xf2c
			 0xb6ce13fd, /// 0xf30
			 0x3d93bfa0, /// 0xf34
			 0x671dcb4a, /// 0xf38
			 0xafb0750d, /// 0xf3c
			 0xd2e514dd, /// 0xf40
			 0x56ebd241, /// 0xf44
			 0x0785cc1e, /// 0xf48
			 0x9ceb2e51, /// 0xf4c
			 0x1c86fa11, /// 0xf50
			 0x2c6a3a45, /// 0xf54
			 0x570b7d7d, /// 0xf58
			 0xfe314ae7, /// 0xf5c
			 0xad1f67b0, /// 0xf60
			 0xe55f8b65, /// 0xf64
			 0x8514eea9, /// 0xf68
			 0xef35785d, /// 0xf6c
			 0x15bef047, /// 0xf70
			 0x7549e654, /// 0xf74
			 0x8856daec, /// 0xf78
			 0xaa529250, /// 0xf7c
			 0x7c012ee6, /// 0xf80
			 0xc1302d65, /// 0xf84
			 0x566ce893, /// 0xf88
			 0x3c95ddbb, /// 0xf8c
			 0xd62135bd, /// 0xf90
			 0xe4cfa1bb, /// 0xf94
			 0xbef6bf29, /// 0xf98
			 0x14d1b515, /// 0xf9c
			 0xcf35f74e, /// 0xfa0
			 0xcb9f68c4, /// 0xfa4
			 0x7483cd0c, /// 0xfa8
			 0x8cf7b278, /// 0xfac
			 0xa0406b1f, /// 0xfb0
			 0x1407f992, /// 0xfb4
			 0x429f5780, /// 0xfb8
			 0xc60c5f1e, /// 0xfbc
			 0xeb73ea0c, /// 0xfc0
			 0x7b895d1a, /// 0xfc4
			 0x5b73d0b3, /// 0xfc8
			 0x552eb20b, /// 0xfcc
			 0x031eaca9, /// 0xfd0
			 0x258c78c7, /// 0xfd4
			 0xa1a08755, /// 0xfd8
			 0xc9985836, /// 0xfdc
			 0xea5c6ff0, /// 0xfe0
			 0x117a132c, /// 0xfe4
			 0x155a4a79, /// 0xfe8
			 0x32d68bf2, /// 0xfec
			 0xda7545b8, /// 0xff0
			 0xb66e595c, /// 0xff4
			 0xe9c82da3, /// 0xff8
			 0xf01b28b5 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00000
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00004
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00008
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0000c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00010
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00014
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00018
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0001c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00020
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x0e000001,                                                  // Trailing 1s:                                /// 0002c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00030
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00038
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0xffc00001,                                                  // -signaling NaN                              /// 00044
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0004c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00050
			 0x00011111,                                                  // 9.7958E-41                                  /// 00054
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x4b000000,                                                  // 8388608.0                                   /// 00064
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0006c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00074
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00080
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0e000003,                                                  // Trailing 1s:                                /// 00088
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0008c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00094
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00100
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0010c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00110
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0011c
			 0x00000000,                                                  // zero                                        /// 00120
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00124
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00128
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0012c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00130
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00138
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00148
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00158
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00160
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00164
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00168
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00170
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00174
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00178
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0017c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00180
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00184
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00188
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00194
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00198
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001d8
			 0x33333333,                                                  // 4 random values                             /// 001dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00200
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00204
			 0x80011111,                                                  // -9.7958E-41                                 /// 00208
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00210
			 0x3f028f5c,                                                  // 0.51                                        /// 00214
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x0c600000,                                                  // Leading 1s:                                 /// 00224
			 0xcb000000,                                                  // -8388608.0                                  /// 00228
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0022c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00230
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00234
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0023c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00240
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00244
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x3f028f5c,                                                  // 0.51                                        /// 0024c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00250
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00254
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00258
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00260
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0026c
			 0xcb000000,                                                  // -8388608.0                                  /// 00270
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x80011111,                                                  // -9.7958E-41                                 /// 0027c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00280
			 0x0c700000,                                                  // Leading 1s:                                 /// 00284
			 0x4b000000,                                                  // 8388608.0                                   /// 00288
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00294
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x7fc00001,                                                  // signaling NaN                               /// 002b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002c4
			 0x7fc00001,                                                  // signaling NaN                               /// 002c8
			 0x7f800000,                                                  // inf                                         /// 002cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00300
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00304
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00308
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00310
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00318
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0031c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00320
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00324
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x4b000000,                                                  // 8388608.0                                   /// 0032c
			 0x80000000,                                                  // -zero                                       /// 00330
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00334
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0033c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00344
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0034c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00354
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00358
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0035c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00360
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00368
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00370
			 0x0c400000,                                                  // Leading 1s:                                 /// 00374
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00380
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00388
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0038c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00390
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00394
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x4b000000,                                                  // 8388608.0                                   /// 0040c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00414
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00418
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00424
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0042c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00438
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0043c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00440
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00448
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0044c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00454
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00458
			 0x0e000003,                                                  // Trailing 1s:                                /// 0045c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00468
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00474
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00480
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00484
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0048c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00494
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00498
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0049c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x80000000,                                                  // -zero                                       /// 004bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x7fc00001,                                                  // signaling NaN                               /// 004e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00508
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x3f028f5c,                                                  // 0.51                                        /// 00510
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00514
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0xbf028f5c,                                                  // -0.51                                       /// 0051c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00520
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0052c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0c780000,                                                  // Leading 1s:                                 /// 00538
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0053c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00548
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00550
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00554
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00558
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0055c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00560
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00568
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0056c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00570
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00574
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0057c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00580
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00588
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0058c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00590
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00594
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00598
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xff800000,                                                  // -inf                                        /// 005b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0xcb000000,                                                  // -8388608.0                                  /// 005fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0060c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00614
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00618
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0061c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00620
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00624
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0062c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00630
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0063c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00640
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00648
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0064c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00654
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00658
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00660
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00670
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00674
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00680
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00684
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00688
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00694
			 0x4b000000,                                                  // 8388608.0                                   /// 00698
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0069c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 006b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00700
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00708
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0070c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00714
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00718
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00724
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0072c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00730
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00734
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00740
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00750
			 0x0e000001,                                                  // Trailing 1s:                                /// 00754
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00760
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0076c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00770
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00774
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00778
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00780
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00784
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00788
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00790
			 0x3f028f5c,                                                  // 0.51                                        /// 00794
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00798
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0079c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007b0
			 0xffc00001,                                                  // -signaling NaN                              /// 007b4
			 0x3f028f5c,                                                  // 0.51                                        /// 007b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c4
			 0xff800000,                                                  // -inf                                        /// 007c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00804
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00808
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00814
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00818
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00820
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00824
			 0x7f800000,                                                  // inf                                         /// 00828
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00830
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00834
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0083c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00840
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00844
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00848
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0084c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00850
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00854
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0085c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00860
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00864
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0xbf028f5c,                                                  // -0.51                                       /// 0086c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00870
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00878
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0087c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00880
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00884
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00890
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00894
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0089c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 008ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 008f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00900
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00908
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0090c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00910
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00918
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0091c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00920
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00928
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00930
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00938
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00944
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0094c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00950
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00954
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0095c
			 0xff800000,                                                  // -inf                                        /// 00960
			 0x0e000003,                                                  // Trailing 1s:                                /// 00964
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00970
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00974
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00978
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0097c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00980
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00984
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00988
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00990
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00994
			 0x55555555,                                                  // 4 random values                             /// 00998
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0099c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x7f800000,                                                  // inf                                         /// 009a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x4b000000,                                                  // 8388608.0                                   /// 009c0
			 0x55555555,                                                  // 4 random values                             /// 009c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 009d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x55555555,                                                  // 4 random values                             /// 009fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a14
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a84
			 0x3f028f5c,                                                  // 0.51                                        /// 00a88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0xff800000,                                                  // -inf                                        /// 00aa0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aa4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aa8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ab0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ab4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ab8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ac0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ac8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00acc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ad0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ad4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ad8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00000000,                                                  // zero                                        /// 00ae0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00af0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00afc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b00
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b0c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b24
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b40
			 0xffc00001,                                                  // -signaling NaN                              /// 00b44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b70
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b84
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0xffc00001,                                                  // -signaling NaN                              /// 00b94
			 0x3f028f5c,                                                  // 0.51                                        /// 00b98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ba0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bb0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bb4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bb8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bbc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bc8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bd0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bdc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00be0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00be8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bf0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0xcb000000,                                                  // -8388608.0                                  /// 00bf8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bfc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c20
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c48
			 0x33333333,                                                  // 4 random values                             /// 00c4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c54
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c74
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c78
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c7c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ca0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cb0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cc0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cc4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ccc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cd0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ce0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ce8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cf4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cf8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d28
			 0xbf028f5c,                                                  // -0.51                                       /// 00d2c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d34
			 0xbf028f5c,                                                  // -0.51                                       /// 00d38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x7fc00001,                                                  // signaling NaN                               /// 00d54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d60
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d70
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00da4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00da8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00db8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dbc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dc0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0xff800000,                                                  // -inf                                        /// 00de0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00de4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00de8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00df4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dfc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00000000,                                                  // zero                                        /// 00e0c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e20
			 0x00000000,                                                  // zero                                        /// 00e24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e30
			 0x33333333,                                                  // 4 random values                             /// 00e34
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e48
			 0xcb000000,                                                  // -8388608.0                                  /// 00e4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e50
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e70
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e80
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ea8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eac
			 0x4b000000,                                                  // 8388608.0                                   /// 00eb0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00eb8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ebc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec8
			 0x3f028f5c,                                                  // 0.51                                        /// 00ecc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ed0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x33333333,                                                  // 4 random values                             /// 00edc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ee4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ef8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f00
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f08
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f30
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f34
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x00000000,                                                  // zero                                        /// 00f64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f78
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f7c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f80
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f8c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x7f800000,                                                  // inf                                         /// 00fa8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fc8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fcc
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fd4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fe0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ff4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ff8
			 0x7f800001 // SNaN                                          // SP +ve numbers                              /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x6d8917f0, /// 0x0
			 0xab6afb47, /// 0x4
			 0xa09cc2b0, /// 0x8
			 0xe0d38a09, /// 0xc
			 0x7d1c2a42, /// 0x10
			 0xb33ce1a8, /// 0x14
			 0xf9c883f0, /// 0x18
			 0xa23489a0, /// 0x1c
			 0x11f205b7, /// 0x20
			 0x8120b08a, /// 0x24
			 0xcd9a6e1f, /// 0x28
			 0x332b59c4, /// 0x2c
			 0xa6cec536, /// 0x30
			 0x10385d04, /// 0x34
			 0x2e4ef811, /// 0x38
			 0xf2d0a175, /// 0x3c
			 0x92e5119d, /// 0x40
			 0x6e3c7e66, /// 0x44
			 0x05b30847, /// 0x48
			 0x73dad221, /// 0x4c
			 0x14c64d0b, /// 0x50
			 0x2a246b8e, /// 0x54
			 0x518b035b, /// 0x58
			 0x2f2d1b22, /// 0x5c
			 0xe1890e29, /// 0x60
			 0xe9e7b6be, /// 0x64
			 0x4e17c0de, /// 0x68
			 0x9ce7d470, /// 0x6c
			 0x2e65e0ed, /// 0x70
			 0x839d65a1, /// 0x74
			 0x85e9f0ca, /// 0x78
			 0xd154ce52, /// 0x7c
			 0x3d9bd303, /// 0x80
			 0x53fede35, /// 0x84
			 0x7dca58af, /// 0x88
			 0x23f9eca1, /// 0x8c
			 0xcb2f0cec, /// 0x90
			 0x29fc45cc, /// 0x94
			 0xff761e9a, /// 0x98
			 0x57540bb2, /// 0x9c
			 0x38f63ea0, /// 0xa0
			 0x87737f41, /// 0xa4
			 0x4cc8afad, /// 0xa8
			 0x2b578ecb, /// 0xac
			 0xefe4d3ad, /// 0xb0
			 0x1043eae5, /// 0xb4
			 0x5b5f2cbf, /// 0xb8
			 0x3aab5aa8, /// 0xbc
			 0xda478d68, /// 0xc0
			 0x5506fea6, /// 0xc4
			 0x59635b61, /// 0xc8
			 0x8d107603, /// 0xcc
			 0xd8592b88, /// 0xd0
			 0xc35f3236, /// 0xd4
			 0x2c41d652, /// 0xd8
			 0xda0b97fb, /// 0xdc
			 0xcb7b8a43, /// 0xe0
			 0xba34c1f7, /// 0xe4
			 0x6ae8c277, /// 0xe8
			 0x9a2ef5c7, /// 0xec
			 0x67f1689a, /// 0xf0
			 0xff6ddd66, /// 0xf4
			 0x3a7cbd73, /// 0xf8
			 0x53fa202a, /// 0xfc
			 0xc815053b, /// 0x100
			 0x26019d3e, /// 0x104
			 0xaafe3c35, /// 0x108
			 0x3941334f, /// 0x10c
			 0xd82a4e28, /// 0x110
			 0xbc223f53, /// 0x114
			 0x43cb7de9, /// 0x118
			 0xb1c4a8c6, /// 0x11c
			 0xdc47a398, /// 0x120
			 0x36801f68, /// 0x124
			 0xbca2bdf7, /// 0x128
			 0x64bcc6b4, /// 0x12c
			 0x154c22de, /// 0x130
			 0x92d59229, /// 0x134
			 0x04fa5752, /// 0x138
			 0xa66b4fc4, /// 0x13c
			 0x2e041554, /// 0x140
			 0x1404a4fe, /// 0x144
			 0xb4d719bb, /// 0x148
			 0x3746eeac, /// 0x14c
			 0xf5694eba, /// 0x150
			 0x0ab51fd5, /// 0x154
			 0x0f512cd5, /// 0x158
			 0xe9d3a804, /// 0x15c
			 0x9cd8f9d2, /// 0x160
			 0x957cbf6a, /// 0x164
			 0xc3dfbcc2, /// 0x168
			 0xb0261082, /// 0x16c
			 0x150ba23d, /// 0x170
			 0xdebc3e72, /// 0x174
			 0x7833fe38, /// 0x178
			 0x46d0c44d, /// 0x17c
			 0xb381be80, /// 0x180
			 0xeae662f3, /// 0x184
			 0x125da137, /// 0x188
			 0x2c266016, /// 0x18c
			 0x4c29c57f, /// 0x190
			 0xfc27e35c, /// 0x194
			 0xbeb40b80, /// 0x198
			 0x758d6712, /// 0x19c
			 0xac259621, /// 0x1a0
			 0xe1012d98, /// 0x1a4
			 0x98e70a3d, /// 0x1a8
			 0xe49788f8, /// 0x1ac
			 0x293c5a57, /// 0x1b0
			 0x9ec60f41, /// 0x1b4
			 0xd6548c68, /// 0x1b8
			 0x58d1a8b9, /// 0x1bc
			 0x8286a926, /// 0x1c0
			 0xff3d1885, /// 0x1c4
			 0x6abecc91, /// 0x1c8
			 0x53d238d0, /// 0x1cc
			 0xae0986f8, /// 0x1d0
			 0xd0cc5829, /// 0x1d4
			 0x6760158b, /// 0x1d8
			 0x536ff93a, /// 0x1dc
			 0x7df40bdb, /// 0x1e0
			 0x4b196c37, /// 0x1e4
			 0xaa3ae826, /// 0x1e8
			 0xc32201cd, /// 0x1ec
			 0xa2b9831d, /// 0x1f0
			 0x18c44beb, /// 0x1f4
			 0x400c124b, /// 0x1f8
			 0xf0e70ff8, /// 0x1fc
			 0x2c11dcdd, /// 0x200
			 0xd56870d6, /// 0x204
			 0xbef4eb67, /// 0x208
			 0x322ff740, /// 0x20c
			 0x39b63f87, /// 0x210
			 0xd66f4685, /// 0x214
			 0xedfc7425, /// 0x218
			 0x2ace6997, /// 0x21c
			 0x6854d2bd, /// 0x220
			 0xb3fa12d5, /// 0x224
			 0xb7cad6ab, /// 0x228
			 0xabceea82, /// 0x22c
			 0x4b64c68f, /// 0x230
			 0x8b795530, /// 0x234
			 0x6917fc6c, /// 0x238
			 0x83c0c274, /// 0x23c
			 0xb7796b86, /// 0x240
			 0x9507190e, /// 0x244
			 0xec814fa6, /// 0x248
			 0xadf4c200, /// 0x24c
			 0x8f00c065, /// 0x250
			 0xb7dbd3c9, /// 0x254
			 0xb3300dd2, /// 0x258
			 0x692d270f, /// 0x25c
			 0xf9d61a78, /// 0x260
			 0x82dabb27, /// 0x264
			 0xa0504b21, /// 0x268
			 0xdfe84cc2, /// 0x26c
			 0xf11de0b6, /// 0x270
			 0x031440af, /// 0x274
			 0x493aa185, /// 0x278
			 0xac22d483, /// 0x27c
			 0xa8412329, /// 0x280
			 0x159532a9, /// 0x284
			 0x3a617aa6, /// 0x288
			 0x1954d698, /// 0x28c
			 0x72d16016, /// 0x290
			 0x7c3438a6, /// 0x294
			 0xc35c5e38, /// 0x298
			 0xda547e6c, /// 0x29c
			 0x749933f3, /// 0x2a0
			 0xc8d085c1, /// 0x2a4
			 0xab57482f, /// 0x2a8
			 0x3edccbf8, /// 0x2ac
			 0x690ad739, /// 0x2b0
			 0x91bc5561, /// 0x2b4
			 0x7e9ca6f1, /// 0x2b8
			 0x54a89bb6, /// 0x2bc
			 0xb290bfd1, /// 0x2c0
			 0x2eb55513, /// 0x2c4
			 0xb353fed2, /// 0x2c8
			 0x60221601, /// 0x2cc
			 0x5a3063e0, /// 0x2d0
			 0xccc29a30, /// 0x2d4
			 0x6ae47ae2, /// 0x2d8
			 0xaf7288e9, /// 0x2dc
			 0xe20b199b, /// 0x2e0
			 0xe36931a3, /// 0x2e4
			 0x20ce1c7e, /// 0x2e8
			 0x00eade14, /// 0x2ec
			 0xe1d78e13, /// 0x2f0
			 0xaa6c292b, /// 0x2f4
			 0x9cb0fdff, /// 0x2f8
			 0xb920886e, /// 0x2fc
			 0xfdedcf2b, /// 0x300
			 0x237c885f, /// 0x304
			 0xff630c53, /// 0x308
			 0x71eabd40, /// 0x30c
			 0x42775735, /// 0x310
			 0x667755c6, /// 0x314
			 0xe04e6343, /// 0x318
			 0x1fe0ca1e, /// 0x31c
			 0xd4b0f36a, /// 0x320
			 0x4a3c41ae, /// 0x324
			 0x34a1e027, /// 0x328
			 0xded924bd, /// 0x32c
			 0xa1d675a1, /// 0x330
			 0x6dbf922e, /// 0x334
			 0x4bb27428, /// 0x338
			 0xc6f1b742, /// 0x33c
			 0x65c1128a, /// 0x340
			 0x32cf6a6d, /// 0x344
			 0x2c4319f7, /// 0x348
			 0x29892725, /// 0x34c
			 0xd88eff3d, /// 0x350
			 0x14067cf7, /// 0x354
			 0xd0e38db1, /// 0x358
			 0xd169fe8f, /// 0x35c
			 0xb432e8ce, /// 0x360
			 0x19135c9d, /// 0x364
			 0x6fe072c2, /// 0x368
			 0xbcbd43c2, /// 0x36c
			 0xc526d351, /// 0x370
			 0x47dafecd, /// 0x374
			 0x55194156, /// 0x378
			 0xfcd51660, /// 0x37c
			 0x49af55e3, /// 0x380
			 0x9c220bb9, /// 0x384
			 0x6421b578, /// 0x388
			 0x90895454, /// 0x38c
			 0x0a187889, /// 0x390
			 0xbb39f516, /// 0x394
			 0x6cbd55e3, /// 0x398
			 0xb8b17f2e, /// 0x39c
			 0x4f21b015, /// 0x3a0
			 0xc6e46dc8, /// 0x3a4
			 0xe601aeb1, /// 0x3a8
			 0xbc3ab05e, /// 0x3ac
			 0xb72af247, /// 0x3b0
			 0x612cc0dc, /// 0x3b4
			 0x37b70622, /// 0x3b8
			 0x1f411a85, /// 0x3bc
			 0x2b62d144, /// 0x3c0
			 0x41ad2644, /// 0x3c4
			 0x2df344f0, /// 0x3c8
			 0x40e38323, /// 0x3cc
			 0x747b5b5a, /// 0x3d0
			 0xf6eab36c, /// 0x3d4
			 0xc0d1a524, /// 0x3d8
			 0x5a81b50d, /// 0x3dc
			 0x15b56bc5, /// 0x3e0
			 0xca4dfc5b, /// 0x3e4
			 0x4db5283d, /// 0x3e8
			 0x5dd83fd8, /// 0x3ec
			 0x2d9e600c, /// 0x3f0
			 0xe05cdaf1, /// 0x3f4
			 0xcf931312, /// 0x3f8
			 0xaa75b09f, /// 0x3fc
			 0x8211f4a6, /// 0x400
			 0xf58ef5fe, /// 0x404
			 0xe5ed17e8, /// 0x408
			 0x3e7d0067, /// 0x40c
			 0x70956fb2, /// 0x410
			 0x904528a0, /// 0x414
			 0x45e9a06f, /// 0x418
			 0xcae2f4ef, /// 0x41c
			 0x0eff7917, /// 0x420
			 0xb42d6fff, /// 0x424
			 0x54dbf127, /// 0x428
			 0x477613db, /// 0x42c
			 0x2e8e7c70, /// 0x430
			 0xb4a2f814, /// 0x434
			 0xc2a4ccb5, /// 0x438
			 0x53c0fd9e, /// 0x43c
			 0x47ee25fa, /// 0x440
			 0xb87b34d4, /// 0x444
			 0x6e7b6c03, /// 0x448
			 0x71346e7d, /// 0x44c
			 0x70c4595f, /// 0x450
			 0xa697e311, /// 0x454
			 0xb3ddb2ea, /// 0x458
			 0xbf8f5ecc, /// 0x45c
			 0xf502f452, /// 0x460
			 0xa46ea582, /// 0x464
			 0x9d94bf4c, /// 0x468
			 0x9265d9f9, /// 0x46c
			 0x72d0736b, /// 0x470
			 0x038c94f7, /// 0x474
			 0x68dc5e72, /// 0x478
			 0x9530aa11, /// 0x47c
			 0x263d8827, /// 0x480
			 0xdbc81888, /// 0x484
			 0xd3bf6d6b, /// 0x488
			 0x219b0aad, /// 0x48c
			 0x937b9476, /// 0x490
			 0x33299db3, /// 0x494
			 0x8a1cba0c, /// 0x498
			 0x14ffb67f, /// 0x49c
			 0x91951d37, /// 0x4a0
			 0xd81d4fc5, /// 0x4a4
			 0x9b504435, /// 0x4a8
			 0xbe61bf5c, /// 0x4ac
			 0xdad1a778, /// 0x4b0
			 0x1cca46ad, /// 0x4b4
			 0x03194dca, /// 0x4b8
			 0x6d10417e, /// 0x4bc
			 0x0f9ccaa0, /// 0x4c0
			 0x7eb96d2b, /// 0x4c4
			 0x5546a98f, /// 0x4c8
			 0xe13de53b, /// 0x4cc
			 0x9273003e, /// 0x4d0
			 0x98ff930f, /// 0x4d4
			 0x23c86f47, /// 0x4d8
			 0xe8ffade8, /// 0x4dc
			 0x18902e36, /// 0x4e0
			 0x43811244, /// 0x4e4
			 0x23147c2d, /// 0x4e8
			 0x83555082, /// 0x4ec
			 0xd20a0e28, /// 0x4f0
			 0xcdada01b, /// 0x4f4
			 0xbe103259, /// 0x4f8
			 0xeff17d2b, /// 0x4fc
			 0x1306a8f8, /// 0x500
			 0x4c1af0b7, /// 0x504
			 0x635e2528, /// 0x508
			 0xcbe32fe7, /// 0x50c
			 0x3e4cc39b, /// 0x510
			 0x4e2906c3, /// 0x514
			 0x869da511, /// 0x518
			 0x28c1f79f, /// 0x51c
			 0xcde196f9, /// 0x520
			 0xfdf139cd, /// 0x524
			 0x94d17743, /// 0x528
			 0x2234c9f6, /// 0x52c
			 0x9f259a8f, /// 0x530
			 0xf1cabcc2, /// 0x534
			 0xc2c047f3, /// 0x538
			 0x2da845eb, /// 0x53c
			 0x2f279f46, /// 0x540
			 0x0b9cc00b, /// 0x544
			 0x39e02711, /// 0x548
			 0x29a31280, /// 0x54c
			 0x5fdeecc4, /// 0x550
			 0x6e479387, /// 0x554
			 0x72517762, /// 0x558
			 0x5350de81, /// 0x55c
			 0x6c2cf712, /// 0x560
			 0xbda89397, /// 0x564
			 0xd7e31f43, /// 0x568
			 0x783c5848, /// 0x56c
			 0x9222cea6, /// 0x570
			 0x95a2e6d1, /// 0x574
			 0xd35f8338, /// 0x578
			 0x7caf2948, /// 0x57c
			 0xf065bdce, /// 0x580
			 0xd73f57e1, /// 0x584
			 0xc05e3a5c, /// 0x588
			 0x9524db64, /// 0x58c
			 0xfab17674, /// 0x590
			 0xf721ab40, /// 0x594
			 0xd4ca2d3e, /// 0x598
			 0xaed82310, /// 0x59c
			 0xad6ddd40, /// 0x5a0
			 0x0eff2dbe, /// 0x5a4
			 0x9d87efd4, /// 0x5a8
			 0x18d1d197, /// 0x5ac
			 0x8c6ba6cd, /// 0x5b0
			 0x24184d95, /// 0x5b4
			 0xa48f9159, /// 0x5b8
			 0x64074b8b, /// 0x5bc
			 0x4679133a, /// 0x5c0
			 0xa490b7fe, /// 0x5c4
			 0x7b46114f, /// 0x5c8
			 0x49ed15c5, /// 0x5cc
			 0x718f490d, /// 0x5d0
			 0x4249641d, /// 0x5d4
			 0xb09b7d6f, /// 0x5d8
			 0xe1e7c91f, /// 0x5dc
			 0xfe94a205, /// 0x5e0
			 0x4271f632, /// 0x5e4
			 0x3031aa10, /// 0x5e8
			 0xd6014ab6, /// 0x5ec
			 0xf40081f7, /// 0x5f0
			 0xb0244f29, /// 0x5f4
			 0x13a998bb, /// 0x5f8
			 0x9436554e, /// 0x5fc
			 0x777b4b58, /// 0x600
			 0xea647171, /// 0x604
			 0x3e355405, /// 0x608
			 0x450caf5a, /// 0x60c
			 0xe212e6fb, /// 0x610
			 0x0bee270a, /// 0x614
			 0xcce82e2b, /// 0x618
			 0x0e7595c1, /// 0x61c
			 0xfa8609d0, /// 0x620
			 0x34d90682, /// 0x624
			 0xef6ff9e5, /// 0x628
			 0x92a21fa7, /// 0x62c
			 0x586afa34, /// 0x630
			 0x2e382556, /// 0x634
			 0x4bfaebd0, /// 0x638
			 0x73a67287, /// 0x63c
			 0x2977c4bc, /// 0x640
			 0x2947480b, /// 0x644
			 0xfed43a18, /// 0x648
			 0x5eb133c8, /// 0x64c
			 0x9691714c, /// 0x650
			 0x830c6a8e, /// 0x654
			 0x194dfc5e, /// 0x658
			 0xe24461f5, /// 0x65c
			 0x53d2018d, /// 0x660
			 0xdbdfa0d7, /// 0x664
			 0xe6df5c23, /// 0x668
			 0xaf8dd2ce, /// 0x66c
			 0x8e04511a, /// 0x670
			 0xa53296af, /// 0x674
			 0xc4353e8b, /// 0x678
			 0x3e67bdec, /// 0x67c
			 0x4b604e0b, /// 0x680
			 0xf6997261, /// 0x684
			 0xd43cfe63, /// 0x688
			 0x5d455bf9, /// 0x68c
			 0xad1aab85, /// 0x690
			 0xf8306aa9, /// 0x694
			 0xd67c16a3, /// 0x698
			 0x1de066f8, /// 0x69c
			 0x3853ee77, /// 0x6a0
			 0xf983b802, /// 0x6a4
			 0xf848fc09, /// 0x6a8
			 0x886760bc, /// 0x6ac
			 0xc8dc741f, /// 0x6b0
			 0xf8ffea61, /// 0x6b4
			 0x2c2cd691, /// 0x6b8
			 0xd0b56f43, /// 0x6bc
			 0x4d5800a3, /// 0x6c0
			 0xceb27411, /// 0x6c4
			 0xc2bb4a75, /// 0x6c8
			 0x5500b2cc, /// 0x6cc
			 0xde13814e, /// 0x6d0
			 0xdb1e87fa, /// 0x6d4
			 0xc2a8994b, /// 0x6d8
			 0x7d690dcd, /// 0x6dc
			 0xe63df809, /// 0x6e0
			 0x73c8ba05, /// 0x6e4
			 0xe1cd756e, /// 0x6e8
			 0xaec05936, /// 0x6ec
			 0x8332e79d, /// 0x6f0
			 0x5e07091f, /// 0x6f4
			 0x1ae81a0e, /// 0x6f8
			 0xa6038e58, /// 0x6fc
			 0xc79ba9e4, /// 0x700
			 0x70295e28, /// 0x704
			 0x24a2ce0c, /// 0x708
			 0xe6a12268, /// 0x70c
			 0x89375023, /// 0x710
			 0xb1d22fbe, /// 0x714
			 0x38269846, /// 0x718
			 0xedb4a90f, /// 0x71c
			 0x58233ebe, /// 0x720
			 0xf1e80495, /// 0x724
			 0xf45fd866, /// 0x728
			 0xec1d7a87, /// 0x72c
			 0x166f14c9, /// 0x730
			 0xd9d8a598, /// 0x734
			 0x37415d33, /// 0x738
			 0xe13ba62b, /// 0x73c
			 0x230eedab, /// 0x740
			 0xda7046f0, /// 0x744
			 0xdbe6c1bd, /// 0x748
			 0x7f653178, /// 0x74c
			 0x20810087, /// 0x750
			 0xaa5ee514, /// 0x754
			 0xd012fa97, /// 0x758
			 0xe594efe9, /// 0x75c
			 0xdb74b11f, /// 0x760
			 0x7fc82f44, /// 0x764
			 0xb3725f60, /// 0x768
			 0xbcac1dc2, /// 0x76c
			 0xb615e811, /// 0x770
			 0x599eaeea, /// 0x774
			 0xb50c6580, /// 0x778
			 0x2cea6aaa, /// 0x77c
			 0x85f6d6d6, /// 0x780
			 0x674d692a, /// 0x784
			 0x1860dd58, /// 0x788
			 0x874e5ec8, /// 0x78c
			 0x459dfee4, /// 0x790
			 0x0e5490b2, /// 0x794
			 0x7fd85b00, /// 0x798
			 0x3a151f9c, /// 0x79c
			 0x406b39ff, /// 0x7a0
			 0xaadb760e, /// 0x7a4
			 0xa40b2c7a, /// 0x7a8
			 0xf473b80f, /// 0x7ac
			 0x53d71027, /// 0x7b0
			 0xf4ce3d12, /// 0x7b4
			 0xa6bd2311, /// 0x7b8
			 0x8ca6f190, /// 0x7bc
			 0x5f3c2fc4, /// 0x7c0
			 0x01c24d75, /// 0x7c4
			 0x0c92c714, /// 0x7c8
			 0x32ca45a9, /// 0x7cc
			 0xa7341a9d, /// 0x7d0
			 0x96f0ca92, /// 0x7d4
			 0xdba09dc9, /// 0x7d8
			 0xadcef23e, /// 0x7dc
			 0xb5778766, /// 0x7e0
			 0x408566e5, /// 0x7e4
			 0x0b91c284, /// 0x7e8
			 0xf6146818, /// 0x7ec
			 0x4464ad62, /// 0x7f0
			 0x465b9623, /// 0x7f4
			 0x49a7b52b, /// 0x7f8
			 0xa5a70f03, /// 0x7fc
			 0xb50cf80f, /// 0x800
			 0x9c4b88c3, /// 0x804
			 0xc4193a80, /// 0x808
			 0xeb378aa7, /// 0x80c
			 0x71816a6b, /// 0x810
			 0x45bf7631, /// 0x814
			 0x76aa8722, /// 0x818
			 0x575265f2, /// 0x81c
			 0xc98337f4, /// 0x820
			 0x85065a19, /// 0x824
			 0xe37d4d60, /// 0x828
			 0x9ce9dd17, /// 0x82c
			 0x0983c42a, /// 0x830
			 0x7d2ec9a9, /// 0x834
			 0x8d811c0f, /// 0x838
			 0x0ebe2f7d, /// 0x83c
			 0x3943ed94, /// 0x840
			 0x8e99eba4, /// 0x844
			 0xcc987e2b, /// 0x848
			 0x4d06f9f7, /// 0x84c
			 0x59c649ab, /// 0x850
			 0x1a7fa281, /// 0x854
			 0x720b0e39, /// 0x858
			 0xcce1364e, /// 0x85c
			 0xbe93cd76, /// 0x860
			 0x9a6c605e, /// 0x864
			 0x9d0b7760, /// 0x868
			 0xbe1a3e07, /// 0x86c
			 0xab0a3cbe, /// 0x870
			 0x7bcade3b, /// 0x874
			 0x5c528239, /// 0x878
			 0x8d14eac5, /// 0x87c
			 0x2fbd6aa7, /// 0x880
			 0x552717ff, /// 0x884
			 0xd005f4a0, /// 0x888
			 0x9dddb633, /// 0x88c
			 0x5324e9b4, /// 0x890
			 0xb0f34a70, /// 0x894
			 0x6959ec0c, /// 0x898
			 0xe0d9821b, /// 0x89c
			 0xf4f4b8d8, /// 0x8a0
			 0x394598b5, /// 0x8a4
			 0xc4d53b5e, /// 0x8a8
			 0x0c5e2821, /// 0x8ac
			 0xc2a65ce5, /// 0x8b0
			 0x97b84888, /// 0x8b4
			 0x6e06aac4, /// 0x8b8
			 0x47d36a4a, /// 0x8bc
			 0x7e766043, /// 0x8c0
			 0x8e54b48e, /// 0x8c4
			 0x851513dd, /// 0x8c8
			 0xb9d9b4ee, /// 0x8cc
			 0xf22307af, /// 0x8d0
			 0x7c0063c1, /// 0x8d4
			 0x264d9339, /// 0x8d8
			 0x50f494ab, /// 0x8dc
			 0x6f18b5e8, /// 0x8e0
			 0x26cb526e, /// 0x8e4
			 0x66948bbf, /// 0x8e8
			 0x29b85413, /// 0x8ec
			 0x2b4bf632, /// 0x8f0
			 0x140a2668, /// 0x8f4
			 0x2634c29a, /// 0x8f8
			 0x5f9ad373, /// 0x8fc
			 0x8eb69c07, /// 0x900
			 0xef50cdb8, /// 0x904
			 0x82d1ecdb, /// 0x908
			 0x95ba1a0a, /// 0x90c
			 0xb60d9f83, /// 0x910
			 0xe41f376a, /// 0x914
			 0x059d65a0, /// 0x918
			 0xa32a64eb, /// 0x91c
			 0xaa90658b, /// 0x920
			 0xeaf7ca2e, /// 0x924
			 0xdd32ea24, /// 0x928
			 0x15fd7e74, /// 0x92c
			 0x6b80f843, /// 0x930
			 0x8975a057, /// 0x934
			 0x3d625907, /// 0x938
			 0x0315816e, /// 0x93c
			 0xadd0a7db, /// 0x940
			 0xb53dea1f, /// 0x944
			 0xa197cfd6, /// 0x948
			 0xc02f67be, /// 0x94c
			 0x26060d8a, /// 0x950
			 0x28f76f94, /// 0x954
			 0x30917e29, /// 0x958
			 0xaf3892df, /// 0x95c
			 0x20809d6d, /// 0x960
			 0x471a30b5, /// 0x964
			 0x331044dd, /// 0x968
			 0x4cbd7d2a, /// 0x96c
			 0x0c57d801, /// 0x970
			 0x449bceab, /// 0x974
			 0x9ec007dc, /// 0x978
			 0x9004ecda, /// 0x97c
			 0xf5f04124, /// 0x980
			 0xf83aae24, /// 0x984
			 0xd84a2855, /// 0x988
			 0x16e44259, /// 0x98c
			 0xa60c8b3a, /// 0x990
			 0xcc470ace, /// 0x994
			 0xf0e7c16a, /// 0x998
			 0xce8bdf57, /// 0x99c
			 0xc41ef4af, /// 0x9a0
			 0xc1086459, /// 0x9a4
			 0x86aac2cf, /// 0x9a8
			 0x5131e565, /// 0x9ac
			 0x3d34a4a2, /// 0x9b0
			 0xd9feec31, /// 0x9b4
			 0xa84752c3, /// 0x9b8
			 0xb1352e9a, /// 0x9bc
			 0xef0809c3, /// 0x9c0
			 0xa1ae3f16, /// 0x9c4
			 0x744eb4b5, /// 0x9c8
			 0xdd66deb6, /// 0x9cc
			 0xc4616f80, /// 0x9d0
			 0x4283b148, /// 0x9d4
			 0x4c2ec1dc, /// 0x9d8
			 0x983a3cb4, /// 0x9dc
			 0x955d43cf, /// 0x9e0
			 0x5750a108, /// 0x9e4
			 0x3ae14111, /// 0x9e8
			 0x548f6dc7, /// 0x9ec
			 0xd1aa9d84, /// 0x9f0
			 0xc4ec5bfb, /// 0x9f4
			 0x8285d726, /// 0x9f8
			 0x645adb51, /// 0x9fc
			 0x6696f41a, /// 0xa00
			 0x272d3117, /// 0xa04
			 0x0c1a56d7, /// 0xa08
			 0xae52e6ab, /// 0xa0c
			 0x25b45efc, /// 0xa10
			 0xee57264c, /// 0xa14
			 0xa96026b0, /// 0xa18
			 0xcc69957d, /// 0xa1c
			 0x503f004c, /// 0xa20
			 0xc828d2b3, /// 0xa24
			 0xb60097eb, /// 0xa28
			 0x241f2e87, /// 0xa2c
			 0xd4fd67c9, /// 0xa30
			 0xba9ad987, /// 0xa34
			 0x70220ab7, /// 0xa38
			 0xeb4c3856, /// 0xa3c
			 0xe3f56db2, /// 0xa40
			 0xafbe51f9, /// 0xa44
			 0x25aa8828, /// 0xa48
			 0x27747f51, /// 0xa4c
			 0x9f24f4b9, /// 0xa50
			 0x3027c2fa, /// 0xa54
			 0x889035ea, /// 0xa58
			 0x564c0119, /// 0xa5c
			 0xb6202ce5, /// 0xa60
			 0x4a3e213a, /// 0xa64
			 0x712f1c38, /// 0xa68
			 0xe11f475b, /// 0xa6c
			 0x9e34413d, /// 0xa70
			 0x7ac95816, /// 0xa74
			 0x0f4c0e98, /// 0xa78
			 0x5a3e107f, /// 0xa7c
			 0x58d66a25, /// 0xa80
			 0xee8a0484, /// 0xa84
			 0x9a3cf055, /// 0xa88
			 0x38985797, /// 0xa8c
			 0x589fd8f7, /// 0xa90
			 0x563435bd, /// 0xa94
			 0x2f3a14b3, /// 0xa98
			 0x1e97bee6, /// 0xa9c
			 0x2e8f5c1f, /// 0xaa0
			 0x488ddcc1, /// 0xaa4
			 0x43fa2cb1, /// 0xaa8
			 0xf7fd6b75, /// 0xaac
			 0x6f542d22, /// 0xab0
			 0xaa28d6f2, /// 0xab4
			 0xf3743855, /// 0xab8
			 0x30cfdb5f, /// 0xabc
			 0x0b6ee13f, /// 0xac0
			 0x636f115e, /// 0xac4
			 0xd3741ab1, /// 0xac8
			 0x9d16ebe2, /// 0xacc
			 0xe08bd85e, /// 0xad0
			 0xd56b4ebc, /// 0xad4
			 0x648709c2, /// 0xad8
			 0xb7545e3e, /// 0xadc
			 0xa38c0ff1, /// 0xae0
			 0x84e6005c, /// 0xae4
			 0x841c1fcd, /// 0xae8
			 0x85324cb9, /// 0xaec
			 0x6fbdcd1c, /// 0xaf0
			 0x4800254f, /// 0xaf4
			 0x6fa26931, /// 0xaf8
			 0xb62bd6a9, /// 0xafc
			 0x42595272, /// 0xb00
			 0x4dc50c42, /// 0xb04
			 0x3bff033c, /// 0xb08
			 0x7ee1d0fe, /// 0xb0c
			 0xe8a48569, /// 0xb10
			 0x47ad6709, /// 0xb14
			 0x9bfbe0cc, /// 0xb18
			 0xa4d49690, /// 0xb1c
			 0x01fc3f84, /// 0xb20
			 0xb9169ba8, /// 0xb24
			 0x9a9e09ce, /// 0xb28
			 0x3abd53e6, /// 0xb2c
			 0x931fe350, /// 0xb30
			 0xf132c6de, /// 0xb34
			 0x37b218a5, /// 0xb38
			 0xf844430a, /// 0xb3c
			 0x2217895f, /// 0xb40
			 0xb0005d96, /// 0xb44
			 0x284e068e, /// 0xb48
			 0x789f6124, /// 0xb4c
			 0xf2a4b242, /// 0xb50
			 0xe2c79057, /// 0xb54
			 0x0dc57c3d, /// 0xb58
			 0xcbe6e9e7, /// 0xb5c
			 0x78bdaac8, /// 0xb60
			 0x1f056869, /// 0xb64
			 0xb565cb92, /// 0xb68
			 0xbc063aa8, /// 0xb6c
			 0x66a2f1a1, /// 0xb70
			 0x170cb9a8, /// 0xb74
			 0xc7dc2530, /// 0xb78
			 0x120dabcb, /// 0xb7c
			 0x67e812ae, /// 0xb80
			 0x23417207, /// 0xb84
			 0xc33f7dfe, /// 0xb88
			 0x23e39902, /// 0xb8c
			 0x08b8e35f, /// 0xb90
			 0xdb075e7f, /// 0xb94
			 0x1cfd37f9, /// 0xb98
			 0x0137b67d, /// 0xb9c
			 0xcfd09a45, /// 0xba0
			 0x918d154d, /// 0xba4
			 0x6eee361e, /// 0xba8
			 0xfd62f2de, /// 0xbac
			 0xbc760b8f, /// 0xbb0
			 0xf878b218, /// 0xbb4
			 0x936d9476, /// 0xbb8
			 0x6aec2132, /// 0xbbc
			 0x09ee4dfc, /// 0xbc0
			 0x947d601b, /// 0xbc4
			 0x195a7f59, /// 0xbc8
			 0xbc15ec47, /// 0xbcc
			 0xe37f1bc9, /// 0xbd0
			 0x9903885d, /// 0xbd4
			 0xd00b4765, /// 0xbd8
			 0xe1fc12d9, /// 0xbdc
			 0xe48aec60, /// 0xbe0
			 0xab197f77, /// 0xbe4
			 0x8777da40, /// 0xbe8
			 0x6d48c78e, /// 0xbec
			 0x01dd77b3, /// 0xbf0
			 0xbc1cad5c, /// 0xbf4
			 0x26ad099c, /// 0xbf8
			 0xa2379c3e, /// 0xbfc
			 0x2bd782e8, /// 0xc00
			 0x0e5e6788, /// 0xc04
			 0xe854051f, /// 0xc08
			 0x82dae830, /// 0xc0c
			 0x473ceb77, /// 0xc10
			 0x43cfc5c5, /// 0xc14
			 0x4a032248, /// 0xc18
			 0x47787b62, /// 0xc1c
			 0x40731aba, /// 0xc20
			 0xf4125c46, /// 0xc24
			 0x928535cc, /// 0xc28
			 0xb79a39c3, /// 0xc2c
			 0xaf68aa74, /// 0xc30
			 0xb40834bd, /// 0xc34
			 0xcaa9404c, /// 0xc38
			 0xae289b50, /// 0xc3c
			 0xd756ad31, /// 0xc40
			 0xf27b07f3, /// 0xc44
			 0x70e369c8, /// 0xc48
			 0xb99aacbc, /// 0xc4c
			 0xd511d416, /// 0xc50
			 0x65db2dc8, /// 0xc54
			 0x3c83ad01, /// 0xc58
			 0xa12ee73c, /// 0xc5c
			 0xf18ae120, /// 0xc60
			 0x8b6fbf0b, /// 0xc64
			 0xb1bdca84, /// 0xc68
			 0xe21199c6, /// 0xc6c
			 0x9eebe10c, /// 0xc70
			 0x3df1fe2a, /// 0xc74
			 0x952aa05f, /// 0xc78
			 0x60e4d41e, /// 0xc7c
			 0xc35a0d97, /// 0xc80
			 0x0e5e65dd, /// 0xc84
			 0xf71ecf4e, /// 0xc88
			 0xe87a241f, /// 0xc8c
			 0x31352f36, /// 0xc90
			 0x45d85435, /// 0xc94
			 0x46306628, /// 0xc98
			 0xa1910e41, /// 0xc9c
			 0x5ebad739, /// 0xca0
			 0xcf96140d, /// 0xca4
			 0x68c3f6b4, /// 0xca8
			 0x44b6b623, /// 0xcac
			 0xf2cc8bb7, /// 0xcb0
			 0xdc6888d2, /// 0xcb4
			 0x7d434dfb, /// 0xcb8
			 0xff184364, /// 0xcbc
			 0x441299b0, /// 0xcc0
			 0x43354e57, /// 0xcc4
			 0x63d24808, /// 0xcc8
			 0x54c2dd16, /// 0xccc
			 0x91bec8b4, /// 0xcd0
			 0xe8c3f215, /// 0xcd4
			 0xab5d7aca, /// 0xcd8
			 0x05c973d6, /// 0xcdc
			 0x7c29f03b, /// 0xce0
			 0x4949f7c4, /// 0xce4
			 0x3c0e83ad, /// 0xce8
			 0x95dda005, /// 0xcec
			 0x9dc8c47a, /// 0xcf0
			 0xb51a6d9b, /// 0xcf4
			 0x339a573c, /// 0xcf8
			 0x97236db3, /// 0xcfc
			 0xfc0163eb, /// 0xd00
			 0x9bf96d73, /// 0xd04
			 0x87d3e0fc, /// 0xd08
			 0x9f0ac75b, /// 0xd0c
			 0xe005ce16, /// 0xd10
			 0xddf67397, /// 0xd14
			 0x8e477475, /// 0xd18
			 0x5da3e0f9, /// 0xd1c
			 0x1d541e35, /// 0xd20
			 0x10e7faa3, /// 0xd24
			 0xcec92874, /// 0xd28
			 0x08af45d5, /// 0xd2c
			 0xe697ea71, /// 0xd30
			 0x659ff40d, /// 0xd34
			 0xbf845a1c, /// 0xd38
			 0xc0cc89e8, /// 0xd3c
			 0x0c748337, /// 0xd40
			 0x85d1ec3e, /// 0xd44
			 0x0e31bf40, /// 0xd48
			 0x82d20610, /// 0xd4c
			 0x2013fe47, /// 0xd50
			 0x54a62236, /// 0xd54
			 0x6b238585, /// 0xd58
			 0xeefb2548, /// 0xd5c
			 0xd9e7bcb9, /// 0xd60
			 0x835a9595, /// 0xd64
			 0xafc9beda, /// 0xd68
			 0x52efde9f, /// 0xd6c
			 0x053490d4, /// 0xd70
			 0xf61dbe09, /// 0xd74
			 0x58193099, /// 0xd78
			 0x30e0b703, /// 0xd7c
			 0xddb0ace1, /// 0xd80
			 0x28d4425d, /// 0xd84
			 0x9bc67ab7, /// 0xd88
			 0xfebcfd0e, /// 0xd8c
			 0xddc2af37, /// 0xd90
			 0xae32ca6c, /// 0xd94
			 0xb0c006cb, /// 0xd98
			 0xd9274f14, /// 0xd9c
			 0xf008e1fb, /// 0xda0
			 0x085b1242, /// 0xda4
			 0x73f379d0, /// 0xda8
			 0x6024dcbd, /// 0xdac
			 0x98e8a83a, /// 0xdb0
			 0xacc07469, /// 0xdb4
			 0x0a4f1473, /// 0xdb8
			 0x87b0ec01, /// 0xdbc
			 0x150b557c, /// 0xdc0
			 0xa702fd0b, /// 0xdc4
			 0xa4d59a6d, /// 0xdc8
			 0xbcab4377, /// 0xdcc
			 0x99bf272b, /// 0xdd0
			 0xc9342d9b, /// 0xdd4
			 0x62741d34, /// 0xdd8
			 0x76495ee6, /// 0xddc
			 0xf2901ff2, /// 0xde0
			 0xf48c47fb, /// 0xde4
			 0x29f29c2c, /// 0xde8
			 0xfb6490ff, /// 0xdec
			 0x9cc11d89, /// 0xdf0
			 0x40f4c6ce, /// 0xdf4
			 0xd685ac57, /// 0xdf8
			 0xa5bf291e, /// 0xdfc
			 0x41be6a76, /// 0xe00
			 0x26389939, /// 0xe04
			 0x91aaf1b7, /// 0xe08
			 0xf22abbe5, /// 0xe0c
			 0x7b9755d6, /// 0xe10
			 0xac3aa208, /// 0xe14
			 0x55c6fca6, /// 0xe18
			 0x269daed4, /// 0xe1c
			 0x4bcc2de2, /// 0xe20
			 0x24e580ac, /// 0xe24
			 0x59717b88, /// 0xe28
			 0xc9233353, /// 0xe2c
			 0x51a2fa63, /// 0xe30
			 0xe5c37921, /// 0xe34
			 0x59319c0c, /// 0xe38
			 0xf377bbb7, /// 0xe3c
			 0xea9943ba, /// 0xe40
			 0xb6f567c4, /// 0xe44
			 0xba488ea1, /// 0xe48
			 0xe6655e81, /// 0xe4c
			 0x6141b524, /// 0xe50
			 0x991725f3, /// 0xe54
			 0x01682c33, /// 0xe58
			 0xc09d756a, /// 0xe5c
			 0x5caa8ebb, /// 0xe60
			 0x6d8cf75f, /// 0xe64
			 0x072aebc6, /// 0xe68
			 0x57bbdb62, /// 0xe6c
			 0xadd70868, /// 0xe70
			 0xc8930c15, /// 0xe74
			 0x3f8b1bbf, /// 0xe78
			 0xf63ec9aa, /// 0xe7c
			 0x46a7877a, /// 0xe80
			 0xa667f3be, /// 0xe84
			 0x0e5585b7, /// 0xe88
			 0x55618d0e, /// 0xe8c
			 0x556f09af, /// 0xe90
			 0xae9a7347, /// 0xe94
			 0x6516688e, /// 0xe98
			 0xe8699186, /// 0xe9c
			 0xd76ec28b, /// 0xea0
			 0xef2a3483, /// 0xea4
			 0x99889253, /// 0xea8
			 0xf514d295, /// 0xeac
			 0x46e4572e, /// 0xeb0
			 0x35176cce, /// 0xeb4
			 0x28153ad7, /// 0xeb8
			 0x58513212, /// 0xebc
			 0x0fd538c8, /// 0xec0
			 0x0d1daeae, /// 0xec4
			 0xb4988654, /// 0xec8
			 0xb19b2a2b, /// 0xecc
			 0x3c605952, /// 0xed0
			 0x735ba27b, /// 0xed4
			 0xccdffa66, /// 0xed8
			 0xbe805805, /// 0xedc
			 0x05e5397d, /// 0xee0
			 0x22dd9e83, /// 0xee4
			 0x3c8bb47a, /// 0xee8
			 0x33f58ee9, /// 0xeec
			 0x6d6430ac, /// 0xef0
			 0x340e0bc2, /// 0xef4
			 0x5b2d7a5d, /// 0xef8
			 0xa3b3fa19, /// 0xefc
			 0xf509d389, /// 0xf00
			 0x9300127c, /// 0xf04
			 0xf64988f9, /// 0xf08
			 0x4a2ea304, /// 0xf0c
			 0xf69fd5db, /// 0xf10
			 0x148d745b, /// 0xf14
			 0x3887e9bf, /// 0xf18
			 0x797ece03, /// 0xf1c
			 0x33762b22, /// 0xf20
			 0x5940d9fb, /// 0xf24
			 0x255a50b9, /// 0xf28
			 0x125aeb7c, /// 0xf2c
			 0x6c065a00, /// 0xf30
			 0x64ca8cbc, /// 0xf34
			 0xa172a7c5, /// 0xf38
			 0xab534f41, /// 0xf3c
			 0x090f8c13, /// 0xf40
			 0x7183f11a, /// 0xf44
			 0xeb72009f, /// 0xf48
			 0x5bda4a85, /// 0xf4c
			 0xfa5dec13, /// 0xf50
			 0x7b23a558, /// 0xf54
			 0x42282bbb, /// 0xf58
			 0x3470ad8f, /// 0xf5c
			 0x6c03292f, /// 0xf60
			 0xab3786b8, /// 0xf64
			 0xcb1278e7, /// 0xf68
			 0x38ecb232, /// 0xf6c
			 0xa0cea3d7, /// 0xf70
			 0x1290b23f, /// 0xf74
			 0xc956eae3, /// 0xf78
			 0x6bca1969, /// 0xf7c
			 0x1212c1f6, /// 0xf80
			 0xa16b093c, /// 0xf84
			 0xfeb36c08, /// 0xf88
			 0x416e5edc, /// 0xf8c
			 0xc291d366, /// 0xf90
			 0xa53e56eb, /// 0xf94
			 0x6a743009, /// 0xf98
			 0x4add502e, /// 0xf9c
			 0xaa2314c0, /// 0xfa0
			 0xf9b1efc6, /// 0xfa4
			 0xa93928a7, /// 0xfa8
			 0x516d12f7, /// 0xfac
			 0xd9e32e35, /// 0xfb0
			 0x6415d303, /// 0xfb4
			 0xb3e1d61d, /// 0xfb8
			 0xcec5fbef, /// 0xfbc
			 0x9f739926, /// 0xfc0
			 0x942679c5, /// 0xfc4
			 0xf0559f71, /// 0xfc8
			 0xb8559000, /// 0xfcc
			 0xdb701580, /// 0xfd0
			 0xbc7fb9fc, /// 0xfd4
			 0xc384932a, /// 0xfd8
			 0xc24053a4, /// 0xfdc
			 0xcfa06ba8, /// 0xfe0
			 0x590a1418, /// 0xfe4
			 0x98be6c86, /// 0xfe8
			 0x5ed2c6ba, /// 0xfec
			 0x80094a5c, /// 0xff0
			 0xb58ee776, /// 0xff4
			 0xc95eb71a, /// 0xff8
			 0xce7b9845 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00000
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00004
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00008
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0000c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00010
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00014
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00020
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0002c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x3f028f5c,                                                  // 0.51                                        /// 0003c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x7f800000,                                                  // inf                                         /// 00048
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0004c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00050
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00058
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00060
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00064
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00074
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00078
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0007c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00080
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00084
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00088
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0008c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00090
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00094
			 0x33333333,                                                  // 4 random values                             /// 00098
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000ac
			 0xcb000000,                                                  // -8388608.0                                  /// 000b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00100
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00104
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00108
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0010c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00110
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00114
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0011c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00120
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00124
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x33333333,                                                  // 4 random values                             /// 0012c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00130
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00134
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00138
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00140
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00144
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0014c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00150
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00158
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00164
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00168
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0016c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00170
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00174
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00178
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0017c
			 0x80000000,                                                  // -zero                                       /// 00180
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00184
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00188
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00190
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00194
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00198
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 001b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001e8
			 0xff800000,                                                  // -inf                                        /// 001ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00200
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00204
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00208
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0020c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00210
			 0x0e000003,                                                  // Trailing 1s:                                /// 00214
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00218
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00220
			 0x00011111,                                                  // 9.7958E-41                                  /// 00224
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x33333333,                                                  // 4 random values                             /// 0022c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00230
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00234
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00238
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0023c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00244
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00248
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0024c
			 0x33333333,                                                  // 4 random values                             /// 00250
			 0x7f800000,                                                  // inf                                         /// 00254
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0025c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00260
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00264
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00270
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00274
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00278
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0027c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00280
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00288
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0028c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00290
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00294
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00298
			 0xbf028f5c,                                                  // -0.51                                       /// 0029c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002a0
			 0x3f028f5c,                                                  // 0.51                                        /// 002a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002c8
			 0xbf028f5c,                                                  // -0.51                                       /// 002cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002f4
			 0x7f800000,                                                  // inf                                         /// 002f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00300
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00304
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00308
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00310
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00314
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00318
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0031c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00324
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00328
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0032c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00330
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00338
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0033c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00340
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00344
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0c700000,                                                  // Leading 1s:                                 /// 00358
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00360
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00364
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00368
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00374
			 0x0e000003,                                                  // Trailing 1s:                                /// 00378
			 0xbf028f5c,                                                  // -0.51                                       /// 0037c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0038c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00394
			 0x7fc00001,                                                  // signaling NaN                               /// 00398
			 0xff800000,                                                  // -inf                                        /// 0039c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003a4
			 0x55555555,                                                  // 4 random values                             /// 003a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00400
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00404
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00410
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0e000007,                                                  // Trailing 1s:                                /// 00418
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0041c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00420
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00424
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00430
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00434
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00438
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0043c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00440
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0044c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00450
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00454
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x0c600000,                                                  // Leading 1s:                                 /// 0045c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00460
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00464
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00468
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0046c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00474
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x0c700000,                                                  // Leading 1s:                                 /// 0047c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00480
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00488
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0048c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00490
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00494
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00498
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0049c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x7f800000,                                                  // inf                                         /// 004d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0xffc00001,                                                  // -signaling NaN                              /// 00504
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0050c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00510
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00514
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00518
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0051c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00520
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00524
			 0x0e000003,                                                  // Trailing 1s:                                /// 00528
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0052c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00530
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00534
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0053c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00540
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80000000,                                                  // -zero                                       /// 00548
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00550
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00554
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00558
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0055c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x0e000001,                                                  // Trailing 1s:                                /// 00564
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00568
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0056c
			 0x55555555,                                                  // 4 random values                             /// 00570
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00574
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00578
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0057c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00580
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00584
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00588
			 0x0c400000,                                                  // Leading 1s:                                 /// 0058c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00590
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00598
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005ec
			 0x4b000000,                                                  // 8388608.0                                   /// 005f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00604
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00608
			 0x80000000,                                                  // -zero                                       /// 0060c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00610
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00614
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00618
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00620
			 0x7f800000,                                                  // inf                                         /// 00624
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00630
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0063c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00640
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00644
			 0x00011111,                                                  // 9.7958E-41                                  /// 00648
			 0x0c600000,                                                  // Leading 1s:                                 /// 0064c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00654
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00658
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xffc00001,                                                  // -signaling NaN                              /// 00660
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00664
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00668
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0066c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00674
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00678
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0067c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00680
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00684
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00688
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0068c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00690
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00694
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0069c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a0
			 0x4b000000,                                                  // 8388608.0                                   /// 006a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006ac
			 0x55555555,                                                  // 4 random values                             /// 006b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006f0
			 0x00000000,                                                  // zero                                        /// 006f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00700
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0070c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x7f800000,                                                  // inf                                         /// 00714
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0071c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00720
			 0xff800000,                                                  // -inf                                        /// 00724
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00728
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00734
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0073c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00740
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00744
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x0c700000,                                                  // Leading 1s:                                 /// 00754
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00758
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0075c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00760
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00764
			 0x0c700000,                                                  // Leading 1s:                                 /// 00768
			 0x55555555,                                                  // 4 random values                             /// 0076c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00770
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00778
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0077c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00780
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00784
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0078c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00790
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00798
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0079c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x7fc00001,                                                  // signaling NaN                               /// 007bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007f8
			 0x00000000,                                                  // zero                                        /// 007fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00800
			 0xcb000000,                                                  // -8388608.0                                  /// 00804
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00808
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00810
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00814
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00818
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00820
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00824
			 0x4b000000,                                                  // 8388608.0                                   /// 00828
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00830
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00834
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00838
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00840
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00844
			 0xcb000000,                                                  // -8388608.0                                  /// 00848
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0084c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00850
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00858
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0085c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00864
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0086c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0xffc00001,                                                  // -signaling NaN                              /// 00884
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00888
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0088c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00890
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00898
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0089c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80000000,                                                  // -zero                                       /// 008c0
			 0x3f028f5c,                                                  // 0.51                                        /// 008c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00904
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00908
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00910
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00914
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00918
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0091c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00920
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00924
			 0x80000000,                                                  // -zero                                       /// 00928
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0092c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00934
			 0x00000000,                                                  // zero                                        /// 00938
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0093c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00948
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0094c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00958
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00000000,                                                  // zero                                        /// 00964
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0096c
			 0xcb000000,                                                  // -8388608.0                                  /// 00970
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00978
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00984
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00988
			 0x0c400000,                                                  // Leading 1s:                                 /// 0098c
			 0xff800000,                                                  // -inf                                        /// 00990
			 0x0c600000,                                                  // Leading 1s:                                 /// 00994
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00998
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a0c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a28
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a34
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80000000,                                                  // -zero                                       /// 00a70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a84
			 0x00000000,                                                  // zero                                        /// 00a88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a90
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a98
			 0x3f028f5c,                                                  // 0.51                                        /// 00a9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00aa4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aa8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00aac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ab0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ab8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ac4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00acc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ad4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ad8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00adc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ae4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ae8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00aec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00af4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b04
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b2c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b68
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b88
			 0x00000000,                                                  // zero                                        /// 00b8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b94
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ba0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ba8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bc8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bcc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bd0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bd4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bdc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00be0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bf0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bf4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bf8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c08
			 0x4b000000,                                                  // 8388608.0                                   /// 00c0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c28
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c2c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c64
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c74
			 0xff800000,                                                  // -inf                                        /// 00c78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c90
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c9c
			 0xff800000,                                                  // -inf                                        /// 00ca0
			 0xff800000,                                                  // -inf                                        /// 00ca4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cb0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cc8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ccc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cd0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cd8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ce4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ce8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cf0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cf4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cfc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d08
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x33333333,                                                  // 4 random values                             /// 00d10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d24
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d30
			 0x4b000000,                                                  // 8388608.0                                   /// 00d34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d38
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d58
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d8c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d94
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00da0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00da8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00db0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00db4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dbc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dc8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dcc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dd8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00de4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x33333333,                                                  // 4 random values                             /// 00df4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00df8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x7f800000,                                                  // inf                                         /// 00e04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e14
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e34
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e58
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x80000000,                                                  // -zero                                       /// 00e60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e90
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e98
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ea4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00eb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00ebc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ecc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ed4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ed8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00edc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ee4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ee8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00eec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ef8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00efc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f64
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f70
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fa4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x00000000,                                                  // zero                                        /// 00fac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fb0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fb4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fb8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fc0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fcc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fd4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fdc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fe0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fe4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fe8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ff0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x7f7ffffe // max norm - 1ulp                               // SP +ve numbers                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00008
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0000c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00014
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0001c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00030
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00034
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00040
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00048
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00050
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00060
			 0x0e000003,                                                  // Trailing 1s:                                /// 00064
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00068
			 0x7f800000,                                                  // inf                                         /// 0006c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00070
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00074
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00078
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0007c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00080
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00084
			 0x0c700000,                                                  // Leading 1s:                                 /// 00088
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00090
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00094
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0009c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000bc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x7f800000,                                                  // inf                                         /// 000e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00100
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00104
			 0x4b000000,                                                  // 8388608.0                                   /// 00108
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0010c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00114
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00118
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0011c
			 0xff800000,                                                  // -inf                                        /// 00120
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0012c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80000000,                                                  // -zero                                       /// 00134
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x55555555,                                                  // 4 random values                             /// 0013c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x7fc00001,                                                  // signaling NaN                               /// 00144
			 0x0e000003,                                                  // Trailing 1s:                                /// 00148
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0014c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00150
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00158
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00164
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00168
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00170
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00000000,                                                  // zero                                        /// 00178
			 0x0c780000,                                                  // Leading 1s:                                 /// 0017c
			 0xff800000,                                                  // -inf                                        /// 00180
			 0x55555555,                                                  // 4 random values                             /// 00184
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00188
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0018c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x00011111,                                                  // 9.7958E-41                                  /// 00194
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0019c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x3f028f5c,                                                  // 0.51                                        /// 001a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00000000,                                                  // zero                                        /// 001b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001f4
			 0x7fc00001,                                                  // signaling NaN                               /// 001f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00200
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00208
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0021c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00220
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00224
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00228
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00234
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00238
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0023c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00240
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00244
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0024c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00250
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x0e000001,                                                  // Trailing 1s:                                /// 00258
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0025c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00260
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00264
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00268
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00270
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00274
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00284
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00288
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00290
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00300
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00304
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00310
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00314
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0031c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00320
			 0xffc00001,                                                  // -signaling NaN                              /// 00324
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0032c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00330
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00338
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00344
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00350
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00354
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00358
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00360
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00364
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00370
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00378
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00380
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00384
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00390
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00394
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00398
			 0x33333333,                                                  // 4 random values                             /// 0039c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003c8
			 0x00000000,                                                  // zero                                        /// 003cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003e0
			 0x3f028f5c,                                                  // 0.51                                        /// 003e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00404
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00408
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00418
			 0x33333333,                                                  // 4 random values                             /// 0041c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00420
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0042c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00430
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00434
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00438
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00448
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0044c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00450
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00454
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00460
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00464
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0046c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00470
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00474
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0047c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00480
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00484
			 0xcb000000,                                                  // -8388608.0                                  /// 00488
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0048c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00490
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00498
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0049c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0xffc00001,                                                  // -signaling NaN                              /// 004c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00500
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00508
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0050c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00510
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00520
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00524
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00528
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0052c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00530
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00534
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00538
			 0xffc00001,                                                  // -signaling NaN                              /// 0053c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00540
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00548
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00550
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00554
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0055c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00564
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00568
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0056c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x00011111,                                                  // 9.7958E-41                                  /// 00578
			 0x0e000001,                                                  // Trailing 1s:                                /// 0057c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00584
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00588
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00590
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0059c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005a8
			 0xff800000,                                                  // -inf                                        /// 005ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 005c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x4b000000,                                                  // 8388608.0                                   /// 005cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005ec
			 0x33333333,                                                  // 4 random values                             /// 005f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00604
			 0x0e000001,                                                  // Trailing 1s:                                /// 00608
			 0x0e000007,                                                  // Trailing 1s:                                /// 0060c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00610
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00614
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0061c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00620
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00628
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0062c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00634
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0063c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00644
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00648
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0064c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00650
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00654
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00664
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0066c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00670
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00674
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00678
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0067c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00684
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c400000,                                                  // Leading 1s:                                 /// 0068c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00698
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0069c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006e4
			 0x7fc00001,                                                  // signaling NaN                               /// 006e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00700
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00708
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00710
			 0x00011111,                                                  // 9.7958E-41                                  /// 00714
			 0x7fc00001,                                                  // signaling NaN                               /// 00718
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00724
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00728
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0072c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00738
			 0x0c700000,                                                  // Leading 1s:                                 /// 0073c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00740
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00744
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00748
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0074c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00750
			 0x55555555,                                                  // 4 random values                             /// 00754
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00760
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00764
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0076c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00770
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00774
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00778
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0077c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00780
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00784
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0078c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x4b000000,                                                  // 8388608.0                                   /// 00794
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00798
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0079c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007bc
			 0x7f800000,                                                  // inf                                         /// 007c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007dc
			 0xff800000,                                                  // -inf                                        /// 007e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00800
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00804
			 0x0c400000,                                                  // Leading 1s:                                 /// 00808
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0080c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0xff800000,                                                  // -inf                                        /// 00814
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00818
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0081c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00820
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00824
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00828
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00838
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00840
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0084c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00858
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0085c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00860
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00868
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0086c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00870
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00874
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00878
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0087c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00884
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00888
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00890
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00894
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0089c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008b8
			 0x4b000000,                                                  // 8388608.0                                   /// 008bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00900
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00908
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0090c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00910
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00914
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00918
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0091c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00924
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00928
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0092c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00930
			 0x4b000000,                                                  // 8388608.0                                   /// 00934
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00938
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00940
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00944
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0094c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00950
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00954
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00958
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0095c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00960
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00964
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00968
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x7fc00001,                                                  // signaling NaN                               /// 00974
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0097c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00980
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00988
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00994
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0099c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009cc
			 0x33333333,                                                  // 4 random values                             /// 009d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a10
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xbf028f5c,                                                  // -0.51                                       /// 00a18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x7fc00001,                                                  // signaling NaN                               /// 00a34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a40
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a44
			 0x33333333,                                                  // 4 random values                             /// 00a48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a7c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00aa4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00aa8
			 0xff800000,                                                  // -inf                                        /// 00aac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ab0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00abc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ac0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ac8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00acc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ad0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ad4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ad8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ae0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ae4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00af0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00af4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00afc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80000000,                                                  // -zero                                       /// 00b2c
			 0x80000000,                                                  // -zero                                       /// 00b30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0xbf028f5c,                                                  // -0.51                                       /// 00b38
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b48
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b64
			 0x55555555,                                                  // 4 random values                             /// 00b68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0xcb000000,                                                  // -8388608.0                                  /// 00b74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b84
			 0x33333333,                                                  // 4 random values                             /// 00b88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ba4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ba8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bb4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bc4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bcc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bd4
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bdc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00be0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bf0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bf4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c08
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c18
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x00000000,                                                  // zero                                        /// 00c28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c40
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c68
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cdc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cf0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cf4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cf8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cfc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d0c
			 0x7f800000,                                                  // inf                                         /// 00d10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d4c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d74
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d78
			 0x80000000,                                                  // -zero                                       /// 00d7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00da4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00db0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00db4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00db8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dcc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ddc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00de8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00df0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00df4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e24
			 0x4b000000,                                                  // 8388608.0                                   /// 00e28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x7f800000,                                                  // inf                                         /// 00e68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e78
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e84
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e90
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ea0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ea8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00eac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eb4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00eb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00ebc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ecc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ed4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ed8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00edc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ee4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00eec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ef0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ef4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00efc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f04
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f2c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f48
			 0x33333333,                                                  // 4 random values                             /// 00f4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f54
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f58
			 0x4b000000,                                                  // 8388608.0                                   /// 00f5c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f90
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f9c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fa4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fbc
			 0x55555555,                                                  // 4 random values                             /// 00fc0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fc4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fe8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ff0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ff4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
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
			 0x000000ac,
			 0x0000062c,
			 0x00000598,
			 0x00000274,
			 0x000001d0,
			 0x0000073c,
			 0x00000370,
			 0x0000061c,

			 /// vpu register f2
			 0x40c00000,
			 0x41800000,
			 0x41d00000,
			 0x41800000,
			 0x40000000,
			 0x41100000,
			 0x41a00000,
			 0x41400000,

			 /// vpu register f3
			 0x41c00000,
			 0x41f80000,
			 0x41400000,
			 0x41900000,
			 0x41e00000,
			 0x41b80000,
			 0x41c00000,
			 0x41f80000,

			 /// vpu register f4
			 0x40e00000,
			 0x41400000,
			 0x41600000,
			 0x41300000,
			 0x41e80000,
			 0x41000000,
			 0x41980000,
			 0x40c00000,

			 /// vpu register f5
			 0x40400000,
			 0x41200000,
			 0x41b00000,
			 0x41800000,
			 0x41900000,
			 0x41100000,
			 0x41d80000,
			 0x40c00000,

			 /// vpu register f6
			 0x41400000,
			 0x41a00000,
			 0x42000000,
			 0x42000000,
			 0x41200000,
			 0x41c00000,
			 0x40a00000,
			 0x41a80000,

			 /// vpu register f7
			 0x41880000,
			 0x40a00000,
			 0x41b80000,
			 0x40c00000,
			 0x41f80000,
			 0x40800000,
			 0x41200000,
			 0x41100000,

			 /// vpu register f8
			 0x41900000,
			 0x41f80000,
			 0x41800000,
			 0x41300000,
			 0x40800000,
			 0x42000000,
			 0x41980000,
			 0x42000000,

			 /// vpu register f9
			 0x41c00000,
			 0x41880000,
			 0x41e80000,
			 0x40000000,
			 0x41e00000,
			 0x40800000,
			 0x41880000,
			 0x41d00000,

			 /// vpu register f10
			 0x41900000,
			 0x41800000,
			 0x41a80000,
			 0x42000000,
			 0x41d80000,
			 0x41d00000,
			 0x41f00000,
			 0x41300000,

			 /// vpu register f11
			 0x41000000,
			 0x41800000,
			 0x40e00000,
			 0x41f00000,
			 0x41400000,
			 0x41100000,
			 0x41b80000,
			 0x41600000,

			 /// vpu register f12
			 0x41d80000,
			 0x41900000,
			 0x41a80000,
			 0x40400000,
			 0x41f00000,
			 0x41980000,
			 0x40400000,
			 0x40000000,

			 /// vpu register f13
			 0x41400000,
			 0x40c00000,
			 0x41900000,
			 0x40c00000,
			 0x41c00000,
			 0x41880000,
			 0x40400000,
			 0x41f80000,

			 /// vpu register f14
			 0x40800000,
			 0x40e00000,
			 0x41800000,
			 0x40800000,
			 0x41500000,
			 0x41a00000,
			 0x41300000,
			 0x41100000,

			 /// vpu register f15
			 0x41d00000,
			 0x40a00000,
			 0x3f800000,
			 0x41880000,
			 0x3f800000,
			 0x41a00000,
			 0x41a00000,
			 0x41100000,

			 /// vpu register f16
			 0x41880000,
			 0x41000000,
			 0x40800000,
			 0x41100000,
			 0x41300000,
			 0x40e00000,
			 0x41b00000,
			 0x41200000,

			 /// vpu register f17
			 0x42000000,
			 0x40800000,
			 0x40a00000,
			 0x41e00000,
			 0x41f80000,
			 0x41e80000,
			 0x41100000,
			 0x41b00000,

			 /// vpu register f18
			 0x40000000,
			 0x41e80000,
			 0x41980000,
			 0x41400000,
			 0x41c00000,
			 0x41a80000,
			 0x3f800000,
			 0x40800000,

			 /// vpu register f19
			 0x41d80000,
			 0x3f800000,
			 0x40e00000,
			 0x41d00000,
			 0x41900000,
			 0x40e00000,
			 0x41500000,
			 0x41100000,

			 /// vpu register f20
			 0x41b00000,
			 0x41b80000,
			 0x42000000,
			 0x41900000,
			 0x40400000,
			 0x3f800000,
			 0x41d00000,
			 0x40800000,

			 /// vpu register f21
			 0x41c80000,
			 0x41000000,
			 0x3f800000,
			 0x41800000,
			 0x41d80000,
			 0x41c00000,
			 0x41500000,
			 0x41f00000,

			 /// vpu register f22
			 0x40a00000,
			 0x41300000,
			 0x41e00000,
			 0x41b80000,
			 0x41980000,
			 0x41e00000,
			 0x41a00000,
			 0x40a00000,

			 /// vpu register f23
			 0x3f800000,
			 0x41980000,
			 0x41c80000,
			 0x41800000,
			 0x41e80000,
			 0x40400000,
			 0x41b80000,
			 0x41b00000,

			 /// vpu register f24
			 0x41880000,
			 0x41e00000,
			 0x41980000,
			 0x41b00000,
			 0x40000000,
			 0x41c00000,
			 0x41c80000,
			 0x41a80000,

			 /// vpu register f25
			 0x40400000,
			 0x41100000,
			 0x41b00000,
			 0x41c80000,
			 0x41100000,
			 0x41c80000,
			 0x41880000,
			 0x41500000,

			 /// vpu register f26
			 0x41c00000,
			 0x41000000,
			 0x40000000,
			 0x42000000,
			 0x41000000,
			 0x40e00000,
			 0x41500000,
			 0x41980000,

			 /// vpu register f27
			 0x41b80000,
			 0x3f800000,
			 0x41b80000,
			 0x40c00000,
			 0x41500000,
			 0x3f800000,
			 0x41d80000,
			 0x3f800000,

			 /// vpu register f28
			 0x41e00000,
			 0x41e80000,
			 0x41e00000,
			 0x40c00000,
			 0x41880000,
			 0x40400000,
			 0x41d80000,
			 0x41500000,

			 /// vpu register f29
			 0x41f00000,
			 0x41300000,
			 0x41980000,
			 0x41880000,
			 0x41980000,
			 0x41d00000,
			 0x41900000,
			 0x41c80000,

			 /// vpu register f30
			 0x41d00000,
			 0x41900000,
			 0x41700000,
			 0x40a00000,
			 0x41100000,
			 0x41e00000,
			 0x41000000,
			 0x3f800000,

			 /// vpu register f31
			 0x40c00000,
			 0x40e00000,
			 0x41b80000,
			 0x41f80000,
			 0x41b00000,
			 0x41c80000,
			 0x41600000,
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
		"fsll.pi f19, f14, f0\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f26, f15, f17\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f21, f18, f24\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f7, f14, f26\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f10, f10, f5\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f30, f4, f13\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f23, f24, f16\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f27, f30, f9\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f12, f10, f18\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f20, f30, f6\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f17, f19, f19\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f9, f19, f2\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f15, f23, f26\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f18, f26, f18\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f18, f4, f24\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f13, f18, f9\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f14, f0, f28\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f21, f22, f18\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f11, f27, f1\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f19, f19, f20\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f15, f0, f9\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f19, f5, f12\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f17, f22, f26\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f20, f12, f6\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f26, f5, f1\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f22, f13, f25\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f17, f11, f24\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f28, f5, f22\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f23, f30, f18\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f16, f15, f30\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f17, f21, f7\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f8, f19, f9\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f2, f7, f15\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f20, f22, f0\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f23, f10, f10\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f10, f13, f26\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f24, f16, f12\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f25, f3, f12\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f8, f20, f22\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f15, f7, f29\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f20, f22, f5\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f22, f7, f6\n"                               ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f2, f12, f30\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f30, f12, f16\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f10, f2, f8\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f7, f0, f17\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f22, f2, f3\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f10, f16, f22\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f19, f10, f4\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f25, f2, f2\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f9, f29, f15\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f18, f6, f28\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f9, f4, f18\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f10, f6, f22\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f4, f18, f18\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f5, f14, f16\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f8, f9, f26\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f18, f25, f1\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f28, f8, f24\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f27, f28, f29\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f22, f14, f8\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f25, f17, f17\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f16, f16, f3\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f27, f27, f29\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f14, f8, f15\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f5, f10, f15\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f10, f19, f25\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f7, f6, f22\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f4, f4, f26\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f26, f21, f0\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f12, f29, f18\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f8, f12, f14\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f22, f20, f11\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f23, f2, f5\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f29, f25, f20\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f12, f10, f17\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f14, f12, f7\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f19, f12, f25\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f24, f22, f15\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f24, f2, f6\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f27, f15, f23\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f17, f16, f23\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f5, f13, f11\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f12, f30, f13\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f29, f19, f22\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f3, f0, f16\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f11, f13, f15\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f29, f2, f17\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f14, f28, f10\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f22, f15, f14\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f14, f3, f2\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f25, f21, f13\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f9, f10, f24\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f23, f22, f1\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f3, f0, f9\n"                                ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f11, f23, f24\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f22, f30, f22\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f15, f27, f10\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsll.pi f24, f29, f4\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
