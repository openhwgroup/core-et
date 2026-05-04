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
			 0x86f7b58f, /// 0x0
			 0xd6c4b226, /// 0x4
			 0x7b64f04a, /// 0x8
			 0xa556f053, /// 0xc
			 0x6cdc9528, /// 0x10
			 0x79e79332, /// 0x14
			 0xd133d287, /// 0x18
			 0x66664434, /// 0x1c
			 0x45db8e7f, /// 0x20
			 0x29872c19, /// 0x24
			 0x2e5adaee, /// 0x28
			 0xe0c6e3c7, /// 0x2c
			 0x41955eca, /// 0x30
			 0x2894eec6, /// 0x34
			 0x461444a1, /// 0x38
			 0x79b67c59, /// 0x3c
			 0x9c290f61, /// 0x40
			 0xe75e5b6d, /// 0x44
			 0xe6434111, /// 0x48
			 0x4b991c04, /// 0x4c
			 0x461a5470, /// 0x50
			 0x4059131f, /// 0x54
			 0x758a9f91, /// 0x58
			 0x836f199a, /// 0x5c
			 0xf1fd91bb, /// 0x60
			 0x0332228a, /// 0x64
			 0x2b3134f2, /// 0x68
			 0x1affdbb9, /// 0x6c
			 0xa427b2bf, /// 0x70
			 0x109699d1, /// 0x74
			 0x86dc0557, /// 0x78
			 0xdb8893fd, /// 0x7c
			 0xe3c5081f, /// 0x80
			 0x69739041, /// 0x84
			 0xafd48e4b, /// 0x88
			 0x1d3408bf, /// 0x8c
			 0x27f327b9, /// 0x90
			 0xa875a1d1, /// 0x94
			 0xedc86f06, /// 0x98
			 0x3476f1fb, /// 0x9c
			 0x2a76bd00, /// 0xa0
			 0x717244be, /// 0xa4
			 0x12afd706, /// 0xa8
			 0xc1be3c7b, /// 0xac
			 0xc2a6cc99, /// 0xb0
			 0xcb8bd896, /// 0xb4
			 0x944a3a5a, /// 0xb8
			 0xc960f9a2, /// 0xbc
			 0xcbc56693, /// 0xc0
			 0x682e8d77, /// 0xc4
			 0x6a78d3f3, /// 0xc8
			 0xac87fc7d, /// 0xcc
			 0x7eba4b3a, /// 0xd0
			 0xbcfc88e2, /// 0xd4
			 0x8f2cc964, /// 0xd8
			 0x365c18b6, /// 0xdc
			 0x2551e4a7, /// 0xe0
			 0x96c3a7f8, /// 0xe4
			 0x02807bb3, /// 0xe8
			 0xbd5aa45b, /// 0xec
			 0x924f53d3, /// 0xf0
			 0xff286cbd, /// 0xf4
			 0x47f4c93a, /// 0xf8
			 0xe740b7f6, /// 0xfc
			 0x24b42b1e, /// 0x100
			 0x6920b8a2, /// 0x104
			 0xb05e3967, /// 0x108
			 0x1c081aa7, /// 0x10c
			 0x5bd8b1b5, /// 0x110
			 0x842b3fe2, /// 0x114
			 0x48c6dc60, /// 0x118
			 0x5e8c19af, /// 0x11c
			 0x1f52fd80, /// 0x120
			 0xbdac508d, /// 0x124
			 0xc266669f, /// 0x128
			 0x8ae8ae12, /// 0x12c
			 0x334ed275, /// 0x130
			 0xfc733fdd, /// 0x134
			 0xaa54a69f, /// 0x138
			 0x2db99617, /// 0x13c
			 0x3983996d, /// 0x140
			 0x8ae4a4b9, /// 0x144
			 0xf6cecea9, /// 0x148
			 0x798cb7e4, /// 0x14c
			 0x8792e395, /// 0x150
			 0x77d73b8b, /// 0x154
			 0x1b0968f4, /// 0x158
			 0xb1bd5ba3, /// 0x15c
			 0x0abb2078, /// 0x160
			 0x1dac1035, /// 0x164
			 0xe901913a, /// 0x168
			 0x565e788a, /// 0x16c
			 0x567b60f4, /// 0x170
			 0x6a8181c5, /// 0x174
			 0x4fab4878, /// 0x178
			 0x3e55b0b3, /// 0x17c
			 0x1d4e434d, /// 0x180
			 0xd21dd48b, /// 0x184
			 0x0c947314, /// 0x188
			 0x834076cf, /// 0x18c
			 0x3ae583e8, /// 0x190
			 0x0306e439, /// 0x194
			 0x73d2b02a, /// 0x198
			 0xa0d24f91, /// 0x19c
			 0x61db225c, /// 0x1a0
			 0x802263d1, /// 0x1a4
			 0xb0268e17, /// 0x1a8
			 0x1b28b4ac, /// 0x1ac
			 0xdf2dda03, /// 0x1b0
			 0xc45ba249, /// 0x1b4
			 0xa2d6a516, /// 0x1b8
			 0x39c170f5, /// 0x1bc
			 0xa81fc739, /// 0x1c0
			 0x63b13a9e, /// 0x1c4
			 0x2e24982f, /// 0x1c8
			 0x188dba46, /// 0x1cc
			 0xc878bc5c, /// 0x1d0
			 0x18fa73ac, /// 0x1d4
			 0x96fc2e48, /// 0x1d8
			 0x45670400, /// 0x1dc
			 0x06b34765, /// 0x1e0
			 0x5f81fdf4, /// 0x1e4
			 0x061aef2c, /// 0x1e8
			 0xc8ddfdc4, /// 0x1ec
			 0xccbb0fe5, /// 0x1f0
			 0x4494fbe2, /// 0x1f4
			 0x8f744b13, /// 0x1f8
			 0x23b197fb, /// 0x1fc
			 0x7ef5b267, /// 0x200
			 0x882935b5, /// 0x204
			 0x23f26c1f, /// 0x208
			 0xabd4d1f0, /// 0x20c
			 0x4f991bfb, /// 0x210
			 0x271545e2, /// 0x214
			 0xab98b40e, /// 0x218
			 0xcacc0072, /// 0x21c
			 0x7dc07633, /// 0x220
			 0xb158fb32, /// 0x224
			 0x8517ef3e, /// 0x228
			 0x68a3d3ef, /// 0x22c
			 0xe414b12f, /// 0x230
			 0x723d1b46, /// 0x234
			 0x1e8294c3, /// 0x238
			 0x8f485625, /// 0x23c
			 0x0a49fe41, /// 0x240
			 0x379beeaa, /// 0x244
			 0xe99668b1, /// 0x248
			 0x094c81b8, /// 0x24c
			 0x71a70b47, /// 0x250
			 0xb41c8a48, /// 0x254
			 0xdd9acc71, /// 0x258
			 0xfeec1b4d, /// 0x25c
			 0x7ff32c32, /// 0x260
			 0x4a48017d, /// 0x264
			 0x989e6e18, /// 0x268
			 0x0e4ccf0c, /// 0x26c
			 0x56e1cedc, /// 0x270
			 0x7f69e09f, /// 0x274
			 0xcb3d5aee, /// 0x278
			 0xfb75060e, /// 0x27c
			 0x40c60647, /// 0x280
			 0xfffb95be, /// 0x284
			 0xd446aa95, /// 0x288
			 0xdf31feef, /// 0x28c
			 0x09c3b6f5, /// 0x290
			 0xda0bd67e, /// 0x294
			 0x1d81d0bd, /// 0x298
			 0x8b3a02f8, /// 0x29c
			 0xd1b54761, /// 0x2a0
			 0x695a990b, /// 0x2a4
			 0xf665db98, /// 0x2a8
			 0x77d75b1b, /// 0x2ac
			 0x75cb05e1, /// 0x2b0
			 0x963debbd, /// 0x2b4
			 0x26d3ee5f, /// 0x2b8
			 0x1f72a2ab, /// 0x2bc
			 0x2e674548, /// 0x2c0
			 0xe811e23f, /// 0x2c4
			 0x06d8d3b2, /// 0x2c8
			 0x5e2f06c6, /// 0x2cc
			 0xca7d8198, /// 0x2d0
			 0x385d9325, /// 0x2d4
			 0x0ceca8dd, /// 0x2d8
			 0xad9668ff, /// 0x2dc
			 0x8bfc8e72, /// 0x2e0
			 0x7521eac5, /// 0x2e4
			 0x7ccf39c7, /// 0x2e8
			 0x0389f6d3, /// 0x2ec
			 0x91365081, /// 0x2f0
			 0x0aba9d29, /// 0x2f4
			 0xb1f3a5fa, /// 0x2f8
			 0x150b9edc, /// 0x2fc
			 0xb41e219e, /// 0x300
			 0xffc681d9, /// 0x304
			 0xd1976dc9, /// 0x308
			 0x3c4beeff, /// 0x30c
			 0xb8e0f461, /// 0x310
			 0xe1b61368, /// 0x314
			 0x195bb6e5, /// 0x318
			 0xf63ff47f, /// 0x31c
			 0xe20cf785, /// 0x320
			 0x5838fd0c, /// 0x324
			 0xd0da3764, /// 0x328
			 0x6bc5f6df, /// 0x32c
			 0x5fc32396, /// 0x330
			 0x8906ad0f, /// 0x334
			 0x3a19ba53, /// 0x338
			 0xc939fba9, /// 0x33c
			 0x4e2babe7, /// 0x340
			 0xa4e4a95b, /// 0x344
			 0x09d1d624, /// 0x348
			 0x39430e2b, /// 0x34c
			 0x0d8b8cff, /// 0x350
			 0xc9251920, /// 0x354
			 0x9db087b8, /// 0x358
			 0x5613e523, /// 0x35c
			 0x6440ef5a, /// 0x360
			 0xcab46d3b, /// 0x364
			 0x5f8ad621, /// 0x368
			 0x5630d01a, /// 0x36c
			 0xdd43bdd5, /// 0x370
			 0x3fe8b978, /// 0x374
			 0x8b6eb4b5, /// 0x378
			 0x3615bf13, /// 0x37c
			 0xe48cd27a, /// 0x380
			 0x42e1d3d1, /// 0x384
			 0xb05e8495, /// 0x388
			 0xfa2b016f, /// 0x38c
			 0xd0022444, /// 0x390
			 0x3e031233, /// 0x394
			 0xe2069914, /// 0x398
			 0xb875c1f2, /// 0x39c
			 0xef33dc2f, /// 0x3a0
			 0x5239ce48, /// 0x3a4
			 0xa9ad732b, /// 0x3a8
			 0x75c50ac0, /// 0x3ac
			 0x929eba09, /// 0x3b0
			 0x7d8ae5af, /// 0x3b4
			 0xa142a80e, /// 0x3b8
			 0x1629cc73, /// 0x3bc
			 0x16e3602b, /// 0x3c0
			 0xb454c7af, /// 0x3c4
			 0x6c9e09b3, /// 0x3c8
			 0xde6ef826, /// 0x3cc
			 0xc4ce08f2, /// 0x3d0
			 0x0b5857e1, /// 0x3d4
			 0xbda4ac79, /// 0x3d8
			 0xe2c86386, /// 0x3dc
			 0x18d2858a, /// 0x3e0
			 0xe1d2b219, /// 0x3e4
			 0xe40e64c8, /// 0x3e8
			 0x71ca532d, /// 0x3ec
			 0x07a9e7c2, /// 0x3f0
			 0x6f657700, /// 0x3f4
			 0xc4fdf9eb, /// 0x3f8
			 0x8e2f4d5d, /// 0x3fc
			 0x8d104665, /// 0x400
			 0x4cd8b042, /// 0x404
			 0x6e115ceb, /// 0x408
			 0xd2001419, /// 0x40c
			 0x4e63438c, /// 0x410
			 0xe32a9ed6, /// 0x414
			 0xe8e3894b, /// 0x418
			 0xd07b8af1, /// 0x41c
			 0x885e1b3b, /// 0x420
			 0x5f77a8cb, /// 0x424
			 0xe130b8b7, /// 0x428
			 0x1342b223, /// 0x42c
			 0x4aee8dfd, /// 0x430
			 0xd13e8387, /// 0x434
			 0x71746643, /// 0x438
			 0xabf22c38, /// 0x43c
			 0x05a2cd2a, /// 0x440
			 0x87f8013f, /// 0x444
			 0xa7967431, /// 0x448
			 0x33c1221e, /// 0x44c
			 0x53d98189, /// 0x450
			 0xefe857ee, /// 0x454
			 0xb85225cb, /// 0x458
			 0xac441e64, /// 0x45c
			 0xa0a28304, /// 0x460
			 0x3e6b5463, /// 0x464
			 0x72e1a75f, /// 0x468
			 0x92e4d534, /// 0x46c
			 0x58505a3e, /// 0x470
			 0x95822c22, /// 0x474
			 0x7fddf1f8, /// 0x478
			 0xd5cd5364, /// 0x47c
			 0x4577afe3, /// 0x480
			 0xa2eef99c, /// 0x484
			 0x60144e83, /// 0x488
			 0xe34869bb, /// 0x48c
			 0x78150688, /// 0x490
			 0xbeec85d6, /// 0x494
			 0x0ee60e5e, /// 0x498
			 0x61686e53, /// 0x49c
			 0x221d01a8, /// 0x4a0
			 0xd6cdcde7, /// 0x4a4
			 0xd0c8fd0c, /// 0x4a8
			 0x21f0a78a, /// 0x4ac
			 0x0018c757, /// 0x4b0
			 0xe136d5a4, /// 0x4b4
			 0x4aaf2be1, /// 0x4b8
			 0x2a50a96f, /// 0x4bc
			 0x3bee5d99, /// 0x4c0
			 0xcda8b30b, /// 0x4c4
			 0xe244ed83, /// 0x4c8
			 0x88bc07c8, /// 0x4cc
			 0x2f85f71c, /// 0x4d0
			 0x208b2ebf, /// 0x4d4
			 0xbd7cf8ab, /// 0x4d8
			 0xa9b0de33, /// 0x4dc
			 0x9bfe818f, /// 0x4e0
			 0x5f416cf3, /// 0x4e4
			 0xef5dc79e, /// 0x4e8
			 0xc414b121, /// 0x4ec
			 0xecf9c0f7, /// 0x4f0
			 0x12f3f6c8, /// 0x4f4
			 0x00a14482, /// 0x4f8
			 0x78a4221e, /// 0x4fc
			 0x8c1fccc9, /// 0x500
			 0x140c6431, /// 0x504
			 0xb2210757, /// 0x508
			 0x63d70f44, /// 0x50c
			 0xab7b6e7b, /// 0x510
			 0xb4aced68, /// 0x514
			 0xa7bd412c, /// 0x518
			 0x42721f9e, /// 0x51c
			 0x692e259b, /// 0x520
			 0x2d7ade6b, /// 0x524
			 0x016ce126, /// 0x528
			 0x8998189e, /// 0x52c
			 0xe85455b9, /// 0x530
			 0x013d58c0, /// 0x534
			 0xf7cb4fba, /// 0x538
			 0xc8df95fe, /// 0x53c
			 0x002c07ab, /// 0x540
			 0x0c5b3e94, /// 0x544
			 0x836b294d, /// 0x548
			 0x7e7ab3ce, /// 0x54c
			 0x7e2f8f54, /// 0x550
			 0x488ab739, /// 0x554
			 0x621e7716, /// 0x558
			 0x4e8a5d85, /// 0x55c
			 0xe400243d, /// 0x560
			 0x157b9500, /// 0x564
			 0x205e6785, /// 0x568
			 0xc514c84f, /// 0x56c
			 0x6055d64b, /// 0x570
			 0x513de82a, /// 0x574
			 0x98e1e945, /// 0x578
			 0xd045cc33, /// 0x57c
			 0xbc30dbd1, /// 0x580
			 0xe68bcec9, /// 0x584
			 0x6456d44a, /// 0x588
			 0xe9014855, /// 0x58c
			 0xf658915a, /// 0x590
			 0xbada8879, /// 0x594
			 0xaf9033f2, /// 0x598
			 0xd557a21c, /// 0x59c
			 0xe69eee25, /// 0x5a0
			 0x689e7573, /// 0x5a4
			 0xd4053d2b, /// 0x5a8
			 0xaefe385f, /// 0x5ac
			 0x6956b141, /// 0x5b0
			 0xcac1151a, /// 0x5b4
			 0xd0020509, /// 0x5b8
			 0xeb215fbb, /// 0x5bc
			 0x0f6a7053, /// 0x5c0
			 0x312e2854, /// 0x5c4
			 0x07822e63, /// 0x5c8
			 0x07589888, /// 0x5cc
			 0xde54b725, /// 0x5d0
			 0x172e0d58, /// 0x5d4
			 0xd413d616, /// 0x5d8
			 0x062d407f, /// 0x5dc
			 0xfff8773a, /// 0x5e0
			 0xf18f138f, /// 0x5e4
			 0x05e1fa08, /// 0x5e8
			 0xf6c15767, /// 0x5ec
			 0xc9cb6505, /// 0x5f0
			 0x8e15804c, /// 0x5f4
			 0xa59c998f, /// 0x5f8
			 0x3a2dc792, /// 0x5fc
			 0x38293155, /// 0x600
			 0x4673a1d3, /// 0x604
			 0xce00ec2f, /// 0x608
			 0xd23f8f75, /// 0x60c
			 0x79569263, /// 0x610
			 0x82fe1c64, /// 0x614
			 0x7c160d5d, /// 0x618
			 0xcfa41564, /// 0x61c
			 0x24605da8, /// 0x620
			 0xab8bde72, /// 0x624
			 0xde9d26f3, /// 0x628
			 0x6954c903, /// 0x62c
			 0x4182472b, /// 0x630
			 0xd0f96960, /// 0x634
			 0x0c328f06, /// 0x638
			 0xfa7dfba8, /// 0x63c
			 0xb947c6c1, /// 0x640
			 0x31b05c16, /// 0x644
			 0x4f7d4c2e, /// 0x648
			 0x1db62d13, /// 0x64c
			 0xfe91ad37, /// 0x650
			 0x35a09159, /// 0x654
			 0x7c60bb9f, /// 0x658
			 0x4123353a, /// 0x65c
			 0x90eb9094, /// 0x660
			 0x7c73c348, /// 0x664
			 0xaebdf8dd, /// 0x668
			 0xc30f4c2f, /// 0x66c
			 0xfcfd90a0, /// 0x670
			 0x8963db6f, /// 0x674
			 0xcac74c9e, /// 0x678
			 0x0d79f134, /// 0x67c
			 0xa049873e, /// 0x680
			 0x05fda0ad, /// 0x684
			 0xbd119878, /// 0x688
			 0x30553231, /// 0x68c
			 0x956f4d1c, /// 0x690
			 0xc36954d8, /// 0x694
			 0xec29f8ad, /// 0x698
			 0x1340b4ac, /// 0x69c
			 0xaed5bdf2, /// 0x6a0
			 0xe44248c9, /// 0x6a4
			 0xee5da2a2, /// 0x6a8
			 0x27adbf99, /// 0x6ac
			 0x9f1b6f8b, /// 0x6b0
			 0x3230b8ea, /// 0x6b4
			 0x7016a8a1, /// 0x6b8
			 0x2e080819, /// 0x6bc
			 0x8aacb541, /// 0x6c0
			 0xd71ee876, /// 0x6c4
			 0x9871d14b, /// 0x6c8
			 0x275103bc, /// 0x6cc
			 0x1236b43d, /// 0x6d0
			 0xd325fb69, /// 0x6d4
			 0x6f076686, /// 0x6d8
			 0x02db2a48, /// 0x6dc
			 0xa976663c, /// 0x6e0
			 0x11a70b0a, /// 0x6e4
			 0x1e4ce05d, /// 0x6e8
			 0xd00e54a2, /// 0x6ec
			 0x51aa3d23, /// 0x6f0
			 0xedf471f9, /// 0x6f4
			 0x1ca966e3, /// 0x6f8
			 0x7466e81a, /// 0x6fc
			 0x8f55dd84, /// 0x700
			 0x12ce6473, /// 0x704
			 0xcc826a2c, /// 0x708
			 0x3931c19c, /// 0x70c
			 0x4be95353, /// 0x710
			 0xb15b048f, /// 0x714
			 0xb58a4efd, /// 0x718
			 0xe535d973, /// 0x71c
			 0xbb4cd259, /// 0x720
			 0x3f88f916, /// 0x724
			 0xfa063ebe, /// 0x728
			 0x3bb242ed, /// 0x72c
			 0xcb5bc275, /// 0x730
			 0xb3b7a279, /// 0x734
			 0xe0a92d0a, /// 0x738
			 0xeea60c8c, /// 0x73c
			 0xfeff5191, /// 0x740
			 0x2ae23a45, /// 0x744
			 0x42358aee, /// 0x748
			 0x30e2851b, /// 0x74c
			 0x6cb7dc03, /// 0x750
			 0x9bdb0223, /// 0x754
			 0xc92e3507, /// 0x758
			 0x62275970, /// 0x75c
			 0x12f4420d, /// 0x760
			 0xc58ff6a0, /// 0x764
			 0x8820812e, /// 0x768
			 0x8e5eb668, /// 0x76c
			 0x19c93b7b, /// 0x770
			 0x35a6e948, /// 0x774
			 0xfe8bcabd, /// 0x778
			 0x376a9694, /// 0x77c
			 0x3902511b, /// 0x780
			 0x35fb897a, /// 0x784
			 0x30a1c196, /// 0x788
			 0x9625c0eb, /// 0x78c
			 0xb5913234, /// 0x790
			 0x9d8cb1df, /// 0x794
			 0xf1610b81, /// 0x798
			 0x30b2eb4c, /// 0x79c
			 0xf5bdc54f, /// 0x7a0
			 0x4d621315, /// 0x7a4
			 0xa945e7ef, /// 0x7a8
			 0xcd7c8241, /// 0x7ac
			 0x66b7382a, /// 0x7b0
			 0x3bc1c166, /// 0x7b4
			 0x09141d9e, /// 0x7b8
			 0xeac2639c, /// 0x7bc
			 0x4e67256e, /// 0x7c0
			 0xc3b8658f, /// 0x7c4
			 0x29165232, /// 0x7c8
			 0xfbf07bfd, /// 0x7cc
			 0xe8e82268, /// 0x7d0
			 0xa8ce9e46, /// 0x7d4
			 0x19b1064e, /// 0x7d8
			 0x09b060b1, /// 0x7dc
			 0x300a0517, /// 0x7e0
			 0x9fe6ab0d, /// 0x7e4
			 0x5c5ae72c, /// 0x7e8
			 0x282852a2, /// 0x7ec
			 0x975624d6, /// 0x7f0
			 0xea80fcc6, /// 0x7f4
			 0xa9734cc0, /// 0x7f8
			 0x9d688e0b, /// 0x7fc
			 0xfb844530, /// 0x800
			 0x1b7acf9d, /// 0x804
			 0xc0d9dfee, /// 0x808
			 0x88774d44, /// 0x80c
			 0xe723343a, /// 0x810
			 0x285fa8c1, /// 0x814
			 0xff6153d7, /// 0x818
			 0xbed49108, /// 0x81c
			 0x92b3d075, /// 0x820
			 0xf05000e1, /// 0x824
			 0x922cc35f, /// 0x828
			 0xbbf94314, /// 0x82c
			 0xf4be0957, /// 0x830
			 0x9ca4f614, /// 0x834
			 0x140fccce, /// 0x838
			 0x2b0f4ac8, /// 0x83c
			 0xe16f6ca8, /// 0x840
			 0x0cb7c10b, /// 0x844
			 0x057d9525, /// 0x848
			 0xb410d2c1, /// 0x84c
			 0x875c5cc9, /// 0x850
			 0x026161ff, /// 0x854
			 0x3aadff26, /// 0x858
			 0xf3fed0c7, /// 0x85c
			 0xbf6bf2c3, /// 0x860
			 0x5f94fe3d, /// 0x864
			 0x3c5e0213, /// 0x868
			 0xbc745ed0, /// 0x86c
			 0x7829fbd3, /// 0x870
			 0xeebe535c, /// 0x874
			 0x88442cd5, /// 0x878
			 0x7d1e866c, /// 0x87c
			 0xe1b2e168, /// 0x880
			 0x93b53588, /// 0x884
			 0x6ab8aff9, /// 0x888
			 0xc64066bd, /// 0x88c
			 0xcbb0a382, /// 0x890
			 0x0a900d30, /// 0x894
			 0x3b6cbd79, /// 0x898
			 0xea0c7628, /// 0x89c
			 0xdefbbcac, /// 0x8a0
			 0x701e7921, /// 0x8a4
			 0x57ac8b9e, /// 0x8a8
			 0x219d36e1, /// 0x8ac
			 0xb4f89a69, /// 0x8b0
			 0xa66fa2e2, /// 0x8b4
			 0xbf476f60, /// 0x8b8
			 0x2d198306, /// 0x8bc
			 0x98022813, /// 0x8c0
			 0x6c3fb60f, /// 0x8c4
			 0x6867f941, /// 0x8c8
			 0x0d5aa2ad, /// 0x8cc
			 0x6195ac06, /// 0x8d0
			 0xb66aee4f, /// 0x8d4
			 0x3c678806, /// 0x8d8
			 0x2b5e1e29, /// 0x8dc
			 0xf0808cac, /// 0x8e0
			 0x9d04bb87, /// 0x8e4
			 0xfd8b9da5, /// 0x8e8
			 0xfa8fc3fb, /// 0x8ec
			 0x4cf54d4e, /// 0x8f0
			 0xef170675, /// 0x8f4
			 0x87145b62, /// 0x8f8
			 0xd1b81413, /// 0x8fc
			 0x3a623b4e, /// 0x900
			 0xb0625375, /// 0x904
			 0x9bb06d09, /// 0x908
			 0x003ea35b, /// 0x90c
			 0x50660de9, /// 0x910
			 0x826065b3, /// 0x914
			 0xc5184fe7, /// 0x918
			 0xdb9454de, /// 0x91c
			 0xb4e54d83, /// 0x920
			 0xdec17ca1, /// 0x924
			 0x1435fd06, /// 0x928
			 0x8d03d2b1, /// 0x92c
			 0x5f5d6d4a, /// 0x930
			 0x98427bde, /// 0x934
			 0x38874bcf, /// 0x938
			 0xc72db9f6, /// 0x93c
			 0xc2564d27, /// 0x940
			 0xca7c62d6, /// 0x944
			 0xe7c7719e, /// 0x948
			 0x450ef061, /// 0x94c
			 0x7664a6b7, /// 0x950
			 0x44a74c3d, /// 0x954
			 0x8897ee79, /// 0x958
			 0x08a2fcda, /// 0x95c
			 0xca74a191, /// 0x960
			 0x2157b1a3, /// 0x964
			 0x6cb1e839, /// 0x968
			 0xa052f8f9, /// 0x96c
			 0x5aed2e4d, /// 0x970
			 0x99b305ca, /// 0x974
			 0xcf4bf692, /// 0x978
			 0x336fe64d, /// 0x97c
			 0x362d9c1d, /// 0x980
			 0xb6f9893b, /// 0x984
			 0x1fb41ccf, /// 0x988
			 0x751c1630, /// 0x98c
			 0x42973bc6, /// 0x990
			 0x36fdd094, /// 0x994
			 0xaa274fc4, /// 0x998
			 0xa9c7eec3, /// 0x99c
			 0x5e05e672, /// 0x9a0
			 0xaf24a934, /// 0x9a4
			 0x66e02fa4, /// 0x9a8
			 0x0680faf6, /// 0x9ac
			 0x181db641, /// 0x9b0
			 0xa7f56eda, /// 0x9b4
			 0x01cd36a4, /// 0x9b8
			 0x071b2be9, /// 0x9bc
			 0x6eab7baf, /// 0x9c0
			 0x2765e5e6, /// 0x9c4
			 0x6bd5dbb1, /// 0x9c8
			 0x7e99ac22, /// 0x9cc
			 0xf0c674b8, /// 0x9d0
			 0x0c48e8b1, /// 0x9d4
			 0xbef0f35a, /// 0x9d8
			 0x5146be02, /// 0x9dc
			 0x3857fee1, /// 0x9e0
			 0x3779c48d, /// 0x9e4
			 0x9989b2dc, /// 0x9e8
			 0xba320c73, /// 0x9ec
			 0xff90a055, /// 0x9f0
			 0x650eaac2, /// 0x9f4
			 0xe2aa5a5c, /// 0x9f8
			 0x6739601d, /// 0x9fc
			 0x40e18424, /// 0xa00
			 0xfd4f5388, /// 0xa04
			 0x1a74da62, /// 0xa08
			 0xa9c4b12e, /// 0xa0c
			 0xe30472e9, /// 0xa10
			 0x354f0d08, /// 0xa14
			 0xa849bc05, /// 0xa18
			 0xbcf00734, /// 0xa1c
			 0x7ba0a09e, /// 0xa20
			 0xb5bc12d8, /// 0xa24
			 0xa76dc6bf, /// 0xa28
			 0x5e6b6810, /// 0xa2c
			 0x668b48af, /// 0xa30
			 0xea53e2a3, /// 0xa34
			 0xfbd7dc49, /// 0xa38
			 0xcb611ae5, /// 0xa3c
			 0x5e853673, /// 0xa40
			 0xac4dd9f9, /// 0xa44
			 0x048033ca, /// 0xa48
			 0xb03f074a, /// 0xa4c
			 0xc7f61ca5, /// 0xa50
			 0x7202a3c3, /// 0xa54
			 0x7d3048c2, /// 0xa58
			 0x8dcb4857, /// 0xa5c
			 0x2dd04ceb, /// 0xa60
			 0x354079dc, /// 0xa64
			 0x572fef77, /// 0xa68
			 0x76436388, /// 0xa6c
			 0xbd82376e, /// 0xa70
			 0x9d50ef00, /// 0xa74
			 0x53970a8e, /// 0xa78
			 0x820f4671, /// 0xa7c
			 0xff40ff0c, /// 0xa80
			 0x329b9be1, /// 0xa84
			 0x63b60ebf, /// 0xa88
			 0xdabf6bed, /// 0xa8c
			 0xc7613034, /// 0xa90
			 0xa5abbe03, /// 0xa94
			 0xa79bf4f4, /// 0xa98
			 0xe80cd329, /// 0xa9c
			 0x080f6207, /// 0xaa0
			 0x729a9830, /// 0xaa4
			 0xae8113da, /// 0xaa8
			 0x254c5477, /// 0xaac
			 0xbbc199d5, /// 0xab0
			 0x1011386a, /// 0xab4
			 0xfab03a98, /// 0xab8
			 0x31d390ef, /// 0xabc
			 0x06650031, /// 0xac0
			 0xb500d54b, /// 0xac4
			 0x5dc431f6, /// 0xac8
			 0x364bc733, /// 0xacc
			 0x94b18c7d, /// 0xad0
			 0x5479e108, /// 0xad4
			 0xa6fc51ab, /// 0xad8
			 0x0b4a29e2, /// 0xadc
			 0xc520f78d, /// 0xae0
			 0x41ce89fb, /// 0xae4
			 0x3180208b, /// 0xae8
			 0x881040f9, /// 0xaec
			 0x758ac98c, /// 0xaf0
			 0xc16dfd28, /// 0xaf4
			 0x9ca66a54, /// 0xaf8
			 0x2c8e1a08, /// 0xafc
			 0xde2f3ee4, /// 0xb00
			 0xadd6ce1a, /// 0xb04
			 0xdb118502, /// 0xb08
			 0x77646d89, /// 0xb0c
			 0x6f4f6cc3, /// 0xb10
			 0xc60610f0, /// 0xb14
			 0xbbb62318, /// 0xb18
			 0x4eda4e93, /// 0xb1c
			 0xfa05694a, /// 0xb20
			 0x53f98565, /// 0xb24
			 0x0068abf2, /// 0xb28
			 0x26345949, /// 0xb2c
			 0x7b064cb3, /// 0xb30
			 0x0d10ea47, /// 0xb34
			 0x2fd959ff, /// 0xb38
			 0x7bc0b519, /// 0xb3c
			 0x502b56e4, /// 0xb40
			 0x580c6d0b, /// 0xb44
			 0x6bbcd7af, /// 0xb48
			 0xcd84a64a, /// 0xb4c
			 0xbb1aabe8, /// 0xb50
			 0x8b800b60, /// 0xb54
			 0x5fe67950, /// 0xb58
			 0x14482696, /// 0xb5c
			 0x1165a153, /// 0xb60
			 0x7bd80d1c, /// 0xb64
			 0xa6baea0f, /// 0xb68
			 0x4810ad4b, /// 0xb6c
			 0x86cafc29, /// 0xb70
			 0x335c71c3, /// 0xb74
			 0x0319b3a4, /// 0xb78
			 0x0f94677a, /// 0xb7c
			 0x640ffc29, /// 0xb80
			 0xeb2f6287, /// 0xb84
			 0xe5a8ad38, /// 0xb88
			 0x80c9b542, /// 0xb8c
			 0x4599d6d6, /// 0xb90
			 0xfffdc2b1, /// 0xb94
			 0x83f26fdf, /// 0xb98
			 0xdc0142fa, /// 0xb9c
			 0x9cbd58a2, /// 0xba0
			 0x98a56df0, /// 0xba4
			 0x052a7617, /// 0xba8
			 0x7455a659, /// 0xbac
			 0x5a41ff4c, /// 0xbb0
			 0x1f6bef8a, /// 0xbb4
			 0xd19f7264, /// 0xbb8
			 0x6441ca36, /// 0xbbc
			 0xfaf7e205, /// 0xbc0
			 0xaa654085, /// 0xbc4
			 0x9c65b6d4, /// 0xbc8
			 0xe2d73226, /// 0xbcc
			 0x9b6a0cb3, /// 0xbd0
			 0xe59afc36, /// 0xbd4
			 0xbe2de78c, /// 0xbd8
			 0xe469c3a7, /// 0xbdc
			 0xe962e28a, /// 0xbe0
			 0xb7324893, /// 0xbe4
			 0x09df256a, /// 0xbe8
			 0xd4cf3318, /// 0xbec
			 0x1ec120eb, /// 0xbf0
			 0xab40167a, /// 0xbf4
			 0x6e8bed5f, /// 0xbf8
			 0xbce34290, /// 0xbfc
			 0xb3038c70, /// 0xc00
			 0x1a41e79e, /// 0xc04
			 0x62649366, /// 0xc08
			 0xa250be83, /// 0xc0c
			 0xef60f73e, /// 0xc10
			 0x10e86623, /// 0xc14
			 0x7a6e1ef1, /// 0xc18
			 0xfac63dcc, /// 0xc1c
			 0xcef62b24, /// 0xc20
			 0x09490ffb, /// 0xc24
			 0x35755b0d, /// 0xc28
			 0xe03a138c, /// 0xc2c
			 0x933488a4, /// 0xc30
			 0x6e2dd0b8, /// 0xc34
			 0xcce84990, /// 0xc38
			 0x4762d6bc, /// 0xc3c
			 0x12f1c3d2, /// 0xc40
			 0x4aba8a9f, /// 0xc44
			 0x173bb1c9, /// 0xc48
			 0x761d6b89, /// 0xc4c
			 0xd2d68999, /// 0xc50
			 0x4d2a1333, /// 0xc54
			 0x3977b39d, /// 0xc58
			 0x51d1e29d, /// 0xc5c
			 0x6a7082c3, /// 0xc60
			 0x358f9b3e, /// 0xc64
			 0xe003181e, /// 0xc68
			 0xfaf52426, /// 0xc6c
			 0x8a308408, /// 0xc70
			 0x3837f053, /// 0xc74
			 0x05312e49, /// 0xc78
			 0x319e8c47, /// 0xc7c
			 0xa6a0d7cb, /// 0xc80
			 0xa831d774, /// 0xc84
			 0x30fb2f9e, /// 0xc88
			 0x287b45fb, /// 0xc8c
			 0xf826be73, /// 0xc90
			 0xfb766f29, /// 0xc94
			 0x117233da, /// 0xc98
			 0xd2173d50, /// 0xc9c
			 0xa8f09fb9, /// 0xca0
			 0x50b04939, /// 0xca4
			 0x8b69c7c1, /// 0xca8
			 0x9807770e, /// 0xcac
			 0x6b04201b, /// 0xcb0
			 0x46e43f31, /// 0xcb4
			 0xfe58cbad, /// 0xcb8
			 0x179875c1, /// 0xcbc
			 0xc8c265cc, /// 0xcc0
			 0xbba60d03, /// 0xcc4
			 0x9440cad0, /// 0xcc8
			 0xf6978e08, /// 0xccc
			 0x71f4f051, /// 0xcd0
			 0x1d840e4b, /// 0xcd4
			 0x755ff5d3, /// 0xcd8
			 0x7b635be9, /// 0xcdc
			 0xd56ce9c6, /// 0xce0
			 0x1be1d3b3, /// 0xce4
			 0xb2258df5, /// 0xce8
			 0x85db740e, /// 0xcec
			 0xe6d8ae1b, /// 0xcf0
			 0x767491da, /// 0xcf4
			 0xc018aaed, /// 0xcf8
			 0x36c15f52, /// 0xcfc
			 0x3cbfb33f, /// 0xd00
			 0xfd75280b, /// 0xd04
			 0x79a70f36, /// 0xd08
			 0x609bcefd, /// 0xd0c
			 0x574c3b6a, /// 0xd10
			 0xd49229bc, /// 0xd14
			 0x22a367bf, /// 0xd18
			 0xcf3b93cd, /// 0xd1c
			 0x529174dd, /// 0xd20
			 0xa7062796, /// 0xd24
			 0xc846eaa1, /// 0xd28
			 0x6d30e146, /// 0xd2c
			 0x01855ff2, /// 0xd30
			 0xb7e1d0e4, /// 0xd34
			 0xf7cd821d, /// 0xd38
			 0xb8985cda, /// 0xd3c
			 0x0b33aad1, /// 0xd40
			 0xf9775092, /// 0xd44
			 0xc9f4bf31, /// 0xd48
			 0x7363d82c, /// 0xd4c
			 0xad45d488, /// 0xd50
			 0x37311b9e, /// 0xd54
			 0x7f5e99df, /// 0xd58
			 0xd0ebe1cd, /// 0xd5c
			 0xed40eef7, /// 0xd60
			 0x5ac40e5f, /// 0xd64
			 0x11bd8fb4, /// 0xd68
			 0x65a87a2f, /// 0xd6c
			 0x9b55e2c2, /// 0xd70
			 0x50347573, /// 0xd74
			 0xc3123cda, /// 0xd78
			 0x56c80617, /// 0xd7c
			 0xd72688a9, /// 0xd80
			 0x8cdba521, /// 0xd84
			 0xfd8d1d57, /// 0xd88
			 0x587b6284, /// 0xd8c
			 0xecc91c10, /// 0xd90
			 0x42d25073, /// 0xd94
			 0x9dd8b86f, /// 0xd98
			 0xbd4bfc2a, /// 0xd9c
			 0xb8c2472b, /// 0xda0
			 0xabde5262, /// 0xda4
			 0xc4a08aef, /// 0xda8
			 0x25d9115b, /// 0xdac
			 0x6b8bdf68, /// 0xdb0
			 0xecbdc07d, /// 0xdb4
			 0x8bfa05e5, /// 0xdb8
			 0x2ec161e6, /// 0xdbc
			 0x63fc6d34, /// 0xdc0
			 0x4b5a5e92, /// 0xdc4
			 0xd78224e8, /// 0xdc8
			 0xcf6665d8, /// 0xdcc
			 0xe1655803, /// 0xdd0
			 0xd14356f9, /// 0xdd4
			 0x4d0f5699, /// 0xdd8
			 0x5f3bf3ab, /// 0xddc
			 0x134dfd89, /// 0xde0
			 0x2bf87f4b, /// 0xde4
			 0xc95b0e0f, /// 0xde8
			 0xed3c9846, /// 0xdec
			 0x3b47dc48, /// 0xdf0
			 0x04abd235, /// 0xdf4
			 0xf6d767c3, /// 0xdf8
			 0x6eb92f0b, /// 0xdfc
			 0xb4bab516, /// 0xe00
			 0xabe7fec1, /// 0xe04
			 0xdf268a52, /// 0xe08
			 0x978f07ba, /// 0xe0c
			 0x28f9172c, /// 0xe10
			 0x539637e6, /// 0xe14
			 0x9990894a, /// 0xe18
			 0x3b3c62b9, /// 0xe1c
			 0xcab29fd6, /// 0xe20
			 0x7c058fda, /// 0xe24
			 0x148df142, /// 0xe28
			 0x75c3670f, /// 0xe2c
			 0xc4357d3e, /// 0xe30
			 0x40b585bb, /// 0xe34
			 0x6f1db416, /// 0xe38
			 0xe70c43f0, /// 0xe3c
			 0x7503d748, /// 0xe40
			 0x61b23575, /// 0xe44
			 0xe7500dfa, /// 0xe48
			 0x9789f640, /// 0xe4c
			 0x61fbec93, /// 0xe50
			 0x64d78b41, /// 0xe54
			 0x6362d865, /// 0xe58
			 0xdb9ebaa7, /// 0xe5c
			 0x6ec8b342, /// 0xe60
			 0xa244a036, /// 0xe64
			 0xc9263f7a, /// 0xe68
			 0xe4f6835b, /// 0xe6c
			 0x1495665f, /// 0xe70
			 0x80e86351, /// 0xe74
			 0xb757d41d, /// 0xe78
			 0x51a69c73, /// 0xe7c
			 0x6ace45f9, /// 0xe80
			 0x32e26532, /// 0xe84
			 0x60a7ff9b, /// 0xe88
			 0x886d90a1, /// 0xe8c
			 0x157fe31a, /// 0xe90
			 0x237495da, /// 0xe94
			 0x4b31bdc4, /// 0xe98
			 0x01953cb5, /// 0xe9c
			 0x05e650a5, /// 0xea0
			 0x69e63c54, /// 0xea4
			 0xaa12adf1, /// 0xea8
			 0xddb6e9fb, /// 0xeac
			 0x60d00ff6, /// 0xeb0
			 0xa0feca1c, /// 0xeb4
			 0x274e210a, /// 0xeb8
			 0xd663df51, /// 0xebc
			 0xa6b6a3bf, /// 0xec0
			 0x60eb5885, /// 0xec4
			 0x43d5af68, /// 0xec8
			 0x16f70923, /// 0xecc
			 0x526feeb2, /// 0xed0
			 0xbcbcdad2, /// 0xed4
			 0xb5c5b2d8, /// 0xed8
			 0x80171951, /// 0xedc
			 0xe68ccb48, /// 0xee0
			 0x9f329cea, /// 0xee4
			 0xa8bd1490, /// 0xee8
			 0x8c38975f, /// 0xeec
			 0x592f27aa, /// 0xef0
			 0xcc0ac253, /// 0xef4
			 0xa882ab9f, /// 0xef8
			 0x013a09d4, /// 0xefc
			 0x22285024, /// 0xf00
			 0x69f4a14e, /// 0xf04
			 0x26f545a5, /// 0xf08
			 0xb05bc323, /// 0xf0c
			 0x3b2452b6, /// 0xf10
			 0x476d4ba4, /// 0xf14
			 0x1d9be920, /// 0xf18
			 0x7d0612ef, /// 0xf1c
			 0x27147283, /// 0xf20
			 0x4820682a, /// 0xf24
			 0x689bce1a, /// 0xf28
			 0x5e9573ed, /// 0xf2c
			 0x0e7283bd, /// 0xf30
			 0x0be6b106, /// 0xf34
			 0x0532f719, /// 0xf38
			 0xe0c9fb3f, /// 0xf3c
			 0x6f67f678, /// 0xf40
			 0x86e465b0, /// 0xf44
			 0x343a96b1, /// 0xf48
			 0x5fd78adc, /// 0xf4c
			 0xecd7c47f, /// 0xf50
			 0x5459a3b1, /// 0xf54
			 0xec787c47, /// 0xf58
			 0xd4a860d9, /// 0xf5c
			 0xc588e0d2, /// 0xf60
			 0x3fc73449, /// 0xf64
			 0x98ef7643, /// 0xf68
			 0x9c61f0bc, /// 0xf6c
			 0xd2277778, /// 0xf70
			 0x63777a7a, /// 0xf74
			 0x3c963e01, /// 0xf78
			 0x95718514, /// 0xf7c
			 0x68431250, /// 0xf80
			 0x8effb4d2, /// 0xf84
			 0x841192aa, /// 0xf88
			 0x4fe1a186, /// 0xf8c
			 0x84be098f, /// 0xf90
			 0x81746a14, /// 0xf94
			 0xfc799ece, /// 0xf98
			 0x9b2991b8, /// 0xf9c
			 0xb4610ea9, /// 0xfa0
			 0x7d1b048d, /// 0xfa4
			 0xbc68c4a1, /// 0xfa8
			 0x698f4faf, /// 0xfac
			 0xa500cd5a, /// 0xfb0
			 0x329d68ba, /// 0xfb4
			 0xd9588e97, /// 0xfb8
			 0x8b7f4307, /// 0xfbc
			 0xbe18f5aa, /// 0xfc0
			 0x6e4eac29, /// 0xfc4
			 0xad847a24, /// 0xfc8
			 0x34c2e0ac, /// 0xfcc
			 0x7427aad3, /// 0xfd0
			 0x53bf33f9, /// 0xfd4
			 0xd810ad9f, /// 0xfd8
			 0xc0158f7b, /// 0xfdc
			 0xa196dfde, /// 0xfe0
			 0x1c622600, /// 0xfe4
			 0x4831c2cf, /// 0xfe8
			 0xb5e447be, /// 0xfec
			 0x9017ad71, /// 0xff0
			 0x31dc58bb, /// 0xff4
			 0xfcc27d78, /// 0xff8
			 0xac1c3468 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x935dfea2, /// 0x0
			 0x1fd5f88e, /// 0x4
			 0x79dd14d9, /// 0x8
			 0x7e360d92, /// 0xc
			 0x466540d7, /// 0x10
			 0xd9ebae9e, /// 0x14
			 0x5b2e313f, /// 0x18
			 0xd5b6c110, /// 0x1c
			 0x6d6665e8, /// 0x20
			 0x3fb91d61, /// 0x24
			 0x4484b5e6, /// 0x28
			 0x31117cd9, /// 0x2c
			 0x8070554f, /// 0x30
			 0x1f14f9e8, /// 0x34
			 0x8b0a31df, /// 0x38
			 0xa0c6c613, /// 0x3c
			 0xd4c0901c, /// 0x40
			 0x3ab454fb, /// 0x44
			 0x0c158c8e, /// 0x48
			 0x70c84930, /// 0x4c
			 0xee90a3cf, /// 0x50
			 0x5c3563b1, /// 0x54
			 0x9b297168, /// 0x58
			 0xb3a4852e, /// 0x5c
			 0x1bc6e9ed, /// 0x60
			 0xe9c82d8b, /// 0x64
			 0x172af8a9, /// 0x68
			 0xed0d6d17, /// 0x6c
			 0xa9fc2481, /// 0x70
			 0xf21b7cac, /// 0x74
			 0x54458615, /// 0x78
			 0x8141272e, /// 0x7c
			 0x8620fafb, /// 0x80
			 0x4b06bab4, /// 0x84
			 0xf599327f, /// 0x88
			 0x34deb3f0, /// 0x8c
			 0xa885418c, /// 0x90
			 0x25bded90, /// 0x94
			 0x5e8d022f, /// 0x98
			 0x52bcd545, /// 0x9c
			 0x6bde1894, /// 0xa0
			 0xd44d67d3, /// 0xa4
			 0xd15c3592, /// 0xa8
			 0x8c825485, /// 0xac
			 0x0c57d0c0, /// 0xb0
			 0x96e34906, /// 0xb4
			 0x39ce5d6a, /// 0xb8
			 0xb4c963bb, /// 0xbc
			 0xe7dcec80, /// 0xc0
			 0x14be866e, /// 0xc4
			 0x0e7c9ce5, /// 0xc8
			 0xa2d1ab96, /// 0xcc
			 0x9f90f3ae, /// 0xd0
			 0x1df1aa0a, /// 0xd4
			 0xce597daa, /// 0xd8
			 0x40fb3954, /// 0xdc
			 0xab901261, /// 0xe0
			 0xaa68f5df, /// 0xe4
			 0x1fb03263, /// 0xe8
			 0xf872afbc, /// 0xec
			 0xff1b8f7a, /// 0xf0
			 0xea4a0b23, /// 0xf4
			 0xc38359d6, /// 0xf8
			 0x989f318b, /// 0xfc
			 0x8739193f, /// 0x100
			 0x6736b46c, /// 0x104
			 0xf489ec8c, /// 0x108
			 0x1678c909, /// 0x10c
			 0xe86a7c89, /// 0x110
			 0xde9cb930, /// 0x114
			 0x37bde84d, /// 0x118
			 0x2d2cf08c, /// 0x11c
			 0xf5c7f2f3, /// 0x120
			 0xa1e4d43b, /// 0x124
			 0xaecd9064, /// 0x128
			 0x00005fe0, /// 0x12c
			 0x8e9a887e, /// 0x130
			 0xd623ff59, /// 0x134
			 0x7672b34c, /// 0x138
			 0x55a69416, /// 0x13c
			 0x1f481cbe, /// 0x140
			 0x0b5b277e, /// 0x144
			 0xa1a20a66, /// 0x148
			 0xacfcb7ae, /// 0x14c
			 0xb460e430, /// 0x150
			 0x8f387043, /// 0x154
			 0x8f390181, /// 0x158
			 0x0aeb6d53, /// 0x15c
			 0x0289ddb5, /// 0x160
			 0x366072ea, /// 0x164
			 0x812afbcf, /// 0x168
			 0x509df289, /// 0x16c
			 0x71f62618, /// 0x170
			 0x1575bf9d, /// 0x174
			 0x343780b8, /// 0x178
			 0xc2ec9e4b, /// 0x17c
			 0x2405e3c3, /// 0x180
			 0x422253cd, /// 0x184
			 0xe7020406, /// 0x188
			 0x71361b35, /// 0x18c
			 0x3e6b5480, /// 0x190
			 0xad1b33d5, /// 0x194
			 0x9ac2c3fb, /// 0x198
			 0xe5d38cd5, /// 0x19c
			 0xf204e883, /// 0x1a0
			 0x8babea8d, /// 0x1a4
			 0xb0a965bb, /// 0x1a8
			 0xff5f79d3, /// 0x1ac
			 0x1df3ca83, /// 0x1b0
			 0xcd1dd514, /// 0x1b4
			 0x71f53839, /// 0x1b8
			 0xbd68094a, /// 0x1bc
			 0x3b916217, /// 0x1c0
			 0x1e20c90e, /// 0x1c4
			 0x8fcfb09e, /// 0x1c8
			 0xc461758f, /// 0x1cc
			 0x226b88fb, /// 0x1d0
			 0x9ed248e5, /// 0x1d4
			 0x7267d40a, /// 0x1d8
			 0x46a136c2, /// 0x1dc
			 0xe8fc0cc9, /// 0x1e0
			 0xce31d19d, /// 0x1e4
			 0x7ec3749c, /// 0x1e8
			 0x5a6c7643, /// 0x1ec
			 0x6f00f260, /// 0x1f0
			 0x963b11f2, /// 0x1f4
			 0xd7ca4e92, /// 0x1f8
			 0x4534c8a1, /// 0x1fc
			 0x261bca6c, /// 0x200
			 0xda085dbd, /// 0x204
			 0xd2cca502, /// 0x208
			 0x6635e60d, /// 0x20c
			 0xf5deabff, /// 0x210
			 0x378f781a, /// 0x214
			 0x89a7ddfb, /// 0x218
			 0x3efe17f9, /// 0x21c
			 0x1eaa1a8e, /// 0x220
			 0xe8fd1f37, /// 0x224
			 0xd7387431, /// 0x228
			 0xe5f37d93, /// 0x22c
			 0xf5c7a55d, /// 0x230
			 0xc393f3a4, /// 0x234
			 0xd03ae030, /// 0x238
			 0x327ffa60, /// 0x23c
			 0x33b45b04, /// 0x240
			 0x07d1e897, /// 0x244
			 0x6480106f, /// 0x248
			 0xf2c026ca, /// 0x24c
			 0xa0452a94, /// 0x250
			 0x8eecf1d4, /// 0x254
			 0x593e5aa4, /// 0x258
			 0x430d334f, /// 0x25c
			 0x1e8d089f, /// 0x260
			 0xfdf8bbcc, /// 0x264
			 0xd977770b, /// 0x268
			 0xa4355088, /// 0x26c
			 0xc66d2fdd, /// 0x270
			 0x2bd5652b, /// 0x274
			 0x648260e3, /// 0x278
			 0x92367ac4, /// 0x27c
			 0xfacfe15d, /// 0x280
			 0x4a325253, /// 0x284
			 0x3fdf5d51, /// 0x288
			 0xf6adcaee, /// 0x28c
			 0x91bd2472, /// 0x290
			 0x9daad471, /// 0x294
			 0x611ed599, /// 0x298
			 0xd2d0f69f, /// 0x29c
			 0xe7b1c5b4, /// 0x2a0
			 0x6cb0ffc7, /// 0x2a4
			 0x8d1790cd, /// 0x2a8
			 0xae6c8f47, /// 0x2ac
			 0x889f5599, /// 0x2b0
			 0xfdcbb2de, /// 0x2b4
			 0xc4dd7b3f, /// 0x2b8
			 0x6508b6ec, /// 0x2bc
			 0x6bb26060, /// 0x2c0
			 0xe83bebf2, /// 0x2c4
			 0x08e23408, /// 0x2c8
			 0xd3272878, /// 0x2cc
			 0xb13e6b7f, /// 0x2d0
			 0xf0fdd158, /// 0x2d4
			 0x8ec3e91d, /// 0x2d8
			 0x31e44632, /// 0x2dc
			 0x9ba8a039, /// 0x2e0
			 0xd2ae686e, /// 0x2e4
			 0x0e9b0555, /// 0x2e8
			 0x122b9680, /// 0x2ec
			 0x077a41aa, /// 0x2f0
			 0xf5a0b59d, /// 0x2f4
			 0xc45574c9, /// 0x2f8
			 0x39861d9d, /// 0x2fc
			 0xbfccb5b0, /// 0x300
			 0xb761fb62, /// 0x304
			 0x1f08e191, /// 0x308
			 0x4a12f1b4, /// 0x30c
			 0x2e2b756b, /// 0x310
			 0xb9e51a9d, /// 0x314
			 0x5a6c7bd7, /// 0x318
			 0xd0c6b957, /// 0x31c
			 0x7b8f3a4a, /// 0x320
			 0x028e6f41, /// 0x324
			 0xcb82b4f8, /// 0x328
			 0xeb49a995, /// 0x32c
			 0xbe572b4f, /// 0x330
			 0x9ec27b8d, /// 0x334
			 0xe206dd73, /// 0x338
			 0xbcbd8fd6, /// 0x33c
			 0x9575bec5, /// 0x340
			 0x81b7ab8c, /// 0x344
			 0x1c087d72, /// 0x348
			 0x056dbaf4, /// 0x34c
			 0x64098a60, /// 0x350
			 0xc1bcec80, /// 0x354
			 0xeacd2266, /// 0x358
			 0xa75fd33b, /// 0x35c
			 0x5181e9bf, /// 0x360
			 0x0861f948, /// 0x364
			 0x0e1d245c, /// 0x368
			 0x8eaac8bd, /// 0x36c
			 0x86f5a148, /// 0x370
			 0xaa872724, /// 0x374
			 0x23727b63, /// 0x378
			 0xba03b95d, /// 0x37c
			 0xc12410ab, /// 0x380
			 0xa64ae55a, /// 0x384
			 0x029f31a1, /// 0x388
			 0x4e375043, /// 0x38c
			 0x7400fc24, /// 0x390
			 0xc581001c, /// 0x394
			 0x7eaa8136, /// 0x398
			 0xf75f4569, /// 0x39c
			 0xc214689b, /// 0x3a0
			 0x2bb239c4, /// 0x3a4
			 0x0da5e453, /// 0x3a8
			 0x87c8486d, /// 0x3ac
			 0x538c7a7c, /// 0x3b0
			 0x212d6c48, /// 0x3b4
			 0xc2c58d5e, /// 0x3b8
			 0x3dd07d67, /// 0x3bc
			 0x5fc6bfa9, /// 0x3c0
			 0xc40e7ced, /// 0x3c4
			 0x7966bea9, /// 0x3c8
			 0x6dac920c, /// 0x3cc
			 0x3f71910f, /// 0x3d0
			 0xf55074ce, /// 0x3d4
			 0x1572c8e9, /// 0x3d8
			 0xa8fa3d30, /// 0x3dc
			 0xccf8a40d, /// 0x3e0
			 0x3cc7ae4a, /// 0x3e4
			 0x78997e44, /// 0x3e8
			 0xb8723886, /// 0x3ec
			 0xdd0ab4e3, /// 0x3f0
			 0xb59053df, /// 0x3f4
			 0xee2fd0b4, /// 0x3f8
			 0x1859d65a, /// 0x3fc
			 0x922bd5c5, /// 0x400
			 0xb9d70643, /// 0x404
			 0xa0560656, /// 0x408
			 0xeeb9fe02, /// 0x40c
			 0x9776a314, /// 0x410
			 0x26b5ae0b, /// 0x414
			 0x8ea0b0d6, /// 0x418
			 0xb3490474, /// 0x41c
			 0xfc191d3f, /// 0x420
			 0x854a11a2, /// 0x424
			 0x0de3a2d3, /// 0x428
			 0x97bb6c4d, /// 0x42c
			 0xece0b024, /// 0x430
			 0xb9a4987a, /// 0x434
			 0x35976a59, /// 0x438
			 0x99dd5d84, /// 0x43c
			 0x71382601, /// 0x440
			 0x6fde540b, /// 0x444
			 0xeeb83738, /// 0x448
			 0x4dd1e313, /// 0x44c
			 0x21600b7c, /// 0x450
			 0x5978b68e, /// 0x454
			 0x9ca847b7, /// 0x458
			 0x539c1af5, /// 0x45c
			 0xf8f539b4, /// 0x460
			 0x95c0e19b, /// 0x464
			 0x8047d429, /// 0x468
			 0x046a8336, /// 0x46c
			 0x860dc2d4, /// 0x470
			 0x638906c6, /// 0x474
			 0xa1611646, /// 0x478
			 0x023904f7, /// 0x47c
			 0x7be50027, /// 0x480
			 0x5aa2dae2, /// 0x484
			 0xc4b0e1fc, /// 0x488
			 0x28731a49, /// 0x48c
			 0x051db4f2, /// 0x490
			 0xc895d7e3, /// 0x494
			 0x39233752, /// 0x498
			 0x76f49c3a, /// 0x49c
			 0xe46440ed, /// 0x4a0
			 0x287af833, /// 0x4a4
			 0xe1c1f1e9, /// 0x4a8
			 0x21c17c59, /// 0x4ac
			 0x43df68f2, /// 0x4b0
			 0x617be549, /// 0x4b4
			 0x6da40eee, /// 0x4b8
			 0xd57d6816, /// 0x4bc
			 0xb9eb41e3, /// 0x4c0
			 0xa23b02ec, /// 0x4c4
			 0x10b34e3b, /// 0x4c8
			 0x6a610295, /// 0x4cc
			 0x9604a442, /// 0x4d0
			 0x93f7fb3b, /// 0x4d4
			 0x35368f26, /// 0x4d8
			 0x8641f574, /// 0x4dc
			 0x5c583f55, /// 0x4e0
			 0x4839a9aa, /// 0x4e4
			 0xd090e869, /// 0x4e8
			 0x7cda6de0, /// 0x4ec
			 0x8cd081a3, /// 0x4f0
			 0xfc019293, /// 0x4f4
			 0x8621abe1, /// 0x4f8
			 0xcfaa1fa6, /// 0x4fc
			 0x7e91eae7, /// 0x500
			 0x63685c47, /// 0x504
			 0x9d21212a, /// 0x508
			 0xafe12340, /// 0x50c
			 0x103819a6, /// 0x510
			 0x851d48a4, /// 0x514
			 0x52514149, /// 0x518
			 0x71badb71, /// 0x51c
			 0xe0abb3f6, /// 0x520
			 0xf71f18da, /// 0x524
			 0x0364b532, /// 0x528
			 0xcf6f176b, /// 0x52c
			 0x3dc810eb, /// 0x530
			 0xd9d15ce5, /// 0x534
			 0xb20b45bd, /// 0x538
			 0xb7c71dde, /// 0x53c
			 0x8e6e5319, /// 0x540
			 0x00fdbe6b, /// 0x544
			 0x9e94f2ab, /// 0x548
			 0x93c48631, /// 0x54c
			 0x045a5332, /// 0x550
			 0xaa7f13a1, /// 0x554
			 0x52554567, /// 0x558
			 0x83069bdc, /// 0x55c
			 0xa850c6fd, /// 0x560
			 0x3d1bd272, /// 0x564
			 0xe96bed76, /// 0x568
			 0xb23099aa, /// 0x56c
			 0xdb2bf7fc, /// 0x570
			 0x5c4d57ea, /// 0x574
			 0x51f23796, /// 0x578
			 0x3fe20f72, /// 0x57c
			 0x8bb62f98, /// 0x580
			 0x95e3c140, /// 0x584
			 0x60cb94e6, /// 0x588
			 0x225d8076, /// 0x58c
			 0xe67d8d05, /// 0x590
			 0xe6a0c87d, /// 0x594
			 0xc6595228, /// 0x598
			 0x75297aa0, /// 0x59c
			 0xd850c2cb, /// 0x5a0
			 0xfe59db3f, /// 0x5a4
			 0xdbd7d971, /// 0x5a8
			 0x152c42b5, /// 0x5ac
			 0x4a090e44, /// 0x5b0
			 0x2756c5a2, /// 0x5b4
			 0x702ccb44, /// 0x5b8
			 0xaaa2c9e7, /// 0x5bc
			 0x4b8210ad, /// 0x5c0
			 0x76b0f373, /// 0x5c4
			 0x4d8fc706, /// 0x5c8
			 0x6dea5bcb, /// 0x5cc
			 0xd9383be5, /// 0x5d0
			 0xc293181d, /// 0x5d4
			 0x1050e23f, /// 0x5d8
			 0x2edf277b, /// 0x5dc
			 0x9cad1124, /// 0x5e0
			 0x59e78d88, /// 0x5e4
			 0x72ed8ffc, /// 0x5e8
			 0x09f4ee3b, /// 0x5ec
			 0x058de37d, /// 0x5f0
			 0xb35070d5, /// 0x5f4
			 0x5b5a5204, /// 0x5f8
			 0x372b8493, /// 0x5fc
			 0x28eb6ea2, /// 0x600
			 0xbf2a4063, /// 0x604
			 0xa7655e69, /// 0x608
			 0x121b92a6, /// 0x60c
			 0x16d4c467, /// 0x610
			 0xdc93e387, /// 0x614
			 0x9a963a3d, /// 0x618
			 0x4c956203, /// 0x61c
			 0x39c9ec6e, /// 0x620
			 0x42b14701, /// 0x624
			 0xef39820d, /// 0x628
			 0xc39cbb08, /// 0x62c
			 0x7a488c12, /// 0x630
			 0x430bd99c, /// 0x634
			 0x619dbd36, /// 0x638
			 0x9136c3e2, /// 0x63c
			 0x9f73807e, /// 0x640
			 0x08cc9d7e, /// 0x644
			 0xb4040553, /// 0x648
			 0xe80d3f0f, /// 0x64c
			 0x4e93a4a9, /// 0x650
			 0xefee60aa, /// 0x654
			 0x4b61ef3e, /// 0x658
			 0x15d5e084, /// 0x65c
			 0xc4e8a4a5, /// 0x660
			 0x2903f96c, /// 0x664
			 0xdfbe5b17, /// 0x668
			 0x648c9eac, /// 0x66c
			 0xdaefa038, /// 0x670
			 0xa8e5d6ca, /// 0x674
			 0xee79ef75, /// 0x678
			 0x9953c2d6, /// 0x67c
			 0x798de480, /// 0x680
			 0x0bc44217, /// 0x684
			 0x0710cfe9, /// 0x688
			 0xbbf6295f, /// 0x68c
			 0x361216fd, /// 0x690
			 0x26704f09, /// 0x694
			 0x813b0b4e, /// 0x698
			 0xdf5e8b62, /// 0x69c
			 0xd88cfde4, /// 0x6a0
			 0xc4305811, /// 0x6a4
			 0x81964b5e, /// 0x6a8
			 0x54752f40, /// 0x6ac
			 0x7ab4ee94, /// 0x6b0
			 0x14703dba, /// 0x6b4
			 0x60bf7c3d, /// 0x6b8
			 0x070e428e, /// 0x6bc
			 0xeb429d53, /// 0x6c0
			 0x1d88ff99, /// 0x6c4
			 0xd130c480, /// 0x6c8
			 0x5e4cfbe1, /// 0x6cc
			 0x0ee0af04, /// 0x6d0
			 0x2c7d7e11, /// 0x6d4
			 0xdc804240, /// 0x6d8
			 0xff57c3ff, /// 0x6dc
			 0xd346e0a9, /// 0x6e0
			 0x1ce5ad51, /// 0x6e4
			 0x3933ee8c, /// 0x6e8
			 0x57df63e3, /// 0x6ec
			 0x9a6a3e49, /// 0x6f0
			 0xd06b8480, /// 0x6f4
			 0x5977fbb3, /// 0x6f8
			 0x6743279c, /// 0x6fc
			 0x643791bb, /// 0x700
			 0x2110b736, /// 0x704
			 0xb7ac157e, /// 0x708
			 0x22ce608b, /// 0x70c
			 0x74d3bb48, /// 0x710
			 0x5e642ccb, /// 0x714
			 0x8dfbf051, /// 0x718
			 0x083149e6, /// 0x71c
			 0x8b9437ba, /// 0x720
			 0xc7eb4364, /// 0x724
			 0x538df091, /// 0x728
			 0x8419735a, /// 0x72c
			 0xb0505fce, /// 0x730
			 0x4a4815e0, /// 0x734
			 0xafcbf2da, /// 0x738
			 0xedf7223a, /// 0x73c
			 0x936027e7, /// 0x740
			 0x0dd09b66, /// 0x744
			 0xa807dbb8, /// 0x748
			 0xca134d5f, /// 0x74c
			 0x11f15379, /// 0x750
			 0x7fe78271, /// 0x754
			 0xb663e7b1, /// 0x758
			 0x5ce76877, /// 0x75c
			 0x32f2a431, /// 0x760
			 0xccc6ff68, /// 0x764
			 0x41997b39, /// 0x768
			 0x6dbc8efc, /// 0x76c
			 0xc21d1ae8, /// 0x770
			 0x31ca09c3, /// 0x774
			 0xccadd412, /// 0x778
			 0x2e937ec9, /// 0x77c
			 0x1236c76c, /// 0x780
			 0x43ad580e, /// 0x784
			 0xe4a92f32, /// 0x788
			 0x421df214, /// 0x78c
			 0xfaa28793, /// 0x790
			 0x12dd0edd, /// 0x794
			 0xbec748cd, /// 0x798
			 0xa9fd0da4, /// 0x79c
			 0xed3193ea, /// 0x7a0
			 0x540fa7ab, /// 0x7a4
			 0x3a58d955, /// 0x7a8
			 0xe69803d8, /// 0x7ac
			 0x484cc498, /// 0x7b0
			 0xeaf76293, /// 0x7b4
			 0x1ea1f860, /// 0x7b8
			 0xcf6a4802, /// 0x7bc
			 0xe5784583, /// 0x7c0
			 0x6aa8e62f, /// 0x7c4
			 0x00826ee5, /// 0x7c8
			 0x88c6edb6, /// 0x7cc
			 0x2751b11e, /// 0x7d0
			 0x8bba8106, /// 0x7d4
			 0xe47c1104, /// 0x7d8
			 0xb7fac0f0, /// 0x7dc
			 0xb00ab4f1, /// 0x7e0
			 0xf0da2f9f, /// 0x7e4
			 0x1e3c7ae5, /// 0x7e8
			 0x62e8a158, /// 0x7ec
			 0xf127906d, /// 0x7f0
			 0xaab5da22, /// 0x7f4
			 0x88f04277, /// 0x7f8
			 0x02e30f8d, /// 0x7fc
			 0x333495b8, /// 0x800
			 0xa95b77c0, /// 0x804
			 0xdf82537a, /// 0x808
			 0x1983f6b6, /// 0x80c
			 0xdfe8058a, /// 0x810
			 0xc0de0fbb, /// 0x814
			 0x8e5d82c1, /// 0x818
			 0xa0db81c6, /// 0x81c
			 0x73156e5a, /// 0x820
			 0x91de8f28, /// 0x824
			 0x52fafcc9, /// 0x828
			 0x96faf286, /// 0x82c
			 0xb837ae7f, /// 0x830
			 0xa9a77815, /// 0x834
			 0x84fa605f, /// 0x838
			 0x35bdab0b, /// 0x83c
			 0x3a8ca7cd, /// 0x840
			 0x34a697a6, /// 0x844
			 0x52227254, /// 0x848
			 0x0a25379d, /// 0x84c
			 0x8a89cc55, /// 0x850
			 0xf329b4ba, /// 0x854
			 0xc25dabaf, /// 0x858
			 0x2e78d2eb, /// 0x85c
			 0x2777cc90, /// 0x860
			 0x88914a60, /// 0x864
			 0x3492da1f, /// 0x868
			 0xf1757eca, /// 0x86c
			 0xaa2544c5, /// 0x870
			 0xb07ae9f7, /// 0x874
			 0x03aea22f, /// 0x878
			 0xb21aa4ad, /// 0x87c
			 0x61ffcd1b, /// 0x880
			 0xa0553f4b, /// 0x884
			 0x10e23585, /// 0x888
			 0x05c489cf, /// 0x88c
			 0x926cdbef, /// 0x890
			 0xba8adb32, /// 0x894
			 0xa28a0162, /// 0x898
			 0x8b0ca531, /// 0x89c
			 0x9ca09758, /// 0x8a0
			 0x4d655242, /// 0x8a4
			 0x08994560, /// 0x8a8
			 0x2234cadc, /// 0x8ac
			 0x9ae071f2, /// 0x8b0
			 0x8403a68c, /// 0x8b4
			 0xf427719f, /// 0x8b8
			 0x1481eb26, /// 0x8bc
			 0xc932f81e, /// 0x8c0
			 0xa3ab76ab, /// 0x8c4
			 0x3e0a4262, /// 0x8c8
			 0xe6bd605d, /// 0x8cc
			 0x79f5cf27, /// 0x8d0
			 0xc3be3a65, /// 0x8d4
			 0xcb5fa163, /// 0x8d8
			 0x47a46d10, /// 0x8dc
			 0x4a0dd9e9, /// 0x8e0
			 0x6411d53d, /// 0x8e4
			 0x2df31b90, /// 0x8e8
			 0xb8620bef, /// 0x8ec
			 0xc636df70, /// 0x8f0
			 0xdb783907, /// 0x8f4
			 0xfeaf24d6, /// 0x8f8
			 0xa5264fe9, /// 0x8fc
			 0x2719a996, /// 0x900
			 0x422eef7e, /// 0x904
			 0xe2fd442b, /// 0x908
			 0xfed23b38, /// 0x90c
			 0x71099aef, /// 0x910
			 0x9f21b3e1, /// 0x914
			 0xe9129e01, /// 0x918
			 0x546f4903, /// 0x91c
			 0x2d9b27d2, /// 0x920
			 0xd4fb75d5, /// 0x924
			 0x9eb240f5, /// 0x928
			 0x4078e495, /// 0x92c
			 0x5e4b6a65, /// 0x930
			 0x43a7e053, /// 0x934
			 0xc617d4b2, /// 0x938
			 0xb9931933, /// 0x93c
			 0x76ad9016, /// 0x940
			 0x05ba87a2, /// 0x944
			 0x9db9505a, /// 0x948
			 0x0307a50c, /// 0x94c
			 0xe0b83fc5, /// 0x950
			 0x85d91583, /// 0x954
			 0x132b2dda, /// 0x958
			 0x04b97ee0, /// 0x95c
			 0xfbe5d969, /// 0x960
			 0x0da68e27, /// 0x964
			 0x8365ebe5, /// 0x968
			 0xd3419569, /// 0x96c
			 0x1b18f361, /// 0x970
			 0xd01d0f01, /// 0x974
			 0x18518ce5, /// 0x978
			 0xcd081b3a, /// 0x97c
			 0x289b6989, /// 0x980
			 0xc89b9ae5, /// 0x984
			 0x7cf18213, /// 0x988
			 0x99099603, /// 0x98c
			 0x3ecf06d4, /// 0x990
			 0xb481a858, /// 0x994
			 0xcdbd13c7, /// 0x998
			 0x7dc0e3c5, /// 0x99c
			 0x32f848f4, /// 0x9a0
			 0x828fb3d7, /// 0x9a4
			 0xd29ae8ed, /// 0x9a8
			 0x9b23dfdf, /// 0x9ac
			 0x822ed465, /// 0x9b0
			 0xfc195077, /// 0x9b4
			 0xc8a6a4f1, /// 0x9b8
			 0xe90ba15b, /// 0x9bc
			 0x9018f129, /// 0x9c0
			 0x38489014, /// 0x9c4
			 0x20ca1ddb, /// 0x9c8
			 0x08f30654, /// 0x9cc
			 0x29059608, /// 0x9d0
			 0x3fcfd48c, /// 0x9d4
			 0x9531ea38, /// 0x9d8
			 0x478ba192, /// 0x9dc
			 0xb3205691, /// 0x9e0
			 0xff36c2f2, /// 0x9e4
			 0xb7d9ef98, /// 0x9e8
			 0x732693e2, /// 0x9ec
			 0x7b73233a, /// 0x9f0
			 0x0a113e7b, /// 0x9f4
			 0xafd5f977, /// 0x9f8
			 0x03a3dcad, /// 0x9fc
			 0x22a3c94b, /// 0xa00
			 0x9b713962, /// 0xa04
			 0x7a9d0034, /// 0xa08
			 0xa7294003, /// 0xa0c
			 0x18f9fe7d, /// 0xa10
			 0x47944116, /// 0xa14
			 0x9e44affc, /// 0xa18
			 0x7c1fc208, /// 0xa1c
			 0xd7f79bbc, /// 0xa20
			 0xeeb75f29, /// 0xa24
			 0x805f79bc, /// 0xa28
			 0xae037686, /// 0xa2c
			 0x026bf341, /// 0xa30
			 0x8bcc3aba, /// 0xa34
			 0x6d9e0ccb, /// 0xa38
			 0x2c470bb9, /// 0xa3c
			 0x3e118aa4, /// 0xa40
			 0xd3464acc, /// 0xa44
			 0x76bff25d, /// 0xa48
			 0x3b6d894e, /// 0xa4c
			 0xc26da901, /// 0xa50
			 0xe47be571, /// 0xa54
			 0x6dc1a52d, /// 0xa58
			 0xc84e1867, /// 0xa5c
			 0xcbe5c21b, /// 0xa60
			 0x750c7db2, /// 0xa64
			 0xeab65de0, /// 0xa68
			 0x9da2655a, /// 0xa6c
			 0x8c01e002, /// 0xa70
			 0x48c50dc6, /// 0xa74
			 0x8495a447, /// 0xa78
			 0x9e883b9d, /// 0xa7c
			 0x8b7112d6, /// 0xa80
			 0xc11ceab4, /// 0xa84
			 0xc056ff44, /// 0xa88
			 0x5dd24cb2, /// 0xa8c
			 0x9da5ba1d, /// 0xa90
			 0xc5e42265, /// 0xa94
			 0x4618bdc8, /// 0xa98
			 0x9cfba032, /// 0xa9c
			 0x3418e952, /// 0xaa0
			 0x1263c894, /// 0xaa4
			 0x318c015a, /// 0xaa8
			 0xbee45559, /// 0xaac
			 0x77f52ff8, /// 0xab0
			 0xdbcd1ca0, /// 0xab4
			 0x3430beeb, /// 0xab8
			 0x875a3214, /// 0xabc
			 0x5f538b6d, /// 0xac0
			 0x7af6b28c, /// 0xac4
			 0xc66480a0, /// 0xac8
			 0xc9b2ffe4, /// 0xacc
			 0x0ee32df6, /// 0xad0
			 0xba39bcbc, /// 0xad4
			 0xcc0e94f7, /// 0xad8
			 0x64eceec6, /// 0xadc
			 0x7e47b159, /// 0xae0
			 0x301b4040, /// 0xae4
			 0x15874438, /// 0xae8
			 0xc05097c1, /// 0xaec
			 0x9d715796, /// 0xaf0
			 0xe234829c, /// 0xaf4
			 0xa067cd39, /// 0xaf8
			 0x3e385437, /// 0xafc
			 0x609fbd22, /// 0xb00
			 0xdc5e8a8a, /// 0xb04
			 0x1c06ee8d, /// 0xb08
			 0x93002fdf, /// 0xb0c
			 0x0406bc35, /// 0xb10
			 0xcfc01e06, /// 0xb14
			 0xb7b31fe6, /// 0xb18
			 0x618b5708, /// 0xb1c
			 0x3a675ad0, /// 0xb20
			 0x2645708d, /// 0xb24
			 0xcc3b73f0, /// 0xb28
			 0x825158b8, /// 0xb2c
			 0xf9032613, /// 0xb30
			 0xbff29d15, /// 0xb34
			 0x0eefaf43, /// 0xb38
			 0xaa171835, /// 0xb3c
			 0xdd92c015, /// 0xb40
			 0xcb51f5df, /// 0xb44
			 0x0be52f5b, /// 0xb48
			 0xf5934a4b, /// 0xb4c
			 0x0a829653, /// 0xb50
			 0xda88992a, /// 0xb54
			 0x64bf93f6, /// 0xb58
			 0x2494a388, /// 0xb5c
			 0x0627c623, /// 0xb60
			 0x9faa83aa, /// 0xb64
			 0x6953259f, /// 0xb68
			 0xa050097c, /// 0xb6c
			 0x9c7092b7, /// 0xb70
			 0xd7ea6174, /// 0xb74
			 0x65fd5ef2, /// 0xb78
			 0xea04974d, /// 0xb7c
			 0xc96fdd6b, /// 0xb80
			 0x4af1980c, /// 0xb84
			 0xdcdeaa49, /// 0xb88
			 0xda3cca27, /// 0xb8c
			 0x20c4f9b1, /// 0xb90
			 0xc5a2f370, /// 0xb94
			 0xe26bcc56, /// 0xb98
			 0xffafc1e3, /// 0xb9c
			 0x86e77a35, /// 0xba0
			 0xdff3a272, /// 0xba4
			 0x4a6601dc, /// 0xba8
			 0x076be8ec, /// 0xbac
			 0xb682b4fd, /// 0xbb0
			 0xd768bf1c, /// 0xbb4
			 0xe58a55bd, /// 0xbb8
			 0x8bb301cf, /// 0xbbc
			 0x7b4622b2, /// 0xbc0
			 0x227a124e, /// 0xbc4
			 0x2971a6af, /// 0xbc8
			 0xcf4222e4, /// 0xbcc
			 0xcb32de3f, /// 0xbd0
			 0x36b60026, /// 0xbd4
			 0x6f6aef42, /// 0xbd8
			 0x421c4f8c, /// 0xbdc
			 0xb0612a10, /// 0xbe0
			 0xd06179a2, /// 0xbe4
			 0xa29afe55, /// 0xbe8
			 0xfe643cf2, /// 0xbec
			 0x811b3cc5, /// 0xbf0
			 0x757e5b12, /// 0xbf4
			 0x659b12a1, /// 0xbf8
			 0xe7fa040c, /// 0xbfc
			 0xb345bbc7, /// 0xc00
			 0x56fa67d7, /// 0xc04
			 0x7736b5c0, /// 0xc08
			 0x242c1be1, /// 0xc0c
			 0xe111f6d0, /// 0xc10
			 0x77dd6b32, /// 0xc14
			 0x54ed5760, /// 0xc18
			 0x0f9cca00, /// 0xc1c
			 0x6349c7c6, /// 0xc20
			 0x59e29657, /// 0xc24
			 0x4ba49a98, /// 0xc28
			 0x7c090526, /// 0xc2c
			 0x3a4172a8, /// 0xc30
			 0x13d2988b, /// 0xc34
			 0xdde04856, /// 0xc38
			 0x0648d56f, /// 0xc3c
			 0x066f3759, /// 0xc40
			 0xdce934e6, /// 0xc44
			 0x25148194, /// 0xc48
			 0x32780fc0, /// 0xc4c
			 0xa4166f5a, /// 0xc50
			 0xbc13c9d8, /// 0xc54
			 0x8491d83b, /// 0xc58
			 0xa752524a, /// 0xc5c
			 0x8e00ca5a, /// 0xc60
			 0x5e2e287b, /// 0xc64
			 0x56568ac0, /// 0xc68
			 0x43b58ed6, /// 0xc6c
			 0xe26cdcfa, /// 0xc70
			 0x33a9f81a, /// 0xc74
			 0x7350a3d4, /// 0xc78
			 0x3a1be49f, /// 0xc7c
			 0xea01fbf7, /// 0xc80
			 0xd094c61a, /// 0xc84
			 0xf66c2735, /// 0xc88
			 0x2192840e, /// 0xc8c
			 0xd4d07b35, /// 0xc90
			 0xe27dc1d0, /// 0xc94
			 0x9be310ef, /// 0xc98
			 0x8ea207b1, /// 0xc9c
			 0xcb99ca5a, /// 0xca0
			 0x4467526b, /// 0xca4
			 0xcbf28dd2, /// 0xca8
			 0x0e91e7d9, /// 0xcac
			 0x5a1d0fb1, /// 0xcb0
			 0x7ee8574b, /// 0xcb4
			 0x15a3d157, /// 0xcb8
			 0x06f417d9, /// 0xcbc
			 0xb43cba7a, /// 0xcc0
			 0x833e9c1c, /// 0xcc4
			 0x78bfe6e8, /// 0xcc8
			 0xa0cdfbf9, /// 0xccc
			 0x14551b3e, /// 0xcd0
			 0x690129f1, /// 0xcd4
			 0xf0f704bd, /// 0xcd8
			 0xa609e05a, /// 0xcdc
			 0xd4d9d324, /// 0xce0
			 0x55283d56, /// 0xce4
			 0x904d933a, /// 0xce8
			 0xd121a284, /// 0xcec
			 0x565b9e71, /// 0xcf0
			 0x1ca7f822, /// 0xcf4
			 0xa305610b, /// 0xcf8
			 0xd84864b9, /// 0xcfc
			 0xbc71144c, /// 0xd00
			 0x18bd45ed, /// 0xd04
			 0x1e877fdf, /// 0xd08
			 0xe5c6c131, /// 0xd0c
			 0xa74c65bf, /// 0xd10
			 0x5ac3980f, /// 0xd14
			 0xff278846, /// 0xd18
			 0xccba76e3, /// 0xd1c
			 0x234a2416, /// 0xd20
			 0xd242241a, /// 0xd24
			 0x5505bfa6, /// 0xd28
			 0x53c5c524, /// 0xd2c
			 0x6b6e0475, /// 0xd30
			 0x66fdd935, /// 0xd34
			 0x57a43e6a, /// 0xd38
			 0x95a28dae, /// 0xd3c
			 0xa80e7a46, /// 0xd40
			 0x22f3768d, /// 0xd44
			 0x39e65bcc, /// 0xd48
			 0xa3aa052e, /// 0xd4c
			 0x8d25c9c3, /// 0xd50
			 0xd3f7ca3c, /// 0xd54
			 0x11e07ff1, /// 0xd58
			 0xf5285e73, /// 0xd5c
			 0x58f98244, /// 0xd60
			 0x2fd2b140, /// 0xd64
			 0xab3e4de8, /// 0xd68
			 0x7cc16147, /// 0xd6c
			 0xcbbb1544, /// 0xd70
			 0xe6664b26, /// 0xd74
			 0xbc2f7644, /// 0xd78
			 0xea652500, /// 0xd7c
			 0x396e845d, /// 0xd80
			 0x64ad3740, /// 0xd84
			 0xf7fc3ec7, /// 0xd88
			 0xa67763ba, /// 0xd8c
			 0xa9a81617, /// 0xd90
			 0x7ab002e7, /// 0xd94
			 0x79c00cc4, /// 0xd98
			 0x86c34db9, /// 0xd9c
			 0x1f5f4741, /// 0xda0
			 0x15582cd3, /// 0xda4
			 0x3901d29e, /// 0xda8
			 0x2addd452, /// 0xdac
			 0x2299847c, /// 0xdb0
			 0xcdb3fad6, /// 0xdb4
			 0xa553bcfd, /// 0xdb8
			 0xaccd1cc5, /// 0xdbc
			 0xbb77ac96, /// 0xdc0
			 0x0afbaeb6, /// 0xdc4
			 0x2aecf260, /// 0xdc8
			 0x4a94b0b5, /// 0xdcc
			 0x7d6d14a9, /// 0xdd0
			 0x3410f5c4, /// 0xdd4
			 0xdeba081d, /// 0xdd8
			 0x3a409c46, /// 0xddc
			 0x33504a90, /// 0xde0
			 0xe8443d19, /// 0xde4
			 0xfa6d7d53, /// 0xde8
			 0x87d71e79, /// 0xdec
			 0x4ac66e09, /// 0xdf0
			 0x3f15d1a0, /// 0xdf4
			 0x36cfec7d, /// 0xdf8
			 0x2faeb67b, /// 0xdfc
			 0x03289f17, /// 0xe00
			 0xbe43895b, /// 0xe04
			 0xbff596cc, /// 0xe08
			 0xe5b421ea, /// 0xe0c
			 0xb33c502a, /// 0xe10
			 0xcbc0d2c8, /// 0xe14
			 0x1fc1c112, /// 0xe18
			 0xbdbab4a4, /// 0xe1c
			 0xaa96f4ce, /// 0xe20
			 0xf9a9913d, /// 0xe24
			 0x0f60e0bc, /// 0xe28
			 0x2fdf62d7, /// 0xe2c
			 0x96a0db17, /// 0xe30
			 0xe11fa27d, /// 0xe34
			 0xdb062275, /// 0xe38
			 0x9653a31a, /// 0xe3c
			 0x657bddd1, /// 0xe40
			 0x833e5096, /// 0xe44
			 0x423fcfe4, /// 0xe48
			 0xc2d23eb5, /// 0xe4c
			 0xc78a6d69, /// 0xe50
			 0xa1f181fe, /// 0xe54
			 0x2f10644b, /// 0xe58
			 0xdca236c7, /// 0xe5c
			 0x6960e92a, /// 0xe60
			 0xa108e6f6, /// 0xe64
			 0xd9ffbc73, /// 0xe68
			 0xd2e85209, /// 0xe6c
			 0x4bade991, /// 0xe70
			 0x76d8b1f8, /// 0xe74
			 0x782db41f, /// 0xe78
			 0x2585ea46, /// 0xe7c
			 0x6b3c82d4, /// 0xe80
			 0x073ad0c3, /// 0xe84
			 0xd9237d22, /// 0xe88
			 0x4313cf84, /// 0xe8c
			 0x59d83716, /// 0xe90
			 0xc6e81273, /// 0xe94
			 0x443be07f, /// 0xe98
			 0xcf8ccaeb, /// 0xe9c
			 0x86c3b2d8, /// 0xea0
			 0x2e5e5438, /// 0xea4
			 0xf2c87e6a, /// 0xea8
			 0x0903b639, /// 0xeac
			 0x3446e0e2, /// 0xeb0
			 0xe7fff057, /// 0xeb4
			 0xf3e0eedc, /// 0xeb8
			 0x95ba1ee2, /// 0xebc
			 0x657361c0, /// 0xec0
			 0x471f70c7, /// 0xec4
			 0x0f782cb9, /// 0xec8
			 0xbbc489db, /// 0xecc
			 0xf252ac97, /// 0xed0
			 0x2fb2b1d0, /// 0xed4
			 0x853c5302, /// 0xed8
			 0x56379507, /// 0xedc
			 0x408c05da, /// 0xee0
			 0x2ed33119, /// 0xee4
			 0x94e55954, /// 0xee8
			 0x51c2784b, /// 0xeec
			 0x7fe8d512, /// 0xef0
			 0x82ff69aa, /// 0xef4
			 0xb386c8d7, /// 0xef8
			 0x85364ba8, /// 0xefc
			 0x43bee638, /// 0xf00
			 0x3ca5ddca, /// 0xf04
			 0x75af2f9c, /// 0xf08
			 0xf7679f27, /// 0xf0c
			 0xad2affd4, /// 0xf10
			 0x7b587f4e, /// 0xf14
			 0xd4e99814, /// 0xf18
			 0xa6370bdf, /// 0xf1c
			 0x2becee94, /// 0xf20
			 0x7a63114c, /// 0xf24
			 0xc9944a44, /// 0xf28
			 0xa52a644a, /// 0xf2c
			 0x62a503d7, /// 0xf30
			 0x81abfc84, /// 0xf34
			 0x8d44bf2f, /// 0xf38
			 0x66a084b1, /// 0xf3c
			 0x52e5b8ae, /// 0xf40
			 0x1d8e6df3, /// 0xf44
			 0xb3b9451a, /// 0xf48
			 0xb56bb46f, /// 0xf4c
			 0xabcced37, /// 0xf50
			 0xd4956d2a, /// 0xf54
			 0xd106b2da, /// 0xf58
			 0x1e83a715, /// 0xf5c
			 0xf373bce2, /// 0xf60
			 0xb14913f2, /// 0xf64
			 0x7d779a4c, /// 0xf68
			 0xf628480c, /// 0xf6c
			 0xac385cb4, /// 0xf70
			 0x4c0e152f, /// 0xf74
			 0x57611c19, /// 0xf78
			 0x32711ae1, /// 0xf7c
			 0x90948973, /// 0xf80
			 0x75cca3ab, /// 0xf84
			 0xd2eb08ff, /// 0xf88
			 0x8f7bb9f9, /// 0xf8c
			 0x2cf588d1, /// 0xf90
			 0x71e03359, /// 0xf94
			 0xbb766e72, /// 0xf98
			 0xe0c0b5b4, /// 0xf9c
			 0x31911e4a, /// 0xfa0
			 0x8154de75, /// 0xfa4
			 0x5d5dfec0, /// 0xfa8
			 0x5545c364, /// 0xfac
			 0x6b9b180b, /// 0xfb0
			 0x84b6d3ad, /// 0xfb4
			 0x080159e2, /// 0xfb8
			 0xaa601c2f, /// 0xfbc
			 0x24cf2430, /// 0xfc0
			 0x890c224e, /// 0xfc4
			 0x09526a30, /// 0xfc8
			 0xf6807bf6, /// 0xfcc
			 0x1e7441c0, /// 0xfd0
			 0x93b202e9, /// 0xfd4
			 0xb12ee660, /// 0xfd8
			 0xefe88f91, /// 0xfdc
			 0xa8c55885, /// 0xfe0
			 0x27e6791c, /// 0xfe4
			 0x71625c8f, /// 0xfe8
			 0x7c825442, /// 0xfec
			 0xd9632093, /// 0xff0
			 0xd8f7a11d, /// 0xff4
			 0x6d4e32f5, /// 0xff8
			 0x82cdc04d /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xb05b0017, /// 0x0
			 0x3037e261, /// 0x4
			 0xfec8aa03, /// 0x8
			 0x216f9625, /// 0xc
			 0xa5c7c5bb, /// 0x10
			 0x3a28886a, /// 0x14
			 0x9bcecf1c, /// 0x18
			 0xdb75d034, /// 0x1c
			 0xbdcd4077, /// 0x20
			 0x2340d619, /// 0x24
			 0x3b525bbf, /// 0x28
			 0xc79fd8b9, /// 0x2c
			 0xe077c881, /// 0x30
			 0xf649621d, /// 0x34
			 0xbd1294d4, /// 0x38
			 0x8c152dc9, /// 0x3c
			 0x5a9f7c40, /// 0x40
			 0xf68a9b7d, /// 0x44
			 0xadd3e3af, /// 0x48
			 0xeedbe98d, /// 0x4c
			 0xfe3d94af, /// 0x50
			 0xa7bb03ba, /// 0x54
			 0x1f94237f, /// 0x58
			 0x1460980f, /// 0x5c
			 0xd42303af, /// 0x60
			 0x9cd953de, /// 0x64
			 0x0362f582, /// 0x68
			 0x834f04c8, /// 0x6c
			 0xcacc7df0, /// 0x70
			 0x86af74f5, /// 0x74
			 0xd05c0e24, /// 0x78
			 0x7275b6d3, /// 0x7c
			 0x2243f898, /// 0x80
			 0xcd4d882d, /// 0x84
			 0xece9c009, /// 0x88
			 0x73e0247f, /// 0x8c
			 0x5185c8f0, /// 0x90
			 0x9d4119d8, /// 0x94
			 0x59f9b04b, /// 0x98
			 0x0a76c4d2, /// 0x9c
			 0xebefe37e, /// 0xa0
			 0x298c1ff8, /// 0xa4
			 0x43b1e8c5, /// 0xa8
			 0xa1afb75f, /// 0xac
			 0x8c89e238, /// 0xb0
			 0x020c3dd5, /// 0xb4
			 0x14eb24d3, /// 0xb8
			 0x1d4612f4, /// 0xbc
			 0x0420478f, /// 0xc0
			 0xce310827, /// 0xc4
			 0x0b146e7d, /// 0xc8
			 0x929bef79, /// 0xcc
			 0x6687421d, /// 0xd0
			 0xd660f10f, /// 0xd4
			 0xd5c6697f, /// 0xd8
			 0xbcce530b, /// 0xdc
			 0xa05170d7, /// 0xe0
			 0xda345a6d, /// 0xe4
			 0xab1ab90b, /// 0xe8
			 0xe100e1a3, /// 0xec
			 0x343f8242, /// 0xf0
			 0x13a53148, /// 0xf4
			 0x434bd320, /// 0xf8
			 0x0c53b0ee, /// 0xfc
			 0xe63bbbb1, /// 0x100
			 0x023ac430, /// 0x104
			 0xc02a7bd5, /// 0x108
			 0x127a1f1d, /// 0x10c
			 0x3a20499d, /// 0x110
			 0x8d631b42, /// 0x114
			 0x206f1290, /// 0x118
			 0x46eff4fd, /// 0x11c
			 0x52fcb8cc, /// 0x120
			 0x5cd38b41, /// 0x124
			 0xc457159d, /// 0x128
			 0x49bee282, /// 0x12c
			 0x9abd0116, /// 0x130
			 0x3678ed9e, /// 0x134
			 0x41bdb715, /// 0x138
			 0xfd77cdbe, /// 0x13c
			 0x4ce9598b, /// 0x140
			 0x1291b769, /// 0x144
			 0xedaafaba, /// 0x148
			 0x46807cfd, /// 0x14c
			 0x92bce092, /// 0x150
			 0xd4fbd7bb, /// 0x154
			 0x77e9f824, /// 0x158
			 0x8c468a18, /// 0x15c
			 0xea4db8b4, /// 0x160
			 0x765a2f9b, /// 0x164
			 0x1ce063a4, /// 0x168
			 0x5642f3dc, /// 0x16c
			 0x88ebc00b, /// 0x170
			 0x8c2f87d0, /// 0x174
			 0xc7570edb, /// 0x178
			 0x66d9975e, /// 0x17c
			 0x9fe45910, /// 0x180
			 0xcc1d8edc, /// 0x184
			 0xa04675b0, /// 0x188
			 0xf8580c9e, /// 0x18c
			 0xc4910c28, /// 0x190
			 0x2494ed7e, /// 0x194
			 0xebe01792, /// 0x198
			 0xc8e76c7a, /// 0x19c
			 0x2bdb2c81, /// 0x1a0
			 0x64a02e11, /// 0x1a4
			 0x25f772e2, /// 0x1a8
			 0xd579df70, /// 0x1ac
			 0xc1fa612c, /// 0x1b0
			 0xddfd257e, /// 0x1b4
			 0xeaa1f689, /// 0x1b8
			 0xd8333121, /// 0x1bc
			 0x62853378, /// 0x1c0
			 0x026c9b1d, /// 0x1c4
			 0x559112e6, /// 0x1c8
			 0x4a9cad93, /// 0x1cc
			 0x5ae6b152, /// 0x1d0
			 0x933357ea, /// 0x1d4
			 0x3a602516, /// 0x1d8
			 0xf02ec661, /// 0x1dc
			 0x83133472, /// 0x1e0
			 0xb840de79, /// 0x1e4
			 0x1bc855db, /// 0x1e8
			 0xb1ac251b, /// 0x1ec
			 0xcab4898c, /// 0x1f0
			 0xc22778f6, /// 0x1f4
			 0x5171d784, /// 0x1f8
			 0x6db4ee9e, /// 0x1fc
			 0xcf053c8a, /// 0x200
			 0x5fb4a2b5, /// 0x204
			 0x6a283944, /// 0x208
			 0xd518d46e, /// 0x20c
			 0xc6e6307b, /// 0x210
			 0xbf2a73fb, /// 0x214
			 0xecc4c3c1, /// 0x218
			 0x4035bca9, /// 0x21c
			 0x7658b187, /// 0x220
			 0x3bc30e25, /// 0x224
			 0x247ecd79, /// 0x228
			 0x47b46c23, /// 0x22c
			 0x7420b6e9, /// 0x230
			 0x5ec19dda, /// 0x234
			 0xd9d1584f, /// 0x238
			 0x75fe0844, /// 0x23c
			 0xd1accf26, /// 0x240
			 0x7e9f3cfe, /// 0x244
			 0x7da1b689, /// 0x248
			 0x5573ec5b, /// 0x24c
			 0xecbbb522, /// 0x250
			 0x14a91f23, /// 0x254
			 0x3ed80cdf, /// 0x258
			 0x1604ca37, /// 0x25c
			 0x12f822c4, /// 0x260
			 0x04b75258, /// 0x264
			 0xa3b539ea, /// 0x268
			 0x2687948f, /// 0x26c
			 0x265c1681, /// 0x270
			 0xc2f67244, /// 0x274
			 0xf7a47fa8, /// 0x278
			 0xd439dce2, /// 0x27c
			 0xf1c9ff01, /// 0x280
			 0x6cc0cd0b, /// 0x284
			 0x6e3de905, /// 0x288
			 0xc43cc1ec, /// 0x28c
			 0x7b9e036a, /// 0x290
			 0x190012ec, /// 0x294
			 0xd9577b31, /// 0x298
			 0x7207de1d, /// 0x29c
			 0x09116957, /// 0x2a0
			 0x720aa1d7, /// 0x2a4
			 0x3381ddbd, /// 0x2a8
			 0x39392b65, /// 0x2ac
			 0xab4fbbda, /// 0x2b0
			 0xeaf4e212, /// 0x2b4
			 0xd13c1f39, /// 0x2b8
			 0x4c58901b, /// 0x2bc
			 0x75eacfba, /// 0x2c0
			 0x035b5131, /// 0x2c4
			 0x4683019c, /// 0x2c8
			 0xa9e3fee5, /// 0x2cc
			 0xa7731207, /// 0x2d0
			 0x01fab5d4, /// 0x2d4
			 0x7f5e7e94, /// 0x2d8
			 0xd60dc103, /// 0x2dc
			 0x6b9f3b7e, /// 0x2e0
			 0xcc959b61, /// 0x2e4
			 0xde5f260e, /// 0x2e8
			 0xc76a27c5, /// 0x2ec
			 0x50d41902, /// 0x2f0
			 0xce440441, /// 0x2f4
			 0x32e62b2c, /// 0x2f8
			 0xf57a960b, /// 0x2fc
			 0xfe8bc552, /// 0x300
			 0x702a36cf, /// 0x304
			 0x4cdc70db, /// 0x308
			 0x407ee4c4, /// 0x30c
			 0x67d959f9, /// 0x310
			 0xab7cc86c, /// 0x314
			 0x4660fe4f, /// 0x318
			 0xa928b08e, /// 0x31c
			 0x8f81eafe, /// 0x320
			 0x953e7f4b, /// 0x324
			 0x2a3a2b74, /// 0x328
			 0xdebc9ec6, /// 0x32c
			 0xa601eab6, /// 0x330
			 0xe9e85f00, /// 0x334
			 0xa9bd1a94, /// 0x338
			 0xe30dfaff, /// 0x33c
			 0xaf27d60c, /// 0x340
			 0x80e85e23, /// 0x344
			 0xa3a5f239, /// 0x348
			 0x8bc978a2, /// 0x34c
			 0x362ea391, /// 0x350
			 0x80ca616b, /// 0x354
			 0x798e4e93, /// 0x358
			 0xf96e45a7, /// 0x35c
			 0xb9733319, /// 0x360
			 0x15eb215c, /// 0x364
			 0x7c87ed93, /// 0x368
			 0xf77d9064, /// 0x36c
			 0x2faac16f, /// 0x370
			 0xd899a3ee, /// 0x374
			 0xb8698a72, /// 0x378
			 0x3193639a, /// 0x37c
			 0x4b78b1e6, /// 0x380
			 0xfee313bf, /// 0x384
			 0x470dfaba, /// 0x388
			 0xc20c0871, /// 0x38c
			 0xa18ecd8e, /// 0x390
			 0x5f06ed0f, /// 0x394
			 0x4a72ab3f, /// 0x398
			 0x9d7c7bbe, /// 0x39c
			 0x2e95c9c3, /// 0x3a0
			 0xd0e27573, /// 0x3a4
			 0x4654bd8c, /// 0x3a8
			 0xda02d23e, /// 0x3ac
			 0xc43826af, /// 0x3b0
			 0xb1f8ef39, /// 0x3b4
			 0x8796de15, /// 0x3b8
			 0xcbf9e8e5, /// 0x3bc
			 0xb43ae940, /// 0x3c0
			 0x7a49d3b8, /// 0x3c4
			 0x24400339, /// 0x3c8
			 0xcd595054, /// 0x3cc
			 0xac4f3256, /// 0x3d0
			 0xe328f22c, /// 0x3d4
			 0xd7897df8, /// 0x3d8
			 0x28553e7c, /// 0x3dc
			 0xf64ca1a5, /// 0x3e0
			 0x8236ab67, /// 0x3e4
			 0xf0cf7031, /// 0x3e8
			 0x3e9d3ed2, /// 0x3ec
			 0x68ad572b, /// 0x3f0
			 0x2362e1f7, /// 0x3f4
			 0xb06dd5fa, /// 0x3f8
			 0x27b3ecc9, /// 0x3fc
			 0xbe1bc987, /// 0x400
			 0x939c2fb1, /// 0x404
			 0x827ff755, /// 0x408
			 0x579c6890, /// 0x40c
			 0x1d24f618, /// 0x410
			 0x471fd940, /// 0x414
			 0x90f8d6ea, /// 0x418
			 0x05337412, /// 0x41c
			 0x949ac2de, /// 0x420
			 0x151ddab7, /// 0x424
			 0x07319c92, /// 0x428
			 0x4bbec7dd, /// 0x42c
			 0xd419fe35, /// 0x430
			 0x9380ef0b, /// 0x434
			 0xa5363ba8, /// 0x438
			 0xed0df972, /// 0x43c
			 0xce3f37c2, /// 0x440
			 0xbe5b3743, /// 0x444
			 0x099fc5c0, /// 0x448
			 0xd740ce3f, /// 0x44c
			 0x89294094, /// 0x450
			 0xce18f503, /// 0x454
			 0xbf38353e, /// 0x458
			 0x5691a22a, /// 0x45c
			 0x3dce432d, /// 0x460
			 0xf042f7fd, /// 0x464
			 0x9a09a963, /// 0x468
			 0xaa597519, /// 0x46c
			 0x5535fc68, /// 0x470
			 0xb4778436, /// 0x474
			 0x36deab16, /// 0x478
			 0x8be80fe4, /// 0x47c
			 0x0ccc6cf7, /// 0x480
			 0xcd92956a, /// 0x484
			 0x4e30d2eb, /// 0x488
			 0x323e3f3d, /// 0x48c
			 0xd9f3f3db, /// 0x490
			 0xd07869f9, /// 0x494
			 0x9f3bce80, /// 0x498
			 0x37de3e55, /// 0x49c
			 0xf532f0e7, /// 0x4a0
			 0xc95ee8d1, /// 0x4a4
			 0xfb0f337b, /// 0x4a8
			 0x92133d2f, /// 0x4ac
			 0x95a79652, /// 0x4b0
			 0x6211a2e8, /// 0x4b4
			 0x6de60d32, /// 0x4b8
			 0xcb029c9c, /// 0x4bc
			 0x2c240378, /// 0x4c0
			 0x179c083c, /// 0x4c4
			 0x9f76ea6c, /// 0x4c8
			 0xf77dba3f, /// 0x4cc
			 0x5d7da6e9, /// 0x4d0
			 0xc66ec704, /// 0x4d4
			 0xe9344472, /// 0x4d8
			 0xb968efb7, /// 0x4dc
			 0x2bcb0038, /// 0x4e0
			 0xcd19dba2, /// 0x4e4
			 0x5d95b326, /// 0x4e8
			 0x5601ac10, /// 0x4ec
			 0xb0c56431, /// 0x4f0
			 0xe2a036ea, /// 0x4f4
			 0xc6b81172, /// 0x4f8
			 0x3ea4ae48, /// 0x4fc
			 0xcbc0e560, /// 0x500
			 0xc1ce7183, /// 0x504
			 0xf2e0b286, /// 0x508
			 0x5702b7c6, /// 0x50c
			 0x5cb98a63, /// 0x510
			 0x021abb47, /// 0x514
			 0xc6677387, /// 0x518
			 0x823bbc9a, /// 0x51c
			 0xd63d47a9, /// 0x520
			 0xd71f8065, /// 0x524
			 0x2e4d9bfc, /// 0x528
			 0x1c8182a3, /// 0x52c
			 0x222425f5, /// 0x530
			 0xeaa38d71, /// 0x534
			 0x1da4d9df, /// 0x538
			 0xde40942f, /// 0x53c
			 0x15004d3a, /// 0x540
			 0xbb602802, /// 0x544
			 0x19f18575, /// 0x548
			 0xc5ad97e3, /// 0x54c
			 0xac0295a1, /// 0x550
			 0xd13cfa47, /// 0x554
			 0xeab259d7, /// 0x558
			 0x597dd43a, /// 0x55c
			 0x1376eb6e, /// 0x560
			 0x484d7faf, /// 0x564
			 0xa047d088, /// 0x568
			 0x74ebec33, /// 0x56c
			 0x8094a874, /// 0x570
			 0xc030f7ff, /// 0x574
			 0x53c9b85a, /// 0x578
			 0x7af37082, /// 0x57c
			 0x691bba2f, /// 0x580
			 0xd3ccfb93, /// 0x584
			 0x7fcafd7a, /// 0x588
			 0x2b115411, /// 0x58c
			 0x21cd4b95, /// 0x590
			 0x091078a8, /// 0x594
			 0x3500dcd6, /// 0x598
			 0xc471efd5, /// 0x59c
			 0x597764fa, /// 0x5a0
			 0xa6a66ac5, /// 0x5a4
			 0x00336b6c, /// 0x5a8
			 0x36acf7fe, /// 0x5ac
			 0x58227e9a, /// 0x5b0
			 0x52959f6c, /// 0x5b4
			 0x5cddb8da, /// 0x5b8
			 0x6814de83, /// 0x5bc
			 0xaac6d44f, /// 0x5c0
			 0x1a56fd88, /// 0x5c4
			 0xe5671c13, /// 0x5c8
			 0xd1f51601, /// 0x5cc
			 0xa99d9712, /// 0x5d0
			 0x0f7e4462, /// 0x5d4
			 0x5ae31562, /// 0x5d8
			 0x8dd8f05a, /// 0x5dc
			 0x7f201994, /// 0x5e0
			 0xcf306571, /// 0x5e4
			 0xac5c5398, /// 0x5e8
			 0x5ab687d3, /// 0x5ec
			 0xd6cebc82, /// 0x5f0
			 0x488c82c1, /// 0x5f4
			 0xdfad0bc3, /// 0x5f8
			 0x23d7b89b, /// 0x5fc
			 0x04e7d191, /// 0x600
			 0x6833eb8b, /// 0x604
			 0x2c0437ba, /// 0x608
			 0x751565bf, /// 0x60c
			 0xe022684a, /// 0x610
			 0x21fe108c, /// 0x614
			 0x5ccb799f, /// 0x618
			 0x0d834e0e, /// 0x61c
			 0xf95fdc2b, /// 0x620
			 0x6ba7ea2c, /// 0x624
			 0x3d64a7a6, /// 0x628
			 0x9aa13122, /// 0x62c
			 0x545bbebd, /// 0x630
			 0xb64ca5af, /// 0x634
			 0xd74a0202, /// 0x638
			 0x1bc52352, /// 0x63c
			 0x4d05558a, /// 0x640
			 0xe18b57b4, /// 0x644
			 0xb4a8cdf4, /// 0x648
			 0x01168064, /// 0x64c
			 0xed808457, /// 0x650
			 0xc3da463a, /// 0x654
			 0x438e61d9, /// 0x658
			 0xa1d82be5, /// 0x65c
			 0x1e0ee321, /// 0x660
			 0x70eb2a69, /// 0x664
			 0x3079750b, /// 0x668
			 0x3a902f8e, /// 0x66c
			 0xfac2396b, /// 0x670
			 0x639feaa3, /// 0x674
			 0xcf994440, /// 0x678
			 0x32965dfb, /// 0x67c
			 0x7c664a52, /// 0x680
			 0x14ff90a7, /// 0x684
			 0xdd51d09c, /// 0x688
			 0xcd586410, /// 0x68c
			 0x72b85fa3, /// 0x690
			 0x353ded16, /// 0x694
			 0xbeb6b499, /// 0x698
			 0xe251d90a, /// 0x69c
			 0xdc473bf2, /// 0x6a0
			 0xc98b977e, /// 0x6a4
			 0xbf54373c, /// 0x6a8
			 0xfbf2998f, /// 0x6ac
			 0x2150c9a5, /// 0x6b0
			 0xab9a5162, /// 0x6b4
			 0xaaf6b299, /// 0x6b8
			 0xfdcdfa79, /// 0x6bc
			 0x96679983, /// 0x6c0
			 0x31943078, /// 0x6c4
			 0xac11b23c, /// 0x6c8
			 0x358c0396, /// 0x6cc
			 0x5c0800ed, /// 0x6d0
			 0x5307e2de, /// 0x6d4
			 0xfbad3ce4, /// 0x6d8
			 0x7c4094f4, /// 0x6dc
			 0xb4a9d962, /// 0x6e0
			 0x065ba767, /// 0x6e4
			 0x8a71d969, /// 0x6e8
			 0x63a28abc, /// 0x6ec
			 0x2c83bd4e, /// 0x6f0
			 0xf7a0a50c, /// 0x6f4
			 0xbf84ec94, /// 0x6f8
			 0x9f600328, /// 0x6fc
			 0x852412f3, /// 0x700
			 0x59a8166f, /// 0x704
			 0x3883e3e4, /// 0x708
			 0xa64fd2ca, /// 0x70c
			 0x101f3f7e, /// 0x710
			 0x3f86821b, /// 0x714
			 0x5d9599b8, /// 0x718
			 0xe2ed2905, /// 0x71c
			 0xc6b92a3b, /// 0x720
			 0x13c594d9, /// 0x724
			 0x1586bcd7, /// 0x728
			 0x52c38644, /// 0x72c
			 0x7bee9150, /// 0x730
			 0xab8b567b, /// 0x734
			 0x6d6e7aec, /// 0x738
			 0xb4872299, /// 0x73c
			 0xba3bb13b, /// 0x740
			 0x1220321b, /// 0x744
			 0x449a2092, /// 0x748
			 0x97b75dc5, /// 0x74c
			 0x746aaa69, /// 0x750
			 0xd72aef47, /// 0x754
			 0x6533c9ca, /// 0x758
			 0x8449db38, /// 0x75c
			 0xca369994, /// 0x760
			 0x8474c58a, /// 0x764
			 0xe2bba5a2, /// 0x768
			 0xada08e7e, /// 0x76c
			 0x7f9f8f4d, /// 0x770
			 0x44901c72, /// 0x774
			 0x1daeabb8, /// 0x778
			 0x17c0c4ab, /// 0x77c
			 0xdc253042, /// 0x780
			 0x1db4873d, /// 0x784
			 0x866c46aa, /// 0x788
			 0xe8071b69, /// 0x78c
			 0x48b23ecd, /// 0x790
			 0xe0427ab6, /// 0x794
			 0x8fdd87cd, /// 0x798
			 0x89899ab6, /// 0x79c
			 0x4c153885, /// 0x7a0
			 0x4a99cb85, /// 0x7a4
			 0xa36c3bc4, /// 0x7a8
			 0x2957093a, /// 0x7ac
			 0x65a0f634, /// 0x7b0
			 0x95755b44, /// 0x7b4
			 0x43b18da9, /// 0x7b8
			 0x9bd4c4c5, /// 0x7bc
			 0x61f6e223, /// 0x7c0
			 0xd7e863f0, /// 0x7c4
			 0xb4db08dc, /// 0x7c8
			 0xe74bdc5a, /// 0x7cc
			 0xb1ef3ac8, /// 0x7d0
			 0xcbcb0499, /// 0x7d4
			 0x11cfb3d0, /// 0x7d8
			 0xc58f92e7, /// 0x7dc
			 0x75d1b812, /// 0x7e0
			 0x644f9abe, /// 0x7e4
			 0xa523a08a, /// 0x7e8
			 0x6a622241, /// 0x7ec
			 0x3050a9b9, /// 0x7f0
			 0xf4dfe7c3, /// 0x7f4
			 0x79d953de, /// 0x7f8
			 0xd7e556ba, /// 0x7fc
			 0x0f5e26b1, /// 0x800
			 0xe5b0cc74, /// 0x804
			 0xe5a053f8, /// 0x808
			 0xbb45d521, /// 0x80c
			 0x27baa961, /// 0x810
			 0x6f3bfefa, /// 0x814
			 0xc7b55760, /// 0x818
			 0x119aae58, /// 0x81c
			 0x70f3a202, /// 0x820
			 0x5858baa4, /// 0x824
			 0x566ef592, /// 0x828
			 0x124aff95, /// 0x82c
			 0x643dcf6f, /// 0x830
			 0x1c1456db, /// 0x834
			 0xf633a0b1, /// 0x838
			 0xee9c178b, /// 0x83c
			 0x8a97c040, /// 0x840
			 0x732eb87d, /// 0x844
			 0xd252d4d2, /// 0x848
			 0xe8465640, /// 0x84c
			 0x2d520226, /// 0x850
			 0x7f3bb2ce, /// 0x854
			 0x39f05359, /// 0x858
			 0x6e389b22, /// 0x85c
			 0x307d0ab7, /// 0x860
			 0x8f54df9a, /// 0x864
			 0x0e31c5b8, /// 0x868
			 0x4871fca5, /// 0x86c
			 0x318753e5, /// 0x870
			 0x0eb6ccea, /// 0x874
			 0xac42b4ba, /// 0x878
			 0xde4b1dde, /// 0x87c
			 0x363c833c, /// 0x880
			 0x02481a80, /// 0x884
			 0x7f37d341, /// 0x888
			 0x88ddbd22, /// 0x88c
			 0x8268cebe, /// 0x890
			 0x32252dc7, /// 0x894
			 0x45561a45, /// 0x898
			 0x7adf0c05, /// 0x89c
			 0xa5e33888, /// 0x8a0
			 0x2fa84c12, /// 0x8a4
			 0x8ca95da8, /// 0x8a8
			 0x2e313e97, /// 0x8ac
			 0xccdf65e5, /// 0x8b0
			 0x5eb43758, /// 0x8b4
			 0xa533ac88, /// 0x8b8
			 0xbe0f4c5a, /// 0x8bc
			 0x33b7b0a2, /// 0x8c0
			 0x23328328, /// 0x8c4
			 0xca1fb1da, /// 0x8c8
			 0x38847308, /// 0x8cc
			 0xd37ae1b5, /// 0x8d0
			 0x4df2ef77, /// 0x8d4
			 0xf78dc991, /// 0x8d8
			 0x81766274, /// 0x8dc
			 0x9b728398, /// 0x8e0
			 0x5cb9a8c9, /// 0x8e4
			 0x355c8ec7, /// 0x8e8
			 0xafcffca9, /// 0x8ec
			 0xcb9588fb, /// 0x8f0
			 0x92d9f031, /// 0x8f4
			 0xedb002b7, /// 0x8f8
			 0xff241b6c, /// 0x8fc
			 0xf7e5152b, /// 0x900
			 0x9888b546, /// 0x904
			 0x20de7435, /// 0x908
			 0xd1a6c733, /// 0x90c
			 0xd32d911c, /// 0x910
			 0x9a5cb27c, /// 0x914
			 0xc8e43c6f, /// 0x918
			 0x9f334c8e, /// 0x91c
			 0x13564cbb, /// 0x920
			 0xaa6966f2, /// 0x924
			 0x3c07dd41, /// 0x928
			 0x30fcc228, /// 0x92c
			 0xe1a58d0d, /// 0x930
			 0x458b5f85, /// 0x934
			 0xe0f91e3f, /// 0x938
			 0x5373b980, /// 0x93c
			 0x5ed36eea, /// 0x940
			 0xe6fe0ca9, /// 0x944
			 0xa3ac46e5, /// 0x948
			 0xd2b13e43, /// 0x94c
			 0x5ae16934, /// 0x950
			 0x5dd6bbec, /// 0x954
			 0x675094cc, /// 0x958
			 0x5ccbe00f, /// 0x95c
			 0x52d8eff1, /// 0x960
			 0xd59827b2, /// 0x964
			 0x54d00afe, /// 0x968
			 0x4ebb6518, /// 0x96c
			 0x1b40a0d1, /// 0x970
			 0x04ebb181, /// 0x974
			 0xa4afe7ef, /// 0x978
			 0xbe2f25b8, /// 0x97c
			 0xab30f6b4, /// 0x980
			 0xe2ab4b9f, /// 0x984
			 0x27bf06a1, /// 0x988
			 0xc072fed4, /// 0x98c
			 0xeadcb52a, /// 0x990
			 0x8f82af46, /// 0x994
			 0x9152a538, /// 0x998
			 0x132e60d0, /// 0x99c
			 0xd6d978e5, /// 0x9a0
			 0xc8b50024, /// 0x9a4
			 0x63124bff, /// 0x9a8
			 0x253f69d8, /// 0x9ac
			 0x74909a72, /// 0x9b0
			 0x2e83c0ae, /// 0x9b4
			 0x9b0f2444, /// 0x9b8
			 0xd196124d, /// 0x9bc
			 0x14780fb1, /// 0x9c0
			 0xb347bd90, /// 0x9c4
			 0xdef86402, /// 0x9c8
			 0x4bd88461, /// 0x9cc
			 0xca16aba7, /// 0x9d0
			 0x804849f2, /// 0x9d4
			 0x1ff2d6b1, /// 0x9d8
			 0x59629bf6, /// 0x9dc
			 0x85d94304, /// 0x9e0
			 0x79c5ff8c, /// 0x9e4
			 0x0dc56e2b, /// 0x9e8
			 0xbfe43a6b, /// 0x9ec
			 0x6f3dc738, /// 0x9f0
			 0x64152eff, /// 0x9f4
			 0x3f6e6da6, /// 0x9f8
			 0xbe35b5f1, /// 0x9fc
			 0x502cce86, /// 0xa00
			 0xe0fdc11a, /// 0xa04
			 0xb4d39ff8, /// 0xa08
			 0x397ea918, /// 0xa0c
			 0x0bbb0507, /// 0xa10
			 0xdb8c56d8, /// 0xa14
			 0xe36948db, /// 0xa18
			 0x5c9fedb6, /// 0xa1c
			 0x9abee311, /// 0xa20
			 0x4427150b, /// 0xa24
			 0x1f006401, /// 0xa28
			 0xb7e3a5e0, /// 0xa2c
			 0x530fc53e, /// 0xa30
			 0x6f0a8b29, /// 0xa34
			 0x0d56cb36, /// 0xa38
			 0xfc7ca0ef, /// 0xa3c
			 0x9b8ed22f, /// 0xa40
			 0x7ff5d01b, /// 0xa44
			 0xb300ea66, /// 0xa48
			 0x23b4be94, /// 0xa4c
			 0xbc21b3c9, /// 0xa50
			 0xf50c79ac, /// 0xa54
			 0x24302834, /// 0xa58
			 0x016b4944, /// 0xa5c
			 0x74da83c2, /// 0xa60
			 0x0a7c2153, /// 0xa64
			 0x70856235, /// 0xa68
			 0x7f5425b6, /// 0xa6c
			 0x8e887075, /// 0xa70
			 0x6848b4de, /// 0xa74
			 0xb114ee46, /// 0xa78
			 0x308b65e4, /// 0xa7c
			 0x6116d709, /// 0xa80
			 0x18af6dd3, /// 0xa84
			 0xd39d894d, /// 0xa88
			 0xc95b68d3, /// 0xa8c
			 0x5fedbcc3, /// 0xa90
			 0x5b60113d, /// 0xa94
			 0x21a9d6f0, /// 0xa98
			 0x2d5ec72c, /// 0xa9c
			 0x249990b3, /// 0xaa0
			 0x6f381fbd, /// 0xaa4
			 0xe5a06fb6, /// 0xaa8
			 0x4937d091, /// 0xaac
			 0xed6eaf05, /// 0xab0
			 0xa6e24fd4, /// 0xab4
			 0x108bd15a, /// 0xab8
			 0xd4f86156, /// 0xabc
			 0xb3a01872, /// 0xac0
			 0xed066d1e, /// 0xac4
			 0x483eeb1a, /// 0xac8
			 0x4e6ed83f, /// 0xacc
			 0xd4c21e3d, /// 0xad0
			 0xb3e76b46, /// 0xad4
			 0x2107e214, /// 0xad8
			 0xca94e73e, /// 0xadc
			 0xaead0aa4, /// 0xae0
			 0xd6c8e733, /// 0xae4
			 0x4ac2bcb4, /// 0xae8
			 0x7e629ac7, /// 0xaec
			 0x2f8e4b79, /// 0xaf0
			 0x4a190cf6, /// 0xaf4
			 0x352c3572, /// 0xaf8
			 0x0c43a811, /// 0xafc
			 0x4ae689f7, /// 0xb00
			 0xff17774c, /// 0xb04
			 0x1d6ea9e5, /// 0xb08
			 0xa409611e, /// 0xb0c
			 0x532e2faa, /// 0xb10
			 0xb2af354b, /// 0xb14
			 0x2e027968, /// 0xb18
			 0x57d3516d, /// 0xb1c
			 0x8c1e480f, /// 0xb20
			 0xaacd7764, /// 0xb24
			 0xe72b2fad, /// 0xb28
			 0xf3ea5365, /// 0xb2c
			 0x9f98c70d, /// 0xb30
			 0x9c940914, /// 0xb34
			 0x5c1c9437, /// 0xb38
			 0x6b7f604f, /// 0xb3c
			 0x3f67ad62, /// 0xb40
			 0x1eca88fc, /// 0xb44
			 0x7cc3815a, /// 0xb48
			 0x7b7b446b, /// 0xb4c
			 0x78e1c7c4, /// 0xb50
			 0x0cb4ac95, /// 0xb54
			 0x01bba99a, /// 0xb58
			 0xf31d5401, /// 0xb5c
			 0x954f30ef, /// 0xb60
			 0x41717c9d, /// 0xb64
			 0xc7d7fe50, /// 0xb68
			 0x9898c0b0, /// 0xb6c
			 0x418b41d5, /// 0xb70
			 0x413c3fc7, /// 0xb74
			 0xef2c28b7, /// 0xb78
			 0x6052a4c1, /// 0xb7c
			 0xfb0ed112, /// 0xb80
			 0x70cbf647, /// 0xb84
			 0xcf33e2bc, /// 0xb88
			 0xdc55e124, /// 0xb8c
			 0x68eccff3, /// 0xb90
			 0x99f21a9a, /// 0xb94
			 0x113e4919, /// 0xb98
			 0xfbaf29a4, /// 0xb9c
			 0x88ca768f, /// 0xba0
			 0x99f74bc2, /// 0xba4
			 0x621adb24, /// 0xba8
			 0x054cd594, /// 0xbac
			 0x7a73ce0b, /// 0xbb0
			 0x9f1bccd3, /// 0xbb4
			 0xc01405a7, /// 0xbb8
			 0x92447f12, /// 0xbbc
			 0x63b3f262, /// 0xbc0
			 0xab117c0f, /// 0xbc4
			 0xa994249d, /// 0xbc8
			 0xa12a8a2d, /// 0xbcc
			 0xc03c21c5, /// 0xbd0
			 0xe9b2e08f, /// 0xbd4
			 0x3d033805, /// 0xbd8
			 0x189376dc, /// 0xbdc
			 0xd5b5db95, /// 0xbe0
			 0x96d6271a, /// 0xbe4
			 0xbbd9eb7c, /// 0xbe8
			 0xea178ed8, /// 0xbec
			 0xe7c7a5f2, /// 0xbf0
			 0x62f7b3f8, /// 0xbf4
			 0x3f88ee6d, /// 0xbf8
			 0x3a9972af, /// 0xbfc
			 0x2990745f, /// 0xc00
			 0x9d2230ec, /// 0xc04
			 0x2aba6b0f, /// 0xc08
			 0xcb66bd73, /// 0xc0c
			 0xfb2dd352, /// 0xc10
			 0x8b1b9dc0, /// 0xc14
			 0xb12572c7, /// 0xc18
			 0x296cf1e6, /// 0xc1c
			 0x62edd3ec, /// 0xc20
			 0xa0215193, /// 0xc24
			 0x8117f8ff, /// 0xc28
			 0x387c76a8, /// 0xc2c
			 0xea0f0e8d, /// 0xc30
			 0x72e56220, /// 0xc34
			 0x80546e4e, /// 0xc38
			 0xb39bfc5d, /// 0xc3c
			 0x9be509c7, /// 0xc40
			 0xbf3b9946, /// 0xc44
			 0x9b5c882c, /// 0xc48
			 0x5cd6516c, /// 0xc4c
			 0xa9d73c1e, /// 0xc50
			 0x7368a791, /// 0xc54
			 0x452efb65, /// 0xc58
			 0x6d059ef1, /// 0xc5c
			 0x6991b48d, /// 0xc60
			 0xc0b22e72, /// 0xc64
			 0x4777b23e, /// 0xc68
			 0x16206920, /// 0xc6c
			 0xdf7bda7d, /// 0xc70
			 0x6771368c, /// 0xc74
			 0xc24fa18b, /// 0xc78
			 0x210807c4, /// 0xc7c
			 0xbdfff755, /// 0xc80
			 0x1701bf32, /// 0xc84
			 0x02ea3ba8, /// 0xc88
			 0xfb317320, /// 0xc8c
			 0x53ceafeb, /// 0xc90
			 0x737db657, /// 0xc94
			 0xf39803a8, /// 0xc98
			 0xdd6518e0, /// 0xc9c
			 0xd4e54670, /// 0xca0
			 0x6e4f227e, /// 0xca4
			 0x932932d9, /// 0xca8
			 0x31ae3e2a, /// 0xcac
			 0x8bd2cb5a, /// 0xcb0
			 0x98423fd1, /// 0xcb4
			 0x8234ca04, /// 0xcb8
			 0x84b3761d, /// 0xcbc
			 0xe975abb3, /// 0xcc0
			 0xd89fb1cb, /// 0xcc4
			 0x51409ca5, /// 0xcc8
			 0x04d545c9, /// 0xccc
			 0x1fa603b4, /// 0xcd0
			 0x81bee692, /// 0xcd4
			 0x3b61db1d, /// 0xcd8
			 0x91ef7788, /// 0xcdc
			 0x98e51a22, /// 0xce0
			 0xfe62fef9, /// 0xce4
			 0x5462ea24, /// 0xce8
			 0xfe0c2bb7, /// 0xcec
			 0x666d62ab, /// 0xcf0
			 0x574cbd30, /// 0xcf4
			 0xab6f51d2, /// 0xcf8
			 0x6b0dbb60, /// 0xcfc
			 0xc2550079, /// 0xd00
			 0xef5ca917, /// 0xd04
			 0x8ac1335c, /// 0xd08
			 0x18e5ec74, /// 0xd0c
			 0x839a9856, /// 0xd10
			 0x208b974c, /// 0xd14
			 0x00a6bf96, /// 0xd18
			 0x88d93af6, /// 0xd1c
			 0xf9ca5cdf, /// 0xd20
			 0xda0a5260, /// 0xd24
			 0x0ee7a793, /// 0xd28
			 0x18bbc1f7, /// 0xd2c
			 0x51c87df6, /// 0xd30
			 0x7b91ef6f, /// 0xd34
			 0xa7139fe4, /// 0xd38
			 0x1df27b20, /// 0xd3c
			 0x6fd5a226, /// 0xd40
			 0xa55fa374, /// 0xd44
			 0x23ee88c8, /// 0xd48
			 0x89d23753, /// 0xd4c
			 0x9869649f, /// 0xd50
			 0x81fec552, /// 0xd54
			 0x61705971, /// 0xd58
			 0x3a035300, /// 0xd5c
			 0xe4810c1a, /// 0xd60
			 0x9683a4bd, /// 0xd64
			 0xb186b25b, /// 0xd68
			 0x1f6215fa, /// 0xd6c
			 0xbdee809c, /// 0xd70
			 0x70cd189a, /// 0xd74
			 0x3cd4f151, /// 0xd78
			 0xbe62f4f3, /// 0xd7c
			 0xfe9984a6, /// 0xd80
			 0xb0d4aaab, /// 0xd84
			 0xfc5bbb96, /// 0xd88
			 0xc9c62a02, /// 0xd8c
			 0xd706ef95, /// 0xd90
			 0xb3e4a0b5, /// 0xd94
			 0xd8511e67, /// 0xd98
			 0x50b4a904, /// 0xd9c
			 0xb051b6c4, /// 0xda0
			 0x024daa01, /// 0xda4
			 0x8d770869, /// 0xda8
			 0xcf582d24, /// 0xdac
			 0xbe2b15f1, /// 0xdb0
			 0x93b9fc99, /// 0xdb4
			 0x0a171bcf, /// 0xdb8
			 0xdb674a24, /// 0xdbc
			 0x15a01b92, /// 0xdc0
			 0x554cf724, /// 0xdc4
			 0xab7e08fe, /// 0xdc8
			 0xff43be40, /// 0xdcc
			 0xd8a38dc5, /// 0xdd0
			 0x48c5a7e4, /// 0xdd4
			 0x3b8a91c1, /// 0xdd8
			 0x1fbcd71a, /// 0xddc
			 0xa1d83638, /// 0xde0
			 0x02a5a460, /// 0xde4
			 0x08ec1d8b, /// 0xde8
			 0xd17dcb0f, /// 0xdec
			 0xc7057419, /// 0xdf0
			 0x202e2e46, /// 0xdf4
			 0xe14d9b08, /// 0xdf8
			 0x9cfb1c70, /// 0xdfc
			 0xc97083c2, /// 0xe00
			 0x012cef96, /// 0xe04
			 0x1903bc66, /// 0xe08
			 0xdc2a3854, /// 0xe0c
			 0x83e7661f, /// 0xe10
			 0x4aa96fd5, /// 0xe14
			 0xc6ae5f54, /// 0xe18
			 0x577d29ed, /// 0xe1c
			 0xd941ce31, /// 0xe20
			 0x95c4e471, /// 0xe24
			 0x741cd86d, /// 0xe28
			 0xda5818e7, /// 0xe2c
			 0x0d1fc573, /// 0xe30
			 0xa6cfcbd6, /// 0xe34
			 0xfcd083b3, /// 0xe38
			 0xda2b88e1, /// 0xe3c
			 0x5e9c34e0, /// 0xe40
			 0xac6122a5, /// 0xe44
			 0x341fee42, /// 0xe48
			 0x0fe2bdd8, /// 0xe4c
			 0xbd464c03, /// 0xe50
			 0x9faf9722, /// 0xe54
			 0xa52a4951, /// 0xe58
			 0x54906969, /// 0xe5c
			 0x40729680, /// 0xe60
			 0xd58ddb96, /// 0xe64
			 0x376bfa39, /// 0xe68
			 0xad22e672, /// 0xe6c
			 0xe7d75c5e, /// 0xe70
			 0xa990a33d, /// 0xe74
			 0x6c207931, /// 0xe78
			 0xaed20653, /// 0xe7c
			 0x5e0067e9, /// 0xe80
			 0x38991385, /// 0xe84
			 0x0a95a899, /// 0xe88
			 0xa7fd5a70, /// 0xe8c
			 0xc978a27e, /// 0xe90
			 0xd1ef70f3, /// 0xe94
			 0x430297bf, /// 0xe98
			 0xfd1b7bc3, /// 0xe9c
			 0x3e2f4342, /// 0xea0
			 0x48a36932, /// 0xea4
			 0x248412c1, /// 0xea8
			 0xea011d07, /// 0xeac
			 0xa5e281b3, /// 0xeb0
			 0x1422d6d4, /// 0xeb4
			 0x008c5ba5, /// 0xeb8
			 0xdb515a8d, /// 0xebc
			 0xe8e946a2, /// 0xec0
			 0xad5b4e88, /// 0xec4
			 0xfd8340a2, /// 0xec8
			 0x1d442e85, /// 0xecc
			 0xdf6505f2, /// 0xed0
			 0x77f145a7, /// 0xed4
			 0x6bbc8a72, /// 0xed8
			 0xfca62c2b, /// 0xedc
			 0xc8423be9, /// 0xee0
			 0x09d030e6, /// 0xee4
			 0xe3cb2ec7, /// 0xee8
			 0x28b0f1fa, /// 0xeec
			 0x2d587c7f, /// 0xef0
			 0x3d705f36, /// 0xef4
			 0x02c60e43, /// 0xef8
			 0x2a6a8987, /// 0xefc
			 0x93025cd4, /// 0xf00
			 0x797bafb9, /// 0xf04
			 0x9c0bf2e7, /// 0xf08
			 0xb286630c, /// 0xf0c
			 0xac324485, /// 0xf10
			 0xdf538009, /// 0xf14
			 0x8dd043f9, /// 0xf18
			 0xcd4e9641, /// 0xf1c
			 0xfdde9cfd, /// 0xf20
			 0x8a039c26, /// 0xf24
			 0xc2304e65, /// 0xf28
			 0xbf584852, /// 0xf2c
			 0x77d0b154, /// 0xf30
			 0x8a9c38fd, /// 0xf34
			 0x625cc2ce, /// 0xf38
			 0x8cece581, /// 0xf3c
			 0x2e499ebe, /// 0xf40
			 0xaae3a832, /// 0xf44
			 0x7e2eb172, /// 0xf48
			 0xe71b2756, /// 0xf4c
			 0x8210dbf8, /// 0xf50
			 0xf7c5a774, /// 0xf54
			 0xe798795d, /// 0xf58
			 0xe545bc2f, /// 0xf5c
			 0xb9c8c4f2, /// 0xf60
			 0xfd2bb7d9, /// 0xf64
			 0xa8bcb5fd, /// 0xf68
			 0x88059d4a, /// 0xf6c
			 0xe37cb0a1, /// 0xf70
			 0x82afca5b, /// 0xf74
			 0xe89cd64b, /// 0xf78
			 0x2649b8e0, /// 0xf7c
			 0x2398f0f5, /// 0xf80
			 0x50e38bb2, /// 0xf84
			 0x60896db6, /// 0xf88
			 0xc07e42c5, /// 0xf8c
			 0xc6999834, /// 0xf90
			 0x47c7cc39, /// 0xf94
			 0x446eb249, /// 0xf98
			 0xba1b9d40, /// 0xf9c
			 0x96a0ee8d, /// 0xfa0
			 0xf05c62ce, /// 0xfa4
			 0x601d4603, /// 0xfa8
			 0xcefefb09, /// 0xfac
			 0x11e520a8, /// 0xfb0
			 0x7541e763, /// 0xfb4
			 0xf38a76cf, /// 0xfb8
			 0x20d0aab4, /// 0xfbc
			 0x78dfdcee, /// 0xfc0
			 0x0dc472fe, /// 0xfc4
			 0x190a10a7, /// 0xfc8
			 0x75f4365f, /// 0xfcc
			 0xdf97fec6, /// 0xfd0
			 0xbda75f9a, /// 0xfd4
			 0x9d03d894, /// 0xfd8
			 0x8f0fd9eb, /// 0xfdc
			 0x5c2e5c10, /// 0xfe0
			 0x61938542, /// 0xfe4
			 0xdd618341, /// 0xfe8
			 0x77744f0d, /// 0xfec
			 0x003b9573, /// 0xff0
			 0x69e56133, /// 0xff4
			 0x3e5a6c66, /// 0xff8
			 0xb1f75060 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00000
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00004
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0000c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00014
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00018
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00024
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x0c780000,                                                  // Leading 1s:                                 /// 0002c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00030
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00034
			 0x00000000,                                                  // zero                                        /// 00038
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0003c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00040
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00044
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0004c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00050
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00054
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00058
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0005c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00060
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00070
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00074
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00078
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0007c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00084
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0008c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00090
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00098
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000c0
			 0x4b000000,                                                  // 8388608.0                                   /// 000c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 000d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 000f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000f4
			 0x33333333,                                                  // 4 random values                             /// 000f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00100
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00104
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00108
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0010c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00110
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0011c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00120
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00124
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0013c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00140
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00144
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00148
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0014c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00150
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00154
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0015c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00160
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00164
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00168
			 0x55555555,                                                  // 4 random values                             /// 0016c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00170
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00174
			 0x00011111,                                                  // 9.7958E-41                                  /// 00178
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0017c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00180
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00188
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0018c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00194
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0019c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001c4
			 0x00000000,                                                  // zero                                        /// 001c8
			 0xcb000000,                                                  // -8388608.0                                  /// 001cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00200
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00204
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00208
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00210
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00218
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0021c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00220
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00224
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00228
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00234
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00238
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00240
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00244
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00248
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0024c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00250
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0025c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00264
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x00000000,                                                  // zero                                        /// 0026c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00270
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00278
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00280
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00284
			 0x7f800000,                                                  // inf                                         /// 00288
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0028c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00290
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00298
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0029c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00300
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00304
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0030c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00314
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00318
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0031c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00320
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00328
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00334
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00338
			 0x0c700000,                                                  // Leading 1s:                                 /// 0033c
			 0x55555555,                                                  // 4 random values                             /// 00340
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00344
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00348
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0034c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0035c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00360
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00364
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00368
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0036c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00374
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00378
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0037c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00380
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00384
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80000000,                                                  // -zero                                       /// 00394
			 0x80011111,                                                  // -9.7958E-41                                 /// 00398
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0039c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b0
			 0x7fc00001,                                                  // signaling NaN                               /// 003b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00404
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00408
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0040c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00410
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0xbf028f5c,                                                  // -0.51                                       /// 0041c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00420
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00424
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00428
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0042c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00430
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00434
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00438
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00440
			 0x0e000007,                                                  // Trailing 1s:                                /// 00444
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00448
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00450
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00454
			 0x0c780000,                                                  // Leading 1s:                                 /// 00458
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0045c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00460
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00464
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x7f800000,                                                  // inf                                         /// 0046c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00470
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00474
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00478
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00484
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00488
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0048c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00490
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00494
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00498
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0049c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 004a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x55555555,                                                  // 4 random values                             /// 004dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004ec
			 0x80000000,                                                  // -zero                                       /// 004f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00500
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00504
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00508
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0050c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00510
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00514
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00520
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00524
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00528
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0052c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00530
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00534
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00538
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00544
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00548
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x33333333,                                                  // 4 random values                             /// 00550
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x3f028f5c,                                                  // 0.51                                        /// 00558
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00564
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00568
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00570
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00574
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00578
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00580
			 0x00011111,                                                  // 9.7958E-41                                  /// 00584
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00588
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00594
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x80000000,                                                  // -zero                                       /// 0059c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005a0
			 0xffc00001,                                                  // -signaling NaN                              /// 005a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80000000,                                                  // -zero                                       /// 005b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00604
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00608
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0060c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00610
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00614
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0061c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0062c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00630
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00638
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00640
			 0xff800000,                                                  // -inf                                        /// 00644
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00648
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0064c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00650
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00654
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00658
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0065c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00670
			 0x80011111,                                                  // -9.7958E-41                                 /// 00674
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0067c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00680
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00688
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x7f800000,                                                  // inf                                         /// 00690
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00694
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00698
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006e4
			 0xbf028f5c,                                                  // -0.51                                       /// 006e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006ec
			 0x80000000,                                                  // -zero                                       /// 006f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00700
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00704
			 0x0c780000,                                                  // Leading 1s:                                 /// 00708
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0070c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00710
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0071c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00724
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00728
			 0x0e000003,                                                  // Trailing 1s:                                /// 0072c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00734
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00744
			 0x0e000003,                                                  // Trailing 1s:                                /// 00748
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0074c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00750
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00758
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00760
			 0x33333333,                                                  // 4 random values                             /// 00764
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00768
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0076c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00774
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00778
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0077c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00780
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00784
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00790
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00798
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0079c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0xff800000,                                                  // -inf                                        /// 007b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007ec
			 0x00000000,                                                  // zero                                        /// 007f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e000007,                                                  // Trailing 1s:                                /// 00804
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00808
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00814
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0e000007,                                                  // Trailing 1s:                                /// 0081c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00824
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0082c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00834
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00844
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x55555555,                                                  // 4 random values                             /// 0084c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00854
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x0c700000,                                                  // Leading 1s:                                 /// 0085c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00860
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00864
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0086c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0e000001,                                                  // Trailing 1s:                                /// 00874
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0087c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00884
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00888
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0088c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0089c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008bc
			 0x00000000,                                                  // zero                                        /// 008c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008c4
			 0x3f028f5c,                                                  // 0.51                                        /// 008c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 008cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00900
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00904
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00908
			 0x0c400000,                                                  // Leading 1s:                                 /// 0090c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00910
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00918
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00920
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0092c
			 0xbf028f5c,                                                  // -0.51                                       /// 00930
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00934
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0093c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00940
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00944
			 0x80011111,                                                  // -9.7958E-41                                 /// 00948
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0094c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00950
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00958
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0095c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00960
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00968
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0096c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00970
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00974
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00978
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00984
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0098c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00990
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00994
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0099c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a2c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a30
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a34
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a38
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a58
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a88
			 0x7f800000,                                                  // inf                                         /// 00a8c
			 0xff800000,                                                  // -inf                                        /// 00a90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aa0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ab0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ab4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00abc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ac0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ac4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ac8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00acc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ad0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ad4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ad8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00af0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00af8
			 0xffc00001,                                                  // -signaling NaN                              /// 00afc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b40
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b48
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b54
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b58
			 0x7f800000,                                                  // inf                                         /// 00b5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b7c
			 0x7f800000,                                                  // inf                                         /// 00b80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ba0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ba4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ba8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80000000,                                                  // -zero                                       /// 00bb4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bb8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bbc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bc0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bd0
			 0x55555555,                                                  // 4 random values                             /// 00bd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bd8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00be0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00be4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bf0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bf4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bf8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c08
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x00000000,                                                  // zero                                        /// 00c18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c30
			 0x4b000000,                                                  // 8388608.0                                   /// 00c34
			 0x00000000,                                                  // zero                                        /// 00c38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c64
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c68
			 0x33333333,                                                  // 4 random values                             /// 00c6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c84
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c98
			 0x3f028f5c,                                                  // 0.51                                        /// 00c9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ca0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cb0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cbc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cc0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ccc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cd0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ce4
			 0x7f800000,                                                  // inf                                         /// 00ce8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cf0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cf4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cfc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d0c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d50
			 0x00000000,                                                  // zero                                        /// 00d54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d60
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d68
			 0x00000000,                                                  // zero                                        /// 00d6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d90
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00db0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00db4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00db8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dbc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dc8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dcc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ddc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00de4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00df4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dfc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e04
			 0xffc00001,                                                  // -signaling NaN                              /// 00e08
			 0xff800000,                                                  // -inf                                        /// 00e0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e10
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e18
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e50
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e5c
			 0x80000000,                                                  // -zero                                       /// 00e60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e90
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e98
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ea0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ea8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eb8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ec0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ec4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ec8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ed0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ed4
			 0x55555555,                                                  // 4 random values                             /// 00ed8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00edc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ee8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ef0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ef4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ef8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f08
			 0x80000000,                                                  // -zero                                       /// 00f0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f18
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f4c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f54
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f6c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f74
			 0xbf028f5c,                                                  // -0.51                                       /// 00f78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fa4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fa8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fb4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fb8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fcc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fd0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fe4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ff4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x7f000000 // norm with max exp min mant                   // SP +ve numbers                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00000
			 0x0c600000,                                                  // Leading 1s:                                 /// 00004
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00008
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0000c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00014
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00018
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00020
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00024
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00028
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00034
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0003c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00040
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00048
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0004c
			 0x7f800000,                                                  // inf                                         /// 00050
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00054
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00058
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00060
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00068
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0006c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00074
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00078
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00080
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00084
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00088
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00100
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00104
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0010c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00110
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00114
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0011c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00120
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00124
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00128
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00138
			 0x0c600000,                                                  // Leading 1s:                                 /// 0013c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00148
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0014c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00150
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00154
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0015c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00164
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00168
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0016c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00170
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00174
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00178
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0017c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00184
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0018c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00194
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00198
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0019c
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 001c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0xff800000,                                                  // -inf                                        /// 001d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001d4
			 0x7f800000,                                                  // inf                                         /// 001d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e8
			 0x4b000000,                                                  // 8388608.0                                   /// 001ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00200
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0020c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00210
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00214
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0021c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00220
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00224
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00228
			 0x0c700000,                                                  // Leading 1s:                                 /// 0022c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00240
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00244
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00248
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00250
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00254
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0025c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0xbf028f5c,                                                  // -0.51                                       /// 00264
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00268
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00270
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00278
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0027c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00280
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00288
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0028c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00290
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00298
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002a0
			 0xffc00001,                                                  // -signaling NaN                              /// 002a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x7f800000,                                                  // inf                                         /// 002b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002b4
			 0x00000000,                                                  // zero                                        /// 002b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 002d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002fc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00308
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0030c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00310
			 0x0e000001,                                                  // Trailing 1s:                                /// 00314
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00320
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00324
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00330
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00334
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0033c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00348
			 0x55555555,                                                  // 4 random values                             /// 0034c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00350
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00354
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00358
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0035c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00360
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00364
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00368
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00374
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00380
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00384
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00388
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0038c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00394
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0xcb000000,                                                  // -8388608.0                                  /// 0039c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x55555555,                                                  // 4 random values                             /// 00400
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00404
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00408
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0040c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00410
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00414
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x7f800000,                                                  // inf                                         /// 0041c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00428
			 0xff800000,                                                  // -inf                                        /// 0042c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00430
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00438
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x33333333,                                                  // 4 random values                             /// 00444
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00450
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00454
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0045c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00460
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0046c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00470
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00478
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0047c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00484
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00488
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0048c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00490
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00494
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0049c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00500
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00504
			 0x33333333,                                                  // 4 random values                             /// 00508
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00514
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00518
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0051c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00520
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00528
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0052c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00530
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00534
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0xff800000,                                                  // -inf                                        /// 00540
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00544
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0054c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00550
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00554
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00560
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00564
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00568
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00570
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00578
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x80000000,                                                  // -zero                                       /// 00580
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0059c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005c8
			 0x00000000,                                                  // zero                                        /// 005cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005e4
			 0x7f800000,                                                  // inf                                         /// 005e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00600
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00604
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0060c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00610
			 0x33333333,                                                  // 4 random values                             /// 00614
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00618
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0xbf028f5c,                                                  // -0.51                                       /// 00620
			 0x0c700000,                                                  // Leading 1s:                                 /// 00624
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00628
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0062c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00630
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00634
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0063c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00648
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0064c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00650
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00654
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0xff800000,                                                  // -inf                                        /// 0065c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00668
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00670
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00674
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x3f028f5c,                                                  // 0.51                                        /// 0067c
			 0x7fc00001,                                                  // signaling NaN                               /// 00680
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00688
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0068c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00690
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00694
			 0xff800000,                                                  // -inf                                        /// 00698
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0069c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006a0
			 0xffc00001,                                                  // -signaling NaN                              /// 006a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00704
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00708
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0070c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00720
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00724
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00728
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00730
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0073c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00740
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00744
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00748
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0074c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00750
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00754
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00758
			 0x00011111,                                                  // 9.7958E-41                                  /// 0075c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00764
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00768
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0076c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00774
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00778
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0077c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00784
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00788
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00790
			 0xffc00001,                                                  // -signaling NaN                              /// 00794
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00798
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0xffc00001,                                                  // -signaling NaN                              /// 007c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 007d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00808
			 0x55555555,                                                  // 4 random values                             /// 0080c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00810
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00818
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0081c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00820
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00824
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0082c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00830
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00838
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00840
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00844
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00848
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00850
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00858
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00860
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00868
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0xbf028f5c,                                                  // -0.51                                       /// 00870
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00874
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00878
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00880
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00884
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00888
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0e000007,                                                  // Trailing 1s:                                /// 00894
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00898
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0089c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x7f800000,                                                  // inf                                         /// 008c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008e4
			 0xffc00001,                                                  // -signaling NaN                              /// 008e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008ec
			 0x7f800000,                                                  // inf                                         /// 008f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00900
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00904
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00908
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0090c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00914
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00918
			 0x0c600000,                                                  // Leading 1s:                                 /// 0091c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00920
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00924
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00928
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00930
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00934
			 0x33333333,                                                  // 4 random values                             /// 00938
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0093c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00940
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00948
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0094c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00950
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00954
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0095c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00960
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00968
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0096c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00984
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00988
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0098c
			 0x00000000,                                                  // zero                                        /// 00990
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00998
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0099c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009d0
			 0x00000000,                                                  // zero                                        /// 009d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009f0
			 0xffc00001,                                                  // -signaling NaN                              /// 009f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00a00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a08
			 0xcb000000,                                                  // -8388608.0                                  /// 00a0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a1c
			 0x7f800000,                                                  // inf                                         /// 00a20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a40
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a7c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a8c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a90
			 0xcb000000,                                                  // -8388608.0                                  /// 00a94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00aa4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00aa8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ab0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ab4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ab8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00abc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ac0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ac8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ad0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ad4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00adc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ae0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ae8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00af8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b34
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b40
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b60
			 0x7fc00001,                                                  // signaling NaN                               /// 00b64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b7c
			 0xff800000,                                                  // -inf                                        /// 00b80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ba8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bb0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bb4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bb8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bc4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bdc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00be4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00be8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bf8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c28
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c50
			 0x00000000,                                                  // zero                                        /// 00c54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00ca0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ca8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cd0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cdc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ce0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ce4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cec
			 0xffc00001,                                                  // -signaling NaN                              /// 00cf0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cf8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d10
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d14
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d18
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d20
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x7fc00001,                                                  // signaling NaN                               /// 00d3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d48
			 0x7f800000,                                                  // inf                                         /// 00d4c
			 0x55555555,                                                  // 4 random values                             /// 00d50
			 0xbf028f5c,                                                  // -0.51                                       /// 00d54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d58
			 0x80000000,                                                  // -zero                                       /// 00d5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d70
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d78
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d80
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d88
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d8c
			 0x00000000,                                                  // zero                                        /// 00d90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d98
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x4b000000,                                                  // 8388608.0                                   /// 00da4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0xff800000,                                                  // -inf                                        /// 00db4
			 0xffc00001,                                                  // -signaling NaN                              /// 00db8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dd4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dd8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00de0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00de4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00de8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00df4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00df8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e00
			 0xffc00001,                                                  // -signaling NaN                              /// 00e04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e0c
			 0x80000000,                                                  // -zero                                       /// 00e10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e50
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e68
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e80
			 0x00000000,                                                  // zero                                        /// 00e84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ea4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00eac
			 0xbf028f5c,                                                  // -0.51                                       /// 00eb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ebc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ec4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ed0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ed8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00edc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ee0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ee4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ef4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ef8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00efc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f04
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f14
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f18
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f50
			 0x80000000,                                                  // -zero                                       /// 00f54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f6c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fa0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fa8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fb0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fb4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fbc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fcc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fe8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ff0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ff8
			 0x00000008                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x00000530,
			 0x000002c4,
			 0x000004f0,
			 0x00000354,
			 0x00000094,
			 0x000001fc,
			 0x000003c8,
			 0x000003a0,

			 /// vpu register f2
			 0x41880000,
			 0x41b80000,
			 0x41b80000,
			 0x40800000,
			 0x41f00000,
			 0x40a00000,
			 0x41300000,
			 0x40400000,

			 /// vpu register f3
			 0x41700000,
			 0x40400000,
			 0x41e80000,
			 0x40e00000,
			 0x41e00000,
			 0x41a00000,
			 0x3f800000,
			 0x41300000,

			 /// vpu register f4
			 0x41900000,
			 0x40e00000,
			 0x42000000,
			 0x40c00000,
			 0x41f00000,
			 0x41100000,
			 0x41700000,
			 0x41e80000,

			 /// vpu register f5
			 0x40c00000,
			 0x41a00000,
			 0x41d80000,
			 0x40c00000,
			 0x41400000,
			 0x40000000,
			 0x41b80000,
			 0x40800000,

			 /// vpu register f6
			 0x41a80000,
			 0x40a00000,
			 0x41200000,
			 0x41c00000,
			 0x41500000,
			 0x41b80000,
			 0x41100000,
			 0x41100000,

			 /// vpu register f7
			 0x41800000,
			 0x41900000,
			 0x41100000,
			 0x41a00000,
			 0x41f80000,
			 0x41f80000,
			 0x41500000,
			 0x41a00000,

			 /// vpu register f8
			 0x41e00000,
			 0x41980000,
			 0x40000000,
			 0x41a00000,
			 0x41a80000,
			 0x41100000,
			 0x40000000,
			 0x41a00000,

			 /// vpu register f9
			 0x41980000,
			 0x41f80000,
			 0x41100000,
			 0x3f800000,
			 0x41200000,
			 0x41c00000,
			 0x41980000,
			 0x41f00000,

			 /// vpu register f10
			 0x40000000,
			 0x41b00000,
			 0x41d00000,
			 0x41900000,
			 0x42000000,
			 0x41a80000,
			 0x40e00000,
			 0x41f80000,

			 /// vpu register f11
			 0x41900000,
			 0x41b80000,
			 0x41f00000,
			 0x41900000,
			 0x41880000,
			 0x41c80000,
			 0x41900000,
			 0x41a80000,

			 /// vpu register f12
			 0x41980000,
			 0x41100000,
			 0x41700000,
			 0x41300000,
			 0x40000000,
			 0x41c80000,
			 0x3f800000,
			 0x41b00000,

			 /// vpu register f13
			 0x41b00000,
			 0x41d00000,
			 0x41f80000,
			 0x41e00000,
			 0x41800000,
			 0x41a00000,
			 0x40c00000,
			 0x42000000,

			 /// vpu register f14
			 0x41980000,
			 0x41700000,
			 0x41a00000,
			 0x40800000,
			 0x41200000,
			 0x41c00000,
			 0x41800000,
			 0x41c00000,

			 /// vpu register f15
			 0x40a00000,
			 0x41a80000,
			 0x41900000,
			 0x40a00000,
			 0x41d80000,
			 0x40e00000,
			 0x41700000,
			 0x41700000,

			 /// vpu register f16
			 0x42000000,
			 0x41d80000,
			 0x41f00000,
			 0x41d80000,
			 0x41d00000,
			 0x41f00000,
			 0x41b00000,
			 0x40c00000,

			 /// vpu register f17
			 0x40800000,
			 0x40c00000,
			 0x40800000,
			 0x41d00000,
			 0x41980000,
			 0x3f800000,
			 0x40000000,
			 0x41100000,

			 /// vpu register f18
			 0x41300000,
			 0x41c80000,
			 0x41400000,
			 0x41e00000,
			 0x41700000,
			 0x40400000,
			 0x41900000,
			 0x41c80000,

			 /// vpu register f19
			 0x41d80000,
			 0x41d00000,
			 0x41800000,
			 0x41e80000,
			 0x41500000,
			 0x41a00000,
			 0x41b80000,
			 0x41980000,

			 /// vpu register f20
			 0x40000000,
			 0x40c00000,
			 0x41f00000,
			 0x40e00000,
			 0x41880000,
			 0x41880000,
			 0x41b00000,
			 0x3f800000,

			 /// vpu register f21
			 0x42000000,
			 0x40400000,
			 0x42000000,
			 0x41a80000,
			 0x41200000,
			 0x41600000,
			 0x40a00000,
			 0x40000000,

			 /// vpu register f22
			 0x40c00000,
			 0x40800000,
			 0x41100000,
			 0x40a00000,
			 0x41a00000,
			 0x41300000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f23
			 0x41e00000,
			 0x41800000,
			 0x41e00000,
			 0x40800000,
			 0x41a00000,
			 0x41200000,
			 0x41c80000,
			 0x41c00000,

			 /// vpu register f24
			 0x41200000,
			 0x41d80000,
			 0x41500000,
			 0x40e00000,
			 0x41800000,
			 0x41f80000,
			 0x41900000,
			 0x41500000,

			 /// vpu register f25
			 0x41300000,
			 0x41800000,
			 0x41b00000,
			 0x40800000,
			 0x41d80000,
			 0x41e00000,
			 0x41f00000,
			 0x41100000,

			 /// vpu register f26
			 0x41b80000,
			 0x41c80000,
			 0x41e00000,
			 0x41500000,
			 0x41a00000,
			 0x41100000,
			 0x41000000,
			 0x40a00000,

			 /// vpu register f27
			 0x41c80000,
			 0x42000000,
			 0x41c80000,
			 0x41d00000,
			 0x41e00000,
			 0x41500000,
			 0x41000000,
			 0x41e00000,

			 /// vpu register f28
			 0x41f00000,
			 0x42000000,
			 0x41a80000,
			 0x41700000,
			 0x41e00000,
			 0x41b00000,
			 0x41300000,
			 0x3f800000,

			 /// vpu register f29
			 0x41700000,
			 0x40e00000,
			 0x41900000,
			 0x42000000,
			 0x40800000,
			 0x41400000,
			 0x41b00000,
			 0x41500000,

			 /// vpu register f30
			 0x41e00000,
			 0x41900000,
			 0x41800000,
			 0x41c00000,
			 0x41c80000,
			 0x41900000,
			 0x40800000,
			 0x41c80000,

			 /// vpu register f31
			 0x40e00000,
			 0x41d80000,
			 0x40000000,
			 0x41d80000,
			 0x41f00000,
			 0x41b80000,
			 0x41e80000,
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
		"fsq2 f24, -1008(x15)\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f13, 1754(x15)\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f29, 67(x13)\n"                                 ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f18, -1502(x13)\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f24, 687(x14)\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f27, -1908(x14)\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f13, 1271(x14)\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f11, 68(x12)\n"                                 ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f23, -326(x11)\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f15, 553(x12)\n"                                ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f11, -520(x11)\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f28, 867(x14)\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f26, -54(x11)\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f2, -1946(x15)\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f16, -1681(x11)\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f0, 1349(x13)\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f1, 200(x13)\n"                                 ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f18, 835(x14)\n"                                ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f4, 330(x15)\n"                                 ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f30, 505(x15)\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f7, -442(x13)\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f3, 1372(x12)\n"                                ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f25, 37(x11)\n"                                 ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f26, 1948(x15)\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f23, 528(x11)\n"                                ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f9, -2040(x14)\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f26, 1188(x11)\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f1, -1346(x14)\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f30, 1829(x14)\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f15, -610(x13)\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f27, 273(x12)\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f16, 800(x11)\n"                                ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f18, -594(x14)\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f10, -350(x11)\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f0, 75(x15)\n"                                  ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f11, -1233(x14)\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f12, -338(x13)\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f28, -1159(x15)\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f8, -663(x13)\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f14, -928(x14)\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f15, 1012(x12)\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f17, -83(x13)\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f24, 1595(x11)\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f10, 1439(x11)\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f27, 214(x12)\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f0, -99(x14)\n"                                 ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f13, 134(x11)\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f24, 78(x11)\n"                                 ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f10, -1470(x11)\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f28, 42(x14)\n"                                 ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f21, -244(x11)\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f30, -282(x11)\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f15, 765(x12)\n"                                ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f9, -48(x13)\n"                                 ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f1, -1579(x15)\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f12, -1864(x13)\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f11, -1325(x14)\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f3, -917(x11)\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f27, -938(x13)\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f8, -1765(x11)\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f7, 398(x14)\n"                                 ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f5, -171(x13)\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f23, 1407(x12)\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f22, 1527(x15)\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f1, 1961(x13)\n"                                ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f22, 1440(x11)\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f21, 1164(x13)\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f0, -1754(x12)\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f30, -302(x11)\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f20, -77(x11)\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f28, -1166(x13)\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f7, 1466(x13)\n"                                ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f23, -904(x13)\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f30, 97(x12)\n"                                 ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f11, -359(x14)\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f30, -304(x11)\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f30, -1110(x12)\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f4, 1339(x11)\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f12, 523(x14)\n"                                ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f3, -1747(x13)\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f25, -301(x13)\n"                               ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f21, 2015(x11)\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f11, -1086(x12)\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f26, -1393(x15)\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f29, -1525(x15)\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f21, 1909(x15)\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f20, 1154(x11)\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f21, -1584(x12)\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f10, -408(x14)\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f26, -1935(x11)\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f25, -1674(x11)\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f22, -1261(x14)\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f24, -372(x14)\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f23, 1747(x12)\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f30, -1537(x13)\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f7, -627(x15)\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f29, 977(x14)\n"                                ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f10, -1781(x11)\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsq2 f3, -191(x11)\n"                                ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
