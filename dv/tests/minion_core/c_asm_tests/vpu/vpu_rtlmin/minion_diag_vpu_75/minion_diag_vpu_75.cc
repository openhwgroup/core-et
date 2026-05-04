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
			 0x09af4afa, /// 0x0
			 0xa359555d, /// 0x4
			 0xce52635a, /// 0x8
			 0xf6539adf, /// 0xc
			 0xf380d4f2, /// 0x10
			 0xbc2d37fb, /// 0x14
			 0xefcfc63c, /// 0x18
			 0x7b2a67dd, /// 0x1c
			 0x591c3b3f, /// 0x20
			 0xb8cf84bb, /// 0x24
			 0x6dbed01c, /// 0x28
			 0x90d879e4, /// 0x2c
			 0x195aafdf, /// 0x30
			 0x707d205f, /// 0x34
			 0xcaf44b23, /// 0x38
			 0x2a1ea510, /// 0x3c
			 0x05500760, /// 0x40
			 0x0f8ede30, /// 0x44
			 0xe1bbd52d, /// 0x48
			 0x6e9da226, /// 0x4c
			 0x4d3d0ae6, /// 0x50
			 0x397778de, /// 0x54
			 0x666742b6, /// 0x58
			 0x16b3d112, /// 0x5c
			 0xf7e919db, /// 0x60
			 0x92193754, /// 0x64
			 0x540b8c43, /// 0x68
			 0x935380e9, /// 0x6c
			 0x87c39c64, /// 0x70
			 0x1435771a, /// 0x74
			 0x69de5bd1, /// 0x78
			 0x1e340243, /// 0x7c
			 0xd6eab50a, /// 0x80
			 0xe2db9b20, /// 0x84
			 0xd89b9f93, /// 0x88
			 0x0fd0fe01, /// 0x8c
			 0x4a6dc95b, /// 0x90
			 0x91ec5e6e, /// 0x94
			 0x89681088, /// 0x98
			 0x31e733a2, /// 0x9c
			 0x347ebdc2, /// 0xa0
			 0x74a0000e, /// 0xa4
			 0x5dc8454d, /// 0xa8
			 0xdafd3a02, /// 0xac
			 0x8f5061a3, /// 0xb0
			 0x59a17aff, /// 0xb4
			 0xf17df8ec, /// 0xb8
			 0x1666a4f4, /// 0xbc
			 0xb472aa2d, /// 0xc0
			 0xb41b8d5b, /// 0xc4
			 0x1bf2566c, /// 0xc8
			 0x7a06e24b, /// 0xcc
			 0x2016062f, /// 0xd0
			 0x1b441fac, /// 0xd4
			 0xd23e6cbb, /// 0xd8
			 0xae437d99, /// 0xdc
			 0xd7a3c591, /// 0xe0
			 0x091a3682, /// 0xe4
			 0x46ecbbdb, /// 0xe8
			 0xc33abf36, /// 0xec
			 0x9b5a9ec5, /// 0xf0
			 0x77c67be9, /// 0xf4
			 0x20214bc0, /// 0xf8
			 0xa8e967b0, /// 0xfc
			 0x12554ed8, /// 0x100
			 0x7c404aac, /// 0x104
			 0xa36e204d, /// 0x108
			 0x0278b09f, /// 0x10c
			 0x2407135e, /// 0x110
			 0x9db06165, /// 0x114
			 0xc7c35aa4, /// 0x118
			 0x2ff7973d, /// 0x11c
			 0xc46d5dd8, /// 0x120
			 0x8e5039e7, /// 0x124
			 0x311d3726, /// 0x128
			 0x42703110, /// 0x12c
			 0x6f86548d, /// 0x130
			 0xc7e18a3b, /// 0x134
			 0x22e69900, /// 0x138
			 0x83d4ff26, /// 0x13c
			 0xcbc17c08, /// 0x140
			 0x0e5d6479, /// 0x144
			 0xf4213ff1, /// 0x148
			 0x746e663a, /// 0x14c
			 0x1ea181fe, /// 0x150
			 0xd9042cfa, /// 0x154
			 0x549f75e4, /// 0x158
			 0xeab10f30, /// 0x15c
			 0xf12e002d, /// 0x160
			 0xac80584a, /// 0x164
			 0x377f1ccd, /// 0x168
			 0x0413922f, /// 0x16c
			 0x5660feb1, /// 0x170
			 0xb07bc3c8, /// 0x174
			 0xf236dd8f, /// 0x178
			 0x6684c1f4, /// 0x17c
			 0xb4a72055, /// 0x180
			 0xc0917476, /// 0x184
			 0xbe9356d8, /// 0x188
			 0x22c433db, /// 0x18c
			 0x2c3f8e9d, /// 0x190
			 0x16c51c76, /// 0x194
			 0x8f927d81, /// 0x198
			 0xc30e6107, /// 0x19c
			 0x2e926a11, /// 0x1a0
			 0xc6206d9f, /// 0x1a4
			 0x620a6dd1, /// 0x1a8
			 0x01497677, /// 0x1ac
			 0x06772d22, /// 0x1b0
			 0x7398e8c3, /// 0x1b4
			 0x3f03393f, /// 0x1b8
			 0xddf71a1a, /// 0x1bc
			 0x03ad93f8, /// 0x1c0
			 0xda9ab58c, /// 0x1c4
			 0x3bd26546, /// 0x1c8
			 0xa1707411, /// 0x1cc
			 0x0b1807fd, /// 0x1d0
			 0x6a3b7d8b, /// 0x1d4
			 0xad22a09a, /// 0x1d8
			 0x58f5b0e8, /// 0x1dc
			 0xa7d493d6, /// 0x1e0
			 0x4df517ae, /// 0x1e4
			 0xf19b96cc, /// 0x1e8
			 0x9db5d439, /// 0x1ec
			 0x8de95c63, /// 0x1f0
			 0x987205a7, /// 0x1f4
			 0xcb3f4c23, /// 0x1f8
			 0x5bda19c7, /// 0x1fc
			 0x9b8e611c, /// 0x200
			 0x86fbc1cd, /// 0x204
			 0x4fc81d8f, /// 0x208
			 0xd031acf1, /// 0x20c
			 0x2d641f52, /// 0x210
			 0x8ae63f5a, /// 0x214
			 0x39d952fd, /// 0x218
			 0x84691c53, /// 0x21c
			 0x784d578c, /// 0x220
			 0x60048198, /// 0x224
			 0x34a69cb8, /// 0x228
			 0x9b30572d, /// 0x22c
			 0x7456de9b, /// 0x230
			 0xbc5cfe72, /// 0x234
			 0x76ba1a6c, /// 0x238
			 0xd5783528, /// 0x23c
			 0x1569de68, /// 0x240
			 0x086d33f0, /// 0x244
			 0xf364b3eb, /// 0x248
			 0x9c785161, /// 0x24c
			 0xfacd8c00, /// 0x250
			 0xf2ac61dd, /// 0x254
			 0xe2187525, /// 0x258
			 0xcc235a33, /// 0x25c
			 0x5092f656, /// 0x260
			 0xfdbf8bab, /// 0x264
			 0xfb5ad8df, /// 0x268
			 0x3f95c5c9, /// 0x26c
			 0xe023dbec, /// 0x270
			 0xe9a35af3, /// 0x274
			 0xa0ddc828, /// 0x278
			 0xf0801915, /// 0x27c
			 0x77aaa507, /// 0x280
			 0x46476169, /// 0x284
			 0xa469553b, /// 0x288
			 0x1db8b714, /// 0x28c
			 0x3a46e754, /// 0x290
			 0x2553775d, /// 0x294
			 0x6f4ff817, /// 0x298
			 0x26239bbf, /// 0x29c
			 0xc074d1dd, /// 0x2a0
			 0xbb130df2, /// 0x2a4
			 0x4f33bdc2, /// 0x2a8
			 0x88850b2f, /// 0x2ac
			 0x6ba23b55, /// 0x2b0
			 0x613b4107, /// 0x2b4
			 0xaaf86671, /// 0x2b8
			 0x96cb4492, /// 0x2bc
			 0xdf1bbc0a, /// 0x2c0
			 0xe40ec681, /// 0x2c4
			 0x220c5a1d, /// 0x2c8
			 0x60caf9a6, /// 0x2cc
			 0xabd26fb4, /// 0x2d0
			 0x243b38a0, /// 0x2d4
			 0x889e0fc9, /// 0x2d8
			 0x35458d85, /// 0x2dc
			 0xb2e224bb, /// 0x2e0
			 0xd9c4d53e, /// 0x2e4
			 0xc0750644, /// 0x2e8
			 0x76755231, /// 0x2ec
			 0x9a2495d8, /// 0x2f0
			 0xce63e593, /// 0x2f4
			 0x83031a75, /// 0x2f8
			 0x29d7578d, /// 0x2fc
			 0x3601b838, /// 0x300
			 0xdc94c443, /// 0x304
			 0x7b3fe0de, /// 0x308
			 0x87ab1766, /// 0x30c
			 0xf33178a4, /// 0x310
			 0x4cab3435, /// 0x314
			 0x5a0047e8, /// 0x318
			 0x9f649e39, /// 0x31c
			 0x0aaef5fb, /// 0x320
			 0x204b7c14, /// 0x324
			 0xfce008b5, /// 0x328
			 0xf65263b8, /// 0x32c
			 0x6ce04bf4, /// 0x330
			 0x3f9f445e, /// 0x334
			 0x6ee2c63d, /// 0x338
			 0x06e62200, /// 0x33c
			 0xc0f82d37, /// 0x340
			 0xf3b43694, /// 0x344
			 0xf68ff866, /// 0x348
			 0xf0342b58, /// 0x34c
			 0x6b2982ff, /// 0x350
			 0xb35e2f57, /// 0x354
			 0xe8d55bd5, /// 0x358
			 0xb234f96c, /// 0x35c
			 0xeb7a293e, /// 0x360
			 0x767a3fc4, /// 0x364
			 0x3d996259, /// 0x368
			 0x5dc3ca41, /// 0x36c
			 0x9628c4f7, /// 0x370
			 0xc0dbf4ef, /// 0x374
			 0x72188357, /// 0x378
			 0xf563e945, /// 0x37c
			 0x037dcced, /// 0x380
			 0x44b53b93, /// 0x384
			 0xc5822e8c, /// 0x388
			 0x47c333f4, /// 0x38c
			 0xd6052a80, /// 0x390
			 0xaa139297, /// 0x394
			 0x82bf0877, /// 0x398
			 0xfc34f539, /// 0x39c
			 0xefdcfd39, /// 0x3a0
			 0xb761efce, /// 0x3a4
			 0xcda258ea, /// 0x3a8
			 0xd48882c4, /// 0x3ac
			 0x1724ff63, /// 0x3b0
			 0xe36eb642, /// 0x3b4
			 0xd1bd1d30, /// 0x3b8
			 0x952d62e4, /// 0x3bc
			 0x9d178cb2, /// 0x3c0
			 0x38e39df4, /// 0x3c4
			 0x09ab6bcd, /// 0x3c8
			 0x4505649e, /// 0x3cc
			 0xa6d17078, /// 0x3d0
			 0xe35576c5, /// 0x3d4
			 0x213e2bca, /// 0x3d8
			 0x586b835b, /// 0x3dc
			 0xfab93757, /// 0x3e0
			 0xc2fe00bb, /// 0x3e4
			 0x9ec9baa6, /// 0x3e8
			 0xf5389d8c, /// 0x3ec
			 0xc8e8becc, /// 0x3f0
			 0x5510e7cf, /// 0x3f4
			 0x868c9a6f, /// 0x3f8
			 0xe75a2574, /// 0x3fc
			 0x1e9dcaea, /// 0x400
			 0xe34949ed, /// 0x404
			 0xcc678e52, /// 0x408
			 0x3be58d24, /// 0x40c
			 0xf27fab01, /// 0x410
			 0xe8d444b3, /// 0x414
			 0xaf1326c5, /// 0x418
			 0xcf37c9db, /// 0x41c
			 0x51f4ce80, /// 0x420
			 0x3b69370b, /// 0x424
			 0x99831009, /// 0x428
			 0x903d6006, /// 0x42c
			 0x6f533839, /// 0x430
			 0xa4f906e2, /// 0x434
			 0xbb24ac46, /// 0x438
			 0xe63f2d21, /// 0x43c
			 0x9a766ea1, /// 0x440
			 0xf919755f, /// 0x444
			 0xb7be9270, /// 0x448
			 0xc57a7fbd, /// 0x44c
			 0xb7c0a9ac, /// 0x450
			 0x2a039678, /// 0x454
			 0xa55cd2e6, /// 0x458
			 0x5a774370, /// 0x45c
			 0xb6819bca, /// 0x460
			 0x1c28b4d5, /// 0x464
			 0xdb9b1424, /// 0x468
			 0x200f12aa, /// 0x46c
			 0x0b0df174, /// 0x470
			 0x01aee4e5, /// 0x474
			 0x8b61779a, /// 0x478
			 0xf0d3a94a, /// 0x47c
			 0x5fd4a24b, /// 0x480
			 0xec778b98, /// 0x484
			 0x053d96ca, /// 0x488
			 0xf70a9306, /// 0x48c
			 0x2823fbdb, /// 0x490
			 0x00e4c805, /// 0x494
			 0xa19e3656, /// 0x498
			 0xacf8d437, /// 0x49c
			 0xfdc63052, /// 0x4a0
			 0x9f5b0aed, /// 0x4a4
			 0x4cb6a42e, /// 0x4a8
			 0x24138d40, /// 0x4ac
			 0xc35e2f79, /// 0x4b0
			 0x66341773, /// 0x4b4
			 0xf3f9e373, /// 0x4b8
			 0x4f8584dc, /// 0x4bc
			 0xc096760d, /// 0x4c0
			 0xcde19d0c, /// 0x4c4
			 0xac8c8828, /// 0x4c8
			 0xdad65f9f, /// 0x4cc
			 0xe8026e2b, /// 0x4d0
			 0x821d7e9c, /// 0x4d4
			 0x44a0497a, /// 0x4d8
			 0x1a087e31, /// 0x4dc
			 0x4470544c, /// 0x4e0
			 0xb5e62b43, /// 0x4e4
			 0x2a4ef784, /// 0x4e8
			 0xe3293f6a, /// 0x4ec
			 0x237941dc, /// 0x4f0
			 0x9cdd0fda, /// 0x4f4
			 0x38459bd0, /// 0x4f8
			 0xb7697352, /// 0x4fc
			 0xf8baeb0e, /// 0x500
			 0xc5bc8e91, /// 0x504
			 0x17393e48, /// 0x508
			 0xe6e982b9, /// 0x50c
			 0x1eb040db, /// 0x510
			 0xd142cf5c, /// 0x514
			 0x69758ecd, /// 0x518
			 0x18deb4db, /// 0x51c
			 0xeee6512c, /// 0x520
			 0x5c407b6e, /// 0x524
			 0x0e4e64ee, /// 0x528
			 0xe787d578, /// 0x52c
			 0x7380276a, /// 0x530
			 0xd616169b, /// 0x534
			 0xe0f6358b, /// 0x538
			 0x44eb8388, /// 0x53c
			 0x7e00983e, /// 0x540
			 0x03bbbd52, /// 0x544
			 0xe3c49200, /// 0x548
			 0xfe52bddb, /// 0x54c
			 0x24bd13b1, /// 0x550
			 0xf91e0436, /// 0x554
			 0x01de3a9d, /// 0x558
			 0x893a5c7a, /// 0x55c
			 0x2d6909b1, /// 0x560
			 0x6472720f, /// 0x564
			 0xc6a7b5da, /// 0x568
			 0x3c40828e, /// 0x56c
			 0xe6e9d37f, /// 0x570
			 0x1d81dbc5, /// 0x574
			 0xd8937661, /// 0x578
			 0x256408b7, /// 0x57c
			 0x1c9b36a8, /// 0x580
			 0x4e15e972, /// 0x584
			 0x159aae34, /// 0x588
			 0x82159b60, /// 0x58c
			 0x5b177ccb, /// 0x590
			 0x3172b684, /// 0x594
			 0x3c5160ee, /// 0x598
			 0x52fd51ce, /// 0x59c
			 0x0854be3e, /// 0x5a0
			 0xe68548dc, /// 0x5a4
			 0xb535a955, /// 0x5a8
			 0x0f125f15, /// 0x5ac
			 0xc2369acd, /// 0x5b0
			 0x89cbc33e, /// 0x5b4
			 0x844023c8, /// 0x5b8
			 0x2f6b04f6, /// 0x5bc
			 0x46705954, /// 0x5c0
			 0x6263351a, /// 0x5c4
			 0x60ff72c7, /// 0x5c8
			 0x13f2337b, /// 0x5cc
			 0x574923d7, /// 0x5d0
			 0xe313c98d, /// 0x5d4
			 0x874e169f, /// 0x5d8
			 0x56d70b21, /// 0x5dc
			 0x5bd84413, /// 0x5e0
			 0xe235e6de, /// 0x5e4
			 0x3e4ad483, /// 0x5e8
			 0xb3d82b27, /// 0x5ec
			 0xe6bf2942, /// 0x5f0
			 0xb994ba8b, /// 0x5f4
			 0x5721ef5b, /// 0x5f8
			 0x81d536e2, /// 0x5fc
			 0x57232e70, /// 0x600
			 0x5284a823, /// 0x604
			 0x70b12d92, /// 0x608
			 0x25eee5e1, /// 0x60c
			 0x34929ea1, /// 0x610
			 0xa4236446, /// 0x614
			 0xf3ee1eaf, /// 0x618
			 0x0d52233b, /// 0x61c
			 0xda7e30ac, /// 0x620
			 0x62083652, /// 0x624
			 0x002b0e0a, /// 0x628
			 0xfa5aca0e, /// 0x62c
			 0xc176d98f, /// 0x630
			 0x23bf9f20, /// 0x634
			 0x396e1560, /// 0x638
			 0xd7495228, /// 0x63c
			 0x70321cdb, /// 0x640
			 0xbcae7d46, /// 0x644
			 0x8388429f, /// 0x648
			 0x3692a57c, /// 0x64c
			 0x08c45fd6, /// 0x650
			 0x946f44c8, /// 0x654
			 0x8dd3d418, /// 0x658
			 0xa33460d1, /// 0x65c
			 0xff4b5b1b, /// 0x660
			 0xe4f6a21b, /// 0x664
			 0xd6b20ec0, /// 0x668
			 0x9563e6f1, /// 0x66c
			 0xac86615e, /// 0x670
			 0x7bed28e5, /// 0x674
			 0xe5da0187, /// 0x678
			 0x85ad67ef, /// 0x67c
			 0xbfa9721f, /// 0x680
			 0x6b827d3a, /// 0x684
			 0xa87a2ce6, /// 0x688
			 0x139ed052, /// 0x68c
			 0x09f7ba9c, /// 0x690
			 0x6f95a6b6, /// 0x694
			 0xab4bba8c, /// 0x698
			 0xb8d9c966, /// 0x69c
			 0x462511ab, /// 0x6a0
			 0x6f22df77, /// 0x6a4
			 0xbf40ff7d, /// 0x6a8
			 0xde981209, /// 0x6ac
			 0x8553f26e, /// 0x6b0
			 0x02dd9c23, /// 0x6b4
			 0xab5b0e81, /// 0x6b8
			 0xbad08e56, /// 0x6bc
			 0xbcabd3e8, /// 0x6c0
			 0x4b3f32b4, /// 0x6c4
			 0x3d44bd03, /// 0x6c8
			 0x56282577, /// 0x6cc
			 0x4d82aa47, /// 0x6d0
			 0xcecd4f8d, /// 0x6d4
			 0x7cd53961, /// 0x6d8
			 0x91f4e8b5, /// 0x6dc
			 0x94c48d78, /// 0x6e0
			 0x934bbadd, /// 0x6e4
			 0xf2a32ad6, /// 0x6e8
			 0x0cf937fa, /// 0x6ec
			 0x2e923370, /// 0x6f0
			 0x78fa3d31, /// 0x6f4
			 0x25bae93f, /// 0x6f8
			 0xd4e4999b, /// 0x6fc
			 0xed085b77, /// 0x700
			 0x397dd38d, /// 0x704
			 0xe71b569c, /// 0x708
			 0x59a764fd, /// 0x70c
			 0xad197010, /// 0x710
			 0xbdb11a92, /// 0x714
			 0xc901a292, /// 0x718
			 0x09786a61, /// 0x71c
			 0x9bc3d25d, /// 0x720
			 0x85e4de8c, /// 0x724
			 0xf1b51f0d, /// 0x728
			 0xa30f024c, /// 0x72c
			 0x75792cff, /// 0x730
			 0x30e120c0, /// 0x734
			 0x7faa129e, /// 0x738
			 0x0f6e82af, /// 0x73c
			 0x421851e9, /// 0x740
			 0xc8f73676, /// 0x744
			 0xe718829d, /// 0x748
			 0x70010e54, /// 0x74c
			 0x990dcad8, /// 0x750
			 0xe4071a37, /// 0x754
			 0x79eef2d8, /// 0x758
			 0x69eedfa3, /// 0x75c
			 0xd7fe7646, /// 0x760
			 0xfe323f53, /// 0x764
			 0xdea03e32, /// 0x768
			 0x1f3868d1, /// 0x76c
			 0xe23b2c63, /// 0x770
			 0x4a969052, /// 0x774
			 0x23d35ffa, /// 0x778
			 0xf6892435, /// 0x77c
			 0xa411344f, /// 0x780
			 0x7f202c9c, /// 0x784
			 0x585970d6, /// 0x788
			 0x9375d528, /// 0x78c
			 0x1d9ed264, /// 0x790
			 0xdfc2cf27, /// 0x794
			 0xe0d85482, /// 0x798
			 0x4c7b2919, /// 0x79c
			 0x95b54b24, /// 0x7a0
			 0xde3cda02, /// 0x7a4
			 0xf5e3d171, /// 0x7a8
			 0x37df6985, /// 0x7ac
			 0x63a3eda4, /// 0x7b0
			 0x9ef7e3d4, /// 0x7b4
			 0xc3cfd96c, /// 0x7b8
			 0xbf4128aa, /// 0x7bc
			 0x88835031, /// 0x7c0
			 0xb6b38a61, /// 0x7c4
			 0xfff1b6e1, /// 0x7c8
			 0xe8e99378, /// 0x7cc
			 0x99545c65, /// 0x7d0
			 0x8678b21b, /// 0x7d4
			 0xa40522be, /// 0x7d8
			 0x19571944, /// 0x7dc
			 0x1f0a83ae, /// 0x7e0
			 0x7df2e13d, /// 0x7e4
			 0x7593b10c, /// 0x7e8
			 0xe4cebc18, /// 0x7ec
			 0xfeb7f5c7, /// 0x7f0
			 0xed274b27, /// 0x7f4
			 0x8b26cb36, /// 0x7f8
			 0x5044263c, /// 0x7fc
			 0xc3a16ee0, /// 0x800
			 0xaeff6807, /// 0x804
			 0x907df7b4, /// 0x808
			 0x0dc4b2cd, /// 0x80c
			 0x73e33f33, /// 0x810
			 0x3030853b, /// 0x814
			 0x55b51c3b, /// 0x818
			 0x64653482, /// 0x81c
			 0x4c00f0ad, /// 0x820
			 0xef96d310, /// 0x824
			 0x54dfb16c, /// 0x828
			 0x51584b08, /// 0x82c
			 0x52d11d5d, /// 0x830
			 0xa38e3447, /// 0x834
			 0xe48a59c0, /// 0x838
			 0x98cbf96c, /// 0x83c
			 0x9d134f88, /// 0x840
			 0x357563c1, /// 0x844
			 0xe36e1011, /// 0x848
			 0x5da1e9c6, /// 0x84c
			 0xaf2322d3, /// 0x850
			 0xd9f3255c, /// 0x854
			 0xd62c0a84, /// 0x858
			 0xad2ec6cd, /// 0x85c
			 0xd1caa0b7, /// 0x860
			 0x175e639f, /// 0x864
			 0x7a8705a2, /// 0x868
			 0x87c096e0, /// 0x86c
			 0x56fbbf7c, /// 0x870
			 0x5487e384, /// 0x874
			 0xe794bd52, /// 0x878
			 0x5058576d, /// 0x87c
			 0x10f18f25, /// 0x880
			 0x8e76db7f, /// 0x884
			 0x4d965785, /// 0x888
			 0x82532516, /// 0x88c
			 0x97d0e881, /// 0x890
			 0x96ce4c58, /// 0x894
			 0x50df285e, /// 0x898
			 0x5a30ec62, /// 0x89c
			 0xd121f518, /// 0x8a0
			 0x52412dde, /// 0x8a4
			 0x8b9c30e9, /// 0x8a8
			 0x41eedd9c, /// 0x8ac
			 0xa88a7bea, /// 0x8b0
			 0x8ae2e458, /// 0x8b4
			 0x155163f4, /// 0x8b8
			 0xddaf9abd, /// 0x8bc
			 0xf62c77d5, /// 0x8c0
			 0x727811ed, /// 0x8c4
			 0x2de45a66, /// 0x8c8
			 0x8eb55c7b, /// 0x8cc
			 0xbe2db31a, /// 0x8d0
			 0x8cd25565, /// 0x8d4
			 0xbd4b572a, /// 0x8d8
			 0x22f5fb29, /// 0x8dc
			 0x6a633e82, /// 0x8e0
			 0x38d4f472, /// 0x8e4
			 0x295f765f, /// 0x8e8
			 0x59f5a03f, /// 0x8ec
			 0xa35c48a4, /// 0x8f0
			 0x4e015dd8, /// 0x8f4
			 0x1fcb742f, /// 0x8f8
			 0xf631b578, /// 0x8fc
			 0xd7910f31, /// 0x900
			 0xaf02a428, /// 0x904
			 0x0e2d2826, /// 0x908
			 0x6c793cb8, /// 0x90c
			 0x1641a1bc, /// 0x910
			 0x3db46d59, /// 0x914
			 0x86071b6f, /// 0x918
			 0x4a384185, /// 0x91c
			 0xf827f951, /// 0x920
			 0x03cd9826, /// 0x924
			 0xa514fc60, /// 0x928
			 0xb3fe6b0e, /// 0x92c
			 0x47a7f013, /// 0x930
			 0x479521bb, /// 0x934
			 0x67b039d5, /// 0x938
			 0xf484ea54, /// 0x93c
			 0xf1629a84, /// 0x940
			 0x47b1749e, /// 0x944
			 0x8a552e17, /// 0x948
			 0xb781c728, /// 0x94c
			 0xba940198, /// 0x950
			 0xbcac43cd, /// 0x954
			 0x9c88b126, /// 0x958
			 0x4390a146, /// 0x95c
			 0x0d27ccd9, /// 0x960
			 0x67e9e5d0, /// 0x964
			 0x1ef563ea, /// 0x968
			 0xfb04bbbe, /// 0x96c
			 0xd1cb3c40, /// 0x970
			 0x66df0581, /// 0x974
			 0xe545b9e1, /// 0x978
			 0xefc27696, /// 0x97c
			 0xb2dd9682, /// 0x980
			 0x74424b60, /// 0x984
			 0xecbfaaf7, /// 0x988
			 0x267a40e9, /// 0x98c
			 0xc10b8fbd, /// 0x990
			 0x3d4f7936, /// 0x994
			 0x1d9a4251, /// 0x998
			 0x91490c56, /// 0x99c
			 0x7e2b818e, /// 0x9a0
			 0xbb97b028, /// 0x9a4
			 0x7efedc3b, /// 0x9a8
			 0xd23f54be, /// 0x9ac
			 0xea225208, /// 0x9b0
			 0x3d8d8675, /// 0x9b4
			 0x4e99ea2d, /// 0x9b8
			 0x4cc2e1e7, /// 0x9bc
			 0xb1a24e33, /// 0x9c0
			 0x2664a4db, /// 0x9c4
			 0xe669f9a2, /// 0x9c8
			 0x56bc69a5, /// 0x9cc
			 0x4d9f6ee0, /// 0x9d0
			 0x06f136c0, /// 0x9d4
			 0x003ac007, /// 0x9d8
			 0xcd421937, /// 0x9dc
			 0x2eb979c5, /// 0x9e0
			 0x24c4a094, /// 0x9e4
			 0x64205221, /// 0x9e8
			 0xb87e4332, /// 0x9ec
			 0x3ce66c08, /// 0x9f0
			 0xce25954e, /// 0x9f4
			 0xedbbcfd1, /// 0x9f8
			 0x0550f8a3, /// 0x9fc
			 0x79fcaa69, /// 0xa00
			 0x57b5b856, /// 0xa04
			 0x137e56ad, /// 0xa08
			 0x12dd39ed, /// 0xa0c
			 0x2920934f, /// 0xa10
			 0xff445bf6, /// 0xa14
			 0x7d802853, /// 0xa18
			 0x72e810fa, /// 0xa1c
			 0x061a1bc0, /// 0xa20
			 0x0ffa84e3, /// 0xa24
			 0xe6ffc118, /// 0xa28
			 0xc63fd620, /// 0xa2c
			 0x901d781d, /// 0xa30
			 0xbb78773c, /// 0xa34
			 0xf11ca090, /// 0xa38
			 0x7eb05679, /// 0xa3c
			 0x68e1dd9e, /// 0xa40
			 0x9b312a88, /// 0xa44
			 0x5931b002, /// 0xa48
			 0x6b85a323, /// 0xa4c
			 0xa4b57e8c, /// 0xa50
			 0xf348fb97, /// 0xa54
			 0x6a3d658a, /// 0xa58
			 0x811ae34b, /// 0xa5c
			 0x4bd73244, /// 0xa60
			 0x06bda771, /// 0xa64
			 0x979e8b33, /// 0xa68
			 0xc13dd9ba, /// 0xa6c
			 0x23910887, /// 0xa70
			 0xa19d3a57, /// 0xa74
			 0xd1032a3b, /// 0xa78
			 0xb9011c96, /// 0xa7c
			 0x8c830ecd, /// 0xa80
			 0x986c604d, /// 0xa84
			 0xc484373f, /// 0xa88
			 0xe440eb45, /// 0xa8c
			 0x254b2ef5, /// 0xa90
			 0x01253304, /// 0xa94
			 0xde953450, /// 0xa98
			 0x22996e0c, /// 0xa9c
			 0xe8529ea0, /// 0xaa0
			 0x47688ec3, /// 0xaa4
			 0xd83c97ea, /// 0xaa8
			 0xae4042f5, /// 0xaac
			 0xc6befea3, /// 0xab0
			 0xa4ca2ebe, /// 0xab4
			 0xedcba7a0, /// 0xab8
			 0x870ed181, /// 0xabc
			 0x79503c30, /// 0xac0
			 0x182c0b22, /// 0xac4
			 0x0f9fc18f, /// 0xac8
			 0xb7a0d689, /// 0xacc
			 0x75f4d2bd, /// 0xad0
			 0x2f9557b9, /// 0xad4
			 0x1f0dc21e, /// 0xad8
			 0xc540765e, /// 0xadc
			 0x73d3a9e4, /// 0xae0
			 0xbf19c97c, /// 0xae4
			 0x4d076b59, /// 0xae8
			 0x1ae0fafa, /// 0xaec
			 0x0ed79813, /// 0xaf0
			 0xfa77fb8f, /// 0xaf4
			 0x82e31faf, /// 0xaf8
			 0xb97478a3, /// 0xafc
			 0x2c014f10, /// 0xb00
			 0x561fee05, /// 0xb04
			 0x42868d5a, /// 0xb08
			 0x51427fa5, /// 0xb0c
			 0x0095a2dd, /// 0xb10
			 0x4a6fbdb0, /// 0xb14
			 0xbd34616d, /// 0xb18
			 0x39e6c38b, /// 0xb1c
			 0x3e55cba0, /// 0xb20
			 0xb7bfd578, /// 0xb24
			 0x7e9fcfcd, /// 0xb28
			 0x289f1b73, /// 0xb2c
			 0xa9740003, /// 0xb30
			 0xb0b1489f, /// 0xb34
			 0x7e88fd14, /// 0xb38
			 0x9accc535, /// 0xb3c
			 0x19ae536e, /// 0xb40
			 0xef91ab5a, /// 0xb44
			 0x4a5d54ab, /// 0xb48
			 0xfad4e971, /// 0xb4c
			 0x13b18181, /// 0xb50
			 0x0ba6dcdc, /// 0xb54
			 0xb2148083, /// 0xb58
			 0x851b786e, /// 0xb5c
			 0x44b80553, /// 0xb60
			 0xbf8a491e, /// 0xb64
			 0x6710c01f, /// 0xb68
			 0xba8da662, /// 0xb6c
			 0xc3e5b6a7, /// 0xb70
			 0x6f9b58de, /// 0xb74
			 0x9d0965e7, /// 0xb78
			 0xf374d37b, /// 0xb7c
			 0xcc6c94a0, /// 0xb80
			 0xad49c4da, /// 0xb84
			 0x3b09f720, /// 0xb88
			 0x6b27890e, /// 0xb8c
			 0x6a3a52f4, /// 0xb90
			 0xf5635e77, /// 0xb94
			 0x89b7c090, /// 0xb98
			 0x50b26494, /// 0xb9c
			 0x36f58360, /// 0xba0
			 0xa911428b, /// 0xba4
			 0x91a08398, /// 0xba8
			 0xca240625, /// 0xbac
			 0xa9f78007, /// 0xbb0
			 0x03a23c06, /// 0xbb4
			 0x945ee045, /// 0xbb8
			 0xe08cf29a, /// 0xbbc
			 0xd509cf3b, /// 0xbc0
			 0xc2670ee7, /// 0xbc4
			 0x6a687510, /// 0xbc8
			 0xd1e75a5b, /// 0xbcc
			 0xebb3bdac, /// 0xbd0
			 0x86f645fe, /// 0xbd4
			 0x2ad6c00e, /// 0xbd8
			 0x582a10ea, /// 0xbdc
			 0x24f2b7f5, /// 0xbe0
			 0x46bd1cbe, /// 0xbe4
			 0x3d42ff2b, /// 0xbe8
			 0x2a4decaa, /// 0xbec
			 0x5ab201ff, /// 0xbf0
			 0xc59aad35, /// 0xbf4
			 0x670fab14, /// 0xbf8
			 0xc30cfaba, /// 0xbfc
			 0x732f9375, /// 0xc00
			 0xf7ad3d32, /// 0xc04
			 0x2e7f7773, /// 0xc08
			 0xe0dd2d2c, /// 0xc0c
			 0xd4190515, /// 0xc10
			 0x92017e64, /// 0xc14
			 0x680070fb, /// 0xc18
			 0x251a10b8, /// 0xc1c
			 0x5d0abedf, /// 0xc20
			 0x239f5d8f, /// 0xc24
			 0xda386066, /// 0xc28
			 0xd8140d5a, /// 0xc2c
			 0x16db5014, /// 0xc30
			 0x07726fcc, /// 0xc34
			 0x383f9139, /// 0xc38
			 0x08210a39, /// 0xc3c
			 0xef55de12, /// 0xc40
			 0x127c08eb, /// 0xc44
			 0x5fbfc890, /// 0xc48
			 0x6fdd0a58, /// 0xc4c
			 0x1841203d, /// 0xc50
			 0xff6133ed, /// 0xc54
			 0x2692bcd5, /// 0xc58
			 0x2f9aabb5, /// 0xc5c
			 0x98a87877, /// 0xc60
			 0x3e4085b2, /// 0xc64
			 0xcff78b30, /// 0xc68
			 0xb99e0a63, /// 0xc6c
			 0x997db820, /// 0xc70
			 0xff467579, /// 0xc74
			 0x547d6239, /// 0xc78
			 0x99e1343d, /// 0xc7c
			 0x1734461a, /// 0xc80
			 0x7359953a, /// 0xc84
			 0x7a42eff2, /// 0xc88
			 0xb7bb1961, /// 0xc8c
			 0x28a0f489, /// 0xc90
			 0x1ae1f49e, /// 0xc94
			 0x0b417a48, /// 0xc98
			 0x0f1cf3bb, /// 0xc9c
			 0x1020fd4d, /// 0xca0
			 0x6dc8ae0d, /// 0xca4
			 0xaf3de8e1, /// 0xca8
			 0xf8bb030b, /// 0xcac
			 0xd82403bc, /// 0xcb0
			 0xee8d180e, /// 0xcb4
			 0xb23a533f, /// 0xcb8
			 0x7d727ebe, /// 0xcbc
			 0x2ef627dd, /// 0xcc0
			 0xaf0020e4, /// 0xcc4
			 0x05ba9baf, /// 0xcc8
			 0xed7abf66, /// 0xccc
			 0x7989cdec, /// 0xcd0
			 0xeff316bf, /// 0xcd4
			 0x0e636f98, /// 0xcd8
			 0x296b728c, /// 0xcdc
			 0x77773736, /// 0xce0
			 0xc32ff541, /// 0xce4
			 0x65f0e4cf, /// 0xce8
			 0x8bac2692, /// 0xcec
			 0xfc6cd6ba, /// 0xcf0
			 0xacd17b23, /// 0xcf4
			 0x1fa0e605, /// 0xcf8
			 0x4f8fdb41, /// 0xcfc
			 0x1e631a33, /// 0xd00
			 0x98c619b2, /// 0xd04
			 0x90ba60ac, /// 0xd08
			 0x23433a1b, /// 0xd0c
			 0x4a0726fe, /// 0xd10
			 0xfeef594b, /// 0xd14
			 0xae8245fc, /// 0xd18
			 0x39d56096, /// 0xd1c
			 0xcd793424, /// 0xd20
			 0xb7f4b9a6, /// 0xd24
			 0xefc019d2, /// 0xd28
			 0x50b67519, /// 0xd2c
			 0xbb25e55a, /// 0xd30
			 0x1c493076, /// 0xd34
			 0x09c71708, /// 0xd38
			 0x4791d722, /// 0xd3c
			 0x885266a6, /// 0xd40
			 0xe1d6230a, /// 0xd44
			 0xd22b0c10, /// 0xd48
			 0x11a0b1c8, /// 0xd4c
			 0xb41fdb75, /// 0xd50
			 0xe1fd463d, /// 0xd54
			 0xf7c95fdc, /// 0xd58
			 0x9a9de82d, /// 0xd5c
			 0x4bdbd3bf, /// 0xd60
			 0x890edf6a, /// 0xd64
			 0x0abf80f6, /// 0xd68
			 0x35a098a7, /// 0xd6c
			 0xd7883d48, /// 0xd70
			 0x5a130ac3, /// 0xd74
			 0x47ded0b7, /// 0xd78
			 0xa6b0bfb3, /// 0xd7c
			 0x227e727a, /// 0xd80
			 0x8a8c8d96, /// 0xd84
			 0xedf1bca1, /// 0xd88
			 0x8ade758f, /// 0xd8c
			 0x62deea9e, /// 0xd90
			 0x70281753, /// 0xd94
			 0xefa102e5, /// 0xd98
			 0x5353cd7c, /// 0xd9c
			 0x1c5a1129, /// 0xda0
			 0xc060bedf, /// 0xda4
			 0x5e6a20be, /// 0xda8
			 0x014f65ce, /// 0xdac
			 0x2ae86cc9, /// 0xdb0
			 0x12d72049, /// 0xdb4
			 0x0f2c44a2, /// 0xdb8
			 0x85002119, /// 0xdbc
			 0x4b06568b, /// 0xdc0
			 0x7a94635e, /// 0xdc4
			 0x8dc42908, /// 0xdc8
			 0x30eb6a19, /// 0xdcc
			 0x83d9e3eb, /// 0xdd0
			 0x6376b854, /// 0xdd4
			 0x51abc5dc, /// 0xdd8
			 0xb98114ea, /// 0xddc
			 0x15d65e84, /// 0xde0
			 0xac3c47d4, /// 0xde4
			 0x49d7c071, /// 0xde8
			 0x96fa98db, /// 0xdec
			 0x30f5cef0, /// 0xdf0
			 0x5752b4ad, /// 0xdf4
			 0xec58594a, /// 0xdf8
			 0xa4689bee, /// 0xdfc
			 0xd8cc3b47, /// 0xe00
			 0x019c2ed0, /// 0xe04
			 0xb799827f, /// 0xe08
			 0xeff01b35, /// 0xe0c
			 0xba1f1ade, /// 0xe10
			 0xdf5a728e, /// 0xe14
			 0xfbbd2cd8, /// 0xe18
			 0xf9aa4776, /// 0xe1c
			 0xf2e3232b, /// 0xe20
			 0x5f77c64d, /// 0xe24
			 0xdda16a92, /// 0xe28
			 0x145993fa, /// 0xe2c
			 0x322ddec8, /// 0xe30
			 0x34cf7152, /// 0xe34
			 0x16784363, /// 0xe38
			 0xe0dcfb6c, /// 0xe3c
			 0x88467c49, /// 0xe40
			 0x7a1342e0, /// 0xe44
			 0x83a6c4f1, /// 0xe48
			 0x908d9e7f, /// 0xe4c
			 0x9e3e5e57, /// 0xe50
			 0xc851b13c, /// 0xe54
			 0x0d5e2e92, /// 0xe58
			 0xef61a881, /// 0xe5c
			 0x132d04fa, /// 0xe60
			 0x1d49f4ba, /// 0xe64
			 0x9eb143ee, /// 0xe68
			 0xd59e0d70, /// 0xe6c
			 0x1ab49de2, /// 0xe70
			 0x6b6ca97f, /// 0xe74
			 0x04068201, /// 0xe78
			 0x790ce188, /// 0xe7c
			 0x07e6ec08, /// 0xe80
			 0x38ad02cd, /// 0xe84
			 0xe38d7cad, /// 0xe88
			 0xa18712a4, /// 0xe8c
			 0x48927086, /// 0xe90
			 0x59750747, /// 0xe94
			 0x1f5a8eb6, /// 0xe98
			 0xeb3a4726, /// 0xe9c
			 0x7a1e0ac7, /// 0xea0
			 0x5b71126e, /// 0xea4
			 0xe2c1780b, /// 0xea8
			 0x629a93a5, /// 0xeac
			 0x60a5ff6f, /// 0xeb0
			 0x997d6c79, /// 0xeb4
			 0x48ec4053, /// 0xeb8
			 0x457531ce, /// 0xebc
			 0xd53fb66e, /// 0xec0
			 0xb5ce75a8, /// 0xec4
			 0x1b1f36a7, /// 0xec8
			 0x63141e3a, /// 0xecc
			 0xc562b287, /// 0xed0
			 0x7058f4fc, /// 0xed4
			 0x517e1787, /// 0xed8
			 0x0aff521c, /// 0xedc
			 0x1b1d1daf, /// 0xee0
			 0x1273822e, /// 0xee4
			 0x1f5199bd, /// 0xee8
			 0xa6e20444, /// 0xeec
			 0x6da3466e, /// 0xef0
			 0x055b7cb3, /// 0xef4
			 0x65667c84, /// 0xef8
			 0x185ee436, /// 0xefc
			 0x75261f45, /// 0xf00
			 0x35bc0d14, /// 0xf04
			 0x67f40113, /// 0xf08
			 0xc0736f90, /// 0xf0c
			 0x9c50c855, /// 0xf10
			 0x410eec18, /// 0xf14
			 0xfe56ea6e, /// 0xf18
			 0xe805503e, /// 0xf1c
			 0x9ce89874, /// 0xf20
			 0xef23b09d, /// 0xf24
			 0x4fd0f182, /// 0xf28
			 0xc222f916, /// 0xf2c
			 0x76b5f00d, /// 0xf30
			 0xee9867ac, /// 0xf34
			 0x24ba885f, /// 0xf38
			 0x4ec81b13, /// 0xf3c
			 0x1edbe341, /// 0xf40
			 0x4c15a814, /// 0xf44
			 0xf245683d, /// 0xf48
			 0x381ebe56, /// 0xf4c
			 0xab22d605, /// 0xf50
			 0x17a32037, /// 0xf54
			 0x0ff10f6b, /// 0xf58
			 0xac4de992, /// 0xf5c
			 0x35bf6032, /// 0xf60
			 0xa3d49893, /// 0xf64
			 0xb9f8039c, /// 0xf68
			 0xdd5e1d0d, /// 0xf6c
			 0x62684f7c, /// 0xf70
			 0x1537d7dc, /// 0xf74
			 0x19bfaec7, /// 0xf78
			 0xe7c4a7fe, /// 0xf7c
			 0xe4286375, /// 0xf80
			 0xbcb8ec0d, /// 0xf84
			 0x3f6a9aec, /// 0xf88
			 0x0be05a76, /// 0xf8c
			 0xc0918dc0, /// 0xf90
			 0xd5dc9eb4, /// 0xf94
			 0x04de3473, /// 0xf98
			 0x20d7e556, /// 0xf9c
			 0xf031f5e8, /// 0xfa0
			 0x4fc572a0, /// 0xfa4
			 0x2c5a7d3c, /// 0xfa8
			 0xa7693f82, /// 0xfac
			 0x7f87dfa9, /// 0xfb0
			 0x7266bbb8, /// 0xfb4
			 0x89377b63, /// 0xfb8
			 0x1b9b868a, /// 0xfbc
			 0xd64615bb, /// 0xfc0
			 0x9519a6c6, /// 0xfc4
			 0x01604479, /// 0xfc8
			 0xd5d203be, /// 0xfcc
			 0x1f331ad8, /// 0xfd0
			 0x9c15d30d, /// 0xfd4
			 0x0e78b5e5, /// 0xfd8
			 0x7a083bbf, /// 0xfdc
			 0x3f7b93e7, /// 0xfe0
			 0x5c78fb5a, /// 0xfe4
			 0xabb049d4, /// 0xfe8
			 0xc3e420de, /// 0xfec
			 0x050f9339, /// 0xff0
			 0xc7fe4791, /// 0xff4
			 0x9e97e5bf, /// 0xff8
			 0xc3a21e3a /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xde425271, /// 0x0
			 0x069f7239, /// 0x4
			 0xe89eb33a, /// 0x8
			 0xaa03e886, /// 0xc
			 0xf524f2c0, /// 0x10
			 0x2b94434e, /// 0x14
			 0x7d34b4ed, /// 0x18
			 0x46d1af39, /// 0x1c
			 0x03efbf37, /// 0x20
			 0xc980f3b9, /// 0x24
			 0x3b19699f, /// 0x28
			 0x2f7cc04c, /// 0x2c
			 0x34619432, /// 0x30
			 0x10d6d7c1, /// 0x34
			 0xf7d83739, /// 0x38
			 0x406a9a68, /// 0x3c
			 0x56950359, /// 0x40
			 0xb1b29aa1, /// 0x44
			 0xe809f8aa, /// 0x48
			 0x3e5d443c, /// 0x4c
			 0x9cae7fa0, /// 0x50
			 0xb0dad940, /// 0x54
			 0x1e18d0f0, /// 0x58
			 0xf2694a16, /// 0x5c
			 0x7bd54454, /// 0x60
			 0x8a10c060, /// 0x64
			 0x99262e63, /// 0x68
			 0x1c5b2980, /// 0x6c
			 0x56958b10, /// 0x70
			 0xdcd48df0, /// 0x74
			 0x535ad759, /// 0x78
			 0xcddd6078, /// 0x7c
			 0x3c885c57, /// 0x80
			 0xdff38a4e, /// 0x84
			 0xf0920715, /// 0x88
			 0x99b1ace0, /// 0x8c
			 0x8bdc594f, /// 0x90
			 0xf6e8458d, /// 0x94
			 0x0d26bf97, /// 0x98
			 0xef0dc799, /// 0x9c
			 0x76028dbf, /// 0xa0
			 0x5740ea9b, /// 0xa4
			 0x81e8a487, /// 0xa8
			 0xcd9d6755, /// 0xac
			 0x8df669ae, /// 0xb0
			 0x85fd9208, /// 0xb4
			 0x1b48b895, /// 0xb8
			 0x677390b5, /// 0xbc
			 0x63e0d6ec, /// 0xc0
			 0x9abdc9b4, /// 0xc4
			 0xf65db30a, /// 0xc8
			 0x5c4b7b93, /// 0xcc
			 0x09fe8abe, /// 0xd0
			 0x043d7fce, /// 0xd4
			 0x8a641696, /// 0xd8
			 0x64dfbb4d, /// 0xdc
			 0xbcfaf12a, /// 0xe0
			 0x22973447, /// 0xe4
			 0x284b4ff7, /// 0xe8
			 0x56fe3104, /// 0xec
			 0x6d842ff2, /// 0xf0
			 0xec62d2c7, /// 0xf4
			 0xb880052e, /// 0xf8
			 0xe022b095, /// 0xfc
			 0x8dfe1849, /// 0x100
			 0xbeb9bb9d, /// 0x104
			 0x5d762ae6, /// 0x108
			 0x1d02b244, /// 0x10c
			 0x396fab48, /// 0x110
			 0x3e000d82, /// 0x114
			 0xc04ebc3a, /// 0x118
			 0x381613b6, /// 0x11c
			 0xb94c87e6, /// 0x120
			 0x4bc23126, /// 0x124
			 0x92d1a9cd, /// 0x128
			 0x3dca55d0, /// 0x12c
			 0x032a427c, /// 0x130
			 0x3b2e7546, /// 0x134
			 0xea08ca40, /// 0x138
			 0x1d97a076, /// 0x13c
			 0x6b0a3ea0, /// 0x140
			 0xa361ed7d, /// 0x144
			 0x6d1e9ff9, /// 0x148
			 0x702d1f75, /// 0x14c
			 0x90fda486, /// 0x150
			 0x3b34166a, /// 0x154
			 0x577feefd, /// 0x158
			 0xba4041a6, /// 0x15c
			 0x0f378071, /// 0x160
			 0x38e1ed44, /// 0x164
			 0xe7da4155, /// 0x168
			 0x15cd01f1, /// 0x16c
			 0x9ed6dab1, /// 0x170
			 0x005828cb, /// 0x174
			 0x2bf1f40e, /// 0x178
			 0x209585ec, /// 0x17c
			 0x4cf886b6, /// 0x180
			 0x41bfbfba, /// 0x184
			 0x65cc27fd, /// 0x188
			 0x2efc2b3a, /// 0x18c
			 0x967dd028, /// 0x190
			 0x7fa30f7e, /// 0x194
			 0xcdfcbc27, /// 0x198
			 0x09452877, /// 0x19c
			 0x1fec7aab, /// 0x1a0
			 0x8878abf6, /// 0x1a4
			 0xc4df0d99, /// 0x1a8
			 0xd7079a07, /// 0x1ac
			 0x5c8c8aa9, /// 0x1b0
			 0xbd1335ad, /// 0x1b4
			 0x9bc7edd0, /// 0x1b8
			 0x2eedc34c, /// 0x1bc
			 0xd45a87e5, /// 0x1c0
			 0xa7ead52d, /// 0x1c4
			 0x0b1d85f7, /// 0x1c8
			 0x719ff1e0, /// 0x1cc
			 0xb5732d68, /// 0x1d0
			 0xad241cd4, /// 0x1d4
			 0x178650bc, /// 0x1d8
			 0xb373d8f4, /// 0x1dc
			 0xfbd386b6, /// 0x1e0
			 0x42c41353, /// 0x1e4
			 0x7dbc21a5, /// 0x1e8
			 0xf99d46cd, /// 0x1ec
			 0x43c21b81, /// 0x1f0
			 0xb99cc05b, /// 0x1f4
			 0x0f838fee, /// 0x1f8
			 0x121312ca, /// 0x1fc
			 0x8d0a747a, /// 0x200
			 0xdaa4fc26, /// 0x204
			 0xfc62f601, /// 0x208
			 0xc524f5bd, /// 0x20c
			 0xdf5ed636, /// 0x210
			 0xe195268c, /// 0x214
			 0x16cbb2b9, /// 0x218
			 0xe02e3893, /// 0x21c
			 0x320d48cf, /// 0x220
			 0xb67ad5e7, /// 0x224
			 0x9ae361c4, /// 0x228
			 0x28df2546, /// 0x22c
			 0x5c431902, /// 0x230
			 0xd915c804, /// 0x234
			 0x26c36011, /// 0x238
			 0xac41a452, /// 0x23c
			 0x01dd3d2d, /// 0x240
			 0xf246d58a, /// 0x244
			 0xf9754545, /// 0x248
			 0x77de2d63, /// 0x24c
			 0x1f4123ad, /// 0x250
			 0xd93f9736, /// 0x254
			 0xb637ae9b, /// 0x258
			 0xd5773d1f, /// 0x25c
			 0xe5015b78, /// 0x260
			 0x82d67412, /// 0x264
			 0xfd00c059, /// 0x268
			 0x7a03b7e7, /// 0x26c
			 0xad5c103d, /// 0x270
			 0xe2f448ba, /// 0x274
			 0xed3ba05c, /// 0x278
			 0xf022af05, /// 0x27c
			 0x4e6aabbc, /// 0x280
			 0x51728f84, /// 0x284
			 0x4d8ee582, /// 0x288
			 0x0e22dc7f, /// 0x28c
			 0xae692e63, /// 0x290
			 0x05b71547, /// 0x294
			 0x4760a54f, /// 0x298
			 0x7ef4e087, /// 0x29c
			 0xa989592d, /// 0x2a0
			 0xf66f7101, /// 0x2a4
			 0x1ac1aeae, /// 0x2a8
			 0xbe714254, /// 0x2ac
			 0x1dadf562, /// 0x2b0
			 0xddf022d8, /// 0x2b4
			 0xc7ffa3c0, /// 0x2b8
			 0x2065367b, /// 0x2bc
			 0xff50505d, /// 0x2c0
			 0xe32539fb, /// 0x2c4
			 0x3be50c97, /// 0x2c8
			 0xb45459c6, /// 0x2cc
			 0xc45d8f4b, /// 0x2d0
			 0x42d01ca9, /// 0x2d4
			 0x42598a57, /// 0x2d8
			 0x5064474e, /// 0x2dc
			 0xf2e5858a, /// 0x2e0
			 0x753efb76, /// 0x2e4
			 0xdf416d00, /// 0x2e8
			 0x04d6abfe, /// 0x2ec
			 0x3efaf101, /// 0x2f0
			 0x072625d4, /// 0x2f4
			 0xcfef552c, /// 0x2f8
			 0x601baa0a, /// 0x2fc
			 0x22bb9c96, /// 0x300
			 0xa4b8803a, /// 0x304
			 0x3f923f32, /// 0x308
			 0x1c73cbee, /// 0x30c
			 0x77d560fa, /// 0x310
			 0x80f6f5d5, /// 0x314
			 0xfd895c49, /// 0x318
			 0xc90daf07, /// 0x31c
			 0xbcb7bfd5, /// 0x320
			 0x70cf15d4, /// 0x324
			 0x7dbf3e03, /// 0x328
			 0xff691274, /// 0x32c
			 0x6bc40502, /// 0x330
			 0x6f354deb, /// 0x334
			 0x7c571e87, /// 0x338
			 0xbdd830b0, /// 0x33c
			 0xc1592114, /// 0x340
			 0x339e6621, /// 0x344
			 0xf9f558fd, /// 0x348
			 0x003fc7c7, /// 0x34c
			 0xa9268c58, /// 0x350
			 0x3fb4ca1e, /// 0x354
			 0x844bba45, /// 0x358
			 0x1d17aa8b, /// 0x35c
			 0x28ab55da, /// 0x360
			 0xb7346b9c, /// 0x364
			 0xa23b61cc, /// 0x368
			 0x74a3aff8, /// 0x36c
			 0x3739f8fc, /// 0x370
			 0x6bd4c8d8, /// 0x374
			 0x7a1fbed5, /// 0x378
			 0x6bf20b32, /// 0x37c
			 0x3c2b955c, /// 0x380
			 0xa86833be, /// 0x384
			 0xc4cce9b5, /// 0x388
			 0x3ea58c92, /// 0x38c
			 0x85b669a4, /// 0x390
			 0x9bdb1076, /// 0x394
			 0xf62305b4, /// 0x398
			 0x2d1bb9c8, /// 0x39c
			 0xa3c3530a, /// 0x3a0
			 0x3931e200, /// 0x3a4
			 0xc586a0f4, /// 0x3a8
			 0x40474aea, /// 0x3ac
			 0x5a303812, /// 0x3b0
			 0xc7f494f6, /// 0x3b4
			 0xd1983488, /// 0x3b8
			 0x9d730acf, /// 0x3bc
			 0x517fddf5, /// 0x3c0
			 0x861647e7, /// 0x3c4
			 0x877997fb, /// 0x3c8
			 0x2d31c022, /// 0x3cc
			 0xd2e557cf, /// 0x3d0
			 0xc758f54e, /// 0x3d4
			 0xe13defd6, /// 0x3d8
			 0xf5817f05, /// 0x3dc
			 0xecd6ef8e, /// 0x3e0
			 0xfdc9d685, /// 0x3e4
			 0x2de82439, /// 0x3e8
			 0xa4feec61, /// 0x3ec
			 0xc70d7a3e, /// 0x3f0
			 0xe85ebce2, /// 0x3f4
			 0x4035a6aa, /// 0x3f8
			 0x706fc199, /// 0x3fc
			 0x891ec869, /// 0x400
			 0xf501841b, /// 0x404
			 0x19f6a947, /// 0x408
			 0x560624fb, /// 0x40c
			 0x3cc488e8, /// 0x410
			 0x78a43819, /// 0x414
			 0xe2256282, /// 0x418
			 0xa43dfcc6, /// 0x41c
			 0x3a41fb93, /// 0x420
			 0xa0114eb9, /// 0x424
			 0xae12585b, /// 0x428
			 0x8eab268d, /// 0x42c
			 0xbd097324, /// 0x430
			 0x2a60de26, /// 0x434
			 0xd72539b2, /// 0x438
			 0x0be870f0, /// 0x43c
			 0xd91df98f, /// 0x440
			 0x60a2bc34, /// 0x444
			 0xb5100fc8, /// 0x448
			 0xb6f596ed, /// 0x44c
			 0x6c527fae, /// 0x450
			 0x67fb7702, /// 0x454
			 0xb63bc030, /// 0x458
			 0xe7761d70, /// 0x45c
			 0xc38c04f1, /// 0x460
			 0x6fd53386, /// 0x464
			 0xff04d873, /// 0x468
			 0xaacec3c2, /// 0x46c
			 0x5f308338, /// 0x470
			 0x095f6893, /// 0x474
			 0x66ed3ee9, /// 0x478
			 0x23174f9a, /// 0x47c
			 0xbd2b87a5, /// 0x480
			 0x11c9c851, /// 0x484
			 0xdd7b35a8, /// 0x488
			 0xa371e870, /// 0x48c
			 0x9d7209ae, /// 0x490
			 0xef547b25, /// 0x494
			 0x4d31f307, /// 0x498
			 0x51b8999d, /// 0x49c
			 0x115963f6, /// 0x4a0
			 0x3040620d, /// 0x4a4
			 0x27828ea9, /// 0x4a8
			 0xc044b191, /// 0x4ac
			 0xd181e843, /// 0x4b0
			 0x561811e4, /// 0x4b4
			 0xccf7feb7, /// 0x4b8
			 0xb75205ff, /// 0x4bc
			 0x6fcb3251, /// 0x4c0
			 0xceb0f892, /// 0x4c4
			 0x8d18ddb1, /// 0x4c8
			 0x2a17f4aa, /// 0x4cc
			 0x47bf1ec9, /// 0x4d0
			 0x35c4915d, /// 0x4d4
			 0xb1a24dcb, /// 0x4d8
			 0x6642470f, /// 0x4dc
			 0x5c2eeec5, /// 0x4e0
			 0xb1989dd1, /// 0x4e4
			 0x2568d000, /// 0x4e8
			 0x9278ec62, /// 0x4ec
			 0x27c015bc, /// 0x4f0
			 0x85220da1, /// 0x4f4
			 0x75af17fd, /// 0x4f8
			 0xe04c18a6, /// 0x4fc
			 0xeb8b3206, /// 0x500
			 0x2c66f6d2, /// 0x504
			 0x60fca513, /// 0x508
			 0xd3bc12dd, /// 0x50c
			 0xf021a124, /// 0x510
			 0x11eb9ac5, /// 0x514
			 0xbbac0eb9, /// 0x518
			 0xe9b665ab, /// 0x51c
			 0x5a93d16d, /// 0x520
			 0xf28874ab, /// 0x524
			 0x53e72cf6, /// 0x528
			 0xf9411dca, /// 0x52c
			 0x4d8452b5, /// 0x530
			 0x0c545417, /// 0x534
			 0xa1c4d191, /// 0x538
			 0x8f3c0c90, /// 0x53c
			 0x182b01c0, /// 0x540
			 0x56b046bb, /// 0x544
			 0xeccd787a, /// 0x548
			 0x6baab423, /// 0x54c
			 0x92824160, /// 0x550
			 0x289fc9f3, /// 0x554
			 0x245c7fe9, /// 0x558
			 0x6e8d52a9, /// 0x55c
			 0xd75b18ac, /// 0x560
			 0xf649d72c, /// 0x564
			 0x10f8ab22, /// 0x568
			 0x59c9a392, /// 0x56c
			 0x90b05ced, /// 0x570
			 0x2ca6b893, /// 0x574
			 0xcd90242b, /// 0x578
			 0x055e3014, /// 0x57c
			 0x60c3c62f, /// 0x580
			 0x70e8299d, /// 0x584
			 0x3740e25c, /// 0x588
			 0xde1ffcf2, /// 0x58c
			 0x7541d0c0, /// 0x590
			 0x578076f7, /// 0x594
			 0x825b5456, /// 0x598
			 0xd9042f81, /// 0x59c
			 0x4d7fab8f, /// 0x5a0
			 0xc34d42e2, /// 0x5a4
			 0x1fe29641, /// 0x5a8
			 0xe34e1d9a, /// 0x5ac
			 0x6de6a60c, /// 0x5b0
			 0x659d5318, /// 0x5b4
			 0x6091126e, /// 0x5b8
			 0x98d1ab2e, /// 0x5bc
			 0x6bbf5953, /// 0x5c0
			 0x84c35a49, /// 0x5c4
			 0x64a59e75, /// 0x5c8
			 0x2e8414b5, /// 0x5cc
			 0x38e586f0, /// 0x5d0
			 0xef02261a, /// 0x5d4
			 0x9e5b8f9f, /// 0x5d8
			 0xbc5bd654, /// 0x5dc
			 0x5f83ce41, /// 0x5e0
			 0x421f2a86, /// 0x5e4
			 0x949efab4, /// 0x5e8
			 0x826c1ae7, /// 0x5ec
			 0xb530a48a, /// 0x5f0
			 0x196c8366, /// 0x5f4
			 0x0919d599, /// 0x5f8
			 0xa0640afb, /// 0x5fc
			 0xb74fc925, /// 0x600
			 0x5ab43444, /// 0x604
			 0x8369ac63, /// 0x608
			 0x7a8b03c4, /// 0x60c
			 0x5cf8f3a4, /// 0x610
			 0x3a8a6913, /// 0x614
			 0x58a06cee, /// 0x618
			 0x9f5a8989, /// 0x61c
			 0x2f0c55c7, /// 0x620
			 0x1a513087, /// 0x624
			 0x5cebb45f, /// 0x628
			 0x62d532bc, /// 0x62c
			 0xb43b3c51, /// 0x630
			 0xd81f537c, /// 0x634
			 0x80eb91f3, /// 0x638
			 0x8ea866f1, /// 0x63c
			 0xa679e837, /// 0x640
			 0x3b8b342e, /// 0x644
			 0x94c092d6, /// 0x648
			 0xced4fa50, /// 0x64c
			 0xa45c8ebc, /// 0x650
			 0x99d249e6, /// 0x654
			 0x05ee3ba3, /// 0x658
			 0x414d42e1, /// 0x65c
			 0x896a6228, /// 0x660
			 0x42b49a50, /// 0x664
			 0xb46f26df, /// 0x668
			 0x6e7badf5, /// 0x66c
			 0xdd076276, /// 0x670
			 0x299752fe, /// 0x674
			 0xf3d1497d, /// 0x678
			 0xe09337c5, /// 0x67c
			 0xc8307042, /// 0x680
			 0x73a89d58, /// 0x684
			 0xef800f23, /// 0x688
			 0xe414b0d6, /// 0x68c
			 0x8ecdb286, /// 0x690
			 0xf473ce07, /// 0x694
			 0x376323f4, /// 0x698
			 0x8951d8a5, /// 0x69c
			 0x3e8888c0, /// 0x6a0
			 0x6ea5a067, /// 0x6a4
			 0x09eef875, /// 0x6a8
			 0xba7b93d5, /// 0x6ac
			 0x4201ee48, /// 0x6b0
			 0x4e54366e, /// 0x6b4
			 0x45d04b2c, /// 0x6b8
			 0xd26c737c, /// 0x6bc
			 0xb683f847, /// 0x6c0
			 0x2bfbc4ab, /// 0x6c4
			 0x036817e7, /// 0x6c8
			 0x3c14311f, /// 0x6cc
			 0x0f692afb, /// 0x6d0
			 0x4adeceae, /// 0x6d4
			 0xce15154c, /// 0x6d8
			 0xd6ace857, /// 0x6dc
			 0x3aafda7d, /// 0x6e0
			 0x71d34225, /// 0x6e4
			 0x23280198, /// 0x6e8
			 0x47a94916, /// 0x6ec
			 0x72aa9723, /// 0x6f0
			 0x24fc0e9a, /// 0x6f4
			 0x916d4900, /// 0x6f8
			 0x0ae6d978, /// 0x6fc
			 0x54900d7e, /// 0x700
			 0x02b05e56, /// 0x704
			 0x28668e02, /// 0x708
			 0xe3f85351, /// 0x70c
			 0x7cac69be, /// 0x710
			 0x2f644298, /// 0x714
			 0xba6f874e, /// 0x718
			 0x61979abb, /// 0x71c
			 0xe9be06af, /// 0x720
			 0x61f301f5, /// 0x724
			 0xe8cd6157, /// 0x728
			 0xb65125d5, /// 0x72c
			 0x6cea0546, /// 0x730
			 0x04c7dae7, /// 0x734
			 0x1db285b4, /// 0x738
			 0xacfea4d0, /// 0x73c
			 0xd5e9f14d, /// 0x740
			 0x844d6dbb, /// 0x744
			 0x87da4465, /// 0x748
			 0x4b91d062, /// 0x74c
			 0xf0c919b6, /// 0x750
			 0x88afefa5, /// 0x754
			 0xd015b181, /// 0x758
			 0xdd3e15d5, /// 0x75c
			 0xe6908c1c, /// 0x760
			 0x173c89bd, /// 0x764
			 0xeff24651, /// 0x768
			 0x7d159e54, /// 0x76c
			 0x40d66fd4, /// 0x770
			 0x47d6942f, /// 0x774
			 0x1ed4c9f4, /// 0x778
			 0x98302092, /// 0x77c
			 0xe99637e2, /// 0x780
			 0x58762958, /// 0x784
			 0x38e45396, /// 0x788
			 0x6239ad4c, /// 0x78c
			 0x34b0c6f4, /// 0x790
			 0x12fdbd31, /// 0x794
			 0xb653ea37, /// 0x798
			 0xa37abed9, /// 0x79c
			 0xb5af68ad, /// 0x7a0
			 0x07a691bf, /// 0x7a4
			 0x9751d126, /// 0x7a8
			 0xf8bd8146, /// 0x7ac
			 0xf9922789, /// 0x7b0
			 0x5a017d14, /// 0x7b4
			 0x853b7d4d, /// 0x7b8
			 0x6a60fc0f, /// 0x7bc
			 0x4807e0e1, /// 0x7c0
			 0xaf9a34d3, /// 0x7c4
			 0x72c5dd56, /// 0x7c8
			 0x88c116c2, /// 0x7cc
			 0x8986ae7d, /// 0x7d0
			 0x95d9697b, /// 0x7d4
			 0x92b9efa8, /// 0x7d8
			 0x2e6ebf50, /// 0x7dc
			 0x05e43c42, /// 0x7e0
			 0x78c45393, /// 0x7e4
			 0xdcbc5ec8, /// 0x7e8
			 0x0e0dab98, /// 0x7ec
			 0x005991c7, /// 0x7f0
			 0x0f8c0ad9, /// 0x7f4
			 0x7e2f4cef, /// 0x7f8
			 0xf4612314, /// 0x7fc
			 0x87e95be6, /// 0x800
			 0x32099f6a, /// 0x804
			 0xe4a78d60, /// 0x808
			 0x71ccdd0c, /// 0x80c
			 0x7d9e971e, /// 0x810
			 0x198f5072, /// 0x814
			 0xf9ce8db3, /// 0x818
			 0x9aae65be, /// 0x81c
			 0xa02ce3fb, /// 0x820
			 0x74c88532, /// 0x824
			 0xb41a24d9, /// 0x828
			 0xcae9b97d, /// 0x82c
			 0x722d8f5e, /// 0x830
			 0x15123732, /// 0x834
			 0x39dfec1e, /// 0x838
			 0x5e477bfc, /// 0x83c
			 0xdbae4b14, /// 0x840
			 0xb96d4c3e, /// 0x844
			 0x712bb985, /// 0x848
			 0x209f827d, /// 0x84c
			 0xdb4b1e11, /// 0x850
			 0x8dc129eb, /// 0x854
			 0xec362aba, /// 0x858
			 0xefab6b99, /// 0x85c
			 0xdd7ffaf4, /// 0x860
			 0x6d7b17a2, /// 0x864
			 0xd020b8df, /// 0x868
			 0xa3ec9f74, /// 0x86c
			 0x332c647f, /// 0x870
			 0x87059ca5, /// 0x874
			 0xfde456b4, /// 0x878
			 0xd694a19a, /// 0x87c
			 0x96bf4834, /// 0x880
			 0xa12dc816, /// 0x884
			 0xad287044, /// 0x888
			 0x66390d51, /// 0x88c
			 0xa590ebad, /// 0x890
			 0x16188e0d, /// 0x894
			 0xc9f12555, /// 0x898
			 0x85304df3, /// 0x89c
			 0x49d01dd8, /// 0x8a0
			 0x015247cc, /// 0x8a4
			 0x6a752951, /// 0x8a8
			 0xcbeca6ab, /// 0x8ac
			 0x8e8e7537, /// 0x8b0
			 0x73807538, /// 0x8b4
			 0x19d327d1, /// 0x8b8
			 0x0dd0ada6, /// 0x8bc
			 0x2250ec8d, /// 0x8c0
			 0x7a8409a8, /// 0x8c4
			 0x89df40d2, /// 0x8c8
			 0x339000fc, /// 0x8cc
			 0x7cd7e68b, /// 0x8d0
			 0xff0fd7fa, /// 0x8d4
			 0x79afc3b3, /// 0x8d8
			 0x7f1ec245, /// 0x8dc
			 0x2df9c56f, /// 0x8e0
			 0xf91c31d6, /// 0x8e4
			 0x9da26df5, /// 0x8e8
			 0x4b1b5125, /// 0x8ec
			 0x0259a293, /// 0x8f0
			 0x60dda32f, /// 0x8f4
			 0xe4335b5e, /// 0x8f8
			 0x43cd80fd, /// 0x8fc
			 0x6e036d09, /// 0x900
			 0x7c7b0539, /// 0x904
			 0x97028618, /// 0x908
			 0x7ba99434, /// 0x90c
			 0x6e393403, /// 0x910
			 0x2de3f4e3, /// 0x914
			 0xa2336ff1, /// 0x918
			 0xe23a2fa9, /// 0x91c
			 0x6e7bc201, /// 0x920
			 0x166f5e38, /// 0x924
			 0x2f55cb4c, /// 0x928
			 0xa4a6bdfa, /// 0x92c
			 0x6da17674, /// 0x930
			 0xedd0313d, /// 0x934
			 0x808f4ea3, /// 0x938
			 0x29002079, /// 0x93c
			 0x6e380886, /// 0x940
			 0xa52191cd, /// 0x944
			 0xbd569fc5, /// 0x948
			 0xedb30619, /// 0x94c
			 0x55a04113, /// 0x950
			 0x26a8d3b8, /// 0x954
			 0x01cc267e, /// 0x958
			 0x76049183, /// 0x95c
			 0xd1ef2af5, /// 0x960
			 0xf4b4cbc9, /// 0x964
			 0xf329540b, /// 0x968
			 0xffededcd, /// 0x96c
			 0xc0c40db4, /// 0x970
			 0xae8ed928, /// 0x974
			 0xd1f73910, /// 0x978
			 0xbe3ea76b, /// 0x97c
			 0x4b2d117e, /// 0x980
			 0x35351d13, /// 0x984
			 0x617ecad0, /// 0x988
			 0xd3103145, /// 0x98c
			 0x8bfaf86c, /// 0x990
			 0x35f195a9, /// 0x994
			 0xab597d09, /// 0x998
			 0xacafe869, /// 0x99c
			 0xb37c80a9, /// 0x9a0
			 0xf74e68b3, /// 0x9a4
			 0x3ec4a5e7, /// 0x9a8
			 0xe55353dc, /// 0x9ac
			 0x777ee90c, /// 0x9b0
			 0x64cf5445, /// 0x9b4
			 0x591bb1e6, /// 0x9b8
			 0x3b06f0c9, /// 0x9bc
			 0x8540089a, /// 0x9c0
			 0x6b5d5d63, /// 0x9c4
			 0x9a63bbac, /// 0x9c8
			 0x3603ccb4, /// 0x9cc
			 0xebb6f667, /// 0x9d0
			 0x0409eaf8, /// 0x9d4
			 0xdf00abcb, /// 0x9d8
			 0xf908b400, /// 0x9dc
			 0xe79cd5c8, /// 0x9e0
			 0x8dc10bca, /// 0x9e4
			 0x78993011, /// 0x9e8
			 0xfcfea52a, /// 0x9ec
			 0x67bec012, /// 0x9f0
			 0xbe982781, /// 0x9f4
			 0x6eeb9be5, /// 0x9f8
			 0x1d4cf554, /// 0x9fc
			 0x8156aa80, /// 0xa00
			 0x730b738a, /// 0xa04
			 0x64b860c3, /// 0xa08
			 0xe034e626, /// 0xa0c
			 0x883af663, /// 0xa10
			 0xbdc89ed6, /// 0xa14
			 0x27f792b1, /// 0xa18
			 0x3d07221f, /// 0xa1c
			 0x585296af, /// 0xa20
			 0x24af2b7d, /// 0xa24
			 0xce50207c, /// 0xa28
			 0xb96777b0, /// 0xa2c
			 0x8669707e, /// 0xa30
			 0x7909cb0c, /// 0xa34
			 0x40487618, /// 0xa38
			 0x24c1a8b4, /// 0xa3c
			 0x6ccf8577, /// 0xa40
			 0x89905df5, /// 0xa44
			 0x5bba77a6, /// 0xa48
			 0x491a7c98, /// 0xa4c
			 0x2ba804cb, /// 0xa50
			 0xb8bc2d0c, /// 0xa54
			 0xcf45034c, /// 0xa58
			 0x0259208f, /// 0xa5c
			 0xe982d9b4, /// 0xa60
			 0xbc22d336, /// 0xa64
			 0xbd23fd45, /// 0xa68
			 0x9110ed03, /// 0xa6c
			 0xe2f55460, /// 0xa70
			 0x697102bd, /// 0xa74
			 0x668bd9a1, /// 0xa78
			 0x785615b9, /// 0xa7c
			 0x62a68961, /// 0xa80
			 0x27ee0ebb, /// 0xa84
			 0xe3f588cc, /// 0xa88
			 0x14da458e, /// 0xa8c
			 0x707d8b4d, /// 0xa90
			 0x797c2f28, /// 0xa94
			 0xf2263fc0, /// 0xa98
			 0xbd188772, /// 0xa9c
			 0xbc468a0a, /// 0xaa0
			 0xb078833c, /// 0xaa4
			 0x0f010d31, /// 0xaa8
			 0x4ffdd4d8, /// 0xaac
			 0x5bac98dd, /// 0xab0
			 0xdf95f88b, /// 0xab4
			 0xbd300088, /// 0xab8
			 0x2c14de42, /// 0xabc
			 0xca31faa8, /// 0xac0
			 0x895ade84, /// 0xac4
			 0x7c589ab5, /// 0xac8
			 0x0c062076, /// 0xacc
			 0x7f6c5475, /// 0xad0
			 0x7d0439c8, /// 0xad4
			 0xc3507bd4, /// 0xad8
			 0xf76573e9, /// 0xadc
			 0xe64ce894, /// 0xae0
			 0x6e8c58c0, /// 0xae4
			 0x34a1fcc1, /// 0xae8
			 0xd89f6216, /// 0xaec
			 0x880710d2, /// 0xaf0
			 0xc73706aa, /// 0xaf4
			 0xf23e5ad1, /// 0xaf8
			 0x7dbb698b, /// 0xafc
			 0x5afda537, /// 0xb00
			 0x1b1e3fca, /// 0xb04
			 0x73e78f7b, /// 0xb08
			 0x1926d0bb, /// 0xb0c
			 0x96fe00c1, /// 0xb10
			 0x166075d3, /// 0xb14
			 0x2d38b92f, /// 0xb18
			 0x9cdf1fa2, /// 0xb1c
			 0xf00e9ffa, /// 0xb20
			 0xf62b6e84, /// 0xb24
			 0x711bdce5, /// 0xb28
			 0x16774918, /// 0xb2c
			 0x4915d32c, /// 0xb30
			 0x83ce5d95, /// 0xb34
			 0x1eeb490f, /// 0xb38
			 0x26475880, /// 0xb3c
			 0x95b0b04a, /// 0xb40
			 0x3343db8f, /// 0xb44
			 0x42c18a01, /// 0xb48
			 0xc5b55dbd, /// 0xb4c
			 0x483b474b, /// 0xb50
			 0xbce30875, /// 0xb54
			 0xf23f4d40, /// 0xb58
			 0x2c5758ce, /// 0xb5c
			 0x602fcce6, /// 0xb60
			 0xb25376ff, /// 0xb64
			 0x29a0217e, /// 0xb68
			 0x8625b914, /// 0xb6c
			 0x0a40931e, /// 0xb70
			 0x5c92c390, /// 0xb74
			 0xbee8ac58, /// 0xb78
			 0x28bdc2aa, /// 0xb7c
			 0x931a339f, /// 0xb80
			 0xcfe15d45, /// 0xb84
			 0x4dfce250, /// 0xb88
			 0x5edd8d41, /// 0xb8c
			 0x1206cdc7, /// 0xb90
			 0x9566a057, /// 0xb94
			 0x6d47574d, /// 0xb98
			 0xb2b31661, /// 0xb9c
			 0x82bb3a27, /// 0xba0
			 0x7e3d440c, /// 0xba4
			 0x4b2a9e55, /// 0xba8
			 0x17995fa1, /// 0xbac
			 0x156b4bc5, /// 0xbb0
			 0x2f964431, /// 0xbb4
			 0x2318192f, /// 0xbb8
			 0x84e1dba8, /// 0xbbc
			 0x8ed78ad9, /// 0xbc0
			 0x23b5a751, /// 0xbc4
			 0xea642b22, /// 0xbc8
			 0x68dfdeae, /// 0xbcc
			 0x46a7b147, /// 0xbd0
			 0xaa8dd3c1, /// 0xbd4
			 0x99f4ac19, /// 0xbd8
			 0xb4eb2c55, /// 0xbdc
			 0x51602df9, /// 0xbe0
			 0x60a7e691, /// 0xbe4
			 0x6743ba90, /// 0xbe8
			 0x3676ed62, /// 0xbec
			 0x7b154987, /// 0xbf0
			 0x17c76ce4, /// 0xbf4
			 0x0c8cb8b3, /// 0xbf8
			 0x8e227ef3, /// 0xbfc
			 0x2a8dda1c, /// 0xc00
			 0x8a9038b7, /// 0xc04
			 0xadf8b91f, /// 0xc08
			 0xdf2a298d, /// 0xc0c
			 0xa2f9ec35, /// 0xc10
			 0xe8ea3514, /// 0xc14
			 0x29deb93f, /// 0xc18
			 0x20459625, /// 0xc1c
			 0x7c0a06d7, /// 0xc20
			 0x91fd3e64, /// 0xc24
			 0x995bba7e, /// 0xc28
			 0xddb77870, /// 0xc2c
			 0x208f6f0e, /// 0xc30
			 0x4c32eb3c, /// 0xc34
			 0xfaa63469, /// 0xc38
			 0xab277787, /// 0xc3c
			 0x89b23788, /// 0xc40
			 0x1c797c22, /// 0xc44
			 0x1263110f, /// 0xc48
			 0xf458ca96, /// 0xc4c
			 0xcaaa292a, /// 0xc50
			 0xe50f0267, /// 0xc54
			 0xd825d24e, /// 0xc58
			 0xc25e7a76, /// 0xc5c
			 0x4f3d34d9, /// 0xc60
			 0x8c4cceba, /// 0xc64
			 0xbb42af56, /// 0xc68
			 0xe2a67450, /// 0xc6c
			 0x450a41ce, /// 0xc70
			 0xcc0e43b8, /// 0xc74
			 0x43d7a28d, /// 0xc78
			 0x39f07827, /// 0xc7c
			 0xcc94ce1c, /// 0xc80
			 0x364f9c7e, /// 0xc84
			 0xdfa2d85e, /// 0xc88
			 0x80cc0d1d, /// 0xc8c
			 0x5ca48d6d, /// 0xc90
			 0x74bbe6ff, /// 0xc94
			 0x3c0c4077, /// 0xc98
			 0xa6e196b7, /// 0xc9c
			 0x89574c65, /// 0xca0
			 0x4476df10, /// 0xca4
			 0xf30bff62, /// 0xca8
			 0x2b9d08eb, /// 0xcac
			 0x459d5e99, /// 0xcb0
			 0x4190644d, /// 0xcb4
			 0x773a818e, /// 0xcb8
			 0x6f25e8ba, /// 0xcbc
			 0x509b06c1, /// 0xcc0
			 0x74ae36bd, /// 0xcc4
			 0xabad82d4, /// 0xcc8
			 0x4f175ffc, /// 0xccc
			 0xbc649dc1, /// 0xcd0
			 0xe8166975, /// 0xcd4
			 0xc68458ef, /// 0xcd8
			 0xa070d737, /// 0xcdc
			 0xf237fb13, /// 0xce0
			 0xac9d70e7, /// 0xce4
			 0x97fd3704, /// 0xce8
			 0x4d8749ba, /// 0xcec
			 0xf36af5a4, /// 0xcf0
			 0x426e3cca, /// 0xcf4
			 0xbd92c5a0, /// 0xcf8
			 0x39792388, /// 0xcfc
			 0x9e15f109, /// 0xd00
			 0x9163e6a1, /// 0xd04
			 0x4b6258bf, /// 0xd08
			 0xdb91606d, /// 0xd0c
			 0x17f8f50c, /// 0xd10
			 0xf22cf25f, /// 0xd14
			 0xd4cbf511, /// 0xd18
			 0x636c0869, /// 0xd1c
			 0xcd134406, /// 0xd20
			 0xd669b366, /// 0xd24
			 0xe91deb9c, /// 0xd28
			 0x00e5a2b0, /// 0xd2c
			 0x2d70d132, /// 0xd30
			 0xfa678291, /// 0xd34
			 0xeebd2068, /// 0xd38
			 0xb41b7073, /// 0xd3c
			 0x1f2e9750, /// 0xd40
			 0x237e47d9, /// 0xd44
			 0x796306de, /// 0xd48
			 0xe6f6427b, /// 0xd4c
			 0xda63e8b5, /// 0xd50
			 0x5b3e8a0c, /// 0xd54
			 0xf2b5683c, /// 0xd58
			 0xd137af77, /// 0xd5c
			 0x6412cc19, /// 0xd60
			 0x80ff4f76, /// 0xd64
			 0x696edb07, /// 0xd68
			 0x0a5eb233, /// 0xd6c
			 0xc706b6d6, /// 0xd70
			 0x503eb7fb, /// 0xd74
			 0x26494eb8, /// 0xd78
			 0x6e4c8914, /// 0xd7c
			 0xe7e8de67, /// 0xd80
			 0xf82fd3fb, /// 0xd84
			 0x922076d4, /// 0xd88
			 0x1a9d18bc, /// 0xd8c
			 0x4eb3ce15, /// 0xd90
			 0x9a0b6345, /// 0xd94
			 0x586920da, /// 0xd98
			 0x0b42a7cf, /// 0xd9c
			 0x57179b1d, /// 0xda0
			 0x66f6e886, /// 0xda4
			 0x4c3aa3d7, /// 0xda8
			 0xc31a5aad, /// 0xdac
			 0x7f2c4f56, /// 0xdb0
			 0x7ef3e746, /// 0xdb4
			 0x54d58034, /// 0xdb8
			 0x2deb4429, /// 0xdbc
			 0x335e8105, /// 0xdc0
			 0x36affe48, /// 0xdc4
			 0x4fb1cc73, /// 0xdc8
			 0x1b18c5aa, /// 0xdcc
			 0x2b217711, /// 0xdd0
			 0xd99773be, /// 0xdd4
			 0x0a53d117, /// 0xdd8
			 0x6d7ead0f, /// 0xddc
			 0xc01e9202, /// 0xde0
			 0xf38907ee, /// 0xde4
			 0xb2ae2ae7, /// 0xde8
			 0x49ae9276, /// 0xdec
			 0xca4b29fc, /// 0xdf0
			 0x502281ad, /// 0xdf4
			 0x827f3c76, /// 0xdf8
			 0x7614ed88, /// 0xdfc
			 0x1241efc5, /// 0xe00
			 0xd2c7cf5b, /// 0xe04
			 0x49379815, /// 0xe08
			 0x0350bc2b, /// 0xe0c
			 0x116ab0a8, /// 0xe10
			 0xa2507e26, /// 0xe14
			 0xac2a92f8, /// 0xe18
			 0xbe7f1579, /// 0xe1c
			 0x23f676bc, /// 0xe20
			 0x95434b6c, /// 0xe24
			 0xc78ce7c4, /// 0xe28
			 0xabc00cb2, /// 0xe2c
			 0xa04f71fa, /// 0xe30
			 0xa4a1fa1b, /// 0xe34
			 0x5f2c6771, /// 0xe38
			 0x03084703, /// 0xe3c
			 0x96a973fb, /// 0xe40
			 0xc25adffd, /// 0xe44
			 0x6e48bf40, /// 0xe48
			 0xac251821, /// 0xe4c
			 0x2d57751e, /// 0xe50
			 0x9b8ef319, /// 0xe54
			 0x0a75271e, /// 0xe58
			 0xfe212c9c, /// 0xe5c
			 0x990a4ce2, /// 0xe60
			 0x9f3c47ae, /// 0xe64
			 0xcc610337, /// 0xe68
			 0x27b0637a, /// 0xe6c
			 0x659d52ac, /// 0xe70
			 0xc33e15de, /// 0xe74
			 0xc7d1f9b8, /// 0xe78
			 0xeddd9121, /// 0xe7c
			 0x2732c6f2, /// 0xe80
			 0x19f9b4aa, /// 0xe84
			 0xcb53e69e, /// 0xe88
			 0x9369d059, /// 0xe8c
			 0x3d023bba, /// 0xe90
			 0xf420a6b2, /// 0xe94
			 0xbafebf84, /// 0xe98
			 0xa4adb150, /// 0xe9c
			 0x508b0214, /// 0xea0
			 0x084732e7, /// 0xea4
			 0x8e3562dc, /// 0xea8
			 0x3409fdbe, /// 0xeac
			 0x3bacd775, /// 0xeb0
			 0xffee9350, /// 0xeb4
			 0x40de44ae, /// 0xeb8
			 0x9cd8731a, /// 0xebc
			 0xb1c3a31c, /// 0xec0
			 0x130479b3, /// 0xec4
			 0x54c0aeb0, /// 0xec8
			 0xbb7e210d, /// 0xecc
			 0x1963caec, /// 0xed0
			 0x92521602, /// 0xed4
			 0xe903840d, /// 0xed8
			 0xd14532bb, /// 0xedc
			 0x0b11e071, /// 0xee0
			 0xf398c80b, /// 0xee4
			 0x83952cd8, /// 0xee8
			 0xc6453dea, /// 0xeec
			 0xd34d99b4, /// 0xef0
			 0x64800c13, /// 0xef4
			 0x46635c4c, /// 0xef8
			 0x0b15dbf6, /// 0xefc
			 0x0c2002ec, /// 0xf00
			 0x4b6cc1f6, /// 0xf04
			 0xbd420ba2, /// 0xf08
			 0x503a58ec, /// 0xf0c
			 0x77a52d4b, /// 0xf10
			 0xe4078d76, /// 0xf14
			 0xa05a1c9a, /// 0xf18
			 0xc3e379ae, /// 0xf1c
			 0xff943012, /// 0xf20
			 0x6c44344d, /// 0xf24
			 0x799a8a37, /// 0xf28
			 0x469cacc3, /// 0xf2c
			 0x03ea7d64, /// 0xf30
			 0x88d5581a, /// 0xf34
			 0x4e781be5, /// 0xf38
			 0xbd2ed7d1, /// 0xf3c
			 0x946e28d3, /// 0xf40
			 0xa98db646, /// 0xf44
			 0x70a8f448, /// 0xf48
			 0xd529c822, /// 0xf4c
			 0x25eb94a2, /// 0xf50
			 0x25c773a9, /// 0xf54
			 0xbae413cd, /// 0xf58
			 0x2ae4656b, /// 0xf5c
			 0x3a3a43c7, /// 0xf60
			 0xd437aaf4, /// 0xf64
			 0x17247397, /// 0xf68
			 0xa31f9d72, /// 0xf6c
			 0xd26be4cf, /// 0xf70
			 0xa91fe444, /// 0xf74
			 0x9fb66383, /// 0xf78
			 0x664f0339, /// 0xf7c
			 0x240e3247, /// 0xf80
			 0xcbcc6a9a, /// 0xf84
			 0xdbe89a86, /// 0xf88
			 0x8395f327, /// 0xf8c
			 0xe0200abe, /// 0xf90
			 0x731fbac1, /// 0xf94
			 0x1f886662, /// 0xf98
			 0xb3878653, /// 0xf9c
			 0x2e781e3b, /// 0xfa0
			 0x61a094eb, /// 0xfa4
			 0x6ad97418, /// 0xfa8
			 0x5e67ed83, /// 0xfac
			 0xb45f3e1c, /// 0xfb0
			 0xe2858067, /// 0xfb4
			 0x38cc1102, /// 0xfb8
			 0x09fd7cd8, /// 0xfbc
			 0xf966a423, /// 0xfc0
			 0x7f52c810, /// 0xfc4
			 0x66558a78, /// 0xfc8
			 0x9822c986, /// 0xfcc
			 0xcce6a2fa, /// 0xfd0
			 0x86608122, /// 0xfd4
			 0x86b5e441, /// 0xfd8
			 0x31f68387, /// 0xfdc
			 0x7f67e2d2, /// 0xfe0
			 0x92168f8f, /// 0xfe4
			 0x31126966, /// 0xfe8
			 0x865eda93, /// 0xfec
			 0x5650eb82, /// 0xff0
			 0x95afc2b6, /// 0xff4
			 0x4de721c7, /// 0xff8
			 0x875aff51 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x38bb17f6, /// 0x0
			 0x300fd610, /// 0x4
			 0x64dc5aa3, /// 0x8
			 0xbd8d241a, /// 0xc
			 0x99e08277, /// 0x10
			 0x24ceb25f, /// 0x14
			 0x6e1aa57b, /// 0x18
			 0x73163bf3, /// 0x1c
			 0xf074ae90, /// 0x20
			 0xc6d134c8, /// 0x24
			 0x395d55eb, /// 0x28
			 0x07512e6e, /// 0x2c
			 0x5addf445, /// 0x30
			 0x99c40405, /// 0x34
			 0x920dd7a9, /// 0x38
			 0xb517e47c, /// 0x3c
			 0x4e6d2081, /// 0x40
			 0x8e19cd4c, /// 0x44
			 0x187e5184, /// 0x48
			 0x8aa589a9, /// 0x4c
			 0xda2fd3ea, /// 0x50
			 0x9154766c, /// 0x54
			 0x6b9f5ef1, /// 0x58
			 0xac02f464, /// 0x5c
			 0x54596ee8, /// 0x60
			 0x04646d26, /// 0x64
			 0xa1660c62, /// 0x68
			 0x25ee5418, /// 0x6c
			 0x8266abe1, /// 0x70
			 0x11a4cfae, /// 0x74
			 0xcdfb1f3b, /// 0x78
			 0x84412dca, /// 0x7c
			 0xde9d7673, /// 0x80
			 0x9aebd59f, /// 0x84
			 0xc55c1fbe, /// 0x88
			 0xe9cbbc11, /// 0x8c
			 0x8f717f83, /// 0x90
			 0xc87d6fba, /// 0x94
			 0xb4b4232d, /// 0x98
			 0xb457f520, /// 0x9c
			 0xff281906, /// 0xa0
			 0xdfcb90dd, /// 0xa4
			 0xbdc0ec9e, /// 0xa8
			 0x7e056cdd, /// 0xac
			 0x10cf4e07, /// 0xb0
			 0xdc9c905a, /// 0xb4
			 0xda2c1839, /// 0xb8
			 0x12b29d9f, /// 0xbc
			 0x89d55105, /// 0xc0
			 0x7749b89b, /// 0xc4
			 0xe6417047, /// 0xc8
			 0x78350123, /// 0xcc
			 0x24113ef6, /// 0xd0
			 0xcaf4b1f9, /// 0xd4
			 0xba59fd7c, /// 0xd8
			 0xfbee59d4, /// 0xdc
			 0x58b31820, /// 0xe0
			 0xfe297252, /// 0xe4
			 0xe8c48ec8, /// 0xe8
			 0x420e4983, /// 0xec
			 0xee5367ed, /// 0xf0
			 0xcc818108, /// 0xf4
			 0x4a0f876d, /// 0xf8
			 0x43baa7c1, /// 0xfc
			 0xfabd94cd, /// 0x100
			 0xf5b525f2, /// 0x104
			 0x83881cfd, /// 0x108
			 0x8dd19a1f, /// 0x10c
			 0x4436a941, /// 0x110
			 0x154ac874, /// 0x114
			 0x4e9c3c5a, /// 0x118
			 0x4eb567a1, /// 0x11c
			 0x51f70131, /// 0x120
			 0xc565f9d0, /// 0x124
			 0x5d8f3f9c, /// 0x128
			 0xd521045d, /// 0x12c
			 0x2288416f, /// 0x130
			 0xcea120c1, /// 0x134
			 0x054ef9ef, /// 0x138
			 0xa5387a36, /// 0x13c
			 0x25c4d9ef, /// 0x140
			 0x141fdb20, /// 0x144
			 0x00cf0505, /// 0x148
			 0xc35f5638, /// 0x14c
			 0xb91129bc, /// 0x150
			 0x3d315e36, /// 0x154
			 0x644a5065, /// 0x158
			 0xa596c92f, /// 0x15c
			 0xbdec5ebb, /// 0x160
			 0x6ec9716f, /// 0x164
			 0x1d3bb3e4, /// 0x168
			 0xf99af8ca, /// 0x16c
			 0xd4c35292, /// 0x170
			 0xe222c7ca, /// 0x174
			 0xe4c810a0, /// 0x178
			 0xe0a966a5, /// 0x17c
			 0xe3533fad, /// 0x180
			 0x3959720a, /// 0x184
			 0x07efa0c0, /// 0x188
			 0xb328d8fc, /// 0x18c
			 0x4e54f710, /// 0x190
			 0x1eaceb7c, /// 0x194
			 0xb4cae221, /// 0x198
			 0xad6a1a94, /// 0x19c
			 0xd8078a1c, /// 0x1a0
			 0x1814da09, /// 0x1a4
			 0x588ebeca, /// 0x1a8
			 0x569d12b6, /// 0x1ac
			 0xa72c9e46, /// 0x1b0
			 0x6d2ddef6, /// 0x1b4
			 0x957ea4df, /// 0x1b8
			 0xee5c48e1, /// 0x1bc
			 0x255edf6e, /// 0x1c0
			 0x5198277b, /// 0x1c4
			 0xc71f92cc, /// 0x1c8
			 0xd3d3a563, /// 0x1cc
			 0x2064e787, /// 0x1d0
			 0x1917310c, /// 0x1d4
			 0xe6261e6f, /// 0x1d8
			 0xb56917aa, /// 0x1dc
			 0x22dfd03f, /// 0x1e0
			 0x72b0d843, /// 0x1e4
			 0x036a0280, /// 0x1e8
			 0x6aa687ec, /// 0x1ec
			 0x94518048, /// 0x1f0
			 0x1611ffa4, /// 0x1f4
			 0x323f68b6, /// 0x1f8
			 0x711f32b2, /// 0x1fc
			 0xfe201e42, /// 0x200
			 0x1f3e9e2d, /// 0x204
			 0x0ca4d5ed, /// 0x208
			 0xbd954657, /// 0x20c
			 0x1c6d9a56, /// 0x210
			 0x8bfbe326, /// 0x214
			 0x4c362955, /// 0x218
			 0x533e7f5d, /// 0x21c
			 0x7c4e9329, /// 0x220
			 0xdc3a0dce, /// 0x224
			 0xeed48aab, /// 0x228
			 0xc0c18959, /// 0x22c
			 0xaac44c71, /// 0x230
			 0xba1efeca, /// 0x234
			 0x034e73dd, /// 0x238
			 0x0ebbd9f6, /// 0x23c
			 0x431426ad, /// 0x240
			 0x32694a99, /// 0x244
			 0xda1cd44c, /// 0x248
			 0x7f15fc17, /// 0x24c
			 0x3b57cc49, /// 0x250
			 0x235203b3, /// 0x254
			 0xd724ee5b, /// 0x258
			 0x89f6a5d4, /// 0x25c
			 0xef0d3f23, /// 0x260
			 0xe5f7055d, /// 0x264
			 0x52505b4b, /// 0x268
			 0x1a730dc0, /// 0x26c
			 0x4460b410, /// 0x270
			 0x2f0891be, /// 0x274
			 0x8ba25dad, /// 0x278
			 0x9e9d33e1, /// 0x27c
			 0xff42f7d3, /// 0x280
			 0x87b2dd61, /// 0x284
			 0xabb685b5, /// 0x288
			 0xef6649f7, /// 0x28c
			 0xe65f1692, /// 0x290
			 0xa46f24d4, /// 0x294
			 0x62c6c70a, /// 0x298
			 0xe4a3cb0f, /// 0x29c
			 0xd7a5a1e4, /// 0x2a0
			 0xa1f6992e, /// 0x2a4
			 0xd5a2c9ce, /// 0x2a8
			 0xc7a782c0, /// 0x2ac
			 0xdd805952, /// 0x2b0
			 0x12dc33af, /// 0x2b4
			 0x411fb445, /// 0x2b8
			 0x8bc67540, /// 0x2bc
			 0x8faea5e9, /// 0x2c0
			 0x4956d063, /// 0x2c4
			 0xd1e646d7, /// 0x2c8
			 0x22e3aa42, /// 0x2cc
			 0x6bf69f27, /// 0x2d0
			 0x9f3d3c39, /// 0x2d4
			 0xd984269c, /// 0x2d8
			 0x592d6af1, /// 0x2dc
			 0x3851358b, /// 0x2e0
			 0x96607232, /// 0x2e4
			 0xadaa9ee2, /// 0x2e8
			 0x969ca5d7, /// 0x2ec
			 0x3503f0b0, /// 0x2f0
			 0x8b179327, /// 0x2f4
			 0xe297de3f, /// 0x2f8
			 0xe079bc57, /// 0x2fc
			 0x594a97df, /// 0x300
			 0x2c6aae9d, /// 0x304
			 0x6c19e156, /// 0x308
			 0xdca20fa6, /// 0x30c
			 0x3396a129, /// 0x310
			 0xd1ab1169, /// 0x314
			 0xf6adbcc4, /// 0x318
			 0x44d77d9f, /// 0x31c
			 0x110ba149, /// 0x320
			 0xd59c9485, /// 0x324
			 0xa48302f1, /// 0x328
			 0x4c8a954a, /// 0x32c
			 0xbfd31179, /// 0x330
			 0x8bcd232c, /// 0x334
			 0xf1900135, /// 0x338
			 0x5f902254, /// 0x33c
			 0x03d6e6c9, /// 0x340
			 0x6178642f, /// 0x344
			 0x5f50d19a, /// 0x348
			 0xa7b88c4c, /// 0x34c
			 0x30cd44a2, /// 0x350
			 0xa0cad4f6, /// 0x354
			 0x2fe491dc, /// 0x358
			 0x8b83b4d3, /// 0x35c
			 0x57b0e8ba, /// 0x360
			 0xa2ab9fb9, /// 0x364
			 0x83da6b5c, /// 0x368
			 0xe7665d2d, /// 0x36c
			 0x178cc8e5, /// 0x370
			 0x4c9e3264, /// 0x374
			 0x0536a344, /// 0x378
			 0xb1ce4fb3, /// 0x37c
			 0xa46c2cd8, /// 0x380
			 0x016a1409, /// 0x384
			 0x730ab506, /// 0x388
			 0x4e778f0e, /// 0x38c
			 0x519d9858, /// 0x390
			 0x90a4b481, /// 0x394
			 0xe1a448cf, /// 0x398
			 0xce621eec, /// 0x39c
			 0x13aa5c5d, /// 0x3a0
			 0x315b0990, /// 0x3a4
			 0xa0da1a7a, /// 0x3a8
			 0x15b42255, /// 0x3ac
			 0xa4ef3321, /// 0x3b0
			 0xc00e704a, /// 0x3b4
			 0xdc5c1953, /// 0x3b8
			 0x4ff93ff5, /// 0x3bc
			 0x7ce8d129, /// 0x3c0
			 0xbffb8586, /// 0x3c4
			 0x52a19294, /// 0x3c8
			 0xcaa6753d, /// 0x3cc
			 0x4b5d69f6, /// 0x3d0
			 0x235f75ee, /// 0x3d4
			 0x3cde3df4, /// 0x3d8
			 0x9858a123, /// 0x3dc
			 0x4dc08127, /// 0x3e0
			 0x4dcb4b60, /// 0x3e4
			 0x20e645af, /// 0x3e8
			 0x9ea8c434, /// 0x3ec
			 0xe4af6c92, /// 0x3f0
			 0x8cdc368d, /// 0x3f4
			 0xd0071c66, /// 0x3f8
			 0x040aa7b8, /// 0x3fc
			 0x1015cffa, /// 0x400
			 0x899f6479, /// 0x404
			 0x0f4150b8, /// 0x408
			 0x548ae9c9, /// 0x40c
			 0x283e2499, /// 0x410
			 0x5f7ac1e4, /// 0x414
			 0x9c87a8a7, /// 0x418
			 0xae9894ef, /// 0x41c
			 0x2cd6de7b, /// 0x420
			 0x8ca4d93f, /// 0x424
			 0x8e5cfa3a, /// 0x428
			 0x9eb22947, /// 0x42c
			 0x91ef64c1, /// 0x430
			 0x2b3246e3, /// 0x434
			 0x460732e4, /// 0x438
			 0x1f4546e8, /// 0x43c
			 0xb0a1825d, /// 0x440
			 0xf1ce0f07, /// 0x444
			 0x7a6cc344, /// 0x448
			 0x6ef322dd, /// 0x44c
			 0x81cc0ce5, /// 0x450
			 0x0687aafe, /// 0x454
			 0xdb8c8d81, /// 0x458
			 0xb8253a52, /// 0x45c
			 0x2fb113c5, /// 0x460
			 0x2d3d722c, /// 0x464
			 0x61a36ab8, /// 0x468
			 0x794e6e37, /// 0x46c
			 0xcf2a665a, /// 0x470
			 0x4edee087, /// 0x474
			 0x0d0391ce, /// 0x478
			 0xb9583d75, /// 0x47c
			 0xc28ec7d7, /// 0x480
			 0x0961244e, /// 0x484
			 0x1c739c99, /// 0x488
			 0xc4e12891, /// 0x48c
			 0xc1502f87, /// 0x490
			 0xc7f7d409, /// 0x494
			 0xe5b59ff5, /// 0x498
			 0x3c8a5ef4, /// 0x49c
			 0x4b2d13f4, /// 0x4a0
			 0x73c6532c, /// 0x4a4
			 0x681ef151, /// 0x4a8
			 0x907a734d, /// 0x4ac
			 0xc90a4250, /// 0x4b0
			 0xb475bbcb, /// 0x4b4
			 0xe3595939, /// 0x4b8
			 0x54a20974, /// 0x4bc
			 0x373480f6, /// 0x4c0
			 0x809d57b6, /// 0x4c4
			 0x74020738, /// 0x4c8
			 0x31b10c6e, /// 0x4cc
			 0x920d8ca8, /// 0x4d0
			 0x2b161799, /// 0x4d4
			 0xc211f215, /// 0x4d8
			 0x51af3772, /// 0x4dc
			 0x1d2fe0ec, /// 0x4e0
			 0x14ae591e, /// 0x4e4
			 0x5b541487, /// 0x4e8
			 0x73482c2a, /// 0x4ec
			 0x5e6d74f0, /// 0x4f0
			 0xfde1fb88, /// 0x4f4
			 0x1a0602c3, /// 0x4f8
			 0x99fe1e2f, /// 0x4fc
			 0xb27b2c65, /// 0x500
			 0xb6263c52, /// 0x504
			 0x3ea1d42b, /// 0x508
			 0x4577ca75, /// 0x50c
			 0x60ac575b, /// 0x510
			 0x630aaec0, /// 0x514
			 0x4f00d97e, /// 0x518
			 0x0673cb25, /// 0x51c
			 0x481cbf20, /// 0x520
			 0xf3642afa, /// 0x524
			 0x85cf3d25, /// 0x528
			 0xf395d4f3, /// 0x52c
			 0x8b1340ea, /// 0x530
			 0xc93ac26c, /// 0x534
			 0x59ee0e07, /// 0x538
			 0x622766ea, /// 0x53c
			 0xb8388071, /// 0x540
			 0x88dc102a, /// 0x544
			 0x303c6fc9, /// 0x548
			 0xd2e2871e, /// 0x54c
			 0x7904f25b, /// 0x550
			 0xbea42df8, /// 0x554
			 0x515eb38c, /// 0x558
			 0x8dfc4660, /// 0x55c
			 0x168ac084, /// 0x560
			 0x30005fec, /// 0x564
			 0x36796909, /// 0x568
			 0xa81c23d5, /// 0x56c
			 0xa8d6652b, /// 0x570
			 0x4a37303d, /// 0x574
			 0x10c8fc66, /// 0x578
			 0x557ea840, /// 0x57c
			 0xab8101f4, /// 0x580
			 0xae992dd2, /// 0x584
			 0xdb546e6b, /// 0x588
			 0x0e08043a, /// 0x58c
			 0x7a4af226, /// 0x590
			 0x87299170, /// 0x594
			 0xdc296c75, /// 0x598
			 0x6d1c672b, /// 0x59c
			 0x7988b709, /// 0x5a0
			 0x2a1c8bdd, /// 0x5a4
			 0x6de2ac79, /// 0x5a8
			 0x919a5f14, /// 0x5ac
			 0xb228a489, /// 0x5b0
			 0x46b6d8a5, /// 0x5b4
			 0x550a0104, /// 0x5b8
			 0x3c728a80, /// 0x5bc
			 0x4b13cd0f, /// 0x5c0
			 0x90aa4832, /// 0x5c4
			 0x09bc8012, /// 0x5c8
			 0xe7e848ce, /// 0x5cc
			 0xaa53d421, /// 0x5d0
			 0xda747fbe, /// 0x5d4
			 0xb1780ec9, /// 0x5d8
			 0x8022a2f8, /// 0x5dc
			 0x85daa4f8, /// 0x5e0
			 0xa640f025, /// 0x5e4
			 0xb2ddc260, /// 0x5e8
			 0xe9937e8b, /// 0x5ec
			 0x56cb1483, /// 0x5f0
			 0xeed6caf6, /// 0x5f4
			 0x88915b99, /// 0x5f8
			 0x044d7416, /// 0x5fc
			 0x4aee23f2, /// 0x600
			 0x26c13ee7, /// 0x604
			 0x7a01b803, /// 0x608
			 0x30528bca, /// 0x60c
			 0x104f43b8, /// 0x610
			 0x73451268, /// 0x614
			 0x5aa10291, /// 0x618
			 0x1e6628e6, /// 0x61c
			 0xeade3df1, /// 0x620
			 0xff9a9798, /// 0x624
			 0x9075baae, /// 0x628
			 0x7d4aca0d, /// 0x62c
			 0x0c1ced3e, /// 0x630
			 0x207c0372, /// 0x634
			 0x0df978fd, /// 0x638
			 0x4cf18d10, /// 0x63c
			 0x47692215, /// 0x640
			 0x12be93c0, /// 0x644
			 0x02cd2560, /// 0x648
			 0x1bc7a20c, /// 0x64c
			 0x67846f74, /// 0x650
			 0x99cdf233, /// 0x654
			 0x0aa2ae1c, /// 0x658
			 0xd3b76041, /// 0x65c
			 0x6e376246, /// 0x660
			 0xbd78a7c5, /// 0x664
			 0x11faf92b, /// 0x668
			 0x674fd17b, /// 0x66c
			 0x3d0542a7, /// 0x670
			 0x2494de42, /// 0x674
			 0x23b74c1c, /// 0x678
			 0xe22b419f, /// 0x67c
			 0x83adedb4, /// 0x680
			 0x034c5792, /// 0x684
			 0xd01c6d36, /// 0x688
			 0x5f16703d, /// 0x68c
			 0x75c15562, /// 0x690
			 0x9802838f, /// 0x694
			 0x3cd914a1, /// 0x698
			 0x0c3f5201, /// 0x69c
			 0xfd90e197, /// 0x6a0
			 0xdd751c53, /// 0x6a4
			 0x77e9e2c3, /// 0x6a8
			 0xc3e6aa1a, /// 0x6ac
			 0x2fcdaecc, /// 0x6b0
			 0xa5ea4427, /// 0x6b4
			 0x513cfff6, /// 0x6b8
			 0x7ef058e9, /// 0x6bc
			 0x520bf9f2, /// 0x6c0
			 0xa9f2bfb2, /// 0x6c4
			 0xb4bcc443, /// 0x6c8
			 0xeb5370ef, /// 0x6cc
			 0xc63dae05, /// 0x6d0
			 0x12cbb4e9, /// 0x6d4
			 0x95e75739, /// 0x6d8
			 0x8a6cb205, /// 0x6dc
			 0x3924d3fd, /// 0x6e0
			 0xaa7f0653, /// 0x6e4
			 0x6274c8de, /// 0x6e8
			 0xa262ff23, /// 0x6ec
			 0x81981dd3, /// 0x6f0
			 0x9ee64aee, /// 0x6f4
			 0x2b37862e, /// 0x6f8
			 0x3226d710, /// 0x6fc
			 0x9226cc3c, /// 0x700
			 0x80302d64, /// 0x704
			 0x1e74fa3b, /// 0x708
			 0xa3eb0faa, /// 0x70c
			 0xf576cd7d, /// 0x710
			 0x30513321, /// 0x714
			 0xe7ea35ad, /// 0x718
			 0x4d04949f, /// 0x71c
			 0xff103517, /// 0x720
			 0x31dedd16, /// 0x724
			 0xe563765c, /// 0x728
			 0x92388b6b, /// 0x72c
			 0x32ff645a, /// 0x730
			 0x046f4c24, /// 0x734
			 0x79312cce, /// 0x738
			 0x9c32dcd8, /// 0x73c
			 0xbaceb811, /// 0x740
			 0xf689f86a, /// 0x744
			 0x8c8fefc0, /// 0x748
			 0x542a4f2d, /// 0x74c
			 0x5a53a9d8, /// 0x750
			 0xe8950dac, /// 0x754
			 0xc17e250b, /// 0x758
			 0xea71a3cc, /// 0x75c
			 0x379260ae, /// 0x760
			 0x742b1279, /// 0x764
			 0x97f09732, /// 0x768
			 0xd9fb771b, /// 0x76c
			 0xaf76c954, /// 0x770
			 0xab4a0a0d, /// 0x774
			 0x631201d8, /// 0x778
			 0xa471fb78, /// 0x77c
			 0x2895b048, /// 0x780
			 0xc074a293, /// 0x784
			 0x5ef82167, /// 0x788
			 0x61e58d2e, /// 0x78c
			 0xb38a1cd3, /// 0x790
			 0xd8303465, /// 0x794
			 0x1f8ffe67, /// 0x798
			 0xab9b15e2, /// 0x79c
			 0x9090254c, /// 0x7a0
			 0x5aef14f7, /// 0x7a4
			 0xf54ac660, /// 0x7a8
			 0xe0e909bf, /// 0x7ac
			 0x4459b6cb, /// 0x7b0
			 0x91c26799, /// 0x7b4
			 0x622c299f, /// 0x7b8
			 0x8ac90c5b, /// 0x7bc
			 0x16588c13, /// 0x7c0
			 0xd7100626, /// 0x7c4
			 0x3b424ff2, /// 0x7c8
			 0xfa362c45, /// 0x7cc
			 0xc7fc048e, /// 0x7d0
			 0x338d5a6c, /// 0x7d4
			 0x92d58dc5, /// 0x7d8
			 0x13c15bf3, /// 0x7dc
			 0x05ad8de5, /// 0x7e0
			 0xd3565d1e, /// 0x7e4
			 0x4428b4b4, /// 0x7e8
			 0x03bcdf7b, /// 0x7ec
			 0x32e43f1f, /// 0x7f0
			 0xce3b858a, /// 0x7f4
			 0x26428737, /// 0x7f8
			 0xd1608a42, /// 0x7fc
			 0x8b91395c, /// 0x800
			 0xb38282c8, /// 0x804
			 0xa9fa032f, /// 0x808
			 0xf49fbe73, /// 0x80c
			 0xce2e91ff, /// 0x810
			 0x6af084e8, /// 0x814
			 0x6db6b905, /// 0x818
			 0xbaa17062, /// 0x81c
			 0x2a78bbfb, /// 0x820
			 0x8364909a, /// 0x824
			 0x55137a71, /// 0x828
			 0x7a77bc83, /// 0x82c
			 0x647e2e28, /// 0x830
			 0xc265ea8d, /// 0x834
			 0x316e0bbd, /// 0x838
			 0xc9e6f012, /// 0x83c
			 0xc8f81b82, /// 0x840
			 0x3c30cd11, /// 0x844
			 0xc0972f5f, /// 0x848
			 0x1aa50a87, /// 0x84c
			 0x60797dab, /// 0x850
			 0x967f72af, /// 0x854
			 0xaac3a90c, /// 0x858
			 0xa5258092, /// 0x85c
			 0xa3fe937a, /// 0x860
			 0x48bf0a6f, /// 0x864
			 0x4bb4adc2, /// 0x868
			 0x236c4c29, /// 0x86c
			 0x79ecd378, /// 0x870
			 0x98a4b564, /// 0x874
			 0x237928b1, /// 0x878
			 0x9fbed309, /// 0x87c
			 0x6bf31b7e, /// 0x880
			 0x77811f29, /// 0x884
			 0xb78d640f, /// 0x888
			 0xd536e6c5, /// 0x88c
			 0xb66de8c5, /// 0x890
			 0xff7057ac, /// 0x894
			 0xc6378e85, /// 0x898
			 0xf56084f5, /// 0x89c
			 0x1978c124, /// 0x8a0
			 0x9b6d12a2, /// 0x8a4
			 0xd84fa2f3, /// 0x8a8
			 0xd160c2bc, /// 0x8ac
			 0x6059ea74, /// 0x8b0
			 0xc886b6ab, /// 0x8b4
			 0x491294b6, /// 0x8b8
			 0x91711616, /// 0x8bc
			 0xd0e242c6, /// 0x8c0
			 0x85daa544, /// 0x8c4
			 0x6f9ef484, /// 0x8c8
			 0x441f3f12, /// 0x8cc
			 0x3011d5f4, /// 0x8d0
			 0xaed17ec8, /// 0x8d4
			 0xa99f6990, /// 0x8d8
			 0xb2c33dea, /// 0x8dc
			 0x93891d7e, /// 0x8e0
			 0xa3e59137, /// 0x8e4
			 0x1770c2f5, /// 0x8e8
			 0x516b7499, /// 0x8ec
			 0x9d666117, /// 0x8f0
			 0xbe277ad1, /// 0x8f4
			 0x3287f297, /// 0x8f8
			 0x2122c07c, /// 0x8fc
			 0xe669479a, /// 0x900
			 0xefab37d3, /// 0x904
			 0x7cf1279a, /// 0x908
			 0xc4d703de, /// 0x90c
			 0x78b58743, /// 0x910
			 0xf73b15c7, /// 0x914
			 0x52ccb471, /// 0x918
			 0x5830510f, /// 0x91c
			 0x8e4dc6f7, /// 0x920
			 0x2d671c67, /// 0x924
			 0x484c8788, /// 0x928
			 0x7f629d07, /// 0x92c
			 0xb3a47aed, /// 0x930
			 0x802f6ad7, /// 0x934
			 0x82233b84, /// 0x938
			 0xb0e4c1ea, /// 0x93c
			 0x41d5ed83, /// 0x940
			 0x565617a3, /// 0x944
			 0xdb58d31f, /// 0x948
			 0x277c93c6, /// 0x94c
			 0x88320b03, /// 0x950
			 0xe0ae8a8a, /// 0x954
			 0x16fb076a, /// 0x958
			 0x8ae2ece1, /// 0x95c
			 0xef62bce0, /// 0x960
			 0xeebef559, /// 0x964
			 0x2155ec29, /// 0x968
			 0x754c2fa7, /// 0x96c
			 0xaea59404, /// 0x970
			 0x6e640297, /// 0x974
			 0x0349b8b6, /// 0x978
			 0xf4609e1f, /// 0x97c
			 0x614f0782, /// 0x980
			 0xbbdeccc7, /// 0x984
			 0xfc2eb736, /// 0x988
			 0xd9bb9bfe, /// 0x98c
			 0xb02da514, /// 0x990
			 0xca02e627, /// 0x994
			 0x5515def7, /// 0x998
			 0xad3a0b14, /// 0x99c
			 0xc729b499, /// 0x9a0
			 0xf5c7c73f, /// 0x9a4
			 0x14776e6b, /// 0x9a8
			 0x0238c31c, /// 0x9ac
			 0x9e506877, /// 0x9b0
			 0x4199ec65, /// 0x9b4
			 0xaf8af49b, /// 0x9b8
			 0xff80c8a2, /// 0x9bc
			 0xad99293e, /// 0x9c0
			 0x4f32b5b1, /// 0x9c4
			 0x3b9ca235, /// 0x9c8
			 0x19422cbc, /// 0x9cc
			 0xb528f73a, /// 0x9d0
			 0xa7907320, /// 0x9d4
			 0x4e4314ec, /// 0x9d8
			 0x9be8da7c, /// 0x9dc
			 0xb3877f47, /// 0x9e0
			 0xb8c52271, /// 0x9e4
			 0x4ffd6686, /// 0x9e8
			 0x560b0b89, /// 0x9ec
			 0xbf539383, /// 0x9f0
			 0x5e917c64, /// 0x9f4
			 0xa0ab3506, /// 0x9f8
			 0xf25a8da9, /// 0x9fc
			 0xe1c845e4, /// 0xa00
			 0x92d09c9a, /// 0xa04
			 0xd31a05fc, /// 0xa08
			 0x123e2c12, /// 0xa0c
			 0x29c380af, /// 0xa10
			 0x339062eb, /// 0xa14
			 0x596f5f8e, /// 0xa18
			 0xeda02bd6, /// 0xa1c
			 0xb2f161da, /// 0xa20
			 0xd24cca69, /// 0xa24
			 0xc0327392, /// 0xa28
			 0x92d6c5c1, /// 0xa2c
			 0x4f3a4a70, /// 0xa30
			 0x03bd9e77, /// 0xa34
			 0x4fa9688c, /// 0xa38
			 0xc45c9902, /// 0xa3c
			 0xf2ad710f, /// 0xa40
			 0xf78d9be6, /// 0xa44
			 0x2d55a06f, /// 0xa48
			 0x61f9cfd0, /// 0xa4c
			 0xd95b9e72, /// 0xa50
			 0x460bd22a, /// 0xa54
			 0x62acd7aa, /// 0xa58
			 0xdd150098, /// 0xa5c
			 0x92fe0290, /// 0xa60
			 0xda6a4ef5, /// 0xa64
			 0x083f5dc4, /// 0xa68
			 0x803aac1a, /// 0xa6c
			 0xbceb02cd, /// 0xa70
			 0xd55c4b6a, /// 0xa74
			 0xf83cd3d7, /// 0xa78
			 0x026e335e, /// 0xa7c
			 0x6923ed57, /// 0xa80
			 0x006004fd, /// 0xa84
			 0xb80f5f49, /// 0xa88
			 0x569a7d0b, /// 0xa8c
			 0xfd856f60, /// 0xa90
			 0x71d0c7c1, /// 0xa94
			 0x177c169b, /// 0xa98
			 0x8ba67fbd, /// 0xa9c
			 0x81e2f983, /// 0xaa0
			 0x1b7c134e, /// 0xaa4
			 0x0b632183, /// 0xaa8
			 0x73667d77, /// 0xaac
			 0xc92e0b8b, /// 0xab0
			 0xe4b52be5, /// 0xab4
			 0x09026e98, /// 0xab8
			 0x3e1ddef6, /// 0xabc
			 0x5603cb37, /// 0xac0
			 0x08bba5fe, /// 0xac4
			 0x4d732fba, /// 0xac8
			 0x180a8df4, /// 0xacc
			 0xde32edf6, /// 0xad0
			 0xf7454579, /// 0xad4
			 0x0649d171, /// 0xad8
			 0x5d55a5be, /// 0xadc
			 0x404c6c34, /// 0xae0
			 0xbed611a0, /// 0xae4
			 0x83a76279, /// 0xae8
			 0x17b8ffb9, /// 0xaec
			 0x4b93cd4d, /// 0xaf0
			 0xdf630af7, /// 0xaf4
			 0xa2c89f35, /// 0xaf8
			 0x2110936b, /// 0xafc
			 0x3efe8301, /// 0xb00
			 0x04494310, /// 0xb04
			 0xb88c1d4c, /// 0xb08
			 0xe5d01381, /// 0xb0c
			 0x473b7b70, /// 0xb10
			 0x5ce88af8, /// 0xb14
			 0x6202e0e7, /// 0xb18
			 0x08eddc2e, /// 0xb1c
			 0x93f2f623, /// 0xb20
			 0x9f88a5a7, /// 0xb24
			 0x60257a53, /// 0xb28
			 0x700a4440, /// 0xb2c
			 0x8ada35cd, /// 0xb30
			 0x397e2bea, /// 0xb34
			 0x04bd9e20, /// 0xb38
			 0x3a7a28cc, /// 0xb3c
			 0xefc8d2fe, /// 0xb40
			 0xf11c52d0, /// 0xb44
			 0x054a7e70, /// 0xb48
			 0x5ffbea43, /// 0xb4c
			 0x17b06978, /// 0xb50
			 0x0319e160, /// 0xb54
			 0xa5bed619, /// 0xb58
			 0x50938162, /// 0xb5c
			 0xe75c9c47, /// 0xb60
			 0xe3ee593a, /// 0xb64
			 0x4cd09bbc, /// 0xb68
			 0xb6c6e34c, /// 0xb6c
			 0xcd2070c8, /// 0xb70
			 0x6882e4eb, /// 0xb74
			 0x0e1022e0, /// 0xb78
			 0xeadc3818, /// 0xb7c
			 0x0c1e3c8a, /// 0xb80
			 0xb2c2a077, /// 0xb84
			 0xd2ec42da, /// 0xb88
			 0x47316a6f, /// 0xb8c
			 0xd8f34511, /// 0xb90
			 0x3ca974df, /// 0xb94
			 0x860aceb6, /// 0xb98
			 0x469be511, /// 0xb9c
			 0x930b71b5, /// 0xba0
			 0x01771625, /// 0xba4
			 0xe0638a03, /// 0xba8
			 0xc2bb0aab, /// 0xbac
			 0x44b9343d, /// 0xbb0
			 0x6256f88a, /// 0xbb4
			 0xaf17842b, /// 0xbb8
			 0x347611ef, /// 0xbbc
			 0x9f991599, /// 0xbc0
			 0xccefdbca, /// 0xbc4
			 0x3692d8cf, /// 0xbc8
			 0x22d79f99, /// 0xbcc
			 0x32605d64, /// 0xbd0
			 0x03c38e75, /// 0xbd4
			 0xe4e97c23, /// 0xbd8
			 0xce6478ed, /// 0xbdc
			 0x45d1a537, /// 0xbe0
			 0xaf699327, /// 0xbe4
			 0xd7b429df, /// 0xbe8
			 0x921f701f, /// 0xbec
			 0x02a99287, /// 0xbf0
			 0x4bc89be6, /// 0xbf4
			 0x8224cedf, /// 0xbf8
			 0x05ab5da8, /// 0xbfc
			 0x67b41bcd, /// 0xc00
			 0xab5aac98, /// 0xc04
			 0xf4b73019, /// 0xc08
			 0xb3012271, /// 0xc0c
			 0x01829d89, /// 0xc10
			 0xcfb58fe4, /// 0xc14
			 0xd77b53c9, /// 0xc18
			 0xdd70b23e, /// 0xc1c
			 0x7be6755d, /// 0xc20
			 0xb7fa239d, /// 0xc24
			 0x1e2e719a, /// 0xc28
			 0xaaeabd7b, /// 0xc2c
			 0x831bc30b, /// 0xc30
			 0xae10e94c, /// 0xc34
			 0xe4037bb4, /// 0xc38
			 0xac080441, /// 0xc3c
			 0x272af907, /// 0xc40
			 0xb240149d, /// 0xc44
			 0x370765ee, /// 0xc48
			 0x10a53144, /// 0xc4c
			 0xa1bd4b78, /// 0xc50
			 0x9eb3799e, /// 0xc54
			 0xfeba8fea, /// 0xc58
			 0x5b34c0d8, /// 0xc5c
			 0x6d07ce02, /// 0xc60
			 0x94821f2a, /// 0xc64
			 0x968ea36e, /// 0xc68
			 0x7e268656, /// 0xc6c
			 0x1f47b707, /// 0xc70
			 0x189c3f37, /// 0xc74
			 0x31fe2172, /// 0xc78
			 0x0b66463c, /// 0xc7c
			 0x788647b3, /// 0xc80
			 0x3b3c5011, /// 0xc84
			 0x469b5a84, /// 0xc88
			 0x3e361a38, /// 0xc8c
			 0x1a0f864c, /// 0xc90
			 0xf7862323, /// 0xc94
			 0x43610abc, /// 0xc98
			 0x63ba78d1, /// 0xc9c
			 0x7420a03a, /// 0xca0
			 0xb3400a93, /// 0xca4
			 0xe44b208e, /// 0xca8
			 0x655ec6b8, /// 0xcac
			 0x0ef0479b, /// 0xcb0
			 0x54ce8023, /// 0xcb4
			 0x2322fc68, /// 0xcb8
			 0xdf832d60, /// 0xcbc
			 0x19b1153b, /// 0xcc0
			 0x4948c280, /// 0xcc4
			 0xa006966f, /// 0xcc8
			 0xaca330ff, /// 0xccc
			 0x6de2cd3b, /// 0xcd0
			 0xa87ba3e0, /// 0xcd4
			 0x724da246, /// 0xcd8
			 0x963cec2e, /// 0xcdc
			 0x87baefb3, /// 0xce0
			 0x3b39ce59, /// 0xce4
			 0x902a4ff5, /// 0xce8
			 0xc586addd, /// 0xcec
			 0x7c5adb94, /// 0xcf0
			 0xd1a51677, /// 0xcf4
			 0x03ceb0bb, /// 0xcf8
			 0x62f3de42, /// 0xcfc
			 0x924b2d67, /// 0xd00
			 0x95a6baab, /// 0xd04
			 0x5f8b90fc, /// 0xd08
			 0xa1c7e753, /// 0xd0c
			 0xf8211a3b, /// 0xd10
			 0x182fee93, /// 0xd14
			 0x0451638e, /// 0xd18
			 0xe5bce4db, /// 0xd1c
			 0x569160e8, /// 0xd20
			 0xc52a14af, /// 0xd24
			 0xc9654a81, /// 0xd28
			 0x4effb1fb, /// 0xd2c
			 0x80cfd3f9, /// 0xd30
			 0x42b71cb1, /// 0xd34
			 0x59e11899, /// 0xd38
			 0x6b7d81f2, /// 0xd3c
			 0x67f131f0, /// 0xd40
			 0x025b6769, /// 0xd44
			 0x70980866, /// 0xd48
			 0xa2f7d78b, /// 0xd4c
			 0x892be4d6, /// 0xd50
			 0xb9135162, /// 0xd54
			 0xa7261b8f, /// 0xd58
			 0xb5a6b06f, /// 0xd5c
			 0x2eb02385, /// 0xd60
			 0x8409b038, /// 0xd64
			 0x1d30e318, /// 0xd68
			 0x47e97c20, /// 0xd6c
			 0x4c7e52e1, /// 0xd70
			 0x96b00ac9, /// 0xd74
			 0x4fc9c138, /// 0xd78
			 0x684ce43c, /// 0xd7c
			 0x3be41946, /// 0xd80
			 0x577ff817, /// 0xd84
			 0xa0bec418, /// 0xd88
			 0xe02873bb, /// 0xd8c
			 0x6c003fec, /// 0xd90
			 0x5be1646a, /// 0xd94
			 0x1800242e, /// 0xd98
			 0x5eab0d52, /// 0xd9c
			 0xffad4582, /// 0xda0
			 0x97dfe6f8, /// 0xda4
			 0x79a8efbc, /// 0xda8
			 0x2097eb73, /// 0xdac
			 0x3cf9c3f5, /// 0xdb0
			 0xd04c2e50, /// 0xdb4
			 0xe1c4f0d0, /// 0xdb8
			 0x2ee71772, /// 0xdbc
			 0x7e69fce6, /// 0xdc0
			 0xfbb95237, /// 0xdc4
			 0x8dc774d7, /// 0xdc8
			 0xd61e40cd, /// 0xdcc
			 0x57029c73, /// 0xdd0
			 0x4dfc169d, /// 0xdd4
			 0x38134791, /// 0xdd8
			 0xc7afc2c7, /// 0xddc
			 0x7a8590d7, /// 0xde0
			 0x70159d8e, /// 0xde4
			 0xf0eaa469, /// 0xde8
			 0x21cae51d, /// 0xdec
			 0x75220309, /// 0xdf0
			 0xbd2825a5, /// 0xdf4
			 0x24c1a071, /// 0xdf8
			 0x81d1dede, /// 0xdfc
			 0x54ab40fa, /// 0xe00
			 0x063c23b3, /// 0xe04
			 0xc3a276cd, /// 0xe08
			 0x2572a0b1, /// 0xe0c
			 0xbfcf744b, /// 0xe10
			 0xbc6291cd, /// 0xe14
			 0xce153ba7, /// 0xe18
			 0x1568ee0a, /// 0xe1c
			 0xa60f674c, /// 0xe20
			 0x1d0d6131, /// 0xe24
			 0x45c9b787, /// 0xe28
			 0xc27360b3, /// 0xe2c
			 0xc773d2f8, /// 0xe30
			 0x7184dc8a, /// 0xe34
			 0x5ed31296, /// 0xe38
			 0x79be8ba2, /// 0xe3c
			 0xc9b11bb1, /// 0xe40
			 0xe26cfc79, /// 0xe44
			 0x8d0fa345, /// 0xe48
			 0x7cece5bd, /// 0xe4c
			 0xd0074b8c, /// 0xe50
			 0x82a688f5, /// 0xe54
			 0x553ad7ff, /// 0xe58
			 0xbe38c7f5, /// 0xe5c
			 0x22f25ded, /// 0xe60
			 0xbe3fbbf7, /// 0xe64
			 0x8c8d2a06, /// 0xe68
			 0xee1b3afb, /// 0xe6c
			 0x5c661246, /// 0xe70
			 0xce871749, /// 0xe74
			 0xe24e5e5a, /// 0xe78
			 0x0e0745d2, /// 0xe7c
			 0xf54a4a44, /// 0xe80
			 0xd0423e06, /// 0xe84
			 0xb7073ed9, /// 0xe88
			 0x1c9b7aa7, /// 0xe8c
			 0x7965b394, /// 0xe90
			 0xf7782334, /// 0xe94
			 0xca3ef3b1, /// 0xe98
			 0x7e361f4c, /// 0xe9c
			 0x2c4383a3, /// 0xea0
			 0x39188ead, /// 0xea4
			 0x5f98ff18, /// 0xea8
			 0x07f78801, /// 0xeac
			 0x5e842a11, /// 0xeb0
			 0xa36341c3, /// 0xeb4
			 0xcf9570b6, /// 0xeb8
			 0xec89c864, /// 0xebc
			 0x52951a2c, /// 0xec0
			 0xb0c98682, /// 0xec4
			 0x63a752d2, /// 0xec8
			 0xc382858e, /// 0xecc
			 0xbb69ee7a, /// 0xed0
			 0xb6610968, /// 0xed4
			 0x61e5610c, /// 0xed8
			 0x7b708344, /// 0xedc
			 0x4e0a24e7, /// 0xee0
			 0x43db3e5d, /// 0xee4
			 0x8466717e, /// 0xee8
			 0xf390544e, /// 0xeec
			 0x1dcbde9f, /// 0xef0
			 0x7bcfaf9d, /// 0xef4
			 0x8ab9949a, /// 0xef8
			 0xe2a6b8d1, /// 0xefc
			 0xf4123424, /// 0xf00
			 0x2504f8e4, /// 0xf04
			 0x2f8e87c2, /// 0xf08
			 0x1c1a8081, /// 0xf0c
			 0x43d6ba7b, /// 0xf10
			 0xcdfdc193, /// 0xf14
			 0xec3e5082, /// 0xf18
			 0xf75b3ca1, /// 0xf1c
			 0xda2c6636, /// 0xf20
			 0xd3196e44, /// 0xf24
			 0xa6aea601, /// 0xf28
			 0xc0cf2671, /// 0xf2c
			 0xefa27785, /// 0xf30
			 0x774f0561, /// 0xf34
			 0xc90a16d6, /// 0xf38
			 0xd2eec432, /// 0xf3c
			 0x0b06e7f1, /// 0xf40
			 0x44f96c26, /// 0xf44
			 0xd718f47e, /// 0xf48
			 0xbae2aa12, /// 0xf4c
			 0x30ea0b2e, /// 0xf50
			 0x8f421565, /// 0xf54
			 0xf71204fc, /// 0xf58
			 0x92e68147, /// 0xf5c
			 0x3703c01e, /// 0xf60
			 0x969c8011, /// 0xf64
			 0x74beaddb, /// 0xf68
			 0x08cb2532, /// 0xf6c
			 0x8ddb5fe0, /// 0xf70
			 0xccada92c, /// 0xf74
			 0x1b733d9b, /// 0xf78
			 0xba4fb009, /// 0xf7c
			 0xc89a1bf6, /// 0xf80
			 0xf9817077, /// 0xf84
			 0xefe9717a, /// 0xf88
			 0x8b8a2822, /// 0xf8c
			 0xf3b0774e, /// 0xf90
			 0x57acd8b2, /// 0xf94
			 0xf81061f3, /// 0xf98
			 0x103be5b6, /// 0xf9c
			 0xf149c45e, /// 0xfa0
			 0xfb031221, /// 0xfa4
			 0x38d11dc6, /// 0xfa8
			 0x728c4ee2, /// 0xfac
			 0x04a17e7b, /// 0xfb0
			 0x2bbbf40e, /// 0xfb4
			 0xca79fffe, /// 0xfb8
			 0xcba98500, /// 0xfbc
			 0x5e995f78, /// 0xfc0
			 0x270b8d65, /// 0xfc4
			 0xd047c5db, /// 0xfc8
			 0xce548f4f, /// 0xfcc
			 0x48ec658c, /// 0xfd0
			 0x8028030e, /// 0xfd4
			 0x17c40cc3, /// 0xfd8
			 0x8c65a741, /// 0xfdc
			 0xbd3f1d15, /// 0xfe0
			 0x3264c526, /// 0xfe4
			 0x5f2385a1, /// 0xfe8
			 0xa5ff6307, /// 0xfec
			 0xc982a830, /// 0xff0
			 0x1e2ef08c, /// 0xff4
			 0xd61dc853, /// 0xff8
			 0xca321306 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00000
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00004
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00008
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80000000,                                                  // -zero                                       /// 00010
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00014
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00018
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0001c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00020
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00024
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0002c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00030
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x0e000007,                                                  // Trailing 1s:                                /// 00038
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0003c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00040
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00044
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00048
			 0x4b000000,                                                  // 8388608.0                                   /// 0004c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0005c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00060
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00064
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00068
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0006c
			 0x3f028f5c,                                                  // 0.51                                        /// 00070
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00074
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00078
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0007c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00080
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00088
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0008c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00090
			 0x00011111,                                                  // 9.7958E-41                                  /// 00094
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0009c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000c8
			 0xffc00001,                                                  // -signaling NaN                              /// 000cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0xff800000,                                                  // -inf                                        /// 000e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0xbf028f5c,                                                  // -0.51                                       /// 00104
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0010c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00110
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00114
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00118
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0011c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00120
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x0c400000,                                                  // Leading 1s:                                 /// 00128
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0012c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00134
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00138
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00140
			 0x7fc00001,                                                  // signaling NaN                               /// 00144
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00000000,                                                  // zero                                        /// 00164
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00174
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00178
			 0x0e000007,                                                  // Trailing 1s:                                /// 0017c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00184
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00188
			 0x4b000000,                                                  // 8388608.0                                   /// 0018c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00190
			 0x00000000,                                                  // zero                                        /// 00194
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00198
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0019c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 001bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x33333333,                                                  // 4 random values                             /// 001c8
			 0x55555555,                                                  // 4 random values                             /// 001cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 001d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00204
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00208
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0020c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x0e000007,                                                  // Trailing 1s:                                /// 0021c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00220
			 0x7fc00001,                                                  // signaling NaN                               /// 00224
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00228
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0022c
			 0x00000000,                                                  // zero                                        /// 00230
			 0x0c780000,                                                  // Leading 1s:                                 /// 00234
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0023c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00240
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00244
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0024c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00250
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00254
			 0x0c780000,                                                  // Leading 1s:                                 /// 00258
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00260
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00264
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00268
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0026c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00270
			 0x0e000001,                                                  // Trailing 1s:                                /// 00274
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00278
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00280
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00284
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00290
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00294
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00298
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0029c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00300
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00304
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00318
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0031c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00320
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00328
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0032c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00334
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00338
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0033c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0034c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00350
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00354
			 0x00000000,                                                  // zero                                        /// 00358
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0035c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00360
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00368
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0036c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00374
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00378
			 0x55555555,                                                  // 4 random values                             /// 0037c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00388
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0038c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00390
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00394
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00398
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0039c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x3f028f5c,                                                  // 0.51                                        /// 00404
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00408
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0040c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00410
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00418
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00420
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0042c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00430
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00438
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0043c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00440
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00444
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00448
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0044c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00450
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00458
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0045c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00460
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00464
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00468
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0046c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00470
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00474
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0047c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00480
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00484
			 0x3f028f5c,                                                  // 0.51                                        /// 00488
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00494
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00498
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x3f028f5c,                                                  // 0.51                                        /// 004e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00500
			 0x00000000,                                                  // zero                                        /// 00504
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00508
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00510
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x55555555,                                                  // 4 random values                             /// 00518
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0051c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00520
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00528
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00534
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00538
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0053c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00540
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0xcb000000,                                                  // -8388608.0                                  /// 0054c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00550
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00554
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00558
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0055c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00560
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00564
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00568
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0056c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00574
			 0xff800000,                                                  // -inf                                        /// 00578
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0057c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00580
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00584
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00588
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0058c
			 0xcb000000,                                                  // -8388608.0                                  /// 00590
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x33333333,                                                  // 4 random values                             /// 00598
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80000000,                                                  // -zero                                       /// 005b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005d0
			 0x7f800000,                                                  // inf                                         /// 005d4
			 0x3f028f5c,                                                  // 0.51                                        /// 005d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x7fc00001,                                                  // signaling NaN                               /// 005e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005f8
			 0x33333333,                                                  // 4 random values                             /// 005fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00600
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00604
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00608
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0060c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00614
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00618
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00620
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00624
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00628
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0062c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00630
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00634
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0064c
			 0x55555555,                                                  // 4 random values                             /// 00650
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00654
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00658
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0065c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0c700000,                                                  // Leading 1s:                                 /// 00664
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00668
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00670
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00674
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0067c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00680
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00684
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0068c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00690
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00698
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0069c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00700
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00708
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0070c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00718
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0071c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00720
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0072c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00730
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00734
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0073c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00740
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00744
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00748
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00754
			 0x0c780000,                                                  // Leading 1s:                                 /// 00758
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0075c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00764
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00768
			 0xff800000,                                                  // -inf                                        /// 0076c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00770
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00774
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00778
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00780
			 0x55555555,                                                  // 4 random values                             /// 00784
			 0x0c600000,                                                  // Leading 1s:                                 /// 00788
			 0x80011111,                                                  // -9.7958E-41                                 /// 0078c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00794
			 0x33333333,                                                  // 4 random values                             /// 00798
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0079c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007cc
			 0xcb000000,                                                  // -8388608.0                                  /// 007d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007f4
			 0xffc00001,                                                  // -signaling NaN                              /// 007f8
			 0xbf028f5c,                                                  // -0.51                                       /// 007fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00808
			 0xbf028f5c,                                                  // -0.51                                       /// 0080c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x33333333,                                                  // 4 random values                             /// 00814
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0081c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00824
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00828
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0082c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00854
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00858
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0085c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00864
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00868
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00870
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00874
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00878
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0087c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00880
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00888
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0088c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00890
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00894
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00898
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0089c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008a8
			 0x33333333,                                                  // 4 random values                             /// 008ac
			 0x33333333,                                                  // 4 random values                             /// 008b0
			 0xffc00001,                                                  // -signaling NaN                              /// 008b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00900
			 0x00011111,                                                  // 9.7958E-41                                  /// 00904
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00908
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00910
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00914
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00918
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0091c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00920
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00924
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0092c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00930
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00934
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00938
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00940
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00948
			 0x7fc00001,                                                  // signaling NaN                               /// 0094c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00950
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00958
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0095c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00960
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00964
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00968
			 0x0e000003,                                                  // Trailing 1s:                                /// 0096c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00970
			 0x3f028f5c,                                                  // 0.51                                        /// 00974
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00978
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0e000001,                                                  // Trailing 1s:                                /// 00984
			 0x3f028f5c,                                                  // 0.51                                        /// 00988
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0098c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00990
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00994
			 0x55555555,                                                  // 4 random values                             /// 00998
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0099c
			 0x33333333,                                                  // 4 random values                             /// 009a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009e0
			 0xbf028f5c,                                                  // -0.51                                       /// 009e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f4
			 0x4b000000,                                                  // 8388608.0                                   /// 009f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a1c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a24
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00000000,                                                  // zero                                        /// 00a2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a38
			 0x33333333,                                                  // 4 random values                             /// 00a3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a44
			 0x3f028f5c,                                                  // 0.51                                        /// 00a48
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a98
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a9c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00aa4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00aac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ab4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ab8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00abc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ac8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00acc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ad0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ae4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00af0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00af8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00afc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b04
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b10
			 0x7fc00001,                                                  // signaling NaN                               /// 00b14
			 0x55555555,                                                  // 4 random values                             /// 00b18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b20
			 0x55555555,                                                  // 4 random values                             /// 00b24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b2c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b34
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b6c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b74
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b7c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b80
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b94
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ba0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ba4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ba8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bb8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bc8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bd0
			 0x33333333,                                                  // 4 random values                             /// 00bd4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bdc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00be8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bf8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bfc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c00
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c14
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c40
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c74
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c80
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c98
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ca0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ca4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ca8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cb0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cd4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x00000000,                                                  // zero                                        /// 00ce8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cf0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cf4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cf8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cfc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d18
			 0x00000000,                                                  // zero                                        /// 00d1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d50
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d78
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d84
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00da0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00da8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00db0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00db4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00db8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dc4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dc8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dcc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00de0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00de4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00de8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00df0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00df8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e00
			 0x55555555,                                                  // 4 random values                             /// 00e04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e40
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e58
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x00000000,                                                  // zero                                        /// 00e70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e74
			 0xcb000000,                                                  // -8388608.0                                  /// 00e78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e84
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e9c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ea4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0xffc00001,                                                  // -signaling NaN                              /// 00eac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eb4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00eb8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ebc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ec0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ecc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ed4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ed8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00edc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ee0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ee4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ee8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ef0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ef4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x55555555,                                                  // 4 random values                             /// 00efc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f14
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f34
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x00000000,                                                  // zero                                        /// 00f9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fa0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fa8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fb0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fb8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fbc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fcc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fd8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fe0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fe4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ff8
			 0x00000020                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80000000,                                                  // -zero                                       /// 00000
			 0x0c400000,                                                  // Leading 1s:                                 /// 00004
			 0x0e000001,                                                  // Trailing 1s:                                /// 00008
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0000c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00010
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00014
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00018
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0001c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00020
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00028
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0002c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00030
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00034
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00040
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00044
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00048
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0004c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00050
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00054
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00058
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00060
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00068
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0006c
			 0x80000000,                                                  // -zero                                       /// 00070
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00074
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00078
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x33333333,                                                  // 4 random values                             /// 00080
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00084
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00090
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00094
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00098
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00100
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0010c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00118
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x3f028f5c,                                                  // 0.51                                        /// 00120
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00124
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00128
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00130
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00134
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00138
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00144
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0014c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00150
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00158
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00160
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00164
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00168
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0016c
			 0x33333333,                                                  // 4 random values                             /// 00170
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00174
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00178
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00180
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0xffc00001,                                                  // -signaling NaN                              /// 00188
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0018c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00194
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0019c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001ac
			 0x55555555,                                                  // 4 random values                             /// 001b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001d8
			 0x7fc00001,                                                  // signaling NaN                               /// 001dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001e8
			 0x4b000000,                                                  // 8388608.0                                   /// 001ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00204
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0020c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00210
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00214
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00220
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0022c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00234
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00238
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0023c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00240
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00248
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0024c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00250
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00260
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00268
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00270
			 0x3f028f5c,                                                  // 0.51                                        /// 00274
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0027c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00288
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00298
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0029c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002ec
			 0x33333333,                                                  // 4 random values                             /// 002f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00300
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00304
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00308
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0030c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00314
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00318
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0031c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00320
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00324
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x4b000000,                                                  // 8388608.0                                   /// 0032c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x3f028f5c,                                                  // 0.51                                        /// 00340
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00344
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00348
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0034c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00350
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00354
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00358
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0035c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00360
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00364
			 0xff800000,                                                  // -inf                                        /// 00368
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00370
			 0x0c400000,                                                  // Leading 1s:                                 /// 00374
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00378
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0037c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00380
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00384
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00388
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x00000000,                                                  // zero                                        /// 00390
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00394
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0039c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003a8
			 0xbf028f5c,                                                  // -0.51                                       /// 003ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003b4
			 0x3f028f5c,                                                  // 0.51                                        /// 003b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003d4
			 0x3f028f5c,                                                  // 0.51                                        /// 003d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 003f0
			 0xffc00001,                                                  // -signaling NaN                              /// 003f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 003fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00404
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00408
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0040c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00418
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0041c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00420
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00424
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00428
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00430
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00434
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00438
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00440
			 0x55555555,                                                  // 4 random values                             /// 00444
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0044c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00454
			 0x80011111,                                                  // -9.7958E-41                                 /// 00458
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00460
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00464
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00468
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0046c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00470
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00474
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00478
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0047c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00490
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00494
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00498
			 0x80011111,                                                  // -9.7958E-41                                 /// 0049c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004f0
			 0x7f800000,                                                  // inf                                         /// 004f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00500
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00508
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0050c
			 0x3f028f5c,                                                  // 0.51                                        /// 00510
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00514
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0051c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00528
			 0xff800000,                                                  // -inf                                        /// 0052c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00530
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00538
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0053c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00540
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00544
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0054c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00554
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00564
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00568
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0056c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00570
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0057c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00580
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00584
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00588
			 0x33333333,                                                  // 4 random values                             /// 0058c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00590
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00598
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x4b000000,                                                  // 8388608.0                                   /// 005dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x33333333,                                                  // 4 random values                             /// 005e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00600
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00604
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00608
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0060c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00610
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0061c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00620
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0e000007,                                                  // Trailing 1s:                                /// 00628
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0062c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00630
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0063c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00640
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00644
			 0x0c780000,                                                  // Leading 1s:                                 /// 00648
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0xffc00001,                                                  // -signaling NaN                              /// 00650
			 0xcb000000,                                                  // -8388608.0                                  /// 00654
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00658
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0065c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00660
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00664
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00668
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0066c
			 0x00000000,                                                  // zero                                        /// 00670
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00674
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00678
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00680
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00684
			 0x0c400000,                                                  // Leading 1s:                                 /// 00688
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0068c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00694
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00698
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0069c
			 0x0e000003,                                                  // Trailing 1s:                                /// 006a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x3f028f5c,                                                  // 0.51                                        /// 006b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x33333333,                                                  // 4 random values                             /// 006e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f0
			 0x00000000,                                                  // zero                                        /// 006f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00700
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00704
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0070c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00714
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0071c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00720
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00724
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00728
			 0x00000000,                                                  // zero                                        /// 0072c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00730
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00734
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00738
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0073c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00740
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00744
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0074c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00750
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00754
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00758
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0075c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00764
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00770
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x55555555,                                                  // 4 random values                             /// 00778
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0xffc00001,                                                  // -signaling NaN                              /// 00780
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00788
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0078c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00790
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00798
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x7fc00001,                                                  // signaling NaN                               /// 007a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007ec
			 0xffc00001,                                                  // -signaling NaN                              /// 007f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x7fc00001,                                                  // signaling NaN                               /// 00804
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00808
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00814
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0e000001,                                                  // Trailing 1s:                                /// 0081c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00824
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00830
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00834
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00840
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00844
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00848
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0084c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00858
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0085c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00868
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0086c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00870
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00874
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00878
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0087c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00880
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00884
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00888
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0088c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00890
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008a8
			 0x7f800000,                                                  // inf                                         /// 008ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008c4
			 0x7fc00001,                                                  // signaling NaN                               /// 008c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00904
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0090c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0091c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xcb000000,                                                  // -8388608.0                                  /// 00924
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0c700000,                                                  // Leading 1s:                                 /// 0092c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00930
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00934
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0093c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00940
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00944
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00948
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0094c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00950
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00954
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00958
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0095c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00960
			 0x80000000,                                                  // -zero                                       /// 00964
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00968
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0096c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00970
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00974
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0097c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00980
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00984
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00988
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0098c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00990
			 0x0e000003,                                                  // Trailing 1s:                                /// 00994
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0099c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x55555555,                                                  // 4 random values                             /// 009b4
			 0xbf028f5c,                                                  // -0.51                                       /// 009b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 009c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a00
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x7f800000,                                                  // inf                                         /// 00a1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a3c
			 0x33333333,                                                  // 4 random values                             /// 00a40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a48
			 0x33333333,                                                  // 4 random values                             /// 00a4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a50
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x33333333,                                                  // 4 random values                             /// 00a70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a98
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aa8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00aac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ab4
			 0xff800000,                                                  // -inf                                        /// 00ab8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00abc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ac0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ac4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ac8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00acc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ad0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ad4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00adc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ae0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ae8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00af0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00af8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b20
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b28
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b2c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b3c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b64
			 0x80000000,                                                  // -zero                                       /// 00b68
			 0x3f028f5c,                                                  // 0.51                                        /// 00b6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b94
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b98
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bb8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bbc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bc4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bd4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bd8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00be0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bf0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c64
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x80000000,                                                  // -zero                                       /// 00c94
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ca8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x7f800000,                                                  // inf                                         /// 00cb4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cc0
			 0xcb000000,                                                  // -8388608.0                                  /// 00cc4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ce4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cf8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d14
			 0xff800000,                                                  // -inf                                        /// 00d18
			 0x4b000000,                                                  // 8388608.0                                   /// 00d1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d5c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d88
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d98
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00da0
			 0x00000000,                                                  // zero                                        /// 00da4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00db0
			 0x80000000,                                                  // -zero                                       /// 00db4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00db8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dc4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ddc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00de0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00de4
			 0x33333333,                                                  // 4 random values                             /// 00de8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dec
			 0x80011111,                                                  // -9.7958E-41                                 /// 00df0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00df4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x33333333,                                                  // 4 random values                             /// 00dfc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x55555555,                                                  // 4 random values                             /// 00e10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e2c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e68
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e6c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e74
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e88
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x55555555,                                                  // 4 random values                             /// 00ea0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ea4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00eac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00eb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00eb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ebc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ec0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ec4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ec8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ecc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ed4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00edc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ee8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x00000000,                                                  // zero                                        /// 00ef4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ef8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00efc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f14
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f30
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f3c
			 0x55555555,                                                  // 4 random values                             /// 00f40
			 0x3f028f5c,                                                  // 0.51                                        /// 00f44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f4c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f5c
			 0x80000000,                                                  // -zero                                       /// 00f60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f84
			 0x33333333,                                                  // 4 random values                             /// 00f88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fa0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fa4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fb8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fbc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fc0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fc4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fcc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fd0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fdc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fe0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fe8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ff0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ff8
			 0x80000000                                                  // -zero                                       /// last
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
			 0x00000430,
			 0x00000754,
			 0x00000368,
			 0x0000055c,
			 0x00000720,
			 0x00000708,
			 0x00000658,
			 0x0000031c,

			 /// vpu register f2
			 0x40800000,
			 0x41d80000,
			 0x41200000,
			 0x41900000,
			 0x40c00000,
			 0x41200000,
			 0x40800000,
			 0x41e80000,

			 /// vpu register f3
			 0x41b80000,
			 0x41880000,
			 0x41e00000,
			 0x41e80000,
			 0x41400000,
			 0x41b00000,
			 0x40800000,
			 0x41000000,

			 /// vpu register f4
			 0x41b80000,
			 0x41e00000,
			 0x41f00000,
			 0x41a00000,
			 0x41880000,
			 0x40000000,
			 0x40800000,
			 0x42000000,

			 /// vpu register f5
			 0x40000000,
			 0x41e00000,
			 0x41000000,
			 0x41000000,
			 0x41800000,
			 0x41880000,
			 0x40c00000,
			 0x41100000,

			 /// vpu register f6
			 0x41500000,
			 0x41200000,
			 0x41400000,
			 0x40e00000,
			 0x40400000,
			 0x41e80000,
			 0x41b80000,
			 0x41500000,

			 /// vpu register f7
			 0x41c80000,
			 0x41c80000,
			 0x40e00000,
			 0x40c00000,
			 0x41980000,
			 0x41e00000,
			 0x41880000,
			 0x41f80000,

			 /// vpu register f8
			 0x41200000,
			 0x41d00000,
			 0x40400000,
			 0x41000000,
			 0x41e00000,
			 0x41c80000,
			 0x41100000,
			 0x41c00000,

			 /// vpu register f9
			 0x3f800000,
			 0x41100000,
			 0x40a00000,
			 0x42000000,
			 0x41800000,
			 0x3f800000,
			 0x40a00000,
			 0x41a00000,

			 /// vpu register f10
			 0x41b00000,
			 0x41c00000,
			 0x41800000,
			 0x3f800000,
			 0x41e80000,
			 0x41400000,
			 0x41e80000,
			 0x41200000,

			 /// vpu register f11
			 0x40800000,
			 0x41880000,
			 0x40000000,
			 0x40e00000,
			 0x40400000,
			 0x40e00000,
			 0x41c80000,
			 0x42000000,

			 /// vpu register f12
			 0x41500000,
			 0x40000000,
			 0x41400000,
			 0x41800000,
			 0x41700000,
			 0x41e00000,
			 0x41800000,
			 0x41c80000,

			 /// vpu register f13
			 0x41d80000,
			 0x41b80000,
			 0x41f80000,
			 0x41d00000,
			 0x41980000,
			 0x40c00000,
			 0x41000000,
			 0x41b00000,

			 /// vpu register f14
			 0x41200000,
			 0x41c00000,
			 0x41200000,
			 0x40400000,
			 0x41c80000,
			 0x41400000,
			 0x41600000,
			 0x41c80000,

			 /// vpu register f15
			 0x41c00000,
			 0x41100000,
			 0x41c80000,
			 0x41100000,
			 0x41500000,
			 0x41f00000,
			 0x41300000,
			 0x40000000,

			 /// vpu register f16
			 0x40a00000,
			 0x41a80000,
			 0x41800000,
			 0x41100000,
			 0x41700000,
			 0x41900000,
			 0x41200000,
			 0x41500000,

			 /// vpu register f17
			 0x41400000,
			 0x41d80000,
			 0x41980000,
			 0x41b80000,
			 0x41c80000,
			 0x41f80000,
			 0x41980000,
			 0x41980000,

			 /// vpu register f18
			 0x41980000,
			 0x40400000,
			 0x41800000,
			 0x41600000,
			 0x41d00000,
			 0x41a80000,
			 0x41e00000,
			 0x41980000,

			 /// vpu register f19
			 0x41000000,
			 0x41e80000,
			 0x41880000,
			 0x41b00000,
			 0x40c00000,
			 0x42000000,
			 0x41c80000,
			 0x41a00000,

			 /// vpu register f20
			 0x41f00000,
			 0x41b80000,
			 0x41900000,
			 0x41700000,
			 0x41000000,
			 0x41a80000,
			 0x41600000,
			 0x41d00000,

			 /// vpu register f21
			 0x41d80000,
			 0x3f800000,
			 0x41200000,
			 0x41200000,
			 0x41d80000,
			 0x41800000,
			 0x41a80000,
			 0x40c00000,

			 /// vpu register f22
			 0x41900000,
			 0x41d80000,
			 0x41c00000,
			 0x41400000,
			 0x40800000,
			 0x41880000,
			 0x41d00000,
			 0x41000000,

			 /// vpu register f23
			 0x41980000,
			 0x41600000,
			 0x41f80000,
			 0x41b00000,
			 0x41300000,
			 0x41800000,
			 0x41c00000,
			 0x41300000,

			 /// vpu register f24
			 0x41d00000,
			 0x41900000,
			 0x3f800000,
			 0x41d00000,
			 0x42000000,
			 0x41b80000,
			 0x41800000,
			 0x41a00000,

			 /// vpu register f25
			 0x41900000,
			 0x40400000,
			 0x41880000,
			 0x41700000,
			 0x41d00000,
			 0x41100000,
			 0x41700000,
			 0x41a80000,

			 /// vpu register f26
			 0x41400000,
			 0x41d80000,
			 0x41e80000,
			 0x41b80000,
			 0x41100000,
			 0x41200000,
			 0x41f80000,
			 0x41e00000,

			 /// vpu register f27
			 0x41000000,
			 0x40e00000,
			 0x40800000,
			 0x41c80000,
			 0x40400000,
			 0x41c00000,
			 0x41f00000,
			 0x41600000,

			 /// vpu register f28
			 0x41100000,
			 0x41f80000,
			 0x41a80000,
			 0x41c80000,
			 0x41700000,
			 0x41000000,
			 0x41500000,
			 0x41b00000,

			 /// vpu register f29
			 0x41700000,
			 0x41a80000,
			 0x40e00000,
			 0x41c80000,
			 0x40800000,
			 0x41d00000,
			 0x3f800000,
			 0x42000000,

			 /// vpu register f30
			 0x41f80000,
			 0x41e00000,
			 0x41a80000,
			 0x41000000,
			 0x41700000,
			 0x41c00000,
			 0x41300000,
			 0x41c00000,

			 /// vpu register f31
			 0x40e00000,
			 0x41880000,
			 0x41e00000,
			 0x3f800000,
			 0x41200000,
			 0x41600000,
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
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f29, f7\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f13, f5\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f13, f10\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f20, f13\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f29, f16\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f30, f27\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f8, f3\n"                                ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f30, f8\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f9, f10\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f28, f14\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f24, f18\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f13, f28\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f13, f14\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f28, f23\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f16, f24\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f11, f6\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f25, f14\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f8, f23\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f18, f24\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f6, f28\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f18, f27\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f18, f8\n"                               ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f3, f0\n"                                ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f29, f6\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f30, f6\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f10, f16\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f2, f1\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f21, f30\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f18, f29\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f14, f1\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f17, f23\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f21, f10\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f5, f28\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f24, f9\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f30, f14\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f2, f17\n"                               ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f29, f20\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f27, f0\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f28, f6\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f17, f28\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f25, f10\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f29, f15\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f19, f4\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f17, f1\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f11, f14\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f14, f30\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f6, f26\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f21, f26\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f3, f19\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f20, f2\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f3, f26\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f16, f10\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f8, f12\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f4, f17\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f26, f25\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f21, f22\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f20, f0\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f25, f30\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f13, f29\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f6, f11\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f23, f8\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f18, f20\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f21, f12\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f10, f9\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f23, f17\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f5, f28\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f11, f25\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f6, f18\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f22, f26\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f10, f4\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f18, f24\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f29, f4\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f15, f0\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f14, f26\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f29, f7\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f15, f18\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f22, f18\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f12, f19\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f7, f3\n"                                ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f17, f22\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f24, f21\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f28, f15\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f16, f18\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f23, f23\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f23, f8\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f15, f10\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f3, f5\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f2, f8\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f30, f29\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f25, f2\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f29, f22\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f10, f19\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f8, f16\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f6, f19\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f4, f0\n"                                ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f21, f2\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f30, f11\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f23, f6\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.f11 f2, f6\n"                                ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
