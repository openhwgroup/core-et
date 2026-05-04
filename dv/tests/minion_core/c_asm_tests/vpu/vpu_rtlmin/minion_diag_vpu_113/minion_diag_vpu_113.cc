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
			 0x449e3d13, /// 0x0
			 0xd09fa770, /// 0x4
			 0x028bc27e, /// 0x8
			 0x44cf5db5, /// 0xc
			 0x5d8dd648, /// 0x10
			 0x35a0b0fc, /// 0x14
			 0xb3e189d8, /// 0x18
			 0x9cf2ff5d, /// 0x1c
			 0xaa47c831, /// 0x20
			 0x5496c4de, /// 0x24
			 0xfcac7c99, /// 0x28
			 0xc12df3d4, /// 0x2c
			 0x70604a88, /// 0x30
			 0x67737d68, /// 0x34
			 0x1a628cfb, /// 0x38
			 0xce3db55d, /// 0x3c
			 0xea5683dd, /// 0x40
			 0x94c3add5, /// 0x44
			 0x2293185d, /// 0x48
			 0xa33118e4, /// 0x4c
			 0xb114c840, /// 0x50
			 0x9d6e319f, /// 0x54
			 0x500a190b, /// 0x58
			 0x4910a272, /// 0x5c
			 0xdf6ab0d6, /// 0x60
			 0xce3f098c, /// 0x64
			 0x51fa7933, /// 0x68
			 0x5930a848, /// 0x6c
			 0x0fc026df, /// 0x70
			 0xd4274d05, /// 0x74
			 0x65c0c931, /// 0x78
			 0xb2dc7e7a, /// 0x7c
			 0x14cbb732, /// 0x80
			 0x70e159f8, /// 0x84
			 0x65cbf9cf, /// 0x88
			 0x4ef0a045, /// 0x8c
			 0x7f763f26, /// 0x90
			 0x7b4a1754, /// 0x94
			 0x71dd66a9, /// 0x98
			 0x3727e475, /// 0x9c
			 0x383580c9, /// 0xa0
			 0x6892cc3d, /// 0xa4
			 0xd432465c, /// 0xa8
			 0x7832bc49, /// 0xac
			 0xca7a2f13, /// 0xb0
			 0x1733601f, /// 0xb4
			 0xfcb7039e, /// 0xb8
			 0x92524454, /// 0xbc
			 0xd359d29e, /// 0xc0
			 0xeb77c56d, /// 0xc4
			 0xc0242f42, /// 0xc8
			 0x531aa1de, /// 0xcc
			 0xefcf5ba8, /// 0xd0
			 0x6b53ec85, /// 0xd4
			 0xc5e6bfbd, /// 0xd8
			 0x700ba62b, /// 0xdc
			 0x9e4408c5, /// 0xe0
			 0xd9cb7d58, /// 0xe4
			 0x910456b1, /// 0xe8
			 0x86b0aa40, /// 0xec
			 0xc97d528d, /// 0xf0
			 0xe927fc91, /// 0xf4
			 0x9dc9bd70, /// 0xf8
			 0xc3c15e69, /// 0xfc
			 0xe45aae25, /// 0x100
			 0x8622c458, /// 0x104
			 0xaf059241, /// 0x108
			 0x60075916, /// 0x10c
			 0x47e94a18, /// 0x110
			 0x6f493d1f, /// 0x114
			 0x2b2b77c0, /// 0x118
			 0x5968e2ed, /// 0x11c
			 0xb447feff, /// 0x120
			 0xc8d10e27, /// 0x124
			 0x5fa9ffd1, /// 0x128
			 0xcb4c9288, /// 0x12c
			 0xc72f7a3c, /// 0x130
			 0xec627602, /// 0x134
			 0xe87ce27c, /// 0x138
			 0x0765ab44, /// 0x13c
			 0x48e4ebd6, /// 0x140
			 0x7ef7f038, /// 0x144
			 0x0608c03b, /// 0x148
			 0x5de58412, /// 0x14c
			 0x2ba880de, /// 0x150
			 0x717a5832, /// 0x154
			 0x5610d38d, /// 0x158
			 0xde44766a, /// 0x15c
			 0xdcfcf1a5, /// 0x160
			 0x0207a82b, /// 0x164
			 0x1abc11da, /// 0x168
			 0xc7489ca1, /// 0x16c
			 0xb00824a3, /// 0x170
			 0x7b0295cb, /// 0x174
			 0x7d07a4fe, /// 0x178
			 0x1ede9004, /// 0x17c
			 0xf1942a50, /// 0x180
			 0x43ac0cbb, /// 0x184
			 0xfe8e9120, /// 0x188
			 0x56ab5133, /// 0x18c
			 0x71437825, /// 0x190
			 0x462a2a09, /// 0x194
			 0x3fcafdb0, /// 0x198
			 0xfd53bfce, /// 0x19c
			 0xd9aff6a2, /// 0x1a0
			 0xca722e1b, /// 0x1a4
			 0x7b319732, /// 0x1a8
			 0xab74c8ce, /// 0x1ac
			 0x6fafc9e5, /// 0x1b0
			 0x2d7774d4, /// 0x1b4
			 0xedf4c9cc, /// 0x1b8
			 0xb71722f8, /// 0x1bc
			 0xf90402c1, /// 0x1c0
			 0x61d30d21, /// 0x1c4
			 0x6a39d05f, /// 0x1c8
			 0x507a9f7f, /// 0x1cc
			 0x53ceb169, /// 0x1d0
			 0x6905fa9c, /// 0x1d4
			 0x87844512, /// 0x1d8
			 0x7a84f934, /// 0x1dc
			 0x9c4b5f83, /// 0x1e0
			 0x46f1ff2b, /// 0x1e4
			 0xe04f26df, /// 0x1e8
			 0x101df49e, /// 0x1ec
			 0xc0e11616, /// 0x1f0
			 0x29173f12, /// 0x1f4
			 0xb5e83d1f, /// 0x1f8
			 0x1d02982b, /// 0x1fc
			 0x601037e0, /// 0x200
			 0x08930a4a, /// 0x204
			 0x385f30cd, /// 0x208
			 0x8804d431, /// 0x20c
			 0xca6d0f5d, /// 0x210
			 0x089c52ad, /// 0x214
			 0x2df98f91, /// 0x218
			 0xb37a0081, /// 0x21c
			 0x136d0338, /// 0x220
			 0x5be9f226, /// 0x224
			 0xb505d255, /// 0x228
			 0xe1a1359c, /// 0x22c
			 0x313577e3, /// 0x230
			 0x2663d4b3, /// 0x234
			 0x4c609ee2, /// 0x238
			 0x6f056444, /// 0x23c
			 0x2cbf98e3, /// 0x240
			 0x221c4830, /// 0x244
			 0x0da93c1d, /// 0x248
			 0xd0d51467, /// 0x24c
			 0xe737ac94, /// 0x250
			 0x1fddd060, /// 0x254
			 0x7c8c56bb, /// 0x258
			 0xb361a2c5, /// 0x25c
			 0x0d9f8c3c, /// 0x260
			 0xf5b2b505, /// 0x264
			 0x37cf10ce, /// 0x268
			 0xc33e58ed, /// 0x26c
			 0x28f7af59, /// 0x270
			 0x6063deeb, /// 0x274
			 0xb2877932, /// 0x278
			 0x03b8ca27, /// 0x27c
			 0xde6e1037, /// 0x280
			 0x1547915a, /// 0x284
			 0x0a1c48e0, /// 0x288
			 0x9bca51ca, /// 0x28c
			 0xcd3090b2, /// 0x290
			 0xd5bd84d6, /// 0x294
			 0xc0598ce4, /// 0x298
			 0x56c6dceb, /// 0x29c
			 0xd7f344d9, /// 0x2a0
			 0x40ae6aed, /// 0x2a4
			 0x322dfebd, /// 0x2a8
			 0xaa3ff786, /// 0x2ac
			 0x3874b1a7, /// 0x2b0
			 0x6c256b35, /// 0x2b4
			 0xe99bc720, /// 0x2b8
			 0xb9a1f544, /// 0x2bc
			 0x48ea5149, /// 0x2c0
			 0xb04b6c50, /// 0x2c4
			 0xdd49c722, /// 0x2c8
			 0x6297d7e3, /// 0x2cc
			 0x02d011e8, /// 0x2d0
			 0x6471d177, /// 0x2d4
			 0x4bcbe743, /// 0x2d8
			 0x49c528de, /// 0x2dc
			 0x889edbd1, /// 0x2e0
			 0xbdc37c88, /// 0x2e4
			 0x6cb05172, /// 0x2e8
			 0xd00eba0d, /// 0x2ec
			 0x64d97d7c, /// 0x2f0
			 0xe0d01c15, /// 0x2f4
			 0xd6450020, /// 0x2f8
			 0x9687ca50, /// 0x2fc
			 0x8faa50e8, /// 0x300
			 0xc0c60f00, /// 0x304
			 0xf94357db, /// 0x308
			 0x146a1560, /// 0x30c
			 0xef90716a, /// 0x310
			 0xdf25025c, /// 0x314
			 0x373b2d03, /// 0x318
			 0xd7bf9879, /// 0x31c
			 0xf3d1a8f5, /// 0x320
			 0xd6757da1, /// 0x324
			 0xd157aca1, /// 0x328
			 0xa5dd9ee5, /// 0x32c
			 0x7ecbf3ee, /// 0x330
			 0xbc90d6ed, /// 0x334
			 0x34566537, /// 0x338
			 0xc52f5a03, /// 0x33c
			 0x86618dc1, /// 0x340
			 0x6ef69d96, /// 0x344
			 0xee00d103, /// 0x348
			 0x5b2e5fce, /// 0x34c
			 0xc9ce8f93, /// 0x350
			 0xa7ee2d58, /// 0x354
			 0x5eada170, /// 0x358
			 0x73f7d9df, /// 0x35c
			 0x88cb0a04, /// 0x360
			 0x5f273ca0, /// 0x364
			 0x0fc87b4e, /// 0x368
			 0x10a1970f, /// 0x36c
			 0xc3dfe73d, /// 0x370
			 0x759e376e, /// 0x374
			 0xc1c4a4f9, /// 0x378
			 0xbf5ab34b, /// 0x37c
			 0x62291d4f, /// 0x380
			 0x6227c4e3, /// 0x384
			 0x8f2aee68, /// 0x388
			 0xdabe87ff, /// 0x38c
			 0xc22710b1, /// 0x390
			 0x9b74bda8, /// 0x394
			 0x7f1b7ed2, /// 0x398
			 0x2e322c2e, /// 0x39c
			 0x29ac71fd, /// 0x3a0
			 0xbd0a0d9a, /// 0x3a4
			 0x5e1e9c06, /// 0x3a8
			 0xdf6ba75b, /// 0x3ac
			 0xa2614df9, /// 0x3b0
			 0x7b063ebf, /// 0x3b4
			 0x1bc51657, /// 0x3b8
			 0xd9caaef4, /// 0x3bc
			 0xabea7ac1, /// 0x3c0
			 0xdf0345e5, /// 0x3c4
			 0xd960a39c, /// 0x3c8
			 0x4d59da46, /// 0x3cc
			 0xe7e93ffc, /// 0x3d0
			 0x178bc495, /// 0x3d4
			 0x68cacd58, /// 0x3d8
			 0xc5143793, /// 0x3dc
			 0xb38707c4, /// 0x3e0
			 0x1ff5dee9, /// 0x3e4
			 0x8acf034d, /// 0x3e8
			 0xda8e5be5, /// 0x3ec
			 0x19f2944f, /// 0x3f0
			 0x3ba206bd, /// 0x3f4
			 0x7ed20e52, /// 0x3f8
			 0xa4712da6, /// 0x3fc
			 0x413e8389, /// 0x400
			 0x847d85d4, /// 0x404
			 0x01666e3b, /// 0x408
			 0x20b806a9, /// 0x40c
			 0x52b14402, /// 0x410
			 0x16d66dee, /// 0x414
			 0x48e1aa9d, /// 0x418
			 0x5b2b5eed, /// 0x41c
			 0x34191ebd, /// 0x420
			 0x3564ea9b, /// 0x424
			 0xd6d1c22e, /// 0x428
			 0xde653fe6, /// 0x42c
			 0x55814d67, /// 0x430
			 0x04c37032, /// 0x434
			 0x58eb2b87, /// 0x438
			 0x7ddcedc5, /// 0x43c
			 0x7fbc2442, /// 0x440
			 0x6d60a010, /// 0x444
			 0x15fb2e79, /// 0x448
			 0xaa03cf4e, /// 0x44c
			 0x4d3312a2, /// 0x450
			 0x6e44beaa, /// 0x454
			 0xdd718507, /// 0x458
			 0xc8c97635, /// 0x45c
			 0x148cff7d, /// 0x460
			 0x2a1f180c, /// 0x464
			 0x43d886c9, /// 0x468
			 0x6ad99c6d, /// 0x46c
			 0x8aea4a12, /// 0x470
			 0xcf381266, /// 0x474
			 0x99e46dc4, /// 0x478
			 0x0e0b858c, /// 0x47c
			 0x00d0240a, /// 0x480
			 0x41668a71, /// 0x484
			 0x8159af31, /// 0x488
			 0x2e9a2c41, /// 0x48c
			 0xa4754326, /// 0x490
			 0x9b11795c, /// 0x494
			 0xe3edf2b2, /// 0x498
			 0xbc765984, /// 0x49c
			 0xeaccf5ed, /// 0x4a0
			 0xdf8a11e2, /// 0x4a4
			 0x39490b5b, /// 0x4a8
			 0x331653b1, /// 0x4ac
			 0x995cbffe, /// 0x4b0
			 0x23b01ad8, /// 0x4b4
			 0x363c5444, /// 0x4b8
			 0x95a2842d, /// 0x4bc
			 0x253a6398, /// 0x4c0
			 0x0f1efc7b, /// 0x4c4
			 0xa96ea138, /// 0x4c8
			 0x3e8413d0, /// 0x4cc
			 0x5fb0b4dd, /// 0x4d0
			 0x7604c4b2, /// 0x4d4
			 0x390298d8, /// 0x4d8
			 0x6946bd78, /// 0x4dc
			 0x683c27c0, /// 0x4e0
			 0xdaa59cca, /// 0x4e4
			 0x540d8cb8, /// 0x4e8
			 0xfb6b83b1, /// 0x4ec
			 0xbcb0410f, /// 0x4f0
			 0xa6cd5238, /// 0x4f4
			 0xfc09a6c8, /// 0x4f8
			 0xbc5128f2, /// 0x4fc
			 0x3cdfa598, /// 0x500
			 0x4fb910a2, /// 0x504
			 0x34ac4724, /// 0x508
			 0x07bb9bc9, /// 0x50c
			 0x3b31a213, /// 0x510
			 0xb85518d6, /// 0x514
			 0xfba4818c, /// 0x518
			 0x1afbff6e, /// 0x51c
			 0xc2b0ca52, /// 0x520
			 0x36b9821c, /// 0x524
			 0x868ca614, /// 0x528
			 0x617777b2, /// 0x52c
			 0xcc232a69, /// 0x530
			 0x91b08003, /// 0x534
			 0x79ef6db3, /// 0x538
			 0x1580db31, /// 0x53c
			 0xfa5e0b3a, /// 0x540
			 0xe5b833d5, /// 0x544
			 0xf8390127, /// 0x548
			 0x69785942, /// 0x54c
			 0xe683836f, /// 0x550
			 0x4e958900, /// 0x554
			 0x7cdccd21, /// 0x558
			 0x5a785675, /// 0x55c
			 0x426eec89, /// 0x560
			 0x7677d5be, /// 0x564
			 0xd2e3d05f, /// 0x568
			 0x2ff06a54, /// 0x56c
			 0xe71eb599, /// 0x570
			 0xce4c7821, /// 0x574
			 0x9a7e4f80, /// 0x578
			 0xfa0b4641, /// 0x57c
			 0x06b8f26f, /// 0x580
			 0x4a9e5306, /// 0x584
			 0x7606fc2d, /// 0x588
			 0x72cbf591, /// 0x58c
			 0xb2f37cb0, /// 0x590
			 0x2a20f83b, /// 0x594
			 0x40d7e748, /// 0x598
			 0x8a3abf81, /// 0x59c
			 0xb4271346, /// 0x5a0
			 0xfdfc47eb, /// 0x5a4
			 0xe47a4d12, /// 0x5a8
			 0x85c2a559, /// 0x5ac
			 0x32c999ba, /// 0x5b0
			 0x047d0861, /// 0x5b4
			 0xfd95a711, /// 0x5b8
			 0x75b88d81, /// 0x5bc
			 0xa0895653, /// 0x5c0
			 0x132797cc, /// 0x5c4
			 0xa3595187, /// 0x5c8
			 0x6cc0e0f1, /// 0x5cc
			 0x3c91768d, /// 0x5d0
			 0x26b34bca, /// 0x5d4
			 0x75492bf9, /// 0x5d8
			 0x5b85831d, /// 0x5dc
			 0x6fa580f4, /// 0x5e0
			 0x0b7ad706, /// 0x5e4
			 0x044c7589, /// 0x5e8
			 0x3d9f5913, /// 0x5ec
			 0x0be467ea, /// 0x5f0
			 0x882e5258, /// 0x5f4
			 0xa3884ac8, /// 0x5f8
			 0xb917f5a3, /// 0x5fc
			 0xe293f606, /// 0x600
			 0x2bdaec5b, /// 0x604
			 0x8cd0077d, /// 0x608
			 0xb8baf749, /// 0x60c
			 0x753a7252, /// 0x610
			 0x83782c5e, /// 0x614
			 0x074ea99b, /// 0x618
			 0xb73e57c5, /// 0x61c
			 0x19d807e4, /// 0x620
			 0x08dc13c8, /// 0x624
			 0x6acac010, /// 0x628
			 0x3f0d923d, /// 0x62c
			 0xd6f6d33b, /// 0x630
			 0xf14d0800, /// 0x634
			 0x5880bef6, /// 0x638
			 0x3bf4e9f7, /// 0x63c
			 0x6427b92d, /// 0x640
			 0x0fe68ece, /// 0x644
			 0x73fe08da, /// 0x648
			 0xdb009d8d, /// 0x64c
			 0x3a7656bb, /// 0x650
			 0x61c10f30, /// 0x654
			 0xb67ab394, /// 0x658
			 0x15d34d4c, /// 0x65c
			 0x8c45220e, /// 0x660
			 0x98a80f03, /// 0x664
			 0x9dbbc138, /// 0x668
			 0xe7341f04, /// 0x66c
			 0xf14be605, /// 0x670
			 0x1ec43401, /// 0x674
			 0x47fcb226, /// 0x678
			 0x939faf3b, /// 0x67c
			 0x3c08c3b3, /// 0x680
			 0x3c933ab7, /// 0x684
			 0x883ad57c, /// 0x688
			 0x3cf3594a, /// 0x68c
			 0x7c0715b0, /// 0x690
			 0x4bedf995, /// 0x694
			 0x8e4d044e, /// 0x698
			 0xe5abec56, /// 0x69c
			 0x74c217e9, /// 0x6a0
			 0xf6d1cebb, /// 0x6a4
			 0x9ae2949a, /// 0x6a8
			 0x72916f49, /// 0x6ac
			 0xa0be5841, /// 0x6b0
			 0x3ec121c4, /// 0x6b4
			 0x55d8115e, /// 0x6b8
			 0xbe385961, /// 0x6bc
			 0x4a79dd28, /// 0x6c0
			 0x035a5570, /// 0x6c4
			 0x32c71196, /// 0x6c8
			 0x38fb88a3, /// 0x6cc
			 0x0f4b8616, /// 0x6d0
			 0x1afd48ff, /// 0x6d4
			 0x6b897d3e, /// 0x6d8
			 0x2e4b6f5d, /// 0x6dc
			 0xde00710f, /// 0x6e0
			 0x1331a25c, /// 0x6e4
			 0xed0613e3, /// 0x6e8
			 0x0d507cea, /// 0x6ec
			 0xdf486b5b, /// 0x6f0
			 0x1d6f85d1, /// 0x6f4
			 0x8ecfea56, /// 0x6f8
			 0xa67e5805, /// 0x6fc
			 0x18bbdbdc, /// 0x700
			 0x8cc6c19b, /// 0x704
			 0x4908e409, /// 0x708
			 0xcd5af3f1, /// 0x70c
			 0x69b64d81, /// 0x710
			 0xdecb435f, /// 0x714
			 0xf1b68633, /// 0x718
			 0x7f4ea099, /// 0x71c
			 0xd32148ab, /// 0x720
			 0x3bf3308e, /// 0x724
			 0x1f94b85a, /// 0x728
			 0x2a90ca66, /// 0x72c
			 0x00821ea5, /// 0x730
			 0xb8b884d9, /// 0x734
			 0x3a6659e1, /// 0x738
			 0x9e6b085a, /// 0x73c
			 0xad59691f, /// 0x740
			 0x11427e3b, /// 0x744
			 0xf32161e0, /// 0x748
			 0x4793b5e4, /// 0x74c
			 0x4bd0221c, /// 0x750
			 0xb73aa74d, /// 0x754
			 0x8baab735, /// 0x758
			 0x5519f300, /// 0x75c
			 0x30c1aaf9, /// 0x760
			 0xf37dc56e, /// 0x764
			 0xa1aa917f, /// 0x768
			 0x3d53a0ac, /// 0x76c
			 0x02fa061f, /// 0x770
			 0xb9755e15, /// 0x774
			 0xf6ec0aa8, /// 0x778
			 0x832bd5d4, /// 0x77c
			 0x5925435b, /// 0x780
			 0x15c115af, /// 0x784
			 0xe5a5f1d9, /// 0x788
			 0xadf003e4, /// 0x78c
			 0x87b95efc, /// 0x790
			 0xb313b979, /// 0x794
			 0x7dc83524, /// 0x798
			 0xca6ecc0f, /// 0x79c
			 0x1b881962, /// 0x7a0
			 0x005309eb, /// 0x7a4
			 0x919cdd52, /// 0x7a8
			 0x30d43946, /// 0x7ac
			 0xfdb844d2, /// 0x7b0
			 0xbc144513, /// 0x7b4
			 0x242c769d, /// 0x7b8
			 0x41cd19d8, /// 0x7bc
			 0x859c7e78, /// 0x7c0
			 0x5292ce9e, /// 0x7c4
			 0xc190e4f5, /// 0x7c8
			 0x1c22ca8e, /// 0x7cc
			 0xe104917c, /// 0x7d0
			 0xb34f3d44, /// 0x7d4
			 0xcd97ba25, /// 0x7d8
			 0x62c48228, /// 0x7dc
			 0x6cfa83c8, /// 0x7e0
			 0x346e6df1, /// 0x7e4
			 0x3c97de7a, /// 0x7e8
			 0x0053e033, /// 0x7ec
			 0x09212426, /// 0x7f0
			 0x41ab6eff, /// 0x7f4
			 0x895d7e6f, /// 0x7f8
			 0xd94a8047, /// 0x7fc
			 0xf5f33fa5, /// 0x800
			 0x38435f6e, /// 0x804
			 0xe3209ab0, /// 0x808
			 0xa7ffe69a, /// 0x80c
			 0xdea106eb, /// 0x810
			 0x42353d5c, /// 0x814
			 0xce6ac30d, /// 0x818
			 0x131336bf, /// 0x81c
			 0x9171408e, /// 0x820
			 0xc354ef54, /// 0x824
			 0x3079d570, /// 0x828
			 0x6a6461a0, /// 0x82c
			 0x26f120bb, /// 0x830
			 0x7f7cadb4, /// 0x834
			 0x7731b827, /// 0x838
			 0x8969f253, /// 0x83c
			 0xbc8cd88c, /// 0x840
			 0x2bac9068, /// 0x844
			 0x209ecddf, /// 0x848
			 0x07730b8c, /// 0x84c
			 0xfb0bd75e, /// 0x850
			 0x58bc36ce, /// 0x854
			 0xddeaadc3, /// 0x858
			 0x05cd6a16, /// 0x85c
			 0x2a6ea194, /// 0x860
			 0x425caabc, /// 0x864
			 0xe8de676f, /// 0x868
			 0xf2343a30, /// 0x86c
			 0xe4823a85, /// 0x870
			 0x64b48c9c, /// 0x874
			 0x2156e24e, /// 0x878
			 0x0573c319, /// 0x87c
			 0x6a53d250, /// 0x880
			 0xf42b6521, /// 0x884
			 0xfe71e176, /// 0x888
			 0xc9fbf579, /// 0x88c
			 0x941cac85, /// 0x890
			 0x9a6d30f4, /// 0x894
			 0x1f4e5a70, /// 0x898
			 0x4320ad4d, /// 0x89c
			 0xabb24123, /// 0x8a0
			 0x764b2699, /// 0x8a4
			 0x70598588, /// 0x8a8
			 0xb7b91c4a, /// 0x8ac
			 0x5cb02c71, /// 0x8b0
			 0xd75b0825, /// 0x8b4
			 0x72b6411b, /// 0x8b8
			 0xd15b927c, /// 0x8bc
			 0x27a25f7a, /// 0x8c0
			 0x32e238bc, /// 0x8c4
			 0xf353649f, /// 0x8c8
			 0xc6ff134e, /// 0x8cc
			 0x2b5eba9b, /// 0x8d0
			 0x8fc4396d, /// 0x8d4
			 0x6330e502, /// 0x8d8
			 0x3032887a, /// 0x8dc
			 0x35f7bd81, /// 0x8e0
			 0x1c31795c, /// 0x8e4
			 0xa8a43249, /// 0x8e8
			 0x9b31c4ac, /// 0x8ec
			 0x1f4f4a51, /// 0x8f0
			 0xdc9458ae, /// 0x8f4
			 0x8fde7de0, /// 0x8f8
			 0xc82f19b8, /// 0x8fc
			 0xba51a8a3, /// 0x900
			 0x5108011a, /// 0x904
			 0x55655463, /// 0x908
			 0x6b124a20, /// 0x90c
			 0x76952bde, /// 0x910
			 0x4f8b37a2, /// 0x914
			 0x363e5dcf, /// 0x918
			 0x40a7bbed, /// 0x91c
			 0x986e6346, /// 0x920
			 0xd22b3663, /// 0x924
			 0x0385c368, /// 0x928
			 0xe95ad123, /// 0x92c
			 0x5074201b, /// 0x930
			 0x3c6aba73, /// 0x934
			 0xcafeda34, /// 0x938
			 0xad8d7f06, /// 0x93c
			 0x20a6a7a4, /// 0x940
			 0x58ca9661, /// 0x944
			 0x2fc20812, /// 0x948
			 0x636e6d1a, /// 0x94c
			 0x1d9fa46f, /// 0x950
			 0x8a7b7e76, /// 0x954
			 0xd9da1916, /// 0x958
			 0x2841de83, /// 0x95c
			 0xd27e89ec, /// 0x960
			 0x9a27449e, /// 0x964
			 0x779710ab, /// 0x968
			 0xd8995578, /// 0x96c
			 0x20457d86, /// 0x970
			 0xb925a194, /// 0x974
			 0xb26a1759, /// 0x978
			 0x323d96ce, /// 0x97c
			 0x8e318eec, /// 0x980
			 0x12b55646, /// 0x984
			 0x8d9fc35f, /// 0x988
			 0xdcbb462d, /// 0x98c
			 0x9c5e60c8, /// 0x990
			 0x64ca0549, /// 0x994
			 0x979adec6, /// 0x998
			 0x4e8b0dc4, /// 0x99c
			 0x2809074e, /// 0x9a0
			 0xd947348a, /// 0x9a4
			 0xf066e238, /// 0x9a8
			 0x438bd718, /// 0x9ac
			 0x3ee89369, /// 0x9b0
			 0xd74e521e, /// 0x9b4
			 0x456dbafe, /// 0x9b8
			 0xac49553d, /// 0x9bc
			 0x4a1c81ce, /// 0x9c0
			 0x799672dc, /// 0x9c4
			 0xa1e3d326, /// 0x9c8
			 0x6be3da53, /// 0x9cc
			 0xa1164098, /// 0x9d0
			 0xf2b7e622, /// 0x9d4
			 0x7d02a9bc, /// 0x9d8
			 0x444bf7f8, /// 0x9dc
			 0xccd0944b, /// 0x9e0
			 0x35455dc6, /// 0x9e4
			 0x5ad73f5f, /// 0x9e8
			 0xd1932585, /// 0x9ec
			 0xdacf7850, /// 0x9f0
			 0x67aed6cf, /// 0x9f4
			 0xd7245eb5, /// 0x9f8
			 0x4202f8af, /// 0x9fc
			 0xd5ed8c0e, /// 0xa00
			 0x64c9ac43, /// 0xa04
			 0xe798e740, /// 0xa08
			 0xabffd409, /// 0xa0c
			 0x629dfd69, /// 0xa10
			 0x8784343a, /// 0xa14
			 0xc2818a2d, /// 0xa18
			 0x92ce5388, /// 0xa1c
			 0x53c79625, /// 0xa20
			 0x78dde043, /// 0xa24
			 0x20690d8f, /// 0xa28
			 0x60eaafbb, /// 0xa2c
			 0x54a2f159, /// 0xa30
			 0x58ea1c9f, /// 0xa34
			 0x5348cd97, /// 0xa38
			 0x7a32b9b0, /// 0xa3c
			 0x0344071c, /// 0xa40
			 0xbdd7e658, /// 0xa44
			 0xa0cb411b, /// 0xa48
			 0x2d5557f0, /// 0xa4c
			 0x66e2051c, /// 0xa50
			 0xd43d260e, /// 0xa54
			 0x42734189, /// 0xa58
			 0x90b2f946, /// 0xa5c
			 0xb16d1fea, /// 0xa60
			 0x96ded002, /// 0xa64
			 0xc77edcc7, /// 0xa68
			 0xacb6e60a, /// 0xa6c
			 0x840c0108, /// 0xa70
			 0xd4be555a, /// 0xa74
			 0x6efa85bb, /// 0xa78
			 0x8245a7ef, /// 0xa7c
			 0x9a09194c, /// 0xa80
			 0x0facc030, /// 0xa84
			 0x3b5c4e29, /// 0xa88
			 0x34cfd4b1, /// 0xa8c
			 0x89dc218d, /// 0xa90
			 0x7027c985, /// 0xa94
			 0xa379716c, /// 0xa98
			 0x2a70d477, /// 0xa9c
			 0x9961945b, /// 0xaa0
			 0xd7567e09, /// 0xaa4
			 0xb15c67f6, /// 0xaa8
			 0xbfab853b, /// 0xaac
			 0x16a62c52, /// 0xab0
			 0xaf14eac4, /// 0xab4
			 0xee85a10b, /// 0xab8
			 0x24d0c8d3, /// 0xabc
			 0x388d01e6, /// 0xac0
			 0x5bf643ea, /// 0xac4
			 0x112f5cdd, /// 0xac8
			 0x50b02f41, /// 0xacc
			 0xe2f41db1, /// 0xad0
			 0xe77ea365, /// 0xad4
			 0x5ae69c60, /// 0xad8
			 0x6768f9ef, /// 0xadc
			 0xb2a4b77b, /// 0xae0
			 0x4f22f9f4, /// 0xae4
			 0xadbaf2bd, /// 0xae8
			 0x91b999ec, /// 0xaec
			 0x42a249a1, /// 0xaf0
			 0x051b7e95, /// 0xaf4
			 0xdc14aacc, /// 0xaf8
			 0x6c9ddd78, /// 0xafc
			 0x30ac49ea, /// 0xb00
			 0xa1a4e166, /// 0xb04
			 0x8697ac9b, /// 0xb08
			 0x19496c6d, /// 0xb0c
			 0xef2c2bbf, /// 0xb10
			 0xdc94b511, /// 0xb14
			 0x23839b02, /// 0xb18
			 0x778b9407, /// 0xb1c
			 0xcf15b3d6, /// 0xb20
			 0xf08c813b, /// 0xb24
			 0x48fe759b, /// 0xb28
			 0x079e76c7, /// 0xb2c
			 0x51e123a0, /// 0xb30
			 0x069bde7d, /// 0xb34
			 0x25fbe610, /// 0xb38
			 0x0dac90f5, /// 0xb3c
			 0x53bd3f8c, /// 0xb40
			 0x62e93f98, /// 0xb44
			 0xae8646e6, /// 0xb48
			 0x771cdba8, /// 0xb4c
			 0xe6ed072b, /// 0xb50
			 0x2284d8dc, /// 0xb54
			 0xb95f7cb8, /// 0xb58
			 0x0fa48f00, /// 0xb5c
			 0x60207e97, /// 0xb60
			 0x2a70c886, /// 0xb64
			 0x9c6ec974, /// 0xb68
			 0xd4c6470a, /// 0xb6c
			 0x76c12fba, /// 0xb70
			 0x7473b9b0, /// 0xb74
			 0x14697b23, /// 0xb78
			 0x6b127ede, /// 0xb7c
			 0x2b2869b8, /// 0xb80
			 0xa113ea68, /// 0xb84
			 0x7b151aca, /// 0xb88
			 0xb20d97ad, /// 0xb8c
			 0x0c01d470, /// 0xb90
			 0x8395d271, /// 0xb94
			 0x652cc07e, /// 0xb98
			 0x8d03b9f9, /// 0xb9c
			 0x3f9828f9, /// 0xba0
			 0xb2ebe785, /// 0xba4
			 0x06646ac0, /// 0xba8
			 0xb5f7d0e2, /// 0xbac
			 0xe37df05a, /// 0xbb0
			 0x38f51053, /// 0xbb4
			 0x9ec958ff, /// 0xbb8
			 0x741d2560, /// 0xbbc
			 0xbc4b1b38, /// 0xbc0
			 0x0327534e, /// 0xbc4
			 0x8f03b94e, /// 0xbc8
			 0xa8f68db3, /// 0xbcc
			 0x37c13a69, /// 0xbd0
			 0x96276551, /// 0xbd4
			 0x6e41c52f, /// 0xbd8
			 0xdfacbbc1, /// 0xbdc
			 0xae357cd0, /// 0xbe0
			 0xbd85ad90, /// 0xbe4
			 0x9620d5c7, /// 0xbe8
			 0x3f2b74b5, /// 0xbec
			 0x513fbcb1, /// 0xbf0
			 0x742fec6a, /// 0xbf4
			 0x41cca80f, /// 0xbf8
			 0x7183b08d, /// 0xbfc
			 0xb19494bf, /// 0xc00
			 0xcf65fc34, /// 0xc04
			 0x435805f5, /// 0xc08
			 0xb692b183, /// 0xc0c
			 0x5c7d6a3b, /// 0xc10
			 0x8004e1d2, /// 0xc14
			 0xb3a8a3b6, /// 0xc18
			 0xe28d3fb3, /// 0xc1c
			 0xdc0fa332, /// 0xc20
			 0x21b181db, /// 0xc24
			 0x4efff425, /// 0xc28
			 0x599701d3, /// 0xc2c
			 0xdb5dc900, /// 0xc30
			 0xf7dadce3, /// 0xc34
			 0x01f8e56a, /// 0xc38
			 0x641e34d9, /// 0xc3c
			 0x8c43dcce, /// 0xc40
			 0xdadc5a8c, /// 0xc44
			 0x3f817f7a, /// 0xc48
			 0x706a8ddb, /// 0xc4c
			 0x6ba923d3, /// 0xc50
			 0x4f7956e6, /// 0xc54
			 0x746523cf, /// 0xc58
			 0xb651f754, /// 0xc5c
			 0x3bd81a9f, /// 0xc60
			 0xb23ce501, /// 0xc64
			 0xa3e41fb3, /// 0xc68
			 0x75592f97, /// 0xc6c
			 0x90c8a5c8, /// 0xc70
			 0xc7b8b561, /// 0xc74
			 0x0ba4c553, /// 0xc78
			 0x5c4aa991, /// 0xc7c
			 0xff393222, /// 0xc80
			 0x5180d956, /// 0xc84
			 0x7e5392b9, /// 0xc88
			 0xa80177ba, /// 0xc8c
			 0x55b6dff2, /// 0xc90
			 0xaae0ffb5, /// 0xc94
			 0x4bc903f0, /// 0xc98
			 0x6e01ce25, /// 0xc9c
			 0x4e76ffc2, /// 0xca0
			 0xdc660f89, /// 0xca4
			 0x1640cfc8, /// 0xca8
			 0x38dc8d12, /// 0xcac
			 0xae6d0018, /// 0xcb0
			 0x41c42ef0, /// 0xcb4
			 0xf7a8e7a6, /// 0xcb8
			 0x9980b2ad, /// 0xcbc
			 0x27b93c86, /// 0xcc0
			 0x03fe3d28, /// 0xcc4
			 0xb7795fa4, /// 0xcc8
			 0x2c782da1, /// 0xccc
			 0x1a2a4ccd, /// 0xcd0
			 0xcf0f76eb, /// 0xcd4
			 0xb9c93b2d, /// 0xcd8
			 0x6cb9be27, /// 0xcdc
			 0x5edece82, /// 0xce0
			 0x5fe394f5, /// 0xce4
			 0x681b6943, /// 0xce8
			 0xd4f6ca1e, /// 0xcec
			 0x65779ca1, /// 0xcf0
			 0xed981dc2, /// 0xcf4
			 0x0f6ac307, /// 0xcf8
			 0xcc7c6438, /// 0xcfc
			 0x77a365d1, /// 0xd00
			 0xff839df5, /// 0xd04
			 0xacccd979, /// 0xd08
			 0x6fd8d171, /// 0xd0c
			 0x4d89db06, /// 0xd10
			 0x692e8b20, /// 0xd14
			 0x521530ce, /// 0xd18
			 0x705b077b, /// 0xd1c
			 0x4e2e78e3, /// 0xd20
			 0xa4c2030d, /// 0xd24
			 0xde451e24, /// 0xd28
			 0xdf30a85d, /// 0xd2c
			 0x5ce87303, /// 0xd30
			 0x5e13d93b, /// 0xd34
			 0x7b14a841, /// 0xd38
			 0x26452c32, /// 0xd3c
			 0x1f9682de, /// 0xd40
			 0x8520b99c, /// 0xd44
			 0xaf40fd86, /// 0xd48
			 0xb2ec9ecf, /// 0xd4c
			 0x81b8ffac, /// 0xd50
			 0x3cd63501, /// 0xd54
			 0xae5d3010, /// 0xd58
			 0x48f59a83, /// 0xd5c
			 0x564400af, /// 0xd60
			 0x429bda39, /// 0xd64
			 0x3d7c5012, /// 0xd68
			 0xc0fc09ef, /// 0xd6c
			 0xffbcd1e1, /// 0xd70
			 0xb35ecbf6, /// 0xd74
			 0x7f251d1d, /// 0xd78
			 0x823ab16f, /// 0xd7c
			 0xc851e3b2, /// 0xd80
			 0xede2b69b, /// 0xd84
			 0xe9932881, /// 0xd88
			 0x3f99ae96, /// 0xd8c
			 0x7bac563b, /// 0xd90
			 0x4f2160a1, /// 0xd94
			 0x126d0c38, /// 0xd98
			 0xa119699e, /// 0xd9c
			 0x1d0b15a2, /// 0xda0
			 0x1972ca9b, /// 0xda4
			 0x9b5d5266, /// 0xda8
			 0x5df7ebfe, /// 0xdac
			 0xb3a3fce7, /// 0xdb0
			 0x6436cafc, /// 0xdb4
			 0x095e3958, /// 0xdb8
			 0x958d535c, /// 0xdbc
			 0x6a6c9191, /// 0xdc0
			 0xd40ae12f, /// 0xdc4
			 0xef2007c2, /// 0xdc8
			 0xe21c1a6a, /// 0xdcc
			 0x916e7d31, /// 0xdd0
			 0x01bd27ab, /// 0xdd4
			 0x9ddc34c9, /// 0xdd8
			 0x0423f789, /// 0xddc
			 0x30b5134d, /// 0xde0
			 0x817357b0, /// 0xde4
			 0x1199b00a, /// 0xde8
			 0xb8ea6b4b, /// 0xdec
			 0x5e07b4bf, /// 0xdf0
			 0x3f84bd9d, /// 0xdf4
			 0xd72a7256, /// 0xdf8
			 0x2813c6bc, /// 0xdfc
			 0xda39b421, /// 0xe00
			 0xa2d7b208, /// 0xe04
			 0x7e0c2e14, /// 0xe08
			 0xc506f4cc, /// 0xe0c
			 0x0eba38b7, /// 0xe10
			 0x01a030a0, /// 0xe14
			 0x1034912b, /// 0xe18
			 0x5fb093a6, /// 0xe1c
			 0xd63ee803, /// 0xe20
			 0x3aace9c5, /// 0xe24
			 0x0990c4f4, /// 0xe28
			 0xfc80c558, /// 0xe2c
			 0x114429aa, /// 0xe30
			 0x53aaf953, /// 0xe34
			 0x125ea024, /// 0xe38
			 0x271bf91e, /// 0xe3c
			 0xa893b107, /// 0xe40
			 0xc6c5869f, /// 0xe44
			 0x289354db, /// 0xe48
			 0x23c79e54, /// 0xe4c
			 0x1f53ae3e, /// 0xe50
			 0x75bbea47, /// 0xe54
			 0x8799da3b, /// 0xe58
			 0x10486080, /// 0xe5c
			 0x2c4383e1, /// 0xe60
			 0xf2f631c5, /// 0xe64
			 0x331c5d1d, /// 0xe68
			 0xb8fd492a, /// 0xe6c
			 0x06d1f626, /// 0xe70
			 0x8d25ee8c, /// 0xe74
			 0x970e3322, /// 0xe78
			 0x41f4f79f, /// 0xe7c
			 0xd1100675, /// 0xe80
			 0x46e8a437, /// 0xe84
			 0x109ac893, /// 0xe88
			 0x9cabcbe8, /// 0xe8c
			 0x9cda602c, /// 0xe90
			 0x35bc066b, /// 0xe94
			 0x2c7169e4, /// 0xe98
			 0xc7c6deec, /// 0xe9c
			 0x54ddab32, /// 0xea0
			 0x6598116a, /// 0xea4
			 0x96f1fd8f, /// 0xea8
			 0x0f0f16de, /// 0xeac
			 0x6b358973, /// 0xeb0
			 0x1e17d48a, /// 0xeb4
			 0x7a0ea797, /// 0xeb8
			 0x83ff2945, /// 0xebc
			 0x1cb82a80, /// 0xec0
			 0xc41d4465, /// 0xec4
			 0xe36f408a, /// 0xec8
			 0x048cfa98, /// 0xecc
			 0x35c17459, /// 0xed0
			 0x35199ec3, /// 0xed4
			 0x03c2e2b0, /// 0xed8
			 0x687722ff, /// 0xedc
			 0x4c2d2e2c, /// 0xee0
			 0xfb0d925c, /// 0xee4
			 0xafdc043a, /// 0xee8
			 0xa795f470, /// 0xeec
			 0x868fac9e, /// 0xef0
			 0x60b2d0ea, /// 0xef4
			 0xe2fcd5ea, /// 0xef8
			 0xaf32ea9a, /// 0xefc
			 0x224a9a50, /// 0xf00
			 0x2494f31f, /// 0xf04
			 0xae28109f, /// 0xf08
			 0xa57307a8, /// 0xf0c
			 0x23d97e76, /// 0xf10
			 0x54e8d687, /// 0xf14
			 0xd978a69b, /// 0xf18
			 0xb7a36836, /// 0xf1c
			 0x9324f92f, /// 0xf20
			 0x6c0de113, /// 0xf24
			 0x897567dd, /// 0xf28
			 0xc0f138bd, /// 0xf2c
			 0x8690fc45, /// 0xf30
			 0xd26f419d, /// 0xf34
			 0x841232af, /// 0xf38
			 0xdbe94d44, /// 0xf3c
			 0xd66fab82, /// 0xf40
			 0x8d81f095, /// 0xf44
			 0xecdcbc13, /// 0xf48
			 0xfd00a32e, /// 0xf4c
			 0xe0d59977, /// 0xf50
			 0xd5b95309, /// 0xf54
			 0x3f13ea85, /// 0xf58
			 0x36a5957a, /// 0xf5c
			 0xa5852533, /// 0xf60
			 0xef3cc6b5, /// 0xf64
			 0x4c29de56, /// 0xf68
			 0x4f446a9f, /// 0xf6c
			 0x58b3fa69, /// 0xf70
			 0x21e5ae89, /// 0xf74
			 0x69f812a4, /// 0xf78
			 0xe3ad0d2d, /// 0xf7c
			 0xddbce637, /// 0xf80
			 0x496f5953, /// 0xf84
			 0x7fa64af2, /// 0xf88
			 0x5f49d834, /// 0xf8c
			 0xdd809d1f, /// 0xf90
			 0xb95d0645, /// 0xf94
			 0x9fb3f5b5, /// 0xf98
			 0xd2f2d5b5, /// 0xf9c
			 0xaa5e981d, /// 0xfa0
			 0x4dddf422, /// 0xfa4
			 0xc55d3374, /// 0xfa8
			 0xaf8edfe8, /// 0xfac
			 0x65692675, /// 0xfb0
			 0xea333134, /// 0xfb4
			 0x098457bc, /// 0xfb8
			 0x99418bbd, /// 0xfbc
			 0xc06ba694, /// 0xfc0
			 0x81d68614, /// 0xfc4
			 0x481a2c66, /// 0xfc8
			 0xfd4d81ac, /// 0xfcc
			 0x1663ee46, /// 0xfd0
			 0x74888678, /// 0xfd4
			 0x93154872, /// 0xfd8
			 0x930dc95d, /// 0xfdc
			 0x607493c9, /// 0xfe0
			 0x0805e471, /// 0xfe4
			 0x1b451ebc, /// 0xfe8
			 0x32ee59b6, /// 0xfec
			 0x9b980e35, /// 0xff0
			 0x71ae1c74, /// 0xff4
			 0x06995e6d, /// 0xff8
			 0x4ac7da02 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xb3611925, /// 0x0
			 0x3f266617, /// 0x4
			 0x2e6e3802, /// 0x8
			 0x89cc97dc, /// 0xc
			 0x28d73996, /// 0x10
			 0x54b37fd1, /// 0x14
			 0x29a7fd3e, /// 0x18
			 0xe3275d9b, /// 0x1c
			 0xc8cadc29, /// 0x20
			 0xd739583a, /// 0x24
			 0x9a9ec5c7, /// 0x28
			 0xbfe993f7, /// 0x2c
			 0x00bd479b, /// 0x30
			 0x1c8d6a11, /// 0x34
			 0xa6edd257, /// 0x38
			 0xe0557ce8, /// 0x3c
			 0x844d812e, /// 0x40
			 0x2aa77904, /// 0x44
			 0x7b033f42, /// 0x48
			 0x26700728, /// 0x4c
			 0x286529da, /// 0x50
			 0x148cb01d, /// 0x54
			 0x6e29e6fe, /// 0x58
			 0x4523ac64, /// 0x5c
			 0xf9670099, /// 0x60
			 0x23d952b8, /// 0x64
			 0xcfabf54a, /// 0x68
			 0x73631e0c, /// 0x6c
			 0xa270c420, /// 0x70
			 0xd1238e53, /// 0x74
			 0x254ab77a, /// 0x78
			 0xee31f813, /// 0x7c
			 0x29512e54, /// 0x80
			 0x3feca5f4, /// 0x84
			 0x345a9935, /// 0x88
			 0x8fd3f9e8, /// 0x8c
			 0xc54100f9, /// 0x90
			 0x36b207e0, /// 0x94
			 0x1d4f8697, /// 0x98
			 0x6f82902b, /// 0x9c
			 0x2048a2eb, /// 0xa0
			 0xaa8e65e8, /// 0xa4
			 0x2edfe782, /// 0xa8
			 0x02a2c227, /// 0xac
			 0xe7469cd7, /// 0xb0
			 0x1c36db50, /// 0xb4
			 0xb994e55f, /// 0xb8
			 0x8d07a8e2, /// 0xbc
			 0x8d2b71c5, /// 0xc0
			 0x6fcdd29d, /// 0xc4
			 0x5dc6578a, /// 0xc8
			 0x917de982, /// 0xcc
			 0xf9e2061b, /// 0xd0
			 0x6adddec9, /// 0xd4
			 0x49640d5f, /// 0xd8
			 0x365032e4, /// 0xdc
			 0x3f52f5d2, /// 0xe0
			 0xe6c5bd25, /// 0xe4
			 0x9905e27e, /// 0xe8
			 0x2a08b533, /// 0xec
			 0x1b768906, /// 0xf0
			 0xcfef5d0c, /// 0xf4
			 0x3418fd24, /// 0xf8
			 0xc62992d8, /// 0xfc
			 0xd388b7ea, /// 0x100
			 0x4a156823, /// 0x104
			 0x44b45a9c, /// 0x108
			 0xda92ce6f, /// 0x10c
			 0x76705e01, /// 0x110
			 0xd24c16f8, /// 0x114
			 0x0a5e24f1, /// 0x118
			 0xdd37b9c3, /// 0x11c
			 0x93105140, /// 0x120
			 0xbd89e5bb, /// 0x124
			 0xa6f3834c, /// 0x128
			 0x274614b5, /// 0x12c
			 0xf8ea5b18, /// 0x130
			 0x06568e65, /// 0x134
			 0xede883e6, /// 0x138
			 0x6955ce36, /// 0x13c
			 0x4ea8b43d, /// 0x140
			 0x9b442ee2, /// 0x144
			 0x7a7750c8, /// 0x148
			 0xe86e1993, /// 0x14c
			 0x29889ef3, /// 0x150
			 0x79505e20, /// 0x154
			 0x33ff5062, /// 0x158
			 0x74bcd3c6, /// 0x15c
			 0xdead8910, /// 0x160
			 0x27267dce, /// 0x164
			 0x68b89192, /// 0x168
			 0x36f6dffe, /// 0x16c
			 0x916b1b56, /// 0x170
			 0xe8773271, /// 0x174
			 0xb9896d5c, /// 0x178
			 0x94637d76, /// 0x17c
			 0x931114b2, /// 0x180
			 0x9c2771fe, /// 0x184
			 0xbf26ae7f, /// 0x188
			 0xf884d7fd, /// 0x18c
			 0x4194d884, /// 0x190
			 0xb88fa303, /// 0x194
			 0x4bf5fb50, /// 0x198
			 0xba0b64e8, /// 0x19c
			 0xe2184f20, /// 0x1a0
			 0xa2194449, /// 0x1a4
			 0xa40293cc, /// 0x1a8
			 0x48ee5708, /// 0x1ac
			 0x70b072a2, /// 0x1b0
			 0x0942771b, /// 0x1b4
			 0xe9305bdd, /// 0x1b8
			 0xa1e5e240, /// 0x1bc
			 0xcb744687, /// 0x1c0
			 0x9716284e, /// 0x1c4
			 0x80b6e356, /// 0x1c8
			 0x380b2d37, /// 0x1cc
			 0x3ba688b2, /// 0x1d0
			 0xf185c59e, /// 0x1d4
			 0x9c89bc5e, /// 0x1d8
			 0x0ba3bbd1, /// 0x1dc
			 0xdc10e670, /// 0x1e0
			 0x7c6a705e, /// 0x1e4
			 0x5004a604, /// 0x1e8
			 0x200710ff, /// 0x1ec
			 0xfc19f7fd, /// 0x1f0
			 0xbd227e0b, /// 0x1f4
			 0x3e947b0a, /// 0x1f8
			 0x3355810a, /// 0x1fc
			 0x27935823, /// 0x200
			 0x132bd6d0, /// 0x204
			 0xacf79972, /// 0x208
			 0x7e1bb293, /// 0x20c
			 0x8df63f12, /// 0x210
			 0xd09ace89, /// 0x214
			 0x8bb4a199, /// 0x218
			 0x8bc78e24, /// 0x21c
			 0xb96173b4, /// 0x220
			 0x209616ab, /// 0x224
			 0xd7e327c7, /// 0x228
			 0x90fb2725, /// 0x22c
			 0x9d08f6a8, /// 0x230
			 0xd8fc426a, /// 0x234
			 0xa280916a, /// 0x238
			 0x602480d6, /// 0x23c
			 0x641374c0, /// 0x240
			 0x9e9bc217, /// 0x244
			 0x50733968, /// 0x248
			 0xe05af217, /// 0x24c
			 0xe7e04107, /// 0x250
			 0xb58d042b, /// 0x254
			 0x89d9bfec, /// 0x258
			 0x15702944, /// 0x25c
			 0x7ccd631f, /// 0x260
			 0x52ec3854, /// 0x264
			 0xad9a491a, /// 0x268
			 0x7f679388, /// 0x26c
			 0xa9d2b961, /// 0x270
			 0xb561f838, /// 0x274
			 0xc0fc81d3, /// 0x278
			 0xf91538d3, /// 0x27c
			 0x076e9e72, /// 0x280
			 0xda8e1a8e, /// 0x284
			 0x6ed7ca7d, /// 0x288
			 0xaf61b9c0, /// 0x28c
			 0x8888d0bd, /// 0x290
			 0x63c62e5a, /// 0x294
			 0x51b70c10, /// 0x298
			 0x1306e4a0, /// 0x29c
			 0x6250c2a0, /// 0x2a0
			 0x49539d43, /// 0x2a4
			 0x4f7248cd, /// 0x2a8
			 0x20161981, /// 0x2ac
			 0x68155129, /// 0x2b0
			 0x97e572c2, /// 0x2b4
			 0x632af821, /// 0x2b8
			 0x1b632beb, /// 0x2bc
			 0xfa3a35ed, /// 0x2c0
			 0xed5220f4, /// 0x2c4
			 0x93137575, /// 0x2c8
			 0x1db208b4, /// 0x2cc
			 0xd85cd264, /// 0x2d0
			 0x59a1c86e, /// 0x2d4
			 0xe7de3180, /// 0x2d8
			 0xc08112e2, /// 0x2dc
			 0x2ce6513f, /// 0x2e0
			 0x785dab62, /// 0x2e4
			 0x3ca1e7dd, /// 0x2e8
			 0x3e3c7fad, /// 0x2ec
			 0x18368326, /// 0x2f0
			 0xc3bc6ead, /// 0x2f4
			 0x0fead341, /// 0x2f8
			 0x8392f39a, /// 0x2fc
			 0xa06b763d, /// 0x300
			 0x2bbc02ca, /// 0x304
			 0x2eb004d4, /// 0x308
			 0x0399838b, /// 0x30c
			 0x3377e1e2, /// 0x310
			 0x0f139bb7, /// 0x314
			 0x22aca5a6, /// 0x318
			 0x823fb1d7, /// 0x31c
			 0x388e910b, /// 0x320
			 0xc0dcf6ce, /// 0x324
			 0x2f1e74a9, /// 0x328
			 0x1afb97d7, /// 0x32c
			 0x7dc137a9, /// 0x330
			 0x8e749955, /// 0x334
			 0xcbf3bfe6, /// 0x338
			 0x8464db5c, /// 0x33c
			 0xf02cc22a, /// 0x340
			 0xd23406df, /// 0x344
			 0xdbc7ad9c, /// 0x348
			 0xa7d62f0c, /// 0x34c
			 0xdfd077de, /// 0x350
			 0x028cfad5, /// 0x354
			 0xb955cfe3, /// 0x358
			 0x66f57c0e, /// 0x35c
			 0xedebd879, /// 0x360
			 0x7078ae24, /// 0x364
			 0x976893fb, /// 0x368
			 0xb2c3add8, /// 0x36c
			 0x4507bdef, /// 0x370
			 0xb7c6af2f, /// 0x374
			 0xba64d698, /// 0x378
			 0x5c5384b6, /// 0x37c
			 0xb63e6e0f, /// 0x380
			 0x5e0fa0c0, /// 0x384
			 0xc773ebe5, /// 0x388
			 0xc822e8fc, /// 0x38c
			 0x3a109fe4, /// 0x390
			 0x8d8ab206, /// 0x394
			 0x4f4652d0, /// 0x398
			 0x6def2625, /// 0x39c
			 0x32b837d6, /// 0x3a0
			 0x1bcec713, /// 0x3a4
			 0x1fd93ba0, /// 0x3a8
			 0x188e6740, /// 0x3ac
			 0x9e93ed92, /// 0x3b0
			 0x75be5269, /// 0x3b4
			 0x9e5490b2, /// 0x3b8
			 0x3d7edc20, /// 0x3bc
			 0x2eeb993f, /// 0x3c0
			 0x381345d7, /// 0x3c4
			 0x0ff97fc5, /// 0x3c8
			 0x8baf7079, /// 0x3cc
			 0xb375bd4f, /// 0x3d0
			 0x4568ad04, /// 0x3d4
			 0x0ae7391f, /// 0x3d8
			 0x53876322, /// 0x3dc
			 0xc1d820f6, /// 0x3e0
			 0x52613c7d, /// 0x3e4
			 0x16e7ec5c, /// 0x3e8
			 0x34ef7191, /// 0x3ec
			 0x7b2974f2, /// 0x3f0
			 0xa19a7758, /// 0x3f4
			 0xa0e30fab, /// 0x3f8
			 0x044ed1f0, /// 0x3fc
			 0xd42f3b24, /// 0x400
			 0xf0e9f2a1, /// 0x404
			 0xc907e001, /// 0x408
			 0x2c8b6767, /// 0x40c
			 0x75d35484, /// 0x410
			 0x3a0b9c48, /// 0x414
			 0x96dc74a3, /// 0x418
			 0xc6a02e63, /// 0x41c
			 0x582852d2, /// 0x420
			 0xead67bfb, /// 0x424
			 0x7bde339d, /// 0x428
			 0x58d11c08, /// 0x42c
			 0xedf153a6, /// 0x430
			 0x05031c41, /// 0x434
			 0xa5a1713a, /// 0x438
			 0x4ac923ea, /// 0x43c
			 0x7559e5f7, /// 0x440
			 0xfda7a38e, /// 0x444
			 0xf4631a8a, /// 0x448
			 0x1b1d330d, /// 0x44c
			 0x7608f178, /// 0x450
			 0xca8833c7, /// 0x454
			 0x3604361c, /// 0x458
			 0xf976036a, /// 0x45c
			 0x547040e1, /// 0x460
			 0xc7d033e5, /// 0x464
			 0xc96f4399, /// 0x468
			 0x9f57040d, /// 0x46c
			 0xd149bdd4, /// 0x470
			 0xb3fc79f1, /// 0x474
			 0x97ab34be, /// 0x478
			 0x910648c2, /// 0x47c
			 0xdcd4bafe, /// 0x480
			 0xc032a8d7, /// 0x484
			 0x13b687d2, /// 0x488
			 0x5890ec97, /// 0x48c
			 0xbb429cee, /// 0x490
			 0x47d4135f, /// 0x494
			 0xcac19e90, /// 0x498
			 0xd7be1436, /// 0x49c
			 0x7f49da88, /// 0x4a0
			 0x80bc362c, /// 0x4a4
			 0x0c4b4db2, /// 0x4a8
			 0x6a7ba9df, /// 0x4ac
			 0xfadee084, /// 0x4b0
			 0x6fd83cd5, /// 0x4b4
			 0x511742f1, /// 0x4b8
			 0xa6298c70, /// 0x4bc
			 0x51fe6c34, /// 0x4c0
			 0xa1b08c3e, /// 0x4c4
			 0x27c192a9, /// 0x4c8
			 0xeef37285, /// 0x4cc
			 0x0eef7f6d, /// 0x4d0
			 0xc793c1fa, /// 0x4d4
			 0x372dd585, /// 0x4d8
			 0x65c728c2, /// 0x4dc
			 0x4684b6d7, /// 0x4e0
			 0x58a93d7a, /// 0x4e4
			 0x988aa790, /// 0x4e8
			 0x3d60fd7d, /// 0x4ec
			 0x459c647d, /// 0x4f0
			 0x551fe04e, /// 0x4f4
			 0x917ab72d, /// 0x4f8
			 0x6b770711, /// 0x4fc
			 0x280ee887, /// 0x500
			 0x5a985bb3, /// 0x504
			 0xed2e3bc9, /// 0x508
			 0x57db310e, /// 0x50c
			 0x6def73fd, /// 0x510
			 0x7079255e, /// 0x514
			 0x6c193253, /// 0x518
			 0xa1c03507, /// 0x51c
			 0xa79a94e7, /// 0x520
			 0x7375e63b, /// 0x524
			 0xac352fd7, /// 0x528
			 0xaad19468, /// 0x52c
			 0xd226a9f8, /// 0x530
			 0x26e1057f, /// 0x534
			 0x73c6e326, /// 0x538
			 0xcdd1af65, /// 0x53c
			 0xea19c3b9, /// 0x540
			 0xde5098f2, /// 0x544
			 0x6f3ebb66, /// 0x548
			 0x0fb69cdc, /// 0x54c
			 0x94a06df6, /// 0x550
			 0x0edb134d, /// 0x554
			 0xe4e4408e, /// 0x558
			 0x1f762ff3, /// 0x55c
			 0xc632c7c9, /// 0x560
			 0x40c23763, /// 0x564
			 0x504e1a2b, /// 0x568
			 0xd8a058eb, /// 0x56c
			 0xf1461564, /// 0x570
			 0xe73e1232, /// 0x574
			 0x9f5ba42d, /// 0x578
			 0x9448225f, /// 0x57c
			 0xf687e209, /// 0x580
			 0x9d039365, /// 0x584
			 0xe4f9a6ba, /// 0x588
			 0xc4f998d4, /// 0x58c
			 0x6f699c25, /// 0x590
			 0x67dd63fc, /// 0x594
			 0xa46cdba1, /// 0x598
			 0xbc183683, /// 0x59c
			 0x470d3c3e, /// 0x5a0
			 0x59a115a0, /// 0x5a4
			 0xe5d74750, /// 0x5a8
			 0xf6573e46, /// 0x5ac
			 0x1b77a488, /// 0x5b0
			 0xd2e9aa0d, /// 0x5b4
			 0xc054063e, /// 0x5b8
			 0xefae0fdd, /// 0x5bc
			 0xa3f9c373, /// 0x5c0
			 0x4d222caf, /// 0x5c4
			 0xdbd45fab, /// 0x5c8
			 0x02507654, /// 0x5cc
			 0xe5331289, /// 0x5d0
			 0x1277f2f2, /// 0x5d4
			 0x1c8f13e8, /// 0x5d8
			 0xadfb5f9a, /// 0x5dc
			 0x3d1f7f94, /// 0x5e0
			 0xe2e23f7d, /// 0x5e4
			 0xd1168aa5, /// 0x5e8
			 0xbf1b80fd, /// 0x5ec
			 0xe52af4a7, /// 0x5f0
			 0x02a8961d, /// 0x5f4
			 0x99702b31, /// 0x5f8
			 0xcfe354a7, /// 0x5fc
			 0x96b64eff, /// 0x600
			 0x7f79971e, /// 0x604
			 0x84f34c35, /// 0x608
			 0xa5f85695, /// 0x60c
			 0x1c5bee49, /// 0x610
			 0xe785cb02, /// 0x614
			 0xc0d24e09, /// 0x618
			 0x741367a6, /// 0x61c
			 0x2b802fd3, /// 0x620
			 0x719f102d, /// 0x624
			 0xdc83a1b9, /// 0x628
			 0xece0cffd, /// 0x62c
			 0x4083126d, /// 0x630
			 0x708e6124, /// 0x634
			 0x882a1a3d, /// 0x638
			 0xa9384150, /// 0x63c
			 0xa8aeaecc, /// 0x640
			 0x7201f925, /// 0x644
			 0x526fd8d6, /// 0x648
			 0xb0b6a037, /// 0x64c
			 0x75c3f70f, /// 0x650
			 0xb9c9e3b4, /// 0x654
			 0xd95d154e, /// 0x658
			 0x918f8122, /// 0x65c
			 0xf22154e6, /// 0x660
			 0x20239a4c, /// 0x664
			 0x9b0aab87, /// 0x668
			 0xe11c0fe2, /// 0x66c
			 0x88f9b68b, /// 0x670
			 0xa2aa40c5, /// 0x674
			 0x0aca3f18, /// 0x678
			 0x9fe5c058, /// 0x67c
			 0x55cf4dd4, /// 0x680
			 0x9cf1503a, /// 0x684
			 0x46368767, /// 0x688
			 0x175ebb71, /// 0x68c
			 0xf71b7396, /// 0x690
			 0xf938e71a, /// 0x694
			 0xe8ba998e, /// 0x698
			 0x33c22454, /// 0x69c
			 0xe6befba6, /// 0x6a0
			 0xf7d53b89, /// 0x6a4
			 0x9d54837b, /// 0x6a8
			 0xe54a49d3, /// 0x6ac
			 0xe19d74fc, /// 0x6b0
			 0x42efc6b5, /// 0x6b4
			 0xdd9ba0c7, /// 0x6b8
			 0x5e4bf98a, /// 0x6bc
			 0xcd68695d, /// 0x6c0
			 0xb6818382, /// 0x6c4
			 0x87572bab, /// 0x6c8
			 0xe03abfcd, /// 0x6cc
			 0x640d0787, /// 0x6d0
			 0x63a8b567, /// 0x6d4
			 0x7b3d5ddd, /// 0x6d8
			 0x725ed6f0, /// 0x6dc
			 0x42ef571d, /// 0x6e0
			 0xcc587c2e, /// 0x6e4
			 0x2faffdb7, /// 0x6e8
			 0xe5bf65e4, /// 0x6ec
			 0x81c4e0c9, /// 0x6f0
			 0x2f94d638, /// 0x6f4
			 0xb76bde3e, /// 0x6f8
			 0xf5bb9600, /// 0x6fc
			 0x479e7130, /// 0x700
			 0x85a894b6, /// 0x704
			 0xcd9872ce, /// 0x708
			 0xe53c1bea, /// 0x70c
			 0x20d5a2a8, /// 0x710
			 0x8db75b82, /// 0x714
			 0x28fc4995, /// 0x718
			 0xf80802fb, /// 0x71c
			 0x6a927a48, /// 0x720
			 0x52f8a424, /// 0x724
			 0xca97797b, /// 0x728
			 0x356d86f9, /// 0x72c
			 0x4b3be978, /// 0x730
			 0x02813032, /// 0x734
			 0x7b7f28cd, /// 0x738
			 0x55d896cc, /// 0x73c
			 0x5d4503a7, /// 0x740
			 0xed40f815, /// 0x744
			 0xfdf1a8d0, /// 0x748
			 0x1765cec2, /// 0x74c
			 0xb876e88d, /// 0x750
			 0xca06beff, /// 0x754
			 0xf7174425, /// 0x758
			 0xcd3129ef, /// 0x75c
			 0xbbb8d6ab, /// 0x760
			 0x16baaad1, /// 0x764
			 0x0df1dff0, /// 0x768
			 0x2a89439b, /// 0x76c
			 0x55d92533, /// 0x770
			 0x86024229, /// 0x774
			 0x94c7962b, /// 0x778
			 0xf4a7c458, /// 0x77c
			 0xd6e21852, /// 0x780
			 0x4f8ecc16, /// 0x784
			 0x49902087, /// 0x788
			 0xee82e315, /// 0x78c
			 0xec6c3fe7, /// 0x790
			 0x40917fab, /// 0x794
			 0x7ba125ed, /// 0x798
			 0x09b5cd15, /// 0x79c
			 0x7ffdb830, /// 0x7a0
			 0x14425fcf, /// 0x7a4
			 0x663101f0, /// 0x7a8
			 0x13c31b17, /// 0x7ac
			 0x450719d6, /// 0x7b0
			 0xc20a2e2a, /// 0x7b4
			 0x9466d15a, /// 0x7b8
			 0x4fc08511, /// 0x7bc
			 0xa35977be, /// 0x7c0
			 0xf34a2090, /// 0x7c4
			 0x49e0ca6f, /// 0x7c8
			 0x82702363, /// 0x7cc
			 0xc4803e9c, /// 0x7d0
			 0xe1c1fe83, /// 0x7d4
			 0x2e4b3981, /// 0x7d8
			 0x484d7f0b, /// 0x7dc
			 0x06f896b4, /// 0x7e0
			 0xc6cf59aa, /// 0x7e4
			 0x6c7c0f79, /// 0x7e8
			 0xd7807e4b, /// 0x7ec
			 0x5e486332, /// 0x7f0
			 0xcbd40fcc, /// 0x7f4
			 0x89947fcc, /// 0x7f8
			 0x824664e2, /// 0x7fc
			 0xa700e1f2, /// 0x800
			 0x403c2d5c, /// 0x804
			 0xa54daab2, /// 0x808
			 0x4bf5730a, /// 0x80c
			 0x7209cbb2, /// 0x810
			 0x71b5dd2b, /// 0x814
			 0x8821bf1b, /// 0x818
			 0xebce2da6, /// 0x81c
			 0x9f705eac, /// 0x820
			 0x2981540e, /// 0x824
			 0x1cacc0ac, /// 0x828
			 0x6cd99b88, /// 0x82c
			 0x64459475, /// 0x830
			 0xa704aaa2, /// 0x834
			 0xb846c8b3, /// 0x838
			 0x664be4eb, /// 0x83c
			 0x5af91d23, /// 0x840
			 0xade372de, /// 0x844
			 0xb94cb2ea, /// 0x848
			 0xe3300ab1, /// 0x84c
			 0x3cd6aeea, /// 0x850
			 0xb831991a, /// 0x854
			 0x42002e95, /// 0x858
			 0x478ccc9f, /// 0x85c
			 0x110a9661, /// 0x860
			 0x8dbc917c, /// 0x864
			 0x8c24f5d6, /// 0x868
			 0x5fdbbeca, /// 0x86c
			 0x92fd1379, /// 0x870
			 0xc2c12cb8, /// 0x874
			 0xf955f902, /// 0x878
			 0x5fbed39b, /// 0x87c
			 0xb33934a8, /// 0x880
			 0x29779059, /// 0x884
			 0x9e360d53, /// 0x888
			 0x808a6830, /// 0x88c
			 0xffd4ee5a, /// 0x890
			 0xc38ca11b, /// 0x894
			 0x3d4bc941, /// 0x898
			 0x6a338763, /// 0x89c
			 0x423ee001, /// 0x8a0
			 0xedc3050e, /// 0x8a4
			 0x7b4e6c12, /// 0x8a8
			 0xd79d1ab0, /// 0x8ac
			 0xcdda0029, /// 0x8b0
			 0x376657a8, /// 0x8b4
			 0xb6deae82, /// 0x8b8
			 0x9aea3876, /// 0x8bc
			 0xcf42511a, /// 0x8c0
			 0x06247ce5, /// 0x8c4
			 0x5a8eace5, /// 0x8c8
			 0x0703f3d2, /// 0x8cc
			 0xe0eadcc4, /// 0x8d0
			 0x1030e5d1, /// 0x8d4
			 0xc7de3a9e, /// 0x8d8
			 0xc8c91085, /// 0x8dc
			 0x0d59c29f, /// 0x8e0
			 0xd98e47d3, /// 0x8e4
			 0x957b1bf1, /// 0x8e8
			 0xabd80998, /// 0x8ec
			 0x527cd3a6, /// 0x8f0
			 0x7ec80ebd, /// 0x8f4
			 0x1b9ae624, /// 0x8f8
			 0x03e78f28, /// 0x8fc
			 0x33b25f59, /// 0x900
			 0x9b9b24bb, /// 0x904
			 0x7727f627, /// 0x908
			 0x572076b7, /// 0x90c
			 0x651a9273, /// 0x910
			 0x18eb4963, /// 0x914
			 0x95022e48, /// 0x918
			 0xd45e7a73, /// 0x91c
			 0x7d946160, /// 0x920
			 0xf7c83952, /// 0x924
			 0xeffdae25, /// 0x928
			 0xc0213224, /// 0x92c
			 0xc6028242, /// 0x930
			 0xc78421ef, /// 0x934
			 0x5bef644b, /// 0x938
			 0xd2ca80ad, /// 0x93c
			 0x563ba28a, /// 0x940
			 0xe43e7adb, /// 0x944
			 0x7de30590, /// 0x948
			 0x475b0a32, /// 0x94c
			 0x22659c50, /// 0x950
			 0x887e2028, /// 0x954
			 0xcb3839af, /// 0x958
			 0x3d59fc3a, /// 0x95c
			 0xd6b9b54a, /// 0x960
			 0xee79f68c, /// 0x964
			 0xb44e2393, /// 0x968
			 0xb116eb28, /// 0x96c
			 0x41ff2754, /// 0x970
			 0x2935dcb8, /// 0x974
			 0x9414aff6, /// 0x978
			 0x1a0d7ef6, /// 0x97c
			 0x2b004592, /// 0x980
			 0x968ce1ae, /// 0x984
			 0x68d5521b, /// 0x988
			 0xaf4bd2da, /// 0x98c
			 0x54ca66a1, /// 0x990
			 0x11a18bb2, /// 0x994
			 0x57907790, /// 0x998
			 0x9e85ff53, /// 0x99c
			 0xb231cd37, /// 0x9a0
			 0x23a5b59d, /// 0x9a4
			 0x29030168, /// 0x9a8
			 0xbacdb7ea, /// 0x9ac
			 0xa12d9aac, /// 0x9b0
			 0xa20c4007, /// 0x9b4
			 0xb6bececf, /// 0x9b8
			 0x7053b1d3, /// 0x9bc
			 0x9f7ce212, /// 0x9c0
			 0xce3fa663, /// 0x9c4
			 0xe5e74077, /// 0x9c8
			 0x19670d2d, /// 0x9cc
			 0x37bf63cc, /// 0x9d0
			 0x35a7880d, /// 0x9d4
			 0x209b9e1c, /// 0x9d8
			 0xe29a1700, /// 0x9dc
			 0x2412e753, /// 0x9e0
			 0xcc94f8cf, /// 0x9e4
			 0xae70906e, /// 0x9e8
			 0x1148d41e, /// 0x9ec
			 0x53804c56, /// 0x9f0
			 0x33b79798, /// 0x9f4
			 0xa06dab6d, /// 0x9f8
			 0xc376c2e9, /// 0x9fc
			 0x29c202c3, /// 0xa00
			 0x4cb7367a, /// 0xa04
			 0x4c2bf1b0, /// 0xa08
			 0xf06c2a11, /// 0xa0c
			 0xff62d61d, /// 0xa10
			 0x4ef4c97a, /// 0xa14
			 0x198b6bb1, /// 0xa18
			 0x0e14c88f, /// 0xa1c
			 0x9e02d0cd, /// 0xa20
			 0xa843782e, /// 0xa24
			 0xe7053693, /// 0xa28
			 0x6cecfd6e, /// 0xa2c
			 0xb280a289, /// 0xa30
			 0x487c99d4, /// 0xa34
			 0x25a922e0, /// 0xa38
			 0x772404cd, /// 0xa3c
			 0x978ab563, /// 0xa40
			 0x566d6d83, /// 0xa44
			 0x65844cff, /// 0xa48
			 0x7826cecb, /// 0xa4c
			 0xb5fdb4b2, /// 0xa50
			 0xc0f43f0e, /// 0xa54
			 0x99a185fb, /// 0xa58
			 0x4b4f9557, /// 0xa5c
			 0xb2d4ccdc, /// 0xa60
			 0x1e9657ba, /// 0xa64
			 0xbbcb8869, /// 0xa68
			 0xfecb2e25, /// 0xa6c
			 0xa6effcdc, /// 0xa70
			 0x57377e71, /// 0xa74
			 0x1ae1b66e, /// 0xa78
			 0xabef778e, /// 0xa7c
			 0x81ee364f, /// 0xa80
			 0x5f2db650, /// 0xa84
			 0x4ae97021, /// 0xa88
			 0xfff35120, /// 0xa8c
			 0xd59cfd23, /// 0xa90
			 0xfbda506a, /// 0xa94
			 0x4167f643, /// 0xa98
			 0x97b3ccb7, /// 0xa9c
			 0x0d419b35, /// 0xaa0
			 0x903ffc7c, /// 0xaa4
			 0x2150bb81, /// 0xaa8
			 0x7885aedd, /// 0xaac
			 0x18370760, /// 0xab0
			 0x856deb73, /// 0xab4
			 0xff081674, /// 0xab8
			 0x18f88d33, /// 0xabc
			 0x9719d010, /// 0xac0
			 0x2b32907a, /// 0xac4
			 0xf8b7e669, /// 0xac8
			 0xa36674a2, /// 0xacc
			 0xc59e95f8, /// 0xad0
			 0xc82b52b9, /// 0xad4
			 0xab58e89d, /// 0xad8
			 0x686bcf88, /// 0xadc
			 0xaee823f6, /// 0xae0
			 0x6b2195c9, /// 0xae4
			 0x248392e8, /// 0xae8
			 0x2c5c4a9a, /// 0xaec
			 0x504a52f7, /// 0xaf0
			 0xa1f0976b, /// 0xaf4
			 0xea0c1eb7, /// 0xaf8
			 0x162ff86b, /// 0xafc
			 0xdbdff22b, /// 0xb00
			 0x433f4b03, /// 0xb04
			 0xb227a857, /// 0xb08
			 0x5eb1d606, /// 0xb0c
			 0x3057d391, /// 0xb10
			 0xd47dd6a2, /// 0xb14
			 0x68581a9e, /// 0xb18
			 0xf54e19d4, /// 0xb1c
			 0xded27764, /// 0xb20
			 0xfca03027, /// 0xb24
			 0x55e202c9, /// 0xb28
			 0x1f5a3e2a, /// 0xb2c
			 0xe8a6cbed, /// 0xb30
			 0xef4871cc, /// 0xb34
			 0x5e57ea58, /// 0xb38
			 0xca5cb77a, /// 0xb3c
			 0x75e2b7a1, /// 0xb40
			 0xbec5fa5e, /// 0xb44
			 0x30d35bda, /// 0xb48
			 0xa9326a73, /// 0xb4c
			 0xbfee78ec, /// 0xb50
			 0x54d579ca, /// 0xb54
			 0xb31f4db1, /// 0xb58
			 0xfcfb1609, /// 0xb5c
			 0x34d00e21, /// 0xb60
			 0x79f8a94d, /// 0xb64
			 0x4f078226, /// 0xb68
			 0x115582a1, /// 0xb6c
			 0xc39e17a9, /// 0xb70
			 0xbae452ae, /// 0xb74
			 0xd8f8d1a6, /// 0xb78
			 0x1271eccf, /// 0xb7c
			 0x2e7a0795, /// 0xb80
			 0x0c9ecf42, /// 0xb84
			 0x3362f2e1, /// 0xb88
			 0x6bbd4ee2, /// 0xb8c
			 0x94d0c25b, /// 0xb90
			 0x6349a5cd, /// 0xb94
			 0xef29577e, /// 0xb98
			 0x5c9c23ee, /// 0xb9c
			 0x61a72ff6, /// 0xba0
			 0x764052d7, /// 0xba4
			 0x85ad2707, /// 0xba8
			 0xcfa153b6, /// 0xbac
			 0x455729ed, /// 0xbb0
			 0x68a69db8, /// 0xbb4
			 0xd7697267, /// 0xbb8
			 0x8d4f56c5, /// 0xbbc
			 0x6e7cfc6a, /// 0xbc0
			 0x6bd7f304, /// 0xbc4
			 0x4e94d3ee, /// 0xbc8
			 0xe5f5cf5b, /// 0xbcc
			 0x3a003d6f, /// 0xbd0
			 0x9bf58556, /// 0xbd4
			 0x09cfccb6, /// 0xbd8
			 0xdbcbc2c4, /// 0xbdc
			 0x88f456d9, /// 0xbe0
			 0x0780855c, /// 0xbe4
			 0xed242ffc, /// 0xbe8
			 0x21551198, /// 0xbec
			 0x4ec21a90, /// 0xbf0
			 0x7733d79f, /// 0xbf4
			 0x110add81, /// 0xbf8
			 0x0b5febce, /// 0xbfc
			 0x865aa42a, /// 0xc00
			 0x30af25d4, /// 0xc04
			 0x78cea710, /// 0xc08
			 0x494889ae, /// 0xc0c
			 0xdc7b7237, /// 0xc10
			 0xc85df66f, /// 0xc14
			 0xe2e75ea5, /// 0xc18
			 0xbee48bf3, /// 0xc1c
			 0x0197712f, /// 0xc20
			 0xc922250e, /// 0xc24
			 0xc4387268, /// 0xc28
			 0xd5e451f7, /// 0xc2c
			 0xd8149518, /// 0xc30
			 0x334eaffa, /// 0xc34
			 0xaeac3721, /// 0xc38
			 0x4bec78f8, /// 0xc3c
			 0xf78aa722, /// 0xc40
			 0x720535ec, /// 0xc44
			 0x0055ebbf, /// 0xc48
			 0xdd2ae195, /// 0xc4c
			 0xc229b4f3, /// 0xc50
			 0x259848da, /// 0xc54
			 0x905ba3bb, /// 0xc58
			 0xcd3a1955, /// 0xc5c
			 0x79783529, /// 0xc60
			 0x02343978, /// 0xc64
			 0xacb25661, /// 0xc68
			 0x7d2f49a7, /// 0xc6c
			 0x5d7c3764, /// 0xc70
			 0xe9e5fe4e, /// 0xc74
			 0xa28e429e, /// 0xc78
			 0x02b1daa8, /// 0xc7c
			 0x6f90be4b, /// 0xc80
			 0x6f84d959, /// 0xc84
			 0xc620cc08, /// 0xc88
			 0x5c2d7e66, /// 0xc8c
			 0xb90c6b46, /// 0xc90
			 0x92088415, /// 0xc94
			 0x274c678c, /// 0xc98
			 0x293b4239, /// 0xc9c
			 0xd5bbed7b, /// 0xca0
			 0xe1e59feb, /// 0xca4
			 0xb24703c5, /// 0xca8
			 0x58c48676, /// 0xcac
			 0x4dc2a0cb, /// 0xcb0
			 0x40eb2829, /// 0xcb4
			 0x3178805c, /// 0xcb8
			 0x466d3d91, /// 0xcbc
			 0x4c611894, /// 0xcc0
			 0xb41f1029, /// 0xcc4
			 0x1af4c1ab, /// 0xcc8
			 0xa23a1050, /// 0xccc
			 0x7e1ccfb5, /// 0xcd0
			 0xacb68c3e, /// 0xcd4
			 0x0e4fd54b, /// 0xcd8
			 0x0591291e, /// 0xcdc
			 0x24ba88ed, /// 0xce0
			 0xa6b92811, /// 0xce4
			 0x54d96ddd, /// 0xce8
			 0xbc437b04, /// 0xcec
			 0x53c0d2fe, /// 0xcf0
			 0xa5fdd1bd, /// 0xcf4
			 0x58973852, /// 0xcf8
			 0x8db61c0b, /// 0xcfc
			 0x20f9fd29, /// 0xd00
			 0xb2b15b28, /// 0xd04
			 0x40b6f552, /// 0xd08
			 0xf269c126, /// 0xd0c
			 0x467f27d1, /// 0xd10
			 0x48670ecf, /// 0xd14
			 0xd8d50e11, /// 0xd18
			 0xf33081db, /// 0xd1c
			 0x3a25a979, /// 0xd20
			 0x67e74132, /// 0xd24
			 0x2c86244b, /// 0xd28
			 0xcace634b, /// 0xd2c
			 0x4a45f237, /// 0xd30
			 0x6bac6715, /// 0xd34
			 0xde0f64ff, /// 0xd38
			 0x9f79d0d2, /// 0xd3c
			 0xdb710024, /// 0xd40
			 0x1a698a85, /// 0xd44
			 0x2d2bbffb, /// 0xd48
			 0x53975eec, /// 0xd4c
			 0x5016d00d, /// 0xd50
			 0x010c5f15, /// 0xd54
			 0xad754655, /// 0xd58
			 0x5f45e662, /// 0xd5c
			 0xe8615137, /// 0xd60
			 0x234b718f, /// 0xd64
			 0xc3677052, /// 0xd68
			 0xacbacad3, /// 0xd6c
			 0xb54be7d2, /// 0xd70
			 0xf399385b, /// 0xd74
			 0xccd1d72e, /// 0xd78
			 0x00394e41, /// 0xd7c
			 0xdc2a1262, /// 0xd80
			 0x4cd89998, /// 0xd84
			 0xc3153626, /// 0xd88
			 0xe77e3843, /// 0xd8c
			 0x96f15de3, /// 0xd90
			 0x369d36d7, /// 0xd94
			 0xd93c5102, /// 0xd98
			 0x343e71f5, /// 0xd9c
			 0x51dddaec, /// 0xda0
			 0x130ff688, /// 0xda4
			 0x5d686774, /// 0xda8
			 0xf57f5173, /// 0xdac
			 0xe0286875, /// 0xdb0
			 0x94b812eb, /// 0xdb4
			 0x1cce7414, /// 0xdb8
			 0x33fad2e6, /// 0xdbc
			 0xffb77324, /// 0xdc0
			 0xb2dbb296, /// 0xdc4
			 0xd8b3608b, /// 0xdc8
			 0xbb8088c4, /// 0xdcc
			 0x9c8a94af, /// 0xdd0
			 0xe470ccdc, /// 0xdd4
			 0xa247e1d6, /// 0xdd8
			 0x50908fbe, /// 0xddc
			 0xa90b2fbf, /// 0xde0
			 0x75d3524c, /// 0xde4
			 0x0b7f1440, /// 0xde8
			 0xaaf2be04, /// 0xdec
			 0x844de1d3, /// 0xdf0
			 0x5f20fc64, /// 0xdf4
			 0xa1755d44, /// 0xdf8
			 0x3877e292, /// 0xdfc
			 0x2730f61a, /// 0xe00
			 0x7cbbf234, /// 0xe04
			 0x1151af4e, /// 0xe08
			 0xaa01d90a, /// 0xe0c
			 0xb1e5565a, /// 0xe10
			 0x8e90b4dc, /// 0xe14
			 0x077e6799, /// 0xe18
			 0x598a2c3f, /// 0xe1c
			 0x9c091ef3, /// 0xe20
			 0xe8f6d157, /// 0xe24
			 0x553505f3, /// 0xe28
			 0x1f5d17a3, /// 0xe2c
			 0x32e5c2b7, /// 0xe30
			 0x812140d0, /// 0xe34
			 0xe8d160cf, /// 0xe38
			 0x18e5527a, /// 0xe3c
			 0x1e81cb8c, /// 0xe40
			 0xb00568ff, /// 0xe44
			 0x8ff3ae5e, /// 0xe48
			 0x61510734, /// 0xe4c
			 0xac187cc9, /// 0xe50
			 0x9c10546f, /// 0xe54
			 0xd1f92f34, /// 0xe58
			 0x7d971540, /// 0xe5c
			 0x9275cdb2, /// 0xe60
			 0x3fe13b65, /// 0xe64
			 0x72e8f2d6, /// 0xe68
			 0x86a86f57, /// 0xe6c
			 0x1a679e01, /// 0xe70
			 0x54f07495, /// 0xe74
			 0xfdc25b31, /// 0xe78
			 0x2c3173da, /// 0xe7c
			 0x00eb3c58, /// 0xe80
			 0xc33bcb45, /// 0xe84
			 0x8da1218f, /// 0xe88
			 0xf55fb380, /// 0xe8c
			 0xb3318ae0, /// 0xe90
			 0x219b4c0c, /// 0xe94
			 0x7757226e, /// 0xe98
			 0x74786d58, /// 0xe9c
			 0x681e4969, /// 0xea0
			 0x7ab7a634, /// 0xea4
			 0xdc822d8b, /// 0xea8
			 0x1e33378a, /// 0xeac
			 0x39863733, /// 0xeb0
			 0xfd0677f0, /// 0xeb4
			 0x61c7f8d8, /// 0xeb8
			 0xdc43ca07, /// 0xebc
			 0x4e19a97c, /// 0xec0
			 0x20b1b0ca, /// 0xec4
			 0xa51578fd, /// 0xec8
			 0xe57a2e10, /// 0xecc
			 0xf48fc59a, /// 0xed0
			 0xca34d20f, /// 0xed4
			 0x0b86eaac, /// 0xed8
			 0x4148fb8b, /// 0xedc
			 0xb03818d1, /// 0xee0
			 0xd2d37944, /// 0xee4
			 0x05896aeb, /// 0xee8
			 0xad78a38d, /// 0xeec
			 0xb6acba59, /// 0xef0
			 0x738027a0, /// 0xef4
			 0xafdb1d25, /// 0xef8
			 0x191d6259, /// 0xefc
			 0x27df74c6, /// 0xf00
			 0xd8d97517, /// 0xf04
			 0x829dadb2, /// 0xf08
			 0xe9ee7204, /// 0xf0c
			 0xf28f1953, /// 0xf10
			 0x0f5c3932, /// 0xf14
			 0x7fe86e05, /// 0xf18
			 0x3dabdca8, /// 0xf1c
			 0x247d75be, /// 0xf20
			 0x5a53d9d5, /// 0xf24
			 0xbb67d886, /// 0xf28
			 0x080114f8, /// 0xf2c
			 0xfa4a3500, /// 0xf30
			 0xcc9e2230, /// 0xf34
			 0xfa77c44b, /// 0xf38
			 0x98a16da0, /// 0xf3c
			 0xc7862fb0, /// 0xf40
			 0xf07d5cd0, /// 0xf44
			 0x051cba44, /// 0xf48
			 0x06f417f9, /// 0xf4c
			 0x8f3d4d10, /// 0xf50
			 0x0bcf2d6a, /// 0xf54
			 0x68eb2aac, /// 0xf58
			 0xf5e8fa6b, /// 0xf5c
			 0x10d00646, /// 0xf60
			 0xefee1e4a, /// 0xf64
			 0x5805c993, /// 0xf68
			 0x47c0db2a, /// 0xf6c
			 0xd087e780, /// 0xf70
			 0x66b003af, /// 0xf74
			 0x2776f068, /// 0xf78
			 0xc19c0a23, /// 0xf7c
			 0xb9cfaac3, /// 0xf80
			 0xd304dc48, /// 0xf84
			 0x87f4b8f7, /// 0xf88
			 0x49038f40, /// 0xf8c
			 0xa4b3f5b5, /// 0xf90
			 0x47374f95, /// 0xf94
			 0x4148d101, /// 0xf98
			 0x8ad69518, /// 0xf9c
			 0x8a173f82, /// 0xfa0
			 0xfb00fa18, /// 0xfa4
			 0x2a43c305, /// 0xfa8
			 0x4192ea22, /// 0xfac
			 0x976efa8f, /// 0xfb0
			 0xf3714676, /// 0xfb4
			 0x7d2a54bd, /// 0xfb8
			 0x69fb50b1, /// 0xfbc
			 0xddf7b5f9, /// 0xfc0
			 0x5e5b4de6, /// 0xfc4
			 0x3ab31a9d, /// 0xfc8
			 0x6ba9deee, /// 0xfcc
			 0xae0c4f8b, /// 0xfd0
			 0x47b0714f, /// 0xfd4
			 0xa08671cc, /// 0xfd8
			 0x261a5a35, /// 0xfdc
			 0xb29c4953, /// 0xfe0
			 0xe46c4323, /// 0xfe4
			 0x960735cb, /// 0xfe8
			 0x5167768b, /// 0xfec
			 0x56b1d056, /// 0xff0
			 0xc58566b4, /// 0xff4
			 0x30c7b82d, /// 0xff8
			 0x412770ab /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00004
			 0xffc00001,                                                  // -signaling NaN                              /// 00008
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x0e000001,                                                  // Trailing 1s:                                /// 00014
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0001c
			 0x00000000,                                                  // zero                                        /// 00020
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00028
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0002c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00034
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0003c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00040
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00048
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00050
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00054
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00058
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00060
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00064
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00068
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00070
			 0x0c700000,                                                  // Leading 1s:                                 /// 00074
			 0x0e000001,                                                  // Trailing 1s:                                /// 00078
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0007c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00084
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00088
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0009c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 000d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x4b000000,                                                  // 8388608.0                                   /// 000d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00104
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00108
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0010c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00110
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00114
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0011c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00120
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00124
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0e000007,                                                  // Trailing 1s:                                /// 0012c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00138
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0014c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00150
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00154
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00158
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0015c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00164
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00168
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0016c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00178
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0017c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00180
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00184
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00188
			 0x0c780000,                                                  // Leading 1s:                                 /// 0018c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00190
			 0x0e000001,                                                  // Trailing 1s:                                /// 00194
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00198
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0019c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 001bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00208
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00210
			 0x0c780000,                                                  // Leading 1s:                                 /// 00214
			 0x00011111,                                                  // 9.7958E-41                                  /// 00218
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0021c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00220
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0022c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00230
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00238
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0023c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00240
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00244
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00248
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0024c
			 0x80000000,                                                  // -zero                                       /// 00250
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00254
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0025c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00260
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0026c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00270
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00274
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00278
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0027c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00280
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00284
			 0x3f028f5c,                                                  // 0.51                                        /// 00288
			 0x0e000007,                                                  // Trailing 1s:                                /// 0028c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00290
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00298
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0029c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00300
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00308
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00310
			 0x0e000001,                                                  // Trailing 1s:                                /// 00314
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00320
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00324
			 0x00000000,                                                  // zero                                        /// 00328
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0032c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0xbf028f5c,                                                  // -0.51                                       /// 00338
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0033c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00348
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0034c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00354
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00358
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0035c
			 0xffc00001,                                                  // -signaling NaN                              /// 00360
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0xff800000,                                                  // -inf                                        /// 0036c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00378
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0037c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00384
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0038c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x00000000,                                                  // zero                                        /// 00394
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0039c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00400
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00408
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00410
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00414
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0c600000,                                                  // Leading 1s:                                 /// 0041c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00420
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00424
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00428
			 0xffc00001,                                                  // -signaling NaN                              /// 0042c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00430
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00434
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00438
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0043c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00440
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0xbf028f5c,                                                  // -0.51                                       /// 00448
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0044c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00450
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00454
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00458
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0045c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00468
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00474
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0047c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00488
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00494
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00498
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004d0
			 0x33333333,                                                  // 4 random values                             /// 004d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7f800000,                                                  // inf                                         /// 004e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004f0
			 0x55555555,                                                  // 4 random values                             /// 004f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00504
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00514
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00518
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x7f800000,                                                  // inf                                         /// 00520
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00528
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0052c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00530
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00534
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00538
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x3f028f5c,                                                  // 0.51                                        /// 00544
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00548
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00550
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00554
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00558
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0055c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00564
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00570
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00578
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0057c
			 0x3f028f5c,                                                  // 0.51                                        /// 00580
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00588
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0058c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00590
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00598
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00604
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00610
			 0x00000000,                                                  // zero                                        /// 00614
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00618
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0061c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00628
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0062c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x7fc00001,                                                  // signaling NaN                               /// 00638
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00644
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00648
			 0x0c600000,                                                  // Leading 1s:                                 /// 0064c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x0c780000,                                                  // Leading 1s:                                 /// 00658
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0065c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00664
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00668
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00670
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00674
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0067c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00680
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00684
			 0x80011111,                                                  // -9.7958E-41                                 /// 00688
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0068c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00690
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00694
			 0xbf028f5c,                                                  // -0.51                                       /// 00698
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0069c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00700
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00704
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00708
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00710
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00714
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00718
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0071c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00720
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00724
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00728
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0072c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00730
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00734
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00738
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0073c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00740
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00748
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0074c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0075c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00760
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00764
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0076c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00770
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00774
			 0x7f800000,                                                  // inf                                         /// 00778
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x3f028f5c,                                                  // 0.51                                        /// 00780
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00784
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00788
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00794
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x4b000000,                                                  // 8388608.0                                   /// 007b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x00000000,                                                  // zero                                        /// 007dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007e4
			 0xcb000000,                                                  // -8388608.0                                  /// 007e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00800
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00808
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0080c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00810
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00818
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0081c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00820
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00824
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0082c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00830
			 0x33333333,                                                  // 4 random values                             /// 00834
			 0xbf028f5c,                                                  // -0.51                                       /// 00838
			 0x0e000003,                                                  // Trailing 1s:                                /// 0083c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00840
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0084c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00850
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00854
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00858
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00860
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00864
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00868
			 0xff800000,                                                  // -inf                                        /// 0086c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00874
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00878
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0xffc00001,                                                  // -signaling NaN                              /// 00880
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00884
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0088c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00894
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0089c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00900
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0090c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00910
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00914
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00918
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00920
			 0xffc00001,                                                  // -signaling NaN                              /// 00924
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00928
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0092c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00930
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0093c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00944
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0094c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00950
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00954
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00958
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00960
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00968
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0096c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00970
			 0x0c700000,                                                  // Leading 1s:                                 /// 00974
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0097c
			 0x4b000000,                                                  // 8388608.0                                   /// 00980
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00984
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00988
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0098c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00990
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00994
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00998
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 009c4
			 0xffc00001,                                                  // -signaling NaN                              /// 009c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009d0
			 0x33333333,                                                  // 4 random values                             /// 009d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 009e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a08
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a28
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xffc00001,                                                  // -signaling NaN                              /// 00a38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a40
			 0x7fc00001,                                                  // signaling NaN                               /// 00a44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a58
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a60
			 0xbf028f5c,                                                  // -0.51                                       /// 00a64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a78
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a88
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ab0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ab4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ab8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00abc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ac4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ac8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00acc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ad0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x55555555,                                                  // 4 random values                             /// 00adc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ae0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ae4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ae8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aec
			 0x7fc00001,                                                  // signaling NaN                               /// 00af0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00af8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00afc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b00
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b24
			 0xff800000,                                                  // -inf                                        /// 00b28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b2c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b3c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b74
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b78
			 0x4b000000,                                                  // 8388608.0                                   /// 00b7c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ba0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bb0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bb4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bc4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bc8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bcc
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd0
			 0xff800000,                                                  // -inf                                        /// 00bd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bdc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00be4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00be8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bf4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bf8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c00
			 0x00000000,                                                  // zero                                        /// 00c04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0xcb000000,                                                  // -8388608.0                                  /// 00c48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c50
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c58
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c80
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x7fc00001,                                                  // signaling NaN                               /// 00c98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ca4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cb8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cbc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cc0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cc8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cd4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cd8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cdc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ce0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ce8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cf0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cf4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d00
			 0xffc00001,                                                  // -signaling NaN                              /// 00d04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d0c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d14
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d18
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x00000000,                                                  // zero                                        /// 00d24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d30
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d54
			 0x33333333,                                                  // 4 random values                             /// 00d58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d8c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d90
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d9c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00da0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00da4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00da8
			 0x7f800000,                                                  // inf                                         /// 00dac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00db8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dc8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dcc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dd0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd4
			 0x7fc00001,                                                  // signaling NaN                               /// 00dd8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00de0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00df0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00df8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e20
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e40
			 0xffc00001,                                                  // -signaling NaN                              /// 00e44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e48
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e7c
			 0x80000000,                                                  // -zero                                       /// 00e80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e84
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0xbf028f5c,                                                  // -0.51                                       /// 00e94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ea8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00eb0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eb4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00eb8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ebc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ec0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ec4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ecc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ed0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ed4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00edc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ee0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ee8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00eec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ef0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ef4
			 0x80000000,                                                  // -zero                                       /// 00ef8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00efc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f08
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f10
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f2c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00f50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f54
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fa4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0xbf028f5c,                                                  // -0.51                                       /// 00fc0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x33333333,                                                  // 4 random values                             /// 00fcc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fd4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fd8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fdc
			 0xcb000000,                                                  // -8388608.0                                  /// 00fe0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00000080                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x4debd2ae, /// 0x0
			 0x7e7930bb, /// 0x4
			 0xa0208143, /// 0x8
			 0xfa7f4a0c, /// 0xc
			 0x22d7c8b7, /// 0x10
			 0x36f40775, /// 0x14
			 0x8baa7227, /// 0x18
			 0x5cbf5f27, /// 0x1c
			 0x202efd4e, /// 0x20
			 0xcc6c8963, /// 0x24
			 0xd494efd8, /// 0x28
			 0x42d44af9, /// 0x2c
			 0x5bb06cb1, /// 0x30
			 0x79fceb1d, /// 0x34
			 0x55ac7d71, /// 0x38
			 0xbd190875, /// 0x3c
			 0xf76019e8, /// 0x40
			 0x5a7eba91, /// 0x44
			 0x4ae2509f, /// 0x48
			 0x4018b5a7, /// 0x4c
			 0x5ebcbb2a, /// 0x50
			 0x12d10fee, /// 0x54
			 0xcf98f338, /// 0x58
			 0xf32e68bf, /// 0x5c
			 0x54a2129c, /// 0x60
			 0xa01550df, /// 0x64
			 0x35596853, /// 0x68
			 0x47c21b94, /// 0x6c
			 0x04d0fe01, /// 0x70
			 0x5d7d7d18, /// 0x74
			 0xde676ee3, /// 0x78
			 0xf3a2bbf8, /// 0x7c
			 0x012eff45, /// 0x80
			 0x7b90ec81, /// 0x84
			 0x0edacc83, /// 0x88
			 0x97be6059, /// 0x8c
			 0x888f1ba1, /// 0x90
			 0x0b406240, /// 0x94
			 0x4fca4b2e, /// 0x98
			 0x87e38e53, /// 0x9c
			 0xfbda7fb5, /// 0xa0
			 0xbe275625, /// 0xa4
			 0x700c16d7, /// 0xa8
			 0x876f5748, /// 0xac
			 0x8e2cd3ec, /// 0xb0
			 0xdd732200, /// 0xb4
			 0x8f0febd4, /// 0xb8
			 0x820925c8, /// 0xbc
			 0xa2cbeac7, /// 0xc0
			 0xcfed86b5, /// 0xc4
			 0x127f2700, /// 0xc8
			 0xe48df306, /// 0xcc
			 0xb6447c67, /// 0xd0
			 0xc7968bf4, /// 0xd4
			 0x4901e0f9, /// 0xd8
			 0x920122a0, /// 0xdc
			 0xd155ac4a, /// 0xe0
			 0x0f651803, /// 0xe4
			 0x53248f0d, /// 0xe8
			 0x60aebe37, /// 0xec
			 0x238f6c04, /// 0xf0
			 0x477b315c, /// 0xf4
			 0xd22dfb8d, /// 0xf8
			 0xac135a48, /// 0xfc
			 0x1943e8d7, /// 0x100
			 0x90847b76, /// 0x104
			 0x4c570092, /// 0x108
			 0x21995e30, /// 0x10c
			 0x348605e5, /// 0x110
			 0x7e25f92a, /// 0x114
			 0xa2b4b8fd, /// 0x118
			 0x104ce3a7, /// 0x11c
			 0x96f5c2b4, /// 0x120
			 0xf371ad26, /// 0x124
			 0xa8d1deaa, /// 0x128
			 0x1d4fb201, /// 0x12c
			 0x7f741712, /// 0x130
			 0xf8a1491c, /// 0x134
			 0x86db1d9b, /// 0x138
			 0x88bf1773, /// 0x13c
			 0x4af35760, /// 0x140
			 0x742d08e1, /// 0x144
			 0xbddcf06b, /// 0x148
			 0x474699b2, /// 0x14c
			 0xc5c957e9, /// 0x150
			 0xc63c0611, /// 0x154
			 0xc9299641, /// 0x158
			 0xe108ac36, /// 0x15c
			 0xe3427078, /// 0x160
			 0x9d215b03, /// 0x164
			 0x1b66930d, /// 0x168
			 0x4f4a28ff, /// 0x16c
			 0xc4d4b60f, /// 0x170
			 0x0f4ff793, /// 0x174
			 0x569afbf3, /// 0x178
			 0xd07654d4, /// 0x17c
			 0x4125200b, /// 0x180
			 0x8a46a353, /// 0x184
			 0xe288fa08, /// 0x188
			 0x215cd4f0, /// 0x18c
			 0x9d16840a, /// 0x190
			 0xce2509b8, /// 0x194
			 0x2cc558ba, /// 0x198
			 0x836cd68e, /// 0x19c
			 0xcdb4c9c1, /// 0x1a0
			 0x8199d993, /// 0x1a4
			 0xbdf4fb31, /// 0x1a8
			 0xc5748cde, /// 0x1ac
			 0x4245131f, /// 0x1b0
			 0x7a43a361, /// 0x1b4
			 0x49c13a67, /// 0x1b8
			 0xb5fc6e48, /// 0x1bc
			 0xa88628dd, /// 0x1c0
			 0x1cdfadc9, /// 0x1c4
			 0xa20983cd, /// 0x1c8
			 0x748d8d5d, /// 0x1cc
			 0x39e07fac, /// 0x1d0
			 0x756bc423, /// 0x1d4
			 0x0587800c, /// 0x1d8
			 0xa7199ce9, /// 0x1dc
			 0x9df12ff3, /// 0x1e0
			 0x599a4661, /// 0x1e4
			 0x4ee25df2, /// 0x1e8
			 0xa0888213, /// 0x1ec
			 0xedc52fda, /// 0x1f0
			 0xd4bc4f0c, /// 0x1f4
			 0xd7498735, /// 0x1f8
			 0xe57099c1, /// 0x1fc
			 0xfd9ac920, /// 0x200
			 0xbc5d4700, /// 0x204
			 0xe07916d7, /// 0x208
			 0x418c5d12, /// 0x20c
			 0x5628f150, /// 0x210
			 0xb4f0d58b, /// 0x214
			 0x6a42dd88, /// 0x218
			 0xf1b02992, /// 0x21c
			 0xe3b764d8, /// 0x220
			 0xfaa2954b, /// 0x224
			 0x1dd2e9f2, /// 0x228
			 0x917911b5, /// 0x22c
			 0x9763d28f, /// 0x230
			 0x89101680, /// 0x234
			 0x7dbe459f, /// 0x238
			 0x01055571, /// 0x23c
			 0xf9e015ac, /// 0x240
			 0xbb344ae4, /// 0x244
			 0xbff07bbe, /// 0x248
			 0x956baf71, /// 0x24c
			 0x6d7b2a80, /// 0x250
			 0x69964311, /// 0x254
			 0x6e3e32ea, /// 0x258
			 0xef805d0f, /// 0x25c
			 0x2b300d35, /// 0x260
			 0x4b837b22, /// 0x264
			 0xb9ef3904, /// 0x268
			 0x07eebb56, /// 0x26c
			 0xf3d72aad, /// 0x270
			 0x57d52ca3, /// 0x274
			 0x02c6da8a, /// 0x278
			 0x9eec4eaf, /// 0x27c
			 0xd6fccf1a, /// 0x280
			 0x87a517d6, /// 0x284
			 0x75dc419f, /// 0x288
			 0x72b42df8, /// 0x28c
			 0x9db37821, /// 0x290
			 0xfe051ef3, /// 0x294
			 0xcc4ead25, /// 0x298
			 0x58fff627, /// 0x29c
			 0xf1bd27c2, /// 0x2a0
			 0xd5f6443c, /// 0x2a4
			 0x9c52713c, /// 0x2a8
			 0x4395a61b, /// 0x2ac
			 0xe59811eb, /// 0x2b0
			 0xc7e4b54a, /// 0x2b4
			 0x09aa139f, /// 0x2b8
			 0xd7227d1d, /// 0x2bc
			 0xbdeadd1e, /// 0x2c0
			 0xf7eade9a, /// 0x2c4
			 0xf461cef2, /// 0x2c8
			 0xf076709f, /// 0x2cc
			 0x9e1f5814, /// 0x2d0
			 0x9fd104f4, /// 0x2d4
			 0x08b97f9d, /// 0x2d8
			 0xa1587070, /// 0x2dc
			 0x1c89bc5e, /// 0x2e0
			 0xcf7b0383, /// 0x2e4
			 0x943aec4e, /// 0x2e8
			 0x8e1757e0, /// 0x2ec
			 0x2e4dca35, /// 0x2f0
			 0xf7a6da65, /// 0x2f4
			 0xf9e00cf4, /// 0x2f8
			 0x060d1460, /// 0x2fc
			 0x1e3408d4, /// 0x300
			 0x13a4da07, /// 0x304
			 0xf1405d3a, /// 0x308
			 0x86d5e0f3, /// 0x30c
			 0x34f6149e, /// 0x310
			 0x3d924c5a, /// 0x314
			 0x2b888611, /// 0x318
			 0x3bbeaaa5, /// 0x31c
			 0x66468175, /// 0x320
			 0x91ba2538, /// 0x324
			 0x5b3e1710, /// 0x328
			 0x3a22dd88, /// 0x32c
			 0xf457d39e, /// 0x330
			 0x6aa62d83, /// 0x334
			 0x19d87522, /// 0x338
			 0x50d5bd7b, /// 0x33c
			 0x6397b100, /// 0x340
			 0x3cc5fb81, /// 0x344
			 0xceda50ac, /// 0x348
			 0xfb68448a, /// 0x34c
			 0xcbf4ebde, /// 0x350
			 0x0ae6760d, /// 0x354
			 0x94c6c705, /// 0x358
			 0x4c4ed9f0, /// 0x35c
			 0x086c4af0, /// 0x360
			 0x6eab655e, /// 0x364
			 0x4300b86e, /// 0x368
			 0x3ea85fdc, /// 0x36c
			 0x62570f1c, /// 0x370
			 0xb27315c0, /// 0x374
			 0xa796e355, /// 0x378
			 0x2bbc3bf3, /// 0x37c
			 0x41e3666b, /// 0x380
			 0x63ed384c, /// 0x384
			 0xfdac9102, /// 0x388
			 0x0203c31b, /// 0x38c
			 0x7947470c, /// 0x390
			 0x8fa734ef, /// 0x394
			 0x6e6f26e2, /// 0x398
			 0x33a16172, /// 0x39c
			 0xcd0e9e4b, /// 0x3a0
			 0xe8304f66, /// 0x3a4
			 0x2954c387, /// 0x3a8
			 0x780434a4, /// 0x3ac
			 0x1c1d948b, /// 0x3b0
			 0xe5c6b641, /// 0x3b4
			 0x279ded4e, /// 0x3b8
			 0x6b24887f, /// 0x3bc
			 0x1b5d9ac7, /// 0x3c0
			 0x647dcbe3, /// 0x3c4
			 0x8fed34f2, /// 0x3c8
			 0xf3be114a, /// 0x3cc
			 0x9bf5459d, /// 0x3d0
			 0x517b06ff, /// 0x3d4
			 0xc900e396, /// 0x3d8
			 0x5551e16d, /// 0x3dc
			 0x0da5cd07, /// 0x3e0
			 0x41604729, /// 0x3e4
			 0x67ee87bf, /// 0x3e8
			 0x081017c6, /// 0x3ec
			 0x2468ca9f, /// 0x3f0
			 0x5dbecd8d, /// 0x3f4
			 0x0c6edfa2, /// 0x3f8
			 0x2e87a137, /// 0x3fc
			 0x9085f3b6, /// 0x400
			 0x53bf59c4, /// 0x404
			 0x2db6daa6, /// 0x408
			 0x03463dd0, /// 0x40c
			 0x05794b1e, /// 0x410
			 0x2e64576e, /// 0x414
			 0xe7d6b799, /// 0x418
			 0x755e70bf, /// 0x41c
			 0x33c95827, /// 0x420
			 0x424081c7, /// 0x424
			 0x77425c37, /// 0x428
			 0x468b7e15, /// 0x42c
			 0xd676b095, /// 0x430
			 0x6d83b0b1, /// 0x434
			 0x05c54ff4, /// 0x438
			 0x070ef6d3, /// 0x43c
			 0xa64609e0, /// 0x440
			 0xc3bd08cb, /// 0x444
			 0xbce8076a, /// 0x448
			 0x4a17e979, /// 0x44c
			 0x35f2847a, /// 0x450
			 0x523d8a1d, /// 0x454
			 0x90ac1122, /// 0x458
			 0x17820155, /// 0x45c
			 0x8579d943, /// 0x460
			 0x8666979a, /// 0x464
			 0x8a4199ff, /// 0x468
			 0x2c487efb, /// 0x46c
			 0x7d675b54, /// 0x470
			 0xdac67f91, /// 0x474
			 0x424213c4, /// 0x478
			 0xdf8602ab, /// 0x47c
			 0x2a6c5fa9, /// 0x480
			 0x6b07a53c, /// 0x484
			 0xd41ffb3d, /// 0x488
			 0x234ac88f, /// 0x48c
			 0x7c37cf1e, /// 0x490
			 0x6fc2371a, /// 0x494
			 0x572c150e, /// 0x498
			 0x9f5f93ef, /// 0x49c
			 0x52a98fba, /// 0x4a0
			 0xc4780a74, /// 0x4a4
			 0x01291ce8, /// 0x4a8
			 0x6bf37bfa, /// 0x4ac
			 0xc49ff682, /// 0x4b0
			 0xed68f34a, /// 0x4b4
			 0x6a99854b, /// 0x4b8
			 0x841ac83d, /// 0x4bc
			 0xd08b07d8, /// 0x4c0
			 0x6988a4b6, /// 0x4c4
			 0xab507ac3, /// 0x4c8
			 0xa9de6ee8, /// 0x4cc
			 0xd1c08d42, /// 0x4d0
			 0x322db134, /// 0x4d4
			 0x3b66ada4, /// 0x4d8
			 0x178f6fa5, /// 0x4dc
			 0x2d49beab, /// 0x4e0
			 0xa1ce4046, /// 0x4e4
			 0x8da97cca, /// 0x4e8
			 0xb472de6e, /// 0x4ec
			 0xc966c2ae, /// 0x4f0
			 0x3ef8cb2c, /// 0x4f4
			 0x75fcac39, /// 0x4f8
			 0x22eb743f, /// 0x4fc
			 0x992a315d, /// 0x500
			 0xab00a988, /// 0x504
			 0x45c33a57, /// 0x508
			 0x963349bb, /// 0x50c
			 0x02b4f64e, /// 0x510
			 0x22db5060, /// 0x514
			 0xeb5a172e, /// 0x518
			 0x3887eaca, /// 0x51c
			 0xb6606f3b, /// 0x520
			 0x159fe57c, /// 0x524
			 0x6894f6a5, /// 0x528
			 0x0fbc2713, /// 0x52c
			 0x62803f29, /// 0x530
			 0xe6af1c63, /// 0x534
			 0xa00ce9da, /// 0x538
			 0xcf7ab2d0, /// 0x53c
			 0x55e5ee6e, /// 0x540
			 0x5a0ea778, /// 0x544
			 0x4f1a8ab4, /// 0x548
			 0xc33b1df0, /// 0x54c
			 0x65a88718, /// 0x550
			 0xf42aa55f, /// 0x554
			 0x947a8f6a, /// 0x558
			 0xc0770834, /// 0x55c
			 0xf99482cf, /// 0x560
			 0xe50e8b2e, /// 0x564
			 0x3d3ef193, /// 0x568
			 0x14d45f08, /// 0x56c
			 0xe91100b1, /// 0x570
			 0x54e6dfd2, /// 0x574
			 0x220001fa, /// 0x578
			 0x1e3b5191, /// 0x57c
			 0x6fd53491, /// 0x580
			 0xc3d3f72a, /// 0x584
			 0x1501e62e, /// 0x588
			 0x0b0b1e4b, /// 0x58c
			 0x7a224e5c, /// 0x590
			 0x0a8b2b0c, /// 0x594
			 0x091ee987, /// 0x598
			 0xdfa23e11, /// 0x59c
			 0x8594ef48, /// 0x5a0
			 0x5423e273, /// 0x5a4
			 0x19dfc925, /// 0x5a8
			 0x3fa693d1, /// 0x5ac
			 0xbf408e1e, /// 0x5b0
			 0xf607df03, /// 0x5b4
			 0x4af7bb1e, /// 0x5b8
			 0xc87d62d3, /// 0x5bc
			 0x4a13ad51, /// 0x5c0
			 0xd654adda, /// 0x5c4
			 0x7561dea5, /// 0x5c8
			 0xb7485de2, /// 0x5cc
			 0x9c262729, /// 0x5d0
			 0xb02a1dbb, /// 0x5d4
			 0xf12dc2ec, /// 0x5d8
			 0xeb64a744, /// 0x5dc
			 0x18399bd2, /// 0x5e0
			 0xce68d616, /// 0x5e4
			 0x43d03bd7, /// 0x5e8
			 0xdf460e54, /// 0x5ec
			 0x448686c8, /// 0x5f0
			 0x928e047a, /// 0x5f4
			 0x20ec0bb8, /// 0x5f8
			 0xd13d7544, /// 0x5fc
			 0xc9c48479, /// 0x600
			 0xd55b2b9f, /// 0x604
			 0xec90473b, /// 0x608
			 0x6b1da162, /// 0x60c
			 0x781ab729, /// 0x610
			 0x04f1bc91, /// 0x614
			 0x9ec0e950, /// 0x618
			 0xe433140e, /// 0x61c
			 0xc55fc702, /// 0x620
			 0x047bb23b, /// 0x624
			 0x13de827e, /// 0x628
			 0xaea274ff, /// 0x62c
			 0xdcd9efae, /// 0x630
			 0xa9aa89dc, /// 0x634
			 0x22d98dc2, /// 0x638
			 0x50129c43, /// 0x63c
			 0x6542ccb9, /// 0x640
			 0x694b487b, /// 0x644
			 0xb69d476d, /// 0x648
			 0xeaac875a, /// 0x64c
			 0x8ec121c8, /// 0x650
			 0xa1f78985, /// 0x654
			 0x361f69cb, /// 0x658
			 0x49728a6f, /// 0x65c
			 0x0007a4d8, /// 0x660
			 0xaa3b73d0, /// 0x664
			 0xd6b6acbd, /// 0x668
			 0x5c252fa2, /// 0x66c
			 0x4c4c5575, /// 0x670
			 0xbe89a1b4, /// 0x674
			 0xcdb05239, /// 0x678
			 0x58ceffa8, /// 0x67c
			 0x63f52be5, /// 0x680
			 0xbd04c8d7, /// 0x684
			 0x0b9ac7aa, /// 0x688
			 0x09d3a229, /// 0x68c
			 0x25cfe6ec, /// 0x690
			 0x3baedbcd, /// 0x694
			 0x44821814, /// 0x698
			 0x91a061c6, /// 0x69c
			 0xf3720c66, /// 0x6a0
			 0x54647516, /// 0x6a4
			 0xb47199c7, /// 0x6a8
			 0xff959f4f, /// 0x6ac
			 0xf7b9a9ba, /// 0x6b0
			 0x71c8e739, /// 0x6b4
			 0x01748c33, /// 0x6b8
			 0x38799690, /// 0x6bc
			 0x619457d3, /// 0x6c0
			 0x55ba6ac2, /// 0x6c4
			 0xff497b90, /// 0x6c8
			 0x32f69cfc, /// 0x6cc
			 0xa5cccb7e, /// 0x6d0
			 0x5ad5becf, /// 0x6d4
			 0x33ce4901, /// 0x6d8
			 0xcc478530, /// 0x6dc
			 0xe12f430c, /// 0x6e0
			 0xc60045eb, /// 0x6e4
			 0x4a15d988, /// 0x6e8
			 0x33af3275, /// 0x6ec
			 0x1b99a40c, /// 0x6f0
			 0x584ca58b, /// 0x6f4
			 0xb6f35f4f, /// 0x6f8
			 0xb7297fb2, /// 0x6fc
			 0x5a9c74a8, /// 0x700
			 0xac967926, /// 0x704
			 0x37e0d638, /// 0x708
			 0xbce954d5, /// 0x70c
			 0x27e980a0, /// 0x710
			 0x4d2721b8, /// 0x714
			 0xf854f92d, /// 0x718
			 0x2870c06c, /// 0x71c
			 0x854a3556, /// 0x720
			 0x709f00a5, /// 0x724
			 0xd0a0e2ee, /// 0x728
			 0x396b5f5b, /// 0x72c
			 0xe1f1353b, /// 0x730
			 0xe03b4fcd, /// 0x734
			 0xf2b552aa, /// 0x738
			 0x592035f7, /// 0x73c
			 0x4f5de791, /// 0x740
			 0xc720595b, /// 0x744
			 0xedf28afd, /// 0x748
			 0xd7ed826c, /// 0x74c
			 0x3d1b642f, /// 0x750
			 0x707a702d, /// 0x754
			 0xdcaac0ac, /// 0x758
			 0x8d3228cd, /// 0x75c
			 0x8cea258d, /// 0x760
			 0xa04cb0e8, /// 0x764
			 0x484f735a, /// 0x768
			 0x5c50ec82, /// 0x76c
			 0x03d8c500, /// 0x770
			 0x8d0f6fe8, /// 0x774
			 0x0219048e, /// 0x778
			 0xe64fad88, /// 0x77c
			 0xab1a6e05, /// 0x780
			 0xb1d61849, /// 0x784
			 0x30f25d51, /// 0x788
			 0x82f2c9c4, /// 0x78c
			 0xfa6f696b, /// 0x790
			 0x3d7e17e0, /// 0x794
			 0x2c97899e, /// 0x798
			 0x96d23c74, /// 0x79c
			 0xa1269bc5, /// 0x7a0
			 0xa7e5ef92, /// 0x7a4
			 0x4e1de137, /// 0x7a8
			 0x649c8dd9, /// 0x7ac
			 0xe0e0692d, /// 0x7b0
			 0xa5f2332e, /// 0x7b4
			 0x7495a102, /// 0x7b8
			 0xfa2ecd01, /// 0x7bc
			 0xc0fc89c2, /// 0x7c0
			 0x6964dd5e, /// 0x7c4
			 0xb66f980c, /// 0x7c8
			 0x7e4e5214, /// 0x7cc
			 0xb03aae63, /// 0x7d0
			 0x33c1a28e, /// 0x7d4
			 0xb66f99dd, /// 0x7d8
			 0xde30662e, /// 0x7dc
			 0x42bcd226, /// 0x7e0
			 0x92575d61, /// 0x7e4
			 0xb5797c65, /// 0x7e8
			 0xc4cd2e4e, /// 0x7ec
			 0xf49af6b5, /// 0x7f0
			 0x9f906cd4, /// 0x7f4
			 0x5b58cd74, /// 0x7f8
			 0xa6e78515, /// 0x7fc
			 0xe7076bd5, /// 0x800
			 0x1914775f, /// 0x804
			 0x2b3d4ab5, /// 0x808
			 0x358c371a, /// 0x80c
			 0xfd4be24f, /// 0x810
			 0xa2e7dd5a, /// 0x814
			 0x0f2d0c87, /// 0x818
			 0x8e9787b0, /// 0x81c
			 0xfd3c1c63, /// 0x820
			 0xaea91934, /// 0x824
			 0xa5062b7c, /// 0x828
			 0xd64674d1, /// 0x82c
			 0x291b9836, /// 0x830
			 0x9d5ec97a, /// 0x834
			 0xe37bd0e0, /// 0x838
			 0x00ea87bb, /// 0x83c
			 0xb24f3eb4, /// 0x840
			 0xcc06b22b, /// 0x844
			 0x861a224f, /// 0x848
			 0xea67cb32, /// 0x84c
			 0xb49cc215, /// 0x850
			 0xa0ceaa44, /// 0x854
			 0x77bcb64a, /// 0x858
			 0x799e42bb, /// 0x85c
			 0x2b69a672, /// 0x860
			 0x5c22269d, /// 0x864
			 0x95131e97, /// 0x868
			 0x763aa67d, /// 0x86c
			 0xff99a908, /// 0x870
			 0xd8b823c4, /// 0x874
			 0xa1882898, /// 0x878
			 0x1d47eff3, /// 0x87c
			 0x5eb9863e, /// 0x880
			 0x29c7c32f, /// 0x884
			 0x06e0cffd, /// 0x888
			 0x60b2fac7, /// 0x88c
			 0x34078393, /// 0x890
			 0x90ea95f1, /// 0x894
			 0x3db587f6, /// 0x898
			 0x8b783474, /// 0x89c
			 0x956021c2, /// 0x8a0
			 0xeba081c6, /// 0x8a4
			 0xb7f9ae71, /// 0x8a8
			 0x620301b6, /// 0x8ac
			 0x7101065a, /// 0x8b0
			 0x290e35a5, /// 0x8b4
			 0xe46d2309, /// 0x8b8
			 0xb9c71bd0, /// 0x8bc
			 0xb84bd804, /// 0x8c0
			 0x7b8da43e, /// 0x8c4
			 0x23f7f302, /// 0x8c8
			 0xe0a0d18f, /// 0x8cc
			 0x28e7226e, /// 0x8d0
			 0x74bda472, /// 0x8d4
			 0x8c432e98, /// 0x8d8
			 0x840eafe6, /// 0x8dc
			 0x32425a20, /// 0x8e0
			 0x30572f63, /// 0x8e4
			 0xd67c7347, /// 0x8e8
			 0xf0127edd, /// 0x8ec
			 0x9def664d, /// 0x8f0
			 0xb3e90254, /// 0x8f4
			 0xb8dd6d00, /// 0x8f8
			 0x9490b808, /// 0x8fc
			 0x1de9a7a4, /// 0x900
			 0xee59f97a, /// 0x904
			 0x2c8049b4, /// 0x908
			 0x371e840c, /// 0x90c
			 0xdc811a80, /// 0x910
			 0x77ea2992, /// 0x914
			 0xc19149e1, /// 0x918
			 0x2b7b1e5d, /// 0x91c
			 0x70fe4bbb, /// 0x920
			 0x90ff0ddc, /// 0x924
			 0xca47dfcd, /// 0x928
			 0x53d94d18, /// 0x92c
			 0xe2bab430, /// 0x930
			 0x25cf5a2b, /// 0x934
			 0x21ab70ba, /// 0x938
			 0x916fee1d, /// 0x93c
			 0xed3d1b1e, /// 0x940
			 0xd5f029a7, /// 0x944
			 0x95e8ac07, /// 0x948
			 0x3fb4b2ef, /// 0x94c
			 0x8b6f1983, /// 0x950
			 0x16ccb5d4, /// 0x954
			 0x9967c649, /// 0x958
			 0x3929b1f3, /// 0x95c
			 0x94b6400c, /// 0x960
			 0x8c447726, /// 0x964
			 0x88b14867, /// 0x968
			 0x74c61922, /// 0x96c
			 0xb57a3472, /// 0x970
			 0xcd0bc599, /// 0x974
			 0x05dec00d, /// 0x978
			 0x2241886e, /// 0x97c
			 0x8ccb01e5, /// 0x980
			 0x74baea94, /// 0x984
			 0x395eafaa, /// 0x988
			 0x8827b9ed, /// 0x98c
			 0xb054db29, /// 0x990
			 0xf43b2b53, /// 0x994
			 0x23e2e356, /// 0x998
			 0x611ee3a9, /// 0x99c
			 0xb9a1784a, /// 0x9a0
			 0x119028fc, /// 0x9a4
			 0xeb8b1baa, /// 0x9a8
			 0xf7302c7b, /// 0x9ac
			 0x5acdbb07, /// 0x9b0
			 0xdf0ba2c0, /// 0x9b4
			 0xdd96fc49, /// 0x9b8
			 0x606e7b4a, /// 0x9bc
			 0x5c465e4d, /// 0x9c0
			 0x83bb81b3, /// 0x9c4
			 0x97b27627, /// 0x9c8
			 0xf93a1ada, /// 0x9cc
			 0xffb19ef3, /// 0x9d0
			 0xbfaad8c6, /// 0x9d4
			 0xba2f4a75, /// 0x9d8
			 0x9689e1b9, /// 0x9dc
			 0x978f6f6d, /// 0x9e0
			 0xc145205b, /// 0x9e4
			 0x4fa948da, /// 0x9e8
			 0x97c9e965, /// 0x9ec
			 0x1ae1531c, /// 0x9f0
			 0xae47dd3b, /// 0x9f4
			 0xd3ab248e, /// 0x9f8
			 0x53080cba, /// 0x9fc
			 0xab192c70, /// 0xa00
			 0x3cf64ecd, /// 0xa04
			 0x15f221d1, /// 0xa08
			 0xd4f77cbf, /// 0xa0c
			 0xcd8ca5a7, /// 0xa10
			 0x93bd3aa6, /// 0xa14
			 0x502f883a, /// 0xa18
			 0x75b2a242, /// 0xa1c
			 0xf24dd924, /// 0xa20
			 0x6295cf9c, /// 0xa24
			 0x097b0e80, /// 0xa28
			 0x47191fde, /// 0xa2c
			 0x7b8db98a, /// 0xa30
			 0x6830712f, /// 0xa34
			 0xd1d7007a, /// 0xa38
			 0x8e48a062, /// 0xa3c
			 0x3cb785e6, /// 0xa40
			 0xc325e87c, /// 0xa44
			 0x37788c84, /// 0xa48
			 0x4ddb08db, /// 0xa4c
			 0xece46517, /// 0xa50
			 0x0f88d133, /// 0xa54
			 0xb0432d9b, /// 0xa58
			 0x2203d5c0, /// 0xa5c
			 0x8252e1d0, /// 0xa60
			 0x01cd5d7f, /// 0xa64
			 0xf31a29d4, /// 0xa68
			 0x89a34c72, /// 0xa6c
			 0x453c8af3, /// 0xa70
			 0x39d86dd9, /// 0xa74
			 0x90212386, /// 0xa78
			 0xa87ca11a, /// 0xa7c
			 0x53097a1a, /// 0xa80
			 0x597c896f, /// 0xa84
			 0xc81d84be, /// 0xa88
			 0x8fa87ae4, /// 0xa8c
			 0x5d76cd17, /// 0xa90
			 0xb91d4a8c, /// 0xa94
			 0x4cb4eb82, /// 0xa98
			 0x4988244a, /// 0xa9c
			 0xbdec02a1, /// 0xaa0
			 0xad71e8bd, /// 0xaa4
			 0x160cb1ab, /// 0xaa8
			 0xbfa6d597, /// 0xaac
			 0x3b981ce2, /// 0xab0
			 0x3fd8d448, /// 0xab4
			 0x3ea7e268, /// 0xab8
			 0x0fdfa973, /// 0xabc
			 0xa47861d0, /// 0xac0
			 0x785c8692, /// 0xac4
			 0x6bee4867, /// 0xac8
			 0xfad1640d, /// 0xacc
			 0x53ed7244, /// 0xad0
			 0x76c97208, /// 0xad4
			 0xef326001, /// 0xad8
			 0xb39bc74b, /// 0xadc
			 0xd3efef2c, /// 0xae0
			 0x709c6ed1, /// 0xae4
			 0xb2d4735f, /// 0xae8
			 0x053e49a1, /// 0xaec
			 0x585028d9, /// 0xaf0
			 0x7c6fdf46, /// 0xaf4
			 0x89bc30bb, /// 0xaf8
			 0xbd1a9f7b, /// 0xafc
			 0x4e0c4c26, /// 0xb00
			 0x6da87d6e, /// 0xb04
			 0x58e4a52c, /// 0xb08
			 0x8771c9a3, /// 0xb0c
			 0x4c8295e5, /// 0xb10
			 0xa7badb73, /// 0xb14
			 0x6493bbec, /// 0xb18
			 0x3e8a649d, /// 0xb1c
			 0x4eab0cec, /// 0xb20
			 0x8bc65e61, /// 0xb24
			 0x34a707c2, /// 0xb28
			 0x763191b4, /// 0xb2c
			 0x87f28446, /// 0xb30
			 0xecff1f2c, /// 0xb34
			 0x9067d1d7, /// 0xb38
			 0xaa8df32c, /// 0xb3c
			 0x9904bc89, /// 0xb40
			 0xff192fb0, /// 0xb44
			 0x91e55386, /// 0xb48
			 0x6420c53a, /// 0xb4c
			 0x141eafc9, /// 0xb50
			 0x67117de6, /// 0xb54
			 0x87c80881, /// 0xb58
			 0x1b51fa28, /// 0xb5c
			 0x6e3896df, /// 0xb60
			 0x9fbc8731, /// 0xb64
			 0x79ad4eb8, /// 0xb68
			 0x1337aece, /// 0xb6c
			 0x7f9ad83c, /// 0xb70
			 0x3cfc0074, /// 0xb74
			 0xfcc4c21a, /// 0xb78
			 0x5baf9164, /// 0xb7c
			 0x56d9a499, /// 0xb80
			 0x39887de1, /// 0xb84
			 0x605d99f9, /// 0xb88
			 0xa567b80a, /// 0xb8c
			 0x4dcc9e78, /// 0xb90
			 0x46e8487c, /// 0xb94
			 0xceaa3168, /// 0xb98
			 0xd7504d6e, /// 0xb9c
			 0x45e67ce7, /// 0xba0
			 0x82ad2cae, /// 0xba4
			 0x7e3eb20f, /// 0xba8
			 0x551a7023, /// 0xbac
			 0x0ce23f9a, /// 0xbb0
			 0xed025842, /// 0xbb4
			 0x99e0f5a8, /// 0xbb8
			 0x1550680d, /// 0xbbc
			 0x9c596859, /// 0xbc0
			 0xf0af189c, /// 0xbc4
			 0x62f03e45, /// 0xbc8
			 0x6d5a2398, /// 0xbcc
			 0x1d8dfe7e, /// 0xbd0
			 0xce2a0ce9, /// 0xbd4
			 0x8f302bd9, /// 0xbd8
			 0x1cf31e65, /// 0xbdc
			 0xed13b632, /// 0xbe0
			 0x518481ef, /// 0xbe4
			 0xb40e8289, /// 0xbe8
			 0xa52e4210, /// 0xbec
			 0x1bbf116b, /// 0xbf0
			 0x6b57ea69, /// 0xbf4
			 0x9a218b50, /// 0xbf8
			 0x5ee509a0, /// 0xbfc
			 0xf88e5d27, /// 0xc00
			 0xef725960, /// 0xc04
			 0xbeb2d3a5, /// 0xc08
			 0x6059b640, /// 0xc0c
			 0x95fbe770, /// 0xc10
			 0x436c2f70, /// 0xc14
			 0x8d11951d, /// 0xc18
			 0x7550fb62, /// 0xc1c
			 0xf5358ead, /// 0xc20
			 0xe164a145, /// 0xc24
			 0x08fe3a65, /// 0xc28
			 0xff5181fd, /// 0xc2c
			 0xf3352809, /// 0xc30
			 0x9415d3dd, /// 0xc34
			 0x844e30b0, /// 0xc38
			 0x8c25c8cf, /// 0xc3c
			 0x0a9508fd, /// 0xc40
			 0x1fd18b76, /// 0xc44
			 0xe9ba7724, /// 0xc48
			 0x92e9a276, /// 0xc4c
			 0x490f3b09, /// 0xc50
			 0xbb1e20bd, /// 0xc54
			 0x1d657cbd, /// 0xc58
			 0xd314757c, /// 0xc5c
			 0xdcdffe67, /// 0xc60
			 0x793637a8, /// 0xc64
			 0x82350b9c, /// 0xc68
			 0x12afc675, /// 0xc6c
			 0x3c746192, /// 0xc70
			 0x693c75e4, /// 0xc74
			 0xcaa59b9b, /// 0xc78
			 0xae510e0c, /// 0xc7c
			 0xfb2fd16c, /// 0xc80
			 0x696f6a20, /// 0xc84
			 0x55ae55ba, /// 0xc88
			 0x07e581ba, /// 0xc8c
			 0xb7220a69, /// 0xc90
			 0x9efa0c4c, /// 0xc94
			 0x8793e163, /// 0xc98
			 0x86a36a53, /// 0xc9c
			 0x5e31abcb, /// 0xca0
			 0x88616585, /// 0xca4
			 0x8494d508, /// 0xca8
			 0xa2dd579b, /// 0xcac
			 0x5df44b0f, /// 0xcb0
			 0x3ccc5ce1, /// 0xcb4
			 0x203d6f5e, /// 0xcb8
			 0xb582acb7, /// 0xcbc
			 0xb4c23c73, /// 0xcc0
			 0xf2a1a1b9, /// 0xcc4
			 0xc9976235, /// 0xcc8
			 0x2f99a7e6, /// 0xccc
			 0xe19c7acb, /// 0xcd0
			 0x21ce1c53, /// 0xcd4
			 0x1b9f4362, /// 0xcd8
			 0xc5464528, /// 0xcdc
			 0x8ffed0d2, /// 0xce0
			 0xabb5025a, /// 0xce4
			 0x1a93bfa9, /// 0xce8
			 0x5a2f803b, /// 0xcec
			 0x936554e8, /// 0xcf0
			 0xdc196af2, /// 0xcf4
			 0x70055fbd, /// 0xcf8
			 0xa8de670b, /// 0xcfc
			 0xfbb87cdc, /// 0xd00
			 0xa9dafaa6, /// 0xd04
			 0x83173381, /// 0xd08
			 0xb3c44caf, /// 0xd0c
			 0xef74a676, /// 0xd10
			 0x277f7006, /// 0xd14
			 0xa44073d9, /// 0xd18
			 0x9caec982, /// 0xd1c
			 0x76b9a506, /// 0xd20
			 0x157c6270, /// 0xd24
			 0x3adb9f3d, /// 0xd28
			 0xab22e350, /// 0xd2c
			 0x509a226e, /// 0xd30
			 0x59f8c660, /// 0xd34
			 0xd775c9b8, /// 0xd38
			 0x495e23d7, /// 0xd3c
			 0x054b1783, /// 0xd40
			 0x0baa0e09, /// 0xd44
			 0x5d479f05, /// 0xd48
			 0x9e795aa4, /// 0xd4c
			 0xc5021c8f, /// 0xd50
			 0x2430d266, /// 0xd54
			 0x5684d4e7, /// 0xd58
			 0x40784951, /// 0xd5c
			 0x3986df01, /// 0xd60
			 0xec6e67c6, /// 0xd64
			 0x80e010e0, /// 0xd68
			 0xdb7ac359, /// 0xd6c
			 0x92914810, /// 0xd70
			 0xacab81fd, /// 0xd74
			 0x8f96559e, /// 0xd78
			 0xb7578861, /// 0xd7c
			 0x32074cbf, /// 0xd80
			 0xa9e79f24, /// 0xd84
			 0x3c3e12cd, /// 0xd88
			 0xbf56c5fd, /// 0xd8c
			 0xff2d68c3, /// 0xd90
			 0xa3eb9532, /// 0xd94
			 0x38552eeb, /// 0xd98
			 0xc382c088, /// 0xd9c
			 0xb3d0dee5, /// 0xda0
			 0x6f67e825, /// 0xda4
			 0xd9b6d482, /// 0xda8
			 0x679edb86, /// 0xdac
			 0x195c742b, /// 0xdb0
			 0x876871fb, /// 0xdb4
			 0x23751b48, /// 0xdb8
			 0x83507bcf, /// 0xdbc
			 0x9fd41ed5, /// 0xdc0
			 0x723c3cff, /// 0xdc4
			 0x4ea37e49, /// 0xdc8
			 0x762878dd, /// 0xdcc
			 0x15822853, /// 0xdd0
			 0x31a0ced8, /// 0xdd4
			 0x9cb7a997, /// 0xdd8
			 0x1dd1a272, /// 0xddc
			 0x4a48643d, /// 0xde0
			 0x1cc49322, /// 0xde4
			 0xb5208706, /// 0xde8
			 0x82c8f0ab, /// 0xdec
			 0xf03abb57, /// 0xdf0
			 0xb2d98642, /// 0xdf4
			 0x52633a55, /// 0xdf8
			 0x1ff0b6bc, /// 0xdfc
			 0x0b41ca80, /// 0xe00
			 0x78860151, /// 0xe04
			 0x23dc471e, /// 0xe08
			 0x16113bb0, /// 0xe0c
			 0xfa23edd2, /// 0xe10
			 0x8fd35df6, /// 0xe14
			 0x23b74253, /// 0xe18
			 0x1b4d6121, /// 0xe1c
			 0xd6a80974, /// 0xe20
			 0x3e8aabc2, /// 0xe24
			 0xad063cc0, /// 0xe28
			 0x8c03ef10, /// 0xe2c
			 0x96bc6ffe, /// 0xe30
			 0x157a8a08, /// 0xe34
			 0xe66b43bb, /// 0xe38
			 0xf873f26a, /// 0xe3c
			 0x53c32f26, /// 0xe40
			 0x8d1eba25, /// 0xe44
			 0xd035d56f, /// 0xe48
			 0xf032e51e, /// 0xe4c
			 0x6b1682ac, /// 0xe50
			 0xfe60d9ff, /// 0xe54
			 0x13da2cc8, /// 0xe58
			 0x7ff7a752, /// 0xe5c
			 0xddfca5aa, /// 0xe60
			 0x6111a659, /// 0xe64
			 0xa451233b, /// 0xe68
			 0xc6d972f9, /// 0xe6c
			 0x2d6b0b5d, /// 0xe70
			 0x592dda94, /// 0xe74
			 0xd4bbce70, /// 0xe78
			 0x728217b4, /// 0xe7c
			 0xb743b383, /// 0xe80
			 0x913c4b5e, /// 0xe84
			 0xe3742bd8, /// 0xe88
			 0x4faf362c, /// 0xe8c
			 0x47472125, /// 0xe90
			 0x820cab45, /// 0xe94
			 0x38d4f262, /// 0xe98
			 0x9b4033f3, /// 0xe9c
			 0x38d584bf, /// 0xea0
			 0x27530ec2, /// 0xea4
			 0xd3b1581d, /// 0xea8
			 0xf64f6646, /// 0xeac
			 0xa1756a06, /// 0xeb0
			 0x377fe071, /// 0xeb4
			 0x5e3cde0d, /// 0xeb8
			 0xd30b992a, /// 0xebc
			 0x3c0a7912, /// 0xec0
			 0x25e0608c, /// 0xec4
			 0xb55d0997, /// 0xec8
			 0x2d1c1a08, /// 0xecc
			 0xa603c41c, /// 0xed0
			 0xcfd53404, /// 0xed4
			 0x616a8bc9, /// 0xed8
			 0xf86dec28, /// 0xedc
			 0x2a8c9a77, /// 0xee0
			 0xc5a469c4, /// 0xee4
			 0x6acb92ee, /// 0xee8
			 0x79739507, /// 0xeec
			 0xf9fee842, /// 0xef0
			 0xd08dbd6a, /// 0xef4
			 0x6adc10d6, /// 0xef8
			 0x4d5eedb4, /// 0xefc
			 0x9acc7087, /// 0xf00
			 0x4d404fd5, /// 0xf04
			 0x7e2376e3, /// 0xf08
			 0xdd13231e, /// 0xf0c
			 0x510c7047, /// 0xf10
			 0xa4c7929b, /// 0xf14
			 0x848fd1c6, /// 0xf18
			 0xfaa3164d, /// 0xf1c
			 0x6b48dc03, /// 0xf20
			 0xe246ef9d, /// 0xf24
			 0x615ceb79, /// 0xf28
			 0x3533ce28, /// 0xf2c
			 0x3ae9675e, /// 0xf30
			 0x53dad988, /// 0xf34
			 0x45096801, /// 0xf38
			 0xeb3ae1d5, /// 0xf3c
			 0xf82e5c71, /// 0xf40
			 0x9e5fd68c, /// 0xf44
			 0x7f355cee, /// 0xf48
			 0xdc528ca4, /// 0xf4c
			 0xd20d013e, /// 0xf50
			 0x5d5bb56c, /// 0xf54
			 0xac989959, /// 0xf58
			 0x788bdc62, /// 0xf5c
			 0x4a790b4d, /// 0xf60
			 0xdc42cc9c, /// 0xf64
			 0x67c638b4, /// 0xf68
			 0xb7641ba1, /// 0xf6c
			 0x4065c840, /// 0xf70
			 0x10fcf787, /// 0xf74
			 0xc6e753e3, /// 0xf78
			 0x1e9a343b, /// 0xf7c
			 0x584de368, /// 0xf80
			 0xd11e6149, /// 0xf84
			 0x76578b28, /// 0xf88
			 0x1307649b, /// 0xf8c
			 0x08451c2c, /// 0xf90
			 0xc8fde09a, /// 0xf94
			 0x87a94f8b, /// 0xf98
			 0xb6603fd9, /// 0xf9c
			 0x00a1f608, /// 0xfa0
			 0x38361cd1, /// 0xfa4
			 0xd66f0a1d, /// 0xfa8
			 0x47a87be5, /// 0xfac
			 0xedc0f693, /// 0xfb0
			 0x25da0f77, /// 0xfb4
			 0x0773a4f9, /// 0xfb8
			 0x460c3ffe, /// 0xfbc
			 0xf22ff35c, /// 0xfc0
			 0xc85c0442, /// 0xfc4
			 0x46aa9568, /// 0xfc8
			 0xd397c1e1, /// 0xfcc
			 0xb87af1ee, /// 0xfd0
			 0xed6e7dbb, /// 0xfd4
			 0xb4a36993, /// 0xfd8
			 0x89a8a397, /// 0xfdc
			 0xb604e607, /// 0xfe0
			 0x1dff2c69, /// 0xfe4
			 0xeb10d2aa, /// 0xfe8
			 0xc6502fbe, /// 0xfec
			 0x33d1da80, /// 0xff0
			 0x8c6c10ff, /// 0xff4
			 0x242b6435, /// 0xff8
			 0x3bb3cb84 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00000
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00004
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00008
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0000c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00010
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00014
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00018
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00020
			 0x80000000,                                                  // -zero                                       /// 00024
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00028
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00030
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00034
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00038
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0003c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00040
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00044
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00050
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00054
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00058
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0005c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00068
			 0x0c600000,                                                  // Leading 1s:                                 /// 0006c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00070
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00074
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0007c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0008c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00098
			 0xffc00001,                                                  // -signaling NaN                              /// 0009c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00104
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0010c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00110
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00114
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00118
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00120
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00128
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0012c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00130
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00134
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00138
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00140
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00148
			 0x7fc00001,                                                  // signaling NaN                               /// 0014c
			 0x4b000000,                                                  // 8388608.0                                   /// 00150
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00158
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00160
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00168
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0016c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00174
			 0x33333333,                                                  // 4 random values                             /// 00178
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00180
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00184
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00188
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0018c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00190
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0019c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b0
			 0x7fc00001,                                                  // signaling NaN                               /// 001b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 001bc
			 0x00000000,                                                  // zero                                        /// 001c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001dc
			 0x55555555,                                                  // 4 random values                             /// 001e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00200
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00204
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00208
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0020c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00210
			 0x0e000001,                                                  // Trailing 1s:                                /// 00214
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00218
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0021c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00220
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00224
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00228
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0022c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00234
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0023c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00240
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00250
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00254
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0025c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00260
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00264
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00000000,                                                  // zero                                        /// 0026c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00270
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00274
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00278
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00288
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0028c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00294
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00298
			 0x0e000001,                                                  // Trailing 1s:                                /// 0029c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00300
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00308
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0030c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00310
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00318
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0031c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00320
			 0xbf028f5c,                                                  // -0.51                                       /// 00324
			 0x7f800000,                                                  // inf                                         /// 00328
			 0xbf028f5c,                                                  // -0.51                                       /// 0032c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00334
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00338
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0033c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00340
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00350
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00354
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00358
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0035c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00368
			 0x7f800000,                                                  // inf                                         /// 0036c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00374
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00378
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0037c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00380
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00384
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00388
			 0xcb000000,                                                  // -8388608.0                                  /// 0038c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00394
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003ac
			 0xbf028f5c,                                                  // -0.51                                       /// 003b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x7f800000,                                                  // inf                                         /// 003bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00400
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00404
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0040c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00410
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00418
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0041c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x55555555,                                                  // 4 random values                             /// 00424
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00430
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00434
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00438
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0043c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00440
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00448
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0044c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00450
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x33333333,                                                  // 4 random values                             /// 00458
			 0x80000000,                                                  // -zero                                       /// 0045c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00460
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00464
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00468
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0046c
			 0x7fc00001,                                                  // signaling NaN                               /// 00470
			 0x0c780000,                                                  // Leading 1s:                                 /// 00474
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0047c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00480
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00488
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0048c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00490
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00498
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004b0
			 0xff800000,                                                  // -inf                                        /// 004b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004f4
			 0x80000000,                                                  // -zero                                       /// 004f8
			 0xffc00001,                                                  // -signaling NaN                              /// 004fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00500
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00504
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00510
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00514
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00518
			 0x7f800000,                                                  // inf                                         /// 0051c
			 0xcb000000,                                                  // -8388608.0                                  /// 00520
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x0c700000,                                                  // Leading 1s:                                 /// 00528
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00538
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0053c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00540
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00544
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00548
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x00000000,                                                  // zero                                        /// 00550
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0x80011111,                                                  // -9.7958E-41                                 /// 00558
			 0x80011111,                                                  // -9.7958E-41                                 /// 0055c
			 0x7fc00001,                                                  // signaling NaN                               /// 00560
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0056c
			 0x00000000,                                                  // zero                                        /// 00570
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x4b000000,                                                  // 8388608.0                                   /// 00578
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0057c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00580
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00588
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0058c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00590
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00598
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005ac
			 0x00000000,                                                  // zero                                        /// 005b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 005d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00600
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0060c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00610
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00618
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0061c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00620
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00624
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x80000000,                                                  // -zero                                       /// 00630
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00634
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0063c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00640
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00644
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00648
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0064c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00650
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0xcb000000,                                                  // -8388608.0                                  /// 00658
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00660
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00664
			 0x0e000001,                                                  // Trailing 1s:                                /// 00668
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0066c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00674
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00678
			 0x0c780000,                                                  // Leading 1s:                                 /// 0067c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00680
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00688
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00690
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00694
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0069c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00700
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00708
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0070c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00710
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00718
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0071c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00720
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00724
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00728
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0072c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00730
			 0x0e000003,                                                  // Trailing 1s:                                /// 00734
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00740
			 0x0c700000,                                                  // Leading 1s:                                 /// 00744
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00748
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0074c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00750
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00754
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00758
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0075c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00760
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00764
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00768
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0076c
			 0x55555555,                                                  // 4 random values                             /// 00770
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0e000003,                                                  // Trailing 1s:                                /// 00778
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0077c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00780
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00790
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00794
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0079c
			 0x0c600000,                                                  // Leading 1s:                                 /// 007a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00800
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00804
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00808
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0080c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0e000007,                                                  // Trailing 1s:                                /// 00818
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0081c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00820
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00830
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00834
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00838
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00840
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00844
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0084c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00854
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x80011111,                                                  // -9.7958E-41                                 /// 0085c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00860
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00864
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00868
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0086c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00870
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00874
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00878
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00880
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00884
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00888
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0088c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00894
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0089c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 008b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008b8
			 0xff800000,                                                  // -inf                                        /// 008bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e8
			 0x00000000,                                                  // zero                                        /// 008ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00900
			 0xcb000000,                                                  // -8388608.0                                  /// 00904
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00908
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00914
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00920
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00924
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00928
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0092c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00930
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00934
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00938
			 0x0c600000,                                                  // Leading 1s:                                 /// 0093c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00944
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00948
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0094c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0c600000,                                                  // Leading 1s:                                 /// 00954
			 0x80011111,                                                  // -9.7958E-41                                 /// 00958
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00960
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00964
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00968
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00974
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00978
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0097c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00980
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00984
			 0x00000000,                                                  // zero                                        /// 00988
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0098c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00990
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00994
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00998
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0099c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009ac
			 0xbf028f5c,                                                  // -0.51                                       /// 009b0
			 0xbf028f5c,                                                  // -0.51                                       /// 009b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009cc
			 0x3f028f5c,                                                  // 0.51                                        /// 009d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x7f800000,                                                  // inf                                         /// 009ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a14
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a20
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a24
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a28
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a30
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a38
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a40
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a8c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aa0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aa4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ab4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ab8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ac0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ad4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ae0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ae8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x33333333,                                                  // 4 random values                             /// 00af4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00af8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b0c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b30
			 0x4b000000,                                                  // 8388608.0                                   /// 00b34
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b38
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b50
			 0xffc00001,                                                  // -signaling NaN                              /// 00b54
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b6c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ba0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bc4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bc8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bcc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bd0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bd8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bdc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00be0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00be8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bf4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bfc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c0c
			 0x33333333,                                                  // 4 random values                             /// 00c10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x7f800000,                                                  // inf                                         /// 00c28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c2c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0xcb000000,                                                  // -8388608.0                                  /// 00c34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c4c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c54
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c64
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c88
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ca0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ca4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ca8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cc0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cd0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cd4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cd8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cdc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ce0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ce8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cf4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cf8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cfc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d18
			 0x00000000,                                                  // zero                                        /// 00d1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d24
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x00000000,                                                  // zero                                        /// 00d60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d64
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d88
			 0x4b000000,                                                  // 8388608.0                                   /// 00d8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00da4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00da8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00db0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00db8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dbc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dc0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dc8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dcc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dd0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dd8
			 0x7f800000,                                                  // inf                                         /// 00ddc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0xcb000000,                                                  // -8388608.0                                  /// 00de4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00de8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00df4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00df8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dfc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e08
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e10
			 0xff800000,                                                  // -inf                                        /// 00e14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e28
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e34
			 0x80000000,                                                  // -zero                                       /// 00e38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e60
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e6c
			 0x33333333,                                                  // 4 random values                             /// 00e70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e90
			 0x4b000000,                                                  // 8388608.0                                   /// 00e94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x55555555,                                                  // 4 random values                             /// 00e9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ea4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ea8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00eb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ebc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ec0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ec4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ecc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ed0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ee0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ee4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ee8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ef4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ef8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00efc
			 0xffc00001,                                                  // -signaling NaN                              /// 00f00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0xffc00001,                                                  // -signaling NaN                              /// 00f08
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f24
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f2c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f4c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f58
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fa8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x7f800000,                                                  // inf                                         /// 00fb8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fc0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fd0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x4b000000,                                                  // 8388608.0                                   /// 00fdc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fe4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fe8
			 0x55555555,                                                  // 4 random values                             /// 00fec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ff0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ff8
			 0x00000002 // min subnorm + 1 ulp                           // SP +ve numbers                              /// last
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
			 0x000002d0,
			 0x00000160,
			 0x00000554,
			 0x00000654,
			 0x000003b0,
			 0x0000067c,
			 0x00000524,
			 0x000000d4,

			 /// vpu register f2
			 0x41880000,
			 0x40e00000,
			 0x41300000,
			 0x41900000,
			 0x41b00000,
			 0x42000000,
			 0x40a00000,
			 0x41800000,

			 /// vpu register f3
			 0x41a80000,
			 0x41e00000,
			 0x41d00000,
			 0x41800000,
			 0x41e80000,
			 0x40800000,
			 0x41880000,
			 0x41400000,

			 /// vpu register f4
			 0x41000000,
			 0x41800000,
			 0x41e80000,
			 0x41200000,
			 0x40c00000,
			 0x41a80000,
			 0x41980000,
			 0x41f80000,

			 /// vpu register f5
			 0x41c00000,
			 0x40a00000,
			 0x41100000,
			 0x40e00000,
			 0x41900000,
			 0x41000000,
			 0x40e00000,
			 0x41d80000,

			 /// vpu register f6
			 0x41700000,
			 0x41800000,
			 0x41600000,
			 0x40800000,
			 0x40800000,
			 0x41000000,
			 0x41c00000,
			 0x41e00000,

			 /// vpu register f7
			 0x40000000,
			 0x41980000,
			 0x41200000,
			 0x3f800000,
			 0x41c80000,
			 0x41600000,
			 0x41800000,
			 0x41880000,

			 /// vpu register f8
			 0x40400000,
			 0x41300000,
			 0x41a80000,
			 0x40000000,
			 0x40800000,
			 0x41f00000,
			 0x41d80000,
			 0x41e00000,

			 /// vpu register f9
			 0x41880000,
			 0x41900000,
			 0x41a80000,
			 0x41f00000,
			 0x41c80000,
			 0x41000000,
			 0x41f80000,
			 0x40c00000,

			 /// vpu register f10
			 0x41000000,
			 0x41d00000,
			 0x41000000,
			 0x41b80000,
			 0x42000000,
			 0x41700000,
			 0x40000000,
			 0x41a80000,

			 /// vpu register f11
			 0x41000000,
			 0x41980000,
			 0x41d00000,
			 0x41800000,
			 0x41b80000,
			 0x41400000,
			 0x41000000,
			 0x41300000,

			 /// vpu register f12
			 0x40e00000,
			 0x41c80000,
			 0x41000000,
			 0x42000000,
			 0x41800000,
			 0x41900000,
			 0x41b00000,
			 0x40000000,

			 /// vpu register f13
			 0x41e80000,
			 0x42000000,
			 0x41a80000,
			 0x41900000,
			 0x41e80000,
			 0x40000000,
			 0x41b80000,
			 0x41800000,

			 /// vpu register f14
			 0x41b80000,
			 0x3f800000,
			 0x41d00000,
			 0x42000000,
			 0x40400000,
			 0x41d80000,
			 0x41d80000,
			 0x41900000,

			 /// vpu register f15
			 0x41000000,
			 0x40e00000,
			 0x41d00000,
			 0x41700000,
			 0x41b80000,
			 0x41a00000,
			 0x41980000,
			 0x40c00000,

			 /// vpu register f16
			 0x41100000,
			 0x41880000,
			 0x40c00000,
			 0x41200000,
			 0x41300000,
			 0x41880000,
			 0x41400000,
			 0x40000000,

			 /// vpu register f17
			 0x41880000,
			 0x41c80000,
			 0x41100000,
			 0x41d00000,
			 0x40e00000,
			 0x41900000,
			 0x41100000,
			 0x40400000,

			 /// vpu register f18
			 0x40800000,
			 0x41800000,
			 0x41b00000,
			 0x41c00000,
			 0x41f80000,
			 0x41900000,
			 0x41600000,
			 0x41d80000,

			 /// vpu register f19
			 0x41800000,
			 0x40000000,
			 0x41d80000,
			 0x41600000,
			 0x40a00000,
			 0x41300000,
			 0x41f80000,
			 0x40e00000,

			 /// vpu register f20
			 0x41000000,
			 0x41d00000,
			 0x41900000,
			 0x41880000,
			 0x3f800000,
			 0x41500000,
			 0x41c00000,
			 0x41700000,

			 /// vpu register f21
			 0x41400000,
			 0x41980000,
			 0x41d00000,
			 0x41500000,
			 0x41a80000,
			 0x41980000,
			 0x41f00000,
			 0x41c80000,

			 /// vpu register f22
			 0x41500000,
			 0x3f800000,
			 0x41800000,
			 0x41d00000,
			 0x41000000,
			 0x41800000,
			 0x41e80000,
			 0x40a00000,

			 /// vpu register f23
			 0x41100000,
			 0x40a00000,
			 0x40e00000,
			 0x42000000,
			 0x41a00000,
			 0x41800000,
			 0x41e00000,
			 0x41100000,

			 /// vpu register f24
			 0x41f80000,
			 0x41000000,
			 0x41a80000,
			 0x41600000,
			 0x41900000,
			 0x41700000,
			 0x41800000,
			 0x41500000,

			 /// vpu register f25
			 0x40000000,
			 0x41900000,
			 0x40800000,
			 0x40c00000,
			 0x41800000,
			 0x3f800000,
			 0x40800000,
			 0x41600000,

			 /// vpu register f26
			 0x41400000,
			 0x41c80000,
			 0x42000000,
			 0x41880000,
			 0x41200000,
			 0x40e00000,
			 0x41300000,
			 0x40c00000,

			 /// vpu register f27
			 0x41a80000,
			 0x40c00000,
			 0x40400000,
			 0x41500000,
			 0x41f00000,
			 0x41900000,
			 0x42000000,
			 0x41000000,

			 /// vpu register f28
			 0x41a80000,
			 0x41800000,
			 0x41200000,
			 0x41800000,
			 0x40000000,
			 0x41880000,
			 0x41300000,
			 0x40000000,

			 /// vpu register f29
			 0x41600000,
			 0x41d80000,
			 0x40c00000,
			 0x40800000,
			 0x40400000,
			 0x41500000,
			 0x41a00000,
			 0x40800000,

			 /// vpu register f30
			 0x40800000,
			 0x41100000,
			 0x40800000,
			 0x40800000,
			 0x41e80000,
			 0x41200000,
			 0x40000000,
			 0x40c00000,

			 /// vpu register f31
			 0x3f800000,
			 0x41c80000,
			 0x41e80000,
			 0x41100000,
			 0x41600000,
			 0x40a00000,
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
		"fmax.s f16, f11, f11\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f14, f14, f3\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f21, f20, f10\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f27, f30, f21\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f21, f30, f9\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f29, f13, f11\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f4, f10, f13\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f9, f6, f20\n"                                ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f18, f7, f24\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f25, f25, f2\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f10, f4, f20\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f8, f3, f11\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f5, f4, f5\n"                                 ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f9, f2, f13\n"                                ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f5, f23, f26\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f16, f11, f6\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f22, f29, f30\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f15, f29, f7\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f24, f15, f1\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f26, f10, f17\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f21, f21, f13\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f7, f18, f6\n"                                ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f30, f27, f29\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f26, f5, f29\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f9, f30, f22\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f14, f17, f20\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f4, f5, f15\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f29, f6, f10\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f29, f26, f26\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f14, f2, f5\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f9, f13, f17\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f13, f19, f13\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f11, f0, f26\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f7, f18, f13\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f15, f24, f24\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f22, f15, f12\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f17, f13, f16\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f11, f3, f13\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f28, f0, f30\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f20, f23, f21\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f25, f25, f13\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f13, f18, f25\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f20, f5, f5\n"                                ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f3, f30, f6\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f7, f3, f15\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f11, f30, f22\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f21, f11, f5\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f15, f17, f15\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f14, f14, f4\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f15, f17, f19\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f16, f23, f8\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f13, f8, f12\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f24, f25, f21\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f25, f22, f15\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f10, f4, f14\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f13, f29, f28\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f5, f25, f8\n"                                ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f11, f5, f17\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f20, f14, f23\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f25, f22, f22\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f14, f30, f17\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f20, f11, f27\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f18, f15, f2\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f8, f29, f26\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f23, f20, f10\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f28, f13, f4\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f21, f7, f4\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f7, f4, f7\n"                                 ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f21, f6, f8\n"                                ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f3, f24, f19\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f30, f28, f25\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f23, f5, f6\n"                                ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f26, f17, f27\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f12, f27, f2\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f27, f15, f0\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f5, f0, f3\n"                                 ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f7, f19, f20\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f6, f16, f4\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f8, f17, f10\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f7, f14, f18\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f27, f11, f25\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f24, f11, f22\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f18, f18, f10\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f11, f13, f17\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f24, f7, f18\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f27, f24, f2\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f3, f19, f4\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f18, f0, f12\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f23, f16, f0\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f29, f4, f16\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f16, f10, f23\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f29, f18, f22\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f22, f5, f8\n"                                ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f30, f21, f26\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f14, f22, f0\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f25, f9, f13\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f23, f17, f5\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f25, f3, f13\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.s f20, f24, f5\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
