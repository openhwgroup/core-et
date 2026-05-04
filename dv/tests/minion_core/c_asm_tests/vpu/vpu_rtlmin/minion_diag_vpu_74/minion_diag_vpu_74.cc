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
			 0x50f98048, /// 0x0
			 0xa3ca7c75, /// 0x4
			 0x92e736d3, /// 0x8
			 0xf57febb1, /// 0xc
			 0x7297350f, /// 0x10
			 0xa2584cc5, /// 0x14
			 0x8f74477f, /// 0x18
			 0xc0d3628c, /// 0x1c
			 0xc2501c06, /// 0x20
			 0x26c7cc84, /// 0x24
			 0xfafbefd6, /// 0x28
			 0x82f0db32, /// 0x2c
			 0x72c9e047, /// 0x30
			 0xfc121f20, /// 0x34
			 0xdcab3c85, /// 0x38
			 0x018c87d2, /// 0x3c
			 0x767ee3b4, /// 0x40
			 0x9fb9ec34, /// 0x44
			 0xe7ad1d91, /// 0x48
			 0x3c94a485, /// 0x4c
			 0x598715a3, /// 0x50
			 0x4b4c91c8, /// 0x54
			 0x8f399e41, /// 0x58
			 0xbe547c67, /// 0x5c
			 0xbb72aa3e, /// 0x60
			 0x9d0f58fc, /// 0x64
			 0xdf35b705, /// 0x68
			 0x36fd880f, /// 0x6c
			 0xb898e531, /// 0x70
			 0xbb9b7ba6, /// 0x74
			 0x3ea069f5, /// 0x78
			 0x1e143af7, /// 0x7c
			 0x4baf4e94, /// 0x80
			 0xb6e66f58, /// 0x84
			 0x22f98255, /// 0x88
			 0xb93ce3b8, /// 0x8c
			 0x50745edb, /// 0x90
			 0xc8d9f5f5, /// 0x94
			 0xfe410167, /// 0x98
			 0x756ff8b7, /// 0x9c
			 0x2b03504f, /// 0xa0
			 0x1e964e3e, /// 0xa4
			 0xd3b71c1b, /// 0xa8
			 0xe4afd8c8, /// 0xac
			 0x3288a915, /// 0xb0
			 0xace23041, /// 0xb4
			 0xe536e49e, /// 0xb8
			 0x0fcc131b, /// 0xbc
			 0x838418a3, /// 0xc0
			 0x3a681b5e, /// 0xc4
			 0x33b87448, /// 0xc8
			 0x35b23a98, /// 0xcc
			 0x86e70bf7, /// 0xd0
			 0x150d2c29, /// 0xd4
			 0x663df029, /// 0xd8
			 0xec85c3c5, /// 0xdc
			 0xaf7c4ff6, /// 0xe0
			 0xffe18a83, /// 0xe4
			 0x720d4a23, /// 0xe8
			 0x7a9f7db6, /// 0xec
			 0xd2012e48, /// 0xf0
			 0xc5707ac2, /// 0xf4
			 0xfa75f424, /// 0xf8
			 0xc1cd3729, /// 0xfc
			 0x423605f6, /// 0x100
			 0x68adca4f, /// 0x104
			 0x823486b4, /// 0x108
			 0xfdf98db8, /// 0x10c
			 0x8bcb881a, /// 0x110
			 0xb2981738, /// 0x114
			 0x07a0864d, /// 0x118
			 0x15f7a27c, /// 0x11c
			 0x94c0f76d, /// 0x120
			 0xa8e0ba58, /// 0x124
			 0x2172a035, /// 0x128
			 0x86ea63ed, /// 0x12c
			 0x1684fe89, /// 0x130
			 0x26379430, /// 0x134
			 0x6634ba91, /// 0x138
			 0xa107b312, /// 0x13c
			 0xa3ee1ca7, /// 0x140
			 0xf6fb9128, /// 0x144
			 0x05d84737, /// 0x148
			 0x86d344ff, /// 0x14c
			 0xc6282364, /// 0x150
			 0xfe1f6cf7, /// 0x154
			 0x8e170be7, /// 0x158
			 0x2bb88956, /// 0x15c
			 0xca47b44e, /// 0x160
			 0x3b90c3d0, /// 0x164
			 0xa3c80170, /// 0x168
			 0xf3766f67, /// 0x16c
			 0x64e0914f, /// 0x170
			 0x04c4f4c3, /// 0x174
			 0xb584d0f4, /// 0x178
			 0x89e97661, /// 0x17c
			 0x4f418cb3, /// 0x180
			 0xc5185be0, /// 0x184
			 0x51c460a2, /// 0x188
			 0x3839127b, /// 0x18c
			 0x2a41897f, /// 0x190
			 0xa68db7a8, /// 0x194
			 0xf79251ac, /// 0x198
			 0xce7304e7, /// 0x19c
			 0xc17b05a4, /// 0x1a0
			 0x9fb7fc09, /// 0x1a4
			 0xa6a50503, /// 0x1a8
			 0x9eb205eb, /// 0x1ac
			 0x6053389d, /// 0x1b0
			 0x73f44702, /// 0x1b4
			 0xd17e4e54, /// 0x1b8
			 0xb0241c6f, /// 0x1bc
			 0x04c2fb1f, /// 0x1c0
			 0xe37b6bb2, /// 0x1c4
			 0x3bbf628f, /// 0x1c8
			 0x8a0d589d, /// 0x1cc
			 0xf538ddfe, /// 0x1d0
			 0xae72b0a9, /// 0x1d4
			 0xb89530af, /// 0x1d8
			 0xb735a3af, /// 0x1dc
			 0x27fcbc0e, /// 0x1e0
			 0x66c58382, /// 0x1e4
			 0xae5a8a7d, /// 0x1e8
			 0xc15eec0b, /// 0x1ec
			 0xf5343fd5, /// 0x1f0
			 0x382bbbc4, /// 0x1f4
			 0x0f761f59, /// 0x1f8
			 0x5d5e9c06, /// 0x1fc
			 0x7c98c620, /// 0x200
			 0xf48f0fef, /// 0x204
			 0x92a1d0fd, /// 0x208
			 0x59d53b5e, /// 0x20c
			 0xbd10a2cd, /// 0x210
			 0x75ec166a, /// 0x214
			 0x85bb4447, /// 0x218
			 0x702395a6, /// 0x21c
			 0x2d14b0d4, /// 0x220
			 0x92462700, /// 0x224
			 0x8eec70e9, /// 0x228
			 0x3cda01b2, /// 0x22c
			 0x1f2b3d84, /// 0x230
			 0x665e0150, /// 0x234
			 0x4d30342a, /// 0x238
			 0x74103a01, /// 0x23c
			 0x0f96f703, /// 0x240
			 0xa4a3e76f, /// 0x244
			 0xd3866659, /// 0x248
			 0x8da251b6, /// 0x24c
			 0x79c7adc6, /// 0x250
			 0xdea03b2d, /// 0x254
			 0x834f3c4b, /// 0x258
			 0xc99b211b, /// 0x25c
			 0x4217edb3, /// 0x260
			 0x47c28a7d, /// 0x264
			 0x83ed06b5, /// 0x268
			 0x9cbdc957, /// 0x26c
			 0x839dfa0f, /// 0x270
			 0xa3cd5508, /// 0x274
			 0x4cfbe4d1, /// 0x278
			 0x467c63c7, /// 0x27c
			 0x85460670, /// 0x280
			 0x64fa6792, /// 0x284
			 0xc90ceacf, /// 0x288
			 0xc281a69a, /// 0x28c
			 0x57d0c796, /// 0x290
			 0x34c97500, /// 0x294
			 0x1b8bc283, /// 0x298
			 0x9216dc88, /// 0x29c
			 0x2a72d7ec, /// 0x2a0
			 0xd78e4f3c, /// 0x2a4
			 0xa0cb224f, /// 0x2a8
			 0x9ba2bbde, /// 0x2ac
			 0xb658cb99, /// 0x2b0
			 0xdf298ee8, /// 0x2b4
			 0x24e1d4f5, /// 0x2b8
			 0xc7d9b513, /// 0x2bc
			 0x57cb69d8, /// 0x2c0
			 0x3996db6d, /// 0x2c4
			 0x912c3979, /// 0x2c8
			 0xd5ef317d, /// 0x2cc
			 0x71a9ddaa, /// 0x2d0
			 0x0edabea5, /// 0x2d4
			 0xf6158f7a, /// 0x2d8
			 0xdb9d0d94, /// 0x2dc
			 0x473bd607, /// 0x2e0
			 0x27156975, /// 0x2e4
			 0xab5667fb, /// 0x2e8
			 0xa22b293f, /// 0x2ec
			 0xf38b87fb, /// 0x2f0
			 0xb07308ab, /// 0x2f4
			 0x57d0ecb4, /// 0x2f8
			 0xb7ab5202, /// 0x2fc
			 0xb6dfdc74, /// 0x300
			 0x4add5f41, /// 0x304
			 0x38528e4d, /// 0x308
			 0x40ee74a3, /// 0x30c
			 0x01f5452d, /// 0x310
			 0x8febf3a8, /// 0x314
			 0xfad2da10, /// 0x318
			 0x95482e03, /// 0x31c
			 0xcf274c8f, /// 0x320
			 0x69808d31, /// 0x324
			 0x3d947ceb, /// 0x328
			 0x275d7731, /// 0x32c
			 0x3b910d3e, /// 0x330
			 0x0c7a95d5, /// 0x334
			 0xc67ef4d9, /// 0x338
			 0x756b4cf7, /// 0x33c
			 0x09fc099b, /// 0x340
			 0xa8206013, /// 0x344
			 0x5841632c, /// 0x348
			 0x262968fe, /// 0x34c
			 0x4313231d, /// 0x350
			 0x4fe2e8e6, /// 0x354
			 0x9f79023a, /// 0x358
			 0x12a9a978, /// 0x35c
			 0xa2f9a54f, /// 0x360
			 0x4e219d65, /// 0x364
			 0x14f0457a, /// 0x368
			 0x8a6a2f04, /// 0x36c
			 0x64d38f0d, /// 0x370
			 0xd31aae89, /// 0x374
			 0x39a6b051, /// 0x378
			 0xf7297d5e, /// 0x37c
			 0x1a20dc66, /// 0x380
			 0xd8a0a504, /// 0x384
			 0x79a6101c, /// 0x388
			 0x552141fa, /// 0x38c
			 0x3d7ee9ec, /// 0x390
			 0xbad540b1, /// 0x394
			 0x6c149afb, /// 0x398
			 0x93def284, /// 0x39c
			 0x5d2f1c85, /// 0x3a0
			 0x82cfc325, /// 0x3a4
			 0x41a4253e, /// 0x3a8
			 0xfb2685f6, /// 0x3ac
			 0xe1a29b67, /// 0x3b0
			 0xaf66ad64, /// 0x3b4
			 0x3b2b00a1, /// 0x3b8
			 0x5300c6be, /// 0x3bc
			 0x20a58078, /// 0x3c0
			 0xdc0c140b, /// 0x3c4
			 0x01dc4ec7, /// 0x3c8
			 0x13af2cff, /// 0x3cc
			 0xa1398a3f, /// 0x3d0
			 0x370287dd, /// 0x3d4
			 0xe68fe2ec, /// 0x3d8
			 0x4ae04498, /// 0x3dc
			 0xc33e8be1, /// 0x3e0
			 0x3028158a, /// 0x3e4
			 0xcbc5820a, /// 0x3e8
			 0x46fc0cd0, /// 0x3ec
			 0x2205565d, /// 0x3f0
			 0x028d2ab5, /// 0x3f4
			 0xc3b1a14d, /// 0x3f8
			 0x577e304f, /// 0x3fc
			 0xa005eaa2, /// 0x400
			 0x4bf915f1, /// 0x404
			 0xecd4ceff, /// 0x408
			 0xfe7e8d6b, /// 0x40c
			 0x8945cc58, /// 0x410
			 0xe7526473, /// 0x414
			 0xa0196fcc, /// 0x418
			 0x32499fb7, /// 0x41c
			 0x7925c828, /// 0x420
			 0x37b39432, /// 0x424
			 0xe345626b, /// 0x428
			 0xdcc670ea, /// 0x42c
			 0xb97cfb5c, /// 0x430
			 0x4fe6df5a, /// 0x434
			 0xe182bcb7, /// 0x438
			 0x64407b87, /// 0x43c
			 0x5be6cc69, /// 0x440
			 0x651f1152, /// 0x444
			 0xfc86199b, /// 0x448
			 0xc2f55ab1, /// 0x44c
			 0x7e452b0a, /// 0x450
			 0x6f7be214, /// 0x454
			 0xf6637a4a, /// 0x458
			 0xb7692500, /// 0x45c
			 0xd060ad26, /// 0x460
			 0x9108b765, /// 0x464
			 0xde0a6c61, /// 0x468
			 0xbdad43ad, /// 0x46c
			 0xcc1f8c4f, /// 0x470
			 0x2cebdc49, /// 0x474
			 0xa33e5b67, /// 0x478
			 0xcfb33f19, /// 0x47c
			 0x10b322c0, /// 0x480
			 0x53582534, /// 0x484
			 0xbc98f2e2, /// 0x488
			 0xc18936f5, /// 0x48c
			 0xee4db60a, /// 0x490
			 0x643178d0, /// 0x494
			 0xdef6be2b, /// 0x498
			 0xfbbf287a, /// 0x49c
			 0x1da54459, /// 0x4a0
			 0x40db5f1d, /// 0x4a4
			 0x0c414564, /// 0x4a8
			 0xa1d393ed, /// 0x4ac
			 0xdbd8a3f1, /// 0x4b0
			 0xa00bd409, /// 0x4b4
			 0x0b3d50b6, /// 0x4b8
			 0xca21963b, /// 0x4bc
			 0xa0c252f9, /// 0x4c0
			 0x4ff11828, /// 0x4c4
			 0x67da6dd0, /// 0x4c8
			 0x304e2b00, /// 0x4cc
			 0x42ee38b1, /// 0x4d0
			 0xda9f11b7, /// 0x4d4
			 0x13731c09, /// 0x4d8
			 0x40bbc0fb, /// 0x4dc
			 0xd313cacc, /// 0x4e0
			 0x4149005b, /// 0x4e4
			 0x1738f3f1, /// 0x4e8
			 0x8a575b67, /// 0x4ec
			 0xa133f090, /// 0x4f0
			 0x26499066, /// 0x4f4
			 0x3a4fa9ad, /// 0x4f8
			 0x1e5ff51c, /// 0x4fc
			 0x5eb222b2, /// 0x500
			 0xe3801eae, /// 0x504
			 0xf878d598, /// 0x508
			 0x35c53606, /// 0x50c
			 0xbe60afe1, /// 0x510
			 0x37b3b86d, /// 0x514
			 0x6cbd9a6a, /// 0x518
			 0x194546e0, /// 0x51c
			 0x14ad75e6, /// 0x520
			 0x9a4923f9, /// 0x524
			 0x4c8a2afa, /// 0x528
			 0x70ebf11e, /// 0x52c
			 0x45b6a976, /// 0x530
			 0x08658725, /// 0x534
			 0xe87c666d, /// 0x538
			 0x909c9b5e, /// 0x53c
			 0x29f36168, /// 0x540
			 0x0e47256b, /// 0x544
			 0xcd58c6a1, /// 0x548
			 0xddd412d0, /// 0x54c
			 0x7ef1db9b, /// 0x550
			 0x59a4d1e1, /// 0x554
			 0x85a6fd62, /// 0x558
			 0x028b8270, /// 0x55c
			 0xdb588d92, /// 0x560
			 0xa047b397, /// 0x564
			 0x5b92a06a, /// 0x568
			 0x1c33bc5f, /// 0x56c
			 0x4ab1a432, /// 0x570
			 0x67f58936, /// 0x574
			 0x7b1721c0, /// 0x578
			 0xa361d9a5, /// 0x57c
			 0x3dc5528f, /// 0x580
			 0x3c451521, /// 0x584
			 0x0cd5c4b1, /// 0x588
			 0x2545b052, /// 0x58c
			 0xf0f91320, /// 0x590
			 0xb1dbe0f4, /// 0x594
			 0x5edbe36f, /// 0x598
			 0xee96fb14, /// 0x59c
			 0x00e7307b, /// 0x5a0
			 0x3e853cee, /// 0x5a4
			 0x32d359b4, /// 0x5a8
			 0xe8b21974, /// 0x5ac
			 0xecafd9d8, /// 0x5b0
			 0x711d7b21, /// 0x5b4
			 0xbb6b0405, /// 0x5b8
			 0xdbbf47fa, /// 0x5bc
			 0xeb176f72, /// 0x5c0
			 0x04ae2dfd, /// 0x5c4
			 0xde87ff89, /// 0x5c8
			 0x9833a146, /// 0x5cc
			 0xf6b88c8e, /// 0x5d0
			 0x3b3f9b65, /// 0x5d4
			 0x5ef95c9c, /// 0x5d8
			 0x0e9756c8, /// 0x5dc
			 0x64c5ded4, /// 0x5e0
			 0x3c168a1c, /// 0x5e4
			 0xa3f22c00, /// 0x5e8
			 0x961e9b71, /// 0x5ec
			 0x12921835, /// 0x5f0
			 0x6f7e09b7, /// 0x5f4
			 0xb46b334d, /// 0x5f8
			 0x771f970e, /// 0x5fc
			 0x9657fcbc, /// 0x600
			 0xfbd7acb7, /// 0x604
			 0x70dd56ae, /// 0x608
			 0x1c4960c0, /// 0x60c
			 0xbf3c6452, /// 0x610
			 0x7bc88046, /// 0x614
			 0x7e035dda, /// 0x618
			 0x99c891c1, /// 0x61c
			 0xa6bf88f1, /// 0x620
			 0xaee641c9, /// 0x624
			 0x26feb8e8, /// 0x628
			 0xa1384d26, /// 0x62c
			 0x5cc67834, /// 0x630
			 0xfd7d7a8a, /// 0x634
			 0x89bbc39b, /// 0x638
			 0xbd9143e0, /// 0x63c
			 0x998e8f56, /// 0x640
			 0xf4013a68, /// 0x644
			 0xdef32f57, /// 0x648
			 0x6d9a5d47, /// 0x64c
			 0xa176c521, /// 0x650
			 0xbc8e986d, /// 0x654
			 0xccf42ca7, /// 0x658
			 0x05697210, /// 0x65c
			 0x250f4a71, /// 0x660
			 0x58f46f6a, /// 0x664
			 0xc2464a20, /// 0x668
			 0x6287aede, /// 0x66c
			 0x443d24c6, /// 0x670
			 0xa410e76a, /// 0x674
			 0xad7b7b62, /// 0x678
			 0xbd0b760c, /// 0x67c
			 0xa1ca86fb, /// 0x680
			 0x6f1a1e91, /// 0x684
			 0x4fdedbbd, /// 0x688
			 0xb2acedcc, /// 0x68c
			 0xc5774905, /// 0x690
			 0xb7aade23, /// 0x694
			 0x37207742, /// 0x698
			 0x28a5a0ca, /// 0x69c
			 0x1f6908c0, /// 0x6a0
			 0xefa57a6d, /// 0x6a4
			 0x8c039364, /// 0x6a8
			 0xcc2d4daa, /// 0x6ac
			 0x7582d443, /// 0x6b0
			 0x2031c487, /// 0x6b4
			 0x9e6a63fb, /// 0x6b8
			 0xdf541a3f, /// 0x6bc
			 0xe0a2a0eb, /// 0x6c0
			 0x50a4b4ed, /// 0x6c4
			 0xf094321f, /// 0x6c8
			 0x3f366c53, /// 0x6cc
			 0x9b7b58d4, /// 0x6d0
			 0x498f4577, /// 0x6d4
			 0x09777e08, /// 0x6d8
			 0x954347ff, /// 0x6dc
			 0xa1cf6952, /// 0x6e0
			 0x505de664, /// 0x6e4
			 0x2d098697, /// 0x6e8
			 0xb59d221c, /// 0x6ec
			 0x3572853f, /// 0x6f0
			 0xe4fb8415, /// 0x6f4
			 0xb393ad86, /// 0x6f8
			 0x6a5cc6fb, /// 0x6fc
			 0x285d77eb, /// 0x700
			 0x1582b0b6, /// 0x704
			 0x6547d487, /// 0x708
			 0x152cd197, /// 0x70c
			 0xf86a1095, /// 0x710
			 0x82f6cceb, /// 0x714
			 0x0175eb42, /// 0x718
			 0xae038f6d, /// 0x71c
			 0x501fa484, /// 0x720
			 0x5d8f6a3e, /// 0x724
			 0x618b5609, /// 0x728
			 0x3c046d9c, /// 0x72c
			 0x41c4b4a0, /// 0x730
			 0x15ab8a5c, /// 0x734
			 0xbd2bc75c, /// 0x738
			 0x010010fb, /// 0x73c
			 0x1331e7aa, /// 0x740
			 0x17c63ecc, /// 0x744
			 0x77930150, /// 0x748
			 0xcd41c8aa, /// 0x74c
			 0xceaa29f2, /// 0x750
			 0x317ab396, /// 0x754
			 0x6b509b8e, /// 0x758
			 0x83663ac7, /// 0x75c
			 0x96dd0d63, /// 0x760
			 0xc3925ab6, /// 0x764
			 0x84941d33, /// 0x768
			 0xed008f16, /// 0x76c
			 0xc813e1f6, /// 0x770
			 0x45a8d590, /// 0x774
			 0x3691c5a4, /// 0x778
			 0x106c1daf, /// 0x77c
			 0x06a1e00f, /// 0x780
			 0x3a0a8b53, /// 0x784
			 0x2784a9bc, /// 0x788
			 0x69997703, /// 0x78c
			 0xf6a2bb4f, /// 0x790
			 0x0daa2137, /// 0x794
			 0xd8c96e22, /// 0x798
			 0x65fbd7d1, /// 0x79c
			 0xa6a42551, /// 0x7a0
			 0xb560d814, /// 0x7a4
			 0x44f28532, /// 0x7a8
			 0xb49b7216, /// 0x7ac
			 0x0a15bf9d, /// 0x7b0
			 0x887c646c, /// 0x7b4
			 0xe40aebf2, /// 0x7b8
			 0x1cc0c5b1, /// 0x7bc
			 0xe622d6f0, /// 0x7c0
			 0x32ac6683, /// 0x7c4
			 0xb27314c4, /// 0x7c8
			 0x26f56b82, /// 0x7cc
			 0xd119e52a, /// 0x7d0
			 0x0e6f9fcd, /// 0x7d4
			 0x740614c9, /// 0x7d8
			 0x3d86bf50, /// 0x7dc
			 0x0ceab9f2, /// 0x7e0
			 0xc75634a6, /// 0x7e4
			 0xc17cef7f, /// 0x7e8
			 0xde027d57, /// 0x7ec
			 0x2d7fa601, /// 0x7f0
			 0x15746d50, /// 0x7f4
			 0xbb9bc069, /// 0x7f8
			 0x8e377591, /// 0x7fc
			 0x69172869, /// 0x800
			 0x06121fbb, /// 0x804
			 0x2462bf34, /// 0x808
			 0x7cab1988, /// 0x80c
			 0x31e86ada, /// 0x810
			 0xba642426, /// 0x814
			 0x76d25cfd, /// 0x818
			 0x4d75b335, /// 0x81c
			 0xac5863a5, /// 0x820
			 0x28c4003e, /// 0x824
			 0xa9c5ee26, /// 0x828
			 0x213d9c3e, /// 0x82c
			 0x31dee4df, /// 0x830
			 0x655ddaa7, /// 0x834
			 0x34384b62, /// 0x838
			 0x088a6c2a, /// 0x83c
			 0x86d83443, /// 0x840
			 0x39fb2d82, /// 0x844
			 0xe6c00732, /// 0x848
			 0xb52638b1, /// 0x84c
			 0x5a4572e4, /// 0x850
			 0x0ea277af, /// 0x854
			 0xb66a531f, /// 0x858
			 0xed86306a, /// 0x85c
			 0xc9c4861d, /// 0x860
			 0x1685d6a3, /// 0x864
			 0xe4486efa, /// 0x868
			 0xe4a10f21, /// 0x86c
			 0xb6fad938, /// 0x870
			 0x6eb3b252, /// 0x874
			 0xb3ff4317, /// 0x878
			 0x4e01c929, /// 0x87c
			 0x78aa6657, /// 0x880
			 0x0cd32f5b, /// 0x884
			 0x9f216a9a, /// 0x888
			 0x4150d114, /// 0x88c
			 0x0764eb32, /// 0x890
			 0x41bf25d1, /// 0x894
			 0x2330ddba, /// 0x898
			 0x2ca6b3c0, /// 0x89c
			 0x5439cc4f, /// 0x8a0
			 0x97ae6b5c, /// 0x8a4
			 0x1f362f4e, /// 0x8a8
			 0xd34dafd7, /// 0x8ac
			 0xb376e325, /// 0x8b0
			 0xb05685d5, /// 0x8b4
			 0x83f523d7, /// 0x8b8
			 0x9e934ebc, /// 0x8bc
			 0x6e605de3, /// 0x8c0
			 0x8a5c9eef, /// 0x8c4
			 0x684bd567, /// 0x8c8
			 0x364b6e72, /// 0x8cc
			 0xa348e1d1, /// 0x8d0
			 0x6237836e, /// 0x8d4
			 0x334257d0, /// 0x8d8
			 0x8a194c94, /// 0x8dc
			 0xddca72b9, /// 0x8e0
			 0x962b0457, /// 0x8e4
			 0x2fce70e9, /// 0x8e8
			 0x5437130c, /// 0x8ec
			 0xec7827a1, /// 0x8f0
			 0xa293496e, /// 0x8f4
			 0x8b29af1a, /// 0x8f8
			 0x4cf56afa, /// 0x8fc
			 0x350d1ea3, /// 0x900
			 0x597a75fd, /// 0x904
			 0xdfc11d5e, /// 0x908
			 0x9a1e0730, /// 0x90c
			 0xc2cb6e61, /// 0x910
			 0x8ec4b624, /// 0x914
			 0xc736d00a, /// 0x918
			 0xd09308ee, /// 0x91c
			 0xee4ef49b, /// 0x920
			 0xd1da8c9a, /// 0x924
			 0x4d6656b0, /// 0x928
			 0xd600466d, /// 0x92c
			 0xf930bd2a, /// 0x930
			 0x96c479be, /// 0x934
			 0x6d339182, /// 0x938
			 0x63e82270, /// 0x93c
			 0xe7cb9704, /// 0x940
			 0x4ceb4a62, /// 0x944
			 0x7c839665, /// 0x948
			 0x5f550528, /// 0x94c
			 0xb5d76dc9, /// 0x950
			 0x4d97715c, /// 0x954
			 0xb3b0541c, /// 0x958
			 0xb0670e95, /// 0x95c
			 0xd563465d, /// 0x960
			 0x1be5abde, /// 0x964
			 0xb3a4b754, /// 0x968
			 0xcbe62e9a, /// 0x96c
			 0xe417238c, /// 0x970
			 0x5a505d99, /// 0x974
			 0x6559092a, /// 0x978
			 0x60e08681, /// 0x97c
			 0x377fa212, /// 0x980
			 0xfdd947cc, /// 0x984
			 0x557c09ea, /// 0x988
			 0x095e70e1, /// 0x98c
			 0x083a4d24, /// 0x990
			 0xbf2357a2, /// 0x994
			 0x5ae7ed97, /// 0x998
			 0x62e2a248, /// 0x99c
			 0x275c93b1, /// 0x9a0
			 0xd10a0724, /// 0x9a4
			 0xdcd72ad2, /// 0x9a8
			 0x618d2d09, /// 0x9ac
			 0x98258240, /// 0x9b0
			 0x660528f2, /// 0x9b4
			 0x9c8c15d8, /// 0x9b8
			 0x234a3091, /// 0x9bc
			 0xddce148d, /// 0x9c0
			 0xc482d72c, /// 0x9c4
			 0x41b5dc16, /// 0x9c8
			 0xf3e5ebb3, /// 0x9cc
			 0x8c16caa8, /// 0x9d0
			 0xea7ac24d, /// 0x9d4
			 0x959bb113, /// 0x9d8
			 0x3d4b5f24, /// 0x9dc
			 0x63f78351, /// 0x9e0
			 0x296bfae9, /// 0x9e4
			 0x8cee362c, /// 0x9e8
			 0x51e34e9f, /// 0x9ec
			 0x95d8beb5, /// 0x9f0
			 0xddc74a93, /// 0x9f4
			 0x58ce10b4, /// 0x9f8
			 0xcafb513c, /// 0x9fc
			 0x98465bdf, /// 0xa00
			 0x079b0314, /// 0xa04
			 0xcff44b2b, /// 0xa08
			 0xb629b332, /// 0xa0c
			 0x16194580, /// 0xa10
			 0x4e729e1d, /// 0xa14
			 0x9d9797c8, /// 0xa18
			 0xe39d4429, /// 0xa1c
			 0x4844a9a9, /// 0xa20
			 0x6484df5a, /// 0xa24
			 0x801a4cc4, /// 0xa28
			 0xd841e4d3, /// 0xa2c
			 0x79023567, /// 0xa30
			 0x24ec3565, /// 0xa34
			 0xe0955940, /// 0xa38
			 0x8e7ac8e4, /// 0xa3c
			 0x48bb548d, /// 0xa40
			 0x3fbff85a, /// 0xa44
			 0x087c452f, /// 0xa48
			 0x1ec453e0, /// 0xa4c
			 0xa75aea57, /// 0xa50
			 0xa944a85d, /// 0xa54
			 0xc2f3c14a, /// 0xa58
			 0x665e8aa9, /// 0xa5c
			 0x422564f1, /// 0xa60
			 0x5dc7ef52, /// 0xa64
			 0xee0f4923, /// 0xa68
			 0xe932f0c3, /// 0xa6c
			 0xdb498283, /// 0xa70
			 0xf2c27049, /// 0xa74
			 0xbc833bde, /// 0xa78
			 0x1ce64d4e, /// 0xa7c
			 0xd403c29c, /// 0xa80
			 0xea36f345, /// 0xa84
			 0x4c65b5d4, /// 0xa88
			 0xc15d2f54, /// 0xa8c
			 0x1aa410f4, /// 0xa90
			 0x47e5c4ec, /// 0xa94
			 0xa6eaa553, /// 0xa98
			 0xea665ef9, /// 0xa9c
			 0x270c0c75, /// 0xaa0
			 0x572c12cd, /// 0xaa4
			 0x3892b328, /// 0xaa8
			 0x996bbf45, /// 0xaac
			 0xd3eb5640, /// 0xab0
			 0xb04c5913, /// 0xab4
			 0xe910a533, /// 0xab8
			 0x4b44e69c, /// 0xabc
			 0xa02aec30, /// 0xac0
			 0x97c5ef19, /// 0xac4
			 0x9a59053d, /// 0xac8
			 0x3c1ebada, /// 0xacc
			 0x8395e12d, /// 0xad0
			 0xbd58d047, /// 0xad4
			 0xcdbce1d0, /// 0xad8
			 0x052686c8, /// 0xadc
			 0x8fe8db80, /// 0xae0
			 0xe62f73db, /// 0xae4
			 0x6311ec87, /// 0xae8
			 0x6b8cf7ab, /// 0xaec
			 0xe08f7114, /// 0xaf0
			 0x8a8821dd, /// 0xaf4
			 0xc05edad0, /// 0xaf8
			 0xe202d76c, /// 0xafc
			 0xa3e907b9, /// 0xb00
			 0x18560c9a, /// 0xb04
			 0x23ed991b, /// 0xb08
			 0x00c0140c, /// 0xb0c
			 0x58157615, /// 0xb10
			 0x10f524eb, /// 0xb14
			 0xa5e6f9f5, /// 0xb18
			 0x9d119fe8, /// 0xb1c
			 0x3d78a607, /// 0xb20
			 0xaf04dce5, /// 0xb24
			 0x1fe15894, /// 0xb28
			 0x19044922, /// 0xb2c
			 0x5946d37b, /// 0xb30
			 0x7b068142, /// 0xb34
			 0x78df32fe, /// 0xb38
			 0x28fe31ba, /// 0xb3c
			 0x61c16d0d, /// 0xb40
			 0xeb7f1633, /// 0xb44
			 0x61cc5c4d, /// 0xb48
			 0x7992ee82, /// 0xb4c
			 0x783ea159, /// 0xb50
			 0x81e618f4, /// 0xb54
			 0xe3e18dc9, /// 0xb58
			 0xb923c8d6, /// 0xb5c
			 0x6453577b, /// 0xb60
			 0x3238ff48, /// 0xb64
			 0xd7ea94bf, /// 0xb68
			 0x323e1e41, /// 0xb6c
			 0x427480e2, /// 0xb70
			 0x79993997, /// 0xb74
			 0xba2cd24b, /// 0xb78
			 0x195ff76e, /// 0xb7c
			 0x86c52315, /// 0xb80
			 0xdad503d4, /// 0xb84
			 0x82101e72, /// 0xb88
			 0x98c1373c, /// 0xb8c
			 0x65e3849a, /// 0xb90
			 0x18ad591d, /// 0xb94
			 0xd92dbe80, /// 0xb98
			 0x3b8ec7ed, /// 0xb9c
			 0xe4768411, /// 0xba0
			 0x987c7533, /// 0xba4
			 0xee94d210, /// 0xba8
			 0xb6888945, /// 0xbac
			 0x700034d6, /// 0xbb0
			 0xa9da950c, /// 0xbb4
			 0xe764dd59, /// 0xbb8
			 0xa017e9ab, /// 0xbbc
			 0xe8dbf2b0, /// 0xbc0
			 0xc8c2ba9e, /// 0xbc4
			 0x5a284009, /// 0xbc8
			 0x69fa0950, /// 0xbcc
			 0x475b5474, /// 0xbd0
			 0x0cf8f3eb, /// 0xbd4
			 0xa2b229e7, /// 0xbd8
			 0x734008b4, /// 0xbdc
			 0xb5b2c5e2, /// 0xbe0
			 0xd4b547ac, /// 0xbe4
			 0xe326be24, /// 0xbe8
			 0x151c5c40, /// 0xbec
			 0x8e1c7128, /// 0xbf0
			 0xf8685f92, /// 0xbf4
			 0x87e39d49, /// 0xbf8
			 0x5b081224, /// 0xbfc
			 0xde7d40c4, /// 0xc00
			 0x969a547e, /// 0xc04
			 0x8b0d78ff, /// 0xc08
			 0x4b7f7e4e, /// 0xc0c
			 0x8b0da4a8, /// 0xc10
			 0x6580051d, /// 0xc14
			 0x3fae0e75, /// 0xc18
			 0x6f995a60, /// 0xc1c
			 0x41c8f856, /// 0xc20
			 0x76838c31, /// 0xc24
			 0x82edbdf7, /// 0xc28
			 0x1266e3a2, /// 0xc2c
			 0x2e9674b4, /// 0xc30
			 0x2f624bf9, /// 0xc34
			 0x9406934a, /// 0xc38
			 0x7a369f3e, /// 0xc3c
			 0x8799e500, /// 0xc40
			 0x8bf6fbf6, /// 0xc44
			 0x5f72adc6, /// 0xc48
			 0xa297e7a6, /// 0xc4c
			 0x2fdf7c32, /// 0xc50
			 0x9c30f90e, /// 0xc54
			 0xbe063a9a, /// 0xc58
			 0x651e1d77, /// 0xc5c
			 0x79475e15, /// 0xc60
			 0x09c4cc0d, /// 0xc64
			 0x084f1919, /// 0xc68
			 0x88713fc8, /// 0xc6c
			 0x25dcaabd, /// 0xc70
			 0x45f089e2, /// 0xc74
			 0x55ab4968, /// 0xc78
			 0x7af81442, /// 0xc7c
			 0xe18dff6a, /// 0xc80
			 0x9bc7dedf, /// 0xc84
			 0xa7573844, /// 0xc88
			 0xbf4c9e35, /// 0xc8c
			 0x8d29addf, /// 0xc90
			 0x9ee94e7d, /// 0xc94
			 0xf0378704, /// 0xc98
			 0x0f65224f, /// 0xc9c
			 0xc31ea7bf, /// 0xca0
			 0xa3b97145, /// 0xca4
			 0xc74b571b, /// 0xca8
			 0x8434d7d6, /// 0xcac
			 0x40ca931a, /// 0xcb0
			 0xb2e4b7fd, /// 0xcb4
			 0x5e9c3fb9, /// 0xcb8
			 0x5677c5ed, /// 0xcbc
			 0x6e53bf86, /// 0xcc0
			 0x78a0bd69, /// 0xcc4
			 0x7e9f47e0, /// 0xcc8
			 0xbd513db9, /// 0xccc
			 0xfd183b12, /// 0xcd0
			 0x311c78a5, /// 0xcd4
			 0xa5c6dc69, /// 0xcd8
			 0xbd5d8ca3, /// 0xcdc
			 0x5b785181, /// 0xce0
			 0x869a9f5c, /// 0xce4
			 0x2c268a80, /// 0xce8
			 0xf32b979e, /// 0xcec
			 0x35ca8829, /// 0xcf0
			 0x70035ae1, /// 0xcf4
			 0xf82ee2ef, /// 0xcf8
			 0x3cc810a8, /// 0xcfc
			 0xa47d660a, /// 0xd00
			 0xba2fc90d, /// 0xd04
			 0x4fb45474, /// 0xd08
			 0x6ce7627a, /// 0xd0c
			 0x96c0eff6, /// 0xd10
			 0x1af85f4b, /// 0xd14
			 0xd5fb6a29, /// 0xd18
			 0x5ada5c7f, /// 0xd1c
			 0x2b69a1bf, /// 0xd20
			 0xde5efbc9, /// 0xd24
			 0x9870babf, /// 0xd28
			 0x13d65bf5, /// 0xd2c
			 0x9484c031, /// 0xd30
			 0x549ceec0, /// 0xd34
			 0x8b3f0dfc, /// 0xd38
			 0x636d363f, /// 0xd3c
			 0x6cb8a1cf, /// 0xd40
			 0x9eb68b93, /// 0xd44
			 0x5eb4ab3b, /// 0xd48
			 0x1dedff4d, /// 0xd4c
			 0x69e5e0bb, /// 0xd50
			 0xa24b6d2f, /// 0xd54
			 0x1e3753f5, /// 0xd58
			 0x0231c22c, /// 0xd5c
			 0x3bdc079c, /// 0xd60
			 0xd5b23eaa, /// 0xd64
			 0x2811fc47, /// 0xd68
			 0x132de797, /// 0xd6c
			 0x0e093461, /// 0xd70
			 0x70d13a76, /// 0xd74
			 0x29ea4659, /// 0xd78
			 0x6b435569, /// 0xd7c
			 0x220e0b7d, /// 0xd80
			 0x58608be0, /// 0xd84
			 0x801dca8e, /// 0xd88
			 0xd83939c7, /// 0xd8c
			 0xd0c32262, /// 0xd90
			 0x920d3d1f, /// 0xd94
			 0xb07cee4d, /// 0xd98
			 0x24af9985, /// 0xd9c
			 0xe34f68d8, /// 0xda0
			 0x422315da, /// 0xda4
			 0x5c616740, /// 0xda8
			 0x7e510cd5, /// 0xdac
			 0x1808b1e4, /// 0xdb0
			 0xe58861d7, /// 0xdb4
			 0x87ce92ff, /// 0xdb8
			 0x45431b51, /// 0xdbc
			 0x20715649, /// 0xdc0
			 0xfc0a7cb1, /// 0xdc4
			 0xc80d8704, /// 0xdc8
			 0x89cc1040, /// 0xdcc
			 0x6736a961, /// 0xdd0
			 0x2424e83c, /// 0xdd4
			 0x0b6226ba, /// 0xdd8
			 0x79f1a880, /// 0xddc
			 0x15a7f06e, /// 0xde0
			 0xe6515ec7, /// 0xde4
			 0x2a1dd7c5, /// 0xde8
			 0x13127548, /// 0xdec
			 0x87b68576, /// 0xdf0
			 0xba1812cf, /// 0xdf4
			 0x6c269364, /// 0xdf8
			 0x73e73966, /// 0xdfc
			 0xd37b30f8, /// 0xe00
			 0x7d51f5c2, /// 0xe04
			 0xee59f759, /// 0xe08
			 0xab997d1e, /// 0xe0c
			 0xb8b4bc12, /// 0xe10
			 0x81574ccc, /// 0xe14
			 0x5022a59f, /// 0xe18
			 0x78893437, /// 0xe1c
			 0xec1e5b1f, /// 0xe20
			 0x69a1858d, /// 0xe24
			 0x7d413096, /// 0xe28
			 0xa3b21dcf, /// 0xe2c
			 0xb3071612, /// 0xe30
			 0xbc3150ce, /// 0xe34
			 0x1f20ee46, /// 0xe38
			 0x501e2896, /// 0xe3c
			 0xf8baf3d0, /// 0xe40
			 0x60eb97af, /// 0xe44
			 0xdc38e4f2, /// 0xe48
			 0xc41b63fe, /// 0xe4c
			 0xf09c0873, /// 0xe50
			 0x2b7e6f41, /// 0xe54
			 0x8952a98e, /// 0xe58
			 0x4bc085c5, /// 0xe5c
			 0x85a5d335, /// 0xe60
			 0x4571dbdf, /// 0xe64
			 0x8f630e52, /// 0xe68
			 0x49eba246, /// 0xe6c
			 0xaac7ab0a, /// 0xe70
			 0xeadbd62a, /// 0xe74
			 0x642802bd, /// 0xe78
			 0xafb3a352, /// 0xe7c
			 0x8964affa, /// 0xe80
			 0x59a83b64, /// 0xe84
			 0xb042c6f2, /// 0xe88
			 0xf6cefa84, /// 0xe8c
			 0x55747f45, /// 0xe90
			 0x5bda580f, /// 0xe94
			 0x58da39bf, /// 0xe98
			 0xdecaaf2b, /// 0xe9c
			 0x931a7dff, /// 0xea0
			 0x837b3a29, /// 0xea4
			 0x6ceb9e18, /// 0xea8
			 0x85010621, /// 0xeac
			 0xf00a4ac3, /// 0xeb0
			 0xb8fcb6a3, /// 0xeb4
			 0x26664698, /// 0xeb8
			 0x2e68b302, /// 0xebc
			 0x9e75e734, /// 0xec0
			 0xa4eb4a67, /// 0xec4
			 0x5a11a3ea, /// 0xec8
			 0xef39c1f0, /// 0xecc
			 0x5d76aff7, /// 0xed0
			 0xccafe31a, /// 0xed4
			 0x993a561a, /// 0xed8
			 0x47e68bca, /// 0xedc
			 0x5bfac16d, /// 0xee0
			 0x21a9233f, /// 0xee4
			 0x9f7d2d4d, /// 0xee8
			 0xec16e800, /// 0xeec
			 0x070dee7e, /// 0xef0
			 0x4c8d5195, /// 0xef4
			 0x4bc16441, /// 0xef8
			 0xbb8b7ac6, /// 0xefc
			 0x2c4f4d35, /// 0xf00
			 0xa2274246, /// 0xf04
			 0xf1ea1a44, /// 0xf08
			 0x0ca2ed49, /// 0xf0c
			 0x43cde84d, /// 0xf10
			 0xdfc80f7f, /// 0xf14
			 0xef8f2a92, /// 0xf18
			 0x6caf277b, /// 0xf1c
			 0xd3cf424a, /// 0xf20
			 0x43670c4a, /// 0xf24
			 0x8d5f89bb, /// 0xf28
			 0x2b61f93a, /// 0xf2c
			 0x50f04b1b, /// 0xf30
			 0xee49bd47, /// 0xf34
			 0xf16aa80f, /// 0xf38
			 0x40aa3feb, /// 0xf3c
			 0x8eba7ac6, /// 0xf40
			 0x7c6ef918, /// 0xf44
			 0x1f7fc3cd, /// 0xf48
			 0xa1247397, /// 0xf4c
			 0xa5badafc, /// 0xf50
			 0xafe0dba9, /// 0xf54
			 0x1f644d96, /// 0xf58
			 0x59407ac5, /// 0xf5c
			 0xb337147e, /// 0xf60
			 0x13a56ff8, /// 0xf64
			 0x2c0f0e84, /// 0xf68
			 0x631f94b6, /// 0xf6c
			 0x52993c0c, /// 0xf70
			 0x3f50987a, /// 0xf74
			 0x76a509a3, /// 0xf78
			 0xd646df1f, /// 0xf7c
			 0x02db03b2, /// 0xf80
			 0xf21315be, /// 0xf84
			 0x5cba2108, /// 0xf88
			 0xae07b684, /// 0xf8c
			 0x0b3f3dd0, /// 0xf90
			 0xbcd313d0, /// 0xf94
			 0x9e81b40a, /// 0xf98
			 0x529b006b, /// 0xf9c
			 0xcaaa4231, /// 0xfa0
			 0xb073d1dc, /// 0xfa4
			 0x9528d68e, /// 0xfa8
			 0xd197866b, /// 0xfac
			 0x77a6c236, /// 0xfb0
			 0xe1afa4bc, /// 0xfb4
			 0x27d64f59, /// 0xfb8
			 0x3ed800c9, /// 0xfbc
			 0x4a74c8d3, /// 0xfc0
			 0x30594e25, /// 0xfc4
			 0xf34f1eba, /// 0xfc8
			 0x52ea650d, /// 0xfcc
			 0xa60734ae, /// 0xfd0
			 0xae64c20c, /// 0xfd4
			 0x1b8904e1, /// 0xfd8
			 0xd84db266, /// 0xfdc
			 0x2ca9cacb, /// 0xfe0
			 0x3ab0e977, /// 0xfe4
			 0x36befe20, /// 0xfe8
			 0xbf11fa46, /// 0xfec
			 0xafc5dc3e, /// 0xff0
			 0xef5ba34c, /// 0xff4
			 0xd279edd3, /// 0xff8
			 0x462b15ac /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00000
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00004
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00008
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0000c
			 0x7f800000,                                                  // inf                                         /// 00010
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00014
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00018
			 0x80000000,                                                  // -zero                                       /// 0001c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00020
			 0x7f800000,                                                  // inf                                         /// 00024
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0xff800000,                                                  // -inf                                        /// 0002c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00030
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00034
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00038
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00040
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00048
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00050
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00054
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00058
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0005c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00068
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00074
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0007c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00080
			 0x0c700000,                                                  // Leading 1s:                                 /// 00084
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00088
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0008c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00090
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00100
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0010c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00110
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00114
			 0xffc00001,                                                  // -signaling NaN                              /// 00118
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0011c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00124
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0c600000,                                                  // Leading 1s:                                 /// 0012c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00130
			 0x0e000001,                                                  // Trailing 1s:                                /// 00134
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00138
			 0x0e000007,                                                  // Trailing 1s:                                /// 0013c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00140
			 0x4b000000,                                                  // 8388608.0                                   /// 00144
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00148
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0014c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00158
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0015c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00168
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0016c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00170
			 0x0c600000,                                                  // Leading 1s:                                 /// 00174
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00180
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0018c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00190
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00194
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00198
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0019c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x3f028f5c,                                                  // 0.51                                        /// 001ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x3f028f5c,                                                  // 0.51                                        /// 001b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 001dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001fc
			 0x33333333,                                                  // 4 random values                             /// 00200
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00210
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00218
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00220
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00224
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00228
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0022c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00234
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00238
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0023c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00240
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00244
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00248
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00250
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00254
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0025c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00264
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00270
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00274
			 0x00000000,                                                  // zero                                        /// 00278
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0027c
			 0xffc00001,                                                  // -signaling NaN                              /// 00280
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00284
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00288
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00290
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00294
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00298
			 0x33333333,                                                  // 4 random values                             /// 0029c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00304
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00308
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0030c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00314
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00318
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0xbf028f5c,                                                  // -0.51                                       /// 00320
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00324
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x4b000000,                                                  // 8388608.0                                   /// 0032c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00330
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00340
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00348
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00350
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00354
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00358
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0035c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00360
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00364
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00368
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0036c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00370
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00380
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00384
			 0x3f028f5c,                                                  // 0.51                                        /// 00388
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00394
			 0x0c400000,                                                  // Leading 1s:                                 /// 00398
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0xcb000000,                                                  // -8388608.0                                  /// 003f0
			 0x00000000,                                                  // zero                                        /// 003f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00400
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0040c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00410
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00414
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00420
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00424
			 0x0c700000,                                                  // Leading 1s:                                 /// 00428
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00434
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00438
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0043c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00440
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00448
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00454
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00458
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00460
			 0x0e000001,                                                  // Trailing 1s:                                /// 00464
			 0xbf028f5c,                                                  // -0.51                                       /// 00468
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0046c
			 0x00000000,                                                  // zero                                        /// 00470
			 0xff800000,                                                  // -inf                                        /// 00474
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00478
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00480
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00484
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00488
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xffc00001,                                                  // -signaling NaN                              /// 00490
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0e000003,                                                  // Trailing 1s:                                /// 0049c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a0
			 0xcb000000,                                                  // -8388608.0                                  /// 004a4
			 0xff800000,                                                  // -inf                                        /// 004a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 004c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 004c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004f0
			 0xff800000,                                                  // -inf                                        /// 004f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00500
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00504
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00508
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00514
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00518
			 0x7fc00001,                                                  // signaling NaN                               /// 0051c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00520
			 0x0c600000,                                                  // Leading 1s:                                 /// 00524
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x55555555,                                                  // 4 random values                             /// 0052c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00530
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0053c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00540
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00544
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00550
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00554
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00558
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00564
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00568
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0056c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0xbf028f5c,                                                  // -0.51                                       /// 00578
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00584
			 0xbf028f5c,                                                  // -0.51                                       /// 00588
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00590
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00598
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0059c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80000000,                                                  // -zero                                       /// 005fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00604
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00608
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0060c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x33333333,                                                  // 4 random values                             /// 00614
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00618
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0061c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00620
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00624
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0062c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00630
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0063c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00650
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00654
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00658
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0066c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00670
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00674
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00678
			 0x00011111,                                                  // 9.7958E-41                                  /// 0067c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00680
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00684
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0068c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00690
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00694
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0069c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006d4
			 0x7f800000,                                                  // inf                                         /// 006d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 006e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x80011111,                                                  // -9.7958E-41                                 /// 00704
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00708
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0070c
			 0xbf028f5c,                                                  // -0.51                                       /// 00710
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00714
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00728
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0072c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00730
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00734
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00740
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00000000,                                                  // zero                                        /// 0074c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00750
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80011111,                                                  // -9.7958E-41                                 /// 00758
			 0x0e000001,                                                  // Trailing 1s:                                /// 0075c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00764
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0076c
			 0x55555555,                                                  // 4 random values                             /// 00770
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00774
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0xcb000000,                                                  // -8388608.0                                  /// 00780
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00794
			 0x80011111,                                                  // -9.7958E-41                                 /// 00798
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0079c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x80000000,                                                  // -zero                                       /// 007a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 007d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 007dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x33333333,                                                  // 4 random values                             /// 007ec
			 0x33333333,                                                  // 4 random values                             /// 007f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007f4
			 0xff800000,                                                  // -inf                                        /// 007f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0080c
			 0x80000000,                                                  // -zero                                       /// 00810
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00814
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00818
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0081c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00820
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00828
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00830
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00834
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00838
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0083c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00844
			 0x0e000003,                                                  // Trailing 1s:                                /// 00848
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00854
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00858
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0xff800000,                                                  // -inf                                        /// 00864
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00868
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00870
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00874
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00884
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0088c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00890
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0089c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x3f028f5c,                                                  // 0.51                                        /// 008a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 008b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008e4
			 0xbf028f5c,                                                  // -0.51                                       /// 008e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008fc
			 0x00000000,                                                  // zero                                        /// 00900
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00904
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00908
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0090c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00918
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x55555555,                                                  // 4 random values                             /// 00920
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00930
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00934
			 0x33333333,                                                  // 4 random values                             /// 00938
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0093c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00948
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0094c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00954
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00000000,                                                  // zero                                        /// 00960
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00964
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00970
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00974
			 0x33333333,                                                  // 4 random values                             /// 00978
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0097c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00980
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00984
			 0x7f800000,                                                  // inf                                         /// 00988
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0098c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00990
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00994
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00998
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0099c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009cc
			 0xff800000,                                                  // -inf                                        /// 009d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009e0
			 0x7f800000,                                                  // inf                                         /// 009e4
			 0x80000000,                                                  // -zero                                       /// 009e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0xffc00001,                                                  // -signaling NaN                              /// 00a28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a2c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x80000000,                                                  // -zero                                       /// 00a48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x00000000,                                                  // zero                                        /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a8c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a94
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x55555555,                                                  // 4 random values                             /// 00aa0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00aa8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ab0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ab4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ab8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ac8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00acc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ad8
			 0xcb000000,                                                  // -8388608.0                                  /// 00adc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ae0
			 0x55555555,                                                  // 4 random values                             /// 00ae4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ae8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00af0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00af4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00af8
			 0xffc00001,                                                  // -signaling NaN                              /// 00afc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b08
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b0c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b4c
			 0x33333333,                                                  // 4 random values                             /// 00b50
			 0xffc00001,                                                  // -signaling NaN                              /// 00b54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b68
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00000000,                                                  // zero                                        /// 00b70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b88
			 0xcb000000,                                                  // -8388608.0                                  /// 00b8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b94
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ba4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ba8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bc0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bdc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bf8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c08
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c14
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c24
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c30
			 0x55555555,                                                  // 4 random values                             /// 00c34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c40
			 0x4b000000,                                                  // 8388608.0                                   /// 00c44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c50
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c58
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c6c
			 0x7f800000,                                                  // inf                                         /// 00c70
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ca8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cc0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ccc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cdc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cf0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d24
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0xff800000,                                                  // -inf                                        /// 00d64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d88
			 0xff800000,                                                  // -inf                                        /// 00d8c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d90
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d98
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00da4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00db0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dc8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dcc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dd0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x33333333,                                                  // 4 random values                             /// 00ddc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00de0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00de8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00df8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x55555555,                                                  // 4 random values                             /// 00e0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e18
			 0xbf028f5c,                                                  // -0.51                                       /// 00e1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e38
			 0xcb000000,                                                  // -8388608.0                                  /// 00e3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e50
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e5c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x7fc00001,                                                  // signaling NaN                               /// 00e68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00eb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ec0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ec4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ec8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ecc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ed0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ed8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00edc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ee0
			 0x33333333,                                                  // 4 random values                             /// 00ee4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ef0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00efc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f08
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f18
			 0x00000000,                                                  // zero                                        /// 00f1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f5c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f70
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f80
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f98
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fa8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fb0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fb4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fc8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fe4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fe8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ff8
			 0x0f7ffffd                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0c400000,                                                  // Leading 1s:                                 /// 00000
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00004
			 0x33333333,                                                  // 4 random values                             /// 00008
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00010
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00014
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00018
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00028
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0002c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0003c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00050
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0005c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00060
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00064
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00070
			 0x7fc00001,                                                  // signaling NaN                               /// 00074
			 0xcb000000,                                                  // -8388608.0                                  /// 00078
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00084
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00088
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0008c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x7f800000,                                                  // inf                                         /// 00094
			 0xffc00001,                                                  // -signaling NaN                              /// 00098
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x80000000,                                                  // -zero                                       /// 000b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000ec
			 0x33333333,                                                  // 4 random values                             /// 000f0
			 0x7fc00001,                                                  // signaling NaN                               /// 000f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00100
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00104
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00110
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00114
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00128
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0012c
			 0xcb000000,                                                  // -8388608.0                                  /// 00130
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0013c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00140
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0014c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00150
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00154
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0015c
			 0xbf028f5c,                                                  // -0.51                                       /// 00160
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00168
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00170
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00174
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00180
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00184
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0018c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00190
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0019c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001c4
			 0x7fc00001,                                                  // signaling NaN                               /// 001c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x55555555,                                                  // 4 random values                             /// 001fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00200
			 0x80000000,                                                  // -zero                                       /// 00204
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00208
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00214
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00218
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0021c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00228
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00230
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00234
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00238
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0023c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00240
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00244
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00248
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0024c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00250
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00254
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00258
			 0x00000000,                                                  // zero                                        /// 0025c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00264
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0026c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00270
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00274
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0027c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00280
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0028c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0029c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x80000000,                                                  // -zero                                       /// 002c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 002e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002e4
			 0x4b000000,                                                  // 8388608.0                                   /// 002e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00304
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00308
			 0x0e000007,                                                  // Trailing 1s:                                /// 0030c
			 0x80000000,                                                  // -zero                                       /// 00310
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00318
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0031c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00320
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00324
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00328
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00340
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00344
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0034c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00354
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00358
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00360
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00364
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00368
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0036c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00370
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00374
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00384
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0038c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00394
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0039c
			 0x7f800000,                                                  // inf                                         /// 003a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003ec
			 0x80000000,                                                  // -zero                                       /// 003f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00400
			 0x00000000,                                                  // zero                                        /// 00404
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0040c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00410
			 0x0c700000,                                                  // Leading 1s:                                 /// 00414
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0e000003,                                                  // Trailing 1s:                                /// 00424
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00428
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00430
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00438
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0043c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00440
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x7fc00001,                                                  // signaling NaN                               /// 00454
			 0x0c780000,                                                  // Leading 1s:                                 /// 00458
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00000000,                                                  // zero                                        /// 00460
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00468
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00470
			 0x0c780000,                                                  // Leading 1s:                                 /// 00474
			 0x4b000000,                                                  // 8388608.0                                   /// 00478
			 0x0e000001,                                                  // Trailing 1s:                                /// 0047c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00484
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00488
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00490
			 0x7f800000,                                                  // inf                                         /// 00494
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00498
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0049c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004a8
			 0xffc00001,                                                  // -signaling NaN                              /// 004ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0xff800000,                                                  // -inf                                        /// 004b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004cc
			 0x7fc00001,                                                  // signaling NaN                               /// 004d0
			 0x4b000000,                                                  // 8388608.0                                   /// 004d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x7f800000,                                                  // inf                                         /// 004f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00500
			 0x4b000000,                                                  // 8388608.0                                   /// 00504
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00508
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0050c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00510
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00518
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00520
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00524
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00530
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00534
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00538
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00540
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00548
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0054c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00550
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0055c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00564
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00568
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00570
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00574
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0057c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00588
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0058c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00594
			 0xbf028f5c,                                                  // -0.51                                       /// 00598
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005a0
			 0x33333333,                                                  // 4 random values                             /// 005a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x4b000000,                                                  // 8388608.0                                   /// 005c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005e8
			 0xcb000000,                                                  // -8388608.0                                  /// 005ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0060c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00610
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00618
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0061c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0062c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00630
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x80011111,                                                  // -9.7958E-41                                 /// 00638
			 0x4b000000,                                                  // 8388608.0                                   /// 0063c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00644
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00648
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00654
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00658
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0065c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00660
			 0x0e000003,                                                  // Trailing 1s:                                /// 00664
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00668
			 0x7f800000,                                                  // inf                                         /// 0066c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00670
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0xff800000,                                                  // -inf                                        /// 0067c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00680
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00684
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0068c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00698
			 0x7f800000,                                                  // inf                                         /// 0069c
			 0x80000000,                                                  // -zero                                       /// 006a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006bc
			 0x33333333,                                                  // 4 random values                             /// 006c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006d0
			 0x3f028f5c,                                                  // 0.51                                        /// 006d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x00000000,                                                  // zero                                        /// 006f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00700
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00704
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00708
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0070c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00714
			 0x0e000007,                                                  // Trailing 1s:                                /// 00718
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0071c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00720
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00728
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00734
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00738
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0073c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00740
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00744
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00750
			 0x80000000,                                                  // -zero                                       /// 00754
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00758
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0075c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00768
			 0x7f800000,                                                  // inf                                         /// 0076c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00774
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00778
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0077c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00780
			 0xffc00001,                                                  // -signaling NaN                              /// 00784
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00788
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0078c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00794
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00798
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x3f028f5c,                                                  // 0.51                                        /// 007ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007b4
			 0x00000000,                                                  // zero                                        /// 007b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00800
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00804
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0081c
			 0xffc00001,                                                  // -signaling NaN                              /// 00820
			 0x0e000003,                                                  // Trailing 1s:                                /// 00824
			 0x0c780000,                                                  // Leading 1s:                                 /// 00828
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00830
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00838
			 0x7f800000,                                                  // inf                                         /// 0083c
			 0x80000000,                                                  // -zero                                       /// 00840
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00848
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0084c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00850
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00854
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00860
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0086c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00870
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0087c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00880
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00884
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00888
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0088c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x7f800000,                                                  // inf                                         /// 00894
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0089c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00000000,                                                  // zero                                        /// 008f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00908
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00910
			 0x0c700000,                                                  // Leading 1s:                                 /// 00914
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00920
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00924
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00928
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0092c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00930
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00934
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00938
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0093c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00940
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00944
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x0c780000,                                                  // Leading 1s:                                 /// 0094c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00950
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00954
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00958
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0095c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00964
			 0x0c400000,                                                  // Leading 1s:                                 /// 00968
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0096c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00970
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00974
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00978
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00980
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0c400000,                                                  // Leading 1s:                                 /// 0098c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00990
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00994
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x4b000000,                                                  // 8388608.0                                   /// 009b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a18
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a50
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a84
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a98
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aa0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x3f028f5c,                                                  // 0.51                                        /// 00aa8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00aac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ab0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ab8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ac0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ad4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ad8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00af4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00af8
			 0xbf028f5c,                                                  // -0.51                                       /// 00afc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b0c
			 0x00000000,                                                  // zero                                        /// 00b10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b1c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b20
			 0x7fc00001,                                                  // signaling NaN                               /// 00b24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b44
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b78
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b84
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b9c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ba4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bb0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bcc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bd0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bd4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bdc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00be0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00be4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bfc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c08
			 0xff800000,                                                  // -inf                                        /// 00c0c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c18
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c3c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c48
			 0x80000000,                                                  // -zero                                       /// 00c4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c54
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c68
			 0xffc00001,                                                  // -signaling NaN                              /// 00c6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0xff800000,                                                  // -inf                                        /// 00c7c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c88
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ca0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cb8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cbc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cc8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ccc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cdc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ce0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ce4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ce8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cf4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d0c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d14
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d1c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x33333333,                                                  // 4 random values                             /// 00d24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d70
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d98
			 0x33333333,                                                  // 4 random values                             /// 00d9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00da0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00da8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00db0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00db8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dbc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dc0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dc4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dd0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00de0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00de8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00df4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00df8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dfc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e14
			 0xcb000000,                                                  // -8388608.0                                  /// 00e18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e54
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e64
			 0x7f800000,                                                  // inf                                         /// 00e68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e6c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e70
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e80
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e8c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e9c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ea0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ea8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ec0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ec4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ec8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ecc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ed0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00edc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ee0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ee4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ee8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00eec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ef0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ef4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ef8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f14
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f3c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f48
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f74
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fa0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fa4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fa8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fb4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fbc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fc4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fcc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fd0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0xbf028f5c,                                                  // -0.51                                       /// 00fe0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fe8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fec
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ff4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ff8
			 0x00800000                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00000
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00004
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00008
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0000c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0001c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00020
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00024
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00028
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00030
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00048
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00054
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00058
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0005c
			 0x4b000000,                                                  // 8388608.0                                   /// 00060
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00068
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000000,                                                  // -zero                                       /// 00070
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00074
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00078
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0007c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00080
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00084
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00088
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0008c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00090
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00098
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0009c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0xbf028f5c,                                                  // -0.51                                       /// 000a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000d8
			 0x55555555,                                                  // 4 random values                             /// 000dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000f4
			 0x4b000000,                                                  // 8388608.0                                   /// 000f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00100
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00104
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00108
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00110
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00118
			 0x0e000003,                                                  // Trailing 1s:                                /// 0011c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00120
			 0x0e000001,                                                  // Trailing 1s:                                /// 00124
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0012c
			 0x7f800000,                                                  // inf                                         /// 00130
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00134
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00138
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0013c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00140
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00148
			 0x3f028f5c,                                                  // 0.51                                        /// 0014c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00150
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00158
			 0x0e000003,                                                  // Trailing 1s:                                /// 0015c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00160
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00164
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00168
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0016c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00170
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00174
			 0x00011111,                                                  // 9.7958E-41                                  /// 00178
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0017c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00180
			 0xff800000,                                                  // -inf                                        /// 00184
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x7fc00001,                                                  // signaling NaN                               /// 0018c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00190
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00198
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0019c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001b8
			 0xff800000,                                                  // -inf                                        /// 001bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00200
			 0xbf028f5c,                                                  // -0.51                                       /// 00204
			 0x4b000000,                                                  // 8388608.0                                   /// 00208
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0020c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00214
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00218
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00220
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00224
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00228
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0022c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00238
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x4b000000,                                                  // 8388608.0                                   /// 00240
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e000001,                                                  // Trailing 1s:                                /// 00248
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0024c
			 0x55555555,                                                  // 4 random values                             /// 00250
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x80000000,                                                  // -zero                                       /// 00260
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0c400000,                                                  // Leading 1s:                                 /// 00268
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0026c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00274
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0027c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00284
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00288
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00290
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00294
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002e8
			 0x33333333,                                                  // 4 random values                             /// 002ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00300
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00308
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0030c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00310
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00318
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0031c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00324
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0032c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00330
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00334
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00338
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0033c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00348
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0034c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x3f028f5c,                                                  // 0.51                                        /// 00354
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00358
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00364
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00370
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00384
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00388
			 0x7f800000,                                                  // inf                                         /// 0038c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00394
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00398
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0xbf028f5c,                                                  // -0.51                                       /// 003a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003a4
			 0xffc00001,                                                  // -signaling NaN                              /// 003a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003ac
			 0x3f028f5c,                                                  // 0.51                                        /// 003b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003b8
			 0xbf028f5c,                                                  // -0.51                                       /// 003bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 003f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00400
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00404
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0040c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00414
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00424
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x4b000000,                                                  // 8388608.0                                   /// 00434
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00438
			 0x00000000,                                                  // zero                                        /// 0043c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00440
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00444
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0044c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00454
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00458
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00460
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00464
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00468
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00470
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00474
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00478
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00480
			 0x00011111,                                                  // 9.7958E-41                                  /// 00484
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0048c
			 0x7fc00001,                                                  // signaling NaN                               /// 00490
			 0x0c600000,                                                  // Leading 1s:                                 /// 00494
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00500
			 0x0e000007,                                                  // Trailing 1s:                                /// 00504
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0050c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00514
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00518
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0051c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00530
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00534
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00538
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0053c
			 0x00000000,                                                  // zero                                        /// 00540
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00548
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0054c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00550
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00554
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00558
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0055c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00568
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00570
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00574
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00578
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0057c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00580
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00584
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00598
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0059c
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00600
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00608
			 0x00011111,                                                  // 9.7958E-41                                  /// 0060c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00614
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00618
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0061c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00620
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00624
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00628
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0062c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x3f028f5c,                                                  // 0.51                                        /// 00634
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00638
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0063c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00644
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00648
			 0x00000000,                                                  // zero                                        /// 0064c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00650
			 0x4b000000,                                                  // 8388608.0                                   /// 00654
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00658
			 0x00000000,                                                  // zero                                        /// 0065c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00660
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00664
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0066c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00674
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0067c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00680
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00684
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0068c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00690
			 0x33333333,                                                  // 4 random values                             /// 00694
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00698
			 0x0c780000,                                                  // Leading 1s:                                 /// 0069c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006ac
			 0x7fc00001,                                                  // signaling NaN                               /// 006b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0xbf028f5c,                                                  // -0.51                                       /// 006e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x33333333,                                                  // 4 random values                             /// 006f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00700
			 0xcb000000,                                                  // -8388608.0                                  /// 00704
			 0xffc00001,                                                  // -signaling NaN                              /// 00708
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0071c
			 0x00000000,                                                  // zero                                        /// 00720
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00724
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0072c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00734
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00738
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0073c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00744
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00748
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0074c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00754
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00760
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00764
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00768
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0076c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00770
			 0x0c600000,                                                  // Leading 1s:                                 /// 00774
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0077c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00788
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00790
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00794
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00798
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0079c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 007b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007f0
			 0x7f800000,                                                  // inf                                         /// 007f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 007fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00800
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00804
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00808
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0080c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00810
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0081c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00820
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0xbf028f5c,                                                  // -0.51                                       /// 0082c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00830
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00834
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0083c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0xffc00001,                                                  // -signaling NaN                              /// 00844
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00848
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00850
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00854
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00858
			 0xff800000,                                                  // -inf                                        /// 0085c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00864
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0086c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00870
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00874
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00878
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00880
			 0xffc00001,                                                  // -signaling NaN                              /// 00884
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00888
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0088c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00894
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00898
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008b4
			 0xcb000000,                                                  // -8388608.0                                  /// 008b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0xcb000000,                                                  // -8388608.0                                  /// 008c8
			 0x4b000000,                                                  // 8388608.0                                   /// 008cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008dc
			 0x55555555,                                                  // 4 random values                             /// 008e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00908
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0090c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00910
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00914
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0091c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00920
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0092c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0093c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00944
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0094c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00964
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00968
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0096c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00970
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00974
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00980
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00988
			 0x0c600000,                                                  // Leading 1s:                                 /// 0098c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00994
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00998
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0099c
			 0xcb000000,                                                  // -8388608.0                                  /// 009a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009c8
			 0xff800000,                                                  // -inf                                        /// 009cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009d0
			 0xff800000,                                                  // -inf                                        /// 009d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00a00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a28
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a4c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a5c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a6c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a70
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a9c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aa8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ab0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ab4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ab8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ac0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ac4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ac8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ad8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00adc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ae8
			 0x00000000,                                                  // zero                                        /// 00aec
			 0x4b000000,                                                  // 8388608.0                                   /// 00af0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00af8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00afc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x00000000,                                                  // zero                                        /// 00b64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b7c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b88
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ba0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ba8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bb8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bbc
			 0x3f028f5c,                                                  // 0.51                                        /// 00bc0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bc4
			 0xcb000000,                                                  // -8388608.0                                  /// 00bc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x00000000,                                                  // zero                                        /// 00bd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bdc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00be0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00be4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bf8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bfc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c14
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c78
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0xff800000,                                                  // -inf                                        /// 00c84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c88
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ca0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ca4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ca8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cb0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cbc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cc4
			 0x7f800000,                                                  // inf                                         /// 00cc8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ccc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cdc
			 0x00000000,                                                  // zero                                        /// 00ce0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ce4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cf0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cf4
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d08
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d24
			 0x55555555,                                                  // 4 random values                             /// 00d28
			 0xff800000,                                                  // -inf                                        /// 00d2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d44
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d4c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x55555555,                                                  // 4 random values                             /// 00d60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d6c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d74
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00da4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00da8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00db4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x33333333,                                                  // 4 random values                             /// 00dbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dcc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00de0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00de4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00de8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00df0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00df8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e08
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e1c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e20
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e24
			 0xbf028f5c,                                                  // -0.51                                       /// 00e28
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e2c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e60
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e74
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e78
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e80
			 0xbf028f5c,                                                  // -0.51                                       /// 00e84
			 0xffc00001,                                                  // -signaling NaN                              /// 00e88
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ea0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ea4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ea8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ec4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ecc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ed0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ed8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00edc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ee8
			 0xcb000000,                                                  // -8388608.0                                  /// 00eec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ef8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f08
			 0xffc00001,                                                  // -signaling NaN                              /// 00f0c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f2c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0xbf028f5c,                                                  // -0.51                                       /// 00f48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f5c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f68
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f74
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f90
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fa0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fa4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fcc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fd0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fdc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fe0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fe8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ff4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ff8
			 0x4b000000                                                  // 8388608.0                                   /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c400000,                                                  // Leading 1s:                                 /// 00000
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00004
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0000c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00010
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00014
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00020
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00024
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0002c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0003c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00040
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00048
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00060
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00064
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00068
			 0x0c700000,                                                  // Leading 1s:                                 /// 0006c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00070
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00078
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0007c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00088
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00090
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00098
			 0x0e000001,                                                  // Trailing 1s:                                /// 0009c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 000b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0xffc00001,                                                  // -signaling NaN                              /// 000d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000e8
			 0x7fc00001,                                                  // signaling NaN                               /// 000ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0010c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0e000003,                                                  // Trailing 1s:                                /// 00114
			 0x0e000003,                                                  // Trailing 1s:                                /// 00118
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0011c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x7f800000,                                                  // inf                                         /// 00124
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00130
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00134
			 0x7f800000,                                                  // inf                                         /// 00138
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0013c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00140
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00144
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0014c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00150
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00154
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00158
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0015c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00160
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00164
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00168
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0016c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00170
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00174
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0017c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00184
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00188
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00194
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00011111,                                                  // 9.7958E-41                                  /// 0019c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0xffc00001,                                                  // -signaling NaN                              /// 001b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00200
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00204
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00208
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0020c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00210
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00214
			 0x0c400000,                                                  // Leading 1s:                                 /// 00218
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x3f028f5c,                                                  // 0.51                                        /// 00220
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00228
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0022c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00230
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00234
			 0x0c400000,                                                  // Leading 1s:                                 /// 00238
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00240
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00244
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0024c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00254
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0025c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00260
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00264
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0xffc00001,                                                  // -signaling NaN                              /// 0026c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00278
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00284
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0028c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00290
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0c780000,                                                  // Leading 1s:                                 /// 00298
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0029c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x55555555,                                                  // 4 random values                             /// 002a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002b0
			 0xffc00001,                                                  // -signaling NaN                              /// 002b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00300
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00304
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0030c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00310
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00318
			 0x0e000007,                                                  // Trailing 1s:                                /// 0031c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00320
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00340
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00344
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00348
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0034c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00350
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00354
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00358
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00364
			 0xff800000,                                                  // -inf                                        /// 00368
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0036c
			 0x00000000,                                                  // zero                                        /// 00370
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00374
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00378
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0037c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00380
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00384
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00388
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0038c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00390
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x33333333,                                                  // 4 random values                             /// 00398
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0039c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x00000000,                                                  // zero                                        /// 003c0
			 0x7fc00001,                                                  // signaling NaN                               /// 003c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e8
			 0x55555555,                                                  // 4 random values                             /// 003ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x7fc00001,                                                  // signaling NaN                               /// 003f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00400
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0040c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00410
			 0x80000000,                                                  // -zero                                       /// 00414
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00418
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0041c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00424
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00428
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00438
			 0xcb000000,                                                  // -8388608.0                                  /// 0043c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00440
			 0x7f800000,                                                  // inf                                         /// 00444
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00448
			 0x7fc00001,                                                  // signaling NaN                               /// 0044c
			 0x4b000000,                                                  // 8388608.0                                   /// 00450
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00454
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0045c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00460
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00470
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00474
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00478
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0047c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00480
			 0x7fc00001,                                                  // signaling NaN                               /// 00484
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00488
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0048c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00490
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00498
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0e000003,                                                  // Trailing 1s:                                /// 004a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004d4
			 0xbf028f5c,                                                  // -0.51                                       /// 004d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00504
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0050c
			 0x7f800000,                                                  // inf                                         /// 00510
			 0x0e000001,                                                  // Trailing 1s:                                /// 00514
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00518
			 0x00011111,                                                  // 9.7958E-41                                  /// 0051c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00524
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00528
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00530
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0053c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00544
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00548
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0054c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0055c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00570
			 0xffc00001,                                                  // -signaling NaN                              /// 00574
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00578
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0057c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00580
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00588
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00594
			 0x0e000003,                                                  // Trailing 1s:                                /// 00598
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0059c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005cc
			 0x33333333,                                                  // 4 random values                             /// 005d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00600
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00604
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00608
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0060c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00610
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00614
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00618
			 0x0c600000,                                                  // Leading 1s:                                 /// 0061c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00620
			 0x55555555,                                                  // 4 random values                             /// 00624
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x80011111,                                                  // -9.7958E-41                                 /// 00638
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00644
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00648
			 0x7f800000,                                                  // inf                                         /// 0064c
			 0x55555555,                                                  // 4 random values                             /// 00650
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00658
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0065c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00660
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00664
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0066c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00670
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00674
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0e000003,                                                  // Trailing 1s:                                /// 0067c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00680
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00684
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00688
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00690
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00694
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0069c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006bc
			 0xff800000,                                                  // -inf                                        /// 006c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 006f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00700
			 0x3f028f5c,                                                  // 0.51                                        /// 00704
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00710
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00714
			 0xff800000,                                                  // -inf                                        /// 00718
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0071c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00720
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00728
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0072c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00730
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00734
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00738
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0073c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00740
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00744
			 0x7fc00001,                                                  // signaling NaN                               /// 00748
			 0x0c600000,                                                  // Leading 1s:                                 /// 0074c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0075c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0xcb000000,                                                  // -8388608.0                                  /// 00764
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00768
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0076c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00774
			 0x80000000,                                                  // -zero                                       /// 00778
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00784
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0078c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00790
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007e4
			 0x55555555,                                                  // 4 random values                             /// 007e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 007fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00800
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00808
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0080c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00810
			 0xcb000000,                                                  // -8388608.0                                  /// 00814
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0081c
			 0x55555555,                                                  // 4 random values                             /// 00820
			 0x55555555,                                                  // 4 random values                             /// 00824
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00828
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0082c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00838
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0xcb000000,                                                  // -8388608.0                                  /// 00840
			 0x00011111,                                                  // 9.7958E-41                                  /// 00844
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00848
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0084c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00850
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00854
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00858
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0085c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00860
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x33333333,                                                  // 4 random values                             /// 0086c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00870
			 0x00000000,                                                  // zero                                        /// 00874
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00878
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0087c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00880
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00884
			 0x80011111,                                                  // -9.7958E-41                                 /// 00888
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00894
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0xffc00001,                                                  // -signaling NaN                              /// 0089c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x00000000,                                                  // zero                                        /// 008a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0xbf028f5c,                                                  // -0.51                                       /// 008dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008e0
			 0x7fc00001,                                                  // signaling NaN                               /// 008e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00908
			 0x55555555,                                                  // 4 random values                             /// 0090c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00914
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00918
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0091c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00920
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00928
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0092c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00934
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0093c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00948
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0094c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00954
			 0x0c780000,                                                  // Leading 1s:                                 /// 00958
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00960
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00964
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x0e000007,                                                  // Trailing 1s:                                /// 0096c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00970
			 0x7fc00001,                                                  // signaling NaN                               /// 00974
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00980
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00988
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0098c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00990
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00998
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0099c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 009b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 009f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a40
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a5c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a90
			 0x55555555,                                                  // 4 random values                             /// 00a94
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ab0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ab8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00abc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ac4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ac8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00acc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ad0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ad4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00af8
			 0x7fc00001,                                                  // signaling NaN                               /// 00afc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b10
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b18
			 0xffc00001,                                                  // -signaling NaN                              /// 00b1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b20
			 0x00000000,                                                  // zero                                        /// 00b24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b28
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b48
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b84
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b98
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ba0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bb0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00000000,                                                  // zero                                        /// 00bc8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bcc
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bd4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bd8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00be0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00be4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00be8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bf4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c40
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x3f028f5c,                                                  // 0.51                                        /// 00c4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c50
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0xffc00001,                                                  // -signaling NaN                              /// 00c9c
			 0x7f800000,                                                  // inf                                         /// 00ca0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ca4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ca8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x00000000,                                                  // zero                                        /// 00cb4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x7f800000,                                                  // inf                                         /// 00cc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ccc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cd4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cdc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ce4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cf0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cf8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cfc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d2c
			 0x55555555,                                                  // 4 random values                             /// 00d30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d44
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d48
			 0x80000000,                                                  // -zero                                       /// 00d4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d60
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d6c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d7c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d98
			 0x80000000,                                                  // -zero                                       /// 00d9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00da4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00da8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0xcb000000,                                                  // -8388608.0                                  /// 00db0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00db4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dbc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dcc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dd0
			 0x3f028f5c,                                                  // 0.51                                        /// 00dd4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ddc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00de0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00de4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dec
			 0x0c400000,                                                  // Leading 1s:                                 /// 00df0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00df8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dfc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e14
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e34
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e48
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e74
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e78
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ea0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ea8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ebc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ec4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ecc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ed4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00edc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ee4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ee8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00eec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ef8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f08
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f64
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f68
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fa8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fbc
			 0xcb000000,                                                  // -8388608.0                                  /// 00fc0
			 0x3f028f5c,                                                  // 0.51                                        /// 00fc4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fd4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fd8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fdc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fe0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fe8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ff0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x80080000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
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
			 0x00000600,
			 0x00000194,
			 0x00000130,
			 0x00000764,
			 0x0000066c,
			 0x00000294,
			 0x00000570,
			 0x00000738,

			 /// vpu register f2
			 0x41b80000,
			 0x41a00000,
			 0x41f00000,
			 0x41d00000,
			 0x41500000,
			 0x41980000,
			 0x40c00000,
			 0x41800000,

			 /// vpu register f3
			 0x40400000,
			 0x40000000,
			 0x41f80000,
			 0x41e00000,
			 0x3f800000,
			 0x40e00000,
			 0x41d00000,
			 0x3f800000,

			 /// vpu register f4
			 0x41e80000,
			 0x41500000,
			 0x42000000,
			 0x41100000,
			 0x41e00000,
			 0x41100000,
			 0x41300000,
			 0x41a00000,

			 /// vpu register f5
			 0x41a80000,
			 0x3f800000,
			 0x41500000,
			 0x41500000,
			 0x41500000,
			 0x41800000,
			 0x41a00000,
			 0x41700000,

			 /// vpu register f6
			 0x42000000,
			 0x41000000,
			 0x41980000,
			 0x41d00000,
			 0x41f80000,
			 0x41b00000,
			 0x41b80000,
			 0x41a00000,

			 /// vpu register f7
			 0x41300000,
			 0x41e00000,
			 0x41700000,
			 0x41f00000,
			 0x41900000,
			 0x40000000,
			 0x40400000,
			 0x41b80000,

			 /// vpu register f8
			 0x40800000,
			 0x41880000,
			 0x41400000,
			 0x41900000,
			 0x41c00000,
			 0x41c00000,
			 0x40400000,
			 0x40800000,

			 /// vpu register f9
			 0x41b80000,
			 0x41b80000,
			 0x41600000,
			 0x41e80000,
			 0x41880000,
			 0x41600000,
			 0x41500000,
			 0x41800000,

			 /// vpu register f10
			 0x41000000,
			 0x41b00000,
			 0x40a00000,
			 0x41c80000,
			 0x3f800000,
			 0x41800000,
			 0x41100000,
			 0x41000000,

			 /// vpu register f11
			 0x41f80000,
			 0x41000000,
			 0x41000000,
			 0x41200000,
			 0x41900000,
			 0x41700000,
			 0x41500000,
			 0x40000000,

			 /// vpu register f12
			 0x40a00000,
			 0x40c00000,
			 0x41b00000,
			 0x41d00000,
			 0x41600000,
			 0x41c00000,
			 0x41c80000,
			 0x41100000,

			 /// vpu register f13
			 0x41980000,
			 0x40e00000,
			 0x41b00000,
			 0x41800000,
			 0x40000000,
			 0x41f00000,
			 0x41600000,
			 0x41980000,

			 /// vpu register f14
			 0x41300000,
			 0x41600000,
			 0x40e00000,
			 0x41100000,
			 0x41a00000,
			 0x41600000,
			 0x41400000,
			 0x41c80000,

			 /// vpu register f15
			 0x41c80000,
			 0x41600000,
			 0x41980000,
			 0x41d00000,
			 0x40400000,
			 0x41900000,
			 0x41f00000,
			 0x40800000,

			 /// vpu register f16
			 0x40400000,
			 0x41f80000,
			 0x41880000,
			 0x41c00000,
			 0x41a80000,
			 0x41e00000,
			 0x41a80000,
			 0x41a00000,

			 /// vpu register f17
			 0x41200000,
			 0x40a00000,
			 0x40e00000,
			 0x41500000,
			 0x40c00000,
			 0x41d00000,
			 0x41500000,
			 0x41800000,

			 /// vpu register f18
			 0x3f800000,
			 0x41200000,
			 0x41880000,
			 0x41a00000,
			 0x40c00000,
			 0x3f800000,
			 0x41400000,
			 0x41b00000,

			 /// vpu register f19
			 0x41f00000,
			 0x41f80000,
			 0x40000000,
			 0x41700000,
			 0x41700000,
			 0x41c80000,
			 0x41f00000,
			 0x40800000,

			 /// vpu register f20
			 0x41c80000,
			 0x42000000,
			 0x41800000,
			 0x3f800000,
			 0x41880000,
			 0x41c00000,
			 0x41900000,
			 0x41b00000,

			 /// vpu register f21
			 0x41000000,
			 0x40400000,
			 0x41d00000,
			 0x41980000,
			 0x40e00000,
			 0x3f800000,
			 0x41f00000,
			 0x41d80000,

			 /// vpu register f22
			 0x41500000,
			 0x41980000,
			 0x41d80000,
			 0x41d00000,
			 0x41300000,
			 0x41600000,
			 0x41d80000,
			 0x40c00000,

			 /// vpu register f23
			 0x41f00000,
			 0x41900000,
			 0x40a00000,
			 0x41b00000,
			 0x41300000,
			 0x41e00000,
			 0x41000000,
			 0x40c00000,

			 /// vpu register f24
			 0x41d00000,
			 0x41880000,
			 0x42000000,
			 0x41900000,
			 0x40400000,
			 0x41600000,
			 0x3f800000,
			 0x41980000,

			 /// vpu register f25
			 0x41880000,
			 0x41900000,
			 0x41200000,
			 0x41e00000,
			 0x3f800000,
			 0x41a80000,
			 0x41400000,
			 0x41c00000,

			 /// vpu register f26
			 0x41a80000,
			 0x40400000,
			 0x41100000,
			 0x41e80000,
			 0x41e00000,
			 0x40a00000,
			 0x40a00000,
			 0x41300000,

			 /// vpu register f27
			 0x40c00000,
			 0x41f00000,
			 0x41400000,
			 0x41800000,
			 0x41400000,
			 0x41900000,
			 0x40c00000,
			 0x41f80000,

			 /// vpu register f28
			 0x41f00000,
			 0x41f00000,
			 0x41100000,
			 0x40a00000,
			 0x42000000,
			 0x40c00000,
			 0x41980000,
			 0x41f80000,

			 /// vpu register f29
			 0x41000000,
			 0x40c00000,
			 0x41900000,
			 0x40000000,
			 0x41980000,
			 0x41100000,
			 0x41d80000,
			 0x40a00000,

			 /// vpu register f30
			 0x41c00000,
			 0x41d00000,
			 0x41980000,
			 0x40000000,
			 0x40a00000,
			 0x41000000,
			 0x41e00000,
			 0x41d80000,

			 /// vpu register f31
			 0x41a80000,
			 0x40a00000,
			 0x41b80000,
			 0x41c80000,
			 0x41c80000,
			 0x3f800000,
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
		"fcvt.ps.f16 f10, f25\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f27, f17\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f3, f7\n"                                ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f21, f5\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f6, f1\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f4, f20\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f5, f23\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f11, f5\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f9, f29\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f12, f6\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f18, f25\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f9, f6\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f5, f7\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f5, f27\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f29, f20\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f28, f17\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f24, f13\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f7, f26\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f27, f2\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f2, f22\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f18, f24\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f12, f14\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f7, f5\n"                                ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f25, f16\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f8, f9\n"                                ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f22, f27\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f9, f2\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f10, f2\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f27, f25\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f29, f5\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f3, f1\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f6, f9\n"                                ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f21, f8\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f18, f8\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f30, f25\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f3, f6\n"                                ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f24, f30\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f13, f7\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f24, f16\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f24, f9\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f10, f18\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f7, f2\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f30, f30\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f18, f1\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f28, f25\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f13, f3\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f8, f23\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f3, f12\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f15, f16\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f21, f1\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f23, f2\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f15, f4\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f15, f12\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f28, f26\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f29, f24\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f28, f23\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f4, f29\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f8, f13\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f22, f20\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f24, f3\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f12, f8\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f24, f14\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f2, f19\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f5, f20\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f29, f29\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f9, f22\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f2, f23\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f17, f7\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f29, f5\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f19, f11\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f23, f15\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f21, f13\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f30, f10\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f4, f20\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f23, f11\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f24, f7\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f18, f6\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f19, f13\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f6, f14\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f4, f23\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f25, f27\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f21, f27\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f14, f22\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f5, f27\n"                               ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f26, f23\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f4, f13\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f9, f8\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f14, f16\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f11, f26\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f15, f19\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f16, f9\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f4, f9\n"                                ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f17, f14\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f6, f5\n"                                ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f7, f24\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f27, f6\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f26, f13\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f21, f18\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f16 f4, f26\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
