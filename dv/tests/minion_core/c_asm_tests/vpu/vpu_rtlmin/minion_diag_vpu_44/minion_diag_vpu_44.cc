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
			 0x6eed297d, /// 0x0
			 0xbcba586e, /// 0x4
			 0x415cc5ff, /// 0x8
			 0xfa0ee4ad, /// 0xc
			 0x1bf18e38, /// 0x10
			 0x15b52912, /// 0x14
			 0x25d311c4, /// 0x18
			 0x94513c0d, /// 0x1c
			 0xe179fe5c, /// 0x20
			 0x7b626088, /// 0x24
			 0xb5f24168, /// 0x28
			 0x6bd563ca, /// 0x2c
			 0xe35c6621, /// 0x30
			 0x59e26c31, /// 0x34
			 0x65d09105, /// 0x38
			 0x0a3d601c, /// 0x3c
			 0xe358cb03, /// 0x40
			 0xb13de0d9, /// 0x44
			 0x5f4c9254, /// 0x48
			 0x5cd416eb, /// 0x4c
			 0x9863f888, /// 0x50
			 0xcaa6bcdc, /// 0x54
			 0x88494e8b, /// 0x58
			 0xd7a859dd, /// 0x5c
			 0xce9cb24e, /// 0x60
			 0xf4c9291d, /// 0x64
			 0x8fcd3f4e, /// 0x68
			 0xe1ca61db, /// 0x6c
			 0xb9b4447f, /// 0x70
			 0xc1711e98, /// 0x74
			 0x5a754c59, /// 0x78
			 0xeb179cff, /// 0x7c
			 0x498e037b, /// 0x80
			 0x7b5012c7, /// 0x84
			 0x95918e14, /// 0x88
			 0x37391d03, /// 0x8c
			 0x21521afe, /// 0x90
			 0x65cc67f9, /// 0x94
			 0xc45ae309, /// 0x98
			 0xe890e71e, /// 0x9c
			 0xb250e8ea, /// 0xa0
			 0x2d7b3765, /// 0xa4
			 0x0c607769, /// 0xa8
			 0x76bd8812, /// 0xac
			 0x86bbe60c, /// 0xb0
			 0x460ac076, /// 0xb4
			 0x1c69eb53, /// 0xb8
			 0x9e7dce5d, /// 0xbc
			 0x31ba93ff, /// 0xc0
			 0x490d833c, /// 0xc4
			 0xdea029b5, /// 0xc8
			 0x3c52564a, /// 0xcc
			 0x4ca9cb1f, /// 0xd0
			 0x056a7bd4, /// 0xd4
			 0x897578d3, /// 0xd8
			 0xe229c2ec, /// 0xdc
			 0xf8190717, /// 0xe0
			 0x7d7361b8, /// 0xe4
			 0x4b655aa7, /// 0xe8
			 0x8fbedc74, /// 0xec
			 0x6bc22eaf, /// 0xf0
			 0x44285d55, /// 0xf4
			 0xca2ac428, /// 0xf8
			 0x95f800dc, /// 0xfc
			 0x32fec11e, /// 0x100
			 0xf631563b, /// 0x104
			 0x97d7f7ba, /// 0x108
			 0x2d9ae41d, /// 0x10c
			 0x866b5a18, /// 0x110
			 0x2284e4c3, /// 0x114
			 0xed576aef, /// 0x118
			 0x3b73d304, /// 0x11c
			 0xa77447aa, /// 0x120
			 0x853394f6, /// 0x124
			 0x423bb5b6, /// 0x128
			 0xf27595e7, /// 0x12c
			 0x5144ea94, /// 0x130
			 0x54d26a68, /// 0x134
			 0x7d003e19, /// 0x138
			 0x73b1d8de, /// 0x13c
			 0xca0918c6, /// 0x140
			 0xe84e1077, /// 0x144
			 0x7edea2ef, /// 0x148
			 0x8d533868, /// 0x14c
			 0xd7a87c55, /// 0x150
			 0xc3de706a, /// 0x154
			 0xf7a602ed, /// 0x158
			 0x5ed82b03, /// 0x15c
			 0x3150a6f0, /// 0x160
			 0xaa4491a1, /// 0x164
			 0x8b19ae3a, /// 0x168
			 0x03b3f6c3, /// 0x16c
			 0xa144c369, /// 0x170
			 0x21fbeef3, /// 0x174
			 0xc6feb14d, /// 0x178
			 0x21dfd047, /// 0x17c
			 0x81d750c3, /// 0x180
			 0xed7d862d, /// 0x184
			 0x7a59e123, /// 0x188
			 0x110aa18a, /// 0x18c
			 0xc3d6bb27, /// 0x190
			 0x4770ebdf, /// 0x194
			 0xe11cfca2, /// 0x198
			 0x0477e900, /// 0x19c
			 0xb4e14a1e, /// 0x1a0
			 0xce56bd9e, /// 0x1a4
			 0xe2ee2d94, /// 0x1a8
			 0xdeb3a19f, /// 0x1ac
			 0x62633271, /// 0x1b0
			 0x233a13fc, /// 0x1b4
			 0x8283cd15, /// 0x1b8
			 0xb475c35d, /// 0x1bc
			 0xc7fec1c1, /// 0x1c0
			 0x0e0f44c3, /// 0x1c4
			 0x58a9d438, /// 0x1c8
			 0x40e79d5f, /// 0x1cc
			 0xa089cabc, /// 0x1d0
			 0x4cf1b6c9, /// 0x1d4
			 0x1a9a8f67, /// 0x1d8
			 0x4475dc1b, /// 0x1dc
			 0x954861fc, /// 0x1e0
			 0xf7868dd6, /// 0x1e4
			 0xb4bd8175, /// 0x1e8
			 0x5ad3daae, /// 0x1ec
			 0x207bc1c4, /// 0x1f0
			 0x28ec9cc3, /// 0x1f4
			 0x4cc7384d, /// 0x1f8
			 0x302e0401, /// 0x1fc
			 0xb526caa6, /// 0x200
			 0x5f773c73, /// 0x204
			 0xf8d12499, /// 0x208
			 0x97efd946, /// 0x20c
			 0xccf72382, /// 0x210
			 0x5cf9e11b, /// 0x214
			 0x216e5772, /// 0x218
			 0x335e12ce, /// 0x21c
			 0x31ded0da, /// 0x220
			 0x7f0e8fc0, /// 0x224
			 0xac888390, /// 0x228
			 0x49d60774, /// 0x22c
			 0x1f1ea277, /// 0x230
			 0xc396d8e8, /// 0x234
			 0x85276f6f, /// 0x238
			 0x4a9c2324, /// 0x23c
			 0xfbd3830e, /// 0x240
			 0x5faa08c1, /// 0x244
			 0xaa05e612, /// 0x248
			 0xa118bc82, /// 0x24c
			 0xe93451ab, /// 0x250
			 0x3210942a, /// 0x254
			 0x83f0cea6, /// 0x258
			 0xa6242533, /// 0x25c
			 0x7e6510a9, /// 0x260
			 0x93ab4649, /// 0x264
			 0xe5013323, /// 0x268
			 0x49f1a9e9, /// 0x26c
			 0x7a04aac1, /// 0x270
			 0xcc4e2c9b, /// 0x274
			 0x0871b554, /// 0x278
			 0x1d374310, /// 0x27c
			 0xc0df507a, /// 0x280
			 0x5e0c325e, /// 0x284
			 0x55b81ffc, /// 0x288
			 0x3be78443, /// 0x28c
			 0x201054cd, /// 0x290
			 0x7b27fe9d, /// 0x294
			 0x91a7d0ef, /// 0x298
			 0xf0a72ab8, /// 0x29c
			 0xcb25c94d, /// 0x2a0
			 0x2cd8ce98, /// 0x2a4
			 0xe614b60c, /// 0x2a8
			 0x09a19b2b, /// 0x2ac
			 0x80b6e0b3, /// 0x2b0
			 0x2beefe99, /// 0x2b4
			 0x8ec75fd7, /// 0x2b8
			 0xcb025b94, /// 0x2bc
			 0x0ca7ca80, /// 0x2c0
			 0xa7455747, /// 0x2c4
			 0x0499cc95, /// 0x2c8
			 0xac53050f, /// 0x2cc
			 0x714ebc3b, /// 0x2d0
			 0x874a65db, /// 0x2d4
			 0xf07fb681, /// 0x2d8
			 0x553d5a61, /// 0x2dc
			 0x2e9f72eb, /// 0x2e0
			 0x1efc45ea, /// 0x2e4
			 0x6fa2fe5f, /// 0x2e8
			 0xd88c2ad0, /// 0x2ec
			 0x4bc70fc5, /// 0x2f0
			 0x02fe4e27, /// 0x2f4
			 0x2215e62c, /// 0x2f8
			 0xa84780ed, /// 0x2fc
			 0xe7ef0ffc, /// 0x300
			 0x8ec641f6, /// 0x304
			 0x0ed149ed, /// 0x308
			 0xc9f1c632, /// 0x30c
			 0x6928b9f6, /// 0x310
			 0x72ad462e, /// 0x314
			 0x52cc8b56, /// 0x318
			 0x8c13a898, /// 0x31c
			 0x0b890449, /// 0x320
			 0xd4e9da5f, /// 0x324
			 0xcd09b74f, /// 0x328
			 0x2339eb28, /// 0x32c
			 0x45ced0a9, /// 0x330
			 0x4b32dea3, /// 0x334
			 0x65eba021, /// 0x338
			 0xa4abf16a, /// 0x33c
			 0xed8998e2, /// 0x340
			 0x3b2220f1, /// 0x344
			 0x610fb41b, /// 0x348
			 0x956c5036, /// 0x34c
			 0x31d1d01c, /// 0x350
			 0xd1b030c2, /// 0x354
			 0x637d0a7f, /// 0x358
			 0x6ed8cc8f, /// 0x35c
			 0x5187f384, /// 0x360
			 0x4435db3d, /// 0x364
			 0xf39fc2ff, /// 0x368
			 0x41da0985, /// 0x36c
			 0x8fffca17, /// 0x370
			 0x2072e6bf, /// 0x374
			 0xab414f80, /// 0x378
			 0x71db5dc3, /// 0x37c
			 0x8d1c5642, /// 0x380
			 0xab476d2e, /// 0x384
			 0x3b1bcd2d, /// 0x388
			 0x4ab20a82, /// 0x38c
			 0x8c0baff0, /// 0x390
			 0xc6d719d5, /// 0x394
			 0xdc44a13b, /// 0x398
			 0xf305e506, /// 0x39c
			 0xf1a712bb, /// 0x3a0
			 0x5f95d538, /// 0x3a4
			 0x55a529eb, /// 0x3a8
			 0xe81c7de5, /// 0x3ac
			 0x7c88673a, /// 0x3b0
			 0xe63e11da, /// 0x3b4
			 0x6c6b962b, /// 0x3b8
			 0x69924f73, /// 0x3bc
			 0x476ed744, /// 0x3c0
			 0xf49ac1e8, /// 0x3c4
			 0x4b9294cd, /// 0x3c8
			 0xe41c3b5b, /// 0x3cc
			 0x1b744da6, /// 0x3d0
			 0xa1f2c5b2, /// 0x3d4
			 0x5425ef50, /// 0x3d8
			 0x49358702, /// 0x3dc
			 0xdf563075, /// 0x3e0
			 0x29f30f19, /// 0x3e4
			 0x15fe0271, /// 0x3e8
			 0xdac6eacf, /// 0x3ec
			 0x4a1f765d, /// 0x3f0
			 0x49cc5792, /// 0x3f4
			 0xddef6f1f, /// 0x3f8
			 0xcd645cab, /// 0x3fc
			 0x671ddaa8, /// 0x400
			 0xbb2c7d85, /// 0x404
			 0xd756263b, /// 0x408
			 0xb077763c, /// 0x40c
			 0x611bdce5, /// 0x410
			 0x9f4c7cc1, /// 0x414
			 0x45155929, /// 0x418
			 0xf0f4202a, /// 0x41c
			 0xb6946008, /// 0x420
			 0x98027431, /// 0x424
			 0xea888364, /// 0x428
			 0xca2568a9, /// 0x42c
			 0xf263f693, /// 0x430
			 0x19d66711, /// 0x434
			 0x6ac9894b, /// 0x438
			 0x1ac5ced7, /// 0x43c
			 0x7e7079bb, /// 0x440
			 0x54580fda, /// 0x444
			 0x28d9e490, /// 0x448
			 0xf2c48a2b, /// 0x44c
			 0x2d1b492d, /// 0x450
			 0xacc75e8c, /// 0x454
			 0xa6e58de5, /// 0x458
			 0xe2c59bed, /// 0x45c
			 0xb3163a1d, /// 0x460
			 0x26d3fe52, /// 0x464
			 0x1b39d133, /// 0x468
			 0x92245b85, /// 0x46c
			 0x6941f42d, /// 0x470
			 0x60246c2e, /// 0x474
			 0x81d5bef6, /// 0x478
			 0xccf3f833, /// 0x47c
			 0xf692b698, /// 0x480
			 0xa7ebbd2c, /// 0x484
			 0x22a0789c, /// 0x488
			 0xe1046002, /// 0x48c
			 0xad02d241, /// 0x490
			 0x038c9a4b, /// 0x494
			 0xc4676c93, /// 0x498
			 0xb5af1448, /// 0x49c
			 0x53131557, /// 0x4a0
			 0x5a9d08cd, /// 0x4a4
			 0x4678cc06, /// 0x4a8
			 0xfe3f7703, /// 0x4ac
			 0x4a0ffa2c, /// 0x4b0
			 0xd10706fa, /// 0x4b4
			 0x51f4d3af, /// 0x4b8
			 0xbc401d94, /// 0x4bc
			 0x0946fda0, /// 0x4c0
			 0xeb6ebdf6, /// 0x4c4
			 0xcfb96adb, /// 0x4c8
			 0xcd7ad364, /// 0x4cc
			 0x7ef6d9d8, /// 0x4d0
			 0x2aa42724, /// 0x4d4
			 0xeaefb8ed, /// 0x4d8
			 0xa63a50a5, /// 0x4dc
			 0x2b172fca, /// 0x4e0
			 0x00b345d1, /// 0x4e4
			 0xf0d73d8d, /// 0x4e8
			 0x5e219329, /// 0x4ec
			 0x896ec679, /// 0x4f0
			 0xc1e7bfaa, /// 0x4f4
			 0xde5e5ee7, /// 0x4f8
			 0x22ba3728, /// 0x4fc
			 0x4ba30264, /// 0x500
			 0xa7f406d5, /// 0x504
			 0x89e2aacd, /// 0x508
			 0x9ed80e63, /// 0x50c
			 0xaaef5b3a, /// 0x510
			 0x478c5e09, /// 0x514
			 0x5c5e9e9e, /// 0x518
			 0xe45bbb5f, /// 0x51c
			 0x8a43decc, /// 0x520
			 0xe268d20c, /// 0x524
			 0x7bd7ca93, /// 0x528
			 0xd5bd9604, /// 0x52c
			 0x5c84c65f, /// 0x530
			 0x54e8b02d, /// 0x534
			 0xeff7e63f, /// 0x538
			 0x32986aff, /// 0x53c
			 0xae750e7a, /// 0x540
			 0xe698590b, /// 0x544
			 0x91ae734d, /// 0x548
			 0x9113a0c9, /// 0x54c
			 0xc7d9ab38, /// 0x550
			 0x0f7a09ca, /// 0x554
			 0x1801ad15, /// 0x558
			 0x07742e87, /// 0x55c
			 0x62e26743, /// 0x560
			 0xe380b06d, /// 0x564
			 0x9b8b53b6, /// 0x568
			 0xa8239368, /// 0x56c
			 0x46b3b1c9, /// 0x570
			 0x8b0e0ed8, /// 0x574
			 0x7e6cb837, /// 0x578
			 0x4627a11b, /// 0x57c
			 0x60e201db, /// 0x580
			 0x7ab06aab, /// 0x584
			 0x767e4f5f, /// 0x588
			 0x25bc8e15, /// 0x58c
			 0xeb7f2f2c, /// 0x590
			 0x4b7250f4, /// 0x594
			 0x32a89b8c, /// 0x598
			 0xd00e179d, /// 0x59c
			 0xdbfbbcb3, /// 0x5a0
			 0x9b396d30, /// 0x5a4
			 0x85e0c95d, /// 0x5a8
			 0x55732a6b, /// 0x5ac
			 0x61c3c2b8, /// 0x5b0
			 0x395c20ec, /// 0x5b4
			 0x2dbb5f1e, /// 0x5b8
			 0x9e9b23d1, /// 0x5bc
			 0xc58f1e7c, /// 0x5c0
			 0x33d64aa2, /// 0x5c4
			 0xc9a213b8, /// 0x5c8
			 0xa7020d90, /// 0x5cc
			 0xc6361803, /// 0x5d0
			 0xc1cda0e4, /// 0x5d4
			 0x1bab964b, /// 0x5d8
			 0xe3e4dd19, /// 0x5dc
			 0x36c1b91f, /// 0x5e0
			 0x04219d19, /// 0x5e4
			 0x7df32458, /// 0x5e8
			 0xa0c459f9, /// 0x5ec
			 0x7df1c20d, /// 0x5f0
			 0x770da825, /// 0x5f4
			 0xc422ec17, /// 0x5f8
			 0x2394052e, /// 0x5fc
			 0xfe20700f, /// 0x600
			 0xd99297fc, /// 0x604
			 0x2cc045fd, /// 0x608
			 0x21190068, /// 0x60c
			 0x63559d0b, /// 0x610
			 0x09f50a2e, /// 0x614
			 0x798a2b3a, /// 0x618
			 0xe10edf95, /// 0x61c
			 0x040a81c3, /// 0x620
			 0xd9971654, /// 0x624
			 0x60a3fad9, /// 0x628
			 0x1e9d9e85, /// 0x62c
			 0xdc175fb3, /// 0x630
			 0xfdcc215c, /// 0x634
			 0x91b9dfe6, /// 0x638
			 0x4ac5632c, /// 0x63c
			 0x56db61a0, /// 0x640
			 0x8ed13f8b, /// 0x644
			 0xd072ad9c, /// 0x648
			 0xa2cca2e7, /// 0x64c
			 0xc8632261, /// 0x650
			 0x63ff0e58, /// 0x654
			 0xaac75943, /// 0x658
			 0x9065a434, /// 0x65c
			 0x26ad4e53, /// 0x660
			 0x519ae641, /// 0x664
			 0x94603d28, /// 0x668
			 0x4fc96199, /// 0x66c
			 0xa42c4f5f, /// 0x670
			 0xbe7ff287, /// 0x674
			 0x6406852b, /// 0x678
			 0xc0cf2f34, /// 0x67c
			 0x037d6187, /// 0x680
			 0xc7fdb194, /// 0x684
			 0x4a9bf78b, /// 0x688
			 0xe4e669f4, /// 0x68c
			 0x1c81b39a, /// 0x690
			 0x6f76f3a8, /// 0x694
			 0x064795bc, /// 0x698
			 0x5ac32ed1, /// 0x69c
			 0xddd8c44d, /// 0x6a0
			 0x0e3fa089, /// 0x6a4
			 0x56f0450d, /// 0x6a8
			 0x54457273, /// 0x6ac
			 0x3360b5f9, /// 0x6b0
			 0x627e78b4, /// 0x6b4
			 0x6f1927ae, /// 0x6b8
			 0xd9b5bffd, /// 0x6bc
			 0x43b1488a, /// 0x6c0
			 0x783c6a84, /// 0x6c4
			 0x21cf1c72, /// 0x6c8
			 0x655b1acd, /// 0x6cc
			 0xd921952b, /// 0x6d0
			 0x009e6158, /// 0x6d4
			 0xefe477bd, /// 0x6d8
			 0x7699ec7e, /// 0x6dc
			 0xf42ffb48, /// 0x6e0
			 0x96d3e1a9, /// 0x6e4
			 0xe8634f86, /// 0x6e8
			 0x8b8f06c8, /// 0x6ec
			 0xcae3b0f8, /// 0x6f0
			 0xcf229b92, /// 0x6f4
			 0xb3f898ce, /// 0x6f8
			 0x3c78f96a, /// 0x6fc
			 0xfd6ace18, /// 0x700
			 0x0dafb44f, /// 0x704
			 0xb327d467, /// 0x708
			 0xff21c61a, /// 0x70c
			 0xb54faed2, /// 0x710
			 0x1c11ee71, /// 0x714
			 0x92c3a847, /// 0x718
			 0x238d314a, /// 0x71c
			 0x88fe8b09, /// 0x720
			 0xc48c763b, /// 0x724
			 0xdc9ff753, /// 0x728
			 0x2cd62bc8, /// 0x72c
			 0x9644c6b9, /// 0x730
			 0x49a5d428, /// 0x734
			 0x50749274, /// 0x738
			 0x6f06ade6, /// 0x73c
			 0xb6cd8dbc, /// 0x740
			 0x4d646a8e, /// 0x744
			 0x0f7d1882, /// 0x748
			 0x43ae7810, /// 0x74c
			 0x0b38aaeb, /// 0x750
			 0x52f3f294, /// 0x754
			 0x89f33892, /// 0x758
			 0x0754ae1a, /// 0x75c
			 0xd98cd26e, /// 0x760
			 0xe59f7ec2, /// 0x764
			 0x9d29896c, /// 0x768
			 0x84f0265a, /// 0x76c
			 0x598173c7, /// 0x770
			 0x1d10bb97, /// 0x774
			 0x95ba0811, /// 0x778
			 0xda5bf035, /// 0x77c
			 0x20840bd2, /// 0x780
			 0x8b500a5c, /// 0x784
			 0x7b3c3290, /// 0x788
			 0x61764645, /// 0x78c
			 0x62b555a5, /// 0x790
			 0x8de0629d, /// 0x794
			 0x0ecb3cd6, /// 0x798
			 0x2cdd06ca, /// 0x79c
			 0x26e43aaf, /// 0x7a0
			 0xa765ff92, /// 0x7a4
			 0x616e41a4, /// 0x7a8
			 0xac6c4557, /// 0x7ac
			 0xae22082e, /// 0x7b0
			 0x9ea24087, /// 0x7b4
			 0x3d272e54, /// 0x7b8
			 0x1cd413b3, /// 0x7bc
			 0x50c9c91b, /// 0x7c0
			 0xf34e6dc7, /// 0x7c4
			 0xd6de69c8, /// 0x7c8
			 0x0d4c2bb5, /// 0x7cc
			 0xcb64b855, /// 0x7d0
			 0x2789ee06, /// 0x7d4
			 0x2118a80d, /// 0x7d8
			 0x90ad5527, /// 0x7dc
			 0xdbe21248, /// 0x7e0
			 0x55d51226, /// 0x7e4
			 0x41a3ab0c, /// 0x7e8
			 0xc073452e, /// 0x7ec
			 0xa19e1d7c, /// 0x7f0
			 0xb79f417e, /// 0x7f4
			 0x5d8f4120, /// 0x7f8
			 0xfdd139c2, /// 0x7fc
			 0x24b57ac0, /// 0x800
			 0xc9be9dc9, /// 0x804
			 0x66f74fc4, /// 0x808
			 0x71e21b62, /// 0x80c
			 0x7a01a095, /// 0x810
			 0x51f45c23, /// 0x814
			 0x5a3f4b7a, /// 0x818
			 0x38d7474f, /// 0x81c
			 0xc8b21b67, /// 0x820
			 0x39a6b9f3, /// 0x824
			 0xc250a600, /// 0x828
			 0x492e2561, /// 0x82c
			 0x4abf1774, /// 0x830
			 0xae18564e, /// 0x834
			 0xdd748ff2, /// 0x838
			 0xf23b5ba1, /// 0x83c
			 0xae8cb3da, /// 0x840
			 0x556b4dd2, /// 0x844
			 0x7286542e, /// 0x848
			 0x8d03c422, /// 0x84c
			 0x0d0acffd, /// 0x850
			 0xe53a91dd, /// 0x854
			 0xbe444b69, /// 0x858
			 0x1bbb404f, /// 0x85c
			 0x2cfd3cdc, /// 0x860
			 0xe0d21222, /// 0x864
			 0xc8b78eaf, /// 0x868
			 0x286782f6, /// 0x86c
			 0x5fcb8178, /// 0x870
			 0x318f32f6, /// 0x874
			 0x7bc55ac0, /// 0x878
			 0xf2ea4679, /// 0x87c
			 0x5d4e5d06, /// 0x880
			 0x9fa6ceea, /// 0x884
			 0x9f064387, /// 0x888
			 0x078569c6, /// 0x88c
			 0xe9930eb9, /// 0x890
			 0x7de2f474, /// 0x894
			 0x88fa9391, /// 0x898
			 0x33fcf42d, /// 0x89c
			 0x5c9feaed, /// 0x8a0
			 0x58bb6ff6, /// 0x8a4
			 0x6e86949f, /// 0x8a8
			 0x063887f7, /// 0x8ac
			 0xf6f8beaf, /// 0x8b0
			 0xb6d9895c, /// 0x8b4
			 0x290127e1, /// 0x8b8
			 0xb1a9fd74, /// 0x8bc
			 0x655a0a44, /// 0x8c0
			 0xfee21c78, /// 0x8c4
			 0xf2459269, /// 0x8c8
			 0xa6f5a638, /// 0x8cc
			 0xeeadad2b, /// 0x8d0
			 0x8ea19e8b, /// 0x8d4
			 0x45424398, /// 0x8d8
			 0x188bbb37, /// 0x8dc
			 0xfe80a99f, /// 0x8e0
			 0x14a55ba6, /// 0x8e4
			 0xc7dff417, /// 0x8e8
			 0xa382ff28, /// 0x8ec
			 0x8b8200a8, /// 0x8f0
			 0x7c6ba5b1, /// 0x8f4
			 0x12c50f35, /// 0x8f8
			 0x3df80010, /// 0x8fc
			 0x19b3bd2b, /// 0x900
			 0x5b12b287, /// 0x904
			 0x58609366, /// 0x908
			 0x875a4798, /// 0x90c
			 0x0dcc8c67, /// 0x910
			 0xca5aec89, /// 0x914
			 0x8453b038, /// 0x918
			 0x785e321b, /// 0x91c
			 0x1895148e, /// 0x920
			 0x3d97e276, /// 0x924
			 0xc7c151d6, /// 0x928
			 0x71424aed, /// 0x92c
			 0xc5d7b8d4, /// 0x930
			 0xbced5c9e, /// 0x934
			 0x49cf8143, /// 0x938
			 0xe7d3a4eb, /// 0x93c
			 0x7f070c59, /// 0x940
			 0x08332955, /// 0x944
			 0x0d69b424, /// 0x948
			 0xc47857a5, /// 0x94c
			 0xe235c458, /// 0x950
			 0xa4997f88, /// 0x954
			 0x3be054a6, /// 0x958
			 0x3bd09f8f, /// 0x95c
			 0x51e33453, /// 0x960
			 0xb0e0049c, /// 0x964
			 0xf7af7656, /// 0x968
			 0x63f2627e, /// 0x96c
			 0x5a7dae4e, /// 0x970
			 0x3a8155b5, /// 0x974
			 0x1c70df2f, /// 0x978
			 0x636d750e, /// 0x97c
			 0xe915af47, /// 0x980
			 0x2199368c, /// 0x984
			 0x680b67ff, /// 0x988
			 0xc8ac3fb9, /// 0x98c
			 0x9cca8fff, /// 0x990
			 0x158009da, /// 0x994
			 0x09397757, /// 0x998
			 0xb9a83ed5, /// 0x99c
			 0x8be35f5a, /// 0x9a0
			 0x4f4997f3, /// 0x9a4
			 0xe6c9d187, /// 0x9a8
			 0x3eefa349, /// 0x9ac
			 0x8590ab6d, /// 0x9b0
			 0xf740fba3, /// 0x9b4
			 0x393087de, /// 0x9b8
			 0x68c698c9, /// 0x9bc
			 0xf845204c, /// 0x9c0
			 0x636654a0, /// 0x9c4
			 0x874a83ff, /// 0x9c8
			 0xf72bb4d0, /// 0x9cc
			 0x3ae6d0f9, /// 0x9d0
			 0x6f04b41f, /// 0x9d4
			 0xab32ee3f, /// 0x9d8
			 0x331db9ec, /// 0x9dc
			 0xbc4b11f8, /// 0x9e0
			 0x78dd6c4f, /// 0x9e4
			 0x57aa0572, /// 0x9e8
			 0x196d0791, /// 0x9ec
			 0xe0015a51, /// 0x9f0
			 0xcde6c6fd, /// 0x9f4
			 0xfe5a64b6, /// 0x9f8
			 0xdcdb62f0, /// 0x9fc
			 0x59d50a00, /// 0xa00
			 0x0c406d30, /// 0xa04
			 0x6808c1bb, /// 0xa08
			 0x6f69ca96, /// 0xa0c
			 0xbecc2beb, /// 0xa10
			 0x040c2395, /// 0xa14
			 0x836001ac, /// 0xa18
			 0xf55ecbd6, /// 0xa1c
			 0x4edaebaf, /// 0xa20
			 0x07105e28, /// 0xa24
			 0x9e7044a0, /// 0xa28
			 0x06f9a369, /// 0xa2c
			 0x853c4802, /// 0xa30
			 0x54f2cc5e, /// 0xa34
			 0xf08aa0b2, /// 0xa38
			 0xb03490f0, /// 0xa3c
			 0x2fa8380f, /// 0xa40
			 0x5063cb5e, /// 0xa44
			 0xad5d47f1, /// 0xa48
			 0xc092f8e8, /// 0xa4c
			 0xe3c2f3ec, /// 0xa50
			 0x15778395, /// 0xa54
			 0xccb55049, /// 0xa58
			 0x93f4fade, /// 0xa5c
			 0x1560ab7b, /// 0xa60
			 0x55fb7d90, /// 0xa64
			 0x2bb0e486, /// 0xa68
			 0x9fd87ce8, /// 0xa6c
			 0x59931474, /// 0xa70
			 0x39cb7807, /// 0xa74
			 0xc0e335d4, /// 0xa78
			 0xbeaff4d0, /// 0xa7c
			 0xc856e06f, /// 0xa80
			 0x2d7034ba, /// 0xa84
			 0xba9041fa, /// 0xa88
			 0xbdc33c9a, /// 0xa8c
			 0x5e207fd2, /// 0xa90
			 0x773e43e6, /// 0xa94
			 0x972d322d, /// 0xa98
			 0x5b0a3da6, /// 0xa9c
			 0x500b71af, /// 0xaa0
			 0x4c41d306, /// 0xaa4
			 0x88947f8a, /// 0xaa8
			 0xe68545cd, /// 0xaac
			 0x20ef4254, /// 0xab0
			 0x22cdadac, /// 0xab4
			 0xdd6de4cd, /// 0xab8
			 0xf526d231, /// 0xabc
			 0x91990c46, /// 0xac0
			 0x68983802, /// 0xac4
			 0x73e1f887, /// 0xac8
			 0x9069c826, /// 0xacc
			 0xe4b3a744, /// 0xad0
			 0xa253bf73, /// 0xad4
			 0xa6166770, /// 0xad8
			 0x3b630eb8, /// 0xadc
			 0x343460a5, /// 0xae0
			 0x0e9f9212, /// 0xae4
			 0x9f0a5d4a, /// 0xae8
			 0xdff9d986, /// 0xaec
			 0x6f5e8fd2, /// 0xaf0
			 0xc5bd8adc, /// 0xaf4
			 0x4d0c661e, /// 0xaf8
			 0x094eaed4, /// 0xafc
			 0x47e9d312, /// 0xb00
			 0x00f5ca0b, /// 0xb04
			 0xc3f77de3, /// 0xb08
			 0x192773d8, /// 0xb0c
			 0x7a3ea136, /// 0xb10
			 0x3d924c55, /// 0xb14
			 0x7a40c66f, /// 0xb18
			 0xcba563d4, /// 0xb1c
			 0x6059e33b, /// 0xb20
			 0x62f1cb37, /// 0xb24
			 0x6908149e, /// 0xb28
			 0x352eb344, /// 0xb2c
			 0xe30c670c, /// 0xb30
			 0x3c92ba6a, /// 0xb34
			 0xcfbf8880, /// 0xb38
			 0xf84d04a6, /// 0xb3c
			 0x052b665c, /// 0xb40
			 0x137f3026, /// 0xb44
			 0x4dd81468, /// 0xb48
			 0xadce3c97, /// 0xb4c
			 0xe00e028c, /// 0xb50
			 0x904cc977, /// 0xb54
			 0xe83f40bc, /// 0xb58
			 0x8e1ad005, /// 0xb5c
			 0x36d13156, /// 0xb60
			 0x2ba826f8, /// 0xb64
			 0xa06851f8, /// 0xb68
			 0x4d66f81b, /// 0xb6c
			 0xd99a130a, /// 0xb70
			 0x404d9e4b, /// 0xb74
			 0xfa1f462d, /// 0xb78
			 0x65d58892, /// 0xb7c
			 0x2770b792, /// 0xb80
			 0xfc3ab9e0, /// 0xb84
			 0x6a3e8a3b, /// 0xb88
			 0x7cb62918, /// 0xb8c
			 0x518bce95, /// 0xb90
			 0x2a7a169b, /// 0xb94
			 0x1255e7b2, /// 0xb98
			 0x70809629, /// 0xb9c
			 0x69242031, /// 0xba0
			 0x8a5ecf2f, /// 0xba4
			 0xf4407146, /// 0xba8
			 0x89d49bfd, /// 0xbac
			 0xff1f2a8a, /// 0xbb0
			 0x0d670634, /// 0xbb4
			 0xd4587121, /// 0xbb8
			 0x8d703c3a, /// 0xbbc
			 0x179cf47f, /// 0xbc0
			 0x8e29e723, /// 0xbc4
			 0x445e142a, /// 0xbc8
			 0x0cdf2d6f, /// 0xbcc
			 0x533cfc30, /// 0xbd0
			 0xeca27f93, /// 0xbd4
			 0x396be9ee, /// 0xbd8
			 0xa6b148f5, /// 0xbdc
			 0x8cbe20a3, /// 0xbe0
			 0x29a7f478, /// 0xbe4
			 0x00b9ecf2, /// 0xbe8
			 0xcbfa2e4e, /// 0xbec
			 0xe4be8b7e, /// 0xbf0
			 0x30c66ea3, /// 0xbf4
			 0x5b7fa583, /// 0xbf8
			 0x51b0bb7f, /// 0xbfc
			 0x0af0502d, /// 0xc00
			 0x39530335, /// 0xc04
			 0x9c42d769, /// 0xc08
			 0xae3378bb, /// 0xc0c
			 0xb79ecfe1, /// 0xc10
			 0xea68685a, /// 0xc14
			 0x707ad00e, /// 0xc18
			 0x9ddb78bc, /// 0xc1c
			 0x3850ad5f, /// 0xc20
			 0x4c033a3b, /// 0xc24
			 0xf9b8d9ca, /// 0xc28
			 0x7c323b00, /// 0xc2c
			 0x454d7562, /// 0xc30
			 0xb30dd862, /// 0xc34
			 0xff6793cf, /// 0xc38
			 0x1e3ab3c5, /// 0xc3c
			 0x77c0e8b7, /// 0xc40
			 0xfaa07b1d, /// 0xc44
			 0xb20c9f29, /// 0xc48
			 0xe509b98c, /// 0xc4c
			 0x5fa321e2, /// 0xc50
			 0xaad53cb4, /// 0xc54
			 0xea803b71, /// 0xc58
			 0x0aa109ff, /// 0xc5c
			 0xd8a2d002, /// 0xc60
			 0xca7c1eee, /// 0xc64
			 0x0a05dade, /// 0xc68
			 0x9b3772d0, /// 0xc6c
			 0xaf19d7fb, /// 0xc70
			 0x41fa15c5, /// 0xc74
			 0x47033a49, /// 0xc78
			 0x1565385d, /// 0xc7c
			 0x480da95a, /// 0xc80
			 0xecd1298f, /// 0xc84
			 0x10bb4392, /// 0xc88
			 0x0b268de6, /// 0xc8c
			 0x32617e97, /// 0xc90
			 0x9c7e98bc, /// 0xc94
			 0x685b966c, /// 0xc98
			 0xf4f27a3d, /// 0xc9c
			 0xf90c2913, /// 0xca0
			 0xba0dd3e9, /// 0xca4
			 0xe71877bf, /// 0xca8
			 0x3dbfae41, /// 0xcac
			 0xe5b1ce3c, /// 0xcb0
			 0x01a87790, /// 0xcb4
			 0x1f94cb64, /// 0xcb8
			 0x86e3a644, /// 0xcbc
			 0xc94ed592, /// 0xcc0
			 0x4dfa583c, /// 0xcc4
			 0x7aa225b3, /// 0xcc8
			 0x32879365, /// 0xccc
			 0x1b155b81, /// 0xcd0
			 0xdc34223c, /// 0xcd4
			 0xfd90b180, /// 0xcd8
			 0xe341bd3d, /// 0xcdc
			 0x13f4ce70, /// 0xce0
			 0x20cd311d, /// 0xce4
			 0x9ce4dadf, /// 0xce8
			 0xb34a8dd5, /// 0xcec
			 0x4cd6b041, /// 0xcf0
			 0xc2ab8c5d, /// 0xcf4
			 0xf2caa0d9, /// 0xcf8
			 0x9dde8131, /// 0xcfc
			 0xbb97c407, /// 0xd00
			 0xf4e0fd46, /// 0xd04
			 0x4dad7fc2, /// 0xd08
			 0xa9e0eb0b, /// 0xd0c
			 0x061f89d1, /// 0xd10
			 0xd0f20362, /// 0xd14
			 0x2895d7d9, /// 0xd18
			 0x80edd174, /// 0xd1c
			 0xd3ce2e47, /// 0xd20
			 0x3775b984, /// 0xd24
			 0x0c42f5ae, /// 0xd28
			 0x2d49298d, /// 0xd2c
			 0xa3cc2aa6, /// 0xd30
			 0x064a4bf6, /// 0xd34
			 0x6fb1749b, /// 0xd38
			 0x83e22707, /// 0xd3c
			 0xf01b4355, /// 0xd40
			 0xd536b49c, /// 0xd44
			 0xc3381387, /// 0xd48
			 0x6f7ad40b, /// 0xd4c
			 0x6530663e, /// 0xd50
			 0x7a92645c, /// 0xd54
			 0x31048351, /// 0xd58
			 0xe96c2425, /// 0xd5c
			 0x2daea85a, /// 0xd60
			 0xb48b60d0, /// 0xd64
			 0xc7ee6b12, /// 0xd68
			 0x123b934d, /// 0xd6c
			 0x07ac0ccc, /// 0xd70
			 0xe2f1f316, /// 0xd74
			 0x7e267a6d, /// 0xd78
			 0x733ef2f8, /// 0xd7c
			 0x1e3c288d, /// 0xd80
			 0x1546bdd7, /// 0xd84
			 0x2aebf681, /// 0xd88
			 0xd02a3428, /// 0xd8c
			 0xe630e9ba, /// 0xd90
			 0xcdf003d9, /// 0xd94
			 0x7b287dd2, /// 0xd98
			 0x42b4bf97, /// 0xd9c
			 0x6902aefd, /// 0xda0
			 0x9bb4756c, /// 0xda4
			 0xfcafec09, /// 0xda8
			 0x27bd0ad5, /// 0xdac
			 0x80a6fd98, /// 0xdb0
			 0x1e4db45c, /// 0xdb4
			 0x19a2f6a5, /// 0xdb8
			 0x54c97f11, /// 0xdbc
			 0xd24516f8, /// 0xdc0
			 0x8da30979, /// 0xdc4
			 0x860ba638, /// 0xdc8
			 0xb604ccfb, /// 0xdcc
			 0x4cf31a3a, /// 0xdd0
			 0x2c008fad, /// 0xdd4
			 0x481955db, /// 0xdd8
			 0xee3cf48b, /// 0xddc
			 0xdb345ee9, /// 0xde0
			 0xe5405f1e, /// 0xde4
			 0xbf4947a0, /// 0xde8
			 0x41f5e882, /// 0xdec
			 0xe15cc70d, /// 0xdf0
			 0x0cf78372, /// 0xdf4
			 0xc78a29cc, /// 0xdf8
			 0xa31970df, /// 0xdfc
			 0x1a3510f3, /// 0xe00
			 0x17b2af04, /// 0xe04
			 0x3d01e0ae, /// 0xe08
			 0x05317c7a, /// 0xe0c
			 0xe22734b6, /// 0xe10
			 0x90c1684d, /// 0xe14
			 0x0e121f8a, /// 0xe18
			 0x089d0746, /// 0xe1c
			 0x829a82aa, /// 0xe20
			 0x1205f565, /// 0xe24
			 0x844784fa, /// 0xe28
			 0xc63dfa8e, /// 0xe2c
			 0x77bacbf8, /// 0xe30
			 0xd9def5fc, /// 0xe34
			 0xa1196590, /// 0xe38
			 0xd941899e, /// 0xe3c
			 0x165cc85f, /// 0xe40
			 0x1de291eb, /// 0xe44
			 0x5f0d7fef, /// 0xe48
			 0xd6218229, /// 0xe4c
			 0x5885a71a, /// 0xe50
			 0xe794b025, /// 0xe54
			 0x2b65e3c4, /// 0xe58
			 0x43f5345b, /// 0xe5c
			 0x7919dbf4, /// 0xe60
			 0x82be4ccf, /// 0xe64
			 0x7c5e8b21, /// 0xe68
			 0x4a09db90, /// 0xe6c
			 0xf5769ae9, /// 0xe70
			 0xa05be02a, /// 0xe74
			 0x4b742860, /// 0xe78
			 0x42321741, /// 0xe7c
			 0xb910cb7d, /// 0xe80
			 0xcef3c0d3, /// 0xe84
			 0x3420d57d, /// 0xe88
			 0xbdbadb03, /// 0xe8c
			 0x279fdfc2, /// 0xe90
			 0x7787b1d7, /// 0xe94
			 0x14b46711, /// 0xe98
			 0x93ba1baf, /// 0xe9c
			 0x3272e69d, /// 0xea0
			 0x8be7fb5d, /// 0xea4
			 0x9cb7adb0, /// 0xea8
			 0x997b882f, /// 0xeac
			 0x4278df95, /// 0xeb0
			 0xb972ef95, /// 0xeb4
			 0x26500926, /// 0xeb8
			 0x3d15c535, /// 0xebc
			 0x48c3b32f, /// 0xec0
			 0x4b4e35a7, /// 0xec4
			 0xc6544cd7, /// 0xec8
			 0x98fb90b1, /// 0xecc
			 0x445e717d, /// 0xed0
			 0x7b4db088, /// 0xed4
			 0x087e688d, /// 0xed8
			 0xcb6e502e, /// 0xedc
			 0x98ee0356, /// 0xee0
			 0x7d89ec65, /// 0xee4
			 0x738c4bc2, /// 0xee8
			 0x617632a3, /// 0xeec
			 0x2866b3c7, /// 0xef0
			 0xc6850e45, /// 0xef4
			 0xa28286b0, /// 0xef8
			 0x0c9e532e, /// 0xefc
			 0xcc2a293b, /// 0xf00
			 0x577990ec, /// 0xf04
			 0x40d0fc16, /// 0xf08
			 0x37e06c65, /// 0xf0c
			 0x3ce36274, /// 0xf10
			 0xb027c393, /// 0xf14
			 0x58706f0c, /// 0xf18
			 0x01c900a5, /// 0xf1c
			 0x47587e29, /// 0xf20
			 0x09df4812, /// 0xf24
			 0x817ad35c, /// 0xf28
			 0x97eddf3e, /// 0xf2c
			 0x4df4a8ab, /// 0xf30
			 0xbe335dd9, /// 0xf34
			 0x4dd4e8b7, /// 0xf38
			 0x995ad353, /// 0xf3c
			 0x49fdddb3, /// 0xf40
			 0xbb75b1b5, /// 0xf44
			 0x9c718847, /// 0xf48
			 0x8e191ca7, /// 0xf4c
			 0x7fc50496, /// 0xf50
			 0x23d397b3, /// 0xf54
			 0x0ff9e81f, /// 0xf58
			 0x146ec454, /// 0xf5c
			 0xfe53af70, /// 0xf60
			 0x76be30b5, /// 0xf64
			 0x0689866f, /// 0xf68
			 0x66f1a4a9, /// 0xf6c
			 0xd5b1fce3, /// 0xf70
			 0x5b5796cb, /// 0xf74
			 0xfefd8659, /// 0xf78
			 0xc462c27b, /// 0xf7c
			 0xd9a0de9b, /// 0xf80
			 0x0b6196ec, /// 0xf84
			 0x5b5f3ac8, /// 0xf88
			 0x73dbebd7, /// 0xf8c
			 0x285b5338, /// 0xf90
			 0x4044747c, /// 0xf94
			 0x9c696a74, /// 0xf98
			 0xc7861ff3, /// 0xf9c
			 0x957a4873, /// 0xfa0
			 0xe0810672, /// 0xfa4
			 0x6c827ce4, /// 0xfa8
			 0x92899250, /// 0xfac
			 0xd67c86f5, /// 0xfb0
			 0xfa9d5627, /// 0xfb4
			 0x0831be7b, /// 0xfb8
			 0x5c15f460, /// 0xfbc
			 0x9a52b913, /// 0xfc0
			 0xb610224b, /// 0xfc4
			 0x1e507ef4, /// 0xfc8
			 0xe4acf60c, /// 0xfcc
			 0x2232f80e, /// 0xfd0
			 0x786f854c, /// 0xfd4
			 0x559f9c9e, /// 0xfd8
			 0x981f772c, /// 0xfdc
			 0x0cd15f36, /// 0xfe0
			 0xa69718ed, /// 0xfe4
			 0xdf5d6125, /// 0xfe8
			 0xebd197be, /// 0xfec
			 0xdd699e32, /// 0xff0
			 0x032c52e2, /// 0xff4
			 0x722986ae, /// 0xff8
			 0xe91fcf29 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xcb000000,                                                  // -8388608.0                                  /// 00000
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00010
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00018
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00020
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00028
			 0x0c400000,                                                  // Leading 1s:                                 /// 0002c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00038
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0003c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00040
			 0x80011111,                                                  // -9.7958E-41                                 /// 00044
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00054
			 0x00011111,                                                  // 9.7958E-41                                  /// 00058
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00060
			 0x3f028f5c,                                                  // 0.51                                        /// 00064
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00068
			 0x00011111,                                                  // 9.7958E-41                                  /// 0006c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00070
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00074
			 0x0c780000,                                                  // Leading 1s:                                 /// 00078
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0008c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00090
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00094
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00098
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0009c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0xbf028f5c,                                                  // -0.51                                       /// 000f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0xff800000,                                                  // -inf                                        /// 000fc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00104
			 0x7f800000,                                                  // inf                                         /// 00108
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0010c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00110
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00114
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00120
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0012c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00130
			 0xbf028f5c,                                                  // -0.51                                       /// 00134
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00138
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0013c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00140
			 0x00011111,                                                  // 9.7958E-41                                  /// 00144
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00148
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0014c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00158
			 0x00011111,                                                  // 9.7958E-41                                  /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00160
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00164
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00168
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0016c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00174
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00178
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0017c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00190
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00194
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x00000000,                                                  // zero                                        /// 001a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00200
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00204
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00214
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00218
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0021c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00224
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00228
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0022c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00230
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x0c600000,                                                  // Leading 1s:                                 /// 00238
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00240
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00244
			 0xbf028f5c,                                                  // -0.51                                       /// 00248
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00250
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00254
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0025c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00260
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00268
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00270
			 0xbf028f5c,                                                  // -0.51                                       /// 00274
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00278
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0027c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00284
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00288
			 0x7fc00001,                                                  // signaling NaN                               /// 0028c
			 0x33333333,                                                  // 4 random values                             /// 00290
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00298
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002dc
			 0x55555555,                                                  // 4 random values                             /// 002e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00300
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00304
			 0x0e000007,                                                  // Trailing 1s:                                /// 00308
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00310
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00314
			 0x7f800000,                                                  // inf                                         /// 00318
			 0xffc00001,                                                  // -signaling NaN                              /// 0031c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00320
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00324
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00328
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00330
			 0x0e000003,                                                  // Trailing 1s:                                /// 00334
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00338
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x55555555,                                                  // 4 random values                             /// 00340
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00344
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00348
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0034c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x80011111,                                                  // -9.7958E-41                                 /// 0035c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00360
			 0xbf028f5c,                                                  // -0.51                                       /// 00364
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00374
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00378
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0037c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00380
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00384
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00388
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0038c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00390
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00398
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x3f028f5c,                                                  // 0.51                                        /// 003b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e0
			 0x3f028f5c,                                                  // 0.51                                        /// 003e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003fc
			 0x80000000,                                                  // -zero                                       /// 00400
			 0xff800000,                                                  // -inf                                        /// 00404
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00408
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0040c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00410
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00414
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00418
			 0xffc00001,                                                  // -signaling NaN                              /// 0041c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00424
			 0xffc00001,                                                  // -signaling NaN                              /// 00428
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00430
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0043c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00440
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00444
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00454
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0045c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00464
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x3f028f5c,                                                  // 0.51                                        /// 00474
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0047c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0048c
			 0xbf028f5c,                                                  // -0.51                                       /// 00490
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00494
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00498
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004dc
			 0x33333333,                                                  // 4 random values                             /// 004e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0xbf028f5c,                                                  // -0.51                                       /// 00504
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00518
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0051c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00520
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00524
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00528
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0052c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00530
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00534
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0053c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0e000003,                                                  // Trailing 1s:                                /// 00544
			 0x80000000,                                                  // -zero                                       /// 00548
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0054c
			 0xcb000000,                                                  // -8388608.0                                  /// 00550
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00558
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0055c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00560
			 0x0c600000,                                                  // Leading 1s:                                 /// 00564
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00568
			 0x0e000001,                                                  // Trailing 1s:                                /// 0056c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00580
			 0xcb000000,                                                  // -8388608.0                                  /// 00584
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00588
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0058c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00590
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00594
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00598
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0059c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005a4
			 0x7fc00001,                                                  // signaling NaN                               /// 005a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00600
			 0xcb000000,                                                  // -8388608.0                                  /// 00604
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00608
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x33333333,                                                  // 4 random values                             /// 00614
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0061c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00630
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00638
			 0x4b000000,                                                  // 8388608.0                                   /// 0063c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00640
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00644
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00648
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0064c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00650
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00658
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xff800000,                                                  // -inf                                        /// 00660
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00664
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00668
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00674
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00678
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0067c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00680
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00688
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00694
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00698
			 0xcb000000,                                                  // -8388608.0                                  /// 0069c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x7fc00001,                                                  // signaling NaN                               /// 006a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00700
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00704
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00708
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00710
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00714
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00718
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00720
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00724
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00728
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00730
			 0x0c400000,                                                  // Leading 1s:                                 /// 00734
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00738
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00748
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x7fc00001,                                                  // signaling NaN                               /// 00750
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00754
			 0x00000000,                                                  // zero                                        /// 00758
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0075c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00760
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00764
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00768
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00770
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00774
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00778
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0077c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00780
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00784
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00790
			 0x0e000003,                                                  // Trailing 1s:                                /// 00794
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00798
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0079c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 007e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007e4
			 0x7f800000,                                                  // inf                                         /// 007e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00808
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0080c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00810
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00814
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0081c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00824
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0082c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00830
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00838
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00840
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0084c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00850
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00860
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00864
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00868
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0086c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00870
			 0x4b000000,                                                  // 8388608.0                                   /// 00874
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00878
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0087c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00880
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00884
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00888
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0088c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00890
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x7f800000,                                                  // inf                                         /// 00898
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0xcb000000,                                                  // -8388608.0                                  /// 008cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00900
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00904
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00908
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0090c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00910
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00918
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00924
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00928
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0092c
			 0x80000000,                                                  // -zero                                       /// 00930
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00934
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0093c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00944
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00948
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0094c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00950
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00954
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00958
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0095c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0096c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00978
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0097c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00980
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00984
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00990
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00994
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00998
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0099c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x4b000000,                                                  // 8388608.0                                   /// 00a0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a18
			 0xffc00001,                                                  // -signaling NaN                              /// 00a1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a28
			 0x3f028f5c,                                                  // 0.51                                        /// 00a2c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a6c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a7c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ab8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ac0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ac8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ad0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ad4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ae4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ae8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00af0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00afc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b30
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b38
			 0xffc00001,                                                  // -signaling NaN                              /// 00b3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b64
			 0x80000000,                                                  // -zero                                       /// 00b68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ba8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bbc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bc0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bc4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bc8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bcc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x7fc00001,                                                  // signaling NaN                               /// 00be0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x4b000000,                                                  // 8388608.0                                   /// 00bf4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c54
			 0x3f028f5c,                                                  // 0.51                                        /// 00c58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c64
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c6c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ca4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cb0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cbc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cc8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ccc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ce0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ce4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ce8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cf0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cf4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d04
			 0xff800000,                                                  // -inf                                        /// 00d08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d0c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xbf028f5c,                                                  // -0.51                                       /// 00d14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d20
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d38
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d60
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d84
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d88
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d90
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d9c
			 0x80000000,                                                  // -zero                                       /// 00da0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00da8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00db4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dc8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dcc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dd0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00de0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00de4
			 0x7f800000,                                                  // inf                                         /// 00de8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00df0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00df4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00df8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e30
			 0xbf028f5c,                                                  // -0.51                                       /// 00e34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e38
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x4b000000,                                                  // 8388608.0                                   /// 00e4c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0xff800000,                                                  // -inf                                        /// 00e84
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ea4
			 0x7f800000,                                                  // inf                                         /// 00ea8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ebc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ec8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ed0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ed8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00edc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ee0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ee8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00eec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ef0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ef4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ef8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00efc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f28
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f68
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fa0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fa4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fa8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fe0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ff0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ff4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ff8
			 0x0e07ffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00000
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0000c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00010
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00014
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00018
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0001c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00020
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00024
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00028
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0002c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00034
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00038
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0003c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00040
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00044
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0004c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00050
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00054
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00058
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0005c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00064
			 0x00011111,                                                  // 9.7958E-41                                  /// 00068
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0006c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00070
			 0x7fc00001,                                                  // signaling NaN                               /// 00074
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00078
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0007c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00084
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0008c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00090
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00098
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0009c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00100
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00104
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00108
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00110
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00114
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00118
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x3f028f5c,                                                  // 0.51                                        /// 00128
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0012c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00130
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00134
			 0x4b000000,                                                  // 8388608.0                                   /// 00138
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0013c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00140
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00144
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00148
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0014c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00150
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x33333333,                                                  // 4 random values                             /// 00158
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00168
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0016c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00170
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00174
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00178
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0017c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00180
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00184
			 0x00011111,                                                  // 9.7958E-41                                  /// 00188
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0018c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00190
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00194
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00198
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0019c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 001a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001ac
			 0x7f800000,                                                  // inf                                         /// 001b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001e4
			 0xcb000000,                                                  // -8388608.0                                  /// 001e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001f0
			 0x33333333,                                                  // 4 random values                             /// 001f4
			 0x80000000,                                                  // -zero                                       /// 001f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00200
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00204
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00210
			 0x0e000007,                                                  // Trailing 1s:                                /// 00214
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0021c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x0e000001,                                                  // Trailing 1s:                                /// 0022c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00230
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00234
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00238
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x3f028f5c,                                                  // 0.51                                        /// 00244
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00248
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0024c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00250
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00254
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00258
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00260
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00264
			 0x00000000,                                                  // zero                                        /// 00268
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00270
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00278
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00280
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00284
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00288
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00294
			 0x0c600000,                                                  // Leading 1s:                                 /// 00298
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0029c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002a0
			 0x33333333,                                                  // 4 random values                             /// 002a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002a8
			 0xcb000000,                                                  // -8388608.0                                  /// 002ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00304
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0030c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x0e000003,                                                  // Trailing 1s:                                /// 0031c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00320
			 0x7f800000,                                                  // inf                                         /// 00324
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00338
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00344
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0034c
			 0xcb000000,                                                  // -8388608.0                                  /// 00350
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00354
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00364
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00368
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00374
			 0x0e000007,                                                  // Trailing 1s:                                /// 00378
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00380
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00384
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00388
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0038c
			 0x33333333,                                                  // 4 random values                             /// 00390
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00398
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0039c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0xbf028f5c,                                                  // -0.51                                       /// 003b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003ec
			 0x4b000000,                                                  // 8388608.0                                   /// 003f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00400
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00404
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00408
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00414
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00418
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0041c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00420
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00428
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00430
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00434
			 0xff800000,                                                  // -inf                                        /// 00438
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00440
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00444
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00448
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0044c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00454
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0045c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00460
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00464
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00468
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00474
			 0x0e000001,                                                  // Trailing 1s:                                /// 00478
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0047c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00484
			 0x80000000,                                                  // -zero                                       /// 00488
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0048c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00490
			 0x80011111,                                                  // -9.7958E-41                                 /// 00494
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00498
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0049c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0xff800000,                                                  // -inf                                        /// 004e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 004f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00500
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00508
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0050c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00518
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0051c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00520
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00524
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00530
			 0x0c780000,                                                  // Leading 1s:                                 /// 00534
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0053c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00540
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00544
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00548
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0054c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00550
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00554
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00558
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0055c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00560
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00564
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00570
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x00011111,                                                  // 9.7958E-41                                  /// 00588
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00594
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00598
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0xcb000000,                                                  // -8388608.0                                  /// 005c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0xffc00001,                                                  // -signaling NaN                              /// 00604
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0060c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0e000001,                                                  // Trailing 1s:                                /// 00614
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00624
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00630
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00638
			 0x0e000007,                                                  // Trailing 1s:                                /// 0063c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00640
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00648
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0064c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00654
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00658
			 0x7fc00001,                                                  // signaling NaN                               /// 0065c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00664
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00668
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0066c
			 0x55555555,                                                  // 4 random values                             /// 00670
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00678
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0067c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00680
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00684
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00688
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0068c
			 0x7fc00001,                                                  // signaling NaN                               /// 00690
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00698
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006b0
			 0x33333333,                                                  // 4 random values                             /// 006b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 006c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00700
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00704
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0070c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00710
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00714
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0071c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00720
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00724
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00728
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00730
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00734
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0073c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00740
			 0x0e000003,                                                  // Trailing 1s:                                /// 00744
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00748
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0074c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00750
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00754
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00758
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0075c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00760
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00764
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0076c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00770
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00774
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00778
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0077c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00780
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00788
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0078c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00794
			 0x00000000,                                                  // zero                                        /// 00798
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0079c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x3f028f5c,                                                  // 0.51                                        /// 007d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0xff800000,                                                  // -inf                                        /// 007e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00800
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00804
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00808
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00814
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00818
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0081c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00820
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00824
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00828
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0082c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00830
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00834
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0083c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00840
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80000000,                                                  // -zero                                       /// 00848
			 0x0c700000,                                                  // Leading 1s:                                 /// 0084c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00850
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00858
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00860
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00864
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00874
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00878
			 0x7f800000,                                                  // inf                                         /// 0087c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00880
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00888
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00890
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008ac
			 0x4b000000,                                                  // 8388608.0                                   /// 008b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 008e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f4
			 0x3f028f5c,                                                  // 0.51                                        /// 008f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00904
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00918
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00920
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00924
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00928
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0092c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00930
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00934
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0093c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00940
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00948
			 0x00011111,                                                  // 9.7958E-41                                  /// 0094c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00950
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0095c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00960
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00964
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0096c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00970
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00978
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0097c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00980
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00984
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00988
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0098c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00990
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0e000001,                                                  // Trailing 1s:                                /// 00998
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0099c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x00000000,                                                  // zero                                        /// 009d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 009ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009f4
			 0x4b000000,                                                  // 8388608.0                                   /// 009f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a18
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a3c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a50
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a5c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aa0
			 0xcb000000,                                                  // -8388608.0                                  /// 00aa4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00aa8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ab0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ab8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ac8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ad0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ad4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ad8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00adc
			 0x7f800000,                                                  // inf                                         /// 00ae0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ae4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00afc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b00
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b04
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b20
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80000000,                                                  // -zero                                       /// 00b3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b54
			 0xffc00001,                                                  // -signaling NaN                              /// 00b58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b7c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b84
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ba0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ba4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ba8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bbc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bc0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bdc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0xcb000000,                                                  // -8388608.0                                  /// 00be8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bf0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bfc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c40
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c60
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c70
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c74
			 0x3f028f5c,                                                  // 0.51                                        /// 00c78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c84
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ca4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x7f800000,                                                  // inf                                         /// 00cb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cbc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cc4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cc8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ccc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x00000000,                                                  // zero                                        /// 00cdc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ce8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cf4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cfc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d10
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d28
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d50
			 0xff800000,                                                  // -inf                                        /// 00d54
			 0x7fc00001,                                                  // signaling NaN                               /// 00d58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d7c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d98
			 0x7f800000,                                                  // inf                                         /// 00d9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00da0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00da8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00db4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00db8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dc0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dcc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ddc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00de8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00df0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dfc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e04
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x7fc00001,                                                  // signaling NaN                               /// 00e18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e20
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e54
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e58
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0xbf028f5c,                                                  // -0.51                                       /// 00e78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e88
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e90
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e98
			 0x00000000,                                                  // zero                                        /// 00e9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ea8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eb4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00eb8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ebc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ec0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ec8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ed4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ed8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ee0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ee4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ee8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ef0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ef4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ef8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00efc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f04
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f10
			 0x55555555,                                                  // 4 random values                             /// 00f14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x7fc00001,                                                  // signaling NaN                               /// 00f38
			 0x7f800000,                                                  // inf                                         /// 00f3c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f50
			 0x7f800000,                                                  // inf                                         /// 00f54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x80000000,                                                  // -zero                                       /// 00f5c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f80
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f90
			 0x00000000,                                                  // zero                                        /// 00f94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fa0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fa8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fc0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fc4
			 0x33333333,                                                  // 4 random values                             /// 00fc8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fcc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fd0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fd8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fe0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fe4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fe8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ff0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ff4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ff8
			 0x0c7fff00                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00000
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0xcb000000,                                                  // -8388608.0                                  /// 00008
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00010
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00014
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0001c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00020
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00024
			 0x00011111,                                                  // 9.7958E-41                                  /// 00028
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0002c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00030
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00034
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00048
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0004c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00050
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00054
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0005c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00064
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00068
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00070
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00074
			 0x0c400000,                                                  // Leading 1s:                                 /// 00078
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0007c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x3f028f5c,                                                  // 0.51                                        /// 0008c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00090
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00094
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x7f800000,                                                  // inf                                         /// 0009c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000e4
			 0x3f028f5c,                                                  // 0.51                                        /// 000e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x55555555,                                                  // 4 random values                             /// 00104
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00108
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00110
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00114
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00118
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0011c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00128
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0012c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00130
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00134
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00148
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00158
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0015c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00160
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00164
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00168
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00170
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00174
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0017c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00188
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0018c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00198
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 001c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001e4
			 0x00000000,                                                  // zero                                        /// 001e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001f0
			 0x7f800000,                                                  // inf                                         /// 001f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00200
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00204
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x4b000000,                                                  // 8388608.0                                   /// 0020c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00214
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00218
			 0xbf028f5c,                                                  // -0.51                                       /// 0021c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00220
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00224
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00228
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00230
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00234
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00238
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0023c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00240
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00244
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00248
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00250
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00258
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0025c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00260
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00268
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00278
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00280
			 0x0c780000,                                                  // Leading 1s:                                 /// 00284
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00294
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00298
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0xcb000000,                                                  // -8388608.0                                  /// 002a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d0
			 0xff800000,                                                  // -inf                                        /// 002d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002d8
			 0xffc00001,                                                  // -signaling NaN                              /// 002dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00304
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00308
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00310
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00314
			 0x0e000003,                                                  // Trailing 1s:                                /// 00318
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0031c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00320
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00324
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00328
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0032c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00334
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x00000000,                                                  // zero                                        /// 00340
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00344
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00348
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0034c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00354
			 0x0c400000,                                                  // Leading 1s:                                 /// 00358
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00364
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00368
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00370
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00374
			 0x0c400000,                                                  // Leading 1s:                                 /// 00378
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00380
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00384
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0038c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x55555555,                                                  // 4 random values                             /// 00398
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0039c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 003ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003b4
			 0x7fc00001,                                                  // signaling NaN                               /// 003b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0xff800000,                                                  // -inf                                        /// 003c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00400
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00408
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00410
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00414
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00418
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0041c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00420
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00424
			 0x0e000003,                                                  // Trailing 1s:                                /// 00428
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0042c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00430
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00438
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x7fc00001,                                                  // signaling NaN                               /// 00440
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00444
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00448
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00450
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00454
			 0x7f800000,                                                  // inf                                         /// 00458
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00460
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00464
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00470
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00474
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0047c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00480
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00484
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00488
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0048c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0xff800000,                                                  // -inf                                        /// 00494
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00498
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0049c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x33333333,                                                  // 4 random values                             /// 004ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x80000000,                                                  // -zero                                       /// 004bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00500
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00504
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00508
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0050c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00510
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0051c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x0c780000,                                                  // Leading 1s:                                 /// 00524
			 0x0e000007,                                                  // Trailing 1s:                                /// 00528
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00530
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00534
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0053c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00540
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00548
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0054c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00558
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00560
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0056c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00570
			 0x80000000,                                                  // -zero                                       /// 00574
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00578
			 0x80011111,                                                  // -9.7958E-41                                 /// 0057c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00580
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00588
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0058c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0059c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00600
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0060c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00610
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00614
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00618
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0061c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00624
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00628
			 0x7f800000,                                                  // inf                                         /// 0062c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00630
			 0x0c700000,                                                  // Leading 1s:                                 /// 00634
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00638
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00640
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00644
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00648
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00650
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0065c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00660
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00668
			 0x7f800000,                                                  // inf                                         /// 0066c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x00011111,                                                  // 9.7958E-41                                  /// 00674
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00680
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00684
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00688
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0068c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00690
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00698
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0069c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x4b000000,                                                  // 8388608.0                                   /// 006bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006d4
			 0xcb000000,                                                  // -8388608.0                                  /// 006d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00700
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00704
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0070c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00718
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00720
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00728
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00744
			 0x00011111,                                                  // 9.7958E-41                                  /// 00748
			 0x0c600000,                                                  // Leading 1s:                                 /// 0074c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00754
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0075c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00764
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00768
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00770
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00774
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00778
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0077c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00780
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00784
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00790
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00794
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x7f800000,                                                  // inf                                         /// 0079c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007ac
			 0x7f800000,                                                  // inf                                         /// 007b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007c8
			 0x80000000,                                                  // -zero                                       /// 007cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00800
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00808
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0080c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00810
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00818
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0081c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00820
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00824
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00828
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0082c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0083c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00840
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00844
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00848
			 0x80000000,                                                  // -zero                                       /// 0084c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00854
			 0x80000000,                                                  // -zero                                       /// 00858
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0085c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00860
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00864
			 0x7fc00001,                                                  // signaling NaN                               /// 00868
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0086c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00870
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0087c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00884
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00894
			 0x55555555,                                                  // 4 random values                             /// 00898
			 0x55555555,                                                  // 4 random values                             /// 0089c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008a8
			 0xbf028f5c,                                                  // -0.51                                       /// 008ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008b4
			 0x33333333,                                                  // 4 random values                             /// 008b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x7f800000,                                                  // inf                                         /// 008c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008f0
			 0x55555555,                                                  // 4 random values                             /// 008f4
			 0x33333333,                                                  // 4 random values                             /// 008f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00900
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00904
			 0x0e000007,                                                  // Trailing 1s:                                /// 00908
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0090c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00914
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00918
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0091c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00920
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00924
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00928
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0092c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00934
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00938
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0093c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00940
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00944
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00948
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00950
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00958
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0095c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00960
			 0xffc00001,                                                  // -signaling NaN                              /// 00964
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00968
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00974
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00978
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00984
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00988
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00990
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00994
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0099c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009b4
			 0x00000000,                                                  // zero                                        /// 009b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 009c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x7f800000,                                                  // inf                                         /// 00a00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x7fc00001,                                                  // signaling NaN                               /// 00a08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a24
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a2c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a3c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a7c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a9c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00aa0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ab8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00adc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ae4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00af0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00afc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b10
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b14
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b28
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b30
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b68
			 0x33333333,                                                  // 4 random values                             /// 00b6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ba0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ba8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bb4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bc0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00be0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00be4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bec
			 0x00000000,                                                  // zero                                        /// 00bf0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bf8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00000000,                                                  // zero                                        /// 00c0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c14
			 0x7f800000,                                                  // inf                                         /// 00c18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c28
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c38
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c74
			 0xffc00001,                                                  // -signaling NaN                              /// 00c78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c7c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c88
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ca8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cb0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cb4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cb8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cc4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cd0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cd8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cdc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ce4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ce8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cf4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cf8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d08
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d14
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d20
			 0x00000000,                                                  // zero                                        /// 00d24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d38
			 0x4b000000,                                                  // 8388608.0                                   /// 00d3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d50
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d7c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00da0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00da4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00da8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00db0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00db4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00db8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dbc
			 0x33333333,                                                  // 4 random values                             /// 00dc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dc4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dc8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00de4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00de8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00df0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00df8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e08
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x3f028f5c,                                                  // 0.51                                        /// 00e34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x7fc00001,                                                  // signaling NaN                               /// 00e3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e48
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e54
			 0x00000000,                                                  // zero                                        /// 00e58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e5c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e84
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e88
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e94
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e98
			 0x4b000000,                                                  // 8388608.0                                   /// 00e9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ea8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eb0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eb4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00eb8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ec4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ecc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ed4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ed8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ee0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ee4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ee8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00eec
			 0x80000000,                                                  // -zero                                       /// 00ef0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ef4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ef8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00efc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f20
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f28
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f70
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f74
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f7c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f98
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fa4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fa8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fb0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fb4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fc0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fcc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fd0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fd4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fdc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fe0
			 0x33333333,                                                  // 4 random values                             /// 00fe4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fe8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ff0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ff8
			 0x0e003fff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x3a000c21, /// 0x0
			 0xaec84fda, /// 0x4
			 0x432addb2, /// 0x8
			 0x3f33a510, /// 0xc
			 0x5ce331ce, /// 0x10
			 0x1ca707df, /// 0x14
			 0xc63509ed, /// 0x18
			 0x0ee3ed27, /// 0x1c
			 0x0038ed15, /// 0x20
			 0x45a753a0, /// 0x24
			 0x6df96e83, /// 0x28
			 0x27d6a673, /// 0x2c
			 0xaa136d79, /// 0x30
			 0x381d8608, /// 0x34
			 0x6e6e391c, /// 0x38
			 0x4755a3a8, /// 0x3c
			 0x1c7df941, /// 0x40
			 0x76547113, /// 0x44
			 0xd54da0a4, /// 0x48
			 0x2f1fd6e5, /// 0x4c
			 0x4001750e, /// 0x50
			 0x75018227, /// 0x54
			 0x66ff21b5, /// 0x58
			 0x573dbe9f, /// 0x5c
			 0xf6a31c84, /// 0x60
			 0x5ab90bf9, /// 0x64
			 0x9bdbafe7, /// 0x68
			 0x572f0c6b, /// 0x6c
			 0xc70a18f0, /// 0x70
			 0x4512fc0a, /// 0x74
			 0xff744c45, /// 0x78
			 0x61d117d5, /// 0x7c
			 0x619d7a6f, /// 0x80
			 0x2d5d5f08, /// 0x84
			 0x5cfaab8c, /// 0x88
			 0xea35ed54, /// 0x8c
			 0x4f72cade, /// 0x90
			 0xa7efe583, /// 0x94
			 0x68301efc, /// 0x98
			 0xba755626, /// 0x9c
			 0xc3b424a7, /// 0xa0
			 0x77c9f28e, /// 0xa4
			 0x602995fb, /// 0xa8
			 0x093f84e7, /// 0xac
			 0x90e87911, /// 0xb0
			 0xb49af2d6, /// 0xb4
			 0x595dc9e6, /// 0xb8
			 0x5beebb1a, /// 0xbc
			 0x44f2d868, /// 0xc0
			 0x817751cf, /// 0xc4
			 0x9be87813, /// 0xc8
			 0x1edfccb2, /// 0xcc
			 0x3a132195, /// 0xd0
			 0xb4569c18, /// 0xd4
			 0x39100770, /// 0xd8
			 0x6182fbbe, /// 0xdc
			 0x07d5684c, /// 0xe0
			 0x63a3515e, /// 0xe4
			 0x36e54194, /// 0xe8
			 0x0a8cd8e6, /// 0xec
			 0x4adfd47b, /// 0xf0
			 0x97fedeb2, /// 0xf4
			 0xf09ffc9b, /// 0xf8
			 0x260aa42e, /// 0xfc
			 0xb008247e, /// 0x100
			 0x58e7ed58, /// 0x104
			 0x1a961d22, /// 0x108
			 0x53678364, /// 0x10c
			 0xbee5c3ea, /// 0x110
			 0x5e093bb9, /// 0x114
			 0x315e60fa, /// 0x118
			 0x84fdfa38, /// 0x11c
			 0x93d87984, /// 0x120
			 0xa6a5a22f, /// 0x124
			 0xe2a92e51, /// 0x128
			 0xca3c2df8, /// 0x12c
			 0x2e47442e, /// 0x130
			 0x9faa9d87, /// 0x134
			 0xab8818da, /// 0x138
			 0x1f1779a4, /// 0x13c
			 0x6fe54f9f, /// 0x140
			 0x864c2409, /// 0x144
			 0xeca265cc, /// 0x148
			 0xcc07e37f, /// 0x14c
			 0xd56e3460, /// 0x150
			 0x47565b2d, /// 0x154
			 0x440cc6c4, /// 0x158
			 0x97ad54b6, /// 0x15c
			 0xd7a67bc4, /// 0x160
			 0xaf6015d6, /// 0x164
			 0x3f12a538, /// 0x168
			 0x53e926bc, /// 0x16c
			 0xb84748d1, /// 0x170
			 0xc9a1b716, /// 0x174
			 0xfd646953, /// 0x178
			 0xad3658cd, /// 0x17c
			 0x4954b863, /// 0x180
			 0xca4e10f7, /// 0x184
			 0x709049ec, /// 0x188
			 0xf6f2b85e, /// 0x18c
			 0xe0ba6f5c, /// 0x190
			 0x27e83204, /// 0x194
			 0x5948b519, /// 0x198
			 0x60e8cf9c, /// 0x19c
			 0x2c1d687b, /// 0x1a0
			 0x45d59dd8, /// 0x1a4
			 0x1be5fd11, /// 0x1a8
			 0x6604f4a4, /// 0x1ac
			 0x213084a9, /// 0x1b0
			 0x6678c752, /// 0x1b4
			 0x7b369a0e, /// 0x1b8
			 0x4b8f88c6, /// 0x1bc
			 0x92676c87, /// 0x1c0
			 0x5b1d02a4, /// 0x1c4
			 0x8db74cdc, /// 0x1c8
			 0x9146a8c4, /// 0x1cc
			 0xfda956cd, /// 0x1d0
			 0x5727de2c, /// 0x1d4
			 0x76d4d502, /// 0x1d8
			 0xe61889c4, /// 0x1dc
			 0x8248dd99, /// 0x1e0
			 0x74e895c4, /// 0x1e4
			 0x5373edde, /// 0x1e8
			 0x51461174, /// 0x1ec
			 0x09a051e3, /// 0x1f0
			 0xd885dfc4, /// 0x1f4
			 0x7457d490, /// 0x1f8
			 0xdef6fb59, /// 0x1fc
			 0x6c22af3d, /// 0x200
			 0x38447c8e, /// 0x204
			 0x3b156c24, /// 0x208
			 0xbabc0729, /// 0x20c
			 0x7465fb6d, /// 0x210
			 0x8b5c1edf, /// 0x214
			 0x22cc7354, /// 0x218
			 0x8a38c2e9, /// 0x21c
			 0xc59ef5e5, /// 0x220
			 0x337f058e, /// 0x224
			 0x15a5a1af, /// 0x228
			 0x08397b33, /// 0x22c
			 0x31504f71, /// 0x230
			 0x02dce507, /// 0x234
			 0x40a46a3d, /// 0x238
			 0x7659b69b, /// 0x23c
			 0xf2a2c02f, /// 0x240
			 0xae5a634c, /// 0x244
			 0xf420528f, /// 0x248
			 0x811e7088, /// 0x24c
			 0xcaae7b8d, /// 0x250
			 0x44914f93, /// 0x254
			 0x66ac547f, /// 0x258
			 0xbfe84e94, /// 0x25c
			 0xb413514e, /// 0x260
			 0x13e1d6ba, /// 0x264
			 0xff229450, /// 0x268
			 0xd7e394aa, /// 0x26c
			 0x61ae944a, /// 0x270
			 0x5dd53613, /// 0x274
			 0xa032d2f3, /// 0x278
			 0xf6519e0c, /// 0x27c
			 0xe191aef9, /// 0x280
			 0x5dfe5a2c, /// 0x284
			 0x82d7c324, /// 0x288
			 0x32704199, /// 0x28c
			 0xf0c31da9, /// 0x290
			 0xdd122112, /// 0x294
			 0x3b7d6baf, /// 0x298
			 0xdf749ac1, /// 0x29c
			 0x566437ff, /// 0x2a0
			 0xe4ef3b45, /// 0x2a4
			 0xded78d09, /// 0x2a8
			 0x8ed5fe84, /// 0x2ac
			 0xd693cffd, /// 0x2b0
			 0xc5f25d46, /// 0x2b4
			 0xc870b6fc, /// 0x2b8
			 0xa8daba34, /// 0x2bc
			 0x4611a984, /// 0x2c0
			 0xbfbb4e6d, /// 0x2c4
			 0x72ccd9d9, /// 0x2c8
			 0x8414d528, /// 0x2cc
			 0xf44451ee, /// 0x2d0
			 0xae777840, /// 0x2d4
			 0x92d8fabb, /// 0x2d8
			 0x66bd8bd7, /// 0x2dc
			 0x74556bc9, /// 0x2e0
			 0xd420897d, /// 0x2e4
			 0x7f8dab29, /// 0x2e8
			 0x0ecd8ff3, /// 0x2ec
			 0xa73a7493, /// 0x2f0
			 0x17d368e1, /// 0x2f4
			 0x5e287544, /// 0x2f8
			 0xb520b14b, /// 0x2fc
			 0xdf4ff748, /// 0x300
			 0xdde00807, /// 0x304
			 0x2a745ac0, /// 0x308
			 0x8078abb8, /// 0x30c
			 0xb861733d, /// 0x310
			 0x96504679, /// 0x314
			 0xb8d0fb2f, /// 0x318
			 0x6e3425ab, /// 0x31c
			 0x3ee8a19e, /// 0x320
			 0x9ce94985, /// 0x324
			 0xf1441b15, /// 0x328
			 0xe7dbcdbf, /// 0x32c
			 0x898195ac, /// 0x330
			 0x644866dc, /// 0x334
			 0x72042ddd, /// 0x338
			 0x0f507d4d, /// 0x33c
			 0xf1108c22, /// 0x340
			 0xb820b834, /// 0x344
			 0x2af689bf, /// 0x348
			 0x531c025e, /// 0x34c
			 0x880c848a, /// 0x350
			 0xc7744489, /// 0x354
			 0x62fdcda4, /// 0x358
			 0x8695c08f, /// 0x35c
			 0x139f1eae, /// 0x360
			 0x9b8440bc, /// 0x364
			 0x03ecc2ea, /// 0x368
			 0xacbf8eec, /// 0x36c
			 0xda55df07, /// 0x370
			 0x5bddeec1, /// 0x374
			 0x009bd2aa, /// 0x378
			 0x7e5a0394, /// 0x37c
			 0x0bb3869f, /// 0x380
			 0x10238613, /// 0x384
			 0xe13f9f9d, /// 0x388
			 0x636401f7, /// 0x38c
			 0xffdf5584, /// 0x390
			 0x068695d7, /// 0x394
			 0x16794882, /// 0x398
			 0xc7c60288, /// 0x39c
			 0x1796c879, /// 0x3a0
			 0x3a6de98d, /// 0x3a4
			 0x9b866089, /// 0x3a8
			 0x5b9a87b7, /// 0x3ac
			 0x0816b073, /// 0x3b0
			 0xf55cfdaa, /// 0x3b4
			 0xd1f4c7ac, /// 0x3b8
			 0x7081c22b, /// 0x3bc
			 0xb3affe8c, /// 0x3c0
			 0x51384ea9, /// 0x3c4
			 0x54e7500c, /// 0x3c8
			 0xf23c2855, /// 0x3cc
			 0xc0e8669c, /// 0x3d0
			 0xdbc7b7ee, /// 0x3d4
			 0xcc7f26e0, /// 0x3d8
			 0x0abe79e5, /// 0x3dc
			 0x65185c8a, /// 0x3e0
			 0x5664dcab, /// 0x3e4
			 0xbc1139a0, /// 0x3e8
			 0xa49f6edd, /// 0x3ec
			 0xded4aa11, /// 0x3f0
			 0x33364dd4, /// 0x3f4
			 0x69d1bf1e, /// 0x3f8
			 0x438bed92, /// 0x3fc
			 0x157ff1b6, /// 0x400
			 0xaec074cc, /// 0x404
			 0x160aa9e7, /// 0x408
			 0x80adaf43, /// 0x40c
			 0x7ed6d994, /// 0x410
			 0x28d8409d, /// 0x414
			 0xe253e22a, /// 0x418
			 0xc6432045, /// 0x41c
			 0x6a951cf5, /// 0x420
			 0x510aaa5b, /// 0x424
			 0x9311cfbf, /// 0x428
			 0x3de3aa9c, /// 0x42c
			 0xebcb0ded, /// 0x430
			 0xdae092da, /// 0x434
			 0xd1d5cead, /// 0x438
			 0xe0277758, /// 0x43c
			 0x907301dd, /// 0x440
			 0x1e1c1d51, /// 0x444
			 0x86ecce69, /// 0x448
			 0x554cba87, /// 0x44c
			 0xbcf69842, /// 0x450
			 0xee225cb2, /// 0x454
			 0xfedaf382, /// 0x458
			 0x42d6a005, /// 0x45c
			 0x2e60fc36, /// 0x460
			 0x99567a1d, /// 0x464
			 0x178b62cb, /// 0x468
			 0x95187bec, /// 0x46c
			 0x65048143, /// 0x470
			 0x9e3fedc8, /// 0x474
			 0x17f0797b, /// 0x478
			 0xfd188cd4, /// 0x47c
			 0xd9133381, /// 0x480
			 0x05df0003, /// 0x484
			 0xd19329c0, /// 0x488
			 0x96e3f952, /// 0x48c
			 0x4b94262e, /// 0x490
			 0x0f558c8f, /// 0x494
			 0x0d4496e1, /// 0x498
			 0x55cac6b5, /// 0x49c
			 0x5964f274, /// 0x4a0
			 0x276626ca, /// 0x4a4
			 0xefebadc3, /// 0x4a8
			 0x8622af8f, /// 0x4ac
			 0x24e74bdc, /// 0x4b0
			 0x8cdc8d2f, /// 0x4b4
			 0x8f64cc04, /// 0x4b8
			 0x8d26d1b0, /// 0x4bc
			 0x1fd9e30c, /// 0x4c0
			 0x63b5ca45, /// 0x4c4
			 0xd705ec77, /// 0x4c8
			 0x67202a93, /// 0x4cc
			 0x5e573415, /// 0x4d0
			 0xf33b83f9, /// 0x4d4
			 0xcfb53b6b, /// 0x4d8
			 0x23e07d79, /// 0x4dc
			 0x02ee691a, /// 0x4e0
			 0x3b55b59a, /// 0x4e4
			 0xf3d335e3, /// 0x4e8
			 0xcfe9f853, /// 0x4ec
			 0x9c164967, /// 0x4f0
			 0x493b5344, /// 0x4f4
			 0x57c0ed82, /// 0x4f8
			 0xf55db9b8, /// 0x4fc
			 0x3623dd09, /// 0x500
			 0x8a4c4d06, /// 0x504
			 0x74c65de9, /// 0x508
			 0x4a1849bf, /// 0x50c
			 0xc49dfe13, /// 0x510
			 0xd9e5bbb6, /// 0x514
			 0xcb98a019, /// 0x518
			 0xc7e98026, /// 0x51c
			 0xb8962fb8, /// 0x520
			 0xfe15aaa3, /// 0x524
			 0xc624d474, /// 0x528
			 0xac6358e0, /// 0x52c
			 0x4fb8fcda, /// 0x530
			 0x21ad0971, /// 0x534
			 0x46f49b3f, /// 0x538
			 0xadb85191, /// 0x53c
			 0x4f49001f, /// 0x540
			 0xa6f52961, /// 0x544
			 0xba4ac769, /// 0x548
			 0x903f8725, /// 0x54c
			 0x7808dc8f, /// 0x550
			 0x87dfdbb1, /// 0x554
			 0xe16ccaba, /// 0x558
			 0x27cc4ef3, /// 0x55c
			 0x53a0264e, /// 0x560
			 0xd1c63f27, /// 0x564
			 0xad027eb8, /// 0x568
			 0x69d47638, /// 0x56c
			 0xd0a70e9d, /// 0x570
			 0x8b0dc56d, /// 0x574
			 0xa2a2c729, /// 0x578
			 0xac949006, /// 0x57c
			 0x3b302a7d, /// 0x580
			 0xfc303bef, /// 0x584
			 0x29681bd2, /// 0x588
			 0xd6df7e72, /// 0x58c
			 0x43daace4, /// 0x590
			 0x50898482, /// 0x594
			 0xb9c36de1, /// 0x598
			 0xde793eeb, /// 0x59c
			 0x6fbfcb51, /// 0x5a0
			 0xe94d501d, /// 0x5a4
			 0x30b29f7f, /// 0x5a8
			 0xfb78fbe0, /// 0x5ac
			 0xf8a72817, /// 0x5b0
			 0xa0b339ee, /// 0x5b4
			 0x8209daab, /// 0x5b8
			 0x3643a033, /// 0x5bc
			 0xadc24abc, /// 0x5c0
			 0x6a7c5bfa, /// 0x5c4
			 0x977f8c0c, /// 0x5c8
			 0xb162ea26, /// 0x5cc
			 0xe010c770, /// 0x5d0
			 0x0e4d7841, /// 0x5d4
			 0x6c81ec00, /// 0x5d8
			 0x81ec109f, /// 0x5dc
			 0x8af55ae9, /// 0x5e0
			 0xc2259892, /// 0x5e4
			 0x05a8f79f, /// 0x5e8
			 0x6fdde388, /// 0x5ec
			 0x38380c61, /// 0x5f0
			 0x5754e059, /// 0x5f4
			 0x68986ead, /// 0x5f8
			 0x47e5958b, /// 0x5fc
			 0xf426d74b, /// 0x600
			 0x1d353779, /// 0x604
			 0xf8976a73, /// 0x608
			 0x75eae18d, /// 0x60c
			 0x6552954c, /// 0x610
			 0xf6ad840e, /// 0x614
			 0xa64a106b, /// 0x618
			 0xcda040d2, /// 0x61c
			 0x7f69d3b3, /// 0x620
			 0xa310f7df, /// 0x624
			 0x44ed6087, /// 0x628
			 0xe21356b3, /// 0x62c
			 0xc59aa8ac, /// 0x630
			 0x6e67e878, /// 0x634
			 0x9886920e, /// 0x638
			 0x5f7bdd25, /// 0x63c
			 0xf3ceb759, /// 0x640
			 0x630c94a8, /// 0x644
			 0xde48c631, /// 0x648
			 0x1df752ea, /// 0x64c
			 0x76eb8813, /// 0x650
			 0x14623d3c, /// 0x654
			 0x92039b89, /// 0x658
			 0xd5614a85, /// 0x65c
			 0xdeb8153f, /// 0x660
			 0x365eeadd, /// 0x664
			 0xf9b125ad, /// 0x668
			 0xa947884c, /// 0x66c
			 0xb61442ec, /// 0x670
			 0x7d977780, /// 0x674
			 0x11392162, /// 0x678
			 0xbf063ca6, /// 0x67c
			 0xc0268694, /// 0x680
			 0x8c423c4d, /// 0x684
			 0xe2135832, /// 0x688
			 0x6272c692, /// 0x68c
			 0x0174adb2, /// 0x690
			 0x9865824a, /// 0x694
			 0xcc7ce492, /// 0x698
			 0xee45b738, /// 0x69c
			 0x8aadda6b, /// 0x6a0
			 0x6ffebe50, /// 0x6a4
			 0x94185143, /// 0x6a8
			 0x68098b8e, /// 0x6ac
			 0xac5e4b1a, /// 0x6b0
			 0x8f911ba4, /// 0x6b4
			 0x725e1706, /// 0x6b8
			 0x270cea79, /// 0x6bc
			 0x8282ca90, /// 0x6c0
			 0x505f1d87, /// 0x6c4
			 0x284f8221, /// 0x6c8
			 0x1d03e417, /// 0x6cc
			 0xdd6ad772, /// 0x6d0
			 0xcdc693ef, /// 0x6d4
			 0xc26694c9, /// 0x6d8
			 0x1a80470e, /// 0x6dc
			 0x9fe4515c, /// 0x6e0
			 0xd949f048, /// 0x6e4
			 0xc2f973e6, /// 0x6e8
			 0x9d0f697d, /// 0x6ec
			 0x61f939c7, /// 0x6f0
			 0x5718ca73, /// 0x6f4
			 0x52316eee, /// 0x6f8
			 0xd8b57c93, /// 0x6fc
			 0x4610b635, /// 0x700
			 0x8c8ad2af, /// 0x704
			 0x9c8d199e, /// 0x708
			 0x7c4ceab6, /// 0x70c
			 0xfd5acadb, /// 0x710
			 0x516de24c, /// 0x714
			 0xe3dbbd3e, /// 0x718
			 0x39a961fd, /// 0x71c
			 0x8247b2b8, /// 0x720
			 0xe6abe989, /// 0x724
			 0x83381ba8, /// 0x728
			 0x098df22a, /// 0x72c
			 0x285237a7, /// 0x730
			 0x35be7383, /// 0x734
			 0xbd4d8450, /// 0x738
			 0x58588bd6, /// 0x73c
			 0x3fa1472e, /// 0x740
			 0xf0b3c1d2, /// 0x744
			 0xc1970069, /// 0x748
			 0x7e47f1db, /// 0x74c
			 0xdffba807, /// 0x750
			 0x6f8eb420, /// 0x754
			 0x37b7860a, /// 0x758
			 0x083a5aa0, /// 0x75c
			 0x8c46c2aa, /// 0x760
			 0x6acc96eb, /// 0x764
			 0xf9bfb37d, /// 0x768
			 0xe59a7f52, /// 0x76c
			 0x9badb0dc, /// 0x770
			 0xa0a66919, /// 0x774
			 0xb012982a, /// 0x778
			 0x1cc5eeeb, /// 0x77c
			 0xdd177812, /// 0x780
			 0xb3b10adb, /// 0x784
			 0x3dfe2823, /// 0x788
			 0x4f010040, /// 0x78c
			 0xcedd1bce, /// 0x790
			 0x6ffe7a86, /// 0x794
			 0xefa6b3e4, /// 0x798
			 0xd632c7cf, /// 0x79c
			 0x7a21bd52, /// 0x7a0
			 0x4d22724a, /// 0x7a4
			 0x93457661, /// 0x7a8
			 0x8b400e60, /// 0x7ac
			 0xb58793c5, /// 0x7b0
			 0xdc824cdd, /// 0x7b4
			 0xfb0a6ed2, /// 0x7b8
			 0x5de5af2b, /// 0x7bc
			 0xaf4d06f1, /// 0x7c0
			 0xf281711d, /// 0x7c4
			 0x7bca129f, /// 0x7c8
			 0xee83c862, /// 0x7cc
			 0xc0381392, /// 0x7d0
			 0xab66b19d, /// 0x7d4
			 0x9b72cbbc, /// 0x7d8
			 0x75449a88, /// 0x7dc
			 0xdedc9b77, /// 0x7e0
			 0x99c72b7c, /// 0x7e4
			 0x964de83c, /// 0x7e8
			 0x6fe29c6c, /// 0x7ec
			 0xd4bb8883, /// 0x7f0
			 0x67e3f3db, /// 0x7f4
			 0xf79dec73, /// 0x7f8
			 0xa52ee791, /// 0x7fc
			 0x23fc556a, /// 0x800
			 0x8094e06e, /// 0x804
			 0x762c86fe, /// 0x808
			 0x5cdb0585, /// 0x80c
			 0x2525426b, /// 0x810
			 0x107bd43f, /// 0x814
			 0xef331fbd, /// 0x818
			 0xe344701f, /// 0x81c
			 0xb70da673, /// 0x820
			 0x095fda07, /// 0x824
			 0x00e80621, /// 0x828
			 0x35946615, /// 0x82c
			 0x5ec79a88, /// 0x830
			 0x79b0176b, /// 0x834
			 0x342f67a0, /// 0x838
			 0xfa6fd7ad, /// 0x83c
			 0xb9ec5cb9, /// 0x840
			 0xadd4021f, /// 0x844
			 0xd63140d6, /// 0x848
			 0x4142938d, /// 0x84c
			 0xd79b576a, /// 0x850
			 0x9265f61d, /// 0x854
			 0x31cdbb3f, /// 0x858
			 0x9db9271c, /// 0x85c
			 0x6a24b63c, /// 0x860
			 0x1e1bf39c, /// 0x864
			 0x6acd0aff, /// 0x868
			 0x40190afd, /// 0x86c
			 0x4604988f, /// 0x870
			 0x393f2c63, /// 0x874
			 0x9290f086, /// 0x878
			 0x0cf61bdf, /// 0x87c
			 0x2109cd39, /// 0x880
			 0x686c0115, /// 0x884
			 0x9b7459df, /// 0x888
			 0x103ab61b, /// 0x88c
			 0xf3302783, /// 0x890
			 0x1cdb3dac, /// 0x894
			 0x300025a3, /// 0x898
			 0xa876c0b2, /// 0x89c
			 0x006bb21f, /// 0x8a0
			 0x08d0809b, /// 0x8a4
			 0x6972db03, /// 0x8a8
			 0x3fd9b4d3, /// 0x8ac
			 0x734ca2d6, /// 0x8b0
			 0x4051fc5a, /// 0x8b4
			 0xda7144a4, /// 0x8b8
			 0xc18b8b6e, /// 0x8bc
			 0x7979f7f9, /// 0x8c0
			 0x505959eb, /// 0x8c4
			 0x1f28cb62, /// 0x8c8
			 0x324c7b6b, /// 0x8cc
			 0xc6904bb3, /// 0x8d0
			 0x49674198, /// 0x8d4
			 0x61b7488c, /// 0x8d8
			 0x0a260461, /// 0x8dc
			 0x5d086470, /// 0x8e0
			 0x2a3615b1, /// 0x8e4
			 0x3865ed94, /// 0x8e8
			 0xe272885d, /// 0x8ec
			 0x92516f81, /// 0x8f0
			 0xef2469bb, /// 0x8f4
			 0x4802d620, /// 0x8f8
			 0xae4e6efa, /// 0x8fc
			 0xff4c65cd, /// 0x900
			 0x8054cffe, /// 0x904
			 0x79774451, /// 0x908
			 0xea5118dd, /// 0x90c
			 0x4cf102e1, /// 0x910
			 0x9c91f02f, /// 0x914
			 0x48f1c633, /// 0x918
			 0xd20cc589, /// 0x91c
			 0x88d5d7a2, /// 0x920
			 0x44d7bcfb, /// 0x924
			 0x4dc81c95, /// 0x928
			 0xf1d804ae, /// 0x92c
			 0x6c1bc986, /// 0x930
			 0xfa126fab, /// 0x934
			 0x7abb3a35, /// 0x938
			 0x9b1cb094, /// 0x93c
			 0x76c1e0f2, /// 0x940
			 0xdd4b678f, /// 0x944
			 0x61d14bd0, /// 0x948
			 0xf0e615b7, /// 0x94c
			 0x4d111858, /// 0x950
			 0x31fe8455, /// 0x954
			 0xd2b0bff9, /// 0x958
			 0x84a7f6c2, /// 0x95c
			 0xe59343b9, /// 0x960
			 0xdac25f59, /// 0x964
			 0x7515a395, /// 0x968
			 0xa6522a52, /// 0x96c
			 0x57dbdb25, /// 0x970
			 0x80db08f2, /// 0x974
			 0xacffeb57, /// 0x978
			 0xa6b8b7f8, /// 0x97c
			 0xb6ff8fea, /// 0x980
			 0x512f6ef4, /// 0x984
			 0x98688c89, /// 0x988
			 0xf7405680, /// 0x98c
			 0x20ca3f45, /// 0x990
			 0xd1f077f4, /// 0x994
			 0x5aeec51b, /// 0x998
			 0x0352bbae, /// 0x99c
			 0x976c4498, /// 0x9a0
			 0x2be76dfc, /// 0x9a4
			 0x37f26057, /// 0x9a8
			 0x8f7d9c2b, /// 0x9ac
			 0xf9f51594, /// 0x9b0
			 0x048b6a4f, /// 0x9b4
			 0x4570b408, /// 0x9b8
			 0xae1481c4, /// 0x9bc
			 0x46e147a3, /// 0x9c0
			 0x2db93c20, /// 0x9c4
			 0xd4919e11, /// 0x9c8
			 0x80205490, /// 0x9cc
			 0x925432ef, /// 0x9d0
			 0x483563bb, /// 0x9d4
			 0x5b145edc, /// 0x9d8
			 0x99f1a4b4, /// 0x9dc
			 0x5bbdcf25, /// 0x9e0
			 0x4fe49152, /// 0x9e4
			 0x4487174a, /// 0x9e8
			 0xcb281f18, /// 0x9ec
			 0x1c01229a, /// 0x9f0
			 0xe594208d, /// 0x9f4
			 0x7ff36790, /// 0x9f8
			 0x0f08415f, /// 0x9fc
			 0xe5b633b0, /// 0xa00
			 0x37f597da, /// 0xa04
			 0xbfae23b6, /// 0xa08
			 0xd528f0cd, /// 0xa0c
			 0x4f3fa1f3, /// 0xa10
			 0x73035ac3, /// 0xa14
			 0xb0aaa2bc, /// 0xa18
			 0xe6abf0d3, /// 0xa1c
			 0x849b6557, /// 0xa20
			 0x0808f85c, /// 0xa24
			 0xb3f374bd, /// 0xa28
			 0x7fd20fdc, /// 0xa2c
			 0x2b24cccd, /// 0xa30
			 0xefcb75c5, /// 0xa34
			 0x7b7d4efe, /// 0xa38
			 0xfd5177bc, /// 0xa3c
			 0x2c3659d5, /// 0xa40
			 0x7404b7e1, /// 0xa44
			 0xf3c55d24, /// 0xa48
			 0x57a7397c, /// 0xa4c
			 0x17a903bc, /// 0xa50
			 0x846435cb, /// 0xa54
			 0xcef89138, /// 0xa58
			 0x4fc70638, /// 0xa5c
			 0xcc746eb7, /// 0xa60
			 0x947d22b9, /// 0xa64
			 0xa28e6a78, /// 0xa68
			 0xab865051, /// 0xa6c
			 0xe32c6c19, /// 0xa70
			 0x61cf9bd9, /// 0xa74
			 0x469de75a, /// 0xa78
			 0x4bafa2e0, /// 0xa7c
			 0xd03b7f97, /// 0xa80
			 0x0f677bd3, /// 0xa84
			 0xb109522f, /// 0xa88
			 0xdab1b10a, /// 0xa8c
			 0xe0850c66, /// 0xa90
			 0x6e289bff, /// 0xa94
			 0xc38400fa, /// 0xa98
			 0x5c322908, /// 0xa9c
			 0xa9ee6963, /// 0xaa0
			 0x8c2211bb, /// 0xaa4
			 0x358a70cf, /// 0xaa8
			 0x4d99ceee, /// 0xaac
			 0x6fae38a4, /// 0xab0
			 0x5566f136, /// 0xab4
			 0x65ca963f, /// 0xab8
			 0x04f8e3bc, /// 0xabc
			 0x6573ae55, /// 0xac0
			 0x77aacfa0, /// 0xac4
			 0xad807ee3, /// 0xac8
			 0x16f592a1, /// 0xacc
			 0xa325c7f0, /// 0xad0
			 0x752523d8, /// 0xad4
			 0x3830bdd8, /// 0xad8
			 0x72d639e5, /// 0xadc
			 0xb8f7696c, /// 0xae0
			 0x83e9020d, /// 0xae4
			 0x1a60d67f, /// 0xae8
			 0x44378f8b, /// 0xaec
			 0x363c084a, /// 0xaf0
			 0xef83d85d, /// 0xaf4
			 0xb039995d, /// 0xaf8
			 0x9439f981, /// 0xafc
			 0xab069363, /// 0xb00
			 0x6fe70a28, /// 0xb04
			 0x9bdea193, /// 0xb08
			 0xd52aa0ee, /// 0xb0c
			 0x2afa5fdd, /// 0xb10
			 0x9323d358, /// 0xb14
			 0x77213d88, /// 0xb18
			 0x387f7644, /// 0xb1c
			 0x992516c4, /// 0xb20
			 0x71a6d28b, /// 0xb24
			 0x035e196b, /// 0xb28
			 0x79169362, /// 0xb2c
			 0xa49aafcc, /// 0xb30
			 0x051646d3, /// 0xb34
			 0xc3b15c01, /// 0xb38
			 0x06dcb7a4, /// 0xb3c
			 0x3e5d0518, /// 0xb40
			 0x156deb93, /// 0xb44
			 0xe0a3f242, /// 0xb48
			 0x8ff42857, /// 0xb4c
			 0x85ae45d4, /// 0xb50
			 0x5df73da8, /// 0xb54
			 0x2296313e, /// 0xb58
			 0x6338ba03, /// 0xb5c
			 0x05115893, /// 0xb60
			 0xc2772170, /// 0xb64
			 0x7950b64f, /// 0xb68
			 0x34340da4, /// 0xb6c
			 0x28abb46c, /// 0xb70
			 0xef4cddb2, /// 0xb74
			 0xb259364f, /// 0xb78
			 0x1482a7db, /// 0xb7c
			 0x15dd12c4, /// 0xb80
			 0xd298b627, /// 0xb84
			 0x122e91b1, /// 0xb88
			 0x60dbf152, /// 0xb8c
			 0x7428cf17, /// 0xb90
			 0x7b74f314, /// 0xb94
			 0x9be5ef13, /// 0xb98
			 0x7cbb3d8e, /// 0xb9c
			 0x632752eb, /// 0xba0
			 0x34bf4e9d, /// 0xba4
			 0x272af72b, /// 0xba8
			 0x43402fbe, /// 0xbac
			 0x81460aaa, /// 0xbb0
			 0xd7e36364, /// 0xbb4
			 0x60ab9cc5, /// 0xbb8
			 0x06fffe9c, /// 0xbbc
			 0x7c896318, /// 0xbc0
			 0x952ff5fe, /// 0xbc4
			 0x94d6bce1, /// 0xbc8
			 0x6769387b, /// 0xbcc
			 0x8ea62f29, /// 0xbd0
			 0xa71189c5, /// 0xbd4
			 0xd81bc4dc, /// 0xbd8
			 0x02a27d1b, /// 0xbdc
			 0x50b09d96, /// 0xbe0
			 0x1ddcfd7a, /// 0xbe4
			 0xe708ffcd, /// 0xbe8
			 0xce3cbbfa, /// 0xbec
			 0xa07e250d, /// 0xbf0
			 0x103a4b7c, /// 0xbf4
			 0x06670831, /// 0xbf8
			 0xa4169264, /// 0xbfc
			 0x27ebfe0d, /// 0xc00
			 0x8bbd117f, /// 0xc04
			 0x4a202047, /// 0xc08
			 0xa86764c6, /// 0xc0c
			 0x48285f1f, /// 0xc10
			 0x499c4a73, /// 0xc14
			 0x643b7670, /// 0xc18
			 0x2e306364, /// 0xc1c
			 0x0df65fc7, /// 0xc20
			 0x82c88cd0, /// 0xc24
			 0xeffd4079, /// 0xc28
			 0x37364274, /// 0xc2c
			 0xb4413dac, /// 0xc30
			 0x685f4532, /// 0xc34
			 0x9d261c73, /// 0xc38
			 0x59f68af7, /// 0xc3c
			 0xd4f7d8dc, /// 0xc40
			 0x5ab28a32, /// 0xc44
			 0xa336eea8, /// 0xc48
			 0x670b8626, /// 0xc4c
			 0x365e0cae, /// 0xc50
			 0xd42a5eb6, /// 0xc54
			 0x333c9c4d, /// 0xc58
			 0x10a6e274, /// 0xc5c
			 0x071e82d6, /// 0xc60
			 0x8f23a59b, /// 0xc64
			 0xecc79526, /// 0xc68
			 0xed934daf, /// 0xc6c
			 0x19841682, /// 0xc70
			 0xdf06edfe, /// 0xc74
			 0x56af051c, /// 0xc78
			 0xfc93c103, /// 0xc7c
			 0x5a955181, /// 0xc80
			 0xb71595b7, /// 0xc84
			 0x35c18c07, /// 0xc88
			 0xd8ba90c2, /// 0xc8c
			 0x2b00b5fc, /// 0xc90
			 0xe17bd424, /// 0xc94
			 0x6fcc1915, /// 0xc98
			 0x1fedf84a, /// 0xc9c
			 0x7a1d30f5, /// 0xca0
			 0x87ddbf45, /// 0xca4
			 0x7d21586c, /// 0xca8
			 0xdda21fd4, /// 0xcac
			 0xdcfd1217, /// 0xcb0
			 0xc1a34319, /// 0xcb4
			 0x0b85750c, /// 0xcb8
			 0x1ef51e5b, /// 0xcbc
			 0x6045055c, /// 0xcc0
			 0x104242d1, /// 0xcc4
			 0x90614e2f, /// 0xcc8
			 0x7509fd2f, /// 0xccc
			 0x6749b3da, /// 0xcd0
			 0x224500b6, /// 0xcd4
			 0xdbe6556e, /// 0xcd8
			 0xc19e30e1, /// 0xcdc
			 0x3fe75771, /// 0xce0
			 0x0c05c6e5, /// 0xce4
			 0x18c90eee, /// 0xce8
			 0xfe15d634, /// 0xcec
			 0xe0a7660b, /// 0xcf0
			 0x833e2da4, /// 0xcf4
			 0xb21b00fb, /// 0xcf8
			 0xf9c06a19, /// 0xcfc
			 0x42e67f36, /// 0xd00
			 0x876e8b54, /// 0xd04
			 0xe2809a35, /// 0xd08
			 0x0cabe830, /// 0xd0c
			 0x1bc60603, /// 0xd10
			 0xd2feacb7, /// 0xd14
			 0x2536475d, /// 0xd18
			 0xd296d283, /// 0xd1c
			 0x3811ec2f, /// 0xd20
			 0x7ba9c9bb, /// 0xd24
			 0x9241ba5f, /// 0xd28
			 0xd9e5c10f, /// 0xd2c
			 0xcb8f9034, /// 0xd30
			 0xbb7af271, /// 0xd34
			 0x5e5c6e1e, /// 0xd38
			 0x5393864a, /// 0xd3c
			 0x7a7b8ec2, /// 0xd40
			 0xf4d90e9d, /// 0xd44
			 0x464d899b, /// 0xd48
			 0xe45fdba4, /// 0xd4c
			 0xdcf3a3bf, /// 0xd50
			 0x1a114019, /// 0xd54
			 0x617f6ee4, /// 0xd58
			 0x0db47fe1, /// 0xd5c
			 0xaaf4a3ad, /// 0xd60
			 0x78127740, /// 0xd64
			 0xe96c7053, /// 0xd68
			 0x5ad1c42b, /// 0xd6c
			 0x44ea078b, /// 0xd70
			 0xa0d170c6, /// 0xd74
			 0x0b140938, /// 0xd78
			 0x588eac5e, /// 0xd7c
			 0x1a66bb3e, /// 0xd80
			 0x4fec06da, /// 0xd84
			 0x22b7197f, /// 0xd88
			 0xdbfe2d70, /// 0xd8c
			 0x9ddbd131, /// 0xd90
			 0xf307a9c8, /// 0xd94
			 0xe1d7b2d9, /// 0xd98
			 0xa12c60ff, /// 0xd9c
			 0xd19c8ae6, /// 0xda0
			 0x75c98b1a, /// 0xda4
			 0x3f4e5dd4, /// 0xda8
			 0xf3576d72, /// 0xdac
			 0xd2cd3432, /// 0xdb0
			 0xfb2ad474, /// 0xdb4
			 0xd59fe167, /// 0xdb8
			 0x6cce9da4, /// 0xdbc
			 0xb7a18971, /// 0xdc0
			 0xab524d4a, /// 0xdc4
			 0xdf0cdf7c, /// 0xdc8
			 0xc50db54b, /// 0xdcc
			 0x8f3d7f80, /// 0xdd0
			 0xa9dd6b77, /// 0xdd4
			 0xfc166b30, /// 0xdd8
			 0x4b1342ae, /// 0xddc
			 0x964eae00, /// 0xde0
			 0xaf847b57, /// 0xde4
			 0x86b63298, /// 0xde8
			 0x9f4f89d6, /// 0xdec
			 0x1bd4351a, /// 0xdf0
			 0x7f9ca2f4, /// 0xdf4
			 0xce9ea499, /// 0xdf8
			 0x85048f66, /// 0xdfc
			 0x0b219ab6, /// 0xe00
			 0x6372178f, /// 0xe04
			 0x52dcbb2a, /// 0xe08
			 0x8e21677c, /// 0xe0c
			 0x0a54ba72, /// 0xe10
			 0x0b1b46d8, /// 0xe14
			 0x751221f7, /// 0xe18
			 0xd324810c, /// 0xe1c
			 0x4c56a281, /// 0xe20
			 0xcea1d0f3, /// 0xe24
			 0xbf0923cb, /// 0xe28
			 0x116a21de, /// 0xe2c
			 0x2fcc4806, /// 0xe30
			 0x77f42586, /// 0xe34
			 0xbb1ba392, /// 0xe38
			 0xdb686cb0, /// 0xe3c
			 0x020cf4a4, /// 0xe40
			 0x3d3232ba, /// 0xe44
			 0x8b8f2d70, /// 0xe48
			 0xb1363041, /// 0xe4c
			 0xfad2451f, /// 0xe50
			 0x4eb913e9, /// 0xe54
			 0x403561c6, /// 0xe58
			 0xdb815322, /// 0xe5c
			 0xf25b7764, /// 0xe60
			 0xdc321b14, /// 0xe64
			 0xb74246b6, /// 0xe68
			 0x82030ca1, /// 0xe6c
			 0x33370c7d, /// 0xe70
			 0x6496b030, /// 0xe74
			 0xfc9420d2, /// 0xe78
			 0x6a3823c0, /// 0xe7c
			 0x29c6b682, /// 0xe80
			 0xff08511a, /// 0xe84
			 0xbfb8bc79, /// 0xe88
			 0x1aaa1a9c, /// 0xe8c
			 0xea99f4ff, /// 0xe90
			 0xe3427beb, /// 0xe94
			 0xa220165b, /// 0xe98
			 0xe29a25e1, /// 0xe9c
			 0xbd5e9b0f, /// 0xea0
			 0xc4111672, /// 0xea4
			 0xa47f030a, /// 0xea8
			 0xc3ea8a1d, /// 0xeac
			 0xe4002bcb, /// 0xeb0
			 0xbb934b19, /// 0xeb4
			 0x0c7f278c, /// 0xeb8
			 0xf1db0304, /// 0xebc
			 0xf9da3b5f, /// 0xec0
			 0x391fc526, /// 0xec4
			 0xeeab6316, /// 0xec8
			 0x6f16db18, /// 0xecc
			 0x8eeea754, /// 0xed0
			 0x301abdaa, /// 0xed4
			 0xf1da5553, /// 0xed8
			 0xd7c3b595, /// 0xedc
			 0xd8e8c67a, /// 0xee0
			 0x91a98b58, /// 0xee4
			 0xe3ee5b89, /// 0xee8
			 0xeea97622, /// 0xeec
			 0x2e946744, /// 0xef0
			 0xeaa0ca48, /// 0xef4
			 0x5fdda0a7, /// 0xef8
			 0xc14694ae, /// 0xefc
			 0x9213eaf9, /// 0xf00
			 0x01c1e1f4, /// 0xf04
			 0xb9ab6554, /// 0xf08
			 0x3d5d1d8c, /// 0xf0c
			 0x1f85ec79, /// 0xf10
			 0xcb80b6e8, /// 0xf14
			 0x28f63cd7, /// 0xf18
			 0xae3faaed, /// 0xf1c
			 0x4c42e812, /// 0xf20
			 0x64899cb1, /// 0xf24
			 0xa707e68d, /// 0xf28
			 0x49f57ec0, /// 0xf2c
			 0x0e0b3571, /// 0xf30
			 0xd8c461a4, /// 0xf34
			 0x3d4e28d8, /// 0xf38
			 0x3c40eb91, /// 0xf3c
			 0xc396fceb, /// 0xf40
			 0xbf9303c4, /// 0xf44
			 0x5679effc, /// 0xf48
			 0xc68e5821, /// 0xf4c
			 0xaed85f50, /// 0xf50
			 0xb697a0cd, /// 0xf54
			 0xc4ba35f6, /// 0xf58
			 0x2f54aaa6, /// 0xf5c
			 0x7873195e, /// 0xf60
			 0x1a5da9c1, /// 0xf64
			 0xe0c0cd03, /// 0xf68
			 0xfbae4fe4, /// 0xf6c
			 0xa92eb5fb, /// 0xf70
			 0x523fd7be, /// 0xf74
			 0xef1e493b, /// 0xf78
			 0x9c05187d, /// 0xf7c
			 0x364804df, /// 0xf80
			 0x8a0d3dbf, /// 0xf84
			 0xca06f1d3, /// 0xf88
			 0xad4c63bd, /// 0xf8c
			 0x26184e82, /// 0xf90
			 0x4aab7ab8, /// 0xf94
			 0x8b0cf31d, /// 0xf98
			 0xfa14c363, /// 0xf9c
			 0x274818f3, /// 0xfa0
			 0x2824c972, /// 0xfa4
			 0xbc05a3c7, /// 0xfa8
			 0x706c6aec, /// 0xfac
			 0xd9416ce7, /// 0xfb0
			 0x984dcdaa, /// 0xfb4
			 0xa0e52df7, /// 0xfb8
			 0x59385232, /// 0xfbc
			 0xcb687fe2, /// 0xfc0
			 0x2d633a82, /// 0xfc4
			 0xb997ece5, /// 0xfc8
			 0x83f27e22, /// 0xfcc
			 0x7053a50d, /// 0xfd0
			 0x0ff53b3f, /// 0xfd4
			 0xb1b4ec59, /// 0xfd8
			 0xd1e59d04, /// 0xfdc
			 0xa5239550, /// 0xfe0
			 0xd4aaf937, /// 0xfe4
			 0x08ae2bbe, /// 0xfe8
			 0xdb23b161, /// 0xfec
			 0x6961e652, /// 0xff0
			 0x8b581d70, /// 0xff4
			 0x05802284, /// 0xff8
			 0xceb601de /// last
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
			 0x00000728,
			 0x00000630,
			 0x00000524,
			 0x000003a0,
			 0x000005d8,
			 0x0000054c,
			 0x000001fc,
			 0x00000118,

			 /// vpu register f2
			 0x40e00000,
			 0x40e00000,
			 0x41c80000,
			 0x41a00000,
			 0x41700000,
			 0x41200000,
			 0x41900000,
			 0x42000000,

			 /// vpu register f3
			 0x40c00000,
			 0x41300000,
			 0x41c80000,
			 0x41100000,
			 0x40800000,
			 0x40e00000,
			 0x41980000,
			 0x42000000,

			 /// vpu register f4
			 0x41880000,
			 0x42000000,
			 0x41600000,
			 0x41e00000,
			 0x41d00000,
			 0x40e00000,
			 0x41c00000,
			 0x41880000,

			 /// vpu register f5
			 0x41c80000,
			 0x41400000,
			 0x41100000,
			 0x41b80000,
			 0x41200000,
			 0x41880000,
			 0x41300000,
			 0x41500000,

			 /// vpu register f6
			 0x40e00000,
			 0x41a00000,
			 0x41b80000,
			 0x41a80000,
			 0x41200000,
			 0x41a80000,
			 0x41c00000,
			 0x41400000,

			 /// vpu register f7
			 0x40c00000,
			 0x40c00000,
			 0x3f800000,
			 0x41500000,
			 0x41000000,
			 0x41a00000,
			 0x40c00000,
			 0x41e00000,

			 /// vpu register f8
			 0x41d00000,
			 0x42000000,
			 0x41c00000,
			 0x41b80000,
			 0x41a00000,
			 0x41d80000,
			 0x40800000,
			 0x41100000,

			 /// vpu register f9
			 0x41d80000,
			 0x42000000,
			 0x41500000,
			 0x41500000,
			 0x41d80000,
			 0x40e00000,
			 0x40a00000,
			 0x41a80000,

			 /// vpu register f10
			 0x40800000,
			 0x41300000,
			 0x41400000,
			 0x41c00000,
			 0x40800000,
			 0x41000000,
			 0x40800000,
			 0x41800000,

			 /// vpu register f11
			 0x41200000,
			 0x41d80000,
			 0x41400000,
			 0x41c80000,
			 0x41a00000,
			 0x40c00000,
			 0x41980000,
			 0x41200000,

			 /// vpu register f12
			 0x41200000,
			 0x41900000,
			 0x41600000,
			 0x41e00000,
			 0x41000000,
			 0x41500000,
			 0x42000000,
			 0x42000000,

			 /// vpu register f13
			 0x40000000,
			 0x41d80000,
			 0x41c80000,
			 0x40e00000,
			 0x40a00000,
			 0x41e80000,
			 0x41300000,
			 0x41700000,

			 /// vpu register f14
			 0x41d80000,
			 0x41f80000,
			 0x41880000,
			 0x41600000,
			 0x3f800000,
			 0x41300000,
			 0x41300000,
			 0x40e00000,

			 /// vpu register f15
			 0x41d80000,
			 0x41300000,
			 0x41800000,
			 0x41900000,
			 0x41600000,
			 0x41f80000,
			 0x3f800000,
			 0x40800000,

			 /// vpu register f16
			 0x40800000,
			 0x41e00000,
			 0x40e00000,
			 0x41300000,
			 0x41b00000,
			 0x40800000,
			 0x40800000,
			 0x41200000,

			 /// vpu register f17
			 0x41100000,
			 0x41f00000,
			 0x41800000,
			 0x40000000,
			 0x41400000,
			 0x41c00000,
			 0x3f800000,
			 0x41900000,

			 /// vpu register f18
			 0x41500000,
			 0x41c80000,
			 0x41100000,
			 0x41e00000,
			 0x41100000,
			 0x40c00000,
			 0x40000000,
			 0x40e00000,

			 /// vpu register f19
			 0x41200000,
			 0x40400000,
			 0x41c00000,
			 0x40800000,
			 0x41900000,
			 0x41700000,
			 0x40a00000,
			 0x3f800000,

			 /// vpu register f20
			 0x41c00000,
			 0x41300000,
			 0x41980000,
			 0x40400000,
			 0x40e00000,
			 0x40400000,
			 0x40000000,
			 0x41a00000,

			 /// vpu register f21
			 0x41a80000,
			 0x40c00000,
			 0x40000000,
			 0x40e00000,
			 0x40000000,
			 0x41400000,
			 0x41400000,
			 0x41500000,

			 /// vpu register f22
			 0x40800000,
			 0x41400000,
			 0x41a80000,
			 0x40c00000,
			 0x40c00000,
			 0x41880000,
			 0x40e00000,
			 0x41880000,

			 /// vpu register f23
			 0x41500000,
			 0x41d80000,
			 0x40800000,
			 0x41800000,
			 0x41500000,
			 0x41400000,
			 0x40e00000,
			 0x41700000,

			 /// vpu register f24
			 0x41300000,
			 0x41900000,
			 0x41800000,
			 0x41a80000,
			 0x40800000,
			 0x41d00000,
			 0x40000000,
			 0x41f80000,

			 /// vpu register f25
			 0x40000000,
			 0x41700000,
			 0x41000000,
			 0x41500000,
			 0x40c00000,
			 0x41700000,
			 0x41880000,
			 0x41300000,

			 /// vpu register f26
			 0x41f00000,
			 0x41400000,
			 0x41300000,
			 0x40800000,
			 0x40400000,
			 0x41980000,
			 0x41700000,
			 0x41e80000,

			 /// vpu register f27
			 0x41500000,
			 0x41980000,
			 0x41400000,
			 0x40e00000,
			 0x41300000,
			 0x41800000,
			 0x41f00000,
			 0x40a00000,

			 /// vpu register f28
			 0x40e00000,
			 0x41e80000,
			 0x41c80000,
			 0x41d80000,
			 0x41300000,
			 0x40800000,
			 0x41a80000,
			 0x41000000,

			 /// vpu register f29
			 0x41700000,
			 0x41800000,
			 0x41a00000,
			 0x41300000,
			 0x41f00000,
			 0x41f00000,
			 0x41d80000,
			 0x40e00000,

			 /// vpu register f30
			 0x41f00000,
			 0x41e80000,
			 0x41c00000,
			 0x41b80000,
			 0x41c80000,
			 0x40400000,
			 0x41d80000,
			 0x41200000,

			 /// vpu register f31
			 0x40400000,
			 0x42000000,
			 0x41b80000,
			 0x41900000,
			 0x41400000,
			 0x41a80000,
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
		"fscbl.ps f11, f1 (x14)\n"                            ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f29, f1 (x11)\n"                            ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f26, f1 (x12)\n"                            ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f4, f1 (x13)\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f1, f1 (x15)\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f14, f1 (x14)\n"                            ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f19, f1 (x14)\n"                            ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x12)\n"                            ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f6, f1 (x14)\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f14, f1 (x11)\n"                            ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f6, f1 (x11)\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f2, f1 (x11)\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f3, f1 (x13)\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f4, f1 (x13)\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f4, f1 (x12)\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f12, f1 (x12)\n"                            ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f29, f1 (x14)\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f30, f1 (x14)\n"                            ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f19, f1 (x13)\n"                            ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f12, f1 (x11)\n"                            ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f13, f1 (x15)\n"                            ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f23, f1 (x13)\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f2, f1 (x13)\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x13)\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x12)\n"                            ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f9, f1 (x15)\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f4, f1 (x11)\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f21, f1 (x11)\n"                            ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f11, f1 (x14)\n"                            ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f0, f1 (x15)\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f21, f1 (x13)\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x14)\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f23, f1 (x14)\n"                            ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f4, f1 (x14)\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f14, f1 (x12)\n"                            ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f3, f1 (x14)\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f7, f1 (x11)\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f25, f1 (x13)\n"                            ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f22, f1 (x14)\n"                            ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f2, f1 (x13)\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x15)\n"                            ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f14, f1 (x14)\n"                            ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x11)\n"                            ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f23, f1 (x11)\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f6, f1 (x15)\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f9, f1 (x11)\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x11)\n"                            ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f15, f1 (x12)\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f25, f1 (x15)\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f26, f1 (x14)\n"                            ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x11)\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f0, f1 (x12)\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f21, f1 (x12)\n"                            ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f0, f1 (x15)\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f9, f1 (x12)\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x15)\n"                            ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f0, f1 (x15)\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f15, f1 (x14)\n"                            ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f28, f1 (x11)\n"                            ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f3, f1 (x14)\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x13)\n"                            ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f8, f1 (x12)\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f15, f1 (x13)\n"                            ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f15, f1 (x11)\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f13, f1 (x15)\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f2, f1 (x13)\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f7, f1 (x15)\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f28, f1 (x12)\n"                            ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f4, f1 (x12)\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f20, f1 (x11)\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f4, f1 (x11)\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f3, f1 (x14)\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f5, f1 (x13)\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f11, f1 (x11)\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f29, f1 (x13)\n"                            ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f7, f1 (x14)\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f25, f1 (x14)\n"                            ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f5, f1 (x13)\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f17, f1 (x11)\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f24, f1 (x13)\n"                            ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f7, f1 (x15)\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f10, f1 (x15)\n"                            ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f28, f1 (x15)\n"                            ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f17, f1 (x14)\n"                            ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f27, f1 (x11)\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f22, f1 (x11)\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f4, f1 (x15)\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f22, f1 (x13)\n"                            ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f6, f1 (x11)\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f28, f1 (x12)\n"                            ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f7, f1 (x14)\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f25, f1 (x12)\n"                            ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f20, f1 (x15)\n"                            ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f21, f1 (x15)\n"                            ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f6, f1 (x13)\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f22, f1 (x14)\n"                            ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f3, f1 (x14)\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f0, f1 (x14)\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbl.ps f7, f1 (x11)\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
