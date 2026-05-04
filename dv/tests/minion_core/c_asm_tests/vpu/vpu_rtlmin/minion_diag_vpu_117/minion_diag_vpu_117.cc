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
			 0x4c6bb592, /// 0x0
			 0x091017ed, /// 0x4
			 0x1a70ab0b, /// 0x8
			 0x0873e662, /// 0xc
			 0xe34f9ee3, /// 0x10
			 0x0e0f75c7, /// 0x14
			 0x5c657e6e, /// 0x18
			 0x8ef7dff0, /// 0x1c
			 0xe12f4539, /// 0x20
			 0x139090e0, /// 0x24
			 0x450370b6, /// 0x28
			 0x5c3fd7b4, /// 0x2c
			 0xedf08214, /// 0x30
			 0x8fba855b, /// 0x34
			 0x44dc1b79, /// 0x38
			 0x750d37cf, /// 0x3c
			 0x206a5226, /// 0x40
			 0x7d0b435c, /// 0x44
			 0x99ebc61a, /// 0x48
			 0x1216a2e7, /// 0x4c
			 0xa3dbe8cf, /// 0x50
			 0xfcf7f600, /// 0x54
			 0x258ebc23, /// 0x58
			 0x2626d3ee, /// 0x5c
			 0x6ac2ac48, /// 0x60
			 0x32c63d21, /// 0x64
			 0x0fbaa30f, /// 0x68
			 0xdb8189b2, /// 0x6c
			 0xd282a6ae, /// 0x70
			 0xba6aee72, /// 0x74
			 0x7f098a30, /// 0x78
			 0xaf241317, /// 0x7c
			 0xfd32a7a5, /// 0x80
			 0x6cc5c5f2, /// 0x84
			 0xd205a4cd, /// 0x88
			 0xc964a8c5, /// 0x8c
			 0xc17f4598, /// 0x90
			 0xf4e8a3c8, /// 0x94
			 0x47239f4a, /// 0x98
			 0x3224ea75, /// 0x9c
			 0x8c84aebc, /// 0xa0
			 0x64c4d2c5, /// 0xa4
			 0x833bbd66, /// 0xa8
			 0xc8ddc46d, /// 0xac
			 0x61b41688, /// 0xb0
			 0x294317b6, /// 0xb4
			 0x7087334d, /// 0xb8
			 0x8a1fa726, /// 0xbc
			 0xa44bf9ff, /// 0xc0
			 0x4f94ae86, /// 0xc4
			 0xfd52cac4, /// 0xc8
			 0x6aec7ca5, /// 0xcc
			 0x2327e472, /// 0xd0
			 0x54eb8854, /// 0xd4
			 0x822e9890, /// 0xd8
			 0x3430f804, /// 0xdc
			 0x4cce4bd2, /// 0xe0
			 0x09014d34, /// 0xe4
			 0xbc0bbfb1, /// 0xe8
			 0x73834feb, /// 0xec
			 0x8c3de6d9, /// 0xf0
			 0x45eebf60, /// 0xf4
			 0xd95a3c00, /// 0xf8
			 0xc91d47fd, /// 0xfc
			 0x2f0d9fda, /// 0x100
			 0x36e097ac, /// 0x104
			 0x6e81ae01, /// 0x108
			 0xc9ecb30e, /// 0x10c
			 0xc623fe2a, /// 0x110
			 0xdce4a06e, /// 0x114
			 0xcdfdc4ac, /// 0x118
			 0x6abb54eb, /// 0x11c
			 0xff861b12, /// 0x120
			 0xcf924ee0, /// 0x124
			 0xd15abeee, /// 0x128
			 0xa16154c4, /// 0x12c
			 0x3c42fbd7, /// 0x130
			 0x4d98fc07, /// 0x134
			 0xe3c6ae2d, /// 0x138
			 0x2c336c35, /// 0x13c
			 0x024c2f4c, /// 0x140
			 0xa5d890ff, /// 0x144
			 0x1abb6df5, /// 0x148
			 0xe76f2370, /// 0x14c
			 0xe6a210b0, /// 0x150
			 0x47588144, /// 0x154
			 0x57127987, /// 0x158
			 0x7ee0adf7, /// 0x15c
			 0x51433945, /// 0x160
			 0x995ba4d7, /// 0x164
			 0x83f61029, /// 0x168
			 0xb97522fa, /// 0x16c
			 0x3df32e4c, /// 0x170
			 0x88f8d0e5, /// 0x174
			 0xddc9acf1, /// 0x178
			 0xd4a35eeb, /// 0x17c
			 0x2f743ca1, /// 0x180
			 0xdf6d984a, /// 0x184
			 0x6e2ce830, /// 0x188
			 0x331d3847, /// 0x18c
			 0x18c0974b, /// 0x190
			 0xd9778ef1, /// 0x194
			 0x989908d0, /// 0x198
			 0xb2b35202, /// 0x19c
			 0xfef3e3ad, /// 0x1a0
			 0xc9f499ad, /// 0x1a4
			 0x935cacde, /// 0x1a8
			 0x376c6ae1, /// 0x1ac
			 0x54c2248c, /// 0x1b0
			 0x9e13c915, /// 0x1b4
			 0x2377977f, /// 0x1b8
			 0x231dafe8, /// 0x1bc
			 0x5090b1de, /// 0x1c0
			 0x19675eda, /// 0x1c4
			 0x93ca1fa9, /// 0x1c8
			 0x612fa84f, /// 0x1cc
			 0x45a5de32, /// 0x1d0
			 0xb6975f91, /// 0x1d4
			 0xdab33739, /// 0x1d8
			 0xd58e6f6e, /// 0x1dc
			 0x9973aaed, /// 0x1e0
			 0x2fb8f272, /// 0x1e4
			 0x70e1b40b, /// 0x1e8
			 0x1ddb280c, /// 0x1ec
			 0x791ebb5f, /// 0x1f0
			 0xe0f148d5, /// 0x1f4
			 0x36d1c433, /// 0x1f8
			 0x88c01ea9, /// 0x1fc
			 0x92f82164, /// 0x200
			 0x7e371f39, /// 0x204
			 0x4787ace3, /// 0x208
			 0xb6a20d83, /// 0x20c
			 0x34255f71, /// 0x210
			 0x42e4b474, /// 0x214
			 0xfd5e9784, /// 0x218
			 0xe54da697, /// 0x21c
			 0xea030c22, /// 0x220
			 0x075ca55d, /// 0x224
			 0x60704a4e, /// 0x228
			 0xba316bd1, /// 0x22c
			 0x1d2c170a, /// 0x230
			 0x9bd1a951, /// 0x234
			 0x6934f045, /// 0x238
			 0x25ed13bd, /// 0x23c
			 0xf07af776, /// 0x240
			 0x4addd412, /// 0x244
			 0x46f269a9, /// 0x248
			 0x5b18044c, /// 0x24c
			 0x8e6aea5e, /// 0x250
			 0x15ba7e92, /// 0x254
			 0x16617370, /// 0x258
			 0x644f005f, /// 0x25c
			 0x4f6de02c, /// 0x260
			 0x73a05164, /// 0x264
			 0x6cd2193f, /// 0x268
			 0xaadc3365, /// 0x26c
			 0xb256c8ee, /// 0x270
			 0xe81b5f9a, /// 0x274
			 0x75436309, /// 0x278
			 0xf594153e, /// 0x27c
			 0x3bdcba0b, /// 0x280
			 0xa6b0881b, /// 0x284
			 0x95335f8f, /// 0x288
			 0xb4b34bcd, /// 0x28c
			 0x3821fed0, /// 0x290
			 0x718a20ef, /// 0x294
			 0x9313885f, /// 0x298
			 0x1395d0e4, /// 0x29c
			 0x16498358, /// 0x2a0
			 0x6d5d0875, /// 0x2a4
			 0xf85546fd, /// 0x2a8
			 0xe59b6d37, /// 0x2ac
			 0xd4b1c37b, /// 0x2b0
			 0xd6b19dfb, /// 0x2b4
			 0xadf50363, /// 0x2b8
			 0x6f2ee8ed, /// 0x2bc
			 0x833e9987, /// 0x2c0
			 0xa60225ba, /// 0x2c4
			 0x8bcc7f81, /// 0x2c8
			 0xe4673d56, /// 0x2cc
			 0x75974d15, /// 0x2d0
			 0xcfc5b1d5, /// 0x2d4
			 0x3f2701b5, /// 0x2d8
			 0x258783d9, /// 0x2dc
			 0x57a63fe8, /// 0x2e0
			 0xd2185ef3, /// 0x2e4
			 0xa3aafd60, /// 0x2e8
			 0xda33dfed, /// 0x2ec
			 0x4b2c5494, /// 0x2f0
			 0x96fda50d, /// 0x2f4
			 0xda5bd781, /// 0x2f8
			 0xf9098cd2, /// 0x2fc
			 0x5feac6cf, /// 0x300
			 0x69c880d4, /// 0x304
			 0xff21d2d9, /// 0x308
			 0x5d67ec2c, /// 0x30c
			 0xc1febca6, /// 0x310
			 0x17cfaaca, /// 0x314
			 0x9077fdef, /// 0x318
			 0xfebfc069, /// 0x31c
			 0xfc6d2bb7, /// 0x320
			 0xbbeff2d4, /// 0x324
			 0x869e6394, /// 0x328
			 0x2c8ed2f1, /// 0x32c
			 0xa59b2909, /// 0x330
			 0xbc621125, /// 0x334
			 0x59fd1276, /// 0x338
			 0x78155e70, /// 0x33c
			 0xd64882f2, /// 0x340
			 0xef28e08c, /// 0x344
			 0xe6fbb498, /// 0x348
			 0x90650290, /// 0x34c
			 0xe32c63e9, /// 0x350
			 0x94d538b6, /// 0x354
			 0x2659e968, /// 0x358
			 0xc5c0b1e7, /// 0x35c
			 0x2d133569, /// 0x360
			 0x18f4f0da, /// 0x364
			 0x55f81b3a, /// 0x368
			 0x5953e6d6, /// 0x36c
			 0x18c9454d, /// 0x370
			 0xaa1705f2, /// 0x374
			 0x3bf59b6c, /// 0x378
			 0x93a58b69, /// 0x37c
			 0x70e7c2e5, /// 0x380
			 0x4a12e232, /// 0x384
			 0x0b8d6cf4, /// 0x388
			 0x9ead8fa2, /// 0x38c
			 0x20fa2cc1, /// 0x390
			 0xa3df4f11, /// 0x394
			 0x185b1ab3, /// 0x398
			 0xea44bde7, /// 0x39c
			 0x6b5eb744, /// 0x3a0
			 0x111ae0c6, /// 0x3a4
			 0x56938cce, /// 0x3a8
			 0x5ae9afea, /// 0x3ac
			 0x70866741, /// 0x3b0
			 0x5529a7ee, /// 0x3b4
			 0xd1e71a2a, /// 0x3b8
			 0xf73d6075, /// 0x3bc
			 0xff5241d3, /// 0x3c0
			 0x6071b305, /// 0x3c4
			 0x0bdb2fc7, /// 0x3c8
			 0x815a4b64, /// 0x3cc
			 0x4b713e4f, /// 0x3d0
			 0x4a98e492, /// 0x3d4
			 0x51b49ef4, /// 0x3d8
			 0xa1f00587, /// 0x3dc
			 0xaf03cacd, /// 0x3e0
			 0x33e85561, /// 0x3e4
			 0x325ba3bc, /// 0x3e8
			 0x1825f764, /// 0x3ec
			 0xd5f51aa9, /// 0x3f0
			 0xe902ed17, /// 0x3f4
			 0xa72a8217, /// 0x3f8
			 0x8d01834a, /// 0x3fc
			 0xc878d273, /// 0x400
			 0xc2611287, /// 0x404
			 0x9c277f30, /// 0x408
			 0xb665ad52, /// 0x40c
			 0x3afec9ad, /// 0x410
			 0xedac5c85, /// 0x414
			 0x16874b20, /// 0x418
			 0xbcfc059d, /// 0x41c
			 0xb1d998e4, /// 0x420
			 0x251b47a9, /// 0x424
			 0x370a0419, /// 0x428
			 0xa95b5f3c, /// 0x42c
			 0x9dab9ccb, /// 0x430
			 0xfad261c8, /// 0x434
			 0x314ea072, /// 0x438
			 0x8604f950, /// 0x43c
			 0xcdbb07ea, /// 0x440
			 0x2a6f0942, /// 0x444
			 0x0bfb8164, /// 0x448
			 0xe08b3e11, /// 0x44c
			 0xe5b03a8d, /// 0x450
			 0xa4db9c61, /// 0x454
			 0xd028cf48, /// 0x458
			 0x0097d50c, /// 0x45c
			 0x736f9eec, /// 0x460
			 0x3e703022, /// 0x464
			 0x0d90c7b2, /// 0x468
			 0xa3c3f793, /// 0x46c
			 0xe2be196c, /// 0x470
			 0x28ad75f8, /// 0x474
			 0x50196592, /// 0x478
			 0xb3550c09, /// 0x47c
			 0x47d795f1, /// 0x480
			 0x8fe69789, /// 0x484
			 0x1d28d1bf, /// 0x488
			 0x1b966f9b, /// 0x48c
			 0x910d7964, /// 0x490
			 0x0d7d2923, /// 0x494
			 0xdfe22ba5, /// 0x498
			 0x619b6d6c, /// 0x49c
			 0x0cea6fa3, /// 0x4a0
			 0xf4c6536c, /// 0x4a4
			 0xc3c232d0, /// 0x4a8
			 0x0d48a570, /// 0x4ac
			 0xff7e9284, /// 0x4b0
			 0xb9475e60, /// 0x4b4
			 0xa2545685, /// 0x4b8
			 0x858332e9, /// 0x4bc
			 0x9d34da1f, /// 0x4c0
			 0xf07920e3, /// 0x4c4
			 0xbbe5ff8f, /// 0x4c8
			 0xe3f31d7d, /// 0x4cc
			 0x66248a83, /// 0x4d0
			 0xdf409606, /// 0x4d4
			 0x7ec7360a, /// 0x4d8
			 0x09252975, /// 0x4dc
			 0xc8503f36, /// 0x4e0
			 0x063d11fe, /// 0x4e4
			 0xb1ee058b, /// 0x4e8
			 0x5a9fb6c9, /// 0x4ec
			 0x29224b25, /// 0x4f0
			 0xe933f3e1, /// 0x4f4
			 0x8d46c6c8, /// 0x4f8
			 0x786d31e8, /// 0x4fc
			 0x44712896, /// 0x500
			 0xee82f5bd, /// 0x504
			 0x79d05e3b, /// 0x508
			 0xafeeab46, /// 0x50c
			 0xbc66a04c, /// 0x510
			 0x1a5fc6f3, /// 0x514
			 0x97f42e6a, /// 0x518
			 0x5d5fabbb, /// 0x51c
			 0x64ab4a8d, /// 0x520
			 0x9f1d9c17, /// 0x524
			 0xbdc6aa8e, /// 0x528
			 0xe72d7f53, /// 0x52c
			 0xe981d9fd, /// 0x530
			 0x9a245f23, /// 0x534
			 0x4e1a06ac, /// 0x538
			 0xd62c379a, /// 0x53c
			 0x01d74071, /// 0x540
			 0x6bb795ea, /// 0x544
			 0x583f3d7d, /// 0x548
			 0xceefd356, /// 0x54c
			 0xd21d9d5d, /// 0x550
			 0x10bb364e, /// 0x554
			 0x5b2cbad1, /// 0x558
			 0x76e08d0f, /// 0x55c
			 0x833cf4c3, /// 0x560
			 0x5aa30fe8, /// 0x564
			 0x6ba290d0, /// 0x568
			 0xc943fbcb, /// 0x56c
			 0x59c55395, /// 0x570
			 0x79669729, /// 0x574
			 0x2d1d990b, /// 0x578
			 0xf64d808c, /// 0x57c
			 0x3301ce01, /// 0x580
			 0xa3d462a1, /// 0x584
			 0x73b7a3de, /// 0x588
			 0x9651e191, /// 0x58c
			 0x2a6da428, /// 0x590
			 0x8b6ca2f7, /// 0x594
			 0xef64fd5a, /// 0x598
			 0xaf16e243, /// 0x59c
			 0x29f27353, /// 0x5a0
			 0x8d023c9d, /// 0x5a4
			 0x8efe5c8d, /// 0x5a8
			 0xdd4be2d7, /// 0x5ac
			 0xcccb4a8d, /// 0x5b0
			 0xfb4d973c, /// 0x5b4
			 0x8c1e4615, /// 0x5b8
			 0xe3078e64, /// 0x5bc
			 0xa392e5da, /// 0x5c0
			 0x6ce36f16, /// 0x5c4
			 0xa692d87e, /// 0x5c8
			 0x291c8ac6, /// 0x5cc
			 0x7e5ec715, /// 0x5d0
			 0x5433cc63, /// 0x5d4
			 0x2af3cbd6, /// 0x5d8
			 0x6d5b8103, /// 0x5dc
			 0xbb271fe5, /// 0x5e0
			 0x2328d96f, /// 0x5e4
			 0x6b01e69b, /// 0x5e8
			 0xa8b3e622, /// 0x5ec
			 0x218cd36b, /// 0x5f0
			 0x20f6752e, /// 0x5f4
			 0x08fb6d50, /// 0x5f8
			 0xed7a8a6d, /// 0x5fc
			 0x7c70f44f, /// 0x600
			 0x8a1bb66c, /// 0x604
			 0x2d5f8758, /// 0x608
			 0x9ed96d1c, /// 0x60c
			 0xe9c4cfe0, /// 0x610
			 0xf60046c0, /// 0x614
			 0x45f68867, /// 0x618
			 0x96e1608c, /// 0x61c
			 0xe13f20dc, /// 0x620
			 0xf46a7d67, /// 0x624
			 0x4f13ef45, /// 0x628
			 0x4c7b470e, /// 0x62c
			 0x3d201804, /// 0x630
			 0xfe451ca7, /// 0x634
			 0x1b25ac08, /// 0x638
			 0x17249d98, /// 0x63c
			 0x51c55917, /// 0x640
			 0x929acee8, /// 0x644
			 0xd624d403, /// 0x648
			 0xb6962065, /// 0x64c
			 0x2fffa6a8, /// 0x650
			 0xe0db368c, /// 0x654
			 0x48c3a913, /// 0x658
			 0x7064189a, /// 0x65c
			 0x114f4b31, /// 0x660
			 0x3a7b04a4, /// 0x664
			 0xe906cd32, /// 0x668
			 0x3fd79223, /// 0x66c
			 0x327b85e2, /// 0x670
			 0xdff9e30d, /// 0x674
			 0x4d1c82d2, /// 0x678
			 0x167acffa, /// 0x67c
			 0xcc3e970e, /// 0x680
			 0x112348b0, /// 0x684
			 0x1308514c, /// 0x688
			 0xd6766bac, /// 0x68c
			 0xcd2d2689, /// 0x690
			 0xc9e0d19b, /// 0x694
			 0xaed83d54, /// 0x698
			 0x73162106, /// 0x69c
			 0xeb6e7e3d, /// 0x6a0
			 0x41f498bc, /// 0x6a4
			 0xe97a1c59, /// 0x6a8
			 0x3c22110f, /// 0x6ac
			 0xb8ada79f, /// 0x6b0
			 0x83873553, /// 0x6b4
			 0x3d820f0c, /// 0x6b8
			 0x56935b33, /// 0x6bc
			 0xcb79be77, /// 0x6c0
			 0x0e16e129, /// 0x6c4
			 0x366aa5ca, /// 0x6c8
			 0x78b42181, /// 0x6cc
			 0x5e0e517f, /// 0x6d0
			 0xe5cf4ed5, /// 0x6d4
			 0x9973977c, /// 0x6d8
			 0xcfb59444, /// 0x6dc
			 0x63df36dd, /// 0x6e0
			 0xae031792, /// 0x6e4
			 0xdf596e0e, /// 0x6e8
			 0xbc711676, /// 0x6ec
			 0xb6afeb16, /// 0x6f0
			 0xd0d0dccf, /// 0x6f4
			 0x490cc476, /// 0x6f8
			 0xfe23cf74, /// 0x6fc
			 0xb58ea083, /// 0x700
			 0xf3502b7b, /// 0x704
			 0xe853973f, /// 0x708
			 0x9ea4a7ff, /// 0x70c
			 0x9cf1e5af, /// 0x710
			 0x72b96d8a, /// 0x714
			 0xb8a82677, /// 0x718
			 0x5cd2e5d0, /// 0x71c
			 0x31c4600f, /// 0x720
			 0x81df2749, /// 0x724
			 0xe048e438, /// 0x728
			 0x1a69a813, /// 0x72c
			 0x3fac1c5f, /// 0x730
			 0x08136901, /// 0x734
			 0x15ff4e49, /// 0x738
			 0xfda1ef11, /// 0x73c
			 0xb55f9a19, /// 0x740
			 0x269767f7, /// 0x744
			 0xe448128b, /// 0x748
			 0x13c05227, /// 0x74c
			 0x4bcacdee, /// 0x750
			 0x16ab8948, /// 0x754
			 0xb7daf7c7, /// 0x758
			 0x7307cd25, /// 0x75c
			 0x29c688f9, /// 0x760
			 0x96f29425, /// 0x764
			 0x59e1d051, /// 0x768
			 0xe5f7eba0, /// 0x76c
			 0x3c735176, /// 0x770
			 0xa02e221b, /// 0x774
			 0x0733f63e, /// 0x778
			 0x5f891d9c, /// 0x77c
			 0x5eca9c74, /// 0x780
			 0x043b07ef, /// 0x784
			 0x231dafaf, /// 0x788
			 0x8fd0995b, /// 0x78c
			 0xef4059d7, /// 0x790
			 0xdc5274f3, /// 0x794
			 0x3696950b, /// 0x798
			 0x18ea7c8a, /// 0x79c
			 0x6917fc8a, /// 0x7a0
			 0xe8d31499, /// 0x7a4
			 0x16d1aba8, /// 0x7a8
			 0xa084af73, /// 0x7ac
			 0x4ab8c2af, /// 0x7b0
			 0xfdd5c964, /// 0x7b4
			 0x1d6bc747, /// 0x7b8
			 0xdd33e3bd, /// 0x7bc
			 0xf0311a17, /// 0x7c0
			 0xe3f83040, /// 0x7c4
			 0x9541150f, /// 0x7c8
			 0xdf974f2f, /// 0x7cc
			 0xb233e70d, /// 0x7d0
			 0xf4893901, /// 0x7d4
			 0xba8c6e57, /// 0x7d8
			 0xaf44223f, /// 0x7dc
			 0xc6454a5b, /// 0x7e0
			 0x650732ea, /// 0x7e4
			 0x3d3615b6, /// 0x7e8
			 0x4d5a7e07, /// 0x7ec
			 0x1a8a0bb4, /// 0x7f0
			 0x4e84d46f, /// 0x7f4
			 0x088ca85e, /// 0x7f8
			 0x128d0ae4, /// 0x7fc
			 0xf08e1dbd, /// 0x800
			 0xaab8979d, /// 0x804
			 0xb8fa7b3c, /// 0x808
			 0x7855dd54, /// 0x80c
			 0xe7dd5b94, /// 0x810
			 0xde680a45, /// 0x814
			 0x32f73fbb, /// 0x818
			 0x5c1177d3, /// 0x81c
			 0xe17b28cc, /// 0x820
			 0x6a305b12, /// 0x824
			 0xd62f1403, /// 0x828
			 0xd71a0c69, /// 0x82c
			 0xa8157244, /// 0x830
			 0x9a86559d, /// 0x834
			 0xf315cde8, /// 0x838
			 0xf027b558, /// 0x83c
			 0x2a1c93b2, /// 0x840
			 0x1702c765, /// 0x844
			 0xeb42149b, /// 0x848
			 0xe64c9004, /// 0x84c
			 0xfcaf36c0, /// 0x850
			 0x9bf1d57d, /// 0x854
			 0xbec2a04a, /// 0x858
			 0xd4c9e68e, /// 0x85c
			 0x50d0648f, /// 0x860
			 0x404c9a25, /// 0x864
			 0x4eb8822a, /// 0x868
			 0x94eafb10, /// 0x86c
			 0x7f7de0a3, /// 0x870
			 0x6f2deafd, /// 0x874
			 0xc28a489d, /// 0x878
			 0xdb9d23a3, /// 0x87c
			 0x6d04baeb, /// 0x880
			 0xe852e584, /// 0x884
			 0x99d3bec7, /// 0x888
			 0x20df15fc, /// 0x88c
			 0x600b5f72, /// 0x890
			 0xa770f8e3, /// 0x894
			 0x98e28fa8, /// 0x898
			 0xa006d84d, /// 0x89c
			 0x08fc7057, /// 0x8a0
			 0x03bda3b6, /// 0x8a4
			 0xcc7baafb, /// 0x8a8
			 0x9d60b759, /// 0x8ac
			 0x7bf048d2, /// 0x8b0
			 0xdbc0ab6e, /// 0x8b4
			 0xd6524337, /// 0x8b8
			 0xdef14fca, /// 0x8bc
			 0x67ea81b7, /// 0x8c0
			 0xcf48b236, /// 0x8c4
			 0x3ef68301, /// 0x8c8
			 0x4081bae2, /// 0x8cc
			 0x36b5f7bb, /// 0x8d0
			 0x274899c7, /// 0x8d4
			 0xdf091586, /// 0x8d8
			 0x02b3947d, /// 0x8dc
			 0x7b3e7495, /// 0x8e0
			 0x5ff5a267, /// 0x8e4
			 0x6371349a, /// 0x8e8
			 0xf9cd814d, /// 0x8ec
			 0xa71b2bb0, /// 0x8f0
			 0x7bcfbfb1, /// 0x8f4
			 0x69f3f831, /// 0x8f8
			 0x96929cff, /// 0x8fc
			 0xbc893002, /// 0x900
			 0x40c30c9a, /// 0x904
			 0xc54a10ca, /// 0x908
			 0x78f3ed5f, /// 0x90c
			 0x15455d50, /// 0x910
			 0x49a93282, /// 0x914
			 0x99d83d2e, /// 0x918
			 0xaaf2da73, /// 0x91c
			 0x90a75569, /// 0x920
			 0x770779ae, /// 0x924
			 0x8974d593, /// 0x928
			 0x6e5be09b, /// 0x92c
			 0x8a65a1be, /// 0x930
			 0x24b3d89d, /// 0x934
			 0x473c9a6e, /// 0x938
			 0xab319109, /// 0x93c
			 0xef7b39c3, /// 0x940
			 0x36e37e2c, /// 0x944
			 0xef1d5ffe, /// 0x948
			 0xabc1d370, /// 0x94c
			 0xe703e465, /// 0x950
			 0x59a01554, /// 0x954
			 0xdd5737b7, /// 0x958
			 0x95aa47e8, /// 0x95c
			 0xb91aa982, /// 0x960
			 0x9ee7aa56, /// 0x964
			 0x90f2cd50, /// 0x968
			 0x25542b8c, /// 0x96c
			 0xb2b6eb70, /// 0x970
			 0x9880eb84, /// 0x974
			 0x8ff193e2, /// 0x978
			 0x3f2b3e03, /// 0x97c
			 0x241bd446, /// 0x980
			 0x5990464b, /// 0x984
			 0x9fb25eb0, /// 0x988
			 0x1d0b2c38, /// 0x98c
			 0xbd372b6a, /// 0x990
			 0x12642e39, /// 0x994
			 0x3ff9067b, /// 0x998
			 0x6f820d15, /// 0x99c
			 0xcaddb116, /// 0x9a0
			 0x2dc030a4, /// 0x9a4
			 0x7523576f, /// 0x9a8
			 0x29e23989, /// 0x9ac
			 0x15ec8106, /// 0x9b0
			 0x99c4a9c8, /// 0x9b4
			 0xdd4cbb67, /// 0x9b8
			 0xd68fcd21, /// 0x9bc
			 0xf1e0480a, /// 0x9c0
			 0x4100a1fe, /// 0x9c4
			 0x63cbaf27, /// 0x9c8
			 0x3a99efc0, /// 0x9cc
			 0x9375c082, /// 0x9d0
			 0x366303fa, /// 0x9d4
			 0xd44100df, /// 0x9d8
			 0xa07e31b4, /// 0x9dc
			 0x520dd0fa, /// 0x9e0
			 0xee0d0860, /// 0x9e4
			 0x190e4c33, /// 0x9e8
			 0x286809af, /// 0x9ec
			 0x0a2dc6ce, /// 0x9f0
			 0x31b63bf4, /// 0x9f4
			 0xca4b05e3, /// 0x9f8
			 0x7de0f9bc, /// 0x9fc
			 0x6193933f, /// 0xa00
			 0xa1af809f, /// 0xa04
			 0x615467cb, /// 0xa08
			 0xc675d0c2, /// 0xa0c
			 0xbd8fd200, /// 0xa10
			 0x313ec6b4, /// 0xa14
			 0x9f1d7e60, /// 0xa18
			 0x28b270ee, /// 0xa1c
			 0x932c142a, /// 0xa20
			 0x26362366, /// 0xa24
			 0xe177f1db, /// 0xa28
			 0x79d2f028, /// 0xa2c
			 0xe6bf862a, /// 0xa30
			 0x203e0e1d, /// 0xa34
			 0xf5799503, /// 0xa38
			 0x45c3f717, /// 0xa3c
			 0xadbd9728, /// 0xa40
			 0x348a63f0, /// 0xa44
			 0xbcf8338e, /// 0xa48
			 0x28d697ec, /// 0xa4c
			 0x86d806c3, /// 0xa50
			 0xd83a1098, /// 0xa54
			 0xd1ac1635, /// 0xa58
			 0xdb450a0e, /// 0xa5c
			 0xc381da6d, /// 0xa60
			 0x58de19b1, /// 0xa64
			 0x496eea1c, /// 0xa68
			 0x66409879, /// 0xa6c
			 0x2388861d, /// 0xa70
			 0xcd9dc1a8, /// 0xa74
			 0x3515e4bf, /// 0xa78
			 0x9af11137, /// 0xa7c
			 0x9d636286, /// 0xa80
			 0xc09dc661, /// 0xa84
			 0xd0617821, /// 0xa88
			 0xd0abb7ea, /// 0xa8c
			 0xe3fc7003, /// 0xa90
			 0xbc4d8610, /// 0xa94
			 0x350d8666, /// 0xa98
			 0x1ae8c97f, /// 0xa9c
			 0xaae028cc, /// 0xaa0
			 0x65d2d7c7, /// 0xaa4
			 0x837ea9e3, /// 0xaa8
			 0x1a1b28bd, /// 0xaac
			 0xa9d4a7a0, /// 0xab0
			 0xc40b0b36, /// 0xab4
			 0xd783f8a7, /// 0xab8
			 0xcff0c6c3, /// 0xabc
			 0x68e85261, /// 0xac0
			 0x67f05b7e, /// 0xac4
			 0x5fb9bde5, /// 0xac8
			 0xc75638e9, /// 0xacc
			 0xcc0e522a, /// 0xad0
			 0x353b223a, /// 0xad4
			 0x2a0fc8c6, /// 0xad8
			 0xa072fe47, /// 0xadc
			 0x4b3ae181, /// 0xae0
			 0x5e8818c9, /// 0xae4
			 0x532e4a89, /// 0xae8
			 0xc1ea3f62, /// 0xaec
			 0x0a8026b0, /// 0xaf0
			 0xc7703971, /// 0xaf4
			 0xb9a77248, /// 0xaf8
			 0x153f1c0c, /// 0xafc
			 0xcc8f9ffc, /// 0xb00
			 0x5d31770a, /// 0xb04
			 0x05d4f5f1, /// 0xb08
			 0x60724ae2, /// 0xb0c
			 0x649439ed, /// 0xb10
			 0x80491b39, /// 0xb14
			 0x356310c6, /// 0xb18
			 0x92a37a22, /// 0xb1c
			 0x36bac3a1, /// 0xb20
			 0x3d54e89c, /// 0xb24
			 0x3ca7a231, /// 0xb28
			 0x406b69e5, /// 0xb2c
			 0xa3ad5b81, /// 0xb30
			 0xc4def597, /// 0xb34
			 0x4c368436, /// 0xb38
			 0x4ae8a321, /// 0xb3c
			 0xec22f796, /// 0xb40
			 0xddb8fb0b, /// 0xb44
			 0xf9e7593c, /// 0xb48
			 0x040b4d51, /// 0xb4c
			 0x01008c8e, /// 0xb50
			 0x897f2dbd, /// 0xb54
			 0xa2778202, /// 0xb58
			 0x2f2c860f, /// 0xb5c
			 0xc3778e62, /// 0xb60
			 0x200303c8, /// 0xb64
			 0xb3cd8c54, /// 0xb68
			 0x88e7bbfe, /// 0xb6c
			 0x76e75125, /// 0xb70
			 0x7d57b795, /// 0xb74
			 0x58bc2496, /// 0xb78
			 0xdc00739d, /// 0xb7c
			 0xe54e899f, /// 0xb80
			 0x7e61f418, /// 0xb84
			 0xa06c8517, /// 0xb88
			 0xd07b0422, /// 0xb8c
			 0x57cdb38f, /// 0xb90
			 0x8eaaf4b9, /// 0xb94
			 0x215cee43, /// 0xb98
			 0xcca92a56, /// 0xb9c
			 0xbdee6775, /// 0xba0
			 0x0ace68cd, /// 0xba4
			 0xbb54178d, /// 0xba8
			 0x7ed30831, /// 0xbac
			 0x2bd677ab, /// 0xbb0
			 0xaac3ba15, /// 0xbb4
			 0xbd48ebdf, /// 0xbb8
			 0x4fe446ba, /// 0xbbc
			 0x37295ded, /// 0xbc0
			 0x239645cd, /// 0xbc4
			 0x9e1871fd, /// 0xbc8
			 0xb554dc94, /// 0xbcc
			 0xa7741e67, /// 0xbd0
			 0x15ed7592, /// 0xbd4
			 0x42f1039f, /// 0xbd8
			 0x0dd65a2f, /// 0xbdc
			 0x1daf8260, /// 0xbe0
			 0xf17835ec, /// 0xbe4
			 0x188d7313, /// 0xbe8
			 0x04afda08, /// 0xbec
			 0xb73f5c5b, /// 0xbf0
			 0xe2b916b0, /// 0xbf4
			 0xaf0cb71a, /// 0xbf8
			 0x7509b319, /// 0xbfc
			 0x6f860447, /// 0xc00
			 0xf3e31b1b, /// 0xc04
			 0xdc50bffc, /// 0xc08
			 0x343b0244, /// 0xc0c
			 0x6492ddea, /// 0xc10
			 0x69681f0f, /// 0xc14
			 0xe8361c82, /// 0xc18
			 0xe41b8cd3, /// 0xc1c
			 0x5ab8f941, /// 0xc20
			 0xa75a5612, /// 0xc24
			 0x4946c6e3, /// 0xc28
			 0x2402ba44, /// 0xc2c
			 0xf9f23206, /// 0xc30
			 0xc5482b3d, /// 0xc34
			 0x6f5d0f4d, /// 0xc38
			 0x0f00b00e, /// 0xc3c
			 0xe31faaa7, /// 0xc40
			 0x68db8f60, /// 0xc44
			 0x4a919b58, /// 0xc48
			 0xd658ff40, /// 0xc4c
			 0xfe816bea, /// 0xc50
			 0x89c25680, /// 0xc54
			 0x9bb8e11f, /// 0xc58
			 0xfe84ea16, /// 0xc5c
			 0xcad702cb, /// 0xc60
			 0xf38c6bec, /// 0xc64
			 0xbd18eea5, /// 0xc68
			 0xcf95e0aa, /// 0xc6c
			 0x43f11133, /// 0xc70
			 0x091ba2f9, /// 0xc74
			 0x3ca551fe, /// 0xc78
			 0xd3f86e08, /// 0xc7c
			 0x56664ee1, /// 0xc80
			 0x13bc6893, /// 0xc84
			 0x1903ab33, /// 0xc88
			 0xc5847669, /// 0xc8c
			 0x41bd2f79, /// 0xc90
			 0xd28ffd98, /// 0xc94
			 0xd721da77, /// 0xc98
			 0x1fa0ac7e, /// 0xc9c
			 0xc41f2f11, /// 0xca0
			 0x4ae6dfa0, /// 0xca4
			 0x903a05fd, /// 0xca8
			 0xad972017, /// 0xcac
			 0x6e466352, /// 0xcb0
			 0x21fa4e00, /// 0xcb4
			 0x767810f7, /// 0xcb8
			 0xd48e9f30, /// 0xcbc
			 0x392128bc, /// 0xcc0
			 0x0eb6bd04, /// 0xcc4
			 0x5b8fb023, /// 0xcc8
			 0xa9c2b103, /// 0xccc
			 0x17b705a2, /// 0xcd0
			 0x265957e9, /// 0xcd4
			 0xbc30019b, /// 0xcd8
			 0x2cd6ad34, /// 0xcdc
			 0x1ef653fc, /// 0xce0
			 0x830c5197, /// 0xce4
			 0x3aabf204, /// 0xce8
			 0xf3056e55, /// 0xcec
			 0xca77500d, /// 0xcf0
			 0x66d942e6, /// 0xcf4
			 0x0192f567, /// 0xcf8
			 0x569ffe9e, /// 0xcfc
			 0xfd465674, /// 0xd00
			 0xffe609e1, /// 0xd04
			 0xa3eda3e2, /// 0xd08
			 0x17c00e2f, /// 0xd0c
			 0x7ac295f7, /// 0xd10
			 0xde9670be, /// 0xd14
			 0x89f88ee3, /// 0xd18
			 0x39090f07, /// 0xd1c
			 0xb44635c6, /// 0xd20
			 0x85fccebc, /// 0xd24
			 0x1d5476c9, /// 0xd28
			 0xf53dda11, /// 0xd2c
			 0x80b6f610, /// 0xd30
			 0x17f282e4, /// 0xd34
			 0xf1af7322, /// 0xd38
			 0x7501d064, /// 0xd3c
			 0xaab55004, /// 0xd40
			 0x5f3e43b1, /// 0xd44
			 0x6ce43617, /// 0xd48
			 0x9a69cae0, /// 0xd4c
			 0x19ebf464, /// 0xd50
			 0x04a2d534, /// 0xd54
			 0xa8383939, /// 0xd58
			 0x698bbc82, /// 0xd5c
			 0x63bdacc6, /// 0xd60
			 0x50e00f0d, /// 0xd64
			 0xb7f0e51e, /// 0xd68
			 0x77004749, /// 0xd6c
			 0x9c5fab94, /// 0xd70
			 0xa19fe3d2, /// 0xd74
			 0xedad2399, /// 0xd78
			 0xc79e2306, /// 0xd7c
			 0x8d496ec2, /// 0xd80
			 0x94d2a673, /// 0xd84
			 0xef3e4f53, /// 0xd88
			 0xfc758e59, /// 0xd8c
			 0x36c728e2, /// 0xd90
			 0xbbddc4f8, /// 0xd94
			 0xb4bfd650, /// 0xd98
			 0xd97fa78c, /// 0xd9c
			 0x07ada9ff, /// 0xda0
			 0x24482469, /// 0xda4
			 0xc9cd09d0, /// 0xda8
			 0x084f2987, /// 0xdac
			 0x6084dc34, /// 0xdb0
			 0x9d6a3771, /// 0xdb4
			 0x02ecad9e, /// 0xdb8
			 0x464f0c15, /// 0xdbc
			 0x1bf82b51, /// 0xdc0
			 0xb802ec16, /// 0xdc4
			 0x371a5c5f, /// 0xdc8
			 0x72f91b16, /// 0xdcc
			 0x5cd95108, /// 0xdd0
			 0x3a713926, /// 0xdd4
			 0x939a07e2, /// 0xdd8
			 0x8404611f, /// 0xddc
			 0x4697acea, /// 0xde0
			 0xef7b6fb8, /// 0xde4
			 0x5c953c57, /// 0xde8
			 0xb2be1cf5, /// 0xdec
			 0x4e14d794, /// 0xdf0
			 0xef04792e, /// 0xdf4
			 0x35f8c530, /// 0xdf8
			 0xfd8bdc7d, /// 0xdfc
			 0xc7aa28d0, /// 0xe00
			 0xa2d7edd9, /// 0xe04
			 0xd2891bf6, /// 0xe08
			 0x800e98cc, /// 0xe0c
			 0xd2bc9658, /// 0xe10
			 0x758e3b2a, /// 0xe14
			 0xe87c85c5, /// 0xe18
			 0xa8547169, /// 0xe1c
			 0xbb0fa87a, /// 0xe20
			 0x2b81bd22, /// 0xe24
			 0x19eaeaa7, /// 0xe28
			 0x5865f99f, /// 0xe2c
			 0xc9353a1c, /// 0xe30
			 0xe1eb9bff, /// 0xe34
			 0x8eb96136, /// 0xe38
			 0xc2f1548f, /// 0xe3c
			 0x97ee3ab4, /// 0xe40
			 0x8186ff13, /// 0xe44
			 0x22b6fe98, /// 0xe48
			 0xb839b686, /// 0xe4c
			 0x3b4e53c8, /// 0xe50
			 0x91a3face, /// 0xe54
			 0xf0b86378, /// 0xe58
			 0xcf3ad9e5, /// 0xe5c
			 0x0cd81295, /// 0xe60
			 0xf7e76830, /// 0xe64
			 0x0e833bd8, /// 0xe68
			 0x825f9d7c, /// 0xe6c
			 0x063380f8, /// 0xe70
			 0x2cdfcd0b, /// 0xe74
			 0x6996e992, /// 0xe78
			 0x2ee417d5, /// 0xe7c
			 0x44967cfb, /// 0xe80
			 0x18203b28, /// 0xe84
			 0x7ff836dd, /// 0xe88
			 0x54669aff, /// 0xe8c
			 0x8e886ce8, /// 0xe90
			 0x30cd7bdd, /// 0xe94
			 0x58aba736, /// 0xe98
			 0xa9f5c1fc, /// 0xe9c
			 0x75ea6f30, /// 0xea0
			 0xf49a60de, /// 0xea4
			 0xd928391b, /// 0xea8
			 0x7be2f8a0, /// 0xeac
			 0x7e74dc12, /// 0xeb0
			 0x02f4ab2e, /// 0xeb4
			 0xfd168b7c, /// 0xeb8
			 0x3e3d54a2, /// 0xebc
			 0xd3f24ce4, /// 0xec0
			 0x7e37a8ed, /// 0xec4
			 0xf985e9c9, /// 0xec8
			 0xf30bd9e2, /// 0xecc
			 0x7e0f3d5e, /// 0xed0
			 0xcb3f15dd, /// 0xed4
			 0x0d0b8a5d, /// 0xed8
			 0x4e393bd6, /// 0xedc
			 0xbf476be9, /// 0xee0
			 0xe9c23583, /// 0xee4
			 0x3f790749, /// 0xee8
			 0x6841d7c2, /// 0xeec
			 0xd657e0c5, /// 0xef0
			 0xdd24dd80, /// 0xef4
			 0x5e152482, /// 0xef8
			 0x4fe9e460, /// 0xefc
			 0x6335dc3c, /// 0xf00
			 0x6b437e68, /// 0xf04
			 0xf467b2d9, /// 0xf08
			 0xe83bcf30, /// 0xf0c
			 0xf303b675, /// 0xf10
			 0xd76aaf17, /// 0xf14
			 0x3827c08c, /// 0xf18
			 0x134c1a36, /// 0xf1c
			 0xf6d603f4, /// 0xf20
			 0xb643c242, /// 0xf24
			 0x11b7e405, /// 0xf28
			 0x57689ca8, /// 0xf2c
			 0x1cebcb89, /// 0xf30
			 0x9ca3a1a8, /// 0xf34
			 0xf1ff1f5d, /// 0xf38
			 0x7a7be022, /// 0xf3c
			 0x08637f7e, /// 0xf40
			 0x73e4ec32, /// 0xf44
			 0xcddc17bc, /// 0xf48
			 0x1706606d, /// 0xf4c
			 0xfadef23c, /// 0xf50
			 0xe7f9da3f, /// 0xf54
			 0x9f332b9d, /// 0xf58
			 0x8c33f58e, /// 0xf5c
			 0x63241240, /// 0xf60
			 0x44ff98cf, /// 0xf64
			 0x2748d6ce, /// 0xf68
			 0x971e3775, /// 0xf6c
			 0xe91dfead, /// 0xf70
			 0x7607f2b4, /// 0xf74
			 0xa372ee93, /// 0xf78
			 0x8ca04510, /// 0xf7c
			 0xcfa2af3c, /// 0xf80
			 0x6bcb8f6c, /// 0xf84
			 0xc5e43836, /// 0xf88
			 0x032c26f5, /// 0xf8c
			 0x047a85c7, /// 0xf90
			 0xf3e16ac1, /// 0xf94
			 0x06199173, /// 0xf98
			 0xd47feea9, /// 0xf9c
			 0x75374036, /// 0xfa0
			 0xb4ca2767, /// 0xfa4
			 0x67d58196, /// 0xfa8
			 0x7700e0ca, /// 0xfac
			 0xe5cf50b2, /// 0xfb0
			 0xe2bc8fa4, /// 0xfb4
			 0x06a7fdb4, /// 0xfb8
			 0x42e1c91a, /// 0xfbc
			 0x775a97de, /// 0xfc0
			 0x20200cc4, /// 0xfc4
			 0x3a8faa0d, /// 0xfc8
			 0x9c4806bd, /// 0xfcc
			 0xa4580b6c, /// 0xfd0
			 0x12f4ba7c, /// 0xfd4
			 0x9589d6be, /// 0xfd8
			 0x6e556435, /// 0xfdc
			 0x063c126f, /// 0xfe0
			 0xc93b3bb5, /// 0xfe4
			 0x5c617677, /// 0xfe8
			 0x1faaf7d7, /// 0xfec
			 0xf54e738e, /// 0xff0
			 0xe37dcef3, /// 0xff4
			 0xa8186126, /// 0xff8
			 0x8bdaa28d /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00000
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00004
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00008
			 0x0c780000,                                                  // Leading 1s:                                 /// 0000c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00010
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00014
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00018
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0001c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00020
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00024
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0002c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00030
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00044
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00048
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00050
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x7fc00001,                                                  // signaling NaN                               /// 0005c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00060
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00074
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00088
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0008c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00094
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00098
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x80000000,                                                  // -zero                                       /// 000ac
			 0x4b000000,                                                  // 8388608.0                                   /// 000b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000c0
			 0xcb000000,                                                  // -8388608.0                                  /// 000c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0xff800000,                                                  // -inf                                        /// 000e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00108
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0010c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xff800000,                                                  // -inf                                        /// 00118
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0011c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00124
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00128
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00134
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00138
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00148
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0014c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00150
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x00011111,                                                  // 9.7958E-41                                  /// 00158
			 0x0e000003,                                                  // Trailing 1s:                                /// 0015c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00160
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0016c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00174
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00184
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00194
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0c780000,                                                  // Leading 1s:                                 /// 001a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00204
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00208
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00210
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00218
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x00011111,                                                  // 9.7958E-41                                  /// 00224
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00230
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00238
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0023c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00240
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00244
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00248
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0024c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00250
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00254
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00258
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00260
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00268
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0026c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00274
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00278
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0027c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00290
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00300
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00308
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0030c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00310
			 0x0e000003,                                                  // Trailing 1s:                                /// 00314
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0031c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00320
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00324
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00328
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0032c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00334
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00338
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0033c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00344
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00348
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00350
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00354
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00360
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00370
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00374
			 0xffc00001,                                                  // -signaling NaN                              /// 00378
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0037c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00380
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00384
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00388
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0038c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00390
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00398
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003c4
			 0xcb000000,                                                  // -8388608.0                                  /// 003c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 003ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00404
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00408
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80000000,                                                  // -zero                                       /// 00410
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00414
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00418
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0xcb000000,                                                  // -8388608.0                                  /// 00428
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0042c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00434
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00438
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0043c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00011111,                                                  // 9.7958E-41                                  /// 0044c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00450
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00454
			 0x80011111,                                                  // -9.7958E-41                                 /// 00458
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0xbf028f5c,                                                  // -0.51                                       /// 00460
			 0x80000000,                                                  // -zero                                       /// 00464
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0046c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00478
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0047c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00480
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00484
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0048c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00490
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00494
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00498
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x80000000,                                                  // -zero                                       /// 004a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004ac
			 0xcb000000,                                                  // -8388608.0                                  /// 004b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x80000000,                                                  // -zero                                       /// 004bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0050c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0xffc00001,                                                  // -signaling NaN                              /// 00514
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0051c
			 0x33333333,                                                  // 4 random values                             /// 00520
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00528
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x55555555,                                                  // 4 random values                             /// 00534
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0xffc00001,                                                  // -signaling NaN                              /// 0053c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00540
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00544
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00548
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00550
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00558
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00564
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00568
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00570
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00574
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0057c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00584
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00588
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0058c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00590
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00594
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00598
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0059c
			 0x0c400000,                                                  // Leading 1s:                                 /// 005a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005c8
			 0xbf028f5c,                                                  // -0.51                                       /// 005cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0xffc00001,                                                  // -signaling NaN                              /// 005d8
			 0x00000000,                                                  // zero                                        /// 005dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005ec
			 0xffc00001,                                                  // -signaling NaN                              /// 005f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00604
			 0x80011111,                                                  // -9.7958E-41                                 /// 00608
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x0c780000,                                                  // Leading 1s:                                 /// 0061c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00620
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00628
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00638
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x7f800000,                                                  // inf                                         /// 00644
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00648
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0064c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00650
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00654
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00658
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0065c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00660
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00664
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x0c600000,                                                  // Leading 1s:                                 /// 0066c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00670
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00674
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00678
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0067c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00680
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00684
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00688
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00690
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00694
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006b0
			 0x80000000,                                                  // -zero                                       /// 006b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0xffc00001,                                                  // -signaling NaN                              /// 006c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00700
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00704
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00708
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0070c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00710
			 0x0e000001,                                                  // Trailing 1s:                                /// 00714
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00718
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0071c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00724
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00728
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0072c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00730
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00738
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0073c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00740
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x33333333,                                                  // 4 random values                             /// 00748
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0074c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00750
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00758
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0075c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00760
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00764
			 0x3f028f5c,                                                  // 0.51                                        /// 00768
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00774
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0077c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00780
			 0x7fc00001,                                                  // signaling NaN                               /// 00784
			 0xffc00001,                                                  // -signaling NaN                              /// 00788
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00790
			 0x0e000007,                                                  // Trailing 1s:                                /// 00794
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0079c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x00000000,                                                  // zero                                        /// 007f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00800
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00804
			 0x00000000,                                                  // zero                                        /// 00808
			 0x0e000001,                                                  // Trailing 1s:                                /// 0080c
			 0xffc00001,                                                  // -signaling NaN                              /// 00810
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0081c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00820
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00824
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00828
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0082c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00834
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00838
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00840
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00844
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00858
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0085c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00860
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00864
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00868
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0086c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00874
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00878
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00880
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00884
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00888
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0088c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00890
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00894
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00898
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008d8
			 0x00000000,                                                  // zero                                        /// 008dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008f0
			 0x4b000000,                                                  // 8388608.0                                   /// 008f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00904
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00908
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0090c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00910
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00914
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00918
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0091c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00920
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00924
			 0x00000000,                                                  // zero                                        /// 00928
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0092c
			 0x4b000000,                                                  // 8388608.0                                   /// 00930
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00938
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00944
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0094c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00954
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0c780000,                                                  // Leading 1s:                                 /// 0095c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00960
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00964
			 0x33333333,                                                  // 4 random values                             /// 00968
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00970
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0097c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00980
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00984
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00988
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0098c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00990
			 0xbf028f5c,                                                  // -0.51                                       /// 00994
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00998
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 009c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x7f800000,                                                  // inf                                         /// 009c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009d8
			 0x80000000,                                                  // -zero                                       /// 009dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009ec
			 0xffc00001,                                                  // -signaling NaN                              /// 009f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a00
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a44
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a64
			 0x4b000000,                                                  // 8388608.0                                   /// 00a68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a74
			 0xff800000,                                                  // -inf                                        /// 00a78
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a98
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a9c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00aa0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aa4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00abc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ac0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ac4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00acc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ad4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ae4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00af4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00af8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00afc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b04
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b08
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b10
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x4b000000,                                                  // 8388608.0                                   /// 00b2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b68
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b74
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b7c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b94
			 0x3f028f5c,                                                  // 0.51                                        /// 00b98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ba0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ba4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bbc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bd8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00be0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00be4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00be8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bf0
			 0x7f800000,                                                  // inf                                         /// 00bf4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bf8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bfc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c30
			 0xcb000000,                                                  // -8388608.0                                  /// 00c34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x80000000,                                                  // -zero                                       /// 00c58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c78
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cb4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cc8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ccc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cd4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cdc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ce0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ce4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ce8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cf8
			 0x80000000,                                                  // -zero                                       /// 00cfc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d18
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d30
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d38
			 0x33333333,                                                  // 4 random values                             /// 00d3c
			 0x00000000,                                                  // zero                                        /// 00d40
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d78
			 0x33333333,                                                  // 4 random values                             /// 00d7c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d80
			 0x00000000,                                                  // zero                                        /// 00d84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00da4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00db0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00db4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dbc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dc4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dc8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ddc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00de8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00df0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00df4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e04
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e48
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e6c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ea0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ea4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00eac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eb8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ecc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00edc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ee0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ee4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ef8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00efc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f0c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f28
			 0x55555555,                                                  // 4 random values                             /// 00f2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f30
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0xbf028f5c,                                                  // -0.51                                       /// 00f64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f7c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f90
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fb4
			 0x80000000,                                                  // -zero                                       /// 00fb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fc0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fc8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fe8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ff8
			 0x0e3fffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00008
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0000c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00018
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0xbf028f5c,                                                  // -0.51                                       /// 00020
			 0x0e000003,                                                  // Trailing 1s:                                /// 00024
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00028
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0002c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00030
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00038
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0003c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00044
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00048
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00050
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00054
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00058
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0005c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00060
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00064
			 0x7f800000,                                                  // inf                                         /// 00068
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00070
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00078
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0007c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00080
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00084
			 0x0e000003,                                                  // Trailing 1s:                                /// 00088
			 0x0c600000,                                                  // Leading 1s:                                 /// 0008c
			 0xbf028f5c,                                                  // -0.51                                       /// 00090
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00094
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00098
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0xffc00001,                                                  // -signaling NaN                              /// 000cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x00000000,                                                  // zero                                        /// 00108
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00110
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00114
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0011c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x0c780000,                                                  // Leading 1s:                                 /// 00124
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00128
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0012c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00130
			 0x80011111,                                                  // -9.7958E-41                                 /// 00134
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00138
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00140
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x0c600000,                                                  // Leading 1s:                                 /// 00148
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0014c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00150
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00158
			 0x0c600000,                                                  // Leading 1s:                                 /// 0015c
			 0x00000000,                                                  // zero                                        /// 00160
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00168
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0016c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00170
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x00011111,                                                  // 9.7958E-41                                  /// 0017c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00180
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00188
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0018c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00190
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00198
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0019c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 001cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00200
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00204
			 0xff800000,                                                  // -inf                                        /// 00208
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00000000,                                                  // zero                                        /// 00210
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00214
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0021c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x0c700000,                                                  // Leading 1s:                                 /// 00224
			 0x0c400000,                                                  // Leading 1s:                                 /// 00228
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0xcb000000,                                                  // -8388608.0                                  /// 00234
			 0x0e000007,                                                  // Trailing 1s:                                /// 00238
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0023c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00240
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00244
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00248
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00258
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0025c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00260
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00268
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0026c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00270
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00274
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0027c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00280
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x0e000001,                                                  // Trailing 1s:                                /// 00288
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00290
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00294
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0029c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0xbf028f5c,                                                  // -0.51                                       /// 002ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0xffc00001,                                                  // -signaling NaN                              /// 002f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00300
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00304
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0030c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00318
			 0xff800000,                                                  // -inf                                        /// 0031c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00328
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0032c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00330
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00334
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00338
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0033c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00344
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00348
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00350
			 0x0c780000,                                                  // Leading 1s:                                 /// 00354
			 0x4b000000,                                                  // 8388608.0                                   /// 00358
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0035c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00360
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0xff800000,                                                  // -inf                                        /// 00368
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0036c
			 0xcb000000,                                                  // -8388608.0                                  /// 00370
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00378
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0037c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00384
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00388
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0038c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00394
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00398
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0039c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x33333333,                                                  // 4 random values                             /// 003a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003c0
			 0x3f028f5c,                                                  // 0.51                                        /// 003c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003cc
			 0xffc00001,                                                  // -signaling NaN                              /// 003d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00404
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00408
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x55555555,                                                  // 4 random values                             /// 00410
			 0x55555555,                                                  // 4 random values                             /// 00414
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0041c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00420
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00428
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0042c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00430
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0043c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00440
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00448
			 0x00000000,                                                  // zero                                        /// 0044c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00450
			 0x00011111,                                                  // 9.7958E-41                                  /// 00454
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00458
			 0x55555555,                                                  // 4 random values                             /// 0045c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00460
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00464
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00468
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0046c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0xbf028f5c,                                                  // -0.51                                       /// 00478
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00484
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00488
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0048c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00494
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00498
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 004e0
			 0x3f028f5c,                                                  // 0.51                                        /// 004e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00500
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00508
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00510
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00520
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00524
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00528
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0052c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00534
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00540
			 0x4b000000,                                                  // 8388608.0                                   /// 00544
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00548
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00550
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0055c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00560
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0056c
			 0x3f028f5c,                                                  // 0.51                                        /// 00570
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00574
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0057c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00580
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00588
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0058c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00590
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0059c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x3f028f5c,                                                  // 0.51                                        /// 005e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00604
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00608
			 0x0e000007,                                                  // Trailing 1s:                                /// 0060c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00614
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00618
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0061c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00620
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00624
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00628
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0062c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00630
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0063c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00644
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00648
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0064c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00650
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00654
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00658
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0065c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00660
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00664
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00668
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0066c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00678
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0067c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00680
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00684
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0068c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00690
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0069c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0xbf028f5c,                                                  // -0.51                                       /// 006c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006dc
			 0x3f028f5c,                                                  // 0.51                                        /// 006e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x7f800000,                                                  // inf                                         /// 006f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00700
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00704
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0070c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00710
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00714
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00718
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0071c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00720
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00724
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00728
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0072c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00730
			 0x00011111,                                                  // 9.7958E-41                                  /// 00734
			 0xcb000000,                                                  // -8388608.0                                  /// 00738
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0073c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00740
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00744
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00750
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00754
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0075c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00760
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00768
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0076c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00770
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00778
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0077c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00780
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00784
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00788
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0078c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00790
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00794
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0c700000,                                                  // Leading 1s:                                 /// 007a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 007e0
			 0xcb000000,                                                  // -8388608.0                                  /// 007e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00804
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00808
			 0x0c780000,                                                  // Leading 1s:                                 /// 0080c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00810
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00818
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0081c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00820
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00828
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0082c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00830
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00834
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0083c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00844
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00848
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0084c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00854
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00858
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0085c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00868
			 0x0e000007,                                                  // Trailing 1s:                                /// 0086c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00870
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00874
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0087c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00880
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00888
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0088c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00894
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0089c
			 0x00000000,                                                  // zero                                        /// 008a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008a8
			 0x00000000,                                                  // zero                                        /// 008ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008ec
			 0x00000000,                                                  // zero                                        /// 008f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00900
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00904
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0090c
			 0x00000000,                                                  // zero                                        /// 00910
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00914
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00918
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00920
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00924
			 0x7f800000,                                                  // inf                                         /// 00928
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0092c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00938
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0093c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00940
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00944
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00948
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00954
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00958
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00964
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00968
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0096c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0097c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00980
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00988
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0098c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x00011111,                                                  // 9.7958E-41                                  /// 00994
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00998
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0xff800000,                                                  // -inf                                        /// 009a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x00000000,                                                  // zero                                        /// 009e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a00
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a18
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a28
			 0x4b000000,                                                  // 8388608.0                                   /// 00a2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a5c
			 0xff800000,                                                  // -inf                                        /// 00a60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a64
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a70
			 0x4b000000,                                                  // 8388608.0                                   /// 00a74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a8c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aa4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aa8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ab0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ab8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00abc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ac0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ac8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ad0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ad4
			 0x00000000,                                                  // zero                                        /// 00ad8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00adc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ae4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ae8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00aec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00af4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b0c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b1c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b4c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b58
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b94
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ba0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ba4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bb8
			 0x55555555,                                                  // 4 random values                             /// 00bbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bc4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bc8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bdc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x33333333,                                                  // 4 random values                             /// 00be4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00be8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bf4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bf8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0xffc00001,                                                  // -signaling NaN                              /// 00c04
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c08
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c40
			 0xcb000000,                                                  // -8388608.0                                  /// 00c44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c50
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c58
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c64
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c94
			 0x4b000000,                                                  // 8388608.0                                   /// 00c98
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ca0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ca4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ca8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cb0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cc4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cd4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cd8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ce4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ce8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cf8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d0c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d30
			 0x3f028f5c,                                                  // 0.51                                        /// 00d34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d54
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d60
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d68
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d70
			 0xbf028f5c,                                                  // -0.51                                       /// 00d74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00da8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00db8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dbc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dc0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dd0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dd4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ddc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00de0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00de4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00de8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0xbf028f5c,                                                  // -0.51                                       /// 00e2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e30
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e3c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e44
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0xff800000,                                                  // -inf                                        /// 00e64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ea0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ea4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00eac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00eb8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ebc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ec4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ecc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ed8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ef0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ef4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f00
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f34
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x3f028f5c,                                                  // 0.51                                        /// 00f3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f48
			 0x4b000000,                                                  // 8388608.0                                   /// 00f4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f74
			 0xffc00001,                                                  // -signaling NaN                              /// 00f78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fa0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fa8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fb0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fb4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fc8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fcc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fd4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fe4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fe8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ff0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ff8
			 0x0c7ffffc                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00000
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00004
			 0x4b000000,                                                  // 8388608.0                                   /// 00008
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0001c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00024
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00028
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0002c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00038
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0003c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00040
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00044
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00054
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0005c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00064
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00068
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x80000000,                                                  // -zero                                       /// 00074
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00078
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00080
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00084
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00090
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00094
			 0x0e000003,                                                  // Trailing 1s:                                /// 00098
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0009c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000a4
			 0xbf028f5c,                                                  // -0.51                                       /// 000a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 000f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00100
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0010c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00114
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00118
			 0x33333333,                                                  // 4 random values                             /// 0011c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00124
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0012c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00130
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0xcb000000,                                                  // -8388608.0                                  /// 00138
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0013c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00150
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0015c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00160
			 0x0e000001,                                                  // Trailing 1s:                                /// 00164
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00168
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00170
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00174
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00178
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0017c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0018c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00190
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0c700000,                                                  // Leading 1s:                                 /// 001a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00204
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0020c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00210
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00218
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00220
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00228
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0022c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00230
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00234
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00238
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0023c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00240
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00244
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00248
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0024c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00250
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00258
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0025c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00264
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00268
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0026c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00278
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0027c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00280
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00284
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00288
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00298
			 0x0e000003,                                                  // Trailing 1s:                                /// 0029c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002a4
			 0x7fc00001,                                                  // signaling NaN                               /// 002a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00304
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0030c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00310
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00314
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00318
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00320
			 0x0e000007,                                                  // Trailing 1s:                                /// 00324
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00328
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0032c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00330
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00334
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0033c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00340
			 0x80011111,                                                  // -9.7958E-41                                 /// 00344
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00348
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00358
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00360
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00364
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00370
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00374
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0037c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0038c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00390
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00394
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00398
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 003bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0xbf028f5c,                                                  // -0.51                                       /// 003d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003f8
			 0xff800000,                                                  // -inf                                        /// 003fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00400
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00404
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0040c
			 0x7f800000,                                                  // inf                                         /// 00410
			 0x00000000,                                                  // zero                                        /// 00414
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0xbf028f5c,                                                  // -0.51                                       /// 0041c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00420
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00430
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00434
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x7fc00001,                                                  // signaling NaN                               /// 0043c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00440
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00444
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00448
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0044c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00454
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00458
			 0x0c400000,                                                  // Leading 1s:                                 /// 0045c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00460
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00464
			 0x0e000003,                                                  // Trailing 1s:                                /// 00468
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0046c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00470
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00474
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0047c
			 0x7fc00001,                                                  // signaling NaN                               /// 00480
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00488
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0048c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00490
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00498
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00500
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00504
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0050c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00510
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00514
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00518
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0051c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00520
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00524
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0052c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00534
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00538
			 0x0c400000,                                                  // Leading 1s:                                 /// 0053c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00540
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00548
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00550
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00554
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0055c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00560
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00564
			 0x00000000,                                                  // zero                                        /// 00568
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0056c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00574
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00578
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0057c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00584
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x80000000,                                                  // -zero                                       /// 00598
			 0x80000000,                                                  // -zero                                       /// 0059c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x55555555,                                                  // 4 random values                             /// 005e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005e4
			 0x7fc00001,                                                  // signaling NaN                               /// 005e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00604
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00608
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0c400000,                                                  // Leading 1s:                                 /// 00614
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00618
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0061c
			 0x33333333,                                                  // 4 random values                             /// 00620
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00624
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00628
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0062c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00634
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00638
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00640
			 0x0c400000,                                                  // Leading 1s:                                 /// 00644
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0064c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00658
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0xff800000,                                                  // -inf                                        /// 00664
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0066c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00670
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00674
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0067c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x80011111,                                                  // -9.7958E-41                                 /// 00684
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x00000000,                                                  // zero                                        /// 0068c
			 0x3f028f5c,                                                  // 0.51                                        /// 00690
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00698
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0069c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00700
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00708
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00710
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00714
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00718
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00728
			 0x0c780000,                                                  // Leading 1s:                                 /// 0072c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00730
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00734
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00738
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0073c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00740
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00748
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0074c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00750
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00758
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0075c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00000000,                                                  // zero                                        /// 00768
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0076c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00774
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00778
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00780
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0078c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00790
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00794
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00798
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 007c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00800
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00804
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00808
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00810
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00814
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0081c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00820
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00828
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0082c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00830
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00834
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00840
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00848
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00850
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00854
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0085c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00864
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00868
			 0xcb000000,                                                  // -8388608.0                                  /// 0086c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0087c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00880
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00884
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00888
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00890
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00894
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0089c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x7f800000,                                                  // inf                                         /// 008a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x4b000000,                                                  // 8388608.0                                   /// 008ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008b8
			 0x7f800000,                                                  // inf                                         /// 008bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008cc
			 0x7f800000,                                                  // inf                                         /// 008d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 008e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00904
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0090c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00918
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0091c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00920
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00928
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0092c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00930
			 0x0e000001,                                                  // Trailing 1s:                                /// 00934
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00940
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00948
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0094c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00954
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00960
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00964
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00968
			 0x80011111,                                                  // -9.7958E-41                                 /// 0096c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00970
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00974
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00978
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00980
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00984
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0098c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00990
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00994
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00998
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0099c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d4
			 0x55555555,                                                  // 4 random values                             /// 009d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a08
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a14
			 0x4b000000,                                                  // 8388608.0                                   /// 00a18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a40
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a68
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a70
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a88
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a94
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a98
			 0x4b000000,                                                  // 8388608.0                                   /// 00a9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00aa0
			 0x80000000,                                                  // -zero                                       /// 00aa4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ab0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ab4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ab8
			 0x7f800000,                                                  // inf                                         /// 00abc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00acc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ad0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00adc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ae0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ae4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ae8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00af0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00af4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b0c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x7fc00001,                                                  // signaling NaN                               /// 00b18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b20
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b50
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b5c
			 0x55555555,                                                  // 4 random values                             /// 00b60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b78
			 0x80000000,                                                  // -zero                                       /// 00b7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b88
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ba0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ba8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0xbf028f5c,                                                  // -0.51                                       /// 00bb0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bb8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bbc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bc8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bd8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bdc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x33333333,                                                  // 4 random values                             /// 00bec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bf8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bfc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c04
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c10
			 0xffc00001,                                                  // -signaling NaN                              /// 00c14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c18
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c1c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c2c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c4c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c50
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c54
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c58
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c68
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c70
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c78
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c90
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ca0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ca4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ca8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cac
			 0x3f028f5c,                                                  // 0.51                                        /// 00cb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cb8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cbc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ccc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cd8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ce4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ce8
			 0xff800000,                                                  // -inf                                        /// 00cec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cf0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cf4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cfc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d18
			 0x7f800000,                                                  // inf                                         /// 00d1c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d58
			 0xffc00001,                                                  // -signaling NaN                              /// 00d5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d60
			 0xbf028f5c,                                                  // -0.51                                       /// 00d64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d7c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d80
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00da4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00db4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00db8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00dd0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dd4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dd8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ddc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00de0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00de8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00df0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00df4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00df8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dfc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e10
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e14
			 0xbf028f5c,                                                  // -0.51                                       /// 00e18
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e38
			 0x7fc00001,                                                  // signaling NaN                               /// 00e3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e60
			 0x7f800000,                                                  // inf                                         /// 00e64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e74
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e84
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e88
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00ea0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ea4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ea8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00eac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eb0
			 0xbf028f5c,                                                  // -0.51                                       /// 00eb4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ecc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ed0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ed8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00edc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ee0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x55555555,                                                  // 4 random values                             /// 00ee8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ef0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ef8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00efc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f18
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f1c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f50
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f5c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f78
			 0xbf028f5c,                                                  // -0.51                                       /// 00f7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x4b000000,                                                  // 8388608.0                                   /// 00f98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fa0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fac
			 0x80000000,                                                  // -zero                                       /// 00fb0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fcc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ff8
			 0x0c7ff800                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xd0e7a5e6, /// 0x0
			 0xff37982e, /// 0x4
			 0x5cc9b570, /// 0x8
			 0x3ba11bd2, /// 0xc
			 0xbea23913, /// 0x10
			 0xa9cd946c, /// 0x14
			 0x9d388414, /// 0x18
			 0xf0f3fece, /// 0x1c
			 0x6b5d22e6, /// 0x20
			 0xf452734a, /// 0x24
			 0x70867ce5, /// 0x28
			 0x9a6423e1, /// 0x2c
			 0x619b6055, /// 0x30
			 0x1b1979fd, /// 0x34
			 0x250449c5, /// 0x38
			 0xf0d805cf, /// 0x3c
			 0x4f0f248e, /// 0x40
			 0x66d1ab43, /// 0x44
			 0x67f02534, /// 0x48
			 0x8a7c2878, /// 0x4c
			 0x1149c26c, /// 0x50
			 0x3ec83214, /// 0x54
			 0x5a0b41f5, /// 0x58
			 0xce40ad95, /// 0x5c
			 0x805e68dc, /// 0x60
			 0xfb78c74d, /// 0x64
			 0x4c17c899, /// 0x68
			 0x44b3c89e, /// 0x6c
			 0x270d3098, /// 0x70
			 0xa902f77c, /// 0x74
			 0xb779a58a, /// 0x78
			 0xf3e65ff6, /// 0x7c
			 0xfb145520, /// 0x80
			 0xf0041b41, /// 0x84
			 0xdca817fd, /// 0x88
			 0xa25edfb2, /// 0x8c
			 0xacc81ac0, /// 0x90
			 0x9f93c650, /// 0x94
			 0xfa082ffe, /// 0x98
			 0xa60bddb0, /// 0x9c
			 0x41dabcf4, /// 0xa0
			 0x109127ea, /// 0xa4
			 0xa8513a39, /// 0xa8
			 0x3f570c70, /// 0xac
			 0x10430994, /// 0xb0
			 0x9cce848f, /// 0xb4
			 0x49674c48, /// 0xb8
			 0xbc4657bc, /// 0xbc
			 0x20183296, /// 0xc0
			 0xc6f7d74f, /// 0xc4
			 0xb5683522, /// 0xc8
			 0xf901f88f, /// 0xcc
			 0x8dbedd40, /// 0xd0
			 0xa487a787, /// 0xd4
			 0x6c67afc9, /// 0xd8
			 0x828fbf18, /// 0xdc
			 0xd191edd3, /// 0xe0
			 0xfb9feece, /// 0xe4
			 0xd1dfdecd, /// 0xe8
			 0x63425c4b, /// 0xec
			 0x5883f83c, /// 0xf0
			 0x9782f4da, /// 0xf4
			 0x943513e0, /// 0xf8
			 0x3a3e4f38, /// 0xfc
			 0xff861858, /// 0x100
			 0x5712081e, /// 0x104
			 0xe9f2c4b5, /// 0x108
			 0xb284f9ab, /// 0x10c
			 0x7ec6fe4f, /// 0x110
			 0x72214138, /// 0x114
			 0x78ad3c82, /// 0x118
			 0x0e9e47c1, /// 0x11c
			 0x18a667bb, /// 0x120
			 0xe75398c3, /// 0x124
			 0xd27cb492, /// 0x128
			 0x451cf368, /// 0x12c
			 0x3cb45b3a, /// 0x130
			 0x7786d743, /// 0x134
			 0x1d91b68f, /// 0x138
			 0x02797110, /// 0x13c
			 0x3bfd5a55, /// 0x140
			 0x8692a86b, /// 0x144
			 0xc48dd304, /// 0x148
			 0xc4354f26, /// 0x14c
			 0xe3d0cb0f, /// 0x150
			 0x76e7e209, /// 0x154
			 0xdfae4969, /// 0x158
			 0x50b67543, /// 0x15c
			 0x55f3b181, /// 0x160
			 0x1eb598cf, /// 0x164
			 0xbc9bb3c1, /// 0x168
			 0x5a840513, /// 0x16c
			 0x5edd1498, /// 0x170
			 0x83816e94, /// 0x174
			 0x56307c2a, /// 0x178
			 0x5ce5b9c1, /// 0x17c
			 0x2fa8e27a, /// 0x180
			 0xa318dd4c, /// 0x184
			 0x0e3dd3c6, /// 0x188
			 0x6d5bb49b, /// 0x18c
			 0x865ad625, /// 0x190
			 0xc55b1fa1, /// 0x194
			 0xe260f9de, /// 0x198
			 0x280593b0, /// 0x19c
			 0x49ac0c00, /// 0x1a0
			 0x641baa2d, /// 0x1a4
			 0x096cfbfa, /// 0x1a8
			 0x5e525058, /// 0x1ac
			 0x2a62dd83, /// 0x1b0
			 0x809284eb, /// 0x1b4
			 0x48c7b2cb, /// 0x1b8
			 0xd38bcffe, /// 0x1bc
			 0xb3ff9f6d, /// 0x1c0
			 0x1bab9d9d, /// 0x1c4
			 0xbc90b412, /// 0x1c8
			 0xdf839ca3, /// 0x1cc
			 0x147d6c17, /// 0x1d0
			 0xf2e39937, /// 0x1d4
			 0x4d84d407, /// 0x1d8
			 0x1dc296f5, /// 0x1dc
			 0xf1a702d9, /// 0x1e0
			 0xeb1e183b, /// 0x1e4
			 0x77afc852, /// 0x1e8
			 0xa33d95e4, /// 0x1ec
			 0x1f6d8441, /// 0x1f0
			 0x03673759, /// 0x1f4
			 0x50e1bf0f, /// 0x1f8
			 0xc92aaaf6, /// 0x1fc
			 0xdf2669c9, /// 0x200
			 0x65a56ca8, /// 0x204
			 0x378a6c78, /// 0x208
			 0xa9312173, /// 0x20c
			 0xced72f3b, /// 0x210
			 0x4a466530, /// 0x214
			 0x6c80256f, /// 0x218
			 0x8a519825, /// 0x21c
			 0xa16fba55, /// 0x220
			 0x03750ebc, /// 0x224
			 0x5896d312, /// 0x228
			 0x4d558b1b, /// 0x22c
			 0x071a3026, /// 0x230
			 0x07610927, /// 0x234
			 0xe377c14f, /// 0x238
			 0x94a45eaf, /// 0x23c
			 0x41a2b23c, /// 0x240
			 0xbc183069, /// 0x244
			 0x8dc486c2, /// 0x248
			 0xb7c5d25c, /// 0x24c
			 0x6533df7b, /// 0x250
			 0x0f53c400, /// 0x254
			 0xb0691387, /// 0x258
			 0xa0b5e795, /// 0x25c
			 0x6c0443e0, /// 0x260
			 0xa0c58d4d, /// 0x264
			 0xcf606a31, /// 0x268
			 0xdca8c518, /// 0x26c
			 0x6a077d90, /// 0x270
			 0x3f01f1ef, /// 0x274
			 0xcf0bc56a, /// 0x278
			 0x4b724af7, /// 0x27c
			 0xeba89a45, /// 0x280
			 0x10ab379a, /// 0x284
			 0xd22b5016, /// 0x288
			 0xb0c89a6f, /// 0x28c
			 0xc846129d, /// 0x290
			 0x906f1fe1, /// 0x294
			 0xba6d4232, /// 0x298
			 0xa14883a0, /// 0x29c
			 0x598bf1e2, /// 0x2a0
			 0x7f8ea12f, /// 0x2a4
			 0x337e5449, /// 0x2a8
			 0xaf976c2c, /// 0x2ac
			 0xead77d9c, /// 0x2b0
			 0x79a6fe0b, /// 0x2b4
			 0x65b74dad, /// 0x2b8
			 0xc4ff9be6, /// 0x2bc
			 0x316701aa, /// 0x2c0
			 0xc2a892aa, /// 0x2c4
			 0xf39ac153, /// 0x2c8
			 0x8ddc9830, /// 0x2cc
			 0x774fb45a, /// 0x2d0
			 0xb488b293, /// 0x2d4
			 0x403843ee, /// 0x2d8
			 0xe30f8f73, /// 0x2dc
			 0x80022d85, /// 0x2e0
			 0x2198adfc, /// 0x2e4
			 0x9cab1409, /// 0x2e8
			 0xd95f4edf, /// 0x2ec
			 0x2fd96f95, /// 0x2f0
			 0xf4df5e77, /// 0x2f4
			 0x59c9684f, /// 0x2f8
			 0x4e90cb43, /// 0x2fc
			 0x76b516ca, /// 0x300
			 0x73c36e0d, /// 0x304
			 0x81157169, /// 0x308
			 0x137dfd0d, /// 0x30c
			 0x7bba742f, /// 0x310
			 0x7c8f0096, /// 0x314
			 0xafa7bb38, /// 0x318
			 0x54e24557, /// 0x31c
			 0xd4ffbce7, /// 0x320
			 0x815d8013, /// 0x324
			 0xfabf4f3a, /// 0x328
			 0x46f95986, /// 0x32c
			 0xa81a6c18, /// 0x330
			 0xfd388686, /// 0x334
			 0xdb0a0837, /// 0x338
			 0xa6ae8090, /// 0x33c
			 0xb0cbc489, /// 0x340
			 0x8540e9aa, /// 0x344
			 0x913e6ee1, /// 0x348
			 0x5cd2c43d, /// 0x34c
			 0x90777c22, /// 0x350
			 0x35ca1574, /// 0x354
			 0xa4d8b2ef, /// 0x358
			 0x7fe90e9a, /// 0x35c
			 0x41bcf6cb, /// 0x360
			 0xed27e83e, /// 0x364
			 0x031ca9f7, /// 0x368
			 0xf8679585, /// 0x36c
			 0x26ed3ee0, /// 0x370
			 0xbca16b0d, /// 0x374
			 0x34b0c4a5, /// 0x378
			 0xe41a07f8, /// 0x37c
			 0xf2032739, /// 0x380
			 0x68e66b68, /// 0x384
			 0x2dfd29cf, /// 0x388
			 0x8feff375, /// 0x38c
			 0x5a595f0a, /// 0x390
			 0xa86d67c0, /// 0x394
			 0x67bba991, /// 0x398
			 0x8a829b6c, /// 0x39c
			 0x934ae99a, /// 0x3a0
			 0xa9bd0752, /// 0x3a4
			 0xff371504, /// 0x3a8
			 0xe58e9b65, /// 0x3ac
			 0x15fea76e, /// 0x3b0
			 0xc20bfeea, /// 0x3b4
			 0x9a9fb355, /// 0x3b8
			 0xe78577cc, /// 0x3bc
			 0x67473869, /// 0x3c0
			 0xbffdca8a, /// 0x3c4
			 0x99906bac, /// 0x3c8
			 0x403a2466, /// 0x3cc
			 0x2f36bcbb, /// 0x3d0
			 0x4836ae4e, /// 0x3d4
			 0xf938706b, /// 0x3d8
			 0x818c2528, /// 0x3dc
			 0x11b16581, /// 0x3e0
			 0xcc919302, /// 0x3e4
			 0x0f92cf16, /// 0x3e8
			 0x37fb9ba3, /// 0x3ec
			 0x5815aa48, /// 0x3f0
			 0x42356778, /// 0x3f4
			 0x6c9887c9, /// 0x3f8
			 0xd58c3927, /// 0x3fc
			 0xede7cef3, /// 0x400
			 0x34e12cd0, /// 0x404
			 0x10a379c4, /// 0x408
			 0xb923f5d1, /// 0x40c
			 0xddb13319, /// 0x410
			 0xcaed1aa5, /// 0x414
			 0x4d3b3a85, /// 0x418
			 0xd80c28aa, /// 0x41c
			 0x660dbd06, /// 0x420
			 0x0f1f2f78, /// 0x424
			 0xcd75d83b, /// 0x428
			 0x84132e93, /// 0x42c
			 0xaa60c620, /// 0x430
			 0xc71f2e9f, /// 0x434
			 0xc16b4017, /// 0x438
			 0x12cb612b, /// 0x43c
			 0xca918272, /// 0x440
			 0x542c1ce4, /// 0x444
			 0x7bb305fe, /// 0x448
			 0x94151190, /// 0x44c
			 0xae07c675, /// 0x450
			 0x590b197e, /// 0x454
			 0xf531a177, /// 0x458
			 0x5f103473, /// 0x45c
			 0x50f0df65, /// 0x460
			 0xf896c64d, /// 0x464
			 0xe3d1676a, /// 0x468
			 0x467429b2, /// 0x46c
			 0x59486468, /// 0x470
			 0xdfc87de8, /// 0x474
			 0x81ae4eec, /// 0x478
			 0xea9964fb, /// 0x47c
			 0x048ddcaa, /// 0x480
			 0xc0919e4e, /// 0x484
			 0xc181c592, /// 0x488
			 0x9b8f5b0c, /// 0x48c
			 0x573cd571, /// 0x490
			 0xc38e411f, /// 0x494
			 0x29a27558, /// 0x498
			 0x94b8bb4f, /// 0x49c
			 0xfd8f2b35, /// 0x4a0
			 0x0ffe8f3c, /// 0x4a4
			 0x2fc70c72, /// 0x4a8
			 0x0b486dc3, /// 0x4ac
			 0x91b269b8, /// 0x4b0
			 0xa769271b, /// 0x4b4
			 0x5dab9890, /// 0x4b8
			 0x91ffece9, /// 0x4bc
			 0x1f7064e5, /// 0x4c0
			 0x77d3bd56, /// 0x4c4
			 0x915cdaa4, /// 0x4c8
			 0x2f658451, /// 0x4cc
			 0xf29293a4, /// 0x4d0
			 0x180b2b0b, /// 0x4d4
			 0x181ce8e2, /// 0x4d8
			 0x71587dd6, /// 0x4dc
			 0xad77c847, /// 0x4e0
			 0xb8a77763, /// 0x4e4
			 0xef274a3d, /// 0x4e8
			 0x7650bd73, /// 0x4ec
			 0xd62b2bf8, /// 0x4f0
			 0xbcd84bcd, /// 0x4f4
			 0x2a0d75ce, /// 0x4f8
			 0x24cdc3c9, /// 0x4fc
			 0x59c21f45, /// 0x500
			 0x2aaec306, /// 0x504
			 0xcd2e8aa1, /// 0x508
			 0xf9df0616, /// 0x50c
			 0xc15c4e7b, /// 0x510
			 0x9bc9739c, /// 0x514
			 0xf62c06c0, /// 0x518
			 0x5a08b169, /// 0x51c
			 0x4239d85c, /// 0x520
			 0x32ab5c85, /// 0x524
			 0xa06068f3, /// 0x528
			 0xd1189aa3, /// 0x52c
			 0xe7748bba, /// 0x530
			 0x7191b71e, /// 0x534
			 0xe6114af9, /// 0x538
			 0xb5908e3c, /// 0x53c
			 0x8523d759, /// 0x540
			 0x82b0d44a, /// 0x544
			 0xa1798fb6, /// 0x548
			 0xd6d5bca0, /// 0x54c
			 0x99649bb1, /// 0x550
			 0xe78b8e3e, /// 0x554
			 0x54a4634a, /// 0x558
			 0xab5a580a, /// 0x55c
			 0xd8b24d30, /// 0x560
			 0x6ac8bef8, /// 0x564
			 0xb9b7664a, /// 0x568
			 0xe6f15a62, /// 0x56c
			 0x2132739a, /// 0x570
			 0x62709319, /// 0x574
			 0xd6d7c5a2, /// 0x578
			 0xd9990ca4, /// 0x57c
			 0xe26ea597, /// 0x580
			 0x649ebb0d, /// 0x584
			 0x8c2bf002, /// 0x588
			 0x87d874ba, /// 0x58c
			 0xa7954cf2, /// 0x590
			 0xba3cd87f, /// 0x594
			 0x7da16340, /// 0x598
			 0x1f407917, /// 0x59c
			 0xb1dc2920, /// 0x5a0
			 0x95954705, /// 0x5a4
			 0xbd5c2a16, /// 0x5a8
			 0x96e0b1eb, /// 0x5ac
			 0x34868615, /// 0x5b0
			 0x4d491a32, /// 0x5b4
			 0xdc27850d, /// 0x5b8
			 0x26e6c7c3, /// 0x5bc
			 0x18b826c3, /// 0x5c0
			 0xc0db6aac, /// 0x5c4
			 0x180f8bef, /// 0x5c8
			 0xa5abd132, /// 0x5cc
			 0xad7a2285, /// 0x5d0
			 0xba55af2a, /// 0x5d4
			 0xe13d20cb, /// 0x5d8
			 0x0facc651, /// 0x5dc
			 0x6c25a70e, /// 0x5e0
			 0xfb4c4794, /// 0x5e4
			 0x3eaae085, /// 0x5e8
			 0x71e8699c, /// 0x5ec
			 0x3193ea0a, /// 0x5f0
			 0x0adc6324, /// 0x5f4
			 0xce2f0cd1, /// 0x5f8
			 0xfbad5b98, /// 0x5fc
			 0xe8f17f61, /// 0x600
			 0x86706309, /// 0x604
			 0x587021d1, /// 0x608
			 0x61d699ca, /// 0x60c
			 0xdf062b1c, /// 0x610
			 0x65a89c38, /// 0x614
			 0xbf8de646, /// 0x618
			 0x42479a5b, /// 0x61c
			 0x06192f1a, /// 0x620
			 0x4eee15ac, /// 0x624
			 0x7781e89d, /// 0x628
			 0x13a952c0, /// 0x62c
			 0x5f11f682, /// 0x630
			 0xb2e7e9b7, /// 0x634
			 0x1f635321, /// 0x638
			 0x02b2f548, /// 0x63c
			 0x3de74099, /// 0x640
			 0xe11cf426, /// 0x644
			 0x77f2db2d, /// 0x648
			 0x9075f0eb, /// 0x64c
			 0xd9a22a22, /// 0x650
			 0x6af3aa13, /// 0x654
			 0x8cc42c66, /// 0x658
			 0xaaa2d8e4, /// 0x65c
			 0x82521e23, /// 0x660
			 0xd7d2e5aa, /// 0x664
			 0xffabec2a, /// 0x668
			 0x2e1cc0f1, /// 0x66c
			 0xab1cc20e, /// 0x670
			 0x2616f3d9, /// 0x674
			 0x9712fcbf, /// 0x678
			 0x3889656c, /// 0x67c
			 0x8ef97f32, /// 0x680
			 0xeb5d96ff, /// 0x684
			 0x5e715f9a, /// 0x688
			 0x20113648, /// 0x68c
			 0x1728671f, /// 0x690
			 0x302ab066, /// 0x694
			 0x15be5c4d, /// 0x698
			 0xc952dfba, /// 0x69c
			 0x7e831718, /// 0x6a0
			 0x1bf9a7e6, /// 0x6a4
			 0x1dbfc0eb, /// 0x6a8
			 0xdf2a2e9f, /// 0x6ac
			 0xd903cdca, /// 0x6b0
			 0x39a665ae, /// 0x6b4
			 0x70881a3e, /// 0x6b8
			 0x918375dc, /// 0x6bc
			 0xc40b9dd0, /// 0x6c0
			 0x6bb3ec3e, /// 0x6c4
			 0xa0aec712, /// 0x6c8
			 0x98a42f0f, /// 0x6cc
			 0x243af95c, /// 0x6d0
			 0x6e8993d4, /// 0x6d4
			 0x607cbfc4, /// 0x6d8
			 0x2ddc5f0c, /// 0x6dc
			 0x519bc26c, /// 0x6e0
			 0x4f3ec61d, /// 0x6e4
			 0xf083363d, /// 0x6e8
			 0xcefcf55a, /// 0x6ec
			 0x0870bbe9, /// 0x6f0
			 0xc38701c5, /// 0x6f4
			 0x66ba0a3b, /// 0x6f8
			 0x10037e62, /// 0x6fc
			 0x3ce31395, /// 0x700
			 0xe2d337fc, /// 0x704
			 0xa3422a9f, /// 0x708
			 0xbb942b40, /// 0x70c
			 0xc5acab54, /// 0x710
			 0x225a58ba, /// 0x714
			 0x80d4534f, /// 0x718
			 0x032b1bb2, /// 0x71c
			 0x422ca519, /// 0x720
			 0x141c4216, /// 0x724
			 0x1f0f57a3, /// 0x728
			 0x53537371, /// 0x72c
			 0xea39be6d, /// 0x730
			 0x9b7584c4, /// 0x734
			 0x4664c069, /// 0x738
			 0xd122c608, /// 0x73c
			 0xbf23ad13, /// 0x740
			 0xfdd93713, /// 0x744
			 0x7abd60aa, /// 0x748
			 0xccd99118, /// 0x74c
			 0x00c5d43a, /// 0x750
			 0x3e5e069f, /// 0x754
			 0x4f6fbd79, /// 0x758
			 0xcae7c932, /// 0x75c
			 0x7ba1a21c, /// 0x760
			 0x338ba9a8, /// 0x764
			 0x7bb72445, /// 0x768
			 0x957a367f, /// 0x76c
			 0x9c57b876, /// 0x770
			 0x92cc2937, /// 0x774
			 0x92ee93a6, /// 0x778
			 0xac177cbf, /// 0x77c
			 0x6fdf5617, /// 0x780
			 0x9042ec8a, /// 0x784
			 0x6ae89fbb, /// 0x788
			 0xea37f1e8, /// 0x78c
			 0x9faafd51, /// 0x790
			 0xbefff7fb, /// 0x794
			 0x2836398f, /// 0x798
			 0x19af0e06, /// 0x79c
			 0xe8e96bb6, /// 0x7a0
			 0xb256aa7f, /// 0x7a4
			 0x8fef758a, /// 0x7a8
			 0x2c6871cb, /// 0x7ac
			 0x185cd065, /// 0x7b0
			 0x2c33a6af, /// 0x7b4
			 0x4004eef1, /// 0x7b8
			 0x2a70e4a8, /// 0x7bc
			 0xef7cc38c, /// 0x7c0
			 0x71e6eb6e, /// 0x7c4
			 0xd3e8ffd1, /// 0x7c8
			 0x1c5b5e34, /// 0x7cc
			 0x7504c045, /// 0x7d0
			 0x571fe6bc, /// 0x7d4
			 0xf65e74dc, /// 0x7d8
			 0x9b1fe5f0, /// 0x7dc
			 0x3bbce26c, /// 0x7e0
			 0x76cad7b4, /// 0x7e4
			 0x79c2b17d, /// 0x7e8
			 0x788bc6c3, /// 0x7ec
			 0xf59cebf8, /// 0x7f0
			 0xafe3fc34, /// 0x7f4
			 0x21511840, /// 0x7f8
			 0xb45ae544, /// 0x7fc
			 0x2e77978b, /// 0x800
			 0x67fae63a, /// 0x804
			 0x0ebb3aac, /// 0x808
			 0x829a3a38, /// 0x80c
			 0xf2d609e1, /// 0x810
			 0xed2ccbe5, /// 0x814
			 0xbc398066, /// 0x818
			 0xf3a10a7e, /// 0x81c
			 0x4bd3fd1c, /// 0x820
			 0xf120464d, /// 0x824
			 0x5c4b6001, /// 0x828
			 0x8322db23, /// 0x82c
			 0x1819f320, /// 0x830
			 0x40cd3770, /// 0x834
			 0x4972f50f, /// 0x838
			 0xbf18f181, /// 0x83c
			 0x7a3b7f4d, /// 0x840
			 0xd5e3d794, /// 0x844
			 0x8036359b, /// 0x848
			 0xcc06c99c, /// 0x84c
			 0x092000dd, /// 0x850
			 0x3ad7ad9d, /// 0x854
			 0xf26ce9b1, /// 0x858
			 0xee1bbeb3, /// 0x85c
			 0x5c8a7e8b, /// 0x860
			 0x571a5505, /// 0x864
			 0x8b33f47a, /// 0x868
			 0x1c2a5523, /// 0x86c
			 0xc9a240ed, /// 0x870
			 0x386028eb, /// 0x874
			 0xeacfba8f, /// 0x878
			 0xc89c53b3, /// 0x87c
			 0xff919f79, /// 0x880
			 0x401e1da7, /// 0x884
			 0x193bb20e, /// 0x888
			 0x6136b954, /// 0x88c
			 0xedd05ef0, /// 0x890
			 0xb8248b05, /// 0x894
			 0x57cf3c62, /// 0x898
			 0x72ea5a39, /// 0x89c
			 0xe0cc9b8f, /// 0x8a0
			 0xeedee60c, /// 0x8a4
			 0x28b285f6, /// 0x8a8
			 0xb857413c, /// 0x8ac
			 0x2b52b9d2, /// 0x8b0
			 0x5a512c4b, /// 0x8b4
			 0x2ae1f918, /// 0x8b8
			 0x15a9f5b6, /// 0x8bc
			 0x7e7f3b2b, /// 0x8c0
			 0x60d9c2ba, /// 0x8c4
			 0x87816b3d, /// 0x8c8
			 0xe175a604, /// 0x8cc
			 0x28805be9, /// 0x8d0
			 0x4b0a683f, /// 0x8d4
			 0x76db9916, /// 0x8d8
			 0x1afedb10, /// 0x8dc
			 0x1c640956, /// 0x8e0
			 0x9ff7edbe, /// 0x8e4
			 0x1f8e3e29, /// 0x8e8
			 0xdead633f, /// 0x8ec
			 0x8f998fcd, /// 0x8f0
			 0x3e067020, /// 0x8f4
			 0xc2722253, /// 0x8f8
			 0xf4eeb86a, /// 0x8fc
			 0x68a9eb5f, /// 0x900
			 0x4037454c, /// 0x904
			 0x732f4b43, /// 0x908
			 0x9cbd7f40, /// 0x90c
			 0x17cd266c, /// 0x910
			 0x35a4489d, /// 0x914
			 0x6aba9cc6, /// 0x918
			 0x5eef63db, /// 0x91c
			 0x02b551b5, /// 0x920
			 0x3c870ded, /// 0x924
			 0x4847565a, /// 0x928
			 0x1edb2a99, /// 0x92c
			 0xa4369b45, /// 0x930
			 0xb4a19dab, /// 0x934
			 0x2eee55e2, /// 0x938
			 0xc66894f7, /// 0x93c
			 0x4f027456, /// 0x940
			 0x69dbc62b, /// 0x944
			 0x596d2542, /// 0x948
			 0xed763ce2, /// 0x94c
			 0x06cfc978, /// 0x950
			 0x2f954430, /// 0x954
			 0xc7cc77b4, /// 0x958
			 0x22ce7a4b, /// 0x95c
			 0xd1cd7d9a, /// 0x960
			 0x4cadc201, /// 0x964
			 0xea2bb923, /// 0x968
			 0x03407384, /// 0x96c
			 0xd0946ff4, /// 0x970
			 0xe04402dd, /// 0x974
			 0x6b0a4337, /// 0x978
			 0x461d35ad, /// 0x97c
			 0x4726a3c9, /// 0x980
			 0xdd8fd6ff, /// 0x984
			 0x7dad7b0b, /// 0x988
			 0x25be61a7, /// 0x98c
			 0x2113e3a2, /// 0x990
			 0xfaebe0c0, /// 0x994
			 0x94e63feb, /// 0x998
			 0xcd76d30f, /// 0x99c
			 0x26af52ca, /// 0x9a0
			 0x17f785b9, /// 0x9a4
			 0xe6e1427a, /// 0x9a8
			 0x7563cd25, /// 0x9ac
			 0x372aaaf6, /// 0x9b0
			 0x8c839b4d, /// 0x9b4
			 0x17d9bb8e, /// 0x9b8
			 0xd763f68b, /// 0x9bc
			 0x6ca62831, /// 0x9c0
			 0x5c2034bf, /// 0x9c4
			 0x9deca719, /// 0x9c8
			 0xad4232e7, /// 0x9cc
			 0x0209f4f8, /// 0x9d0
			 0x4aece17b, /// 0x9d4
			 0x67cf57c9, /// 0x9d8
			 0x533bccb7, /// 0x9dc
			 0x3f3e0a79, /// 0x9e0
			 0xa954f473, /// 0x9e4
			 0x573ec094, /// 0x9e8
			 0x371f58f4, /// 0x9ec
			 0xf10696bc, /// 0x9f0
			 0xb7c8eeb2, /// 0x9f4
			 0x695b2acd, /// 0x9f8
			 0xcad66e74, /// 0x9fc
			 0xdcb59800, /// 0xa00
			 0x2efb6484, /// 0xa04
			 0x82e6a1e7, /// 0xa08
			 0xe27f924e, /// 0xa0c
			 0x44ab6769, /// 0xa10
			 0xbede5116, /// 0xa14
			 0x6c93e2d1, /// 0xa18
			 0xd0182a9d, /// 0xa1c
			 0xc5f2f5a6, /// 0xa20
			 0x6a1d833c, /// 0xa24
			 0xd90899bf, /// 0xa28
			 0x56e847b0, /// 0xa2c
			 0x2eab26b2, /// 0xa30
			 0x035b3b9a, /// 0xa34
			 0xf0c573fd, /// 0xa38
			 0xa791c62f, /// 0xa3c
			 0xbec9a32f, /// 0xa40
			 0x8bf988b1, /// 0xa44
			 0x301ba0f6, /// 0xa48
			 0x36bb3f84, /// 0xa4c
			 0x59c59465, /// 0xa50
			 0x43b7fbd3, /// 0xa54
			 0xe958b2c4, /// 0xa58
			 0x7174ffa2, /// 0xa5c
			 0x026b75df, /// 0xa60
			 0x7e733845, /// 0xa64
			 0xed82dc0a, /// 0xa68
			 0xc0bf175c, /// 0xa6c
			 0x1c569e3f, /// 0xa70
			 0x80d30b49, /// 0xa74
			 0x1e925d9d, /// 0xa78
			 0xb644f49a, /// 0xa7c
			 0x9dd5e8be, /// 0xa80
			 0x6321553d, /// 0xa84
			 0x0b77c680, /// 0xa88
			 0xbcdf6a9c, /// 0xa8c
			 0x3a688242, /// 0xa90
			 0xc85e8a09, /// 0xa94
			 0xd32bc7f0, /// 0xa98
			 0x2191e85d, /// 0xa9c
			 0x2dc9678d, /// 0xaa0
			 0x194ee8ac, /// 0xaa4
			 0x56678deb, /// 0xaa8
			 0x4f91c23d, /// 0xaac
			 0x7a8515d5, /// 0xab0
			 0x1788ef4d, /// 0xab4
			 0x9a145924, /// 0xab8
			 0xdb443a98, /// 0xabc
			 0x7bbbad50, /// 0xac0
			 0xf0258c3b, /// 0xac4
			 0x49a9e22a, /// 0xac8
			 0x9380fcd5, /// 0xacc
			 0xd2d0da92, /// 0xad0
			 0xc9f6a000, /// 0xad4
			 0xd23cd43c, /// 0xad8
			 0x863b3122, /// 0xadc
			 0xb66fee45, /// 0xae0
			 0xc6e36d6f, /// 0xae4
			 0x97493054, /// 0xae8
			 0x2cfa4523, /// 0xaec
			 0xf3508ce1, /// 0xaf0
			 0x00e5b70b, /// 0xaf4
			 0x75167ccd, /// 0xaf8
			 0xfdc87b5f, /// 0xafc
			 0x2fc369b3, /// 0xb00
			 0x1bfcbab9, /// 0xb04
			 0xd65e4ed8, /// 0xb08
			 0x21a78083, /// 0xb0c
			 0x52981727, /// 0xb10
			 0x73917a3e, /// 0xb14
			 0x6d572954, /// 0xb18
			 0x350b639b, /// 0xb1c
			 0x62177e92, /// 0xb20
			 0xf8ee42ec, /// 0xb24
			 0x6d402bda, /// 0xb28
			 0x7d452cd2, /// 0xb2c
			 0x8c56a62d, /// 0xb30
			 0xf237c3af, /// 0xb34
			 0xce01a7e5, /// 0xb38
			 0x594b682d, /// 0xb3c
			 0x83c61906, /// 0xb40
			 0x9f96e051, /// 0xb44
			 0x94479367, /// 0xb48
			 0x3f8beb78, /// 0xb4c
			 0xb3a50f06, /// 0xb50
			 0xf8048f1d, /// 0xb54
			 0x9bab1831, /// 0xb58
			 0xf3d023f4, /// 0xb5c
			 0x5eb1a3e8, /// 0xb60
			 0x65d517fa, /// 0xb64
			 0x6bf499ad, /// 0xb68
			 0x22e7fccc, /// 0xb6c
			 0xfdbb1919, /// 0xb70
			 0x41d9f605, /// 0xb74
			 0x54d9ed4a, /// 0xb78
			 0x13c82acc, /// 0xb7c
			 0x9d803de0, /// 0xb80
			 0x7ed87fde, /// 0xb84
			 0xa6c56ed8, /// 0xb88
			 0x3779bdaa, /// 0xb8c
			 0x9460842e, /// 0xb90
			 0x5deae806, /// 0xb94
			 0xbe920159, /// 0xb98
			 0x3a15aee0, /// 0xb9c
			 0xf90f875f, /// 0xba0
			 0x629b70d2, /// 0xba4
			 0x6d219f01, /// 0xba8
			 0x08e5ec1f, /// 0xbac
			 0x51956227, /// 0xbb0
			 0xe6dbc2b5, /// 0xbb4
			 0x9cb9c680, /// 0xbb8
			 0x20fb8fb2, /// 0xbbc
			 0xbe082818, /// 0xbc0
			 0x38e5c009, /// 0xbc4
			 0x5c18beb1, /// 0xbc8
			 0x733309f7, /// 0xbcc
			 0x6a5fcdea, /// 0xbd0
			 0x8d8a381e, /// 0xbd4
			 0xf65f0ab2, /// 0xbd8
			 0xd508406a, /// 0xbdc
			 0x2614b3f2, /// 0xbe0
			 0xf4b2c3e9, /// 0xbe4
			 0x037e5bb0, /// 0xbe8
			 0x106cc6c4, /// 0xbec
			 0x5233837b, /// 0xbf0
			 0xc296a8dc, /// 0xbf4
			 0xad016d9b, /// 0xbf8
			 0x58a7577d, /// 0xbfc
			 0xbed6053d, /// 0xc00
			 0x8c0fb6ef, /// 0xc04
			 0x0b0d0ccb, /// 0xc08
			 0xbcdcf210, /// 0xc0c
			 0x454d7dea, /// 0xc10
			 0x80a12e44, /// 0xc14
			 0x89f0010e, /// 0xc18
			 0x30e93190, /// 0xc1c
			 0xf97f7414, /// 0xc20
			 0x3dabd116, /// 0xc24
			 0x7f42bc44, /// 0xc28
			 0x0af96a0a, /// 0xc2c
			 0x7db5d5cd, /// 0xc30
			 0x9492f620, /// 0xc34
			 0x93369cec, /// 0xc38
			 0x95fcd38c, /// 0xc3c
			 0x1057b8a9, /// 0xc40
			 0xb2887631, /// 0xc44
			 0x34d1a48c, /// 0xc48
			 0x7062869d, /// 0xc4c
			 0xe2bab8ca, /// 0xc50
			 0x659cad16, /// 0xc54
			 0x75a4edbf, /// 0xc58
			 0x1cb2a7b3, /// 0xc5c
			 0x58519ace, /// 0xc60
			 0xfe61cbdf, /// 0xc64
			 0x3bd523cd, /// 0xc68
			 0x5df22ba2, /// 0xc6c
			 0x76d0109b, /// 0xc70
			 0xeb089097, /// 0xc74
			 0x3f142039, /// 0xc78
			 0xef39c8ad, /// 0xc7c
			 0x6a61d213, /// 0xc80
			 0xf86196ff, /// 0xc84
			 0x489248e9, /// 0xc88
			 0x1e30d7f2, /// 0xc8c
			 0x5e5f33cc, /// 0xc90
			 0xa0c7ed61, /// 0xc94
			 0x9a0ca3af, /// 0xc98
			 0xe19ad145, /// 0xc9c
			 0x7551529a, /// 0xca0
			 0x186b70da, /// 0xca4
			 0x67c01507, /// 0xca8
			 0xe400f19c, /// 0xcac
			 0xb919ad51, /// 0xcb0
			 0xabb68cbe, /// 0xcb4
			 0x538d7ddd, /// 0xcb8
			 0x70e0d6fa, /// 0xcbc
			 0x21f68b7d, /// 0xcc0
			 0xa7117416, /// 0xcc4
			 0x72241312, /// 0xcc8
			 0x3c0f3149, /// 0xccc
			 0x7c6722e6, /// 0xcd0
			 0x1f264ef7, /// 0xcd4
			 0xbfc930c7, /// 0xcd8
			 0x81f41d9f, /// 0xcdc
			 0xf1a01371, /// 0xce0
			 0xd3f2eacc, /// 0xce4
			 0x3b8e6064, /// 0xce8
			 0xbc60b130, /// 0xcec
			 0x6db8dff5, /// 0xcf0
			 0x9a02eb24, /// 0xcf4
			 0x5a45d79e, /// 0xcf8
			 0x12650dee, /// 0xcfc
			 0x7e58c678, /// 0xd00
			 0x4f7e8a6d, /// 0xd04
			 0xb4ac54cd, /// 0xd08
			 0xe7db20e8, /// 0xd0c
			 0x9f0db822, /// 0xd10
			 0x6d2f734b, /// 0xd14
			 0x8e9d8b71, /// 0xd18
			 0xaedc70d0, /// 0xd1c
			 0xa430776b, /// 0xd20
			 0x726b7fa8, /// 0xd24
			 0xd399203c, /// 0xd28
			 0xb5f01a7e, /// 0xd2c
			 0x9cb11dad, /// 0xd30
			 0xef3809e8, /// 0xd34
			 0x09feae12, /// 0xd38
			 0x16ce167b, /// 0xd3c
			 0xc33802d6, /// 0xd40
			 0xffb62d94, /// 0xd44
			 0x5db0769b, /// 0xd48
			 0xd7ac6147, /// 0xd4c
			 0xcc9bfa78, /// 0xd50
			 0x725cd14d, /// 0xd54
			 0x0523249a, /// 0xd58
			 0x757a1afb, /// 0xd5c
			 0x6d14b951, /// 0xd60
			 0x1de71db2, /// 0xd64
			 0x7e11e765, /// 0xd68
			 0x5f4dee96, /// 0xd6c
			 0x069930f6, /// 0xd70
			 0xe978683c, /// 0xd74
			 0x4698d428, /// 0xd78
			 0x20f27c86, /// 0xd7c
			 0x7625ae86, /// 0xd80
			 0xcd81b4ed, /// 0xd84
			 0x5442258a, /// 0xd88
			 0x41336543, /// 0xd8c
			 0x74ece332, /// 0xd90
			 0xfe657b69, /// 0xd94
			 0x67579b9a, /// 0xd98
			 0xf21575c5, /// 0xd9c
			 0x68013322, /// 0xda0
			 0xbe6517b7, /// 0xda4
			 0x2148961e, /// 0xda8
			 0x59a192f4, /// 0xdac
			 0xebac810e, /// 0xdb0
			 0xf1041252, /// 0xdb4
			 0x78d07879, /// 0xdb8
			 0xb4b81d62, /// 0xdbc
			 0xb2d23935, /// 0xdc0
			 0xbcf5a31d, /// 0xdc4
			 0xa47bd175, /// 0xdc8
			 0x551b721a, /// 0xdcc
			 0x75cb7675, /// 0xdd0
			 0x96226716, /// 0xdd4
			 0x618ee281, /// 0xdd8
			 0xa19490f6, /// 0xddc
			 0xffac5bdd, /// 0xde0
			 0x3be85e36, /// 0xde4
			 0xdc919a50, /// 0xde8
			 0xaea29b9e, /// 0xdec
			 0x36e2b5a6, /// 0xdf0
			 0x75c35ea0, /// 0xdf4
			 0x3e1fd613, /// 0xdf8
			 0x5ed94044, /// 0xdfc
			 0x782ba39c, /// 0xe00
			 0x90b75fc5, /// 0xe04
			 0x1afbf67d, /// 0xe08
			 0x1fd93792, /// 0xe0c
			 0x7614b379, /// 0xe10
			 0x21d417d4, /// 0xe14
			 0x9c9a2089, /// 0xe18
			 0xebc99d1c, /// 0xe1c
			 0x9978dfa7, /// 0xe20
			 0x993e2a21, /// 0xe24
			 0x275b1eb0, /// 0xe28
			 0x7fcd7746, /// 0xe2c
			 0x7ad9bdf4, /// 0xe30
			 0x06889dbb, /// 0xe34
			 0xd7311558, /// 0xe38
			 0xdddc0890, /// 0xe3c
			 0x5bab0b58, /// 0xe40
			 0xd8d23d1d, /// 0xe44
			 0x6ca2ca2a, /// 0xe48
			 0x4bc908c2, /// 0xe4c
			 0x9da9698a, /// 0xe50
			 0xc5b97892, /// 0xe54
			 0xe588d82d, /// 0xe58
			 0x3e1af59b, /// 0xe5c
			 0x2a8675a5, /// 0xe60
			 0xbcdc219d, /// 0xe64
			 0x2df7a38c, /// 0xe68
			 0x25d0356c, /// 0xe6c
			 0x31f32cef, /// 0xe70
			 0x537612af, /// 0xe74
			 0xf7ab4b01, /// 0xe78
			 0x29af4f8e, /// 0xe7c
			 0xc4598795, /// 0xe80
			 0x94cf1c3e, /// 0xe84
			 0xef50fa75, /// 0xe88
			 0xc9b80c62, /// 0xe8c
			 0x860f2e2c, /// 0xe90
			 0x1ba6a547, /// 0xe94
			 0x9f304442, /// 0xe98
			 0xeb07fea2, /// 0xe9c
			 0x93173b91, /// 0xea0
			 0xb02cc2cb, /// 0xea4
			 0x76bcb0ef, /// 0xea8
			 0x77d01c16, /// 0xeac
			 0x6cb147c6, /// 0xeb0
			 0x1cbe27aa, /// 0xeb4
			 0x1bbeb66c, /// 0xeb8
			 0xff745497, /// 0xebc
			 0xe5c8fecc, /// 0xec0
			 0xad0bcde7, /// 0xec4
			 0x1d239c6b, /// 0xec8
			 0x2a3cf1a6, /// 0xecc
			 0x080fd126, /// 0xed0
			 0xdd7314a7, /// 0xed4
			 0x252fb07d, /// 0xed8
			 0x2e88d1e2, /// 0xedc
			 0x63f79353, /// 0xee0
			 0x2d4bda5b, /// 0xee4
			 0x77e6f326, /// 0xee8
			 0x085bedf3, /// 0xeec
			 0x01f38994, /// 0xef0
			 0xcb2aeb8d, /// 0xef4
			 0x12ae7081, /// 0xef8
			 0x5da4bccb, /// 0xefc
			 0x3a204eaf, /// 0xf00
			 0x50f72855, /// 0xf04
			 0x1f92d2f3, /// 0xf08
			 0x003f0ba2, /// 0xf0c
			 0x11b8365c, /// 0xf10
			 0x43d1a50c, /// 0xf14
			 0xddf72a18, /// 0xf18
			 0x27d2ebd4, /// 0xf1c
			 0xbef37f0c, /// 0xf20
			 0x94f33b6c, /// 0xf24
			 0x4097f35f, /// 0xf28
			 0x57ac38d4, /// 0xf2c
			 0x9cd64a6c, /// 0xf30
			 0x4439348e, /// 0xf34
			 0xadec2f81, /// 0xf38
			 0x56fcbbb5, /// 0xf3c
			 0x560e57a3, /// 0xf40
			 0xe5d1c578, /// 0xf44
			 0x73f553fa, /// 0xf48
			 0xb67e99ac, /// 0xf4c
			 0x84e2db0a, /// 0xf50
			 0x0bd2255d, /// 0xf54
			 0x18a6e526, /// 0xf58
			 0x99a4a2df, /// 0xf5c
			 0x5ffd441d, /// 0xf60
			 0x2c4ffb00, /// 0xf64
			 0xe45ba3f6, /// 0xf68
			 0x8bd49071, /// 0xf6c
			 0xb95aa1c9, /// 0xf70
			 0x978df6a5, /// 0xf74
			 0x5179f8a7, /// 0xf78
			 0xea36a28c, /// 0xf7c
			 0x603b1aa2, /// 0xf80
			 0xe9d1a491, /// 0xf84
			 0x2b5f4f28, /// 0xf88
			 0x78481bb1, /// 0xf8c
			 0x36e93999, /// 0xf90
			 0x65e01658, /// 0xf94
			 0xd00a5da6, /// 0xf98
			 0xe44a5c8f, /// 0xf9c
			 0xa1a01433, /// 0xfa0
			 0x56f9a5a5, /// 0xfa4
			 0x66b858d7, /// 0xfa8
			 0x96dd41b6, /// 0xfac
			 0x78564ea9, /// 0xfb0
			 0x5d544d89, /// 0xfb4
			 0xd889fc62, /// 0xfb8
			 0xfe1723e7, /// 0xfbc
			 0x8775140b, /// 0xfc0
			 0xa82bd482, /// 0xfc4
			 0x21669b1c, /// 0xfc8
			 0x39004c5d, /// 0xfcc
			 0xe78ca36f, /// 0xfd0
			 0x8a6836b9, /// 0xfd4
			 0x9bebdad8, /// 0xfd8
			 0x0d435c97, /// 0xfdc
			 0xe8d2f268, /// 0xfe0
			 0x849c39de, /// 0xfe4
			 0x23fb3ca6, /// 0xfe8
			 0xb92bc194, /// 0xfec
			 0xbe0bf25f, /// 0xff0
			 0xf8819e52, /// 0xff4
			 0xbb8712f5, /// 0xff8
			 0x32b96b34 /// last
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
			 0x00000604,
			 0x00000368,
			 0x000004a0,
			 0x000001e0,
			 0x00000030,
			 0x000004d4,
			 0x000004b4,
			 0x00000538,

			 /// vpu register f2
			 0x41600000,
			 0x41d00000,
			 0x41980000,
			 0x41400000,
			 0x41200000,
			 0x3f800000,
			 0x40e00000,
			 0x41e80000,

			 /// vpu register f3
			 0x41800000,
			 0x40a00000,
			 0x40a00000,
			 0x41700000,
			 0x41500000,
			 0x40c00000,
			 0x41c00000,
			 0x41600000,

			 /// vpu register f4
			 0x41800000,
			 0x41b00000,
			 0x41e80000,
			 0x41100000,
			 0x40800000,
			 0x41f00000,
			 0x41d00000,
			 0x41e80000,

			 /// vpu register f5
			 0x40a00000,
			 0x3f800000,
			 0x41980000,
			 0x41800000,
			 0x41b80000,
			 0x41e00000,
			 0x41b00000,
			 0x41b00000,

			 /// vpu register f6
			 0x41500000,
			 0x41400000,
			 0x41600000,
			 0x41a80000,
			 0x40400000,
			 0x41980000,
			 0x41c80000,
			 0x41000000,

			 /// vpu register f7
			 0x41a80000,
			 0x41000000,
			 0x42000000,
			 0x41000000,
			 0x41d00000,
			 0x40000000,
			 0x41c80000,
			 0x41e00000,

			 /// vpu register f8
			 0x41f00000,
			 0x41880000,
			 0x41900000,
			 0x41900000,
			 0x41000000,
			 0x41a00000,
			 0x41b00000,
			 0x41a80000,

			 /// vpu register f9
			 0x41800000,
			 0x41980000,
			 0x40000000,
			 0x41b80000,
			 0x40a00000,
			 0x41100000,
			 0x40c00000,
			 0x41600000,

			 /// vpu register f10
			 0x41000000,
			 0x41200000,
			 0x40e00000,
			 0x41b00000,
			 0x3f800000,
			 0x40400000,
			 0x41e00000,
			 0x41b80000,

			 /// vpu register f11
			 0x41880000,
			 0x41f80000,
			 0x41e80000,
			 0x41700000,
			 0x40a00000,
			 0x41300000,
			 0x3f800000,
			 0x41300000,

			 /// vpu register f12
			 0x40400000,
			 0x41c80000,
			 0x41c80000,
			 0x42000000,
			 0x41400000,
			 0x41b00000,
			 0x41800000,
			 0x40800000,

			 /// vpu register f13
			 0x41c00000,
			 0x41880000,
			 0x41980000,
			 0x40000000,
			 0x41980000,
			 0x41800000,
			 0x41600000,
			 0x41f80000,

			 /// vpu register f14
			 0x41200000,
			 0x41e80000,
			 0x41100000,
			 0x40c00000,
			 0x40800000,
			 0x41800000,
			 0x41880000,
			 0x41e00000,

			 /// vpu register f15
			 0x42000000,
			 0x41c80000,
			 0x40a00000,
			 0x41f80000,
			 0x41f80000,
			 0x41e80000,
			 0x41500000,
			 0x41900000,

			 /// vpu register f16
			 0x41b80000,
			 0x41e80000,
			 0x41880000,
			 0x41a00000,
			 0x41000000,
			 0x41a80000,
			 0x41f00000,
			 0x41a00000,

			 /// vpu register f17
			 0x41f80000,
			 0x41600000,
			 0x41b80000,
			 0x3f800000,
			 0x41b00000,
			 0x41000000,
			 0x41f80000,
			 0x41b00000,

			 /// vpu register f18
			 0x40c00000,
			 0x41a80000,
			 0x41880000,
			 0x40000000,
			 0x40c00000,
			 0x41d00000,
			 0x41800000,
			 0x41500000,

			 /// vpu register f19
			 0x40800000,
			 0x41f00000,
			 0x41c80000,
			 0x41b00000,
			 0x41500000,
			 0x41e00000,
			 0x40800000,
			 0x41f00000,

			 /// vpu register f20
			 0x40e00000,
			 0x40400000,
			 0x40c00000,
			 0x41880000,
			 0x41f00000,
			 0x41880000,
			 0x41980000,
			 0x41900000,

			 /// vpu register f21
			 0x41f00000,
			 0x40c00000,
			 0x41d80000,
			 0x41a00000,
			 0x41e80000,
			 0x41e00000,
			 0x40800000,
			 0x40c00000,

			 /// vpu register f22
			 0x41300000,
			 0x40800000,
			 0x3f800000,
			 0x40400000,
			 0x41000000,
			 0x42000000,
			 0x41100000,
			 0x41e00000,

			 /// vpu register f23
			 0x41b80000,
			 0x41880000,
			 0x40c00000,
			 0x40000000,
			 0x41c80000,
			 0x41e80000,
			 0x41d80000,
			 0x41800000,

			 /// vpu register f24
			 0x41600000,
			 0x41500000,
			 0x41880000,
			 0x40400000,
			 0x40e00000,
			 0x41b00000,
			 0x41d00000,
			 0x41a00000,

			 /// vpu register f25
			 0x41200000,
			 0x41100000,
			 0x41f00000,
			 0x41f80000,
			 0x41e80000,
			 0x40e00000,
			 0x41900000,
			 0x41b80000,

			 /// vpu register f26
			 0x41b80000,
			 0x41e80000,
			 0x40c00000,
			 0x40c00000,
			 0x41b00000,
			 0x41b00000,
			 0x40000000,
			 0x41900000,

			 /// vpu register f27
			 0x41000000,
			 0x40e00000,
			 0x41e00000,
			 0x41b00000,
			 0x3f800000,
			 0x40800000,
			 0x41d00000,
			 0x41980000,

			 /// vpu register f28
			 0x40400000,
			 0x40c00000,
			 0x41b00000,
			 0x41b00000,
			 0x41200000,
			 0x41700000,
			 0x41a80000,
			 0x41000000,

			 /// vpu register f29
			 0x41600000,
			 0x40400000,
			 0x3f800000,
			 0x41a80000,
			 0x41100000,
			 0x40c00000,
			 0x41a80000,
			 0x41400000,

			 /// vpu register f30
			 0x40a00000,
			 0x41f00000,
			 0x40000000,
			 0x42000000,
			 0x41800000,
			 0x40e00000,
			 0x40e00000,
			 0x41500000,

			 /// vpu register f31
			 0x40400000,
			 0x41300000,
			 0x41e00000,
			 0x41d80000,
			 0x40400000,
			 0x40e00000,
			 0x41800000,
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
		"fmv.x.s x3, f0\n"                                    ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f10\n"                                   ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f10\n"                                   ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f27\n"                                   ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f27\n"                                   ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f26\n"                                   ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f25\n"                                   ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f12\n"                                   ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f0\n"                                    ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f26\n"                                   ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f8\n"                                    ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f4\n"                                    ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f5\n"                                    ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f19\n"                                   ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f11\n"                                   ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f10\n"                                   ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f7\n"                                    ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f23\n"                                   ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f3\n"                                    ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f5\n"                                    ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f19\n"                                   ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f5\n"                                    ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f16\n"                                   ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f15\n"                                   ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f6\n"                                    ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f16\n"                                   ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f22\n"                                   ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f2\n"                                    ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f15\n"                                   ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f28\n"                                   ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f23\n"                                   ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f2\n"                                    ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f6\n"                                    ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f13\n"                                   ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f18\n"                                   ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f5\n"                                    ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f20\n"                                   ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f2\n"                                    ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f28\n"                                   ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f26\n"                                   ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f3\n"                                    ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f25\n"                                   ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f1\n"                                    ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f15\n"                                   ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f28\n"                                   ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f12\n"                                   ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f13\n"                                   ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f14\n"                                   ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f3\n"                                    ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f14\n"                                   ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f16\n"                                   ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f24\n"                                   ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f26\n"                                   ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f24\n"                                   ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f13\n"                                   ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f4\n"                                    ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f28\n"                                   ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f18\n"                                   ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f16\n"                                   ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f7\n"                                    ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f2\n"                                    ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f24\n"                                   ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f24\n"                                   ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f7\n"                                    ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f9\n"                                    ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f1\n"                                    ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f8\n"                                    ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f29\n"                                   ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f5\n"                                    ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f5\n"                                    ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f27\n"                                   ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f0\n"                                    ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f18\n"                                   ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f14\n"                                   ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f28\n"                                   ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f1\n"                                    ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f8\n"                                    ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f6\n"                                    ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f12\n"                                   ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f6\n"                                    ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f3\n"                                    ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f3\n"                                    ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f9\n"                                    ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f4\n"                                    ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f21\n"                                   ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f12\n"                                   ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f1\n"                                    ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f0\n"                                    ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x7, f4\n"                                    ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f21\n"                                   ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f11\n"                                   ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f28\n"                                   ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x9, f15\n"                                   ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x4, f11\n"                                   ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f13\n"                                   ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f8\n"                                    ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x5, f10\n"                                   ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x6, f28\n"                                   ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.x.s x3, f3\n"                                    ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
