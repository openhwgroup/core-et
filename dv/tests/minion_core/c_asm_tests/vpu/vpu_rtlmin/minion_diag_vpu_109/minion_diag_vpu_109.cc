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
			 0x821c8215, /// 0x0
			 0xf3ea6cd8, /// 0x4
			 0x80d43acd, /// 0x8
			 0x6677be0d, /// 0xc
			 0xff09a9e5, /// 0x10
			 0xc5343533, /// 0x14
			 0x22793d7f, /// 0x18
			 0x53394d0e, /// 0x1c
			 0xe3e7185d, /// 0x20
			 0x9dc38398, /// 0x24
			 0xd9bddf2f, /// 0x28
			 0x1c49f1ec, /// 0x2c
			 0x97da4aa4, /// 0x30
			 0x1362339f, /// 0x34
			 0x26e11ebc, /// 0x38
			 0x89227318, /// 0x3c
			 0x8c44cf35, /// 0x40
			 0xbbe6ffc4, /// 0x44
			 0xd57d17ba, /// 0x48
			 0xe4b0e128, /// 0x4c
			 0x05cf0497, /// 0x50
			 0x83e017a9, /// 0x54
			 0x185aaf5c, /// 0x58
			 0xa1f3a3ee, /// 0x5c
			 0xe1fccc1e, /// 0x60
			 0xe8ee1c94, /// 0x64
			 0xd9d648ba, /// 0x68
			 0x7ee7a4bf, /// 0x6c
			 0x4c9be952, /// 0x70
			 0x1a055732, /// 0x74
			 0x20610eaf, /// 0x78
			 0x69e8fde5, /// 0x7c
			 0x9d4c2599, /// 0x80
			 0x64f7f4e8, /// 0x84
			 0x44510c7d, /// 0x88
			 0xd602f3ed, /// 0x8c
			 0xf2dc4b15, /// 0x90
			 0x03cf7634, /// 0x94
			 0xa4ba743e, /// 0x98
			 0x935d8efc, /// 0x9c
			 0xa5fe641b, /// 0xa0
			 0xb4c6250c, /// 0xa4
			 0x406bde13, /// 0xa8
			 0x529bc255, /// 0xac
			 0x63689863, /// 0xb0
			 0x9ec50ff7, /// 0xb4
			 0x6516f4f2, /// 0xb8
			 0x473f6982, /// 0xbc
			 0x164086f9, /// 0xc0
			 0xa1e34a8d, /// 0xc4
			 0xaeee3130, /// 0xc8
			 0x76a39bd3, /// 0xcc
			 0xa54964e1, /// 0xd0
			 0xd816d337, /// 0xd4
			 0x9d2b83ce, /// 0xd8
			 0x36839fb1, /// 0xdc
			 0xabb2cc48, /// 0xe0
			 0x773d212f, /// 0xe4
			 0x0fa5c2be, /// 0xe8
			 0xfa1f86ca, /// 0xec
			 0x0a19e142, /// 0xf0
			 0x55b548b6, /// 0xf4
			 0xffdcc21e, /// 0xf8
			 0x5bc4f126, /// 0xfc
			 0xc579cefa, /// 0x100
			 0xfebca834, /// 0x104
			 0xffb0c69b, /// 0x108
			 0x1094c405, /// 0x10c
			 0xdca28843, /// 0x110
			 0x49b07ff2, /// 0x114
			 0xb4b9f977, /// 0x118
			 0x2ca7161c, /// 0x11c
			 0x2a4620b8, /// 0x120
			 0x071e34c9, /// 0x124
			 0x3ae70db6, /// 0x128
			 0x01c2a022, /// 0x12c
			 0x1f2d2dec, /// 0x130
			 0x4930f52c, /// 0x134
			 0xaeeeddf8, /// 0x138
			 0x1df4506b, /// 0x13c
			 0x7a91ad21, /// 0x140
			 0xb406cf3c, /// 0x144
			 0xee95408d, /// 0x148
			 0x5bff2f16, /// 0x14c
			 0x895ff310, /// 0x150
			 0x89eb7e96, /// 0x154
			 0x2fbdd7c1, /// 0x158
			 0xa07516d6, /// 0x15c
			 0x5c580763, /// 0x160
			 0x515a1d35, /// 0x164
			 0x24d8ebaa, /// 0x168
			 0x7369f2b7, /// 0x16c
			 0x0bba2eb9, /// 0x170
			 0xfd25b33c, /// 0x174
			 0x0392a35d, /// 0x178
			 0x4cf7249f, /// 0x17c
			 0x57cda1e9, /// 0x180
			 0xe4e963a7, /// 0x184
			 0x2e1862c2, /// 0x188
			 0x736efaec, /// 0x18c
			 0xdd17577f, /// 0x190
			 0x247af4cc, /// 0x194
			 0xdbc01b46, /// 0x198
			 0x39191fcd, /// 0x19c
			 0x4d454a62, /// 0x1a0
			 0xbd8350c0, /// 0x1a4
			 0x76a91ff2, /// 0x1a8
			 0xbbd2ba7c, /// 0x1ac
			 0xfb6b5d94, /// 0x1b0
			 0x1194729e, /// 0x1b4
			 0x48e21158, /// 0x1b8
			 0x1dacd5d0, /// 0x1bc
			 0x9b7f9482, /// 0x1c0
			 0xb8c555ed, /// 0x1c4
			 0xf3a351f8, /// 0x1c8
			 0xceca8648, /// 0x1cc
			 0x86535892, /// 0x1d0
			 0xeb37e39b, /// 0x1d4
			 0xca6d808f, /// 0x1d8
			 0x29f3ff35, /// 0x1dc
			 0x15149fde, /// 0x1e0
			 0x186e882e, /// 0x1e4
			 0xc18c6019, /// 0x1e8
			 0xcf392e97, /// 0x1ec
			 0x9fa755da, /// 0x1f0
			 0x8ed80714, /// 0x1f4
			 0x830fe4cd, /// 0x1f8
			 0xbdea06ce, /// 0x1fc
			 0x36171729, /// 0x200
			 0xa4f364b4, /// 0x204
			 0x80a4a761, /// 0x208
			 0xffe673d8, /// 0x20c
			 0xa5755ba0, /// 0x210
			 0xc8d018c0, /// 0x214
			 0xaa82f4e6, /// 0x218
			 0xbfda4f50, /// 0x21c
			 0x5dc38a2c, /// 0x220
			 0xdf0568d8, /// 0x224
			 0xb888e243, /// 0x228
			 0x3b3368d6, /// 0x22c
			 0x6606fd83, /// 0x230
			 0x79c71189, /// 0x234
			 0xf4e2a738, /// 0x238
			 0xfb580336, /// 0x23c
			 0xb352d2b3, /// 0x240
			 0x8d929198, /// 0x244
			 0x2172f37a, /// 0x248
			 0x235285f9, /// 0x24c
			 0xe99e70f2, /// 0x250
			 0xb0230ed8, /// 0x254
			 0x8d21dfb9, /// 0x258
			 0x976f1b56, /// 0x25c
			 0xab884ee0, /// 0x260
			 0x5b5cfbce, /// 0x264
			 0x3d153eb0, /// 0x268
			 0x40fa128c, /// 0x26c
			 0xd5f22d8d, /// 0x270
			 0x6cc7f26e, /// 0x274
			 0x193810e5, /// 0x278
			 0x34e1c499, /// 0x27c
			 0x865cbd5c, /// 0x280
			 0xf3db4722, /// 0x284
			 0x6201736b, /// 0x288
			 0xfc37a6f6, /// 0x28c
			 0xd9c76155, /// 0x290
			 0xc953caea, /// 0x294
			 0x5f6c2f4c, /// 0x298
			 0x9efeae3c, /// 0x29c
			 0x8f378d19, /// 0x2a0
			 0x7a50e917, /// 0x2a4
			 0x3f3da41c, /// 0x2a8
			 0x8387c09f, /// 0x2ac
			 0x52945460, /// 0x2b0
			 0x83d50aad, /// 0x2b4
			 0x91ba6f7f, /// 0x2b8
			 0xb038943d, /// 0x2bc
			 0x2c637fd8, /// 0x2c0
			 0x790b82eb, /// 0x2c4
			 0x34b091af, /// 0x2c8
			 0x36bb9225, /// 0x2cc
			 0x6b622f2f, /// 0x2d0
			 0x38a37250, /// 0x2d4
			 0x4d319d34, /// 0x2d8
			 0xa3a5af4e, /// 0x2dc
			 0x504434ce, /// 0x2e0
			 0xc0ff8e2a, /// 0x2e4
			 0x39c95cc8, /// 0x2e8
			 0x6aa83cb6, /// 0x2ec
			 0x37519e9d, /// 0x2f0
			 0xe279fb70, /// 0x2f4
			 0xf14044ef, /// 0x2f8
			 0xa58be7e8, /// 0x2fc
			 0xc9f16193, /// 0x300
			 0xda747584, /// 0x304
			 0xbd45d6c9, /// 0x308
			 0x39606df8, /// 0x30c
			 0xc6915b43, /// 0x310
			 0xf625c6ab, /// 0x314
			 0x8a7d3a7a, /// 0x318
			 0xf075f788, /// 0x31c
			 0xc448516e, /// 0x320
			 0x6190335c, /// 0x324
			 0xb1f642ed, /// 0x328
			 0x0fdb28c8, /// 0x32c
			 0xe380ac61, /// 0x330
			 0xa1ef2882, /// 0x334
			 0x6d45b07f, /// 0x338
			 0x184aaf79, /// 0x33c
			 0xd874817d, /// 0x340
			 0x914338bd, /// 0x344
			 0x05bcb9b1, /// 0x348
			 0xe079dfb8, /// 0x34c
			 0x398297a9, /// 0x350
			 0x0c002024, /// 0x354
			 0x7b9f83ff, /// 0x358
			 0x55e8920b, /// 0x35c
			 0x1db0eddc, /// 0x360
			 0x6116051a, /// 0x364
			 0x1ca6f6b8, /// 0x368
			 0x24444218, /// 0x36c
			 0x2626663d, /// 0x370
			 0x18a2b0e6, /// 0x374
			 0x3451537b, /// 0x378
			 0xc2ca5d4f, /// 0x37c
			 0x67931f5d, /// 0x380
			 0xf6c3fa24, /// 0x384
			 0x75f6b69f, /// 0x388
			 0x698d7766, /// 0x38c
			 0xfa5ed8b3, /// 0x390
			 0xe58c5a6b, /// 0x394
			 0x86f8f4ca, /// 0x398
			 0xe4663a60, /// 0x39c
			 0x27ff330a, /// 0x3a0
			 0xd112bb99, /// 0x3a4
			 0xfc5dd701, /// 0x3a8
			 0xfa006a35, /// 0x3ac
			 0xf4d675fe, /// 0x3b0
			 0x3e6ddc8d, /// 0x3b4
			 0x78386382, /// 0x3b8
			 0x60b04585, /// 0x3bc
			 0xe0ad2521, /// 0x3c0
			 0xf9849b5a, /// 0x3c4
			 0x71f044fd, /// 0x3c8
			 0x548b796b, /// 0x3cc
			 0x2ae21613, /// 0x3d0
			 0xa63067cc, /// 0x3d4
			 0xce7fb04d, /// 0x3d8
			 0xd9bc73e2, /// 0x3dc
			 0x4501f0b2, /// 0x3e0
			 0x5dbad74a, /// 0x3e4
			 0x2964e382, /// 0x3e8
			 0x8e82efd8, /// 0x3ec
			 0xc0373ab5, /// 0x3f0
			 0x8742483f, /// 0x3f4
			 0x82c42952, /// 0x3f8
			 0x68d72d62, /// 0x3fc
			 0xc9feae98, /// 0x400
			 0x4498ea1f, /// 0x404
			 0x617761c5, /// 0x408
			 0x52224154, /// 0x40c
			 0x28e4dc5a, /// 0x410
			 0xbeca9204, /// 0x414
			 0x0a8bf3d2, /// 0x418
			 0xe4e3d292, /// 0x41c
			 0x28b0f4b3, /// 0x420
			 0xebc6a9f0, /// 0x424
			 0x63480de9, /// 0x428
			 0xeea5aa1a, /// 0x42c
			 0xc06935bf, /// 0x430
			 0x5284f5fb, /// 0x434
			 0xd16c9e4b, /// 0x438
			 0x339ad10d, /// 0x43c
			 0x5520ae37, /// 0x440
			 0xb0a22f4e, /// 0x444
			 0x042323c0, /// 0x448
			 0x827befbf, /// 0x44c
			 0x9beea764, /// 0x450
			 0x57e748d0, /// 0x454
			 0x419a43ac, /// 0x458
			 0x1c683190, /// 0x45c
			 0x6932593d, /// 0x460
			 0xbb5ae656, /// 0x464
			 0x211825e9, /// 0x468
			 0x956836e5, /// 0x46c
			 0x85de9f02, /// 0x470
			 0x1ce8d9aa, /// 0x474
			 0xe92e6a23, /// 0x478
			 0x0f0cdaa8, /// 0x47c
			 0xcbb1f993, /// 0x480
			 0x5b54c7de, /// 0x484
			 0xc4864e61, /// 0x488
			 0x6441edce, /// 0x48c
			 0x79074ab1, /// 0x490
			 0x5fb1db5f, /// 0x494
			 0x305e398a, /// 0x498
			 0x8352641d, /// 0x49c
			 0xae7f05b9, /// 0x4a0
			 0xb13d8ad4, /// 0x4a4
			 0x0aea2c78, /// 0x4a8
			 0xe00be02b, /// 0x4ac
			 0xa2ec46a8, /// 0x4b0
			 0x58c9b111, /// 0x4b4
			 0x337b1715, /// 0x4b8
			 0xc8fb784d, /// 0x4bc
			 0x1473d901, /// 0x4c0
			 0x92d96b52, /// 0x4c4
			 0xd73d9eb4, /// 0x4c8
			 0xb8adc0fc, /// 0x4cc
			 0xf1b0ab4b, /// 0x4d0
			 0x255be23c, /// 0x4d4
			 0x090c5471, /// 0x4d8
			 0x4d619404, /// 0x4dc
			 0xf3aa609d, /// 0x4e0
			 0x00bd0b90, /// 0x4e4
			 0x70652372, /// 0x4e8
			 0x10ce8708, /// 0x4ec
			 0x88838d82, /// 0x4f0
			 0xd2431614, /// 0x4f4
			 0x37b3e423, /// 0x4f8
			 0x7c0eb463, /// 0x4fc
			 0xf6c00bc3, /// 0x500
			 0xe05be9d7, /// 0x504
			 0xd0f78dd5, /// 0x508
			 0x2edd6c69, /// 0x50c
			 0x5ed987ec, /// 0x510
			 0xb927d1a7, /// 0x514
			 0x56deadae, /// 0x518
			 0xc90329d4, /// 0x51c
			 0xe5b45f06, /// 0x520
			 0x23a0c6da, /// 0x524
			 0x3861af13, /// 0x528
			 0xafd8465a, /// 0x52c
			 0x02553be1, /// 0x530
			 0x06b86d9d, /// 0x534
			 0x4967208f, /// 0x538
			 0x6471f2a3, /// 0x53c
			 0x290ffe4f, /// 0x540
			 0x204f5f37, /// 0x544
			 0x0a775c9e, /// 0x548
			 0x9e2b6699, /// 0x54c
			 0xec1ab08f, /// 0x550
			 0x20e8531d, /// 0x554
			 0xf31ecfe4, /// 0x558
			 0x99a216e6, /// 0x55c
			 0x8791f510, /// 0x560
			 0x29e0e88a, /// 0x564
			 0x4fdc5cca, /// 0x568
			 0x269999d7, /// 0x56c
			 0xb0cdd24b, /// 0x570
			 0xf3955974, /// 0x574
			 0x2a7772d5, /// 0x578
			 0xb227a7fa, /// 0x57c
			 0x4e9f5efa, /// 0x580
			 0x685150d8, /// 0x584
			 0x8a6791c8, /// 0x588
			 0xa208ac60, /// 0x58c
			 0x0e3addd2, /// 0x590
			 0xd9d0f525, /// 0x594
			 0x24c62051, /// 0x598
			 0x4029e693, /// 0x59c
			 0xbd17855b, /// 0x5a0
			 0x6beb889c, /// 0x5a4
			 0x0c049bca, /// 0x5a8
			 0x569e717d, /// 0x5ac
			 0xfef01d94, /// 0x5b0
			 0x0822a720, /// 0x5b4
			 0xfc01b5ec, /// 0x5b8
			 0x7030f830, /// 0x5bc
			 0x46ba9355, /// 0x5c0
			 0x03df50e2, /// 0x5c4
			 0x44dea706, /// 0x5c8
			 0xe5374d40, /// 0x5cc
			 0x3178e910, /// 0x5d0
			 0x54f946df, /// 0x5d4
			 0xf011d8d1, /// 0x5d8
			 0x654b4cd5, /// 0x5dc
			 0x442131ac, /// 0x5e0
			 0x1e8ac679, /// 0x5e4
			 0xc81b4cf5, /// 0x5e8
			 0x994d9de0, /// 0x5ec
			 0x73c96e0c, /// 0x5f0
			 0xa605c31b, /// 0x5f4
			 0x84803643, /// 0x5f8
			 0xeecc19a3, /// 0x5fc
			 0x671893ca, /// 0x600
			 0xf5a128f8, /// 0x604
			 0x62e0dd64, /// 0x608
			 0xad61cbbb, /// 0x60c
			 0x09127f9a, /// 0x610
			 0xee45e382, /// 0x614
			 0x62a2e797, /// 0x618
			 0x3d83867e, /// 0x61c
			 0x560d03ff, /// 0x620
			 0x7f066282, /// 0x624
			 0x30d2983a, /// 0x628
			 0x48410bbd, /// 0x62c
			 0x89abd154, /// 0x630
			 0xfbeb8034, /// 0x634
			 0xe06b248c, /// 0x638
			 0x2e51b0a3, /// 0x63c
			 0xbac9752c, /// 0x640
			 0x461723e7, /// 0x644
			 0xfd350f36, /// 0x648
			 0xf14aed1f, /// 0x64c
			 0x6ff69d6a, /// 0x650
			 0xd9329a65, /// 0x654
			 0x4188cc25, /// 0x658
			 0x413923c3, /// 0x65c
			 0x1c8614d0, /// 0x660
			 0xc7b3c8a8, /// 0x664
			 0x87300ece, /// 0x668
			 0xf933ac53, /// 0x66c
			 0x4aef2721, /// 0x670
			 0xfab84cf9, /// 0x674
			 0xbf5c6a57, /// 0x678
			 0x1e322e9b, /// 0x67c
			 0xa27bc05e, /// 0x680
			 0x44f80f9d, /// 0x684
			 0x06541541, /// 0x688
			 0x68d38ed6, /// 0x68c
			 0x488ad1e5, /// 0x690
			 0x69497e2d, /// 0x694
			 0x608358eb, /// 0x698
			 0xc5a99044, /// 0x69c
			 0x13012b5d, /// 0x6a0
			 0x00cd263a, /// 0x6a4
			 0x1a681540, /// 0x6a8
			 0xc6127481, /// 0x6ac
			 0xf780ee4f, /// 0x6b0
			 0xe46af680, /// 0x6b4
			 0x38b22bad, /// 0x6b8
			 0x02294d6f, /// 0x6bc
			 0x2d29ba1c, /// 0x6c0
			 0x53900874, /// 0x6c4
			 0x3949da13, /// 0x6c8
			 0x59a7cad2, /// 0x6cc
			 0x57c57219, /// 0x6d0
			 0x7b000880, /// 0x6d4
			 0xd5c41e1f, /// 0x6d8
			 0x77bba989, /// 0x6dc
			 0x5a6d0369, /// 0x6e0
			 0x5e017601, /// 0x6e4
			 0x0637c6e8, /// 0x6e8
			 0xa2dbb892, /// 0x6ec
			 0xf6a31560, /// 0x6f0
			 0xddf74c8a, /// 0x6f4
			 0xf7eb206d, /// 0x6f8
			 0xd74ed5b0, /// 0x6fc
			 0xc398bf34, /// 0x700
			 0x3c019229, /// 0x704
			 0x3e4ff9d6, /// 0x708
			 0xabe8a299, /// 0x70c
			 0x75d4c096, /// 0x710
			 0x93aa8d41, /// 0x714
			 0xe441399f, /// 0x718
			 0x97fff1ee, /// 0x71c
			 0xaa34d4b0, /// 0x720
			 0xffbae2f1, /// 0x724
			 0xacecc835, /// 0x728
			 0xc653c1e7, /// 0x72c
			 0x1f4c04a5, /// 0x730
			 0x325b6570, /// 0x734
			 0x90bdf7b7, /// 0x738
			 0x742bd40f, /// 0x73c
			 0x18eeb9e4, /// 0x740
			 0x4c103382, /// 0x744
			 0x26b0e21a, /// 0x748
			 0x6cd46d03, /// 0x74c
			 0x5ad7cd08, /// 0x750
			 0x4c620dc5, /// 0x754
			 0x6815d62a, /// 0x758
			 0xdb4d8c41, /// 0x75c
			 0x441c61ca, /// 0x760
			 0xab1eba52, /// 0x764
			 0x46c7e619, /// 0x768
			 0x1e22632a, /// 0x76c
			 0x6fe684c1, /// 0x770
			 0x773f31a0, /// 0x774
			 0xc87ba618, /// 0x778
			 0x58f88a80, /// 0x77c
			 0x1ededf3b, /// 0x780
			 0x76bdc97a, /// 0x784
			 0xe0e6b9fc, /// 0x788
			 0x13b08a17, /// 0x78c
			 0x83fe5a7c, /// 0x790
			 0x61075be1, /// 0x794
			 0x58f37ddd, /// 0x798
			 0xac8185aa, /// 0x79c
			 0xa333109c, /// 0x7a0
			 0xc00598a5, /// 0x7a4
			 0xe815eea0, /// 0x7a8
			 0xb91d87b4, /// 0x7ac
			 0x12c652f4, /// 0x7b0
			 0x1571413b, /// 0x7b4
			 0x69765f73, /// 0x7b8
			 0x8f2b7c3d, /// 0x7bc
			 0x23a917dc, /// 0x7c0
			 0xc46bd2ba, /// 0x7c4
			 0x3ef2def5, /// 0x7c8
			 0x0fc9e036, /// 0x7cc
			 0x4439fee9, /// 0x7d0
			 0x90578e20, /// 0x7d4
			 0xeceaefc9, /// 0x7d8
			 0xf2ac138c, /// 0x7dc
			 0x35d48c2a, /// 0x7e0
			 0xa5940cfa, /// 0x7e4
			 0x03c4238c, /// 0x7e8
			 0xf6106b94, /// 0x7ec
			 0x707fe115, /// 0x7f0
			 0xb9fb59ff, /// 0x7f4
			 0xbb6ff43d, /// 0x7f8
			 0x81fb7aa2, /// 0x7fc
			 0xb78abd29, /// 0x800
			 0x05a6ea12, /// 0x804
			 0xdc27d22b, /// 0x808
			 0xe31412c7, /// 0x80c
			 0x7fd4f406, /// 0x810
			 0x810cc06a, /// 0x814
			 0x5bf93943, /// 0x818
			 0x9b304de1, /// 0x81c
			 0xf5ac4cd6, /// 0x820
			 0xa26667f3, /// 0x824
			 0xb14729ad, /// 0x828
			 0xd6e36a42, /// 0x82c
			 0xec3ea4c0, /// 0x830
			 0x6d3eb84f, /// 0x834
			 0x1b6c0d52, /// 0x838
			 0xed72a694, /// 0x83c
			 0x6234c142, /// 0x840
			 0x17c242a0, /// 0x844
			 0x7eef022f, /// 0x848
			 0xb7625ea7, /// 0x84c
			 0x37e283e8, /// 0x850
			 0x21ce0f66, /// 0x854
			 0x45c905a0, /// 0x858
			 0x766688e7, /// 0x85c
			 0xc19c8414, /// 0x860
			 0xf5e2db72, /// 0x864
			 0x21f2f657, /// 0x868
			 0x8898179e, /// 0x86c
			 0xe69012de, /// 0x870
			 0x9149aa69, /// 0x874
			 0xc35b5d1d, /// 0x878
			 0x35417623, /// 0x87c
			 0xe7455c2b, /// 0x880
			 0x27ca1c16, /// 0x884
			 0xf2f775b6, /// 0x888
			 0x6ef9e29a, /// 0x88c
			 0x66b77aa0, /// 0x890
			 0x72aae249, /// 0x894
			 0x4c51bc10, /// 0x898
			 0xd599519a, /// 0x89c
			 0x6f3fdb47, /// 0x8a0
			 0x195e4280, /// 0x8a4
			 0x2a59a059, /// 0x8a8
			 0xa7f29ecb, /// 0x8ac
			 0x0443556d, /// 0x8b0
			 0xfa9036e5, /// 0x8b4
			 0xce4e2b6e, /// 0x8b8
			 0xe76fffca, /// 0x8bc
			 0x820e28d4, /// 0x8c0
			 0x62d8f8ea, /// 0x8c4
			 0x99c6d390, /// 0x8c8
			 0x0bf218cc, /// 0x8cc
			 0xb67fc251, /// 0x8d0
			 0x8425221b, /// 0x8d4
			 0xd8d95635, /// 0x8d8
			 0xb2dad0c2, /// 0x8dc
			 0x146ec894, /// 0x8e0
			 0x52a62f2e, /// 0x8e4
			 0x98c2df43, /// 0x8e8
			 0xfafeaab9, /// 0x8ec
			 0x078501b4, /// 0x8f0
			 0x9a63bd13, /// 0x8f4
			 0x6b4b082f, /// 0x8f8
			 0xc99393be, /// 0x8fc
			 0x02d20bd4, /// 0x900
			 0xf625482e, /// 0x904
			 0x50234c15, /// 0x908
			 0x68c615d3, /// 0x90c
			 0x884226fa, /// 0x910
			 0xe9005309, /// 0x914
			 0x4a3cbc7c, /// 0x918
			 0xa780ff07, /// 0x91c
			 0xb7da29d3, /// 0x920
			 0xf9944986, /// 0x924
			 0x05d10f82, /// 0x928
			 0x796265ce, /// 0x92c
			 0x2946c8b0, /// 0x930
			 0xc67ac06a, /// 0x934
			 0xf6451716, /// 0x938
			 0xb4510957, /// 0x93c
			 0x4702442c, /// 0x940
			 0x714f221e, /// 0x944
			 0x31e76be4, /// 0x948
			 0x5b18ed99, /// 0x94c
			 0xc24ff799, /// 0x950
			 0x71d7e9d1, /// 0x954
			 0x70cbca14, /// 0x958
			 0xff2049de, /// 0x95c
			 0x0223bdfc, /// 0x960
			 0x51a694df, /// 0x964
			 0xfaec6372, /// 0x968
			 0xf03d59a8, /// 0x96c
			 0xf0d8fd51, /// 0x970
			 0x2b847df4, /// 0x974
			 0x978092b4, /// 0x978
			 0x6d757044, /// 0x97c
			 0x16b415d2, /// 0x980
			 0x9c37e28c, /// 0x984
			 0x01ddde94, /// 0x988
			 0xd13696e3, /// 0x98c
			 0x5d381af8, /// 0x990
			 0x6c346fde, /// 0x994
			 0x3d8d83d7, /// 0x998
			 0x0d118dbc, /// 0x99c
			 0x6f2d8275, /// 0x9a0
			 0x184ad9b3, /// 0x9a4
			 0x2d7aaf5f, /// 0x9a8
			 0x721f2fd8, /// 0x9ac
			 0x524eb36f, /// 0x9b0
			 0x42b64698, /// 0x9b4
			 0x3dfaa325, /// 0x9b8
			 0x35cfa28a, /// 0x9bc
			 0x3ea469a9, /// 0x9c0
			 0x594b8142, /// 0x9c4
			 0x218cf2f1, /// 0x9c8
			 0x3b482c4b, /// 0x9cc
			 0x84a66c16, /// 0x9d0
			 0x7aa85210, /// 0x9d4
			 0x19a6777f, /// 0x9d8
			 0xb8821b17, /// 0x9dc
			 0x2471fd1c, /// 0x9e0
			 0x1c6f608a, /// 0x9e4
			 0x887a5a7d, /// 0x9e8
			 0xb6398d81, /// 0x9ec
			 0xdb0b8bc0, /// 0x9f0
			 0x1062b187, /// 0x9f4
			 0x4acce117, /// 0x9f8
			 0x8e550f06, /// 0x9fc
			 0x79a60e15, /// 0xa00
			 0x359890d8, /// 0xa04
			 0x3222769a, /// 0xa08
			 0x2ef48034, /// 0xa0c
			 0x27eff0c4, /// 0xa10
			 0xe8855eb5, /// 0xa14
			 0x4c51c8d9, /// 0xa18
			 0x1be22cae, /// 0xa1c
			 0x8c44ad79, /// 0xa20
			 0x47196945, /// 0xa24
			 0x822e0929, /// 0xa28
			 0x59372683, /// 0xa2c
			 0x1993fbc8, /// 0xa30
			 0x411ae4c7, /// 0xa34
			 0x379947be, /// 0xa38
			 0xb40e46d4, /// 0xa3c
			 0x73ac3193, /// 0xa40
			 0x91ee2c9d, /// 0xa44
			 0x839d8576, /// 0xa48
			 0xb1a4825d, /// 0xa4c
			 0x0796470c, /// 0xa50
			 0x52f99736, /// 0xa54
			 0x511cce10, /// 0xa58
			 0xc8f5a135, /// 0xa5c
			 0x91d6f731, /// 0xa60
			 0xd84652f5, /// 0xa64
			 0x6ee41689, /// 0xa68
			 0x687c1031, /// 0xa6c
			 0x39fd2f40, /// 0xa70
			 0xacea1e1b, /// 0xa74
			 0xa17fbfa7, /// 0xa78
			 0x531e77f6, /// 0xa7c
			 0xddd2eb64, /// 0xa80
			 0x22c70bf0, /// 0xa84
			 0x13205450, /// 0xa88
			 0x520574f6, /// 0xa8c
			 0x4a6ac058, /// 0xa90
			 0x93f3b428, /// 0xa94
			 0x873441fc, /// 0xa98
			 0x33d176d6, /// 0xa9c
			 0x14e7c515, /// 0xaa0
			 0x7cec7c16, /// 0xaa4
			 0x624b72dc, /// 0xaa8
			 0x12dcd1bf, /// 0xaac
			 0x12ff137f, /// 0xab0
			 0x668a90cc, /// 0xab4
			 0x58ef4173, /// 0xab8
			 0x041460f1, /// 0xabc
			 0x26c9fafa, /// 0xac0
			 0x501be1db, /// 0xac4
			 0xe1d48879, /// 0xac8
			 0x54743f45, /// 0xacc
			 0xebbfab71, /// 0xad0
			 0xc599a1a6, /// 0xad4
			 0xdbddc8b2, /// 0xad8
			 0x7f9dd591, /// 0xadc
			 0xa5cc827b, /// 0xae0
			 0x4deb292c, /// 0xae4
			 0xff7c3755, /// 0xae8
			 0x10731b23, /// 0xaec
			 0x2d674204, /// 0xaf0
			 0xeeb88795, /// 0xaf4
			 0xc8942aad, /// 0xaf8
			 0x48268af6, /// 0xafc
			 0xc0758f39, /// 0xb00
			 0x98977e52, /// 0xb04
			 0xfab841f8, /// 0xb08
			 0xd3412780, /// 0xb0c
			 0x08169e8d, /// 0xb10
			 0xbce1ec3f, /// 0xb14
			 0x9e7508ab, /// 0xb18
			 0xc3be761e, /// 0xb1c
			 0xc2ea84ca, /// 0xb20
			 0x2c8cc8d8, /// 0xb24
			 0x1a4395f7, /// 0xb28
			 0xf5d2a8df, /// 0xb2c
			 0x88f1aa99, /// 0xb30
			 0xb475200a, /// 0xb34
			 0xea097e79, /// 0xb38
			 0xb034a55d, /// 0xb3c
			 0x5a9f5d7f, /// 0xb40
			 0x8aa8cc98, /// 0xb44
			 0xf48d4d33, /// 0xb48
			 0x3aba5132, /// 0xb4c
			 0x1ebd313f, /// 0xb50
			 0x98e52184, /// 0xb54
			 0x3c2f9ba3, /// 0xb58
			 0xa2c5a67a, /// 0xb5c
			 0x1592a8ff, /// 0xb60
			 0x71329c52, /// 0xb64
			 0x0fc55ee7, /// 0xb68
			 0x4580891a, /// 0xb6c
			 0xab390c55, /// 0xb70
			 0x410602d1, /// 0xb74
			 0x1c20e2b1, /// 0xb78
			 0x1e6b8642, /// 0xb7c
			 0xc482c397, /// 0xb80
			 0x9eae5ca3, /// 0xb84
			 0xe5891c83, /// 0xb88
			 0xebf1479b, /// 0xb8c
			 0xa7cd28e6, /// 0xb90
			 0xb0b0540b, /// 0xb94
			 0xee525e1d, /// 0xb98
			 0x3961718c, /// 0xb9c
			 0x8a3e9630, /// 0xba0
			 0xc77afa25, /// 0xba4
			 0x91690e6e, /// 0xba8
			 0x8edec349, /// 0xbac
			 0x5bdd8ae4, /// 0xbb0
			 0x03f493c4, /// 0xbb4
			 0x1c71cf8d, /// 0xbb8
			 0xd3f4cff9, /// 0xbbc
			 0x58f0f848, /// 0xbc0
			 0x7cced6b8, /// 0xbc4
			 0xf49253ba, /// 0xbc8
			 0xd4834d86, /// 0xbcc
			 0xce3ef34d, /// 0xbd0
			 0x43e1abc8, /// 0xbd4
			 0xa2dc1040, /// 0xbd8
			 0xe705b2bc, /// 0xbdc
			 0xad4533d0, /// 0xbe0
			 0x830f8616, /// 0xbe4
			 0x41137c56, /// 0xbe8
			 0xf2912aa2, /// 0xbec
			 0x63f300c2, /// 0xbf0
			 0x1497bdfa, /// 0xbf4
			 0xaec0ec93, /// 0xbf8
			 0xceb4e542, /// 0xbfc
			 0x50a5d3cd, /// 0xc00
			 0xfdb076b1, /// 0xc04
			 0x5b0fc09c, /// 0xc08
			 0xdd89bdb6, /// 0xc0c
			 0xffb3af7a, /// 0xc10
			 0xf59cd253, /// 0xc14
			 0x2db5848b, /// 0xc18
			 0x892f4b34, /// 0xc1c
			 0x96165cd2, /// 0xc20
			 0x872d8ddc, /// 0xc24
			 0xb89795ed, /// 0xc28
			 0x39ef120e, /// 0xc2c
			 0x39ad365a, /// 0xc30
			 0x7b79c603, /// 0xc34
			 0x0639abfb, /// 0xc38
			 0x1635e862, /// 0xc3c
			 0xcbd2ef29, /// 0xc40
			 0x7a74c607, /// 0xc44
			 0x3bef508b, /// 0xc48
			 0x5b1502e7, /// 0xc4c
			 0x9dfec0f7, /// 0xc50
			 0x413a66ec, /// 0xc54
			 0xd829b0a5, /// 0xc58
			 0x6acd85ab, /// 0xc5c
			 0xf66b73c9, /// 0xc60
			 0x5c9ac581, /// 0xc64
			 0x99b68f1b, /// 0xc68
			 0xda91fc35, /// 0xc6c
			 0x8828ba89, /// 0xc70
			 0xbf0fe9b3, /// 0xc74
			 0x0ddff80a, /// 0xc78
			 0xced2d632, /// 0xc7c
			 0x8484e7a4, /// 0xc80
			 0xa2af6a34, /// 0xc84
			 0x8a4f2732, /// 0xc88
			 0x7ed642a0, /// 0xc8c
			 0x9efbebaa, /// 0xc90
			 0xb274fd6c, /// 0xc94
			 0x7760f8ba, /// 0xc98
			 0x4cec210e, /// 0xc9c
			 0x82592fc4, /// 0xca0
			 0xce4b17a1, /// 0xca4
			 0xcf8f2bab, /// 0xca8
			 0x9a42b045, /// 0xcac
			 0x2517342e, /// 0xcb0
			 0xd8debe15, /// 0xcb4
			 0xb2682541, /// 0xcb8
			 0x917743cb, /// 0xcbc
			 0xc9a6a8e5, /// 0xcc0
			 0x3db696c8, /// 0xcc4
			 0x4dcb0a6f, /// 0xcc8
			 0x1fe5ac00, /// 0xccc
			 0x68a7652f, /// 0xcd0
			 0x49da8dd9, /// 0xcd4
			 0x4bcab87e, /// 0xcd8
			 0xe48fb1aa, /// 0xcdc
			 0xaca9218f, /// 0xce0
			 0xc24576cf, /// 0xce4
			 0x189f55a5, /// 0xce8
			 0xd7f5d930, /// 0xcec
			 0x24cf1268, /// 0xcf0
			 0x02b1e7b4, /// 0xcf4
			 0x29060593, /// 0xcf8
			 0xe6d36a76, /// 0xcfc
			 0xa8b9eaf5, /// 0xd00
			 0x2f237d0f, /// 0xd04
			 0x58913bd4, /// 0xd08
			 0x89888f0f, /// 0xd0c
			 0x94015533, /// 0xd10
			 0xe629058d, /// 0xd14
			 0x27abe13c, /// 0xd18
			 0xb9e76442, /// 0xd1c
			 0xf085ff76, /// 0xd20
			 0x854a2659, /// 0xd24
			 0x2ec12dd9, /// 0xd28
			 0xca6b5d54, /// 0xd2c
			 0x3cf4721f, /// 0xd30
			 0xd1f8084a, /// 0xd34
			 0x534bc1ab, /// 0xd38
			 0x8e249c5f, /// 0xd3c
			 0x8e539f48, /// 0xd40
			 0xecee2d2c, /// 0xd44
			 0xbc966abc, /// 0xd48
			 0x4512e4ab, /// 0xd4c
			 0xf29b3c1d, /// 0xd50
			 0x60f29fe9, /// 0xd54
			 0xeb20b257, /// 0xd58
			 0x70c999d0, /// 0xd5c
			 0x149d8297, /// 0xd60
			 0x3a1a90e7, /// 0xd64
			 0x0b66bc48, /// 0xd68
			 0xb69ad219, /// 0xd6c
			 0x97465d52, /// 0xd70
			 0x6a78df56, /// 0xd74
			 0xa4269754, /// 0xd78
			 0x5bc63bf3, /// 0xd7c
			 0xc906fffe, /// 0xd80
			 0x8b864c61, /// 0xd84
			 0x2a9621a0, /// 0xd88
			 0x1f506117, /// 0xd8c
			 0xc13292eb, /// 0xd90
			 0x7c0db08c, /// 0xd94
			 0xd1b27b71, /// 0xd98
			 0x4a313bde, /// 0xd9c
			 0x106867c4, /// 0xda0
			 0x36b59950, /// 0xda4
			 0xd19b6f77, /// 0xda8
			 0x399d56e7, /// 0xdac
			 0x770770c9, /// 0xdb0
			 0x544f83a7, /// 0xdb4
			 0x5480cbd3, /// 0xdb8
			 0xbcab4a06, /// 0xdbc
			 0xabbea055, /// 0xdc0
			 0x555f53e9, /// 0xdc4
			 0x21e6a64c, /// 0xdc8
			 0xfb5ea4ae, /// 0xdcc
			 0x4e16c893, /// 0xdd0
			 0xf7b4f86c, /// 0xdd4
			 0x0389377a, /// 0xdd8
			 0x7c220c2a, /// 0xddc
			 0xf45e2cf8, /// 0xde0
			 0xc3fdfcee, /// 0xde4
			 0x6b65a7b9, /// 0xde8
			 0xf688da11, /// 0xdec
			 0xb910d2d8, /// 0xdf0
			 0xbf815bc2, /// 0xdf4
			 0x18a0044e, /// 0xdf8
			 0xf924fbd5, /// 0xdfc
			 0x96caaff2, /// 0xe00
			 0x4f772994, /// 0xe04
			 0x2160996e, /// 0xe08
			 0xa8874bd7, /// 0xe0c
			 0x05345e9b, /// 0xe10
			 0x96c854f5, /// 0xe14
			 0x106b4e67, /// 0xe18
			 0x5c46829d, /// 0xe1c
			 0x9ecea3a7, /// 0xe20
			 0x835c76d8, /// 0xe24
			 0xf1899a51, /// 0xe28
			 0x68f0c460, /// 0xe2c
			 0x69d50a0e, /// 0xe30
			 0xf87d1b91, /// 0xe34
			 0x3ca9a3d8, /// 0xe38
			 0x6c28cd77, /// 0xe3c
			 0x60e68773, /// 0xe40
			 0x82bab242, /// 0xe44
			 0x3c49765a, /// 0xe48
			 0x4667bc9a, /// 0xe4c
			 0xe4457ddc, /// 0xe50
			 0x67025772, /// 0xe54
			 0x23b64386, /// 0xe58
			 0x9e90a2a6, /// 0xe5c
			 0xa1a7a1b9, /// 0xe60
			 0x22b0d412, /// 0xe64
			 0xe5255cbf, /// 0xe68
			 0xab2cf07c, /// 0xe6c
			 0xd35decb3, /// 0xe70
			 0xff318ce3, /// 0xe74
			 0xae02beeb, /// 0xe78
			 0xb0aad2e4, /// 0xe7c
			 0xc0a54f76, /// 0xe80
			 0xa6438c6a, /// 0xe84
			 0xa1a28565, /// 0xe88
			 0x9e642624, /// 0xe8c
			 0x8d5437da, /// 0xe90
			 0x1152220f, /// 0xe94
			 0xb9cb3980, /// 0xe98
			 0x37eff53b, /// 0xe9c
			 0x18bf5da3, /// 0xea0
			 0xa23a6cda, /// 0xea4
			 0x54390d89, /// 0xea8
			 0x57244498, /// 0xeac
			 0x440979b5, /// 0xeb0
			 0x514ab201, /// 0xeb4
			 0xddd32beb, /// 0xeb8
			 0x280c001a, /// 0xebc
			 0xeaa2390f, /// 0xec0
			 0xb393b55b, /// 0xec4
			 0xecb28ee4, /// 0xec8
			 0x161b15bb, /// 0xecc
			 0xa6147683, /// 0xed0
			 0x90a027cc, /// 0xed4
			 0x1d21c50a, /// 0xed8
			 0x83cecf2e, /// 0xedc
			 0x82a0b7bf, /// 0xee0
			 0x789e7975, /// 0xee4
			 0x09948064, /// 0xee8
			 0xf488c64f, /// 0xeec
			 0x87aea42b, /// 0xef0
			 0x4e471aa1, /// 0xef4
			 0xa0f3510c, /// 0xef8
			 0xdb3bd8c5, /// 0xefc
			 0xce8f8e95, /// 0xf00
			 0xc1c36c14, /// 0xf04
			 0x512f1c74, /// 0xf08
			 0x6ac9ced6, /// 0xf0c
			 0x0a7ea90b, /// 0xf10
			 0x0b154423, /// 0xf14
			 0x3f096dd1, /// 0xf18
			 0xfc483561, /// 0xf1c
			 0xeefaf9ba, /// 0xf20
			 0x3c419156, /// 0xf24
			 0x75a06a57, /// 0xf28
			 0x0803b73d, /// 0xf2c
			 0x772ca05f, /// 0xf30
			 0xc7c1a867, /// 0xf34
			 0x317d8c93, /// 0xf38
			 0x4f8da9fb, /// 0xf3c
			 0xe304dcc9, /// 0xf40
			 0x3af40ce3, /// 0xf44
			 0x4390de2a, /// 0xf48
			 0x512b089d, /// 0xf4c
			 0x18328a58, /// 0xf50
			 0x27df0ebc, /// 0xf54
			 0xc5cb7aee, /// 0xf58
			 0xdfe9421a, /// 0xf5c
			 0x4237544a, /// 0xf60
			 0x21ca0d14, /// 0xf64
			 0x7cc4f492, /// 0xf68
			 0x64e12729, /// 0xf6c
			 0x126f33bf, /// 0xf70
			 0x5c24236a, /// 0xf74
			 0x37228162, /// 0xf78
			 0xf1bf0249, /// 0xf7c
			 0x16417703, /// 0xf80
			 0x57449def, /// 0xf84
			 0x01865685, /// 0xf88
			 0x4d925d6b, /// 0xf8c
			 0x7be77495, /// 0xf90
			 0x195b0691, /// 0xf94
			 0xf3fb2f1e, /// 0xf98
			 0xb7a323e3, /// 0xf9c
			 0xeec23f11, /// 0xfa0
			 0x496a7aba, /// 0xfa4
			 0xd287ecbc, /// 0xfa8
			 0x45b9afd1, /// 0xfac
			 0x054648e7, /// 0xfb0
			 0x6396aa3a, /// 0xfb4
			 0x644b00ab, /// 0xfb8
			 0x36cc0009, /// 0xfbc
			 0xc31fd088, /// 0xfc0
			 0xcf69c889, /// 0xfc4
			 0x9d9a9a56, /// 0xfc8
			 0x4bc1f037, /// 0xfcc
			 0x3ba3419c, /// 0xfd0
			 0x3dd53aaf, /// 0xfd4
			 0x31e27fb7, /// 0xfd8
			 0x165ec50f, /// 0xfdc
			 0x1a74db15, /// 0xfe0
			 0x0ddf9c58, /// 0xfe4
			 0x23bc5841, /// 0xfe8
			 0xb0792e13, /// 0xfec
			 0x1bf2634c, /// 0xff0
			 0xac015731, /// 0xff4
			 0x3385e031, /// 0xff8
			 0xf111a142 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00010
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00018
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0001c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00028
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0002c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00034
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0003c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00040
			 0x7fc00001,                                                  // signaling NaN                               /// 00044
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00048
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0004c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00050
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00054
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0005c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00060
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00064
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0006c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00070
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00074
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00078
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0007c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00080
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00084
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00088
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00094
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00098
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00100
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0e000003,                                                  // Trailing 1s:                                /// 00108
			 0x33333333,                                                  // 4 random values                             /// 0010c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00110
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00114
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x00000000,                                                  // zero                                        /// 0011c
			 0x33333333,                                                  // 4 random values                             /// 00120
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00124
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00128
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0012c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00130
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00138
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0013c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00140
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00144
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0014c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00154
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00158
			 0xbf028f5c,                                                  // -0.51                                       /// 0015c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00160
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00168
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00174
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00178
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00180
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00184
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0018c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00190
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00198
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0019c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00200
			 0xffc00001,                                                  // -signaling NaN                              /// 00204
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00208
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00218
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00220
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00228
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0022c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00230
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00234
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0023c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00244
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00248
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80000000,                                                  // -zero                                       /// 00250
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00264
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0027c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00280
			 0x33333333,                                                  // 4 random values                             /// 00284
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x55555555,                                                  // 4 random values                             /// 0028c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00294
			 0x7fc00001,                                                  // signaling NaN                               /// 00298
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0029c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 002b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 002b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e4
			 0xbf028f5c,                                                  // -0.51                                       /// 002e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 002f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00300
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00304
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00308
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0030c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00318
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0031c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00320
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00324
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0032c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00330
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00338
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00340
			 0x0c780000,                                                  // Leading 1s:                                 /// 00344
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00348
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0034c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00354
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00358
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0035c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00364
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00368
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0036c
			 0x4b000000,                                                  // 8388608.0                                   /// 00370
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0037c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00384
			 0x0c600000,                                                  // Leading 1s:                                 /// 00388
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0038c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00390
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0039c
			 0x7f800000,                                                  // inf                                         /// 003a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00404
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00408
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00410
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00414
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00418
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00420
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00428
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0042c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00430
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0xbf028f5c,                                                  // -0.51                                       /// 00440
			 0x0c600000,                                                  // Leading 1s:                                 /// 00444
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0045c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00460
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00464
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0046c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x7fc00001,                                                  // signaling NaN                               /// 00474
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00480
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0xbf028f5c,                                                  // -0.51                                       /// 00488
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0048c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00494
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00498
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004a8
			 0x7fc00001,                                                  // signaling NaN                               /// 004ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004cc
			 0x80000000,                                                  // -zero                                       /// 004d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00500
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00504
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00508
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x4b000000,                                                  // 8388608.0                                   /// 00510
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00514
			 0x0c780000,                                                  // Leading 1s:                                 /// 00518
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0051c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00520
			 0x3f028f5c,                                                  // 0.51                                        /// 00524
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00528
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00530
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00534
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00538
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00540
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00544
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00548
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0054c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00554
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00558
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0055c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00560
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00564
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00568
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0056c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00570
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0xffc00001,                                                  // -signaling NaN                              /// 0057c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00584
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0058c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00598
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0059c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005bc
			 0xffc00001,                                                  // -signaling NaN                              /// 005c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005c4
			 0x7f800000,                                                  // inf                                         /// 005c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00600
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00604
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00608
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0060c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00610
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0061c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x0e000007,                                                  // Trailing 1s:                                /// 00624
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00628
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00630
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00634
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00644
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00648
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0064c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00650
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00654
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00658
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0065c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00660
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00664
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00668
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00678
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0067c
			 0x33333333,                                                  // 4 random values                             /// 00680
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00688
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00690
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x33333333,                                                  // 4 random values                             /// 00698
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0069c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006cc
			 0x00000000,                                                  // zero                                        /// 006d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006ec
			 0xffc00001,                                                  // -signaling NaN                              /// 006f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006f4
			 0x55555555,                                                  // 4 random values                             /// 006f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00700
			 0x80011111,                                                  // -9.7958E-41                                 /// 00704
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00708
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0070c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x80011111,                                                  // -9.7958E-41                                 /// 00714
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00718
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0071c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00720
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x00011111,                                                  // 9.7958E-41                                  /// 00728
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0072c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0073c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00740
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0074c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00758
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0075c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00760
			 0x00011111,                                                  // 9.7958E-41                                  /// 00764
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00768
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0076c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00770
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00774
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00778
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00784
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00788
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0078c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00790
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00798
			 0x00000000,                                                  // zero                                        /// 0079c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007ac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x7fc00001,                                                  // signaling NaN                               /// 007d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0080c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00814
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00820
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00824
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00830
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00834
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00838
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00844
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0084c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00850
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00854
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00858
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00864
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00868
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0086c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x0e000003,                                                  // Trailing 1s:                                /// 00874
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00880
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00884
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00888
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0088c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e000007,                                                  // Trailing 1s:                                /// 00894
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0089c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00900
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00904
			 0xff800000,                                                  // -inf                                        /// 00908
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0090c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00914
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00918
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00924
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00928
			 0xbf028f5c,                                                  // -0.51                                       /// 0092c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00934
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00938
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0093c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00940
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0094c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00958
			 0x0e000007,                                                  // Trailing 1s:                                /// 0095c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00964
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00968
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0096c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00970
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00974
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00978
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0097c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00984
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00988
			 0x0c400000,                                                  // Leading 1s:                                 /// 0098c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00998
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009c4
			 0xffc00001,                                                  // -signaling NaN                              /// 009c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 009e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009f8
			 0x33333333,                                                  // 4 random values                             /// 009fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a10
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a28
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a4c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a54
			 0xcb000000,                                                  // -8388608.0                                  /// 00a58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00000000,                                                  // zero                                        /// 00a70
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x55555555,                                                  // 4 random values                             /// 00aa4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0xcb000000,                                                  // -8388608.0                                  /// 00aac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ab0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ab4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ab8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ac0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00acc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00adc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ae0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00aec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00af4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00afc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b04
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b38
			 0x4b000000,                                                  // 8388608.0                                   /// 00b3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0xbf028f5c,                                                  // -0.51                                       /// 00b64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b90
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ba0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ba4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bc4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bcc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bdc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00be0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00be8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bf0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80000000,                                                  // -zero                                       /// 00c00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c48
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c9c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ca0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ca4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cb8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x80000000,                                                  // -zero                                       /// 00cd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cdc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ce8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cf8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0xffc00001,                                                  // -signaling NaN                              /// 00d2c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d30
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d48
			 0xff800000,                                                  // -inf                                        /// 00d4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00000000,                                                  // zero                                        /// 00d54
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d58
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d5c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00da0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00da4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00da8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00db0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00db4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00db8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dd0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dd8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ddc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00de4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00de8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00df0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00df4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x33333333,                                                  // 4 random values                             /// 00e04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e08
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e10
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e20
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x00000000,                                                  // zero                                        /// 00e28
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e38
			 0xcb000000,                                                  // -8388608.0                                  /// 00e3c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e7c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e84
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80000000,                                                  // -zero                                       /// 00ea4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0xbf028f5c,                                                  // -0.51                                       /// 00eb0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ec4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ecc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ed0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ed8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00edc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ee8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ef0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ef8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00efc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f08
			 0x4b000000,                                                  // 8388608.0                                   /// 00f0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f18
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f34
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x7f800000,                                                  // inf                                         /// 00f3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f64
			 0x00000000,                                                  // zero                                        /// 00f68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f70
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f98
			 0xcb000000,                                                  // -8388608.0                                  /// 00f9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fa4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fb8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fbc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x4b000000,                                                  // 8388608.0                                   /// 00fc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fd0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fd4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fd8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fe0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fe4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ff0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ff4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ff8
			 0xffc00000                                                  // -cquiet NaN                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x3f028f5c,                                                  // 0.51                                        /// 0000c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00010
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0c780000,                                                  // Leading 1s:                                 /// 0001c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00020
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00024
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00028
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x7fc00001,                                                  // signaling NaN                               /// 00030
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00038
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00040
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00044
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0004c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00050
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00054
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00058
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0005c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00060
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0006c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00070
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00074
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00078
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0007c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00084
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00088
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0008c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00090
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00094
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00098
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0009c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00108
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0010c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00110
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00114
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00118
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0011c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00120
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00128
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0012c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00130
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00134
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00138
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00140
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00144
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80000000,                                                  // -zero                                       /// 0014c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00150
			 0x80000000,                                                  // -zero                                       /// 00154
			 0xcb000000,                                                  // -8388608.0                                  /// 00158
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0015c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00164
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00168
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00170
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00174
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x80011111,                                                  // -9.7958E-41                                 /// 0017c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00180
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00184
			 0x00011111,                                                  // 9.7958E-41                                  /// 00188
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0018c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00194
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x3f028f5c,                                                  // 0.51                                        /// 0019c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001c8
			 0x80000000,                                                  // -zero                                       /// 001cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00204
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00208
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0020c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00210
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00214
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0021c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00224
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00228
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0022c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00234
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00238
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00244
			 0x0c700000,                                                  // Leading 1s:                                 /// 00248
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0024c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00250
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00258
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0025c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0026c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00270
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00278
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0027c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00280
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00288
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0028c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00290
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00298
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x4b000000,                                                  // 8388608.0                                   /// 002c0
			 0xff800000,                                                  // -inf                                        /// 002c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0xff800000,                                                  // -inf                                        /// 002e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00300
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00304
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00310
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00314
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00318
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0031c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00320
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00324
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00328
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00334
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00338
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00344
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00348
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0034c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00350
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00354
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0035c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00360
			 0x0e000001,                                                  // Trailing 1s:                                /// 00364
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00368
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0036c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00370
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00374
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00378
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0037c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00380
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0038c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00390
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00398
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0xffc00001,                                                  // -signaling NaN                              /// 003c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00404
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00408
			 0x0e000001,                                                  // Trailing 1s:                                /// 0040c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00410
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00414
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00418
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00424
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0042c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00430
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00434
			 0x0c780000,                                                  // Leading 1s:                                 /// 00438
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0043c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00440
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00444
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00448
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0044c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00450
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00454
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0045c
			 0x33333333,                                                  // 4 random values                             /// 00460
			 0xff800000,                                                  // -inf                                        /// 00464
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00468
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0046c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00470
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00474
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x80011111,                                                  // -9.7958E-41                                 /// 00488
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0048c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x3f028f5c,                                                  // 0.51                                        /// 00494
			 0x0e000001,                                                  // Trailing 1s:                                /// 00498
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0049c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004bc
			 0x7f800000,                                                  // inf                                         /// 004c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 004f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x00011111,                                                  // 9.7958E-41                                  /// 00508
			 0xff800000,                                                  // -inf                                        /// 0050c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x55555555,                                                  // 4 random values                             /// 00514
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00518
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0051c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00524
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0052c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00530
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00534
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00538
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0053c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00548
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x3f028f5c,                                                  // 0.51                                        /// 00550
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00554
			 0x3f028f5c,                                                  // 0.51                                        /// 00558
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0055c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00560
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0xff800000,                                                  // -inf                                        /// 00568
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0056c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00570
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00574
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0057c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00580
			 0x7fc00001,                                                  // signaling NaN                               /// 00584
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00588
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0058c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00594
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0059c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x3f028f5c,                                                  // 0.51                                        /// 005b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0xbf028f5c,                                                  // -0.51                                       /// 005ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00604
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00608
			 0x3f028f5c,                                                  // 0.51                                        /// 0060c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00614
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0061c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00624
			 0x80000000,                                                  // -zero                                       /// 00628
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0062c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00630
			 0xbf028f5c,                                                  // -0.51                                       /// 00634
			 0x00000000,                                                  // zero                                        /// 00638
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0063c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00644
			 0x00000000,                                                  // zero                                        /// 00648
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00650
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00654
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0065c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00668
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00670
			 0x0c400000,                                                  // Leading 1s:                                 /// 00674
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00678
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0067c
			 0x00000000,                                                  // zero                                        /// 00680
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00688
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00690
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00694
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0069c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 006b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x33333333,                                                  // 4 random values                             /// 006c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006e4
			 0x00000000,                                                  // zero                                        /// 006e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80000000,                                                  // -zero                                       /// 00700
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00704
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00708
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00710
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0071c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00724
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00728
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0072c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00730
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00734
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00738
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0073c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00740
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00744
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00748
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0074c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00750
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00754
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00758
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0075c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00760
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00764
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00768
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00770
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00774
			 0x80000000,                                                  // -zero                                       /// 00778
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0077c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00788
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0078c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00798
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0079c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007b4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00800
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00808
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0080c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00810
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00818
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00824
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0082c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00830
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00838
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0083c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00840
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00848
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0084c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00858
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0085c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00860
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00864
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00868
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0086c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00874
			 0x80011111,                                                  // -9.7958E-41                                 /// 00878
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00880
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00888
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0088c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00890
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0089c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 008b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e0
			 0x7f800000,                                                  // inf                                         /// 008e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00900
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00904
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00908
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0xcb000000,                                                  // -8388608.0                                  /// 00914
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00918
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0091c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00920
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00924
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00928
			 0x0e000003,                                                  // Trailing 1s:                                /// 0092c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x33333333,                                                  // 4 random values                             /// 00934
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0093c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00940
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00944
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0094c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00950
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00954
			 0x0c400000,                                                  // Leading 1s:                                 /// 00958
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0095c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00964
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00968
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0096c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00974
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00978
			 0x0c780000,                                                  // Leading 1s:                                 /// 0097c
			 0xff800000,                                                  // -inf                                        /// 00980
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x55555555,                                                  // 4 random values                             /// 00988
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0098c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00990
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0099c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 009a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009b8
			 0xcb000000,                                                  // -8388608.0                                  /// 009bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x7fc00001,                                                  // signaling NaN                               /// 009e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009f0
			 0xbf028f5c,                                                  // -0.51                                       /// 009f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x4b000000,                                                  // 8388608.0                                   /// 00a08
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a14
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a28
			 0x3f028f5c,                                                  // 0.51                                        /// 00a2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a40
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a5c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aa4
			 0x55555555,                                                  // 4 random values                             /// 00aa8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00aac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ab4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00abc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ac0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ac4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ad4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00adc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ae0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ae8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00aec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00af4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00afc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80000000,                                                  // -zero                                       /// 00b58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b64
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b74
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b88
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b90
			 0x3f028f5c,                                                  // 0.51                                        /// 00b94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ba0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ba4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ba8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bb0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bb8
			 0x80000000,                                                  // -zero                                       /// 00bbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bcc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bd4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bdc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00be0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bf0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c10
			 0xbf028f5c,                                                  // -0.51                                       /// 00c14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c28
			 0x33333333,                                                  // 4 random values                             /// 00c2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c58
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c6c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c8c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ca0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cac
			 0x80000000,                                                  // -zero                                       /// 00cb0
			 0x3f028f5c,                                                  // 0.51                                        /// 00cb4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cc8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ce0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ce4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cfc
			 0x7fc00001,                                                  // signaling NaN                               /// 00d00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d2c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80000000,                                                  // -zero                                       /// 00d54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d74
			 0x3f028f5c,                                                  // 0.51                                        /// 00d78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00da0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00da8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00db0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00db4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00db8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dc0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dd0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00de0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00de4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00df4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00df8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dfc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x7f800000,                                                  // inf                                         /// 00e30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e58
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e6c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e70
			 0xffc00001,                                                  // -signaling NaN                              /// 00e74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x7f800000,                                                  // inf                                         /// 00e90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ea0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ea4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eb0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eb4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x00000000,                                                  // zero                                        /// 00ec4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ed0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ed4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ed8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00edc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ee0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ee8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00eec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ef4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ef8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00efc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f08
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f4c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f54
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f64
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f68
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f94
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fa0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x7f800000,                                                  // inf                                         /// 00fac
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fc0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fc4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fc8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fcc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fe0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fe8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ff4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ff8
			 0x0c7f8000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xa529acb6, /// 0x0
			 0x6f36d709, /// 0x4
			 0xf4511fc0, /// 0x8
			 0xae51e06d, /// 0xc
			 0x1fb5447b, /// 0x10
			 0xf48a4a6b, /// 0x14
			 0xa9f03588, /// 0x18
			 0xff1895c7, /// 0x1c
			 0x7c42ad50, /// 0x20
			 0xacb688a0, /// 0x24
			 0xb97c0e3d, /// 0x28
			 0xfe259357, /// 0x2c
			 0x9659706a, /// 0x30
			 0xa98a1acd, /// 0x34
			 0xb82df71d, /// 0x38
			 0x905afcc3, /// 0x3c
			 0xeebcc410, /// 0x40
			 0x077c836d, /// 0x44
			 0xc5ad91b4, /// 0x48
			 0xffa78463, /// 0x4c
			 0x5231ccf6, /// 0x50
			 0xcfd64600, /// 0x54
			 0x60baf398, /// 0x58
			 0x9c6a2e3c, /// 0x5c
			 0x0536be6d, /// 0x60
			 0x8fdba434, /// 0x64
			 0x6a43c216, /// 0x68
			 0x1f0c2b41, /// 0x6c
			 0xdb79182e, /// 0x70
			 0xeb90e7f8, /// 0x74
			 0x20546e2e, /// 0x78
			 0xf8be0cf3, /// 0x7c
			 0x4f73f06e, /// 0x80
			 0x33022712, /// 0x84
			 0xd7412ceb, /// 0x88
			 0xeeedb71d, /// 0x8c
			 0xacaceb97, /// 0x90
			 0xfa953b87, /// 0x94
			 0xa29794c7, /// 0x98
			 0xbf2f6315, /// 0x9c
			 0x0ee249bd, /// 0xa0
			 0xef6742fd, /// 0xa4
			 0xa91a0053, /// 0xa8
			 0x85f02d78, /// 0xac
			 0x7fb74e70, /// 0xb0
			 0x575e1733, /// 0xb4
			 0x84b2acd8, /// 0xb8
			 0x943b98c1, /// 0xbc
			 0x567338b1, /// 0xc0
			 0x2ff280d1, /// 0xc4
			 0xbb7e21e8, /// 0xc8
			 0xa67d2284, /// 0xcc
			 0x166263e6, /// 0xd0
			 0xe16d85b6, /// 0xd4
			 0xd19a53e2, /// 0xd8
			 0x9ba89ae2, /// 0xdc
			 0x82df5345, /// 0xe0
			 0x7137243b, /// 0xe4
			 0x1de65846, /// 0xe8
			 0x3c5424b5, /// 0xec
			 0x074b8b3e, /// 0xf0
			 0x70e4cdd1, /// 0xf4
			 0xbcd6d2cd, /// 0xf8
			 0x1a8f0637, /// 0xfc
			 0xfb876266, /// 0x100
			 0xf88ed6f9, /// 0x104
			 0x3cda03ba, /// 0x108
			 0x966ef06b, /// 0x10c
			 0x75564c04, /// 0x110
			 0xd378e5fc, /// 0x114
			 0xa3a55873, /// 0x118
			 0x53496c5e, /// 0x11c
			 0x88a9ebf5, /// 0x120
			 0xe23094ca, /// 0x124
			 0x77c90eb6, /// 0x128
			 0x90cd87fb, /// 0x12c
			 0x5c77733d, /// 0x130
			 0xedc3ea2a, /// 0x134
			 0x0753a5ac, /// 0x138
			 0x3b0bdc04, /// 0x13c
			 0xdffe5a46, /// 0x140
			 0x41d4a945, /// 0x144
			 0x46981fa9, /// 0x148
			 0xac3fa225, /// 0x14c
			 0xad4aad43, /// 0x150
			 0x52283966, /// 0x154
			 0xcc887041, /// 0x158
			 0x5001458d, /// 0x15c
			 0xea234876, /// 0x160
			 0x5f258e2a, /// 0x164
			 0x3ebcc09a, /// 0x168
			 0xf97d3e40, /// 0x16c
			 0x58073405, /// 0x170
			 0x200d006d, /// 0x174
			 0xa0c451e9, /// 0x178
			 0x77ec1a59, /// 0x17c
			 0x551a5dad, /// 0x180
			 0x2ac70b64, /// 0x184
			 0x9700eab3, /// 0x188
			 0xc2bd435e, /// 0x18c
			 0xbcacbab3, /// 0x190
			 0x22afe682, /// 0x194
			 0x1a3dc653, /// 0x198
			 0xf4be5df9, /// 0x19c
			 0x43661de0, /// 0x1a0
			 0xa4c94ebf, /// 0x1a4
			 0x3c379df3, /// 0x1a8
			 0x1bc3bc2b, /// 0x1ac
			 0xd1436bad, /// 0x1b0
			 0x103a4578, /// 0x1b4
			 0xfac06181, /// 0x1b8
			 0x0039dc89, /// 0x1bc
			 0x88da65cf, /// 0x1c0
			 0xfe0f9356, /// 0x1c4
			 0x6f22072a, /// 0x1c8
			 0x683f86c4, /// 0x1cc
			 0x3f198171, /// 0x1d0
			 0x8f6ce8e3, /// 0x1d4
			 0x20c7dcb1, /// 0x1d8
			 0x52a94901, /// 0x1dc
			 0x6a8c1409, /// 0x1e0
			 0x91f76dce, /// 0x1e4
			 0xfd5cdfdd, /// 0x1e8
			 0x8e1585f3, /// 0x1ec
			 0xf0e65999, /// 0x1f0
			 0x634710b4, /// 0x1f4
			 0xfd3f5b80, /// 0x1f8
			 0x838b4c91, /// 0x1fc
			 0x7667bf42, /// 0x200
			 0xe91fd10f, /// 0x204
			 0x184e8663, /// 0x208
			 0x04aaf095, /// 0x20c
			 0xc87dd560, /// 0x210
			 0x7c030d07, /// 0x214
			 0x6731a718, /// 0x218
			 0x295b82a3, /// 0x21c
			 0x33cdd9ae, /// 0x220
			 0x39c581fa, /// 0x224
			 0x35887f89, /// 0x228
			 0xc34c1b15, /// 0x22c
			 0xe2bfd4a7, /// 0x230
			 0xebbbeb61, /// 0x234
			 0x242fc60f, /// 0x238
			 0xc4af3677, /// 0x23c
			 0x146525e5, /// 0x240
			 0x31ae0e6b, /// 0x244
			 0x028adabb, /// 0x248
			 0xd26ab7ea, /// 0x24c
			 0x16ad7055, /// 0x250
			 0x9e5c69a6, /// 0x254
			 0xbfe9cab8, /// 0x258
			 0xc41a5bf6, /// 0x25c
			 0xa8f2ab11, /// 0x260
			 0x45a579b7, /// 0x264
			 0x2fdbccf4, /// 0x268
			 0x73205df6, /// 0x26c
			 0x4e602ff5, /// 0x270
			 0x184cea3d, /// 0x274
			 0xa236312c, /// 0x278
			 0x269f3f31, /// 0x27c
			 0xeead9acb, /// 0x280
			 0xbebdd6c6, /// 0x284
			 0x1be82b04, /// 0x288
			 0x13eac0c1, /// 0x28c
			 0x491c263c, /// 0x290
			 0xab6a37bc, /// 0x294
			 0xc4a39145, /// 0x298
			 0xf6acaf44, /// 0x29c
			 0xa75da935, /// 0x2a0
			 0x735523cf, /// 0x2a4
			 0xc5418fa8, /// 0x2a8
			 0x3f6a7d77, /// 0x2ac
			 0x37240e8e, /// 0x2b0
			 0x45a741ba, /// 0x2b4
			 0x1cca395b, /// 0x2b8
			 0x01267a7f, /// 0x2bc
			 0xde215735, /// 0x2c0
			 0x8df8b004, /// 0x2c4
			 0xea255ec5, /// 0x2c8
			 0x5073dc91, /// 0x2cc
			 0x5f91e9ad, /// 0x2d0
			 0x715f1ab2, /// 0x2d4
			 0xb6b8d262, /// 0x2d8
			 0xf9c23285, /// 0x2dc
			 0x742438fa, /// 0x2e0
			 0xf98b8e4d, /// 0x2e4
			 0x7c95cdf0, /// 0x2e8
			 0xc7adc3f4, /// 0x2ec
			 0xdda042fb, /// 0x2f0
			 0xda3b68d7, /// 0x2f4
			 0x12783876, /// 0x2f8
			 0x93c4ff5d, /// 0x2fc
			 0x3941260a, /// 0x300
			 0x2be260c3, /// 0x304
			 0x8ebca478, /// 0x308
			 0xb5ef326a, /// 0x30c
			 0xdf93aec3, /// 0x310
			 0x548675fc, /// 0x314
			 0x59b0533b, /// 0x318
			 0xddedc63a, /// 0x31c
			 0xc1195084, /// 0x320
			 0xb8081bf3, /// 0x324
			 0x178c5749, /// 0x328
			 0x392e884c, /// 0x32c
			 0x1ca1d4b7, /// 0x330
			 0x7935cf92, /// 0x334
			 0x152074d8, /// 0x338
			 0x9f25cb7e, /// 0x33c
			 0x571fc142, /// 0x340
			 0x69611139, /// 0x344
			 0xbd252591, /// 0x348
			 0xab0169d8, /// 0x34c
			 0xbfb8e101, /// 0x350
			 0x2188bde6, /// 0x354
			 0x1d0b709e, /// 0x358
			 0xdd11dfc8, /// 0x35c
			 0xa9f5ef42, /// 0x360
			 0xf998a626, /// 0x364
			 0x65cb6698, /// 0x368
			 0x828be5d0, /// 0x36c
			 0x9276b9ac, /// 0x370
			 0xb02b2b59, /// 0x374
			 0x6d23e84a, /// 0x378
			 0x7a0d640d, /// 0x37c
			 0x6bab37d2, /// 0x380
			 0x76b3290b, /// 0x384
			 0x589081a4, /// 0x388
			 0x3b30a21d, /// 0x38c
			 0x39605779, /// 0x390
			 0x9e97d894, /// 0x394
			 0x0c78791d, /// 0x398
			 0x62279357, /// 0x39c
			 0x0c310141, /// 0x3a0
			 0x4a40eb22, /// 0x3a4
			 0x120a232e, /// 0x3a8
			 0x5f1d2f16, /// 0x3ac
			 0xe827ed90, /// 0x3b0
			 0xfa60b37c, /// 0x3b4
			 0x35efccf9, /// 0x3b8
			 0x61a6cfcd, /// 0x3bc
			 0xb7a242bc, /// 0x3c0
			 0x09737ce3, /// 0x3c4
			 0x70541552, /// 0x3c8
			 0x3d4f75cd, /// 0x3cc
			 0x7707d580, /// 0x3d0
			 0x9f1e385a, /// 0x3d4
			 0x6f8fd8e0, /// 0x3d8
			 0x20ac2bcb, /// 0x3dc
			 0x0e6702aa, /// 0x3e0
			 0x8b3ca9f7, /// 0x3e4
			 0xad5476da, /// 0x3e8
			 0xc6d8b9e4, /// 0x3ec
			 0x3ad0959f, /// 0x3f0
			 0x6789259f, /// 0x3f4
			 0x4677076c, /// 0x3f8
			 0xbab335f4, /// 0x3fc
			 0xa3ca7912, /// 0x400
			 0xa8c22580, /// 0x404
			 0x6f69b05f, /// 0x408
			 0xa6cfd4fc, /// 0x40c
			 0x1f23c6b2, /// 0x410
			 0xad6705f0, /// 0x414
			 0xe0c2a066, /// 0x418
			 0x0580d2d8, /// 0x41c
			 0x8a384170, /// 0x420
			 0x30c7cf6b, /// 0x424
			 0x57ef8766, /// 0x428
			 0x01f9374e, /// 0x42c
			 0xafee4914, /// 0x430
			 0xff1d87e7, /// 0x434
			 0x3c5f59cc, /// 0x438
			 0x09b28184, /// 0x43c
			 0xd9a7c86a, /// 0x440
			 0x116bfc19, /// 0x444
			 0x9025d2a7, /// 0x448
			 0x2966b8ad, /// 0x44c
			 0x35123645, /// 0x450
			 0xfe0eec0f, /// 0x454
			 0xdc7a9902, /// 0x458
			 0xa47cf70f, /// 0x45c
			 0x49140379, /// 0x460
			 0xc6d507d8, /// 0x464
			 0x232d5d1d, /// 0x468
			 0xc0da4d56, /// 0x46c
			 0x6ecd842d, /// 0x470
			 0xcc3f82ef, /// 0x474
			 0x4427e5c7, /// 0x478
			 0x4b3c871e, /// 0x47c
			 0x0456958c, /// 0x480
			 0xedff3e3e, /// 0x484
			 0x0d353a1b, /// 0x488
			 0x294a2b9e, /// 0x48c
			 0x5662738b, /// 0x490
			 0x22133716, /// 0x494
			 0x5c1b37ba, /// 0x498
			 0x821dcf69, /// 0x49c
			 0x747434fc, /// 0x4a0
			 0xc6868697, /// 0x4a4
			 0x1dfe1943, /// 0x4a8
			 0x8c2ca879, /// 0x4ac
			 0xc519f232, /// 0x4b0
			 0x7cd7670a, /// 0x4b4
			 0xbc661474, /// 0x4b8
			 0x5cc57e61, /// 0x4bc
			 0x7e0827b7, /// 0x4c0
			 0xe56380e5, /// 0x4c4
			 0xce6cce5f, /// 0x4c8
			 0xe4f8dbb1, /// 0x4cc
			 0x8bbcb7ec, /// 0x4d0
			 0xee626912, /// 0x4d4
			 0xe1e549cb, /// 0x4d8
			 0x9a700e4c, /// 0x4dc
			 0x04a37b8d, /// 0x4e0
			 0x6d4ed181, /// 0x4e4
			 0x78a6ee0c, /// 0x4e8
			 0x979bfc5b, /// 0x4ec
			 0x3ba00324, /// 0x4f0
			 0x0b10b741, /// 0x4f4
			 0x2b867c68, /// 0x4f8
			 0x8e0b4df3, /// 0x4fc
			 0x95d013c5, /// 0x500
			 0xb23bc10b, /// 0x504
			 0xc7ea6e7e, /// 0x508
			 0xe33e24e0, /// 0x50c
			 0xb9fd943b, /// 0x510
			 0x4bcd6ebc, /// 0x514
			 0x096c3e66, /// 0x518
			 0x22a731dc, /// 0x51c
			 0xa420074f, /// 0x520
			 0x775e9dbb, /// 0x524
			 0x75666598, /// 0x528
			 0x501ab226, /// 0x52c
			 0x6eb9a17c, /// 0x530
			 0xb2dab896, /// 0x534
			 0x599a8185, /// 0x538
			 0x4e64b0b2, /// 0x53c
			 0x97cf7e3e, /// 0x540
			 0x3928c815, /// 0x544
			 0x5ad07ef7, /// 0x548
			 0x4c818627, /// 0x54c
			 0x44002e7d, /// 0x550
			 0x577fef8f, /// 0x554
			 0x0892c629, /// 0x558
			 0x1193ef8a, /// 0x55c
			 0x060c63a7, /// 0x560
			 0x6366a7e8, /// 0x564
			 0xf8c27520, /// 0x568
			 0x6765bf81, /// 0x56c
			 0x5ccecc35, /// 0x570
			 0x20092563, /// 0x574
			 0x3ff4ea90, /// 0x578
			 0xcdbb27d7, /// 0x57c
			 0x868e58b1, /// 0x580
			 0x939416ff, /// 0x584
			 0xb8134e8f, /// 0x588
			 0xd09bad5f, /// 0x58c
			 0x3346d593, /// 0x590
			 0x1a0f22ea, /// 0x594
			 0x3f931f4a, /// 0x598
			 0x3f363369, /// 0x59c
			 0x2b66053e, /// 0x5a0
			 0xe1e639c4, /// 0x5a4
			 0xfa304183, /// 0x5a8
			 0xacb46331, /// 0x5ac
			 0x175b5e21, /// 0x5b0
			 0xfa05b824, /// 0x5b4
			 0x7245a336, /// 0x5b8
			 0xa093c0a9, /// 0x5bc
			 0x18b83d14, /// 0x5c0
			 0xcd923735, /// 0x5c4
			 0xe5f300ea, /// 0x5c8
			 0xed5eb56b, /// 0x5cc
			 0x1e15e676, /// 0x5d0
			 0xc29d0e37, /// 0x5d4
			 0x7f5d4d55, /// 0x5d8
			 0xd6259a8d, /// 0x5dc
			 0x25ae75f1, /// 0x5e0
			 0x067a996b, /// 0x5e4
			 0x444959b4, /// 0x5e8
			 0xd1c8da99, /// 0x5ec
			 0x9be3d652, /// 0x5f0
			 0xeb82dd0b, /// 0x5f4
			 0x109126be, /// 0x5f8
			 0x4e5e01da, /// 0x5fc
			 0x64faae90, /// 0x600
			 0xccf85ec8, /// 0x604
			 0xa155264a, /// 0x608
			 0x6e4ab3f7, /// 0x60c
			 0x0d72c055, /// 0x610
			 0x6dcffa1e, /// 0x614
			 0x13cb0aed, /// 0x618
			 0x93ab669c, /// 0x61c
			 0x8200533e, /// 0x620
			 0x82da6629, /// 0x624
			 0xc89d8d69, /// 0x628
			 0x60d8a962, /// 0x62c
			 0xdfeb43a2, /// 0x630
			 0x2998ca42, /// 0x634
			 0x2af38fba, /// 0x638
			 0x83049e48, /// 0x63c
			 0xbde183af, /// 0x640
			 0xb3e0c2d7, /// 0x644
			 0x62284ce5, /// 0x648
			 0xfe9a2508, /// 0x64c
			 0xe8588ec4, /// 0x650
			 0xad28df65, /// 0x654
			 0x52a44e1c, /// 0x658
			 0x5c72ef56, /// 0x65c
			 0x29545589, /// 0x660
			 0x7375ee55, /// 0x664
			 0x86faf00f, /// 0x668
			 0x160faf7e, /// 0x66c
			 0x54e7505b, /// 0x670
			 0x101c0b3b, /// 0x674
			 0x942c2006, /// 0x678
			 0x2ee24521, /// 0x67c
			 0x056d52bd, /// 0x680
			 0x3df772a2, /// 0x684
			 0x23c52dc3, /// 0x688
			 0x9ca463a7, /// 0x68c
			 0x27a3b755, /// 0x690
			 0x4c1fd750, /// 0x694
			 0x76e73355, /// 0x698
			 0x23311be8, /// 0x69c
			 0x93b22e6a, /// 0x6a0
			 0x251bd078, /// 0x6a4
			 0x1e1851ab, /// 0x6a8
			 0x9aaaec65, /// 0x6ac
			 0x053eb254, /// 0x6b0
			 0xf5f2824a, /// 0x6b4
			 0x07e8d02a, /// 0x6b8
			 0x35bf8f8d, /// 0x6bc
			 0x244069c7, /// 0x6c0
			 0x1e382502, /// 0x6c4
			 0x4579a824, /// 0x6c8
			 0xc5fc407c, /// 0x6cc
			 0x31ae81f4, /// 0x6d0
			 0x7d0e8d05, /// 0x6d4
			 0x1b4318f5, /// 0x6d8
			 0x54c9518d, /// 0x6dc
			 0x919b0987, /// 0x6e0
			 0x079bd393, /// 0x6e4
			 0x47f50e70, /// 0x6e8
			 0x5c0e380d, /// 0x6ec
			 0x3cc882f4, /// 0x6f0
			 0xc962bfae, /// 0x6f4
			 0xbc70036d, /// 0x6f8
			 0xc1deb4d6, /// 0x6fc
			 0xaa05bc6d, /// 0x700
			 0x4762e548, /// 0x704
			 0xabbd04a8, /// 0x708
			 0x3097a1a0, /// 0x70c
			 0x295f82cb, /// 0x710
			 0x6754ad7b, /// 0x714
			 0xfb0ad6aa, /// 0x718
			 0x2f498d34, /// 0x71c
			 0x141e89ca, /// 0x720
			 0xb737f857, /// 0x724
			 0x02f00889, /// 0x728
			 0xfaa4fd8a, /// 0x72c
			 0xa8761b8d, /// 0x730
			 0xe7d1bdd9, /// 0x734
			 0x6c7db4df, /// 0x738
			 0xf275968c, /// 0x73c
			 0x4850d874, /// 0x740
			 0xbc7f6be5, /// 0x744
			 0xda6e41d1, /// 0x748
			 0x97216841, /// 0x74c
			 0x4a0c3170, /// 0x750
			 0x65ed9902, /// 0x754
			 0x6222a1a0, /// 0x758
			 0x5ed5776e, /// 0x75c
			 0x2dd4ba38, /// 0x760
			 0x9de53a39, /// 0x764
			 0x7c271f9c, /// 0x768
			 0x05bdb87e, /// 0x76c
			 0xccea661c, /// 0x770
			 0x917bafb7, /// 0x774
			 0x044f49d5, /// 0x778
			 0xcf0ea80b, /// 0x77c
			 0xaf738b52, /// 0x780
			 0xea6b61f6, /// 0x784
			 0x71bf4a51, /// 0x788
			 0x2432dbc8, /// 0x78c
			 0xbadd1e39, /// 0x790
			 0x76485971, /// 0x794
			 0xfa115886, /// 0x798
			 0x715e0697, /// 0x79c
			 0x347b763e, /// 0x7a0
			 0xf9137e02, /// 0x7a4
			 0x4ac7ea28, /// 0x7a8
			 0xe438e563, /// 0x7ac
			 0x88aba3ac, /// 0x7b0
			 0x716273ca, /// 0x7b4
			 0x31779531, /// 0x7b8
			 0xf6b7db4b, /// 0x7bc
			 0x18a5253f, /// 0x7c0
			 0x675b105d, /// 0x7c4
			 0x48e3d8bf, /// 0x7c8
			 0x7690d0b8, /// 0x7cc
			 0x08d925e1, /// 0x7d0
			 0x3924b54c, /// 0x7d4
			 0xb5f0b5b6, /// 0x7d8
			 0x0eb353b3, /// 0x7dc
			 0x930080eb, /// 0x7e0
			 0xc4f2b4a9, /// 0x7e4
			 0x955b8ad4, /// 0x7e8
			 0xaf4a243c, /// 0x7ec
			 0x078be0a6, /// 0x7f0
			 0xb9c83de5, /// 0x7f4
			 0xa6972fb9, /// 0x7f8
			 0xab9bcdfd, /// 0x7fc
			 0xadb7c0f5, /// 0x800
			 0xfe366907, /// 0x804
			 0xa5482467, /// 0x808
			 0x2b609fb4, /// 0x80c
			 0x4cef2177, /// 0x810
			 0x0628db7e, /// 0x814
			 0xfe750c76, /// 0x818
			 0x24da53a1, /// 0x81c
			 0x9d3408c5, /// 0x820
			 0xaf0972dc, /// 0x824
			 0xf18bced8, /// 0x828
			 0x1adada72, /// 0x82c
			 0xdf8d23a9, /// 0x830
			 0xd7a552f5, /// 0x834
			 0x3b0bc241, /// 0x838
			 0xfc134d49, /// 0x83c
			 0x5d3d01f0, /// 0x840
			 0x7d8cfae1, /// 0x844
			 0x932bbdbe, /// 0x848
			 0x4268e0ab, /// 0x84c
			 0xcf841056, /// 0x850
			 0x45b5ade5, /// 0x854
			 0xa99477cd, /// 0x858
			 0x0e93ecb4, /// 0x85c
			 0x915b95d4, /// 0x860
			 0x855e1246, /// 0x864
			 0xae9248bf, /// 0x868
			 0xfbdef206, /// 0x86c
			 0x25edbe7c, /// 0x870
			 0x04b1deef, /// 0x874
			 0x9b662615, /// 0x878
			 0x5e80ac6d, /// 0x87c
			 0x3a5d56b6, /// 0x880
			 0xb16b2820, /// 0x884
			 0x49801a3b, /// 0x888
			 0xad5bdeea, /// 0x88c
			 0xac5d94c8, /// 0x890
			 0x2c661940, /// 0x894
			 0x8ce83b6d, /// 0x898
			 0x89dbef04, /// 0x89c
			 0xe8f5b973, /// 0x8a0
			 0x88b35d75, /// 0x8a4
			 0xed8c0ad5, /// 0x8a8
			 0x0e88858f, /// 0x8ac
			 0x27055e77, /// 0x8b0
			 0xbe07500f, /// 0x8b4
			 0x9dc7952a, /// 0x8b8
			 0x2353b705, /// 0x8bc
			 0x89fbab78, /// 0x8c0
			 0xa8034b64, /// 0x8c4
			 0x89911043, /// 0x8c8
			 0x53171ad1, /// 0x8cc
			 0x8f38895a, /// 0x8d0
			 0xf8a160ee, /// 0x8d4
			 0x2a487eec, /// 0x8d8
			 0xf33c7aa5, /// 0x8dc
			 0xcbe65299, /// 0x8e0
			 0xba0526f0, /// 0x8e4
			 0x77aa2141, /// 0x8e8
			 0x466e7571, /// 0x8ec
			 0x580d7984, /// 0x8f0
			 0x08b32e75, /// 0x8f4
			 0x23b12023, /// 0x8f8
			 0xda4e7c73, /// 0x8fc
			 0xa921eb11, /// 0x900
			 0xbade7c9c, /// 0x904
			 0x1dcac967, /// 0x908
			 0x72419f68, /// 0x90c
			 0x9e490ad3, /// 0x910
			 0xa211ed7a, /// 0x914
			 0x5516d194, /// 0x918
			 0x0b99731f, /// 0x91c
			 0x37d17a22, /// 0x920
			 0xd097e019, /// 0x924
			 0x00579cc1, /// 0x928
			 0x71aabaf8, /// 0x92c
			 0xb8576f29, /// 0x930
			 0x16f5be55, /// 0x934
			 0x3c0a10c3, /// 0x938
			 0xc4c2ca5d, /// 0x93c
			 0x6312fb65, /// 0x940
			 0x8a4b4b8a, /// 0x944
			 0xefe491bc, /// 0x948
			 0xe12ef098, /// 0x94c
			 0xaf7239c3, /// 0x950
			 0x7d4017b9, /// 0x954
			 0xdc6519f5, /// 0x958
			 0x71fe0a35, /// 0x95c
			 0x15b2138f, /// 0x960
			 0x3a607796, /// 0x964
			 0x7a43acfb, /// 0x968
			 0xf20068e4, /// 0x96c
			 0x6ebbf8a4, /// 0x970
			 0x682b0fc5, /// 0x974
			 0x9ac07c81, /// 0x978
			 0xbb2ea043, /// 0x97c
			 0xae1fc5f3, /// 0x980
			 0x61cb496d, /// 0x984
			 0xbd37fc73, /// 0x988
			 0xff380558, /// 0x98c
			 0x3f7c40e4, /// 0x990
			 0xe00a3fa8, /// 0x994
			 0x0abe2e6c, /// 0x998
			 0x10bb9ae1, /// 0x99c
			 0xd4b93cc3, /// 0x9a0
			 0xa7d825ee, /// 0x9a4
			 0xc15f3066, /// 0x9a8
			 0xe6d308b6, /// 0x9ac
			 0x786c7612, /// 0x9b0
			 0x893a72af, /// 0x9b4
			 0x4c52f7ff, /// 0x9b8
			 0x27200bd4, /// 0x9bc
			 0xebe6695c, /// 0x9c0
			 0x7ffd9c05, /// 0x9c4
			 0xc04e1516, /// 0x9c8
			 0xea3842aa, /// 0x9cc
			 0x97d56249, /// 0x9d0
			 0xbfe764f1, /// 0x9d4
			 0x0f0ab6a4, /// 0x9d8
			 0x28a63aa3, /// 0x9dc
			 0x07c82dc0, /// 0x9e0
			 0x7475b83a, /// 0x9e4
			 0x744c93fc, /// 0x9e8
			 0x142818b5, /// 0x9ec
			 0x7c872ec2, /// 0x9f0
			 0x7182eac2, /// 0x9f4
			 0x615cc720, /// 0x9f8
			 0xcbb85c2f, /// 0x9fc
			 0x486be368, /// 0xa00
			 0xec65cd1f, /// 0xa04
			 0xd12c125d, /// 0xa08
			 0xd21984ae, /// 0xa0c
			 0x85af827d, /// 0xa10
			 0x235827c7, /// 0xa14
			 0x3a8834bf, /// 0xa18
			 0x1890f9c6, /// 0xa1c
			 0x68da6fcc, /// 0xa20
			 0xed6b8722, /// 0xa24
			 0x8ab02dc4, /// 0xa28
			 0x72c53405, /// 0xa2c
			 0x46d7e42d, /// 0xa30
			 0x3d46c9f1, /// 0xa34
			 0x259fa002, /// 0xa38
			 0x63bc0e9d, /// 0xa3c
			 0x2b6b7ca4, /// 0xa40
			 0x6d79821e, /// 0xa44
			 0x498b0798, /// 0xa48
			 0xaebb78ad, /// 0xa4c
			 0x4a62c33c, /// 0xa50
			 0x3bc263d0, /// 0xa54
			 0x094a841f, /// 0xa58
			 0x675e4e4b, /// 0xa5c
			 0xacaf4d6a, /// 0xa60
			 0x577db5e7, /// 0xa64
			 0xf4817bd3, /// 0xa68
			 0x3ce4b4b0, /// 0xa6c
			 0xa090d476, /// 0xa70
			 0xeab5d218, /// 0xa74
			 0x58aef1b8, /// 0xa78
			 0x9de1ed9a, /// 0xa7c
			 0x03c63f9c, /// 0xa80
			 0x2f6e394e, /// 0xa84
			 0x00e72ccb, /// 0xa88
			 0x49a87623, /// 0xa8c
			 0xa039e5d5, /// 0xa90
			 0xf246a316, /// 0xa94
			 0xbcb9ac7f, /// 0xa98
			 0x39b4ae7e, /// 0xa9c
			 0x27b5080d, /// 0xaa0
			 0x8ce84385, /// 0xaa4
			 0xab5bcad7, /// 0xaa8
			 0x9192d042, /// 0xaac
			 0xb8049258, /// 0xab0
			 0x6a7c1f8a, /// 0xab4
			 0x6083b40f, /// 0xab8
			 0x1af8ae02, /// 0xabc
			 0x5ec220ca, /// 0xac0
			 0xd1be8227, /// 0xac4
			 0xd8a7ab0b, /// 0xac8
			 0x96e5dfaf, /// 0xacc
			 0xc0679c34, /// 0xad0
			 0xc5e2bb97, /// 0xad4
			 0xd0e31798, /// 0xad8
			 0x6c0ccac0, /// 0xadc
			 0x71265fb4, /// 0xae0
			 0x9266bf15, /// 0xae4
			 0xa9e9bc14, /// 0xae8
			 0x87b2a18d, /// 0xaec
			 0xa108b420, /// 0xaf0
			 0x713f5f5a, /// 0xaf4
			 0xfb785fa4, /// 0xaf8
			 0xa5527e0d, /// 0xafc
			 0x1ca7ec2f, /// 0xb00
			 0x007385bc, /// 0xb04
			 0x5d149009, /// 0xb08
			 0x59db2d47, /// 0xb0c
			 0xa02986b0, /// 0xb10
			 0x070e93c7, /// 0xb14
			 0xd5e4adae, /// 0xb18
			 0xc46acfc0, /// 0xb1c
			 0x1ab03204, /// 0xb20
			 0x61d6cedf, /// 0xb24
			 0x2cb66673, /// 0xb28
			 0x25625b21, /// 0xb2c
			 0x60ee968d, /// 0xb30
			 0x11d1991d, /// 0xb34
			 0xbb6b2cec, /// 0xb38
			 0xd3f926cf, /// 0xb3c
			 0x4015cb5c, /// 0xb40
			 0x22313fad, /// 0xb44
			 0xd5344cb9, /// 0xb48
			 0xd8f8972d, /// 0xb4c
			 0xe9f738d3, /// 0xb50
			 0xa72db0f6, /// 0xb54
			 0xec9a3791, /// 0xb58
			 0x3b3fb402, /// 0xb5c
			 0x9a8d5ca6, /// 0xb60
			 0xcacc00ad, /// 0xb64
			 0xbca1b5c1, /// 0xb68
			 0xbba7a875, /// 0xb6c
			 0x216acb0b, /// 0xb70
			 0x28321699, /// 0xb74
			 0xc2e46248, /// 0xb78
			 0x15aff1f9, /// 0xb7c
			 0x23ca959c, /// 0xb80
			 0x617eda29, /// 0xb84
			 0xec98b4e9, /// 0xb88
			 0xa9e9368e, /// 0xb8c
			 0xe106d2c1, /// 0xb90
			 0xb276aaf2, /// 0xb94
			 0xf2d3d4f3, /// 0xb98
			 0xa01fdbb8, /// 0xb9c
			 0xaa74540a, /// 0xba0
			 0xfc41ecd9, /// 0xba4
			 0xee9652e5, /// 0xba8
			 0xcf06dac8, /// 0xbac
			 0xb2289041, /// 0xbb0
			 0xbcad7a2c, /// 0xbb4
			 0xd4578a82, /// 0xbb8
			 0x664583d9, /// 0xbbc
			 0x8a183e99, /// 0xbc0
			 0x2a44d082, /// 0xbc4
			 0x27c4bab4, /// 0xbc8
			 0xcb00edb2, /// 0xbcc
			 0x21a03e97, /// 0xbd0
			 0xeb6e26d6, /// 0xbd4
			 0xad03eb96, /// 0xbd8
			 0x73c41140, /// 0xbdc
			 0x507fd7a6, /// 0xbe0
			 0x96d24160, /// 0xbe4
			 0x122e3717, /// 0xbe8
			 0x5066518a, /// 0xbec
			 0xe17d1345, /// 0xbf0
			 0x7c5a96e5, /// 0xbf4
			 0x00b8c834, /// 0xbf8
			 0xa79a3f01, /// 0xbfc
			 0xf568e426, /// 0xc00
			 0x11397f46, /// 0xc04
			 0xe0ff8fa3, /// 0xc08
			 0x19a5e90a, /// 0xc0c
			 0xbda4ea5b, /// 0xc10
			 0x74427e16, /// 0xc14
			 0x35253471, /// 0xc18
			 0x6722140a, /// 0xc1c
			 0xd3a084d0, /// 0xc20
			 0x31b3cee5, /// 0xc24
			 0xadd317d2, /// 0xc28
			 0xc5f07d27, /// 0xc2c
			 0x59d1a4ae, /// 0xc30
			 0xbb56afa2, /// 0xc34
			 0x735e87b4, /// 0xc38
			 0x428a1f23, /// 0xc3c
			 0xd8e723cc, /// 0xc40
			 0x502d5785, /// 0xc44
			 0x9c9ebb4b, /// 0xc48
			 0x8b7850af, /// 0xc4c
			 0xa8182747, /// 0xc50
			 0xff88c513, /// 0xc54
			 0x508073a3, /// 0xc58
			 0xa3a40708, /// 0xc5c
			 0xb3a85141, /// 0xc60
			 0x8db4821e, /// 0xc64
			 0xfda0ccc8, /// 0xc68
			 0x30715978, /// 0xc6c
			 0x51d69fba, /// 0xc70
			 0x57a5e75e, /// 0xc74
			 0x18d1cefb, /// 0xc78
			 0x321d63fd, /// 0xc7c
			 0x0f2f4d4f, /// 0xc80
			 0x04fb7c9d, /// 0xc84
			 0x2ef3a542, /// 0xc88
			 0xd8f2168b, /// 0xc8c
			 0x43890800, /// 0xc90
			 0xebdebedd, /// 0xc94
			 0x18f2779a, /// 0xc98
			 0xb45291e0, /// 0xc9c
			 0xe2ac10c1, /// 0xca0
			 0x81d3b01a, /// 0xca4
			 0x544716e3, /// 0xca8
			 0x2b646058, /// 0xcac
			 0x9121d1f4, /// 0xcb0
			 0x35f44668, /// 0xcb4
			 0xebeba988, /// 0xcb8
			 0x95222135, /// 0xcbc
			 0xd97a2c8d, /// 0xcc0
			 0x3200d7cf, /// 0xcc4
			 0xe29e246c, /// 0xcc8
			 0x99973de9, /// 0xccc
			 0x18eeaf0c, /// 0xcd0
			 0xa393bc18, /// 0xcd4
			 0x4fa10f3b, /// 0xcd8
			 0x9d91fe47, /// 0xcdc
			 0x3a41e42c, /// 0xce0
			 0x1337026c, /// 0xce4
			 0xb293a9c4, /// 0xce8
			 0xe7fd8a3d, /// 0xcec
			 0x49f09530, /// 0xcf0
			 0x8c059fd1, /// 0xcf4
			 0x79ca05a2, /// 0xcf8
			 0xbfe30aac, /// 0xcfc
			 0x2119d51f, /// 0xd00
			 0xf99ac232, /// 0xd04
			 0x74362944, /// 0xd08
			 0xe956e8c8, /// 0xd0c
			 0x177a35b5, /// 0xd10
			 0xea9ea62d, /// 0xd14
			 0x5537e9ad, /// 0xd18
			 0xc60bd94b, /// 0xd1c
			 0xb407bc2d, /// 0xd20
			 0xc5463845, /// 0xd24
			 0x4d145014, /// 0xd28
			 0x7275856c, /// 0xd2c
			 0xea4c1ffd, /// 0xd30
			 0x69bdae9d, /// 0xd34
			 0x1ad6c437, /// 0xd38
			 0xded3f356, /// 0xd3c
			 0xc1490b8a, /// 0xd40
			 0x68636b58, /// 0xd44
			 0x1a2fd4d9, /// 0xd48
			 0x7bf037ac, /// 0xd4c
			 0xf5da4342, /// 0xd50
			 0xef60cc35, /// 0xd54
			 0x751308de, /// 0xd58
			 0xe7d6c499, /// 0xd5c
			 0x898b9e8d, /// 0xd60
			 0xece5245c, /// 0xd64
			 0x59fa6d04, /// 0xd68
			 0x8f3b3891, /// 0xd6c
			 0xdd6d2c6a, /// 0xd70
			 0xa3fbc8f1, /// 0xd74
			 0x8091b3ce, /// 0xd78
			 0xec463a8a, /// 0xd7c
			 0x586cbc1f, /// 0xd80
			 0x0fb62caf, /// 0xd84
			 0x1936af27, /// 0xd88
			 0x3debe52f, /// 0xd8c
			 0xeaaa5e69, /// 0xd90
			 0x38662aff, /// 0xd94
			 0x4a2c1d44, /// 0xd98
			 0x287e5d8e, /// 0xd9c
			 0x1d6fe81f, /// 0xda0
			 0x74d98089, /// 0xda4
			 0x4bb239ae, /// 0xda8
			 0xd7f86343, /// 0xdac
			 0x85c2f7a9, /// 0xdb0
			 0xab35b282, /// 0xdb4
			 0x8671b322, /// 0xdb8
			 0x7c17ec0b, /// 0xdbc
			 0xf6d59d83, /// 0xdc0
			 0x4602be9e, /// 0xdc4
			 0x1b54fa3c, /// 0xdc8
			 0x061022c5, /// 0xdcc
			 0xbd65ba60, /// 0xdd0
			 0x44ac6024, /// 0xdd4
			 0x109010c9, /// 0xdd8
			 0x021928d1, /// 0xddc
			 0x7300833e, /// 0xde0
			 0xf83edc94, /// 0xde4
			 0xd1368eae, /// 0xde8
			 0x8a77dce7, /// 0xdec
			 0xeb3713c8, /// 0xdf0
			 0x2d85baac, /// 0xdf4
			 0x2b4f8f94, /// 0xdf8
			 0x4e1ef91a, /// 0xdfc
			 0x0d491302, /// 0xe00
			 0xd14b4312, /// 0xe04
			 0x1029dbf9, /// 0xe08
			 0xf204720b, /// 0xe0c
			 0xbbe658d2, /// 0xe10
			 0xb6d03c31, /// 0xe14
			 0xd976bc66, /// 0xe18
			 0x8f671cc2, /// 0xe1c
			 0x9b724133, /// 0xe20
			 0x9d1463ae, /// 0xe24
			 0x0061a789, /// 0xe28
			 0x408b1915, /// 0xe2c
			 0x572e1d49, /// 0xe30
			 0xc50cf4cb, /// 0xe34
			 0x94875d1b, /// 0xe38
			 0xa2be44a6, /// 0xe3c
			 0x02b30850, /// 0xe40
			 0x04aaae64, /// 0xe44
			 0xd98e3983, /// 0xe48
			 0x4aaa464f, /// 0xe4c
			 0xdf5a9a8e, /// 0xe50
			 0xa6b2fcc5, /// 0xe54
			 0x66cbe0e2, /// 0xe58
			 0xc4e930c1, /// 0xe5c
			 0x0e884797, /// 0xe60
			 0x1f2df02a, /// 0xe64
			 0x5ca90408, /// 0xe68
			 0xbdd75e49, /// 0xe6c
			 0x8a2c573b, /// 0xe70
			 0x6952c400, /// 0xe74
			 0xe94bfd66, /// 0xe78
			 0xd36cdd21, /// 0xe7c
			 0x33c44525, /// 0xe80
			 0x458f41c1, /// 0xe84
			 0xfa2ba8a9, /// 0xe88
			 0x78b9621f, /// 0xe8c
			 0x15b01e8b, /// 0xe90
			 0x4d11e582, /// 0xe94
			 0xdca4654d, /// 0xe98
			 0xb6ca8cca, /// 0xe9c
			 0x3b13ea45, /// 0xea0
			 0x1e1dfce3, /// 0xea4
			 0x942e080f, /// 0xea8
			 0xb78e27f0, /// 0xeac
			 0x9feb4483, /// 0xeb0
			 0xe5a7e317, /// 0xeb4
			 0x85ad497f, /// 0xeb8
			 0xd7dce758, /// 0xebc
			 0xca5b591e, /// 0xec0
			 0x4374b395, /// 0xec4
			 0xc36b25a2, /// 0xec8
			 0xf2a67c3d, /// 0xecc
			 0xbfc50eac, /// 0xed0
			 0xfab15fb2, /// 0xed4
			 0x1336a5e9, /// 0xed8
			 0xbb6acc26, /// 0xedc
			 0x531bf704, /// 0xee0
			 0x3d4eb70e, /// 0xee4
			 0x6da7efd2, /// 0xee8
			 0x0576ab3e, /// 0xeec
			 0xa96871bf, /// 0xef0
			 0x219df152, /// 0xef4
			 0x2192b649, /// 0xef8
			 0x3d2cfa6f, /// 0xefc
			 0x113c1b0e, /// 0xf00
			 0x59d9d188, /// 0xf04
			 0x96f97e55, /// 0xf08
			 0x92f906a8, /// 0xf0c
			 0xb6fa334a, /// 0xf10
			 0xff5ddaae, /// 0xf14
			 0xa98cfd1e, /// 0xf18
			 0xcbf00f0c, /// 0xf1c
			 0x305aa4c1, /// 0xf20
			 0xbe79cd60, /// 0xf24
			 0xadf78380, /// 0xf28
			 0x11ab15eb, /// 0xf2c
			 0x0badbf3c, /// 0xf30
			 0x08d4480b, /// 0xf34
			 0xac1a9579, /// 0xf38
			 0x64d618fc, /// 0xf3c
			 0x210bd57a, /// 0xf40
			 0xeea4e485, /// 0xf44
			 0x5300d573, /// 0xf48
			 0x3ef8f578, /// 0xf4c
			 0xf5d0c4b1, /// 0xf50
			 0xf3004cbd, /// 0xf54
			 0x252ae2e1, /// 0xf58
			 0x9d1ff167, /// 0xf5c
			 0xac50e994, /// 0xf60
			 0x5105cce3, /// 0xf64
			 0x1aa8eaff, /// 0xf68
			 0x7cd3a678, /// 0xf6c
			 0xa9c70ef2, /// 0xf70
			 0x20d26342, /// 0xf74
			 0xfce1d1fc, /// 0xf78
			 0xeb4afbd2, /// 0xf7c
			 0x7ceaf298, /// 0xf80
			 0x7ffd076e, /// 0xf84
			 0xba5cd3e4, /// 0xf88
			 0x3782439c, /// 0xf8c
			 0xcbbb8002, /// 0xf90
			 0x40d3fcbf, /// 0xf94
			 0xc0f57f90, /// 0xf98
			 0xbb26518e, /// 0xf9c
			 0xd3c74f0a, /// 0xfa0
			 0xc91fa598, /// 0xfa4
			 0x9e4ea5c7, /// 0xfa8
			 0x4dae6acf, /// 0xfac
			 0xd25781df, /// 0xfb0
			 0x46059d7c, /// 0xfb4
			 0x52a96f2b, /// 0xfb8
			 0xb8af09f2, /// 0xfbc
			 0x44ff1f51, /// 0xfc0
			 0xe36a01bf, /// 0xfc4
			 0x61c840ff, /// 0xfc8
			 0xfed19d97, /// 0xfcc
			 0x6fe87f23, /// 0xfd0
			 0x00d488a5, /// 0xfd4
			 0x80dc3fe6, /// 0xfd8
			 0x6db37a7b, /// 0xfdc
			 0xaaac3e9a, /// 0xfe0
			 0xc0c5fd9b, /// 0xfe4
			 0x6f3bd219, /// 0xfe8
			 0x0308875c, /// 0xfec
			 0x1aea481f, /// 0xff0
			 0x69bcb167, /// 0xff4
			 0x8750a9e6, /// 0xff8
			 0xd179bd42 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00004
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0000c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00010
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00014
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0001c
			 0xff800000,                                                  // -inf                                        /// 00020
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00024
			 0xcb000000,                                                  // -8388608.0                                  /// 00028
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0002c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00030
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00034
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00038
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0003c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00040
			 0x0c780000,                                                  // Leading 1s:                                 /// 00044
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00048
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0004c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00050
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00054
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00064
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00068
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0006c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00070
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00074
			 0x0c400000,                                                  // Leading 1s:                                 /// 00078
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0007c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00080
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00084
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00088
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0008c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00090
			 0x00000000,                                                  // zero                                        /// 00094
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0009c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80000000,                                                  // -zero                                       /// 000e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00100
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0010c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00110
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00114
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00118
			 0x00011111,                                                  // 9.7958E-41                                  /// 0011c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00120
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00128
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0012c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00130
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00134
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0013c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00140
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00148
			 0xff800000,                                                  // -inf                                        /// 0014c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00154
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00158
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00160
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00164
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00168
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0016c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00170
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00174
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0xcb000000,                                                  // -8388608.0                                  /// 0017c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00180
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00184
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00188
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00190
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00198
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0019c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001a0
			 0xcb000000,                                                  // -8388608.0                                  /// 001a4
			 0xcb000000,                                                  // -8388608.0                                  /// 001a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001c4
			 0xbf028f5c,                                                  // -0.51                                       /// 001c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00200
			 0xcb000000,                                                  // -8388608.0                                  /// 00204
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00208
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0020c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00210
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0021c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00220
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00224
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x7f800000,                                                  // inf                                         /// 0022c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00230
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00238
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00240
			 0x0e000001,                                                  // Trailing 1s:                                /// 00244
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00248
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00250
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00254
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0025c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00260
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00268
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0026c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00270
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00278
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0027c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00280
			 0x80000000,                                                  // -zero                                       /// 00284
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00288
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x80000000,                                                  // -zero                                       /// 00290
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00298
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002f8
			 0xcb000000,                                                  // -8388608.0                                  /// 002fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00308
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0030c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00310
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00314
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00320
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00328
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0032c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00344
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00348
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0034c
			 0xffc00001,                                                  // -signaling NaN                              /// 00350
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00354
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00358
			 0x55555555,                                                  // 4 random values                             /// 0035c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00364
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00368
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0036c
			 0xff800000,                                                  // -inf                                        /// 00370
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00374
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x33333333,                                                  // 4 random values                             /// 00380
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00384
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x80011111,                                                  // -9.7958E-41                                 /// 0038c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00390
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00394
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00398
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0039c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003b4
			 0xcb000000,                                                  // -8388608.0                                  /// 003b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 003cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003e0
			 0x3f028f5c,                                                  // 0.51                                        /// 003e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 003f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00404
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00408
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0040c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00410
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00418
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00424
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00428
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0042c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00430
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00434
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00438
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0043c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00444
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00448
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00450
			 0x0c400000,                                                  // Leading 1s:                                 /// 00454
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00458
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0045c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00460
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00464
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00468
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0046c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00470
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00474
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00478
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0xff800000,                                                  // -inf                                        /// 00484
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00488
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0048c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00490
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0049c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004c0
			 0xffc00001,                                                  // -signaling NaN                              /// 004c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 004d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00508
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x00000000,                                                  // zero                                        /// 00518
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0051c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00520
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00524
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00530
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00534
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00538
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0053c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00540
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00544
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00548
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0054c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00554
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00560
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00564
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0056c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00570
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00578
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00580
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00584
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00588
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00594
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0059c
			 0x4b000000,                                                  // 8388608.0                                   /// 005a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005a4
			 0x55555555,                                                  // 4 random values                             /// 005a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 005ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005b4
			 0x7fc00001,                                                  // signaling NaN                               /// 005b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 005c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d4
			 0x3f028f5c,                                                  // 0.51                                        /// 005d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005e0
			 0x7fc00001,                                                  // signaling NaN                               /// 005e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00600
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00604
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00608
			 0x80011111,                                                  // -9.7958E-41                                 /// 0060c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00618
			 0xcb000000,                                                  // -8388608.0                                  /// 0061c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00620
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x3f028f5c,                                                  // 0.51                                        /// 0062c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x4b000000,                                                  // 8388608.0                                   /// 00638
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0063c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00640
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00644
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00648
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0064c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00650
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00654
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00658
			 0x0c700000,                                                  // Leading 1s:                                 /// 0065c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00660
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0066c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00670
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00674
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00680
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00684
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00688
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00698
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00700
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00704
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00708
			 0x4b000000,                                                  // 8388608.0                                   /// 0070c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00710
			 0x3f028f5c,                                                  // 0.51                                        /// 00714
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00718
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00720
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00724
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00728
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0072c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00730
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00734
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0073c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00740
			 0x00011111,                                                  // 9.7958E-41                                  /// 00744
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00748
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0074c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00758
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0075c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00764
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00768
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0076c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00774
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00778
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0077c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0078c
			 0xff800000,                                                  // -inf                                        /// 00790
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00794
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00798
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0079c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007cc
			 0xcb000000,                                                  // -8388608.0                                  /// 007d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 007e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007e8
			 0x4b000000,                                                  // 8388608.0                                   /// 007ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 007f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00800
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00804
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00808
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0080c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00814
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00818
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00820
			 0x33333333,                                                  // 4 random values                             /// 00824
			 0xff800000,                                                  // -inf                                        /// 00828
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00834
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00840
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00844
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0xbf028f5c,                                                  // -0.51                                       /// 0084c
			 0x55555555,                                                  // 4 random values                             /// 00850
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x80011111,                                                  // -9.7958E-41                                 /// 0085c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00860
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00864
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00868
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x4b000000,                                                  // 8388608.0                                   /// 00874
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00878
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0087c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00884
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0088c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00890
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00894
			 0x00000000,                                                  // zero                                        /// 00898
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008cc
			 0x80000000,                                                  // -zero                                       /// 008d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x55555555,                                                  // 4 random values                             /// 008e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00900
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00904
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00908
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00914
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0091c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x00011111,                                                  // 9.7958E-41                                  /// 00924
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00928
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0092c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00930
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00934
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00938
			 0x0e000007,                                                  // Trailing 1s:                                /// 0093c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00944
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00948
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0094c
			 0x55555555,                                                  // 4 random values                             /// 00950
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00954
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00958
			 0x0e000007,                                                  // Trailing 1s:                                /// 0095c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00960
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00964
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00968
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0096c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00970
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00974
			 0x0e000001,                                                  // Trailing 1s:                                /// 00978
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00984
			 0xff800000,                                                  // -inf                                        /// 00988
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00990
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00994
			 0x0c400000,                                                  // Leading 1s:                                 /// 00998
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0099c
			 0xbf028f5c,                                                  // -0.51                                       /// 009a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009dc
			 0x80000000,                                                  // -zero                                       /// 009e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a3c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a40
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a90
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a98
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00aa4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00aa8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00aac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ab4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ab8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ac0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00acc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x33333333,                                                  // 4 random values                             /// 00ad4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ad8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ae0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ae4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00af4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00af8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b04
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b10
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b38
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0xff800000,                                                  // -inf                                        /// 00b60
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b64
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b88
			 0x80000000,                                                  // -zero                                       /// 00b8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b90
			 0xbf028f5c,                                                  // -0.51                                       /// 00b94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ba0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x7f800000,                                                  // inf                                         /// 00ba8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bb4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bc0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bc4
			 0x00000000,                                                  // zero                                        /// 00bc8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bcc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bd8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bdc
			 0xff800000,                                                  // -inf                                        /// 00be0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00be4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be8
			 0x3f028f5c,                                                  // 0.51                                        /// 00bec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bf0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bf4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0xff800000,                                                  // -inf                                        /// 00c10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c18
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x7f800000,                                                  // inf                                         /// 00c34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c38
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c50
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c58
			 0xffc00001,                                                  // -signaling NaN                              /// 00c5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c98
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cb8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cbc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cc4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cc8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cd4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ce0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cf0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d04
			 0x7f800000,                                                  // inf                                         /// 00d08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d3c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d54
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d58
			 0xbf028f5c,                                                  // -0.51                                       /// 00d5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d84
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00da0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00da8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00db8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dbc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dc8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dcc
			 0x00000000,                                                  // zero                                        /// 00dd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dd4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ddc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00de4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00de8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00df4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00df8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dfc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e14
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e24
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80000000,                                                  // -zero                                       /// 00e4c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e50
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e58
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e5c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e90
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ea4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ea8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ebc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ec4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ed0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ed4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ed8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ee4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00eec
			 0x7f800000,                                                  // inf                                         /// 00ef0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ef8
			 0x7fc00001,                                                  // signaling NaN                               /// 00efc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f3c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f74
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fa0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fa4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fc8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fcc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fd8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fe0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ff0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ff4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x80001000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
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
			 0x000002bc,
			 0x0000026c,
			 0x0000020c,
			 0x00000578,
			 0x00000294,
			 0x00000344,
			 0x00000210,
			 0x000005e0,

			 /// vpu register f2
			 0x41a00000,
			 0x41200000,
			 0x41800000,
			 0x41800000,
			 0x41100000,
			 0x40a00000,
			 0x41d00000,
			 0x41a00000,

			 /// vpu register f3
			 0x3f800000,
			 0x41b80000,
			 0x41b80000,
			 0x41e80000,
			 0x41a80000,
			 0x42000000,
			 0x41a00000,
			 0x41300000,

			 /// vpu register f4
			 0x41f80000,
			 0x41500000,
			 0x3f800000,
			 0x41f00000,
			 0x41400000,
			 0x41b00000,
			 0x41600000,
			 0x40c00000,

			 /// vpu register f5
			 0x41a80000,
			 0x41b80000,
			 0x41b80000,
			 0x41f80000,
			 0x41880000,
			 0x41a80000,
			 0x40000000,
			 0x40800000,

			 /// vpu register f6
			 0x41400000,
			 0x41700000,
			 0x41b00000,
			 0x41e00000,
			 0x41f00000,
			 0x41d00000,
			 0x41400000,
			 0x41400000,

			 /// vpu register f7
			 0x42000000,
			 0x41500000,
			 0x41700000,
			 0x41600000,
			 0x41800000,
			 0x41980000,
			 0x41980000,
			 0x41c00000,

			 /// vpu register f8
			 0x41900000,
			 0x41d00000,
			 0x41000000,
			 0x41d00000,
			 0x41c80000,
			 0x41b80000,
			 0x41b80000,
			 0x41100000,

			 /// vpu register f9
			 0x40c00000,
			 0x41c00000,
			 0x41800000,
			 0x41c00000,
			 0x41500000,
			 0x41f00000,
			 0x41100000,
			 0x41600000,

			 /// vpu register f10
			 0x40e00000,
			 0x40400000,
			 0x41f80000,
			 0x40a00000,
			 0x41900000,
			 0x41000000,
			 0x40400000,
			 0x41880000,

			 /// vpu register f11
			 0x41e80000,
			 0x41d80000,
			 0x41e00000,
			 0x41200000,
			 0x41d00000,
			 0x40800000,
			 0x41e00000,
			 0x41d00000,

			 /// vpu register f12
			 0x41500000,
			 0x41d80000,
			 0x41f80000,
			 0x42000000,
			 0x41000000,
			 0x41500000,
			 0x41880000,
			 0x41a00000,

			 /// vpu register f13
			 0x41700000,
			 0x41100000,
			 0x41900000,
			 0x41100000,
			 0x41600000,
			 0x40e00000,
			 0x41e80000,
			 0x41d00000,

			 /// vpu register f14
			 0x41700000,
			 0x41200000,
			 0x40400000,
			 0x41500000,
			 0x40c00000,
			 0x3f800000,
			 0x42000000,
			 0x41b80000,

			 /// vpu register f15
			 0x40800000,
			 0x40400000,
			 0x41500000,
			 0x40400000,
			 0x40c00000,
			 0x40000000,
			 0x41500000,
			 0x41f00000,

			 /// vpu register f16
			 0x41500000,
			 0x41880000,
			 0x41900000,
			 0x41400000,
			 0x40a00000,
			 0x40e00000,
			 0x41a80000,
			 0x41c80000,

			 /// vpu register f17
			 0x41f80000,
			 0x41600000,
			 0x40000000,
			 0x41b00000,
			 0x41a80000,
			 0x41e80000,
			 0x41200000,
			 0x41b80000,

			 /// vpu register f18
			 0x40800000,
			 0x41000000,
			 0x40a00000,
			 0x41300000,
			 0x41f80000,
			 0x41e00000,
			 0x41500000,
			 0x41c80000,

			 /// vpu register f19
			 0x41b00000,
			 0x41a00000,
			 0x41000000,
			 0x40a00000,
			 0x41a00000,
			 0x41800000,
			 0x41b00000,
			 0x41300000,

			 /// vpu register f20
			 0x41400000,
			 0x41d80000,
			 0x41400000,
			 0x41900000,
			 0x40000000,
			 0x41e00000,
			 0x40800000,
			 0x41880000,

			 /// vpu register f21
			 0x41f00000,
			 0x41c00000,
			 0x41d80000,
			 0x41000000,
			 0x41000000,
			 0x40000000,
			 0x41d80000,
			 0x41e00000,

			 /// vpu register f22
			 0x41e00000,
			 0x40e00000,
			 0x41880000,
			 0x40400000,
			 0x41880000,
			 0x41800000,
			 0x41800000,
			 0x40000000,

			 /// vpu register f23
			 0x41300000,
			 0x40c00000,
			 0x41e00000,
			 0x41d80000,
			 0x41700000,
			 0x41880000,
			 0x41c00000,
			 0x41b80000,

			 /// vpu register f24
			 0x41c00000,
			 0x41f80000,
			 0x40800000,
			 0x41700000,
			 0x42000000,
			 0x41e00000,
			 0x41900000,
			 0x40000000,

			 /// vpu register f25
			 0x40800000,
			 0x41000000,
			 0x40e00000,
			 0x41500000,
			 0x41800000,
			 0x41100000,
			 0x41300000,
			 0x42000000,

			 /// vpu register f26
			 0x41300000,
			 0x41300000,
			 0x41400000,
			 0x41300000,
			 0x42000000,
			 0x40e00000,
			 0x41800000,
			 0x41100000,

			 /// vpu register f27
			 0x41200000,
			 0x40c00000,
			 0x41880000,
			 0x41200000,
			 0x42000000,
			 0x41a00000,
			 0x40e00000,
			 0x41f80000,

			 /// vpu register f28
			 0x41a00000,
			 0x41700000,
			 0x41c80000,
			 0x41c80000,
			 0x40e00000,
			 0x41b00000,
			 0x41e80000,
			 0x41980000,

			 /// vpu register f29
			 0x40800000,
			 0x41000000,
			 0x40e00000,
			 0x41100000,
			 0x41700000,
			 0x41500000,
			 0x41000000,
			 0x41100000,

			 /// vpu register f30
			 0x41f80000,
			 0x41e80000,
			 0x41500000,
			 0x41d80000,
			 0x41800000,
			 0x42000000,
			 0x41c00000,
			 0x41c00000,

			 /// vpu register f31
			 0x41e80000,
			 0x41a00000,
			 0x41980000,
			 0x41c80000,
			 0x41c80000,
			 0x41e80000,
			 0x41a80000,
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
		"fsw f28, -1032(x15)\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f21, -1116(x11)\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f22, -82(x14)\n"                                 ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f14, -402(x14)\n"                                ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f2, 1203(x11)\n"                                 ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f7, -362(x13)\n"                                 ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f8, -1636(x12)\n"                                ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f28, -416(x14)\n"                                ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f17, -172(x11)\n"                                ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f11, -87(x12)\n"                                 ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f11, 1927(x12)\n"                                ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f11, 153(x15)\n"                                 ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f26, -1875(x14)\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f9, -1937(x13)\n"                                ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f5, 220(x15)\n"                                  ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f13, 199(x13)\n"                                 ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f28, 215(x14)\n"                                 ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f12, -1419(x12)\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f27, 998(x12)\n"                                 ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f10, -712(x13)\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f12, -1370(x15)\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f9, 340(x15)\n"                                  ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f11, 655(x11)\n"                                 ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f5, 2041(x11)\n"                                 ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f26, -1562(x13)\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f24, 317(x14)\n"                                 ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f9, 1805(x11)\n"                                 ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f11, 105(x15)\n"                                 ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f16, 972(x15)\n"                                 ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f21, -1773(x13)\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f7, -384(x14)\n"                                 ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f2, 1192(x13)\n"                                 ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f21, -651(x12)\n"                                ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f21, -526(x14)\n"                                ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f11, -702(x11)\n"                                ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f27, 839(x14)\n"                                 ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f23, 1170(x14)\n"                                ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f0, -958(x12)\n"                                 ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f7, 1040(x12)\n"                                 ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f27, -659(x12)\n"                                ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f22, 1452(x15)\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f4, 572(x12)\n"                                  ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f29, 11(x13)\n"                                  ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f22, -1952(x12)\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f27, -583(x14)\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f17, 1582(x15)\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f10, -539(x11)\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f21, 1849(x15)\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f27, -594(x11)\n"                                ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f30, -609(x13)\n"                                ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f29, 422(x14)\n"                                 ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f8, -1198(x15)\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f19, -1589(x15)\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f25, -1090(x14)\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f27, 1707(x14)\n"                                ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f16, 1614(x12)\n"                                ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f10, -1661(x12)\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f24, 1222(x13)\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f15, -1659(x13)\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f20, 1105(x11)\n"                                ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f7, -357(x13)\n"                                 ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f7, -1760(x12)\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f28, 1461(x15)\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f29, 789(x15)\n"                                 ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f1, 744(x11)\n"                                  ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f12, 1244(x12)\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f1, 254(x13)\n"                                  ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f29, 312(x14)\n"                                 ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f17, 225(x15)\n"                                 ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f19, 130(x15)\n"                                 ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f23, -63(x13)\n"                                 ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f27, -1657(x13)\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f22, 6(x13)\n"                                   ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f1, 948(x15)\n"                                  ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f10, 1211(x11)\n"                                ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f14, 187(x15)\n"                                 ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f21, -999(x12)\n"                                ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f2, 944(x15)\n"                                  ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f30, 567(x14)\n"                                 ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f13, -576(x15)\n"                                ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f5, -1809(x14)\n"                                ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f0, -269(x15)\n"                                 ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f22, -1000(x14)\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f14, 1156(x11)\n"                                ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f27, 946(x11)\n"                                 ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f21, -548(x13)\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f10, 1453(x13)\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f6, -1614(x15)\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f14, -1363(x12)\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f21, -999(x14)\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f29, 1679(x14)\n"                                ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f19, 1817(x11)\n"                                ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f7, 1916(x12)\n"                                 ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f21, 190(x11)\n"                                 ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f28, -20(x15)\n"                                 ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f29, -920(x13)\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f6, 430(x14)\n"                                  ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f7, -523(x12)\n"                                 ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw f3, 775(x12)\n"                                  ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
