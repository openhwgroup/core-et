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
			 0xc1d829f1, /// 0x0
			 0x1e2ba79a, /// 0x4
			 0xf1d19f0f, /// 0x8
			 0x403645d9, /// 0xc
			 0xfc0f8080, /// 0x10
			 0xbd70ce37, /// 0x14
			 0xa65d8867, /// 0x18
			 0x5414eff6, /// 0x1c
			 0x74c3918e, /// 0x20
			 0x9a11a291, /// 0x24
			 0x7c8b6fb6, /// 0x28
			 0x7f0afce5, /// 0x2c
			 0x96960ef0, /// 0x30
			 0x30507997, /// 0x34
			 0x0a1925e3, /// 0x38
			 0x3e01b8b6, /// 0x3c
			 0x8a808941, /// 0x40
			 0x22a5a02a, /// 0x44
			 0x4fafc52a, /// 0x48
			 0xac6790f0, /// 0x4c
			 0xe28730e4, /// 0x50
			 0x3fd183f5, /// 0x54
			 0xeb4ea35b, /// 0x58
			 0x06f2b5a1, /// 0x5c
			 0xf8691bf2, /// 0x60
			 0x48da1eba, /// 0x64
			 0xbc95f752, /// 0x68
			 0x4649772c, /// 0x6c
			 0xe736b992, /// 0x70
			 0xfeaf5324, /// 0x74
			 0xc4a4a2e5, /// 0x78
			 0xdfd092af, /// 0x7c
			 0xdac397b3, /// 0x80
			 0x1bee8ddf, /// 0x84
			 0xb6d6228d, /// 0x88
			 0x7b0ff54c, /// 0x8c
			 0xed98ebda, /// 0x90
			 0x5958ef5f, /// 0x94
			 0x0a8eb89f, /// 0x98
			 0x561a6cf6, /// 0x9c
			 0x5ee0a584, /// 0xa0
			 0x25fc620e, /// 0xa4
			 0xf46095e4, /// 0xa8
			 0xa230afb4, /// 0xac
			 0xec8f8919, /// 0xb0
			 0xbf71c638, /// 0xb4
			 0x7867e5f8, /// 0xb8
			 0x5608c507, /// 0xbc
			 0x0be10e99, /// 0xc0
			 0xd5223181, /// 0xc4
			 0x86e5deef, /// 0xc8
			 0xb4ca0586, /// 0xcc
			 0x9c61c878, /// 0xd0
			 0xf2b291e0, /// 0xd4
			 0xec5697ce, /// 0xd8
			 0xb22f11ae, /// 0xdc
			 0xcb3a944d, /// 0xe0
			 0xd27a6e51, /// 0xe4
			 0xa56de4e8, /// 0xe8
			 0x54084668, /// 0xec
			 0x86c6f7da, /// 0xf0
			 0x34152dbd, /// 0xf4
			 0x3a8340ee, /// 0xf8
			 0x8d807d26, /// 0xfc
			 0x8c69d2eb, /// 0x100
			 0x0778d089, /// 0x104
			 0xfa07a1e8, /// 0x108
			 0x0f75a21b, /// 0x10c
			 0x2429f076, /// 0x110
			 0xd1f56ed8, /// 0x114
			 0x0e5ff36f, /// 0x118
			 0xde8526ed, /// 0x11c
			 0x85087ba5, /// 0x120
			 0xbd954b13, /// 0x124
			 0x05a76e04, /// 0x128
			 0xaa884a21, /// 0x12c
			 0x411e597b, /// 0x130
			 0x2a6f51cf, /// 0x134
			 0x46d6656c, /// 0x138
			 0x2b5c4539, /// 0x13c
			 0xb49c41d5, /// 0x140
			 0x9dcf6b65, /// 0x144
			 0x20eca773, /// 0x148
			 0x1a65670d, /// 0x14c
			 0x08e49bee, /// 0x150
			 0x61a7cd0e, /// 0x154
			 0x12adc122, /// 0x158
			 0xe8cab376, /// 0x15c
			 0x78ba106f, /// 0x160
			 0xc238ad7c, /// 0x164
			 0x1d00f152, /// 0x168
			 0x7ff8d5f3, /// 0x16c
			 0x976d5060, /// 0x170
			 0xf91993ee, /// 0x174
			 0x63edcb6c, /// 0x178
			 0xc10400f4, /// 0x17c
			 0x0eefa5de, /// 0x180
			 0xde2f02bb, /// 0x184
			 0x42d18930, /// 0x188
			 0x34fdae56, /// 0x18c
			 0x948670d4, /// 0x190
			 0x00b8bbc5, /// 0x194
			 0x8d4247d3, /// 0x198
			 0xdaa96158, /// 0x19c
			 0x54feb956, /// 0x1a0
			 0xf3665d5f, /// 0x1a4
			 0x902065ca, /// 0x1a8
			 0x8ba46fde, /// 0x1ac
			 0x720558d2, /// 0x1b0
			 0xf8430ac2, /// 0x1b4
			 0x5224fa9c, /// 0x1b8
			 0xb3cd6d65, /// 0x1bc
			 0x5623459f, /// 0x1c0
			 0x3e1b1c0c, /// 0x1c4
			 0xd222add8, /// 0x1c8
			 0x2820360b, /// 0x1cc
			 0x0380f08c, /// 0x1d0
			 0x89c3094b, /// 0x1d4
			 0xc6519b01, /// 0x1d8
			 0x6363d4e7, /// 0x1dc
			 0x69fb3422, /// 0x1e0
			 0xaa2cfbc4, /// 0x1e4
			 0x6e76fd9a, /// 0x1e8
			 0x0a787940, /// 0x1ec
			 0xbccd325c, /// 0x1f0
			 0xaed6741d, /// 0x1f4
			 0xe72eae7b, /// 0x1f8
			 0xaa390c85, /// 0x1fc
			 0xd6774faa, /// 0x200
			 0xe2238339, /// 0x204
			 0x64f22139, /// 0x208
			 0x72b21870, /// 0x20c
			 0x4456c14d, /// 0x210
			 0x41320cbd, /// 0x214
			 0x921876cf, /// 0x218
			 0xc1eee1dc, /// 0x21c
			 0x8af0e4ff, /// 0x220
			 0x4ac5bd70, /// 0x224
			 0x6646a74d, /// 0x228
			 0xaba4b61d, /// 0x22c
			 0x362b8328, /// 0x230
			 0x10817c32, /// 0x234
			 0x76790888, /// 0x238
			 0xc14075ed, /// 0x23c
			 0x6d9e5dba, /// 0x240
			 0x4449dfc8, /// 0x244
			 0xcd6cddf3, /// 0x248
			 0x472acca1, /// 0x24c
			 0xc7fef9b5, /// 0x250
			 0x4044a97c, /// 0x254
			 0x07ce4c80, /// 0x258
			 0xef440f5e, /// 0x25c
			 0xdb65ce85, /// 0x260
			 0x95e428ce, /// 0x264
			 0x593be7d7, /// 0x268
			 0x89004d2f, /// 0x26c
			 0xbaf446e9, /// 0x270
			 0x5d02cd51, /// 0x274
			 0x0ca1529e, /// 0x278
			 0x3fb95c1d, /// 0x27c
			 0x334530db, /// 0x280
			 0xb710b9f6, /// 0x284
			 0xf0839ed7, /// 0x288
			 0x1cf05026, /// 0x28c
			 0x6a2dddd5, /// 0x290
			 0x6ed7b5e2, /// 0x294
			 0x5a7cd66f, /// 0x298
			 0xcbfd4873, /// 0x29c
			 0x542f7c8b, /// 0x2a0
			 0x592dbc5f, /// 0x2a4
			 0x842a5fae, /// 0x2a8
			 0x3cb27a32, /// 0x2ac
			 0xf5a546db, /// 0x2b0
			 0x7f2ab36a, /// 0x2b4
			 0x19913f00, /// 0x2b8
			 0x5172ee61, /// 0x2bc
			 0x13e08467, /// 0x2c0
			 0x1dc79b20, /// 0x2c4
			 0x0bd43e7f, /// 0x2c8
			 0x8260771c, /// 0x2cc
			 0x618c6f31, /// 0x2d0
			 0x22c3126e, /// 0x2d4
			 0xc3f93b8e, /// 0x2d8
			 0x8fc754f8, /// 0x2dc
			 0x007dca65, /// 0x2e0
			 0xb26b0fcf, /// 0x2e4
			 0xd8bce6a0, /// 0x2e8
			 0x9be3166a, /// 0x2ec
			 0xaf59626b, /// 0x2f0
			 0x72a26fd6, /// 0x2f4
			 0xdbb68dc2, /// 0x2f8
			 0x199f643e, /// 0x2fc
			 0x0891313a, /// 0x300
			 0x6de48e4f, /// 0x304
			 0x33293720, /// 0x308
			 0x6f32b800, /// 0x30c
			 0x863f2aa8, /// 0x310
			 0xb217a0fb, /// 0x314
			 0xeca66973, /// 0x318
			 0x5d47dd0f, /// 0x31c
			 0xebdd3d97, /// 0x320
			 0x8a591467, /// 0x324
			 0x663def5a, /// 0x328
			 0xde9ce53d, /// 0x32c
			 0xe9ac6cf2, /// 0x330
			 0xda0dd6f0, /// 0x334
			 0x1ec20967, /// 0x338
			 0x25016eb3, /// 0x33c
			 0x4df48dfb, /// 0x340
			 0x489d4990, /// 0x344
			 0xd98bb730, /// 0x348
			 0xef9bf46d, /// 0x34c
			 0xb4bf085c, /// 0x350
			 0x7ee066d1, /// 0x354
			 0x9220810e, /// 0x358
			 0x3e5cac3b, /// 0x35c
			 0x2ce04c47, /// 0x360
			 0xdda97377, /// 0x364
			 0xc1e4898e, /// 0x368
			 0xbd7aac7e, /// 0x36c
			 0xc46047ba, /// 0x370
			 0x4248a5e1, /// 0x374
			 0x8bf7420d, /// 0x378
			 0x92927089, /// 0x37c
			 0x727c8473, /// 0x380
			 0x2f686343, /// 0x384
			 0x84adf517, /// 0x388
			 0xa365a420, /// 0x38c
			 0x8624e1bb, /// 0x390
			 0x13aec68c, /// 0x394
			 0xfbef2a2f, /// 0x398
			 0x8cbdeacd, /// 0x39c
			 0x44de7303, /// 0x3a0
			 0xccf675f4, /// 0x3a4
			 0xf28afcde, /// 0x3a8
			 0xdfc9dc90, /// 0x3ac
			 0xa26a50eb, /// 0x3b0
			 0x7ab862b6, /// 0x3b4
			 0xbbac0236, /// 0x3b8
			 0xd18d41b0, /// 0x3bc
			 0x4b31f109, /// 0x3c0
			 0x3adfd36c, /// 0x3c4
			 0xdba28eb5, /// 0x3c8
			 0x50524163, /// 0x3cc
			 0x33d4bbba, /// 0x3d0
			 0x947493fe, /// 0x3d4
			 0x09d8c058, /// 0x3d8
			 0xf454fb28, /// 0x3dc
			 0x75d8dbc4, /// 0x3e0
			 0x1d0929b9, /// 0x3e4
			 0xc3b05d91, /// 0x3e8
			 0x447ddda8, /// 0x3ec
			 0xc8fea1e8, /// 0x3f0
			 0xf1262002, /// 0x3f4
			 0x38e5cc5d, /// 0x3f8
			 0x3dca2618, /// 0x3fc
			 0x14509c56, /// 0x400
			 0x33bca6ec, /// 0x404
			 0xa5b17b7a, /// 0x408
			 0x2bc20ff5, /// 0x40c
			 0x22919776, /// 0x410
			 0xe98fb943, /// 0x414
			 0xeb76fce5, /// 0x418
			 0x9d430145, /// 0x41c
			 0xd66cef23, /// 0x420
			 0x70d8b82c, /// 0x424
			 0xde42ce19, /// 0x428
			 0x7001966e, /// 0x42c
			 0x85f4009f, /// 0x430
			 0x67c5062c, /// 0x434
			 0x5b422fc9, /// 0x438
			 0x9f891dc2, /// 0x43c
			 0xc47d7bdf, /// 0x440
			 0x155374cc, /// 0x444
			 0x6ab38834, /// 0x448
			 0x891e5887, /// 0x44c
			 0xbbefa8a0, /// 0x450
			 0xd280a012, /// 0x454
			 0x247fa11b, /// 0x458
			 0xc030dee4, /// 0x45c
			 0x41f51a49, /// 0x460
			 0x2a0e49fa, /// 0x464
			 0x0bf0e3bb, /// 0x468
			 0xd032bdc1, /// 0x46c
			 0xad23b6db, /// 0x470
			 0xfcb7eb48, /// 0x474
			 0x41a95bf0, /// 0x478
			 0xe20ec25d, /// 0x47c
			 0xdb1611c1, /// 0x480
			 0x5b22b7d7, /// 0x484
			 0x7892c746, /// 0x488
			 0xf737d2fc, /// 0x48c
			 0x2590afed, /// 0x490
			 0x20956fd5, /// 0x494
			 0x8e712277, /// 0x498
			 0xdfdd86c6, /// 0x49c
			 0x156c0a5a, /// 0x4a0
			 0xd52cfd80, /// 0x4a4
			 0xff3a080b, /// 0x4a8
			 0xf544bce8, /// 0x4ac
			 0xd6feb0c6, /// 0x4b0
			 0xcb12204b, /// 0x4b4
			 0x18cb1b9c, /// 0x4b8
			 0x0ecdfe11, /// 0x4bc
			 0x0bb89d76, /// 0x4c0
			 0x02bd8688, /// 0x4c4
			 0xb12cbc49, /// 0x4c8
			 0x87b09f3c, /// 0x4cc
			 0xf2dd0a8a, /// 0x4d0
			 0x8c4917f5, /// 0x4d4
			 0x42f75737, /// 0x4d8
			 0x49d796c6, /// 0x4dc
			 0xf6972620, /// 0x4e0
			 0x15749374, /// 0x4e4
			 0xdf69f286, /// 0x4e8
			 0xf91010c8, /// 0x4ec
			 0x16e933ac, /// 0x4f0
			 0x08f7db06, /// 0x4f4
			 0x3c44d5a3, /// 0x4f8
			 0xb288c0a6, /// 0x4fc
			 0x80e6dee5, /// 0x500
			 0xc3104cf4, /// 0x504
			 0x2501fb03, /// 0x508
			 0xe81a8481, /// 0x50c
			 0x14d50191, /// 0x510
			 0x9dac27e4, /// 0x514
			 0x6fb2abaf, /// 0x518
			 0x66cdaee6, /// 0x51c
			 0x3ca89b7c, /// 0x520
			 0xaafce372, /// 0x524
			 0x28b95678, /// 0x528
			 0x80bdfea4, /// 0x52c
			 0xb2c8dc77, /// 0x530
			 0xe831cec9, /// 0x534
			 0x7691ca6c, /// 0x538
			 0x7d7c1571, /// 0x53c
			 0x8f90d3e9, /// 0x540
			 0x470ce888, /// 0x544
			 0x2cb22d32, /// 0x548
			 0x02f7673c, /// 0x54c
			 0x7039b8d6, /// 0x550
			 0x0a4b801d, /// 0x554
			 0xe986a2b4, /// 0x558
			 0xec9174b5, /// 0x55c
			 0xd9ca6958, /// 0x560
			 0xd9d9d955, /// 0x564
			 0xd345cd7d, /// 0x568
			 0xfce7e24a, /// 0x56c
			 0x5cfee0f4, /// 0x570
			 0xa3196c85, /// 0x574
			 0x110b3f02, /// 0x578
			 0x3c0f4ea8, /// 0x57c
			 0x3ab10e92, /// 0x580
			 0x82efc6ca, /// 0x584
			 0xcb260c3d, /// 0x588
			 0xb2f60081, /// 0x58c
			 0x7405c48d, /// 0x590
			 0xb91a34c6, /// 0x594
			 0x7f50fe0a, /// 0x598
			 0xc4ebcad0, /// 0x59c
			 0x753f6e68, /// 0x5a0
			 0xf86bf41f, /// 0x5a4
			 0x58f46cca, /// 0x5a8
			 0x851a425f, /// 0x5ac
			 0x64c39cd0, /// 0x5b0
			 0x3b6d38ea, /// 0x5b4
			 0x5ee4f188, /// 0x5b8
			 0x63dffa9a, /// 0x5bc
			 0x1b2ebe77, /// 0x5c0
			 0x19687d69, /// 0x5c4
			 0xbff28ff9, /// 0x5c8
			 0x08178952, /// 0x5cc
			 0x5f3629ef, /// 0x5d0
			 0xb1b8ed6b, /// 0x5d4
			 0xc037d389, /// 0x5d8
			 0x24083e1c, /// 0x5dc
			 0x50c14d11, /// 0x5e0
			 0x3e185dcc, /// 0x5e4
			 0xbb824976, /// 0x5e8
			 0x0f2bbf79, /// 0x5ec
			 0xac701cfd, /// 0x5f0
			 0x75461977, /// 0x5f4
			 0xaad123ce, /// 0x5f8
			 0x6b24573b, /// 0x5fc
			 0x1a0d1b14, /// 0x600
			 0xd9e3e1a0, /// 0x604
			 0x51f8eb7c, /// 0x608
			 0x18c908c6, /// 0x60c
			 0xabb190ae, /// 0x610
			 0x05bcdf89, /// 0x614
			 0x44781443, /// 0x618
			 0xf979bfea, /// 0x61c
			 0x45ec409f, /// 0x620
			 0x247aad53, /// 0x624
			 0x5e870027, /// 0x628
			 0x89e902fb, /// 0x62c
			 0xcd8b1b0c, /// 0x630
			 0xa51dd274, /// 0x634
			 0x7748ceb8, /// 0x638
			 0x5e330e68, /// 0x63c
			 0x211f7ca3, /// 0x640
			 0x36ef9ba0, /// 0x644
			 0xfc74ede8, /// 0x648
			 0x01ab582d, /// 0x64c
			 0xb4d12d06, /// 0x650
			 0x9aaa35af, /// 0x654
			 0xb40c3e87, /// 0x658
			 0x52f4747e, /// 0x65c
			 0x34c529d8, /// 0x660
			 0xc827f56b, /// 0x664
			 0x3df068b5, /// 0x668
			 0x0d249c43, /// 0x66c
			 0x145d1f5c, /// 0x670
			 0xd86a0b46, /// 0x674
			 0x38b2c6a9, /// 0x678
			 0x5b2a69fd, /// 0x67c
			 0x116b2fad, /// 0x680
			 0x7fb985d2, /// 0x684
			 0xdafdec22, /// 0x688
			 0xf83ef511, /// 0x68c
			 0x058b0f6b, /// 0x690
			 0x39749b8e, /// 0x694
			 0x23641356, /// 0x698
			 0x1667f315, /// 0x69c
			 0xac945165, /// 0x6a0
			 0xdf1c9875, /// 0x6a4
			 0xfc7efb8e, /// 0x6a8
			 0xad8c91b4, /// 0x6ac
			 0x3f8dc0ff, /// 0x6b0
			 0xd10920a9, /// 0x6b4
			 0x2c28bc43, /// 0x6b8
			 0x6f0aa12d, /// 0x6bc
			 0x318db6dd, /// 0x6c0
			 0x42d26944, /// 0x6c4
			 0x677d46b8, /// 0x6c8
			 0x621ca7d1, /// 0x6cc
			 0xaf4befe9, /// 0x6d0
			 0x870ec2dd, /// 0x6d4
			 0xa0fbb6aa, /// 0x6d8
			 0x29c46bf6, /// 0x6dc
			 0x01a81c4d, /// 0x6e0
			 0x6d052f40, /// 0x6e4
			 0xc90f8f45, /// 0x6e8
			 0xddad30ee, /// 0x6ec
			 0x7b6f6256, /// 0x6f0
			 0xdb29cec6, /// 0x6f4
			 0xf44e3270, /// 0x6f8
			 0x567cf309, /// 0x6fc
			 0x97b75bef, /// 0x700
			 0x12876781, /// 0x704
			 0x5e2d98c3, /// 0x708
			 0xf70c8090, /// 0x70c
			 0x355b698c, /// 0x710
			 0xb005ea73, /// 0x714
			 0xfeff2e3f, /// 0x718
			 0x1810163a, /// 0x71c
			 0xe8e3090c, /// 0x720
			 0x1fcb23fd, /// 0x724
			 0x35ad4042, /// 0x728
			 0xbd87e19d, /// 0x72c
			 0x49a64fe3, /// 0x730
			 0x7549bc5c, /// 0x734
			 0x91143422, /// 0x738
			 0x6180e367, /// 0x73c
			 0x435eda91, /// 0x740
			 0xe5306fa0, /// 0x744
			 0x7d0fb2c1, /// 0x748
			 0x200b9b46, /// 0x74c
			 0xda32ae68, /// 0x750
			 0xb16dcc70, /// 0x754
			 0xf6dbe07f, /// 0x758
			 0xc8723977, /// 0x75c
			 0xde0aa22b, /// 0x760
			 0x84187b90, /// 0x764
			 0x032fb9c4, /// 0x768
			 0x6407b5df, /// 0x76c
			 0xf04869fa, /// 0x770
			 0xb738e618, /// 0x774
			 0xa545fa38, /// 0x778
			 0xca46bc23, /// 0x77c
			 0xb72039f6, /// 0x780
			 0xf2547e66, /// 0x784
			 0x037b7d07, /// 0x788
			 0x499f6ae6, /// 0x78c
			 0xe8c54580, /// 0x790
			 0xd3d060e0, /// 0x794
			 0x92ee0c90, /// 0x798
			 0xa95fabf9, /// 0x79c
			 0x10a649e7, /// 0x7a0
			 0x664101a7, /// 0x7a4
			 0x6dfbde36, /// 0x7a8
			 0x3d46fd29, /// 0x7ac
			 0x6b171902, /// 0x7b0
			 0x8296cae4, /// 0x7b4
			 0xcae9790d, /// 0x7b8
			 0x287d3dc8, /// 0x7bc
			 0xeb9182e7, /// 0x7c0
			 0x0f7dfe2d, /// 0x7c4
			 0x19bb434d, /// 0x7c8
			 0x55303d41, /// 0x7cc
			 0x6b60faf7, /// 0x7d0
			 0xc1300d53, /// 0x7d4
			 0xf5aa17ea, /// 0x7d8
			 0x96e8f53c, /// 0x7dc
			 0x82b7e543, /// 0x7e0
			 0x69d78784, /// 0x7e4
			 0x2447d9c9, /// 0x7e8
			 0x993f7173, /// 0x7ec
			 0x51e8abe1, /// 0x7f0
			 0xf6e2a54b, /// 0x7f4
			 0x5976373b, /// 0x7f8
			 0xf8ac802f, /// 0x7fc
			 0x24e3b0ff, /// 0x800
			 0xdfc4dc8b, /// 0x804
			 0x420ae3a9, /// 0x808
			 0x24ee91c8, /// 0x80c
			 0x96310dfc, /// 0x810
			 0x394456ab, /// 0x814
			 0x15da14cc, /// 0x818
			 0x3e4c6d36, /// 0x81c
			 0x70183386, /// 0x820
			 0x978afbcd, /// 0x824
			 0x130e19c8, /// 0x828
			 0xa3f8b608, /// 0x82c
			 0x514ad7f2, /// 0x830
			 0xd34989c5, /// 0x834
			 0x964fa572, /// 0x838
			 0x3c49b98d, /// 0x83c
			 0x8007a075, /// 0x840
			 0x0353b064, /// 0x844
			 0xbf068088, /// 0x848
			 0x9b05732f, /// 0x84c
			 0xa2cb1884, /// 0x850
			 0xd329b7b3, /// 0x854
			 0x900a3206, /// 0x858
			 0x08a529bb, /// 0x85c
			 0x17087f75, /// 0x860
			 0xcc67f3d8, /// 0x864
			 0xe6fca59e, /// 0x868
			 0x7e7e3158, /// 0x86c
			 0x72d938af, /// 0x870
			 0x18788968, /// 0x874
			 0xc1b9714a, /// 0x878
			 0x2f3502c7, /// 0x87c
			 0xf060aacd, /// 0x880
			 0x135e7a82, /// 0x884
			 0xd5c62937, /// 0x888
			 0x57207c56, /// 0x88c
			 0x792bbb30, /// 0x890
			 0x14246b3b, /// 0x894
			 0x7fe44b7b, /// 0x898
			 0xacbb04e7, /// 0x89c
			 0x692fece0, /// 0x8a0
			 0xdfa1bbc5, /// 0x8a4
			 0xf9419160, /// 0x8a8
			 0xb2c47451, /// 0x8ac
			 0x3f3bbc7a, /// 0x8b0
			 0xa1a172d7, /// 0x8b4
			 0x7c07797a, /// 0x8b8
			 0xf229033c, /// 0x8bc
			 0xf26cb6a0, /// 0x8c0
			 0x1a505705, /// 0x8c4
			 0x925e29c8, /// 0x8c8
			 0x72456d7f, /// 0x8cc
			 0xfdf4761c, /// 0x8d0
			 0x5bb0fa2c, /// 0x8d4
			 0xc4b5ed22, /// 0x8d8
			 0x1f7f63af, /// 0x8dc
			 0x549f8ad8, /// 0x8e0
			 0xdf6ff10a, /// 0x8e4
			 0x3bb7a93a, /// 0x8e8
			 0x425ad20b, /// 0x8ec
			 0xd54fc983, /// 0x8f0
			 0x4b8798d5, /// 0x8f4
			 0x6d95405f, /// 0x8f8
			 0x777bcb75, /// 0x8fc
			 0xe4b92a65, /// 0x900
			 0xbbc3315a, /// 0x904
			 0x39c6ca0b, /// 0x908
			 0x604d9efa, /// 0x90c
			 0x8947d373, /// 0x910
			 0xb78cf824, /// 0x914
			 0x089e6870, /// 0x918
			 0x59b8aacf, /// 0x91c
			 0x7e9676a5, /// 0x920
			 0x2f008708, /// 0x924
			 0x046f670e, /// 0x928
			 0x253228d8, /// 0x92c
			 0x7b9348f9, /// 0x930
			 0x24788384, /// 0x934
			 0x91b39998, /// 0x938
			 0x9d4f0084, /// 0x93c
			 0x4c02dade, /// 0x940
			 0x40fef2a3, /// 0x944
			 0x9fe682a7, /// 0x948
			 0x8712e54a, /// 0x94c
			 0x44446cd5, /// 0x950
			 0xc5bdcb91, /// 0x954
			 0x2c589c0d, /// 0x958
			 0x9fa2db1a, /// 0x95c
			 0x57c0589c, /// 0x960
			 0x0e3adae6, /// 0x964
			 0x6616fd1f, /// 0x968
			 0x7effba39, /// 0x96c
			 0x039387cb, /// 0x970
			 0x0846b9b4, /// 0x974
			 0x3986702d, /// 0x978
			 0x6ad342b0, /// 0x97c
			 0xfcc7bbd9, /// 0x980
			 0x7ae3aed9, /// 0x984
			 0x79c3e022, /// 0x988
			 0x8339329e, /// 0x98c
			 0xb044bb6f, /// 0x990
			 0xa0cb47b9, /// 0x994
			 0xe764944b, /// 0x998
			 0x3a7bca96, /// 0x99c
			 0xf61b7c8c, /// 0x9a0
			 0x097726e3, /// 0x9a4
			 0xe42720aa, /// 0x9a8
			 0xbfae897e, /// 0x9ac
			 0x82366324, /// 0x9b0
			 0xd29548f6, /// 0x9b4
			 0x43874d9f, /// 0x9b8
			 0xcba6b2ed, /// 0x9bc
			 0x41cd9d80, /// 0x9c0
			 0x84ae018e, /// 0x9c4
			 0xa2ad28ea, /// 0x9c8
			 0x2a5f5519, /// 0x9cc
			 0x3010dfb5, /// 0x9d0
			 0xd8243468, /// 0x9d4
			 0x04e2c09e, /// 0x9d8
			 0x17ddd138, /// 0x9dc
			 0x7543fd00, /// 0x9e0
			 0x94b6e58a, /// 0x9e4
			 0x40c40abe, /// 0x9e8
			 0x0e0f76d2, /// 0x9ec
			 0x622c1903, /// 0x9f0
			 0x76f3ad7c, /// 0x9f4
			 0xf8ed729b, /// 0x9f8
			 0xd947fd46, /// 0x9fc
			 0x6cafb828, /// 0xa00
			 0x360e1c0b, /// 0xa04
			 0x508a7b3c, /// 0xa08
			 0x884aa2c6, /// 0xa0c
			 0x6c78858f, /// 0xa10
			 0x05196726, /// 0xa14
			 0x7a46f0ae, /// 0xa18
			 0x8b167aa7, /// 0xa1c
			 0x63f24325, /// 0xa20
			 0xbfe56004, /// 0xa24
			 0x306d24c5, /// 0xa28
			 0x2a4dbf37, /// 0xa2c
			 0x054cab2b, /// 0xa30
			 0x53d63e3c, /// 0xa34
			 0x12b3ed3d, /// 0xa38
			 0xedffb0c7, /// 0xa3c
			 0x1d7c0298, /// 0xa40
			 0x387821cd, /// 0xa44
			 0x12dba768, /// 0xa48
			 0x52a06100, /// 0xa4c
			 0x226d657a, /// 0xa50
			 0xc663a02c, /// 0xa54
			 0x1792babc, /// 0xa58
			 0x5a30d036, /// 0xa5c
			 0xebc1fe45, /// 0xa60
			 0xde01fb57, /// 0xa64
			 0x6b44a3d6, /// 0xa68
			 0xf6a9d765, /// 0xa6c
			 0xc822d677, /// 0xa70
			 0x6be7a107, /// 0xa74
			 0x49f48bbe, /// 0xa78
			 0xd20326eb, /// 0xa7c
			 0x2986f61b, /// 0xa80
			 0x92db4dfe, /// 0xa84
			 0xf00bf791, /// 0xa88
			 0x809fc288, /// 0xa8c
			 0xa8060347, /// 0xa90
			 0x7d7192bf, /// 0xa94
			 0x5c65d0b4, /// 0xa98
			 0x3fc38eaa, /// 0xa9c
			 0xc063393a, /// 0xaa0
			 0x441a85ac, /// 0xaa4
			 0x8ae39e55, /// 0xaa8
			 0x68200b1e, /// 0xaac
			 0xc6969b29, /// 0xab0
			 0x7377953a, /// 0xab4
			 0x0be319c5, /// 0xab8
			 0x255f11d2, /// 0xabc
			 0x89292ba2, /// 0xac0
			 0xe3502784, /// 0xac4
			 0x12f8c6c7, /// 0xac8
			 0x9c7e878e, /// 0xacc
			 0xcabfdfd8, /// 0xad0
			 0xd91df4de, /// 0xad4
			 0x8e066b1f, /// 0xad8
			 0xc46037b8, /// 0xadc
			 0x7ce72137, /// 0xae0
			 0x7af32c97, /// 0xae4
			 0xb3c5c286, /// 0xae8
			 0x303f1f4d, /// 0xaec
			 0xac5885bb, /// 0xaf0
			 0x67b5bfb8, /// 0xaf4
			 0xe38c4579, /// 0xaf8
			 0x9e63227e, /// 0xafc
			 0x9aaf8cdc, /// 0xb00
			 0xcf434efa, /// 0xb04
			 0x9c286e7a, /// 0xb08
			 0x295b5c43, /// 0xb0c
			 0x3fd1d162, /// 0xb10
			 0x23d450cf, /// 0xb14
			 0xc5b4f68f, /// 0xb18
			 0x1ba180b2, /// 0xb1c
			 0x311c40a0, /// 0xb20
			 0xc4cfa2db, /// 0xb24
			 0x1e674aa0, /// 0xb28
			 0x7072aa76, /// 0xb2c
			 0x2591c7ff, /// 0xb30
			 0x5ad95c00, /// 0xb34
			 0x15382085, /// 0xb38
			 0xfcd779f0, /// 0xb3c
			 0x0746adb0, /// 0xb40
			 0x49f7ac5a, /// 0xb44
			 0xe4358bbd, /// 0xb48
			 0xd0ec2eb3, /// 0xb4c
			 0x1450fafc, /// 0xb50
			 0x15e73037, /// 0xb54
			 0x3d9a667e, /// 0xb58
			 0x7ab53d71, /// 0xb5c
			 0xb23a84b1, /// 0xb60
			 0x1b444df4, /// 0xb64
			 0x547a137f, /// 0xb68
			 0x6d2e6eee, /// 0xb6c
			 0x18a9481e, /// 0xb70
			 0xb4956ec2, /// 0xb74
			 0x79563190, /// 0xb78
			 0x71f7ebf6, /// 0xb7c
			 0xcdc15883, /// 0xb80
			 0x0d39a624, /// 0xb84
			 0x61cef647, /// 0xb88
			 0x485e01e6, /// 0xb8c
			 0x1674f18a, /// 0xb90
			 0xbf84ec33, /// 0xb94
			 0x85d61a1c, /// 0xb98
			 0x0ed8e135, /// 0xb9c
			 0xcdf5af1e, /// 0xba0
			 0x3bf9d963, /// 0xba4
			 0xe4f03f7c, /// 0xba8
			 0xe95f183b, /// 0xbac
			 0x631b470d, /// 0xbb0
			 0x00313e51, /// 0xbb4
			 0xc9a55c23, /// 0xbb8
			 0x7ec5aced, /// 0xbbc
			 0x31d56075, /// 0xbc0
			 0x6f9cae0e, /// 0xbc4
			 0xda33fa02, /// 0xbc8
			 0xf0f7d2fd, /// 0xbcc
			 0x057dcb96, /// 0xbd0
			 0x4a066cf1, /// 0xbd4
			 0xf5bbb9eb, /// 0xbd8
			 0x639b05da, /// 0xbdc
			 0x78bfc79a, /// 0xbe0
			 0x8b7fc92c, /// 0xbe4
			 0x36025035, /// 0xbe8
			 0x78c96c19, /// 0xbec
			 0xd94e4ba2, /// 0xbf0
			 0x9d15f60f, /// 0xbf4
			 0xd5155de9, /// 0xbf8
			 0x519f8e1e, /// 0xbfc
			 0xf4975174, /// 0xc00
			 0x17220392, /// 0xc04
			 0x66a9b4e0, /// 0xc08
			 0x18492757, /// 0xc0c
			 0xe7431a6a, /// 0xc10
			 0x27124ffd, /// 0xc14
			 0xea4f3abd, /// 0xc18
			 0x8f9602f7, /// 0xc1c
			 0xacb29669, /// 0xc20
			 0xebea8824, /// 0xc24
			 0x11f65b2c, /// 0xc28
			 0x9156c365, /// 0xc2c
			 0xf62ff4c3, /// 0xc30
			 0xcc64c01d, /// 0xc34
			 0x4a6ad6bb, /// 0xc38
			 0x7ded2ea8, /// 0xc3c
			 0x92d69651, /// 0xc40
			 0x2bc953e2, /// 0xc44
			 0x4359e681, /// 0xc48
			 0xfbedd36c, /// 0xc4c
			 0xafb9e1e2, /// 0xc50
			 0xde89cdce, /// 0xc54
			 0xff4f6d53, /// 0xc58
			 0xd7bb5454, /// 0xc5c
			 0x6f207bd3, /// 0xc60
			 0xd0886251, /// 0xc64
			 0x248b8bbc, /// 0xc68
			 0x88117f1a, /// 0xc6c
			 0x1fe55286, /// 0xc70
			 0xb835a4c4, /// 0xc74
			 0x0ca5d90d, /// 0xc78
			 0x673511eb, /// 0xc7c
			 0xc9b68f4d, /// 0xc80
			 0x812c0a83, /// 0xc84
			 0xaabf6df3, /// 0xc88
			 0x52affac9, /// 0xc8c
			 0x4d1b6516, /// 0xc90
			 0xd9694fb9, /// 0xc94
			 0x7841fa35, /// 0xc98
			 0xa7b36b7f, /// 0xc9c
			 0x23e17d6f, /// 0xca0
			 0x7bfd7222, /// 0xca4
			 0xa05be5f7, /// 0xca8
			 0xd0390c21, /// 0xcac
			 0xcc60beb8, /// 0xcb0
			 0x4c1590c2, /// 0xcb4
			 0x4ff88925, /// 0xcb8
			 0x04ca3b35, /// 0xcbc
			 0xc76453bb, /// 0xcc0
			 0xdf68f5e3, /// 0xcc4
			 0x6d2c3633, /// 0xcc8
			 0xd50380c8, /// 0xccc
			 0x3c09096e, /// 0xcd0
			 0xa08937eb, /// 0xcd4
			 0x287079c3, /// 0xcd8
			 0xc01145b8, /// 0xcdc
			 0x62651560, /// 0xce0
			 0xc6ced810, /// 0xce4
			 0x26179cbe, /// 0xce8
			 0x8ef68dbf, /// 0xcec
			 0x92ec46ae, /// 0xcf0
			 0xd58d7233, /// 0xcf4
			 0xffa3813e, /// 0xcf8
			 0x30a4c81b, /// 0xcfc
			 0xd97ae1fa, /// 0xd00
			 0xa008fd61, /// 0xd04
			 0x4a906583, /// 0xd08
			 0xbff09715, /// 0xd0c
			 0xff8ee5c2, /// 0xd10
			 0x5cae3ada, /// 0xd14
			 0x27393ba5, /// 0xd18
			 0x61243208, /// 0xd1c
			 0x9d6e37b7, /// 0xd20
			 0x245d18e1, /// 0xd24
			 0x3539b0dd, /// 0xd28
			 0x029f98d3, /// 0xd2c
			 0x58284f6c, /// 0xd30
			 0x4f21c8b6, /// 0xd34
			 0xc7ae4d0f, /// 0xd38
			 0x4b0069e7, /// 0xd3c
			 0xf7784ffe, /// 0xd40
			 0x3953af62, /// 0xd44
			 0x57f393c1, /// 0xd48
			 0x2300a54e, /// 0xd4c
			 0xe8ea09cf, /// 0xd50
			 0x5f2bf7a5, /// 0xd54
			 0xb805f5d1, /// 0xd58
			 0x0801facc, /// 0xd5c
			 0xbbb66247, /// 0xd60
			 0x5e147e4b, /// 0xd64
			 0xdddd82fe, /// 0xd68
			 0x3d2c3f42, /// 0xd6c
			 0x29e32baf, /// 0xd70
			 0x3a7f3258, /// 0xd74
			 0x1e7e54c1, /// 0xd78
			 0xd5811b1d, /// 0xd7c
			 0x22fea98a, /// 0xd80
			 0x2f027b8e, /// 0xd84
			 0x61dba10f, /// 0xd88
			 0x5034a21c, /// 0xd8c
			 0x12fcb195, /// 0xd90
			 0xe5c065db, /// 0xd94
			 0xc713e320, /// 0xd98
			 0x972687c1, /// 0xd9c
			 0xfbfc0f5f, /// 0xda0
			 0x0784ef9e, /// 0xda4
			 0x8742b1a3, /// 0xda8
			 0xc1fdd5db, /// 0xdac
			 0x515a53a8, /// 0xdb0
			 0xf68e604b, /// 0xdb4
			 0x2d7c6c50, /// 0xdb8
			 0x7e06c886, /// 0xdbc
			 0xcae83818, /// 0xdc0
			 0xbbeda719, /// 0xdc4
			 0xfda5fe3c, /// 0xdc8
			 0x0e4d875d, /// 0xdcc
			 0xe3884b8e, /// 0xdd0
			 0x7e4f4cf2, /// 0xdd4
			 0xc1d5c23c, /// 0xdd8
			 0xc26f5d2a, /// 0xddc
			 0x33952eaa, /// 0xde0
			 0x0a8cdcf2, /// 0xde4
			 0x7a549d0e, /// 0xde8
			 0xc5dde096, /// 0xdec
			 0xb687bebd, /// 0xdf0
			 0xabdb232c, /// 0xdf4
			 0x5be1546d, /// 0xdf8
			 0xe7e7d7f9, /// 0xdfc
			 0x48cf0b53, /// 0xe00
			 0x0c7ac963, /// 0xe04
			 0xfaf0995b, /// 0xe08
			 0x10fddd11, /// 0xe0c
			 0x76b455cd, /// 0xe10
			 0x38daafff, /// 0xe14
			 0x48950099, /// 0xe18
			 0x73f747b8, /// 0xe1c
			 0x366caf0b, /// 0xe20
			 0xafbbc5ba, /// 0xe24
			 0x88e12259, /// 0xe28
			 0x700a4003, /// 0xe2c
			 0x72abc667, /// 0xe30
			 0x92a6a295, /// 0xe34
			 0x8bbdaabb, /// 0xe38
			 0xbf4c4e20, /// 0xe3c
			 0xe3f11700, /// 0xe40
			 0x99f75ae4, /// 0xe44
			 0x2af26950, /// 0xe48
			 0xd8a32a6e, /// 0xe4c
			 0x6901af85, /// 0xe50
			 0x09fb3cea, /// 0xe54
			 0xff657000, /// 0xe58
			 0xadb89bd1, /// 0xe5c
			 0x43ba0acd, /// 0xe60
			 0x64c019a1, /// 0xe64
			 0xd30dd4e3, /// 0xe68
			 0xb0182132, /// 0xe6c
			 0xfda6337d, /// 0xe70
			 0xa592500e, /// 0xe74
			 0x2794441f, /// 0xe78
			 0xc9c888e3, /// 0xe7c
			 0x6006b779, /// 0xe80
			 0x46e70c34, /// 0xe84
			 0x67db2e59, /// 0xe88
			 0x915dfae3, /// 0xe8c
			 0xace98198, /// 0xe90
			 0x3619b6f0, /// 0xe94
			 0x186b3ad8, /// 0xe98
			 0xfb5fb243, /// 0xe9c
			 0x409b5624, /// 0xea0
			 0xc72df33d, /// 0xea4
			 0x1b2c3638, /// 0xea8
			 0x0c1cdae2, /// 0xeac
			 0x24b883fe, /// 0xeb0
			 0x74bf36f5, /// 0xeb4
			 0xca9064c7, /// 0xeb8
			 0xd98533e4, /// 0xebc
			 0xcb3c0dea, /// 0xec0
			 0x44b53a3b, /// 0xec4
			 0x859bc422, /// 0xec8
			 0x959aa255, /// 0xecc
			 0xf6773a2e, /// 0xed0
			 0xb2402146, /// 0xed4
			 0xb60c791c, /// 0xed8
			 0xba1e71b6, /// 0xedc
			 0xe2cd98ae, /// 0xee0
			 0x78759cda, /// 0xee4
			 0x48e1a6ea, /// 0xee8
			 0xd8b5404b, /// 0xeec
			 0x4f8f73fe, /// 0xef0
			 0x17c3a816, /// 0xef4
			 0x1994aa09, /// 0xef8
			 0x894efc2b, /// 0xefc
			 0x4d30c09d, /// 0xf00
			 0x0fd1879a, /// 0xf04
			 0xcba24c92, /// 0xf08
			 0x6fceffab, /// 0xf0c
			 0x011597c8, /// 0xf10
			 0x5c1501cd, /// 0xf14
			 0x5b29fcfc, /// 0xf18
			 0x01a75d97, /// 0xf1c
			 0x2cee0d4b, /// 0xf20
			 0xb03c578f, /// 0xf24
			 0x5f2907b6, /// 0xf28
			 0x2d3dbd76, /// 0xf2c
			 0x85256dfc, /// 0xf30
			 0x1e646840, /// 0xf34
			 0x9abc8a8a, /// 0xf38
			 0x26758e39, /// 0xf3c
			 0xbda9c63e, /// 0xf40
			 0xe305af8f, /// 0xf44
			 0xd57fe6d9, /// 0xf48
			 0x62d07bb8, /// 0xf4c
			 0x768c1a1e, /// 0xf50
			 0xaf79a383, /// 0xf54
			 0x29a0eac7, /// 0xf58
			 0x04d8edf7, /// 0xf5c
			 0x9cbe7656, /// 0xf60
			 0x3e5791e9, /// 0xf64
			 0x0776796e, /// 0xf68
			 0x191468a4, /// 0xf6c
			 0x25c86975, /// 0xf70
			 0xdfe08fc2, /// 0xf74
			 0x76dbb527, /// 0xf78
			 0xff21c395, /// 0xf7c
			 0x61eceb72, /// 0xf80
			 0xc09e8af2, /// 0xf84
			 0xfd7604d2, /// 0xf88
			 0x7ddc8af8, /// 0xf8c
			 0x44e2b613, /// 0xf90
			 0x7582cc17, /// 0xf94
			 0x1ccbe33c, /// 0xf98
			 0xfa47c9e8, /// 0xf9c
			 0xb2b1eaec, /// 0xfa0
			 0xde98a18f, /// 0xfa4
			 0x4a758c74, /// 0xfa8
			 0x3bf9cd8f, /// 0xfac
			 0x9816a283, /// 0xfb0
			 0xe66e7fbd, /// 0xfb4
			 0x3d9032d1, /// 0xfb8
			 0xddd6461f, /// 0xfbc
			 0xcc31b3d6, /// 0xfc0
			 0x941808c7, /// 0xfc4
			 0x7c62b4f6, /// 0xfc8
			 0x5ab17995, /// 0xfcc
			 0x818ab965, /// 0xfd0
			 0x397cc26b, /// 0xfd4
			 0xba68308d, /// 0xfd8
			 0x9e86f2ed, /// 0xfdc
			 0xddc8dfd6, /// 0xfe0
			 0x23acf34f, /// 0xfe4
			 0x9eaed36a, /// 0xfe8
			 0xad2165a0, /// 0xfec
			 0x3163200b, /// 0xff0
			 0x30508593, /// 0xff4
			 0xa9fb8897, /// 0xff8
			 0x5d07accd /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00000
			 0x4b000000,                                                  // 8388608.0                                   /// 00004
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00008
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0000c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00014
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x80011111,                                                  // -9.7958E-41                                 /// 0001c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00024
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0002c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00038
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00040
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00044
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00048
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0004c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00054
			 0x55555555,                                                  // 4 random values                             /// 00058
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0005c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00060
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00064
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0006c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00070
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00074
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00078
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00080
			 0xcb000000,                                                  // -8388608.0                                  /// 00084
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00088
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0008c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00090
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00094
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00098
			 0xffc00001,                                                  // -signaling NaN                              /// 0009c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000a0
			 0xffc00001,                                                  // -signaling NaN                              /// 000a4
			 0xcb000000,                                                  // -8388608.0                                  /// 000a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00100
			 0x0e000001,                                                  // Trailing 1s:                                /// 00104
			 0x0e000003,                                                  // Trailing 1s:                                /// 00108
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0010c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00110
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00120
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x7f800000,                                                  // inf                                         /// 00128
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00130
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x3f028f5c,                                                  // 0.51                                        /// 00138
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00144
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00148
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00158
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0015c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x00000000,                                                  // zero                                        /// 00168
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0016c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00170
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00174
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00178
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0017c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00180
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00184
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00190
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00194
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00198
			 0x0c600000,                                                  // Leading 1s:                                 /// 0019c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001d8
			 0x80000000,                                                  // -zero                                       /// 001dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001e0
			 0x7f800000,                                                  // inf                                         /// 001e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00200
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00204
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00208
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0020c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00210
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00224
			 0xcb000000,                                                  // -8388608.0                                  /// 00228
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0022c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00230
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00234
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00240
			 0x00000000,                                                  // zero                                        /// 00244
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0024c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00254
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00258
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0025c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00260
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00268
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0026c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x4b000000,                                                  // 8388608.0                                   /// 00274
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00278
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0027c
			 0x00000000,                                                  // zero                                        /// 00280
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00288
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0028c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00290
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0029c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a4
			 0xcb000000,                                                  // -8388608.0                                  /// 002a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002cc
			 0x7fc00001,                                                  // signaling NaN                               /// 002d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d4
			 0x33333333,                                                  // 4 random values                             /// 002d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00300
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00304
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00310
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00314
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00320
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00328
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0032c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00330
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00334
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0033c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00340
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00344
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00348
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0034c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00350
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00354
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00358
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00360
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00368
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0036c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00370
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00374
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0037c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00384
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00388
			 0x3f028f5c,                                                  // 0.51                                        /// 0038c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00398
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0039c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003a4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003ec
			 0xffc00001,                                                  // -signaling NaN                              /// 003f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00400
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00404
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00408
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00410
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0c780000,                                                  // Leading 1s:                                 /// 00418
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0041c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00420
			 0x55555555,                                                  // 4 random values                             /// 00424
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00428
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0042c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00430
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00434
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00438
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0043c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00440
			 0x4b000000,                                                  // 8388608.0                                   /// 00444
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00448
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00450
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00454
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00458
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0045c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00460
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00464
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00468
			 0x00011111,                                                  // 9.7958E-41                                  /// 0046c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00470
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00474
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0047c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00480
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00484
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00488
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0048c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00490
			 0x0c700000,                                                  // Leading 1s:                                 /// 00494
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00498
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0049c
			 0x0c780000,                                                  // Leading 1s:                                 /// 004a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004ac
			 0xffc00001,                                                  // -signaling NaN                              /// 004b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004b8
			 0x80000000,                                                  // -zero                                       /// 004bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c0
			 0x7f800000,                                                  // inf                                         /// 004c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 004f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00500
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00504
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00508
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00510
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00514
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00518
			 0xbf028f5c,                                                  // -0.51                                       /// 0051c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00520
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00528
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0052c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00530
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00538
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0053c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0054c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00550
			 0x7f800000,                                                  // inf                                         /// 00554
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0055c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00560
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00564
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00568
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00570
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00578
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0xbf028f5c,                                                  // -0.51                                       /// 00580
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00584
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00588
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0058c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00594
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00598
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 005d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005f4
			 0x7f800000,                                                  // inf                                         /// 005f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00600
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00604
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0060c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00610
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00614
			 0x0c780000,                                                  // Leading 1s:                                 /// 00618
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0061c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00624
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x3f028f5c,                                                  // 0.51                                        /// 0062c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00630
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00634
			 0xffc00001,                                                  // -signaling NaN                              /// 00638
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0063c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00650
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00658
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0065c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x80011111,                                                  // -9.7958E-41                                 /// 00664
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00668
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x3f028f5c,                                                  // 0.51                                        /// 00670
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00674
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0067c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00680
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00688
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0068c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00690
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00694
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00698
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006e4
			 0x80000000,                                                  // -zero                                       /// 006e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00700
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0070c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00710
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00718
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0071c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00720
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00724
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00728
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00734
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00738
			 0x80000000,                                                  // -zero                                       /// 0073c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00740
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00744
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0074c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00750
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00754
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00758
			 0xff800000,                                                  // -inf                                        /// 0075c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00760
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00770
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00774
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00778
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00780
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00788
			 0x7f800000,                                                  // inf                                         /// 0078c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00794
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00798
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0079c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ac
			 0xcb000000,                                                  // -8388608.0                                  /// 007b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 007b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00800
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00804
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00808
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0080c
			 0x7f800000,                                                  // inf                                         /// 00810
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00814
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00818
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0081c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00824
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00834
			 0x4b000000,                                                  // 8388608.0                                   /// 00838
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0083c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00848
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0084c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00850
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00854
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00858
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0085c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00860
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00864
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00868
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0086c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00870
			 0x33333333,                                                  // 4 random values                             /// 00874
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00878
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0087c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00880
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00884
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00888
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0088c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00890
			 0x0e000007,                                                  // Trailing 1s:                                /// 00894
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 008b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x3f028f5c,                                                  // 0.51                                        /// 008b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008d4
			 0x3f028f5c,                                                  // 0.51                                        /// 008d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0xbf028f5c,                                                  // -0.51                                       /// 008f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00900
			 0x0c600000,                                                  // Leading 1s:                                 /// 00904
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00908
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00910
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00918
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0091c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00928
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0092c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00934
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00938
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0093c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00940
			 0x0e000003,                                                  // Trailing 1s:                                /// 00944
			 0x0c600000,                                                  // Leading 1s:                                 /// 00948
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0094c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0095c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00960
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00964
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00968
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0096c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0c700000,                                                  // Leading 1s:                                 /// 00974
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00978
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0097c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00980
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00984
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00988
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0098c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00990
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00994
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00998
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0099c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 009bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80000000,                                                  // -zero                                       /// 009c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009e0
			 0x3f028f5c,                                                  // 0.51                                        /// 009e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a08
			 0x7fc00001,                                                  // signaling NaN                               /// 00a0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a14
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a1c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a3c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a48
			 0xcb000000,                                                  // -8388608.0                                  /// 00a4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a68
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a74
			 0x55555555,                                                  // 4 random values                             /// 00a78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x33333333,                                                  // 4 random values                             /// 00aa0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aa4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00aac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ab8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00abc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x4b000000,                                                  // 8388608.0                                   /// 00adc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ae0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ae4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ae8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00af4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00afc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b2c
			 0xff800000,                                                  // -inf                                        /// 00b30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b54
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b6c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b74
			 0x3f028f5c,                                                  // 0.51                                        /// 00b78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80000000,                                                  // -zero                                       /// 00b8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b98
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ba8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bb4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bc0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bc4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bd0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00be8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bf0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bf4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0xcb000000,                                                  // -8388608.0                                  /// 00c00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c48
			 0x3f028f5c,                                                  // 0.51                                        /// 00c4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c54
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ca4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cb0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cb4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cb8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00cd4
			 0x55555555,                                                  // 4 random values                             /// 00cd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cdc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ce8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cf8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cfc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d00
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d24
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d34
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d38
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7f800000,                                                  // inf                                         /// 00d54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d5c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d64
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d9c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00da8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dc0
			 0xcb000000,                                                  // -8388608.0                                  /// 00dc4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dd0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0xff800000,                                                  // -inf                                        /// 00dd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ddc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00de0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00de4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00df0
			 0xff800000,                                                  // -inf                                        /// 00df4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00df8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e20
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x00000000,                                                  // zero                                        /// 00e28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e40
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e54
			 0x4b000000,                                                  // 8388608.0                                   /// 00e58
			 0x4b000000,                                                  // 8388608.0                                   /// 00e5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e74
			 0x33333333,                                                  // 4 random values                             /// 00e78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e88
			 0x7f800000,                                                  // inf                                         /// 00e8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ea0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ea4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0xffc00001,                                                  // -signaling NaN                              /// 00eb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00ebc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ec8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ecc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ed8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00edc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ee0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ee4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ee8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ef0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ef4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00efc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f00
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f30
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f38
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f40
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f64
			 0x7fc00001,                                                  // signaling NaN                               /// 00f68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f6c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f7c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f80
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fa0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fa8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fb0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fb4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fb8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fbc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000000,                                                  // zero                                        /// 00fcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0xffc00001,                                                  // -signaling NaN                              /// 00fe0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ff4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ff8
			 0x0f7ffffe                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0000c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00014
			 0x33333333,                                                  // 4 random values                             /// 00018
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00020
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00028
			 0x0e000003,                                                  // Trailing 1s:                                /// 0002c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00030
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00038
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0003c
			 0x7f800000,                                                  // inf                                         /// 00040
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00044
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00048
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00050
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00054
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00058
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0005c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00060
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00064
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00068
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0006c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00070
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0xcb000000,                                                  // -8388608.0                                  /// 00078
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0007c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00080
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00088
			 0xff800000,                                                  // -inf                                        /// 0008c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00094
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00100
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00104
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00108
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00114
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00118
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0011c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00120
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00124
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00128
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0012c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00130
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00134
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00138
			 0x80011111,                                                  // -9.7958E-41                                 /// 0013c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x7fc00001,                                                  // signaling NaN                               /// 0014c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00150
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00154
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0015c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x00011111,                                                  // 9.7958E-41                                  /// 00164
			 0xff800000,                                                  // -inf                                        /// 00168
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0016c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00174
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0017c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00188
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0018c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0019c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001ec
			 0x33333333,                                                  // 4 random values                             /// 001f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x7f800000,                                                  // inf                                         /// 00200
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00204
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00208
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0020c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00210
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00218
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0021c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00220
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00228
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00234
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00238
			 0x0e000003,                                                  // Trailing 1s:                                /// 0023c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00254
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00260
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00264
			 0x7f800000,                                                  // inf                                         /// 00268
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00274
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00278
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00280
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00284
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0028c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00298
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00304
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00308
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0030c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00310
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x00011111,                                                  // 9.7958E-41                                  /// 00318
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0031c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00320
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00328
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0032c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00330
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00334
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00338
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00344
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00348
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0034c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00350
			 0x00000000,                                                  // zero                                        /// 00354
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00358
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0035c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00364
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00368
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0036c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0037c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00394
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003f4
			 0xffc00001,                                                  // -signaling NaN                              /// 003f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00400
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00404
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00408
			 0x33333333,                                                  // 4 random values                             /// 0040c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00414
			 0x80000000,                                                  // -zero                                       /// 00418
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00420
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00424
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00428
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00430
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00434
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00438
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00440
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00448
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00458
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00464
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00468
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0046c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00470
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00474
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0047c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00480
			 0xcb000000,                                                  // -8388608.0                                  /// 00484
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00488
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00498
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004ac
			 0x7f800000,                                                  // inf                                         /// 004b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00504
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00518
			 0x80011111,                                                  // -9.7958E-41                                 /// 0051c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00528
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0052c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00530
			 0x33333333,                                                  // 4 random values                             /// 00534
			 0x0c780000,                                                  // Leading 1s:                                 /// 00538
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0053c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00544
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00548
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0054c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00550
			 0x0e000007,                                                  // Trailing 1s:                                /// 00554
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00558
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0055c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00560
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00564
			 0x33333333,                                                  // 4 random values                             /// 00568
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x4b000000,                                                  // 8388608.0                                   /// 00570
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00574
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00578
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x4b000000,                                                  // 8388608.0                                   /// 00580
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00584
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00590
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00594
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0059c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005a4
			 0x80000000,                                                  // -zero                                       /// 005a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xbf028f5c,                                                  // -0.51                                       /// 005b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x7f800000,                                                  // inf                                         /// 005f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00600
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0060c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00610
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x33333333,                                                  // 4 random values                             /// 00618
			 0x7f800000,                                                  // inf                                         /// 0061c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00620
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00628
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00630
			 0x0c700000,                                                  // Leading 1s:                                 /// 00634
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00638
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00640
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00644
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00648
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00658
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0065c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00660
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00668
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0066c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00670
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00674
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00678
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0067c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00688
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00690
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00698
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0069c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006e0
			 0x4b000000,                                                  // 8388608.0                                   /// 006e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00700
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00704
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00708
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00718
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0071c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00720
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00724
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00728
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0072c
			 0x4b000000,                                                  // 8388608.0                                   /// 00730
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00734
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00738
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0073c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00750
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00754
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00758
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00760
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00764
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00768
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0076c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00770
			 0x4b000000,                                                  // 8388608.0                                   /// 00774
			 0x00011111,                                                  // 9.7958E-41                                  /// 00778
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00788
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00790
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00794
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007a8
			 0xff800000,                                                  // -inf                                        /// 007ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0xbf028f5c,                                                  // -0.51                                       /// 007d0
			 0xbf028f5c,                                                  // -0.51                                       /// 007d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00800
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00804
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00808
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0080c
			 0xcb000000,                                                  // -8388608.0                                  /// 00810
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00814
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00818
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0081c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00824
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0082c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00838
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00840
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00844
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00848
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00850
			 0x55555555,                                                  // 4 random values                             /// 00854
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00860
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00864
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00868
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0086c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00874
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00878
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0087c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00880
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00884
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00888
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0088c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00890
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00894
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00898
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0089c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008e0
			 0xcb000000,                                                  // -8388608.0                                  /// 008e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00900
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0090c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00910
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00914
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00918
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0091c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00920
			 0x33333333,                                                  // 4 random values                             /// 00924
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00928
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0092c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00934
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x33333333,                                                  // 4 random values                             /// 0093c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00940
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00944
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0094c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00950
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00954
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0095c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00960
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00964
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00968
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0096c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00970
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00974
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00978
			 0xbf028f5c,                                                  // -0.51                                       /// 0097c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00980
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00984
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0098c
			 0x80000000,                                                  // -zero                                       /// 00990
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00994
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00998
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x3f028f5c,                                                  // 0.51                                        /// 009a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009ec
			 0xff800000,                                                  // -inf                                        /// 009f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a1c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a20
			 0x80000000,                                                  // -zero                                       /// 00a24
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a54
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a80
			 0xcb000000,                                                  // -8388608.0                                  /// 00a84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x3f028f5c,                                                  // 0.51                                        /// 00a8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aa0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x7fc00001,                                                  // signaling NaN                               /// 00aac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ab4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00abc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ac0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00acc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ad4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ad8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00adc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ae0
			 0x33333333,                                                  // 4 random values                             /// 00ae4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ae8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00af0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00af4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00afc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b68
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bb0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bc8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bd0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bd8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bdc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00be0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00be4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00be8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bf4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bf8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c18
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c4c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c50
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0xbf028f5c,                                                  // -0.51                                       /// 00c58
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c70
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c74
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c90
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ca4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ca8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cb4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cc0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ccc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cdc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ce0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ce8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cf8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cfc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d18
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d1c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d2c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d38
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d3c
			 0xff800000,                                                  // -inf                                        /// 00d40
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d50
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d5c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x3f028f5c,                                                  // 0.51                                        /// 00d78
			 0xffc00001,                                                  // -signaling NaN                              /// 00d7c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d88
			 0x4b000000,                                                  // 8388608.0                                   /// 00d8c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00da0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00da4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00db0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00db8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dbc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dc0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dcc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dd0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dd4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dd8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ddc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00de0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00de4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00de8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00df4
			 0x7fc00001,                                                  // signaling NaN                               /// 00df8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dfc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0xff800000,                                                  // -inf                                        /// 00e24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e2c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x7fc00001,                                                  // signaling NaN                               /// 00e58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e60
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e6c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e70
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e90
			 0xcb000000,                                                  // -8388608.0                                  /// 00e94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ea4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ea8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ebc
			 0xff800000,                                                  // -inf                                        /// 00ec0
			 0x80000000,                                                  // -zero                                       /// 00ec4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ec8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ed4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ed8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00edc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ee0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ee4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ee8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x00000000,                                                  // zero                                        /// 00ef0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ef8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0xffc00001,                                                  // -signaling NaN                              /// 00f00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f08
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f0c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f10
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f14
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f38
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f40
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f60
			 0xcb000000,                                                  // -8388608.0                                  /// 00f64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0xbf028f5c,                                                  // -0.51                                       /// 00f7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f80
			 0xff800000,                                                  // -inf                                        /// 00f84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f90
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00fa0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fa4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fb4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fb8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fc4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fc8
			 0x55555555,                                                  // 4 random values                             /// 00fcc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fd8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fe0
			 0x00000000,                                                  // zero                                        /// 00fe4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fe8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0xff7ffffe // max norm - 1ulp                               // SP - ve numbers                             /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00004
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00008
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0000c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00014
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0001c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00020
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0002c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0003c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00040
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00044
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00048
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00050
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00054
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00058
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0005c
			 0x7fc00001,                                                  // signaling NaN                               /// 00060
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00070
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00074
			 0xcb000000,                                                  // -8388608.0                                  /// 00078
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00080
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00084
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00090
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0009c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000a4
			 0xcb000000,                                                  // -8388608.0                                  /// 000a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000cc
			 0xffc00001,                                                  // -signaling NaN                              /// 000d0
			 0xff800000,                                                  // -inf                                        /// 000d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 000f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00100
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00104
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0010c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00110
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00118
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0011c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0012c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00130
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00134
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00138
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0013c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00140
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00144
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00148
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0014c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00150
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00158
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0015c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00164
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0016c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00170
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00178
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00180
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00188
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00194
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00198
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0019c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x7f800000,                                                  // inf                                         /// 001b0
			 0x80000000,                                                  // -zero                                       /// 001b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0xbf028f5c,                                                  // -0.51                                       /// 001d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001f0
			 0x33333333,                                                  // 4 random values                             /// 001f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x33333333,                                                  // 4 random values                             /// 001fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00200
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00208
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0020c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00218
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0021c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00220
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00224
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00228
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0022c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00230
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00234
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00238
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0023c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00240
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00244
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0024c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00250
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00254
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00258
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0025c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00260
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00264
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00268
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0026c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00270
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00274
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0027c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00280
			 0x7f800000,                                                  // inf                                         /// 00284
			 0x7fc00001,                                                  // signaling NaN                               /// 00288
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0028c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00294
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00298
			 0x80011111,                                                  // -9.7958E-41                                 /// 0029c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002a0
			 0xffc00001,                                                  // -signaling NaN                              /// 002a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002ec
			 0x7f800000,                                                  // inf                                         /// 002f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00300
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00304
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00314
			 0x4b000000,                                                  // 8388608.0                                   /// 00318
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0031c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00320
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00324
			 0x00000000,                                                  // zero                                        /// 00328
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0032c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00330
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00338
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0033c
			 0x33333333,                                                  // 4 random values                             /// 00340
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00344
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00348
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0034c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00350
			 0x33333333,                                                  // 4 random values                             /// 00354
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00364
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00370
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00374
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0037c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00380
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00384
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00388
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00390
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00394
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0039c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00400
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00404
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00408
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0040c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00414
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00418
			 0x7fc00001,                                                  // signaling NaN                               /// 0041c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00420
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00428
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x0c780000,                                                  // Leading 1s:                                 /// 00434
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00438
			 0x0c400000,                                                  // Leading 1s:                                 /// 0043c
			 0x7fc00001,                                                  // signaling NaN                               /// 00440
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00444
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00448
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0044c
			 0x4b000000,                                                  // 8388608.0                                   /// 00450
			 0xff800000,                                                  // -inf                                        /// 00454
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00458
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00460
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00464
			 0x0c780000,                                                  // Leading 1s:                                 /// 00468
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0046c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00478
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0047c
			 0x4b000000,                                                  // 8388608.0                                   /// 00480
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0048c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00490
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004c0
			 0x4b000000,                                                  // 8388608.0                                   /// 004c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 004d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00504
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00510
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x0c600000,                                                  // Leading 1s:                                 /// 00518
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0051c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00524
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00528
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0052c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00534
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00538
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00540
			 0x7fc00001,                                                  // signaling NaN                               /// 00544
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0054c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00554
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x80000000,                                                  // -zero                                       /// 00560
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0056c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00574
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00578
			 0x0e000007,                                                  // Trailing 1s:                                /// 0057c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00580
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00588
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00594
			 0x0c780000,                                                  // Leading 1s:                                 /// 00598
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0059c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 005a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005ac
			 0xcb000000,                                                  // -8388608.0                                  /// 005b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00600
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00604
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00608
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0060c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0c600000,                                                  // Leading 1s:                                 /// 00614
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00618
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00620
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00624
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00628
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0062c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00630
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00634
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00638
			 0x00000000,                                                  // zero                                        /// 0063c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00640
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00648
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00650
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00654
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80000000,                                                  // -zero                                       /// 0065c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00664
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00668
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0066c
			 0x00000000,                                                  // zero                                        /// 00670
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00678
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00684
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00688
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0068c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00690
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00694
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00698
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006b8
			 0x80000000,                                                  // -zero                                       /// 006bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006f8
			 0x80000000,                                                  // -zero                                       /// 006fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00704
			 0x0c400000,                                                  // Leading 1s:                                 /// 00708
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0070c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00710
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00714
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00718
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0071c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00728
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0xbf028f5c,                                                  // -0.51                                       /// 00730
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00738
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0073c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00740
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00748
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0074c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00750
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00754
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00758
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0075c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00764
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00768
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0076c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00770
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0xffc00001,                                                  // -signaling NaN                              /// 00780
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00788
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0xcb000000,                                                  // -8388608.0                                  /// 00794
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0079c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007a0
			 0xff800000,                                                  // -inf                                        /// 007a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d0
			 0xff800000,                                                  // -inf                                        /// 007d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 007d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007ec
			 0xff800000,                                                  // -inf                                        /// 007f0
			 0x4b000000,                                                  // 8388608.0                                   /// 007f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00800
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00804
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00808
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0080c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00810
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00814
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00824
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00828
			 0x0e000003,                                                  // Trailing 1s:                                /// 0082c
			 0x33333333,                                                  // 4 random values                             /// 00830
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00834
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00838
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0083c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00840
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00850
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00854
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00858
			 0xffc00001,                                                  // -signaling NaN                              /// 0085c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00860
			 0x4b000000,                                                  // 8388608.0                                   /// 00864
			 0x7f800000,                                                  // inf                                         /// 00868
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00874
			 0x0c780000,                                                  // Leading 1s:                                 /// 00878
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0087c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00884
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00888
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00894
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0089c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008b4
			 0x7fc00001,                                                  // signaling NaN                               /// 008b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008e0
			 0x3f028f5c,                                                  // 0.51                                        /// 008e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008e8
			 0xcb000000,                                                  // -8388608.0                                  /// 008ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 008fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00908
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0090c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00910
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00914
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00918
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0091c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x80011111,                                                  // -9.7958E-41                                 /// 00924
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00928
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0092c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00930
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00934
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0093c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00944
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00948
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0094c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x4b000000,                                                  // 8388608.0                                   /// 00958
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0095c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00964
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00968
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0096c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00970
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00978
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0097c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00980
			 0x0e000001,                                                  // Trailing 1s:                                /// 00984
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00988
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0098c
			 0x55555555,                                                  // 4 random values                             /// 00990
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00994
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00998
			 0xbf028f5c,                                                  // -0.51                                       /// 0099c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a04
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a3c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a50
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a88
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ab4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ab8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00abc
			 0x00000000,                                                  // zero                                        /// 00ac0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ac8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ad0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ad4
			 0xff800000,                                                  // -inf                                        /// 00ad8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ae4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00af4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00af8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00afc
			 0x80000000,                                                  // -zero                                       /// 00b00
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b28
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b44
			 0x7fc00001,                                                  // signaling NaN                               /// 00b48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b4c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b64
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b70
			 0x4b000000,                                                  // 8388608.0                                   /// 00b74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x4b000000,                                                  // 8388608.0                                   /// 00b88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b8c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ba0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ba4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ba8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bc0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bcc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bdc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00be0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bf0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bf4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bf8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bfc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c08
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c1c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c2c
			 0x7f800000,                                                  // inf                                         /// 00c30
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0xcb000000,                                                  // -8388608.0                                  /// 00c3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c50
			 0x7fc00001,                                                  // signaling NaN                               /// 00c54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c58
			 0xff800000,                                                  // -inf                                        /// 00c5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c78
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c80
			 0x7fc00001,                                                  // signaling NaN                               /// 00c84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cb8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cbc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cc0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cc4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cc8
			 0x55555555,                                                  // 4 random values                             /// 00ccc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cd4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cdc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cf0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cf8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cfc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d0c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d10
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d3c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d58
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d60
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x3f028f5c,                                                  // 0.51                                        /// 00d94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00da0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00db0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00db4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00db8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00de4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00de8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00df4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e58
			 0x00000000,                                                  // zero                                        /// 00e5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x55555555,                                                  // 4 random values                             /// 00e64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00000000,                                                  // zero                                        /// 00e78
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x4b000000,                                                  // 8388608.0                                   /// 00e8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ea0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00eb4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ebc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ec0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ec4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ec8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ed0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00eec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ef0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ef4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ef8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00efc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f20
			 0x7f800000,                                                  // inf                                         /// 00f24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f3c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f94
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f98
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fa0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fb4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fc8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fd4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fe0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fe4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe8
			 0x33333333,                                                  // 4 random values                             /// 00fec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ff8
			 0xAAAAAAAA                                                  // 4 random values                             /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0afaaae3, /// 0x0
			 0xd5aafd29, /// 0x4
			 0xe11873a1, /// 0x8
			 0xf51f2864, /// 0xc
			 0xbcb274d4, /// 0x10
			 0x4a70fc12, /// 0x14
			 0xe17e6a64, /// 0x18
			 0x76ac04f7, /// 0x1c
			 0xff320cfe, /// 0x20
			 0x03d71ffa, /// 0x24
			 0xcc6823ec, /// 0x28
			 0xa00095ea, /// 0x2c
			 0xc698672a, /// 0x30
			 0x0b4c1188, /// 0x34
			 0xa3c3456d, /// 0x38
			 0x4d128004, /// 0x3c
			 0xfff134cc, /// 0x40
			 0x8c2cac9b, /// 0x44
			 0x1f803b7a, /// 0x48
			 0x1246117c, /// 0x4c
			 0x632ad573, /// 0x50
			 0x87914bfc, /// 0x54
			 0x6780c4ed, /// 0x58
			 0xce1504c2, /// 0x5c
			 0x6f4d0b34, /// 0x60
			 0xb042d6e5, /// 0x64
			 0x6ef53f30, /// 0x68
			 0xd594b8a3, /// 0x6c
			 0x18d1130a, /// 0x70
			 0x7fca112e, /// 0x74
			 0x64f4b9d9, /// 0x78
			 0x3f8c9c40, /// 0x7c
			 0xcb682b82, /// 0x80
			 0x14c8d3b6, /// 0x84
			 0x146dbdef, /// 0x88
			 0x4a51a53e, /// 0x8c
			 0x98d67c03, /// 0x90
			 0x02ddb0d0, /// 0x94
			 0xbe4d3e18, /// 0x98
			 0x10e3aecf, /// 0x9c
			 0xd83b8398, /// 0xa0
			 0x4010c550, /// 0xa4
			 0xf4dd0b43, /// 0xa8
			 0xe2d607fd, /// 0xac
			 0xae038407, /// 0xb0
			 0x52f4ce23, /// 0xb4
			 0x34fa93a2, /// 0xb8
			 0xd47ef7ab, /// 0xbc
			 0xbcd2c74c, /// 0xc0
			 0x17222732, /// 0xc4
			 0x67b94c65, /// 0xc8
			 0xbc90ce87, /// 0xcc
			 0x23089384, /// 0xd0
			 0x92ca121d, /// 0xd4
			 0xbae1858b, /// 0xd8
			 0xaf6948cc, /// 0xdc
			 0x0d377a4e, /// 0xe0
			 0x65113447, /// 0xe4
			 0x73e5abcc, /// 0xe8
			 0x594a6728, /// 0xec
			 0x1a42a45f, /// 0xf0
			 0x7445f9b4, /// 0xf4
			 0xdbf7c8e6, /// 0xf8
			 0xacfd75ac, /// 0xfc
			 0x592aa5ec, /// 0x100
			 0x1d7d93b2, /// 0x104
			 0xcadb5712, /// 0x108
			 0x92fbe815, /// 0x10c
			 0x723d5fac, /// 0x110
			 0x7c94fccb, /// 0x114
			 0x191c6e6f, /// 0x118
			 0xd1371e49, /// 0x11c
			 0x15acd4ca, /// 0x120
			 0x670b4c89, /// 0x124
			 0x9b8a34de, /// 0x128
			 0x21a2bc49, /// 0x12c
			 0x2054088e, /// 0x130
			 0xf166ed22, /// 0x134
			 0x9e3a22a8, /// 0x138
			 0xa01c84be, /// 0x13c
			 0x4701c6a2, /// 0x140
			 0x0a050ea2, /// 0x144
			 0x69104eae, /// 0x148
			 0xde6e7f6b, /// 0x14c
			 0x0e985345, /// 0x150
			 0x22b67545, /// 0x154
			 0x956258e0, /// 0x158
			 0x03f865f1, /// 0x15c
			 0x570cce1a, /// 0x160
			 0x06e7d9cd, /// 0x164
			 0xfc56ea5f, /// 0x168
			 0xcea41cf3, /// 0x16c
			 0x5d59532d, /// 0x170
			 0xb7453001, /// 0x174
			 0x39c41eea, /// 0x178
			 0xd6df9dad, /// 0x17c
			 0xd6adf979, /// 0x180
			 0xbefdff5e, /// 0x184
			 0x2df38b56, /// 0x188
			 0x79a3d510, /// 0x18c
			 0x83ee0f69, /// 0x190
			 0xd78f9b7b, /// 0x194
			 0xb30b5260, /// 0x198
			 0x9aaf7c47, /// 0x19c
			 0x7ed62478, /// 0x1a0
			 0xadd76eeb, /// 0x1a4
			 0x2d379c0b, /// 0x1a8
			 0x1a65ae11, /// 0x1ac
			 0x94cb458e, /// 0x1b0
			 0x8ab06930, /// 0x1b4
			 0x20b60397, /// 0x1b8
			 0x23b188bd, /// 0x1bc
			 0x4103ecf7, /// 0x1c0
			 0xc7c43679, /// 0x1c4
			 0x08afedfa, /// 0x1c8
			 0xcbda8654, /// 0x1cc
			 0xa9c8a9ee, /// 0x1d0
			 0xea990eef, /// 0x1d4
			 0xd5a3d3ff, /// 0x1d8
			 0xfd199982, /// 0x1dc
			 0x8a6b039e, /// 0x1e0
			 0x6f651aa7, /// 0x1e4
			 0x5e48b2e1, /// 0x1e8
			 0x1b03016d, /// 0x1ec
			 0x94c184ec, /// 0x1f0
			 0x9abaf78a, /// 0x1f4
			 0x7ca8a25e, /// 0x1f8
			 0x71a91a07, /// 0x1fc
			 0x2c24bcaa, /// 0x200
			 0x70632899, /// 0x204
			 0xec766513, /// 0x208
			 0x3dbd0a9d, /// 0x20c
			 0x45795495, /// 0x210
			 0xa8072b36, /// 0x214
			 0x94eb80e7, /// 0x218
			 0x8001bb5e, /// 0x21c
			 0x396e7afa, /// 0x220
			 0xff32d471, /// 0x224
			 0xee0314c0, /// 0x228
			 0xb0645635, /// 0x22c
			 0x447b2f8b, /// 0x230
			 0x0409bafc, /// 0x234
			 0x236dedd5, /// 0x238
			 0x22e7e73e, /// 0x23c
			 0xda5f8d2a, /// 0x240
			 0x3c2cf8da, /// 0x244
			 0xbd27c791, /// 0x248
			 0x13f854a2, /// 0x24c
			 0x6db1e72a, /// 0x250
			 0xd294d939, /// 0x254
			 0x9f21f6f1, /// 0x258
			 0x2fb0241f, /// 0x25c
			 0x1a58a5fa, /// 0x260
			 0xdca009c0, /// 0x264
			 0x39a8457a, /// 0x268
			 0xfc3e68ea, /// 0x26c
			 0xe36cf2eb, /// 0x270
			 0x351bbe64, /// 0x274
			 0x0469fc79, /// 0x278
			 0xb93af8c0, /// 0x27c
			 0xc00b1da5, /// 0x280
			 0x40201c76, /// 0x284
			 0x8a447207, /// 0x288
			 0x34cb5247, /// 0x28c
			 0x7b9eb4e4, /// 0x290
			 0x1265f7cb, /// 0x294
			 0x013bc57b, /// 0x298
			 0x3eec0788, /// 0x29c
			 0xa140047f, /// 0x2a0
			 0xb6332d09, /// 0x2a4
			 0x5c9077e4, /// 0x2a8
			 0x63388707, /// 0x2ac
			 0x17142146, /// 0x2b0
			 0x42e4dc60, /// 0x2b4
			 0x4b94abb0, /// 0x2b8
			 0x733c0d90, /// 0x2bc
			 0x2a070fc9, /// 0x2c0
			 0x45868798, /// 0x2c4
			 0xfaa5f897, /// 0x2c8
			 0x89d10a62, /// 0x2cc
			 0x81fd086c, /// 0x2d0
			 0x998a40a1, /// 0x2d4
			 0x6b45b651, /// 0x2d8
			 0x624dcd50, /// 0x2dc
			 0x3239316c, /// 0x2e0
			 0x5c6433de, /// 0x2e4
			 0xe802005c, /// 0x2e8
			 0x76533fae, /// 0x2ec
			 0x9c9e83c4, /// 0x2f0
			 0x851daaab, /// 0x2f4
			 0x28ed7962, /// 0x2f8
			 0x41e87a16, /// 0x2fc
			 0x644ced3f, /// 0x300
			 0xb968c000, /// 0x304
			 0x0cf52194, /// 0x308
			 0x1ba91525, /// 0x30c
			 0x5c376826, /// 0x310
			 0xe3627432, /// 0x314
			 0xa37d56ed, /// 0x318
			 0xb712dc27, /// 0x31c
			 0x32410047, /// 0x320
			 0x1850a909, /// 0x324
			 0x75d2c7bd, /// 0x328
			 0x7c205345, /// 0x32c
			 0x180e9123, /// 0x330
			 0x8a92c580, /// 0x334
			 0xce1b53c8, /// 0x338
			 0x18257254, /// 0x33c
			 0x3c242f3b, /// 0x340
			 0xdd02e14e, /// 0x344
			 0x2dd974c8, /// 0x348
			 0x0f7c14ff, /// 0x34c
			 0xb852e38b, /// 0x350
			 0xb938571a, /// 0x354
			 0x5378b53b, /// 0x358
			 0x7eb26a79, /// 0x35c
			 0x85618a81, /// 0x360
			 0xe9746f4c, /// 0x364
			 0xd50afb39, /// 0x368
			 0x9a388a58, /// 0x36c
			 0x2b6e8362, /// 0x370
			 0x73564ec2, /// 0x374
			 0xafbef83b, /// 0x378
			 0x8c562190, /// 0x37c
			 0x4dd7734d, /// 0x380
			 0x68b357a5, /// 0x384
			 0x18dd5c79, /// 0x388
			 0x3e1178d4, /// 0x38c
			 0x7a102c2d, /// 0x390
			 0x9b744199, /// 0x394
			 0xba63c4e4, /// 0x398
			 0xe81e8b8b, /// 0x39c
			 0x9eb0318c, /// 0x3a0
			 0x90353dde, /// 0x3a4
			 0xb7772113, /// 0x3a8
			 0x40494b96, /// 0x3ac
			 0xd1b82e5c, /// 0x3b0
			 0x87d0cf95, /// 0x3b4
			 0x45cf2612, /// 0x3b8
			 0x7d6b9b58, /// 0x3bc
			 0x43b777f9, /// 0x3c0
			 0x68c896d8, /// 0x3c4
			 0x05e610a3, /// 0x3c8
			 0xe0ea122b, /// 0x3cc
			 0xf2a679c9, /// 0x3d0
			 0x863a551d, /// 0x3d4
			 0x8ef8eb50, /// 0x3d8
			 0xc212255d, /// 0x3dc
			 0x5e0c4d32, /// 0x3e0
			 0x8a492dd1, /// 0x3e4
			 0xc3e54d54, /// 0x3e8
			 0x9665ce44, /// 0x3ec
			 0xee2c2900, /// 0x3f0
			 0xc89ed28b, /// 0x3f4
			 0xa540a628, /// 0x3f8
			 0x146505e2, /// 0x3fc
			 0xfba9f28c, /// 0x400
			 0xa7e48d27, /// 0x404
			 0x0eb9b155, /// 0x408
			 0xf346fd7a, /// 0x40c
			 0xa34f8592, /// 0x410
			 0x7213faf8, /// 0x414
			 0x6e28334c, /// 0x418
			 0x573b6819, /// 0x41c
			 0x877fec30, /// 0x420
			 0xdbb9acf0, /// 0x424
			 0xe2e1423c, /// 0x428
			 0x26e2da35, /// 0x42c
			 0x3dd61edc, /// 0x430
			 0xe202f4e9, /// 0x434
			 0xbb748a7a, /// 0x438
			 0xbfa58c85, /// 0x43c
			 0xa4c1975a, /// 0x440
			 0xd931db7c, /// 0x444
			 0xa14b19ff, /// 0x448
			 0xa37ecf9a, /// 0x44c
			 0xefa706ea, /// 0x450
			 0x336d51db, /// 0x454
			 0xd2bea8cd, /// 0x458
			 0x818bc1ea, /// 0x45c
			 0x4033440c, /// 0x460
			 0xa967bfc0, /// 0x464
			 0x06770471, /// 0x468
			 0xfaf5667c, /// 0x46c
			 0x5f4999f2, /// 0x470
			 0x1c3dccd0, /// 0x474
			 0xe522950e, /// 0x478
			 0x97479b0f, /// 0x47c
			 0xd8946d22, /// 0x480
			 0xf620c616, /// 0x484
			 0xee56bc16, /// 0x488
			 0x650d2460, /// 0x48c
			 0xe15cc523, /// 0x490
			 0x6eebe4e6, /// 0x494
			 0x610ce35e, /// 0x498
			 0x5fced384, /// 0x49c
			 0x78fdfd39, /// 0x4a0
			 0xca3298d8, /// 0x4a4
			 0x53a24d3b, /// 0x4a8
			 0x6523a92a, /// 0x4ac
			 0xcba71dab, /// 0x4b0
			 0xa6f9e83b, /// 0x4b4
			 0xaf9f829b, /// 0x4b8
			 0x68e2cd6f, /// 0x4bc
			 0x47522403, /// 0x4c0
			 0xf52eed1d, /// 0x4c4
			 0xbcd5dcbb, /// 0x4c8
			 0x2a58acc4, /// 0x4cc
			 0x8bc70ca0, /// 0x4d0
			 0xa6275862, /// 0x4d4
			 0x71768f72, /// 0x4d8
			 0x5e333177, /// 0x4dc
			 0xb710f4bf, /// 0x4e0
			 0x4549ce30, /// 0x4e4
			 0x6b630cd0, /// 0x4e8
			 0x03bb42da, /// 0x4ec
			 0x1552bc4c, /// 0x4f0
			 0x1786f163, /// 0x4f4
			 0x725ba2ed, /// 0x4f8
			 0xbe186eb9, /// 0x4fc
			 0x351608d2, /// 0x500
			 0x85c4e8d3, /// 0x504
			 0x71fca59b, /// 0x508
			 0x3c9d32c5, /// 0x50c
			 0x4e50104b, /// 0x510
			 0xd690adc0, /// 0x514
			 0x0f0cf87b, /// 0x518
			 0x46e366a4, /// 0x51c
			 0xb8e6dbbf, /// 0x520
			 0x64c59b65, /// 0x524
			 0xfe0bd4ab, /// 0x528
			 0x2997c5a1, /// 0x52c
			 0x5480a0e4, /// 0x530
			 0x4a76e103, /// 0x534
			 0xcaab6c2d, /// 0x538
			 0xabec127b, /// 0x53c
			 0x173db40f, /// 0x540
			 0xa53dd7e2, /// 0x544
			 0x7c699ce7, /// 0x548
			 0xb8659e00, /// 0x54c
			 0x12079bf6, /// 0x550
			 0xbc52ae37, /// 0x554
			 0x07877ef2, /// 0x558
			 0x7a1e18d3, /// 0x55c
			 0x259cb776, /// 0x560
			 0xe63e7764, /// 0x564
			 0x54c18109, /// 0x568
			 0xeee741b3, /// 0x56c
			 0x73513bed, /// 0x570
			 0x8f7833f6, /// 0x574
			 0xf992f137, /// 0x578
			 0xb7c88f2c, /// 0x57c
			 0x2a729d75, /// 0x580
			 0x04aec5fa, /// 0x584
			 0xa1a8299b, /// 0x588
			 0x7e0b4e8b, /// 0x58c
			 0x6dc73b5e, /// 0x590
			 0xc5994427, /// 0x594
			 0x82852b49, /// 0x598
			 0xc7fca765, /// 0x59c
			 0x20724a47, /// 0x5a0
			 0xdf03124c, /// 0x5a4
			 0x255d6041, /// 0x5a8
			 0x454127d9, /// 0x5ac
			 0xbf34f8cd, /// 0x5b0
			 0x5f601739, /// 0x5b4
			 0x37426327, /// 0x5b8
			 0xb576768f, /// 0x5bc
			 0x2fad4777, /// 0x5c0
			 0x7a6cdacc, /// 0x5c4
			 0x65bcf2a3, /// 0x5c8
			 0xf29b6d33, /// 0x5cc
			 0xd011f415, /// 0x5d0
			 0x33c4cfb0, /// 0x5d4
			 0x0e6f7270, /// 0x5d8
			 0xa51add83, /// 0x5dc
			 0x0619b24a, /// 0x5e0
			 0x9cec3308, /// 0x5e4
			 0x69b29170, /// 0x5e8
			 0x8234b403, /// 0x5ec
			 0x2aa042c6, /// 0x5f0
			 0x603f07b1, /// 0x5f4
			 0x5df27f07, /// 0x5f8
			 0x0c9361e8, /// 0x5fc
			 0xcdf33e46, /// 0x600
			 0xa017134f, /// 0x604
			 0x34a73ba3, /// 0x608
			 0x2450b270, /// 0x60c
			 0x873ea785, /// 0x610
			 0xeaa0ea17, /// 0x614
			 0xf8e69847, /// 0x618
			 0xe12d2859, /// 0x61c
			 0xc1098496, /// 0x620
			 0xecbc8741, /// 0x624
			 0x3949bccb, /// 0x628
			 0x4b17018f, /// 0x62c
			 0x96803378, /// 0x630
			 0x33b6b683, /// 0x634
			 0xc5262f0f, /// 0x638
			 0x9139bae9, /// 0x63c
			 0xde7a6583, /// 0x640
			 0x85e76d11, /// 0x644
			 0x6a28fee7, /// 0x648
			 0x2c5b54a4, /// 0x64c
			 0x6121d146, /// 0x650
			 0x58694eef, /// 0x654
			 0xe136945e, /// 0x658
			 0xc055e570, /// 0x65c
			 0x26d07921, /// 0x660
			 0xed8e71ce, /// 0x664
			 0x9905806e, /// 0x668
			 0xc9c78668, /// 0x66c
			 0x1cbcb965, /// 0x670
			 0x1d589ade, /// 0x674
			 0x20f9380e, /// 0x678
			 0xe200b481, /// 0x67c
			 0x228b677d, /// 0x680
			 0x599f84d0, /// 0x684
			 0x5100a255, /// 0x688
			 0xf499f4e9, /// 0x68c
			 0x41764c6e, /// 0x690
			 0xd76a3d25, /// 0x694
			 0x47bab188, /// 0x698
			 0x2bed3f10, /// 0x69c
			 0x264157cd, /// 0x6a0
			 0x68db561c, /// 0x6a4
			 0xf86c640e, /// 0x6a8
			 0xd1bdfdf3, /// 0x6ac
			 0xd5213df1, /// 0x6b0
			 0x8cf6555e, /// 0x6b4
			 0x80dfa81a, /// 0x6b8
			 0x58feba12, /// 0x6bc
			 0x9290339c, /// 0x6c0
			 0x7e10b974, /// 0x6c4
			 0xe65d3dcd, /// 0x6c8
			 0xe5f424d8, /// 0x6cc
			 0xe07f3b4e, /// 0x6d0
			 0x16608616, /// 0x6d4
			 0x6b4257a7, /// 0x6d8
			 0x7f055dc0, /// 0x6dc
			 0x3921a659, /// 0x6e0
			 0xc0b76bab, /// 0x6e4
			 0x46ef2919, /// 0x6e8
			 0x6b5c7bcc, /// 0x6ec
			 0x9047b7ac, /// 0x6f0
			 0x485fb5a8, /// 0x6f4
			 0x9b6b4486, /// 0x6f8
			 0x8e89791c, /// 0x6fc
			 0x0dd241fb, /// 0x700
			 0xd611050f, /// 0x704
			 0x51a27878, /// 0x708
			 0x0bad5a9b, /// 0x70c
			 0xb01edec3, /// 0x710
			 0x1a6ed92f, /// 0x714
			 0xb57fa07d, /// 0x718
			 0x14a06193, /// 0x71c
			 0x3d46582b, /// 0x720
			 0xc5ca6aac, /// 0x724
			 0x3ba8c167, /// 0x728
			 0xb443b0c0, /// 0x72c
			 0x57150712, /// 0x730
			 0xd2b2b684, /// 0x734
			 0x02e0bf39, /// 0x738
			 0xdaba6d3d, /// 0x73c
			 0x0d17ccc2, /// 0x740
			 0x3a648013, /// 0x744
			 0x587813dc, /// 0x748
			 0x8e62baa6, /// 0x74c
			 0x0a3be695, /// 0x750
			 0x3b7fb43c, /// 0x754
			 0x40e8e0ea, /// 0x758
			 0xf45301db, /// 0x75c
			 0x4ebd38b1, /// 0x760
			 0xbdec854b, /// 0x764
			 0x44c63944, /// 0x768
			 0xb00bc58d, /// 0x76c
			 0x1a1d7ae6, /// 0x770
			 0xe6628d9d, /// 0x774
			 0x377ffb3a, /// 0x778
			 0x8eb1fead, /// 0x77c
			 0x07fb7cb6, /// 0x780
			 0x64b8d334, /// 0x784
			 0x8dd1e707, /// 0x788
			 0xb7329091, /// 0x78c
			 0x1d2f62fa, /// 0x790
			 0x6b9827b4, /// 0x794
			 0x44532746, /// 0x798
			 0x3ae2e72a, /// 0x79c
			 0x09fcf3d7, /// 0x7a0
			 0xc2c159b9, /// 0x7a4
			 0xc9d03e66, /// 0x7a8
			 0x7b243682, /// 0x7ac
			 0x4054f3f1, /// 0x7b0
			 0xdbede217, /// 0x7b4
			 0x02ec648a, /// 0x7b8
			 0x87beee53, /// 0x7bc
			 0x683a9dd8, /// 0x7c0
			 0x15967b22, /// 0x7c4
			 0x05d81878, /// 0x7c8
			 0xe6a8d687, /// 0x7cc
			 0xda10febb, /// 0x7d0
			 0xb2bce780, /// 0x7d4
			 0xa90f1fca, /// 0x7d8
			 0x74ef7161, /// 0x7dc
			 0x84d07098, /// 0x7e0
			 0xc6118e29, /// 0x7e4
			 0x6a8ca938, /// 0x7e8
			 0x7cb5ef5a, /// 0x7ec
			 0x1dab2e3b, /// 0x7f0
			 0x578ec22d, /// 0x7f4
			 0x5c1f5bde, /// 0x7f8
			 0x44b6624b, /// 0x7fc
			 0x864d71c7, /// 0x800
			 0x3000808a, /// 0x804
			 0xa1105bc0, /// 0x808
			 0xad6f7c8c, /// 0x80c
			 0x586b4aa4, /// 0x810
			 0x55b442a6, /// 0x814
			 0xbe970798, /// 0x818
			 0x880a1021, /// 0x81c
			 0x4e4572c3, /// 0x820
			 0xe1eeab74, /// 0x824
			 0x8ca198b3, /// 0x828
			 0xf4f3be90, /// 0x82c
			 0xa9a6ad83, /// 0x830
			 0x59ff506d, /// 0x834
			 0x45ec1c82, /// 0x838
			 0xb389ad3d, /// 0x83c
			 0x39b018f8, /// 0x840
			 0x08735af2, /// 0x844
			 0x36e88883, /// 0x848
			 0x53d6ba36, /// 0x84c
			 0x53711e7a, /// 0x850
			 0x78676792, /// 0x854
			 0xc16576b2, /// 0x858
			 0x97a054be, /// 0x85c
			 0x546a868f, /// 0x860
			 0x8abdac8b, /// 0x864
			 0x93312726, /// 0x868
			 0xfe34bbe0, /// 0x86c
			 0x573e5cb9, /// 0x870
			 0xbda4428d, /// 0x874
			 0x14c634ca, /// 0x878
			 0x0e87e67e, /// 0x87c
			 0x4a4a302d, /// 0x880
			 0x7826b58f, /// 0x884
			 0xc6b4e13f, /// 0x888
			 0x56b8f581, /// 0x88c
			 0x6fa8a27e, /// 0x890
			 0x41a769f9, /// 0x894
			 0xbd953562, /// 0x898
			 0x44558d02, /// 0x89c
			 0x3c1c8570, /// 0x8a0
			 0x93a5e801, /// 0x8a4
			 0x2595f34c, /// 0x8a8
			 0xf41b8999, /// 0x8ac
			 0xf679cf07, /// 0x8b0
			 0x1bc0d6ca, /// 0x8b4
			 0x9651f59c, /// 0x8b8
			 0xeb52b2d6, /// 0x8bc
			 0x397d3f98, /// 0x8c0
			 0x3feb0a43, /// 0x8c4
			 0xba414fb5, /// 0x8c8
			 0x232b4706, /// 0x8cc
			 0x1d1cf635, /// 0x8d0
			 0xd545f360, /// 0x8d4
			 0xaee016ea, /// 0x8d8
			 0x945c58d8, /// 0x8dc
			 0x6694bbae, /// 0x8e0
			 0x32b97443, /// 0x8e4
			 0x5dd9511b, /// 0x8e8
			 0xaba2ec4f, /// 0x8ec
			 0xd4e5f96e, /// 0x8f0
			 0x947d1326, /// 0x8f4
			 0x53912605, /// 0x8f8
			 0x9c14a4ba, /// 0x8fc
			 0x7703a9e5, /// 0x900
			 0x9dc917e7, /// 0x904
			 0x68be274f, /// 0x908
			 0xe59a7a6c, /// 0x90c
			 0x2f79da38, /// 0x910
			 0x16745e5f, /// 0x914
			 0x06d474af, /// 0x918
			 0x6cfbe47f, /// 0x91c
			 0x9476ffda, /// 0x920
			 0xb7b3259f, /// 0x924
			 0xf69032f2, /// 0x928
			 0x691d2a65, /// 0x92c
			 0xa37e9a60, /// 0x930
			 0x4e6d51c9, /// 0x934
			 0xab286907, /// 0x938
			 0xa242702f, /// 0x93c
			 0xd1f53aa2, /// 0x940
			 0x8df69721, /// 0x944
			 0x89d5095e, /// 0x948
			 0x5a6fa5f3, /// 0x94c
			 0x77b1a7a7, /// 0x950
			 0xce9d48a3, /// 0x954
			 0x2e187311, /// 0x958
			 0x25dc53cb, /// 0x95c
			 0xe8090ba5, /// 0x960
			 0xb80198f7, /// 0x964
			 0x5a2d45eb, /// 0x968
			 0x8d3501e5, /// 0x96c
			 0x3b4ef62a, /// 0x970
			 0x079f5ba0, /// 0x974
			 0x7ab62d65, /// 0x978
			 0x43b8db7d, /// 0x97c
			 0xa0a34694, /// 0x980
			 0xad1c2e07, /// 0x984
			 0xb75b4222, /// 0x988
			 0x73ad9004, /// 0x98c
			 0x205ee0c1, /// 0x990
			 0xdd30f49d, /// 0x994
			 0x0ffe0bd5, /// 0x998
			 0x51716ec9, /// 0x99c
			 0x79d66abe, /// 0x9a0
			 0xd90a2996, /// 0x9a4
			 0xecdbb477, /// 0x9a8
			 0x2df234a1, /// 0x9ac
			 0x7648ee41, /// 0x9b0
			 0xf2003d0b, /// 0x9b4
			 0x46fd590a, /// 0x9b8
			 0xd766aa4d, /// 0x9bc
			 0x25fc8a3d, /// 0x9c0
			 0xd3d99f3f, /// 0x9c4
			 0xcd2b6eaa, /// 0x9c8
			 0x0ac43de1, /// 0x9cc
			 0x58bef428, /// 0x9d0
			 0x725d0abe, /// 0x9d4
			 0x62ddd6c1, /// 0x9d8
			 0x370e8dd7, /// 0x9dc
			 0xf020553f, /// 0x9e0
			 0x88bbb928, /// 0x9e4
			 0x92674d11, /// 0x9e8
			 0x3bf23079, /// 0x9ec
			 0x71049ac0, /// 0x9f0
			 0xf0afd01c, /// 0x9f4
			 0xd8db3d1b, /// 0x9f8
			 0x39fefac9, /// 0x9fc
			 0xc9db5a25, /// 0xa00
			 0xcace31d7, /// 0xa04
			 0x97af8f17, /// 0xa08
			 0x1b76d08c, /// 0xa0c
			 0xfa82bfad, /// 0xa10
			 0x4e5c7ba7, /// 0xa14
			 0xf1db9e78, /// 0xa18
			 0x4fa17650, /// 0xa1c
			 0x9d96eee6, /// 0xa20
			 0x130a0e06, /// 0xa24
			 0x05455f61, /// 0xa28
			 0x67e78b6a, /// 0xa2c
			 0x4b4e938a, /// 0xa30
			 0x9bcdc6c0, /// 0xa34
			 0xecbd8ea2, /// 0xa38
			 0x1552195a, /// 0xa3c
			 0x7bfb38ec, /// 0xa40
			 0xce0d6192, /// 0xa44
			 0xf2f3aac2, /// 0xa48
			 0x366c206d, /// 0xa4c
			 0x84064526, /// 0xa50
			 0x8c2107e6, /// 0xa54
			 0x3b52b011, /// 0xa58
			 0x3f344907, /// 0xa5c
			 0x51209ea8, /// 0xa60
			 0xe7bba7b4, /// 0xa64
			 0x7ffbec32, /// 0xa68
			 0x76a418fc, /// 0xa6c
			 0x41f57206, /// 0xa70
			 0x2369855b, /// 0xa74
			 0x83140610, /// 0xa78
			 0x75d2c879, /// 0xa7c
			 0xae2c1559, /// 0xa80
			 0x96ca4803, /// 0xa84
			 0xd22d4eda, /// 0xa88
			 0x78b75465, /// 0xa8c
			 0x6b4d6459, /// 0xa90
			 0x944edebf, /// 0xa94
			 0x0563f165, /// 0xa98
			 0x81aa0c78, /// 0xa9c
			 0x0a32511b, /// 0xaa0
			 0x70e97932, /// 0xaa4
			 0x44291e32, /// 0xaa8
			 0xdd5399b7, /// 0xaac
			 0xc1285ff1, /// 0xab0
			 0x02990fed, /// 0xab4
			 0x711bf411, /// 0xab8
			 0xd39cd7e4, /// 0xabc
			 0xf788dc99, /// 0xac0
			 0x3cb9697f, /// 0xac4
			 0x33fde606, /// 0xac8
			 0x34e64e1b, /// 0xacc
			 0x27b54b4e, /// 0xad0
			 0x51543909, /// 0xad4
			 0x045ec732, /// 0xad8
			 0xf1cf7a53, /// 0xadc
			 0xf482227b, /// 0xae0
			 0x48d3d816, /// 0xae4
			 0xa07b1016, /// 0xae8
			 0x7e450661, /// 0xaec
			 0x97dcc87c, /// 0xaf0
			 0x44007e5a, /// 0xaf4
			 0x141797dd, /// 0xaf8
			 0xcb741128, /// 0xafc
			 0x31019c12, /// 0xb00
			 0x66a2c3e8, /// 0xb04
			 0xf7e0d4d5, /// 0xb08
			 0xd2d100a7, /// 0xb0c
			 0xad0fe8a2, /// 0xb10
			 0x8f32baea, /// 0xb14
			 0x0406af42, /// 0xb18
			 0xe41578dc, /// 0xb1c
			 0xe2b9de1b, /// 0xb20
			 0x8b8dd66c, /// 0xb24
			 0x13a895a9, /// 0xb28
			 0xbdf569cb, /// 0xb2c
			 0x4f4406a8, /// 0xb30
			 0x8fbcc005, /// 0xb34
			 0xafd7f74b, /// 0xb38
			 0xd6e221c8, /// 0xb3c
			 0x5af34795, /// 0xb40
			 0x005022a2, /// 0xb44
			 0x55d5bf63, /// 0xb48
			 0x0645eed0, /// 0xb4c
			 0x8a60f622, /// 0xb50
			 0xfa5aa89a, /// 0xb54
			 0x5c677ba9, /// 0xb58
			 0x2d0b73b0, /// 0xb5c
			 0x42b2f3cc, /// 0xb60
			 0x9bd03c74, /// 0xb64
			 0x4c4d3f4a, /// 0xb68
			 0xb9e69cb9, /// 0xb6c
			 0x922909ce, /// 0xb70
			 0x372dbb26, /// 0xb74
			 0x86d54cf3, /// 0xb78
			 0x4e91c6d4, /// 0xb7c
			 0x7e5a0f0f, /// 0xb80
			 0x0a7da43b, /// 0xb84
			 0x6ac7635c, /// 0xb88
			 0x0e629051, /// 0xb8c
			 0x8c6f9b7e, /// 0xb90
			 0x6691a16b, /// 0xb94
			 0xf35a9ad2, /// 0xb98
			 0x1c659896, /// 0xb9c
			 0x20102449, /// 0xba0
			 0x394c0879, /// 0xba4
			 0xc1c61951, /// 0xba8
			 0x45ced322, /// 0xbac
			 0x7a6ce334, /// 0xbb0
			 0xc00f980c, /// 0xbb4
			 0xc8ca4013, /// 0xbb8
			 0x590e0eed, /// 0xbbc
			 0x85a9f970, /// 0xbc0
			 0xeddbf3a3, /// 0xbc4
			 0x0a232086, /// 0xbc8
			 0xaeec23ff, /// 0xbcc
			 0xf5191bfa, /// 0xbd0
			 0xf266046a, /// 0xbd4
			 0x661a83c8, /// 0xbd8
			 0xd3cfd9e4, /// 0xbdc
			 0x98da07f6, /// 0xbe0
			 0x3fc1997f, /// 0xbe4
			 0x9c9d8a2e, /// 0xbe8
			 0xcf13d4ab, /// 0xbec
			 0xab84ea04, /// 0xbf0
			 0x29a37bf1, /// 0xbf4
			 0x33cf34b4, /// 0xbf8
			 0x862451b7, /// 0xbfc
			 0xc326a4b3, /// 0xc00
			 0x98ec744e, /// 0xc04
			 0x24f33eb9, /// 0xc08
			 0x29a3616c, /// 0xc0c
			 0x3417d0f9, /// 0xc10
			 0xb7254911, /// 0xc14
			 0x42fa93e3, /// 0xc18
			 0x4d997750, /// 0xc1c
			 0x8e257580, /// 0xc20
			 0x40e91f12, /// 0xc24
			 0xe731cdf5, /// 0xc28
			 0x39a6fc4d, /// 0xc2c
			 0x68e76d12, /// 0xc30
			 0x7fdb1020, /// 0xc34
			 0x51835ee2, /// 0xc38
			 0x13f38171, /// 0xc3c
			 0x76903328, /// 0xc40
			 0xf22db4f2, /// 0xc44
			 0x00f27ff0, /// 0xc48
			 0xa6251188, /// 0xc4c
			 0xee511374, /// 0xc50
			 0xfbb7a76f, /// 0xc54
			 0x83ee3af5, /// 0xc58
			 0xc0141b93, /// 0xc5c
			 0xbb5edadb, /// 0xc60
			 0x0f358c8b, /// 0xc64
			 0xb896f3f3, /// 0xc68
			 0x0ce1f758, /// 0xc6c
			 0x80f0cd2f, /// 0xc70
			 0x6b532136, /// 0xc74
			 0x90c3a53a, /// 0xc78
			 0xd9c0f605, /// 0xc7c
			 0x813542c3, /// 0xc80
			 0xd26477e8, /// 0xc84
			 0xcbd574eb, /// 0xc88
			 0x095f5ad5, /// 0xc8c
			 0xbc7a8fa8, /// 0xc90
			 0x872fffcc, /// 0xc94
			 0xe9843e41, /// 0xc98
			 0x8520319e, /// 0xc9c
			 0xdbb671b0, /// 0xca0
			 0x50836a68, /// 0xca4
			 0xb533d416, /// 0xca8
			 0x4aa72a72, /// 0xcac
			 0x4af9a32c, /// 0xcb0
			 0x173e2d05, /// 0xcb4
			 0xeaa9d66e, /// 0xcb8
			 0xc8782360, /// 0xcbc
			 0xc6134cb1, /// 0xcc0
			 0x79266d12, /// 0xcc4
			 0x7b40c119, /// 0xcc8
			 0xbc8fa578, /// 0xccc
			 0xd856be26, /// 0xcd0
			 0x706156d4, /// 0xcd4
			 0x91cf053b, /// 0xcd8
			 0x28ec06c1, /// 0xcdc
			 0xd6a38190, /// 0xce0
			 0x4edd67ea, /// 0xce4
			 0xb7dc8151, /// 0xce8
			 0xfdfc4a7c, /// 0xcec
			 0x2731da17, /// 0xcf0
			 0x2fb7f1c6, /// 0xcf4
			 0xb102e720, /// 0xcf8
			 0xf9889c04, /// 0xcfc
			 0x6867fef8, /// 0xd00
			 0xc383f004, /// 0xd04
			 0x3997f642, /// 0xd08
			 0xa09c80f3, /// 0xd0c
			 0x5c8e2366, /// 0xd10
			 0xc9f07dc8, /// 0xd14
			 0xa3712e84, /// 0xd18
			 0x662597d6, /// 0xd1c
			 0x42c9504a, /// 0xd20
			 0xc643a6f5, /// 0xd24
			 0xd2586bcf, /// 0xd28
			 0x8913084f, /// 0xd2c
			 0x21bd51cd, /// 0xd30
			 0x88a8ad44, /// 0xd34
			 0x0181b7fb, /// 0xd38
			 0x3cf83110, /// 0xd3c
			 0xb524f6b3, /// 0xd40
			 0x1163b1d2, /// 0xd44
			 0x2a251ee1, /// 0xd48
			 0xe605e51f, /// 0xd4c
			 0xe97efbbb, /// 0xd50
			 0x622bd0fe, /// 0xd54
			 0x0c72d6c1, /// 0xd58
			 0xb8a45df1, /// 0xd5c
			 0xb2ed4779, /// 0xd60
			 0x1d5eb262, /// 0xd64
			 0xc2108fab, /// 0xd68
			 0xca822650, /// 0xd6c
			 0x256d65d1, /// 0xd70
			 0xc5fa8161, /// 0xd74
			 0x44969e9b, /// 0xd78
			 0xc0ee01b9, /// 0xd7c
			 0x304a128c, /// 0xd80
			 0xf1d6ecf7, /// 0xd84
			 0xaaca0aef, /// 0xd88
			 0x2e83fd0c, /// 0xd8c
			 0x6fafb3a7, /// 0xd90
			 0x69c4627e, /// 0xd94
			 0xcb42a6a9, /// 0xd98
			 0x39b8aff1, /// 0xd9c
			 0x7dc57328, /// 0xda0
			 0x16483724, /// 0xda4
			 0x5f2294bc, /// 0xda8
			 0xf833dcf3, /// 0xdac
			 0x6c6598c4, /// 0xdb0
			 0x5eca5e39, /// 0xdb4
			 0x11b527f4, /// 0xdb8
			 0x0d0287ad, /// 0xdbc
			 0x39196889, /// 0xdc0
			 0x1a0bb916, /// 0xdc4
			 0x9e466c2c, /// 0xdc8
			 0x0ed2b547, /// 0xdcc
			 0x537d083f, /// 0xdd0
			 0x29114b04, /// 0xdd4
			 0xdda65c59, /// 0xdd8
			 0x6fab02c5, /// 0xddc
			 0x0d56c0ce, /// 0xde0
			 0x0ae6ba50, /// 0xde4
			 0x1b5f661d, /// 0xde8
			 0xfc53685e, /// 0xdec
			 0xa26f3021, /// 0xdf0
			 0x43143c61, /// 0xdf4
			 0x9997cd00, /// 0xdf8
			 0xeef82735, /// 0xdfc
			 0xfcfc9d7d, /// 0xe00
			 0x56553f3f, /// 0xe04
			 0x121cc2b3, /// 0xe08
			 0x1687e1e9, /// 0xe0c
			 0xca07dd01, /// 0xe10
			 0xf9df852f, /// 0xe14
			 0x1687d22a, /// 0xe18
			 0xb4f44d05, /// 0xe1c
			 0x8c25a395, /// 0xe20
			 0x89710d08, /// 0xe24
			 0x7a94865a, /// 0xe28
			 0xc4ec0349, /// 0xe2c
			 0x6c007166, /// 0xe30
			 0xff6560be, /// 0xe34
			 0x1b8ced4a, /// 0xe38
			 0xf4896b96, /// 0xe3c
			 0xd8a2fc94, /// 0xe40
			 0xdc3f00a2, /// 0xe44
			 0xf016f7c2, /// 0xe48
			 0x3761f5ae, /// 0xe4c
			 0xe0e7c01e, /// 0xe50
			 0x208ca98e, /// 0xe54
			 0xd08952a2, /// 0xe58
			 0xbcf0a751, /// 0xe5c
			 0xca13c2ab, /// 0xe60
			 0x97a43082, /// 0xe64
			 0x95010e5c, /// 0xe68
			 0x8ddc2caf, /// 0xe6c
			 0x8afbad56, /// 0xe70
			 0xf355c635, /// 0xe74
			 0xdcfdcd32, /// 0xe78
			 0x492b49ed, /// 0xe7c
			 0x6716b342, /// 0xe80
			 0x60ca32ca, /// 0xe84
			 0x9b99636e, /// 0xe88
			 0x42b04b42, /// 0xe8c
			 0x1bc9efb7, /// 0xe90
			 0xdc5cdd97, /// 0xe94
			 0xc0da6933, /// 0xe98
			 0xff952756, /// 0xe9c
			 0x497c43f2, /// 0xea0
			 0xf98f4544, /// 0xea4
			 0x2c1350df, /// 0xea8
			 0xfc33f755, /// 0xeac
			 0x7ff1c504, /// 0xeb0
			 0x9fd0939c, /// 0xeb4
			 0x0126d9f2, /// 0xeb8
			 0xade45a6c, /// 0xebc
			 0x750fbf59, /// 0xec0
			 0xc72e0fe2, /// 0xec4
			 0xe3331406, /// 0xec8
			 0xc3a7cc85, /// 0xecc
			 0x3586150c, /// 0xed0
			 0x7312c236, /// 0xed4
			 0x139d9429, /// 0xed8
			 0x372626e8, /// 0xedc
			 0x4254b2db, /// 0xee0
			 0x4f6ebc27, /// 0xee4
			 0xc63b2a23, /// 0xee8
			 0x0da20541, /// 0xeec
			 0x85219404, /// 0xef0
			 0xe66fc4f8, /// 0xef4
			 0xaf4781e8, /// 0xef8
			 0xd806df11, /// 0xefc
			 0x1aea7d18, /// 0xf00
			 0xff6b0907, /// 0xf04
			 0xadadf75a, /// 0xf08
			 0xb78e2044, /// 0xf0c
			 0x0eecfefe, /// 0xf10
			 0x46fe78eb, /// 0xf14
			 0x1405209c, /// 0xf18
			 0xaec199a4, /// 0xf1c
			 0xe510b3e9, /// 0xf20
			 0xaecb11fe, /// 0xf24
			 0x12f3db77, /// 0xf28
			 0x05075a68, /// 0xf2c
			 0x4eb78af3, /// 0xf30
			 0x7752af68, /// 0xf34
			 0x4ac74de5, /// 0xf38
			 0x607abcd2, /// 0xf3c
			 0xb962b439, /// 0xf40
			 0x72fc265f, /// 0xf44
			 0xb1caf15d, /// 0xf48
			 0xd360798f, /// 0xf4c
			 0x54e4389f, /// 0xf50
			 0x18854ffb, /// 0xf54
			 0x9dffcbe7, /// 0xf58
			 0xcb75b440, /// 0xf5c
			 0xf62b44bd, /// 0xf60
			 0xb180dc3a, /// 0xf64
			 0xda306553, /// 0xf68
			 0xef8451d8, /// 0xf6c
			 0xb3c536a6, /// 0xf70
			 0x147c14b4, /// 0xf74
			 0xc70e39d5, /// 0xf78
			 0x503916c2, /// 0xf7c
			 0xacabcac0, /// 0xf80
			 0x02316a68, /// 0xf84
			 0xcba6a551, /// 0xf88
			 0xf31b85df, /// 0xf8c
			 0xcf439317, /// 0xf90
			 0x06c504ac, /// 0xf94
			 0xb1ef8696, /// 0xf98
			 0x204f36f4, /// 0xf9c
			 0x54b8c62c, /// 0xfa0
			 0x2db99122, /// 0xfa4
			 0x74155259, /// 0xfa8
			 0x165d7875, /// 0xfac
			 0xca61f436, /// 0xfb0
			 0xbed79ffe, /// 0xfb4
			 0x18f0fb1f, /// 0xfb8
			 0x92178b55, /// 0xfbc
			 0xe16bdbfb, /// 0xfc0
			 0x1ef64e1f, /// 0xfc4
			 0xc4ae0314, /// 0xfc8
			 0x1b69791e, /// 0xfcc
			 0x8d9d1a4d, /// 0xfd0
			 0x2e9c6e55, /// 0xfd4
			 0xe6016d0c, /// 0xfd8
			 0x13be26a9, /// 0xfdc
			 0x0cc59d2e, /// 0xfe0
			 0x0e15452d, /// 0xfe4
			 0xe076f2d0, /// 0xfe8
			 0xa1e4721c, /// 0xfec
			 0xd0711b6b, /// 0xff0
			 0xd0a0b0e5, /// 0xff4
			 0x0d3bcac5, /// 0xff8
			 0xc06e0f17 /// last
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
			 0x000004dc,
			 0x0000009c,
			 0x00000750,
			 0x000007c4,
			 0x0000022c,
			 0x000002d8,
			 0x000001a4,
			 0x00000508,

			 /// vpu register f2
			 0x40e00000,
			 0x41500000,
			 0x41f80000,
			 0x41700000,
			 0x3f800000,
			 0x41400000,
			 0x40e00000,
			 0x41800000,

			 /// vpu register f3
			 0x41f00000,
			 0x40a00000,
			 0x41800000,
			 0x41b00000,
			 0x41400000,
			 0x41880000,
			 0x41400000,
			 0x41100000,

			 /// vpu register f4
			 0x41b80000,
			 0x41e80000,
			 0x41a00000,
			 0x42000000,
			 0x41f80000,
			 0x41880000,
			 0x41980000,
			 0x41a00000,

			 /// vpu register f5
			 0x41c80000,
			 0x40c00000,
			 0x41b80000,
			 0x41e00000,
			 0x41a00000,
			 0x41300000,
			 0x40a00000,
			 0x41e00000,

			 /// vpu register f6
			 0x40000000,
			 0x41900000,
			 0x41000000,
			 0x41900000,
			 0x41000000,
			 0x41a00000,
			 0x41980000,
			 0x41c80000,

			 /// vpu register f7
			 0x40000000,
			 0x41c80000,
			 0x41e00000,
			 0x3f800000,
			 0x41f80000,
			 0x41f80000,
			 0x40a00000,
			 0x41400000,

			 /// vpu register f8
			 0x41880000,
			 0x40000000,
			 0x40a00000,
			 0x41400000,
			 0x40c00000,
			 0x41800000,
			 0x41d00000,
			 0x40a00000,

			 /// vpu register f9
			 0x41b80000,
			 0x41900000,
			 0x41e80000,
			 0x40e00000,
			 0x40a00000,
			 0x41400000,
			 0x41000000,
			 0x41200000,

			 /// vpu register f10
			 0x3f800000,
			 0x41b80000,
			 0x40400000,
			 0x41c80000,
			 0x41d00000,
			 0x41a00000,
			 0x40e00000,
			 0x40e00000,

			 /// vpu register f11
			 0x41500000,
			 0x41c00000,
			 0x41c00000,
			 0x41d00000,
			 0x41980000,
			 0x41900000,
			 0x41000000,
			 0x41200000,

			 /// vpu register f12
			 0x41c00000,
			 0x40400000,
			 0x41200000,
			 0x40400000,
			 0x41b80000,
			 0x41800000,
			 0x40800000,
			 0x40400000,

			 /// vpu register f13
			 0x41900000,
			 0x42000000,
			 0x41880000,
			 0x41000000,
			 0x41600000,
			 0x41b00000,
			 0x41700000,
			 0x41e80000,

			 /// vpu register f14
			 0x40a00000,
			 0x41600000,
			 0x40000000,
			 0x40c00000,
			 0x41900000,
			 0x40e00000,
			 0x40e00000,
			 0x42000000,

			 /// vpu register f15
			 0x40400000,
			 0x41980000,
			 0x41b80000,
			 0x40e00000,
			 0x41c00000,
			 0x41f80000,
			 0x41f00000,
			 0x41600000,

			 /// vpu register f16
			 0x40a00000,
			 0x41c00000,
			 0x40800000,
			 0x41900000,
			 0x42000000,
			 0x41c00000,
			 0x40e00000,
			 0x41900000,

			 /// vpu register f17
			 0x41b80000,
			 0x40a00000,
			 0x41600000,
			 0x41700000,
			 0x41a80000,
			 0x40400000,
			 0x41600000,
			 0x41880000,

			 /// vpu register f18
			 0x41a00000,
			 0x41a00000,
			 0x40000000,
			 0x40800000,
			 0x41f00000,
			 0x42000000,
			 0x41c80000,
			 0x40e00000,

			 /// vpu register f19
			 0x41e80000,
			 0x41f80000,
			 0x41400000,
			 0x41a00000,
			 0x40e00000,
			 0x41200000,
			 0x41e80000,
			 0x41600000,

			 /// vpu register f20
			 0x40400000,
			 0x41800000,
			 0x41f00000,
			 0x41c00000,
			 0x41500000,
			 0x41f00000,
			 0x40a00000,
			 0x41f80000,

			 /// vpu register f21
			 0x41f00000,
			 0x41e00000,
			 0x40e00000,
			 0x41d00000,
			 0x40800000,
			 0x41500000,
			 0x41b00000,
			 0x41d80000,

			 /// vpu register f22
			 0x41300000,
			 0x41f80000,
			 0x40800000,
			 0x41c00000,
			 0x41700000,
			 0x41700000,
			 0x41600000,
			 0x41000000,

			 /// vpu register f23
			 0x40a00000,
			 0x41e80000,
			 0x42000000,
			 0x41880000,
			 0x40400000,
			 0x41400000,
			 0x40000000,
			 0x41d00000,

			 /// vpu register f24
			 0x41b00000,
			 0x41d80000,
			 0x41a00000,
			 0x41000000,
			 0x41e00000,
			 0x41b80000,
			 0x41e00000,
			 0x41880000,

			 /// vpu register f25
			 0x41500000,
			 0x40e00000,
			 0x41d80000,
			 0x40800000,
			 0x41400000,
			 0x3f800000,
			 0x41e80000,
			 0x40a00000,

			 /// vpu register f26
			 0x40c00000,
			 0x41e00000,
			 0x41f00000,
			 0x41d00000,
			 0x41e00000,
			 0x41b80000,
			 0x42000000,
			 0x41600000,

			 /// vpu register f27
			 0x41e00000,
			 0x41800000,
			 0x41f00000,
			 0x41e80000,
			 0x41700000,
			 0x40800000,
			 0x41200000,
			 0x41200000,

			 /// vpu register f28
			 0x41500000,
			 0x41b00000,
			 0x41980000,
			 0x41100000,
			 0x41d00000,
			 0x40400000,
			 0x41b80000,
			 0x3f800000,

			 /// vpu register f29
			 0x41400000,
			 0x41d80000,
			 0x40000000,
			 0x41b00000,
			 0x3f800000,
			 0x41e80000,
			 0x40c00000,
			 0x41e80000,

			 /// vpu register f30
			 0x41e80000,
			 0x40c00000,
			 0x41980000,
			 0x40a00000,
			 0x41d00000,
			 0x41980000,
			 0x42000000,
			 0x41980000,

			 /// vpu register f31
			 0x41600000,
			 0x41e80000,
			 0x41a00000,
			 0x41f00000,
			 0x40e00000,
			 0x40e00000,
			 0x41a00000,
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
		"cubeface.ps f4, f18, f3\n"                           ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f24, f9, f5\n"                           ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f10, f18, f3\n"                          ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f6, f20, f14\n"                          ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f20, f12, f19\n"                         ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f2, f12, f16\n"                          ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f8, f21, f14\n"                          ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f23, f27, f17\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f12, f20, f2\n"                          ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f22, f29, f0\n"                          ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f12, f24, f29\n"                         ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f26, f19, f22\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f25, f26, f19\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f29, f0, f6\n"                           ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f26, f15, f26\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f7, f24, f1\n"                           ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f17, f19, f2\n"                          ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f11, f24, f15\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f4, f9, f18\n"                           ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f22, f21, f6\n"                          ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f4, f10, f16\n"                          ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f11, f26, f28\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f13, f6, f8\n"                           ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f8, f7, f22\n"                           ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f27, f26, f0\n"                          ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f19, f25, f6\n"                          ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f28, f6, f29\n"                          ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f10, f2, f27\n"                          ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f26, f1, f22\n"                          ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f16, f24, f13\n"                         ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f3, f0, f1\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f20, f22, f3\n"                          ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f6, f25, f9\n"                           ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f21, f22, f18\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f30, f11, f23\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f8, f18, f18\n"                          ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f15, f25, f24\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f15, f29, f26\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f22, f3, f29\n"                          ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f9, f6, f19\n"                           ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f3, f19, f7\n"                           ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f17, f18, f9\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f18, f5, f28\n"                          ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f15, f12, f22\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f10, f21, f4\n"                          ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f19, f4, f3\n"                           ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f12, f25, f4\n"                          ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f10, f1, f26\n"                          ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f30, f21, f15\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f17, f2, f27\n"                          ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f26, f13, f14\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f30, f18, f9\n"                          ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f7, f8, f29\n"                           ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f28, f8, f29\n"                          ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f12, f18, f5\n"                          ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f11, f7, f9\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f10, f0, f10\n"                          ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f8, f23, f18\n"                          ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f28, f8, f26\n"                          ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f14, f11, f13\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f3, f27, f19\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f12, f2, f8\n"                           ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f26, f4, f4\n"                           ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f25, f5, f1\n"                           ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f2, f22, f9\n"                           ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f27, f4, f28\n"                          ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f10, f21, f13\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f21, f0, f1\n"                           ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f12, f8, f28\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f17, f22, f0\n"                          ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f18, f5, f17\n"                          ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f24, f16, f10\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f9, f0, f11\n"                           ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f29, f0, f9\n"                           ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f18, f29, f10\n"                         ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f25, f16, f7\n"                          ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f4, f14, f5\n"                           ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f28, f19, f8\n"                          ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f4, f17, f22\n"                          ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f6, f11, f27\n"                          ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f28, f25, f11\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f12, f1, f18\n"                          ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f13, f4, f3\n"                           ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f30, f25, f6\n"                          ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f10, f6, f21\n"                          ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f4, f18, f7\n"                           ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f5, f10, f23\n"                          ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f11, f21, f16\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f7, f21, f26\n"                          ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f18, f28, f6\n"                          ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f28, f24, f20\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f25, f18, f2\n"                          ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f16, f20, f30\n"                         ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f9, f18, f1\n"                           ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f29, f16, f9\n"                          ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f25, f13, f28\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f23, f1, f4\n"                           ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f16, f7, f24\n"                          ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubeface.ps f4, f6, f27\n"                           ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
