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
			 0x3de8a9fe, /// 0x0
			 0x2412fecc, /// 0x4
			 0x23da76b0, /// 0x8
			 0x8f8d9db2, /// 0xc
			 0x26750078, /// 0x10
			 0xc740612a, /// 0x14
			 0xda6d0122, /// 0x18
			 0x7a09e7d1, /// 0x1c
			 0x3991aa4d, /// 0x20
			 0xa04cab89, /// 0x24
			 0xb0af4b5d, /// 0x28
			 0x65e7d6b0, /// 0x2c
			 0x3d94f932, /// 0x30
			 0x19976c86, /// 0x34
			 0xb4cf2e72, /// 0x38
			 0x98c66993, /// 0x3c
			 0x8b49751b, /// 0x40
			 0x1026377f, /// 0x44
			 0x42925b02, /// 0x48
			 0x5a4841a3, /// 0x4c
			 0x91bd4d0e, /// 0x50
			 0x47fdbe82, /// 0x54
			 0x4a4d8360, /// 0x58
			 0x7d9cb751, /// 0x5c
			 0xcdc5f30d, /// 0x60
			 0xff0a4679, /// 0x64
			 0x33fed199, /// 0x68
			 0xae403900, /// 0x6c
			 0xd994e7ac, /// 0x70
			 0x77bf52e7, /// 0x74
			 0x2c41b2cf, /// 0x78
			 0xedbb7216, /// 0x7c
			 0x47f782c3, /// 0x80
			 0xd737b723, /// 0x84
			 0xd8172e99, /// 0x88
			 0xe3da2c20, /// 0x8c
			 0xa86e639f, /// 0x90
			 0x5f92a628, /// 0x94
			 0x3f5124f1, /// 0x98
			 0xb163491d, /// 0x9c
			 0xf14f7109, /// 0xa0
			 0xa94e4f68, /// 0xa4
			 0xde78a875, /// 0xa8
			 0x840931b4, /// 0xac
			 0x8b4b8c90, /// 0xb0
			 0x5d91d486, /// 0xb4
			 0x3958a198, /// 0xb8
			 0x83317463, /// 0xbc
			 0x30fac7c1, /// 0xc0
			 0x4d4d36fb, /// 0xc4
			 0x05080774, /// 0xc8
			 0x4b7b1838, /// 0xcc
			 0xb92fadad, /// 0xd0
			 0x1e28575c, /// 0xd4
			 0xb22fef42, /// 0xd8
			 0x63090d38, /// 0xdc
			 0x01652ccc, /// 0xe0
			 0x0c130f71, /// 0xe4
			 0x93be39c8, /// 0xe8
			 0x91960b2a, /// 0xec
			 0x514ff9b9, /// 0xf0
			 0x325c8eb8, /// 0xf4
			 0x8b7393d2, /// 0xf8
			 0x9c56e09a, /// 0xfc
			 0xccbe35f4, /// 0x100
			 0x809780a4, /// 0x104
			 0x4bedbb00, /// 0x108
			 0x8442af2c, /// 0x10c
			 0xeb7d7ef9, /// 0x110
			 0xd8d1cf3b, /// 0x114
			 0x99082def, /// 0x118
			 0xee0b70ad, /// 0x11c
			 0xaaf21ce4, /// 0x120
			 0xb77db00d, /// 0x124
			 0x00705d3d, /// 0x128
			 0x29f1438e, /// 0x12c
			 0x4e7de53b, /// 0x130
			 0x18369d95, /// 0x134
			 0x5b4f73e9, /// 0x138
			 0x37b2cd08, /// 0x13c
			 0xcf0c5e17, /// 0x140
			 0xf5da34f8, /// 0x144
			 0x9f4cb1dd, /// 0x148
			 0x77900e12, /// 0x14c
			 0x4037107a, /// 0x150
			 0x37a3332b, /// 0x154
			 0x7f2bbf2a, /// 0x158
			 0x57dec4b9, /// 0x15c
			 0x0b88c04c, /// 0x160
			 0x33b9438f, /// 0x164
			 0x1bbde855, /// 0x168
			 0x40e1cb0a, /// 0x16c
			 0x301fcbd0, /// 0x170
			 0xd7235acb, /// 0x174
			 0x940810ee, /// 0x178
			 0x47cb9f8a, /// 0x17c
			 0x7f1001fe, /// 0x180
			 0xb63a0fef, /// 0x184
			 0x0b5c2b9f, /// 0x188
			 0x7822446d, /// 0x18c
			 0x7776ea78, /// 0x190
			 0xc997b2c6, /// 0x194
			 0x2ada5933, /// 0x198
			 0xfa3aed79, /// 0x19c
			 0x6e475428, /// 0x1a0
			 0x2a1ca5bf, /// 0x1a4
			 0xa7c3ec3c, /// 0x1a8
			 0xcaef73de, /// 0x1ac
			 0xf3353df7, /// 0x1b0
			 0x57f3669c, /// 0x1b4
			 0x620629e5, /// 0x1b8
			 0x41cd50f1, /// 0x1bc
			 0x9f08bd9f, /// 0x1c0
			 0x57a507cd, /// 0x1c4
			 0xc91ce276, /// 0x1c8
			 0xe481467b, /// 0x1cc
			 0x8d9deffa, /// 0x1d0
			 0x91aedf35, /// 0x1d4
			 0x37a49af9, /// 0x1d8
			 0x38100595, /// 0x1dc
			 0x605306d6, /// 0x1e0
			 0x2bd40d62, /// 0x1e4
			 0xf3d69173, /// 0x1e8
			 0xdf010470, /// 0x1ec
			 0x7be3fece, /// 0x1f0
			 0x92a8616d, /// 0x1f4
			 0xf109b78a, /// 0x1f8
			 0x8a2df7d4, /// 0x1fc
			 0x7f78bb45, /// 0x200
			 0xa32bdd00, /// 0x204
			 0x76840379, /// 0x208
			 0xe1505bd7, /// 0x20c
			 0x41600639, /// 0x210
			 0x1593292c, /// 0x214
			 0x952c38aa, /// 0x218
			 0x1f84d06f, /// 0x21c
			 0x8ba41b33, /// 0x220
			 0x92b025ec, /// 0x224
			 0x7af82a52, /// 0x228
			 0x2a704149, /// 0x22c
			 0xe877bc4d, /// 0x230
			 0x90c4c606, /// 0x234
			 0xe2dbc432, /// 0x238
			 0x4ab6c607, /// 0x23c
			 0x52968ac3, /// 0x240
			 0x14f1dd67, /// 0x244
			 0xf4b50926, /// 0x248
			 0x0cc400a4, /// 0x24c
			 0x8307b0c2, /// 0x250
			 0x15926b92, /// 0x254
			 0x9c109a08, /// 0x258
			 0xa15d9030, /// 0x25c
			 0x768b9805, /// 0x260
			 0x25d125af, /// 0x264
			 0x6cd1f251, /// 0x268
			 0xcde5b32f, /// 0x26c
			 0x9b6bf38c, /// 0x270
			 0x5ba1fa06, /// 0x274
			 0xf9afd510, /// 0x278
			 0x5f14574f, /// 0x27c
			 0x616a8884, /// 0x280
			 0xa1e740a9, /// 0x284
			 0x93ecfa0b, /// 0x288
			 0xc3835e52, /// 0x28c
			 0x401cb9ce, /// 0x290
			 0x3c2a41aa, /// 0x294
			 0xa2711e56, /// 0x298
			 0xaddee85c, /// 0x29c
			 0x59f5d6e5, /// 0x2a0
			 0x0813be2c, /// 0x2a4
			 0xb1907d9d, /// 0x2a8
			 0xc93ff2b9, /// 0x2ac
			 0xaf4e1ae8, /// 0x2b0
			 0x0e22ff50, /// 0x2b4
			 0xb38c6375, /// 0x2b8
			 0xfcedea61, /// 0x2bc
			 0xd710f158, /// 0x2c0
			 0xde76cd1d, /// 0x2c4
			 0x5b8c9f36, /// 0x2c8
			 0x4714973c, /// 0x2cc
			 0xf767d0ad, /// 0x2d0
			 0x69e64102, /// 0x2d4
			 0xff4f6bd4, /// 0x2d8
			 0x492a94d5, /// 0x2dc
			 0x915a97f7, /// 0x2e0
			 0x0efb6801, /// 0x2e4
			 0xc8298031, /// 0x2e8
			 0xbf0163dd, /// 0x2ec
			 0x7b2aaa01, /// 0x2f0
			 0x44ad3162, /// 0x2f4
			 0xe0565c9f, /// 0x2f8
			 0x96730035, /// 0x2fc
			 0xa4099a78, /// 0x300
			 0x6f7b7763, /// 0x304
			 0x8315417f, /// 0x308
			 0x09a790a1, /// 0x30c
			 0x1ec2e6c6, /// 0x310
			 0x66dcf154, /// 0x314
			 0xd2ddf269, /// 0x318
			 0x697a87b2, /// 0x31c
			 0xc99b6143, /// 0x320
			 0x21472eb7, /// 0x324
			 0x66b24626, /// 0x328
			 0x34308682, /// 0x32c
			 0xc5d8d702, /// 0x330
			 0x97e0686e, /// 0x334
			 0x0719d295, /// 0x338
			 0x4fd54878, /// 0x33c
			 0xba293546, /// 0x340
			 0x70ef1d5c, /// 0x344
			 0xaa83583f, /// 0x348
			 0x69d32f8c, /// 0x34c
			 0xa11dd2fc, /// 0x350
			 0x527000d4, /// 0x354
			 0x44da4094, /// 0x358
			 0x066b2da3, /// 0x35c
			 0x915df0c3, /// 0x360
			 0xc1dd8b91, /// 0x364
			 0x032c1111, /// 0x368
			 0xd2722ee2, /// 0x36c
			 0x69e14bde, /// 0x370
			 0xa423088a, /// 0x374
			 0x7eebd2bf, /// 0x378
			 0x3f83f60e, /// 0x37c
			 0xefb18934, /// 0x380
			 0x736f094c, /// 0x384
			 0x50946934, /// 0x388
			 0xeec42b7f, /// 0x38c
			 0xb19d6f4f, /// 0x390
			 0xa54cff10, /// 0x394
			 0x1ea49064, /// 0x398
			 0xf0f6e09a, /// 0x39c
			 0x9127cb30, /// 0x3a0
			 0x679b6bfb, /// 0x3a4
			 0xd0e5f1aa, /// 0x3a8
			 0x3afe5c4e, /// 0x3ac
			 0x2c1dd5b1, /// 0x3b0
			 0xae954f7f, /// 0x3b4
			 0x5fa7da52, /// 0x3b8
			 0xe430515c, /// 0x3bc
			 0x2f4d56e6, /// 0x3c0
			 0xb27192ab, /// 0x3c4
			 0x7c0d0d01, /// 0x3c8
			 0x57746f63, /// 0x3cc
			 0xcd96d9d0, /// 0x3d0
			 0xeb1dab3a, /// 0x3d4
			 0x4452e908, /// 0x3d8
			 0x7b84cb9b, /// 0x3dc
			 0x2724583a, /// 0x3e0
			 0xae02903a, /// 0x3e4
			 0x6625abbd, /// 0x3e8
			 0x47f494c0, /// 0x3ec
			 0xa1146e27, /// 0x3f0
			 0xac136b13, /// 0x3f4
			 0x27532aae, /// 0x3f8
			 0xd0b87885, /// 0x3fc
			 0x85dbe0d6, /// 0x400
			 0x14b6c162, /// 0x404
			 0xcbb60e64, /// 0x408
			 0x6cb00473, /// 0x40c
			 0xe916a05c, /// 0x410
			 0xa3a9c16d, /// 0x414
			 0x1c05d61e, /// 0x418
			 0x891b0db6, /// 0x41c
			 0x6a7e9fda, /// 0x420
			 0xe0fdf08c, /// 0x424
			 0x156fd249, /// 0x428
			 0xd65cd457, /// 0x42c
			 0x8ac46a40, /// 0x430
			 0x531708ee, /// 0x434
			 0x0dd2555e, /// 0x438
			 0x55d29f95, /// 0x43c
			 0x7df9c06f, /// 0x440
			 0x0abad2ab, /// 0x444
			 0x4f83eb58, /// 0x448
			 0xe71a0792, /// 0x44c
			 0xc787d7b9, /// 0x450
			 0x9951c4f5, /// 0x454
			 0x9621ecda, /// 0x458
			 0xb94f9f67, /// 0x45c
			 0xd3ac151f, /// 0x460
			 0x87f5f556, /// 0x464
			 0x4b9ad497, /// 0x468
			 0xac975786, /// 0x46c
			 0xfa7bce46, /// 0x470
			 0xe343db7a, /// 0x474
			 0x81b657d5, /// 0x478
			 0x11fb318f, /// 0x47c
			 0x61d87e28, /// 0x480
			 0x6f09b322, /// 0x484
			 0xd40703b6, /// 0x488
			 0x795b168d, /// 0x48c
			 0x7c9c1c82, /// 0x490
			 0x55df999a, /// 0x494
			 0xebdae8ba, /// 0x498
			 0xf054c7d3, /// 0x49c
			 0x003ba1d5, /// 0x4a0
			 0x1b435d67, /// 0x4a4
			 0x9151df18, /// 0x4a8
			 0x2f521a5f, /// 0x4ac
			 0xf45c58e4, /// 0x4b0
			 0x0feb6577, /// 0x4b4
			 0x6989e5fa, /// 0x4b8
			 0x05c22ec7, /// 0x4bc
			 0xd4015d0f, /// 0x4c0
			 0xed16a9c9, /// 0x4c4
			 0x03c6a832, /// 0x4c8
			 0xb878c06d, /// 0x4cc
			 0x2872b4f8, /// 0x4d0
			 0x6d517a70, /// 0x4d4
			 0x9022c909, /// 0x4d8
			 0x43cb2091, /// 0x4dc
			 0x61ab6e96, /// 0x4e0
			 0xa7508f83, /// 0x4e4
			 0xb486a4b3, /// 0x4e8
			 0x37d0a6e5, /// 0x4ec
			 0x4450a17c, /// 0x4f0
			 0xa4874bdd, /// 0x4f4
			 0xfaed2cfa, /// 0x4f8
			 0x68224857, /// 0x4fc
			 0xc6c5af96, /// 0x500
			 0x2b3b8210, /// 0x504
			 0x30714d97, /// 0x508
			 0x403a6a80, /// 0x50c
			 0x2b465cc9, /// 0x510
			 0x0f3862e9, /// 0x514
			 0xb7beee8a, /// 0x518
			 0x154d656c, /// 0x51c
			 0x2a015141, /// 0x520
			 0xb201f0da, /// 0x524
			 0xb5d49948, /// 0x528
			 0xa65ef26b, /// 0x52c
			 0x667d1b0d, /// 0x530
			 0xc7368b97, /// 0x534
			 0xe5d4742f, /// 0x538
			 0x408add75, /// 0x53c
			 0x2721347e, /// 0x540
			 0x031efef5, /// 0x544
			 0xe10d6aab, /// 0x548
			 0x5420adde, /// 0x54c
			 0x8369995c, /// 0x550
			 0xc7a72bcf, /// 0x554
			 0x830704fa, /// 0x558
			 0x2c49c227, /// 0x55c
			 0xdc78291c, /// 0x560
			 0xa1a2e559, /// 0x564
			 0x970b4575, /// 0x568
			 0x74ec670a, /// 0x56c
			 0xc362c7ce, /// 0x570
			 0x3078edbf, /// 0x574
			 0x993c642a, /// 0x578
			 0x088f60b3, /// 0x57c
			 0x5e315d4d, /// 0x580
			 0x858fed3e, /// 0x584
			 0x1a824cf2, /// 0x588
			 0xbbe676bb, /// 0x58c
			 0xab348935, /// 0x590
			 0xa9f8ec90, /// 0x594
			 0x835b6cfc, /// 0x598
			 0xa1614576, /// 0x59c
			 0x2200bc82, /// 0x5a0
			 0xba837500, /// 0x5a4
			 0x35e172bf, /// 0x5a8
			 0xc2158ca4, /// 0x5ac
			 0x76a97e03, /// 0x5b0
			 0x49cc8eb7, /// 0x5b4
			 0x55dd45f6, /// 0x5b8
			 0x97664f5e, /// 0x5bc
			 0x857277bf, /// 0x5c0
			 0x30fe3286, /// 0x5c4
			 0x1d79cf2d, /// 0x5c8
			 0x9eb2c86a, /// 0x5cc
			 0xbdd1a31f, /// 0x5d0
			 0x52f3379a, /// 0x5d4
			 0x681f60e3, /// 0x5d8
			 0x4105f905, /// 0x5dc
			 0x8904d87f, /// 0x5e0
			 0x9308f210, /// 0x5e4
			 0x91829c0f, /// 0x5e8
			 0xf127273d, /// 0x5ec
			 0x416c1c05, /// 0x5f0
			 0xebba8dde, /// 0x5f4
			 0x912340cd, /// 0x5f8
			 0x4972631e, /// 0x5fc
			 0x8be3d8b2, /// 0x600
			 0x56e86753, /// 0x604
			 0x2c913692, /// 0x608
			 0x120c4531, /// 0x60c
			 0x87769be7, /// 0x610
			 0x167138a4, /// 0x614
			 0x54c454db, /// 0x618
			 0x74e66677, /// 0x61c
			 0xf9ac9351, /// 0x620
			 0x1507b40b, /// 0x624
			 0xaf52332b, /// 0x628
			 0x198d3595, /// 0x62c
			 0x1e71d421, /// 0x630
			 0x3dbe4d71, /// 0x634
			 0xf25292cb, /// 0x638
			 0x906b6504, /// 0x63c
			 0x73a18005, /// 0x640
			 0xdb91d672, /// 0x644
			 0xbe8da285, /// 0x648
			 0xfecacabe, /// 0x64c
			 0x3c93fec8, /// 0x650
			 0xc8c7edf9, /// 0x654
			 0x1b50b20a, /// 0x658
			 0x7831f5dc, /// 0x65c
			 0x7bc31595, /// 0x660
			 0xb956814d, /// 0x664
			 0x30991bf4, /// 0x668
			 0x93e1e047, /// 0x66c
			 0x7ee6488e, /// 0x670
			 0x5f2a4e17, /// 0x674
			 0x8f6a32f5, /// 0x678
			 0x62504250, /// 0x67c
			 0xf7dce25e, /// 0x680
			 0xdc698e7c, /// 0x684
			 0x2f877950, /// 0x688
			 0x51174d43, /// 0x68c
			 0xf23c0946, /// 0x690
			 0xf5d3a005, /// 0x694
			 0xdfbf2329, /// 0x698
			 0x0f75b962, /// 0x69c
			 0x6b7236a1, /// 0x6a0
			 0x3c15e487, /// 0x6a4
			 0xf5b739c1, /// 0x6a8
			 0x8c26a184, /// 0x6ac
			 0xfdb1b8e7, /// 0x6b0
			 0x3f0bc756, /// 0x6b4
			 0xbefb1dcd, /// 0x6b8
			 0x483ad4d1, /// 0x6bc
			 0xaf25ca8f, /// 0x6c0
			 0x5a701493, /// 0x6c4
			 0x48be23aa, /// 0x6c8
			 0x4854a60d, /// 0x6cc
			 0x28248b27, /// 0x6d0
			 0x1321ac5a, /// 0x6d4
			 0xdd7acb75, /// 0x6d8
			 0x22e8c195, /// 0x6dc
			 0x34d7d9c6, /// 0x6e0
			 0x11ef42d3, /// 0x6e4
			 0x3d167a67, /// 0x6e8
			 0x0185e057, /// 0x6ec
			 0xeece63b7, /// 0x6f0
			 0x2f9fe680, /// 0x6f4
			 0x1c07822f, /// 0x6f8
			 0xe58acfc4, /// 0x6fc
			 0x1dc23d6c, /// 0x700
			 0x6064fa8c, /// 0x704
			 0x55d8a3e1, /// 0x708
			 0xad2fa22d, /// 0x70c
			 0xce9294fe, /// 0x710
			 0x3713194e, /// 0x714
			 0xc365a4bd, /// 0x718
			 0xc2e6eed5, /// 0x71c
			 0xb9579edd, /// 0x720
			 0xf662c310, /// 0x724
			 0xd738b1c7, /// 0x728
			 0x84e4168c, /// 0x72c
			 0xa9c7c988, /// 0x730
			 0x3d8b3da6, /// 0x734
			 0x5c2b5261, /// 0x738
			 0x83e9f8e3, /// 0x73c
			 0x38bf3093, /// 0x740
			 0x6d11e198, /// 0x744
			 0xb6fe71e8, /// 0x748
			 0xde4f9717, /// 0x74c
			 0x004880d2, /// 0x750
			 0xc0347d3e, /// 0x754
			 0xcf9c2ea5, /// 0x758
			 0x141a57f9, /// 0x75c
			 0x508333ca, /// 0x760
			 0x10c68b40, /// 0x764
			 0x65907998, /// 0x768
			 0xb7137ad8, /// 0x76c
			 0x246fe7f7, /// 0x770
			 0xc5870ca6, /// 0x774
			 0xf247bd0d, /// 0x778
			 0x2e29589a, /// 0x77c
			 0xedfebf08, /// 0x780
			 0x609ae4e5, /// 0x784
			 0x9707833a, /// 0x788
			 0x973c615d, /// 0x78c
			 0x25463706, /// 0x790
			 0xa7daef68, /// 0x794
			 0x097ce7d9, /// 0x798
			 0x583d8ece, /// 0x79c
			 0x88cb6258, /// 0x7a0
			 0x052bd231, /// 0x7a4
			 0x26207b30, /// 0x7a8
			 0x6f7ad890, /// 0x7ac
			 0x22506ae1, /// 0x7b0
			 0xcad15285, /// 0x7b4
			 0x4b968dd5, /// 0x7b8
			 0xf4ec827e, /// 0x7bc
			 0x4b915715, /// 0x7c0
			 0xb9d5c1a8, /// 0x7c4
			 0x9132662f, /// 0x7c8
			 0x3490731c, /// 0x7cc
			 0x76a332cf, /// 0x7d0
			 0xff87f247, /// 0x7d4
			 0x9400cc07, /// 0x7d8
			 0xe1ef78d2, /// 0x7dc
			 0xad73716f, /// 0x7e0
			 0xe100def5, /// 0x7e4
			 0x45fa4a4f, /// 0x7e8
			 0x7be02618, /// 0x7ec
			 0xb8f089cf, /// 0x7f0
			 0x614858ac, /// 0x7f4
			 0x3ad90bfa, /// 0x7f8
			 0x6b24e6eb, /// 0x7fc
			 0x5ae129d1, /// 0x800
			 0x846359f5, /// 0x804
			 0x9facab65, /// 0x808
			 0xd173eda8, /// 0x80c
			 0x3647465b, /// 0x810
			 0xc6c50c36, /// 0x814
			 0xc169c7cf, /// 0x818
			 0x26691986, /// 0x81c
			 0x58bb8ce6, /// 0x820
			 0x652828f6, /// 0x824
			 0x19d16074, /// 0x828
			 0xd970325f, /// 0x82c
			 0x5d0c3952, /// 0x830
			 0x50183d6c, /// 0x834
			 0x4843e187, /// 0x838
			 0xcdca54ce, /// 0x83c
			 0x8b249f28, /// 0x840
			 0xa91c8e5a, /// 0x844
			 0x4eab3562, /// 0x848
			 0x19fc7123, /// 0x84c
			 0xf32edad6, /// 0x850
			 0x3b63cb63, /// 0x854
			 0xc0d2297b, /// 0x858
			 0xf52921c7, /// 0x85c
			 0xfeaf0ca9, /// 0x860
			 0xd12895c1, /// 0x864
			 0x7b1ff4f6, /// 0x868
			 0xe3295cfe, /// 0x86c
			 0x32696368, /// 0x870
			 0x414cdb33, /// 0x874
			 0x10f99972, /// 0x878
			 0x4513825d, /// 0x87c
			 0x5ba8adc2, /// 0x880
			 0x5460995b, /// 0x884
			 0x51fde793, /// 0x888
			 0xf7183221, /// 0x88c
			 0xf2808450, /// 0x890
			 0x251678ee, /// 0x894
			 0x30b94a80, /// 0x898
			 0x0a98a7b8, /// 0x89c
			 0xa9c0b8d5, /// 0x8a0
			 0x264b9945, /// 0x8a4
			 0x29417231, /// 0x8a8
			 0xfff616ee, /// 0x8ac
			 0x13de811c, /// 0x8b0
			 0xdb84ff10, /// 0x8b4
			 0x9e70b9e7, /// 0x8b8
			 0xf485212f, /// 0x8bc
			 0xf698af94, /// 0x8c0
			 0x985fa592, /// 0x8c4
			 0x15e1b7b6, /// 0x8c8
			 0x8bd7224d, /// 0x8cc
			 0x21ae12e9, /// 0x8d0
			 0x2f563caf, /// 0x8d4
			 0xd780c86d, /// 0x8d8
			 0x933aaecc, /// 0x8dc
			 0x13770c66, /// 0x8e0
			 0x6c6f171b, /// 0x8e4
			 0x2e1b8989, /// 0x8e8
			 0x63f37607, /// 0x8ec
			 0x2798be32, /// 0x8f0
			 0xdfbe0b93, /// 0x8f4
			 0xc065286c, /// 0x8f8
			 0x56615791, /// 0x8fc
			 0x2c311cab, /// 0x900
			 0xd23e49f0, /// 0x904
			 0x85ddd53a, /// 0x908
			 0x29e99ba0, /// 0x90c
			 0x7a5ac6a4, /// 0x910
			 0x237fd4c8, /// 0x914
			 0xc44ed93b, /// 0x918
			 0xf05fbea3, /// 0x91c
			 0x1dc72b8a, /// 0x920
			 0x4a39bf69, /// 0x924
			 0xd822b6c8, /// 0x928
			 0x1b113477, /// 0x92c
			 0x8ed9363d, /// 0x930
			 0x269966ca, /// 0x934
			 0x21cd268f, /// 0x938
			 0xbda120b4, /// 0x93c
			 0x4a9160a0, /// 0x940
			 0x792d9172, /// 0x944
			 0x701c75fe, /// 0x948
			 0xde50912e, /// 0x94c
			 0x40f5413b, /// 0x950
			 0x5604ca9d, /// 0x954
			 0xec070412, /// 0x958
			 0x1e424991, /// 0x95c
			 0x37077297, /// 0x960
			 0xccdbf290, /// 0x964
			 0x4eae7b58, /// 0x968
			 0x5668abb3, /// 0x96c
			 0x3aa25368, /// 0x970
			 0x444f91b6, /// 0x974
			 0xed32e361, /// 0x978
			 0x1229c351, /// 0x97c
			 0x5613a0a0, /// 0x980
			 0x78ec8aa8, /// 0x984
			 0x02277461, /// 0x988
			 0xe2de9991, /// 0x98c
			 0x70c61712, /// 0x990
			 0x87fc3434, /// 0x994
			 0x197a0da0, /// 0x998
			 0x98016ebf, /// 0x99c
			 0xf42debba, /// 0x9a0
			 0x99bc0f47, /// 0x9a4
			 0x06844092, /// 0x9a8
			 0xe2688838, /// 0x9ac
			 0xf48179bb, /// 0x9b0
			 0x09966808, /// 0x9b4
			 0xa0f60ebc, /// 0x9b8
			 0x17e0ff9d, /// 0x9bc
			 0x72c635f2, /// 0x9c0
			 0x310090a8, /// 0x9c4
			 0xf559e6fd, /// 0x9c8
			 0x00d0a159, /// 0x9cc
			 0x039e96c6, /// 0x9d0
			 0x00f48fb4, /// 0x9d4
			 0x4c144c96, /// 0x9d8
			 0x1d8adfa3, /// 0x9dc
			 0x033d3d41, /// 0x9e0
			 0x4eaba3d0, /// 0x9e4
			 0xf95cc869, /// 0x9e8
			 0x6b394918, /// 0x9ec
			 0xc3773494, /// 0x9f0
			 0x2aba435c, /// 0x9f4
			 0x0f80107e, /// 0x9f8
			 0x24ff27c8, /// 0x9fc
			 0xa8693a20, /// 0xa00
			 0x219ca5ff, /// 0xa04
			 0x68fbbe93, /// 0xa08
			 0xe419fd6b, /// 0xa0c
			 0x2c23a4b3, /// 0xa10
			 0xb2c8dc57, /// 0xa14
			 0x929d81f5, /// 0xa18
			 0x340940d9, /// 0xa1c
			 0xbb51063a, /// 0xa20
			 0xe2862989, /// 0xa24
			 0x510d6415, /// 0xa28
			 0x0e84cdbf, /// 0xa2c
			 0x2a0679f5, /// 0xa30
			 0x54098067, /// 0xa34
			 0xcbbdf62e, /// 0xa38
			 0xc885fb28, /// 0xa3c
			 0x84e0aa4c, /// 0xa40
			 0x1351d2c1, /// 0xa44
			 0xeb148a88, /// 0xa48
			 0x5c2b0ad1, /// 0xa4c
			 0x285313dd, /// 0xa50
			 0x5f432c48, /// 0xa54
			 0x8b029a55, /// 0xa58
			 0x2d46969b, /// 0xa5c
			 0x5f7523f5, /// 0xa60
			 0xbeb91982, /// 0xa64
			 0x192fbc05, /// 0xa68
			 0x05f3c341, /// 0xa6c
			 0x035ccf82, /// 0xa70
			 0x022c295c, /// 0xa74
			 0x3ba52063, /// 0xa78
			 0x52fd14cf, /// 0xa7c
			 0x94a95569, /// 0xa80
			 0x057d71da, /// 0xa84
			 0x0cddb820, /// 0xa88
			 0x8d31c307, /// 0xa8c
			 0x4e7e804f, /// 0xa90
			 0xcb76f73e, /// 0xa94
			 0xc3d2f03b, /// 0xa98
			 0x9f7456ae, /// 0xa9c
			 0x15151fae, /// 0xaa0
			 0xabde69e2, /// 0xaa4
			 0x2ca494b7, /// 0xaa8
			 0xb24fec37, /// 0xaac
			 0xbdba8542, /// 0xab0
			 0x18ef723b, /// 0xab4
			 0xca5c6745, /// 0xab8
			 0xfabca850, /// 0xabc
			 0x730c6c9e, /// 0xac0
			 0xb62e9ac8, /// 0xac4
			 0x4cd30828, /// 0xac8
			 0x384ab9f6, /// 0xacc
			 0x9401f905, /// 0xad0
			 0x50063351, /// 0xad4
			 0xd85c7603, /// 0xad8
			 0x99389e78, /// 0xadc
			 0x608df98a, /// 0xae0
			 0xc306f503, /// 0xae4
			 0x1ac70884, /// 0xae8
			 0xea177919, /// 0xaec
			 0x9a1ae013, /// 0xaf0
			 0x66180ae1, /// 0xaf4
			 0x88e419e1, /// 0xaf8
			 0x3d8b3d50, /// 0xafc
			 0x39c946ce, /// 0xb00
			 0xb7b41010, /// 0xb04
			 0x3ecb5758, /// 0xb08
			 0xfaf09a05, /// 0xb0c
			 0x1446b467, /// 0xb10
			 0x296316ae, /// 0xb14
			 0x2c3ad460, /// 0xb18
			 0x1c84cbac, /// 0xb1c
			 0x4342e3d2, /// 0xb20
			 0x51f2964b, /// 0xb24
			 0x9ff45c67, /// 0xb28
			 0x97cd95e3, /// 0xb2c
			 0xce9c2c23, /// 0xb30
			 0x779fdeaf, /// 0xb34
			 0xae6b4f84, /// 0xb38
			 0xbda4bd31, /// 0xb3c
			 0xaa4d364d, /// 0xb40
			 0xfe03d93c, /// 0xb44
			 0xd526c7a8, /// 0xb48
			 0xd9bbf555, /// 0xb4c
			 0x93933fca, /// 0xb50
			 0x514943e7, /// 0xb54
			 0x90081374, /// 0xb58
			 0x75051ae6, /// 0xb5c
			 0xa2e1df05, /// 0xb60
			 0x7a97fe1b, /// 0xb64
			 0xc93b0aaf, /// 0xb68
			 0xbc3963bd, /// 0xb6c
			 0xcb65bb83, /// 0xb70
			 0x41e41c62, /// 0xb74
			 0xae35350d, /// 0xb78
			 0xe1242be7, /// 0xb7c
			 0xd23fb85f, /// 0xb80
			 0x94e0390b, /// 0xb84
			 0xf207c8e5, /// 0xb88
			 0x27f5c723, /// 0xb8c
			 0x5bc0d72b, /// 0xb90
			 0xdd2d9516, /// 0xb94
			 0x20fbb60f, /// 0xb98
			 0x7d5f4e2c, /// 0xb9c
			 0x84ba2b9a, /// 0xba0
			 0xb9893418, /// 0xba4
			 0x05bce4d6, /// 0xba8
			 0x241975b3, /// 0xbac
			 0xbf2e780e, /// 0xbb0
			 0x19fd66ce, /// 0xbb4
			 0x3e223da5, /// 0xbb8
			 0x00f28ffd, /// 0xbbc
			 0x47089a11, /// 0xbc0
			 0x3b8d5dbd, /// 0xbc4
			 0x2f8f4ea7, /// 0xbc8
			 0xb4b1c86b, /// 0xbcc
			 0x797c1126, /// 0xbd0
			 0xef29b812, /// 0xbd4
			 0x797768ca, /// 0xbd8
			 0xb3885dde, /// 0xbdc
			 0x5ad87c05, /// 0xbe0
			 0x1312bd1b, /// 0xbe4
			 0x0f4283aa, /// 0xbe8
			 0x837f5311, /// 0xbec
			 0xcf914ac8, /// 0xbf0
			 0xda796b2b, /// 0xbf4
			 0x1fd9e1f1, /// 0xbf8
			 0x49e02e84, /// 0xbfc
			 0xd6a9a542, /// 0xc00
			 0xd8dbddea, /// 0xc04
			 0x5d2307ca, /// 0xc08
			 0x925bfc1e, /// 0xc0c
			 0x046315bf, /// 0xc10
			 0xce539253, /// 0xc14
			 0x278f73f5, /// 0xc18
			 0x52a1b8f7, /// 0xc1c
			 0xddaf7f4b, /// 0xc20
			 0x92e81ae0, /// 0xc24
			 0xa25be96a, /// 0xc28
			 0x5f5bc66c, /// 0xc2c
			 0x4ac76fce, /// 0xc30
			 0x7d820733, /// 0xc34
			 0xa5451886, /// 0xc38
			 0x69b3bc3c, /// 0xc3c
			 0xfbdc9949, /// 0xc40
			 0xd20e0b49, /// 0xc44
			 0x4cb499b0, /// 0xc48
			 0xa25368f0, /// 0xc4c
			 0xfddf7fe5, /// 0xc50
			 0xf726db04, /// 0xc54
			 0x3ccc7087, /// 0xc58
			 0x5144ce33, /// 0xc5c
			 0x84e03a4e, /// 0xc60
			 0x840202f2, /// 0xc64
			 0x16de5175, /// 0xc68
			 0xd0d0388e, /// 0xc6c
			 0x3acf830c, /// 0xc70
			 0xeeeff26c, /// 0xc74
			 0xf29d923d, /// 0xc78
			 0x1b35ca57, /// 0xc7c
			 0x6fe1a1aa, /// 0xc80
			 0xc722dc83, /// 0xc84
			 0x020f04f5, /// 0xc88
			 0x49023dd1, /// 0xc8c
			 0xe18349ee, /// 0xc90
			 0x17f5f844, /// 0xc94
			 0x60512c22, /// 0xc98
			 0xc006541f, /// 0xc9c
			 0x7e09ce31, /// 0xca0
			 0xd05ee5ab, /// 0xca4
			 0x3798715f, /// 0xca8
			 0x0074a287, /// 0xcac
			 0xcf94900b, /// 0xcb0
			 0xd8a4ec72, /// 0xcb4
			 0xe6fa03ab, /// 0xcb8
			 0x1ccacba1, /// 0xcbc
			 0x48a77574, /// 0xcc0
			 0xfc8dd6aa, /// 0xcc4
			 0x7351e675, /// 0xcc8
			 0xf10c5259, /// 0xccc
			 0xb4f448e9, /// 0xcd0
			 0x5d991b08, /// 0xcd4
			 0xed1c2b47, /// 0xcd8
			 0x53d87a14, /// 0xcdc
			 0x71cc56bd, /// 0xce0
			 0x90d63bdd, /// 0xce4
			 0x388de628, /// 0xce8
			 0x978e2ef8, /// 0xcec
			 0x7d7acc6a, /// 0xcf0
			 0x94243cfd, /// 0xcf4
			 0x7bbf127f, /// 0xcf8
			 0xaff286b9, /// 0xcfc
			 0x4a4eb529, /// 0xd00
			 0x4af255ff, /// 0xd04
			 0xd7b28b16, /// 0xd08
			 0x8d51f479, /// 0xd0c
			 0x9df25643, /// 0xd10
			 0x2e20f8ba, /// 0xd14
			 0x655ec255, /// 0xd18
			 0x1ef60154, /// 0xd1c
			 0x9edc20f4, /// 0xd20
			 0x7a4e4c7a, /// 0xd24
			 0x35b64823, /// 0xd28
			 0x6c10a4e3, /// 0xd2c
			 0xa0e29fee, /// 0xd30
			 0xc37d5d07, /// 0xd34
			 0x888b6a24, /// 0xd38
			 0x3183baae, /// 0xd3c
			 0x39c3a75b, /// 0xd40
			 0x89089b25, /// 0xd44
			 0x622e2b39, /// 0xd48
			 0xd6180073, /// 0xd4c
			 0x253861e6, /// 0xd50
			 0x81919998, /// 0xd54
			 0x665c4f2a, /// 0xd58
			 0x2bf9c302, /// 0xd5c
			 0x1f3a4f91, /// 0xd60
			 0x571eee4e, /// 0xd64
			 0xe7926ef7, /// 0xd68
			 0xe103a9c3, /// 0xd6c
			 0xf8799667, /// 0xd70
			 0x8ccfcba1, /// 0xd74
			 0xd44f70e6, /// 0xd78
			 0x95688eca, /// 0xd7c
			 0x9e2ebebc, /// 0xd80
			 0x7a3705a9, /// 0xd84
			 0xfe44a2ba, /// 0xd88
			 0xd875346d, /// 0xd8c
			 0xfdfa131e, /// 0xd90
			 0xe71f096a, /// 0xd94
			 0xf741ff85, /// 0xd98
			 0x3b00348f, /// 0xd9c
			 0x7f663010, /// 0xda0
			 0x73317fa2, /// 0xda4
			 0xd8f87111, /// 0xda8
			 0x5192857c, /// 0xdac
			 0xa1337f5b, /// 0xdb0
			 0x5221b85c, /// 0xdb4
			 0x0eb01551, /// 0xdb8
			 0x21a7cd94, /// 0xdbc
			 0xa1ce0b23, /// 0xdc0
			 0x81f9735e, /// 0xdc4
			 0x32e3ea38, /// 0xdc8
			 0xb30e4ec7, /// 0xdcc
			 0x5821c9d8, /// 0xdd0
			 0xcf50ea48, /// 0xdd4
			 0xd7f4ea12, /// 0xdd8
			 0xb8573d34, /// 0xddc
			 0xb519c865, /// 0xde0
			 0xd695b1ab, /// 0xde4
			 0x950fec01, /// 0xde8
			 0x1c7332b5, /// 0xdec
			 0x460e8907, /// 0xdf0
			 0x52e81052, /// 0xdf4
			 0x00ae8a90, /// 0xdf8
			 0x6c83b394, /// 0xdfc
			 0xf59a165d, /// 0xe00
			 0xa2f0ec2b, /// 0xe04
			 0x638fcf4d, /// 0xe08
			 0x5be8b806, /// 0xe0c
			 0x4abe2abf, /// 0xe10
			 0x180678b9, /// 0xe14
			 0x15778534, /// 0xe18
			 0x47f11a53, /// 0xe1c
			 0x3bfc2105, /// 0xe20
			 0x295a3c85, /// 0xe24
			 0xa5b0a11e, /// 0xe28
			 0xe0196ea3, /// 0xe2c
			 0x31fe129f, /// 0xe30
			 0xffa3deed, /// 0xe34
			 0x6887bd8c, /// 0xe38
			 0xadf4336a, /// 0xe3c
			 0x847a1136, /// 0xe40
			 0xf8aeff7b, /// 0xe44
			 0xcf05cc98, /// 0xe48
			 0xfe9f0f7e, /// 0xe4c
			 0x1d9d2e58, /// 0xe50
			 0xbef8cdff, /// 0xe54
			 0x2ffbf2d5, /// 0xe58
			 0x49d6c681, /// 0xe5c
			 0x8b7001c5, /// 0xe60
			 0xf1d484ce, /// 0xe64
			 0xbb3b63f3, /// 0xe68
			 0x9e7a51cb, /// 0xe6c
			 0x59750d0d, /// 0xe70
			 0x584fffea, /// 0xe74
			 0xe8d03655, /// 0xe78
			 0x76c5664a, /// 0xe7c
			 0x26ec1e10, /// 0xe80
			 0x32d1dd96, /// 0xe84
			 0x68849935, /// 0xe88
			 0xb0318657, /// 0xe8c
			 0x12b83c08, /// 0xe90
			 0xb44ed426, /// 0xe94
			 0x34923527, /// 0xe98
			 0x63554a19, /// 0xe9c
			 0x270c59e8, /// 0xea0
			 0x3b45fbd6, /// 0xea4
			 0xb093e118, /// 0xea8
			 0x69171426, /// 0xeac
			 0xa8ecf33b, /// 0xeb0
			 0x03ea90d9, /// 0xeb4
			 0xc92bbca6, /// 0xeb8
			 0xb22fe907, /// 0xebc
			 0xb4eb24ba, /// 0xec0
			 0x67a3bacd, /// 0xec4
			 0x06af26f8, /// 0xec8
			 0x1c9c343a, /// 0xecc
			 0x17017698, /// 0xed0
			 0xaabf6596, /// 0xed4
			 0x00111b34, /// 0xed8
			 0x43ec29ce, /// 0xedc
			 0xee21f76f, /// 0xee0
			 0xf9ea3e77, /// 0xee4
			 0xa8357df2, /// 0xee8
			 0x6b40611e, /// 0xeec
			 0xed296e5a, /// 0xef0
			 0x89600e6c, /// 0xef4
			 0xdb88f427, /// 0xef8
			 0x1bfc1cd4, /// 0xefc
			 0x4b57a181, /// 0xf00
			 0x7d357c39, /// 0xf04
			 0x29a575b1, /// 0xf08
			 0xbb4aceb2, /// 0xf0c
			 0x0811852b, /// 0xf10
			 0xa4e1c86c, /// 0xf14
			 0xdb218dea, /// 0xf18
			 0x8d9d4d28, /// 0xf1c
			 0xf85dbead, /// 0xf20
			 0x37d8b1f5, /// 0xf24
			 0x4c89597e, /// 0xf28
			 0xf5536b84, /// 0xf2c
			 0x28f32352, /// 0xf30
			 0x7988bcfa, /// 0xf34
			 0x9237367a, /// 0xf38
			 0x7dcaecb3, /// 0xf3c
			 0xc076a055, /// 0xf40
			 0x25353ea2, /// 0xf44
			 0x895be0e7, /// 0xf48
			 0xb475061f, /// 0xf4c
			 0x11cc382f, /// 0xf50
			 0xf8a31d08, /// 0xf54
			 0xf0731084, /// 0xf58
			 0xa5938bb8, /// 0xf5c
			 0xdae86f06, /// 0xf60
			 0x0b10f3ef, /// 0xf64
			 0x1169b03a, /// 0xf68
			 0xfc6f2b46, /// 0xf6c
			 0x0ce9e174, /// 0xf70
			 0x4cb79921, /// 0xf74
			 0xd33a3af8, /// 0xf78
			 0x32a838b2, /// 0xf7c
			 0xdaed4a3e, /// 0xf80
			 0x68bef184, /// 0xf84
			 0xef52425e, /// 0xf88
			 0xb6ad15a7, /// 0xf8c
			 0xd281a876, /// 0xf90
			 0x89b8754f, /// 0xf94
			 0x2e6bea35, /// 0xf98
			 0x84f98cd7, /// 0xf9c
			 0x38732dcd, /// 0xfa0
			 0xab3f8ac8, /// 0xfa4
			 0xe23fcd73, /// 0xfa8
			 0xd6624d90, /// 0xfac
			 0xcd5db760, /// 0xfb0
			 0x75aa9112, /// 0xfb4
			 0xf4345989, /// 0xfb8
			 0x1e9cca02, /// 0xfbc
			 0xb8aef58a, /// 0xfc0
			 0xf3dc32d7, /// 0xfc4
			 0x172560aa, /// 0xfc8
			 0x0a37a62e, /// 0xfcc
			 0xb81a9c01, /// 0xfd0
			 0x42eb1145, /// 0xfd4
			 0xc4a4a1af, /// 0xfd8
			 0x1588af06, /// 0xfdc
			 0x0347bb27, /// 0xfe0
			 0x3b9988a4, /// 0xfe4
			 0x8154fc24, /// 0xfe8
			 0xa146024c, /// 0xfec
			 0x1165b3a5, /// 0xff0
			 0x8f00a457, /// 0xff4
			 0x256533cd, /// 0xff8
			 0x82aca77a /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00000
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x33333333,                                                  // 4 random values                             /// 00008
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00018
			 0x0c600000,                                                  // Leading 1s:                                 /// 0001c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0002c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00030
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00034
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00038
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00040
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00044
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00048
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0004c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00054
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00058
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0005c
			 0x7f800000,                                                  // inf                                         /// 00060
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00064
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00068
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0006c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0007c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00080
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00084
			 0xcb000000,                                                  // -8388608.0                                  /// 00088
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0008c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00098
			 0xcb000000,                                                  // -8388608.0                                  /// 0009c
			 0x0e000001,                                                  // Trailing 1s:                                /// 000a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 000f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00104
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00108
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0010c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00110
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00114
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00118
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00120
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00128
			 0x00011111,                                                  // 9.7958E-41                                  /// 0012c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00130
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x4b000000,                                                  // 8388608.0                                   /// 00138
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00148
			 0x0c780000,                                                  // Leading 1s:                                 /// 0014c
			 0x55555555,                                                  // 4 random values                             /// 00150
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0015c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00164
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00168
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00170
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0017c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00180
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00184
			 0xff800000,                                                  // -inf                                        /// 00188
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0018c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00190
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00194
			 0x7fc00001,                                                  // signaling NaN                               /// 00198
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x0e000001,                                                  // Trailing 1s:                                /// 001a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00000000,                                                  // zero                                        /// 001b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001fc
			 0x00000000,                                                  // zero                                        /// 00200
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00204
			 0x00000000,                                                  // zero                                        /// 00208
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0020c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00210
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00214
			 0xff800000,                                                  // -inf                                        /// 00218
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0021c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00228
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0022c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00230
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0023c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00240
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0024c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x55555555,                                                  // 4 random values                             /// 00254
			 0x0c700000,                                                  // Leading 1s:                                 /// 00258
			 0x80011111,                                                  // -9.7958E-41                                 /// 0025c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00264
			 0x55555555,                                                  // 4 random values                             /// 00268
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00270
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00274
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00284
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00288
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0028c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00290
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00294
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00298
			 0x0c700000,                                                  // Leading 1s:                                 /// 0029c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002a4
			 0x80000000,                                                  // -zero                                       /// 002a8
			 0x00000000,                                                  // zero                                        /// 002ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00300
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00308
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00310
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0031c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00324
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00334
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00338
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00340
			 0x0c600000,                                                  // Leading 1s:                                 /// 00344
			 0x7f800000,                                                  // inf                                         /// 00348
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0034c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0035c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00360
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00364
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0036c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00370
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00374
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0e000007,                                                  // Trailing 1s:                                /// 0037c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0xffc00001,                                                  // -signaling NaN                              /// 00384
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0038c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00394
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00398
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0039c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 003ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00400
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00404
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00408
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0040c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00410
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00414
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0041c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0042c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00434
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00440
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00444
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0044c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00450
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00454
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0045c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00460
			 0x7fc00001,                                                  // signaling NaN                               /// 00464
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00468
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00470
			 0x00000000,                                                  // zero                                        /// 00474
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00478
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00480
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00484
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x3f028f5c,                                                  // 0.51                                        /// 0048c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00490
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00498
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0049c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0xff800000,                                                  // -inf                                        /// 004b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x7fc00001,                                                  // signaling NaN                               /// 004bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00518
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0051c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0052c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00530
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00534
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00538
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00540
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0xff800000,                                                  // -inf                                        /// 00548
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00554
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00558
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00560
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x00000000,                                                  // zero                                        /// 00568
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0056c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00570
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00574
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00580
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00584
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00588
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0058c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00590
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00594
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00598
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0059c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 005cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005f4
			 0x7f800000,                                                  // inf                                         /// 005f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00600
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00604
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0060c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00610
			 0xcb000000,                                                  // -8388608.0                                  /// 00614
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x00011111,                                                  // 9.7958E-41                                  /// 0061c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00620
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00624
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00628
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0062c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0063c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00644
			 0x80011111,                                                  // -9.7958E-41                                 /// 00648
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0064c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00650
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00654
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0065c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00660
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00664
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00668
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0066c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00670
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00674
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0067c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00680
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00684
			 0x0c780000,                                                  // Leading 1s:                                 /// 00688
			 0x0c780000,                                                  // Leading 1s:                                 /// 0068c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00690
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0c780000,                                                  // Leading 1s:                                 /// 00698
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0c700000,                                                  // Leading 1s:                                 /// 006a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a8
			 0xcb000000,                                                  // -8388608.0                                  /// 006ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006dc
			 0xffc00001,                                                  // -signaling NaN                              /// 006e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00700
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00704
			 0x80011111,                                                  // -9.7958E-41                                 /// 00708
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00718
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0071c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00720
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00728
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0072c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0c400000,                                                  // Leading 1s:                                 /// 00734
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00738
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00744
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0074c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00750
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00754
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00758
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0075c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00760
			 0x0c780000,                                                  // Leading 1s:                                 /// 00764
			 0x80011111,                                                  // -9.7958E-41                                 /// 00768
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0076c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00770
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x7fc00001,                                                  // signaling NaN                               /// 00778
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00784
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007c4
			 0x7fc00001,                                                  // signaling NaN                               /// 007c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007e4
			 0x55555555,                                                  // 4 random values                             /// 007e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 007fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00800
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00808
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0080c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00810
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00818
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0081c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00828
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0082c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00830
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00834
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00838
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0083c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00840
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00844
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00848
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0084c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00854
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00858
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0085c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00860
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00864
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00868
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00870
			 0xbf028f5c,                                                  // -0.51                                       /// 00874
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00878
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0087c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00880
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00884
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00888
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0088c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00890
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00894
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00898
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0089c
			 0xcb000000,                                                  // -8388608.0                                  /// 008a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x33333333,                                                  // 4 random values                             /// 008bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008cc
			 0x7fc00001,                                                  // signaling NaN                               /// 008d0
			 0x4b000000,                                                  // 8388608.0                                   /// 008d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00900
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00904
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0090c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00914
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00918
			 0x0c600000,                                                  // Leading 1s:                                 /// 0091c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00920
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00924
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00930
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00934
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00938
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00940
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00948
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0094c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00950
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00954
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00958
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0095c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00960
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00964
			 0xffc00001,                                                  // -signaling NaN                              /// 00968
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0096c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0097c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00980
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00984
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00988
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0099c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x80000000,                                                  // -zero                                       /// 009cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 009d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009dc
			 0x80000000,                                                  // -zero                                       /// 009e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 009f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00000000,                                                  // zero                                        /// 00a10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x3f028f5c,                                                  // 0.51                                        /// 00a18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a20
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a28
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a40
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0xffc00001,                                                  // -signaling NaN                              /// 00a4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a54
			 0x7fc00001,                                                  // signaling NaN                               /// 00a58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aa0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ab0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ab4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ab8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00abc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ac4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ac8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00acc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ad4
			 0x7f800000,                                                  // inf                                         /// 00ad8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ae0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ae8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00af0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00af4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00af8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00afc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80000000,                                                  // -zero                                       /// 00b04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0xff800000,                                                  // -inf                                        /// 00b38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b54
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b70
			 0x4b000000,                                                  // 8388608.0                                   /// 00b74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b94
			 0xff800000,                                                  // -inf                                        /// 00b98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ba0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bb8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bc0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bd4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bdc
			 0x7fc00001,                                                  // signaling NaN                               /// 00be0
			 0xff800000,                                                  // -inf                                        /// 00be4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bf0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bf8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c14
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c38
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x4b000000,                                                  // 8388608.0                                   /// 00c44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c50
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c68
			 0x80000000,                                                  // -zero                                       /// 00c6c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0xcb000000,                                                  // -8388608.0                                  /// 00c84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c88
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c90
			 0x55555555,                                                  // 4 random values                             /// 00c94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c98
			 0x3f028f5c,                                                  // 0.51                                        /// 00c9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ca4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cb4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cb8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cc4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ccc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cd4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cd8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cdc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ce0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ce4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x3f028f5c,                                                  // 0.51                                        /// 00d00
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d10
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d2c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d48
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d58
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d7c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d88
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d98
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00da4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00da8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00db0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00db4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dc0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dcc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dd0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00de0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00de4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00de8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00df0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00df4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00df8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e50
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x00000000,                                                  // zero                                        /// 00e5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e60
			 0xcb000000,                                                  // -8388608.0                                  /// 00e64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e80
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e84
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e98
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ea0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ea4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ea8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00eb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0xffc00001,                                                  // -signaling NaN                              /// 00eb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ec0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ed4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00edc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ee0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ee4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ee8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ef0
			 0x80000000,                                                  // -zero                                       /// 00ef4
			 0x55555555,                                                  // 4 random values                             /// 00ef8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f5c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0xbf028f5c,                                                  // -0.51                                       /// 00f7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x55555555,                                                  // 4 random values                             /// 00fa4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fa8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fc4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fcc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fdc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fe0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fe4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fe8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ff0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0x0d00fff0                                                  // Set of 1s                                   /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x1540a371, /// 0x0
			 0xd5e25ac8, /// 0x4
			 0xddd7c890, /// 0x8
			 0x0df23e92, /// 0xc
			 0x27268136, /// 0x10
			 0x9f2fe797, /// 0x14
			 0xdddcae4f, /// 0x18
			 0x810416da, /// 0x1c
			 0xaab5e371, /// 0x20
			 0x9f77142e, /// 0x24
			 0xea29ace3, /// 0x28
			 0x4d6cfa93, /// 0x2c
			 0x51571d28, /// 0x30
			 0x77a5af39, /// 0x34
			 0x8f3932ac, /// 0x38
			 0x7a432452, /// 0x3c
			 0x769367c1, /// 0x40
			 0x8cd079d4, /// 0x44
			 0xad3f6991, /// 0x48
			 0xa2c1790e, /// 0x4c
			 0x6a4880a1, /// 0x50
			 0xa5d08a25, /// 0x54
			 0xb270c5be, /// 0x58
			 0x65476483, /// 0x5c
			 0x1b4c6235, /// 0x60
			 0x01200b99, /// 0x64
			 0x68ddc9da, /// 0x68
			 0x17848b76, /// 0x6c
			 0x52c6095b, /// 0x70
			 0xa0b0c0ba, /// 0x74
			 0xff148288, /// 0x78
			 0x113f1745, /// 0x7c
			 0x69efbbb6, /// 0x80
			 0x98d37a1a, /// 0x84
			 0x3bc17eb8, /// 0x88
			 0x25e0d036, /// 0x8c
			 0xf4050330, /// 0x90
			 0xaa514209, /// 0x94
			 0x56eaff76, /// 0x98
			 0x21920fe4, /// 0x9c
			 0x9a82e2f0, /// 0xa0
			 0x40f11dc4, /// 0xa4
			 0x4aced6c1, /// 0xa8
			 0xaf31f5f3, /// 0xac
			 0x9dfeca77, /// 0xb0
			 0xfb1b1086, /// 0xb4
			 0x9ec4adb5, /// 0xb8
			 0xeffbc591, /// 0xbc
			 0xb10449ac, /// 0xc0
			 0xea5008e2, /// 0xc4
			 0x43f7665a, /// 0xc8
			 0xaa4e2072, /// 0xcc
			 0x7f42e491, /// 0xd0
			 0xebb741bc, /// 0xd4
			 0xe4551755, /// 0xd8
			 0x3876ddd3, /// 0xdc
			 0x43848b7b, /// 0xe0
			 0x146d9198, /// 0xe4
			 0x30577f33, /// 0xe8
			 0x5217a8f5, /// 0xec
			 0x2c8a3431, /// 0xf0
			 0x27affb28, /// 0xf4
			 0xa5db9c77, /// 0xf8
			 0x8f5077cd, /// 0xfc
			 0x144bd9c6, /// 0x100
			 0xde0355f5, /// 0x104
			 0x2408f7e8, /// 0x108
			 0xdff9d62e, /// 0x10c
			 0xab0d9044, /// 0x110
			 0x9351142d, /// 0x114
			 0xb4efca37, /// 0x118
			 0xb41227c5, /// 0x11c
			 0xf18e969b, /// 0x120
			 0xc5b2a45a, /// 0x124
			 0xa62c3055, /// 0x128
			 0xdc7240bd, /// 0x12c
			 0xee301d40, /// 0x130
			 0x9593f5e9, /// 0x134
			 0xf0ac2c8f, /// 0x138
			 0x75511e5b, /// 0x13c
			 0x68e3c483, /// 0x140
			 0xed68f684, /// 0x144
			 0x1040b07b, /// 0x148
			 0xeee7098c, /// 0x14c
			 0x4a31577b, /// 0x150
			 0x1d4f5d9b, /// 0x154
			 0x1558575a, /// 0x158
			 0x569e5ab0, /// 0x15c
			 0x6fc41192, /// 0x160
			 0xd582b641, /// 0x164
			 0x1fa433c4, /// 0x168
			 0x49b8f971, /// 0x16c
			 0x2d99b44d, /// 0x170
			 0x2c446206, /// 0x174
			 0x1492ce0f, /// 0x178
			 0xc2bf4d11, /// 0x17c
			 0xc35482e4, /// 0x180
			 0x564388a1, /// 0x184
			 0x93ca7b4e, /// 0x188
			 0xe7e8eb79, /// 0x18c
			 0x8dcf2a65, /// 0x190
			 0x10d747ca, /// 0x194
			 0x6a241eb2, /// 0x198
			 0xbabf5a81, /// 0x19c
			 0x98085f1d, /// 0x1a0
			 0x37bb190a, /// 0x1a4
			 0x42043e48, /// 0x1a8
			 0x954dca37, /// 0x1ac
			 0xcf2f57e8, /// 0x1b0
			 0x0c0a428d, /// 0x1b4
			 0x25ecbea9, /// 0x1b8
			 0xe84114ef, /// 0x1bc
			 0x191aba20, /// 0x1c0
			 0xbecc4e04, /// 0x1c4
			 0x79beb496, /// 0x1c8
			 0xd9ab968c, /// 0x1cc
			 0x822a89fa, /// 0x1d0
			 0xf9f10ba8, /// 0x1d4
			 0x007e8637, /// 0x1d8
			 0xe19f22ee, /// 0x1dc
			 0xf0c00f7b, /// 0x1e0
			 0x9af13d93, /// 0x1e4
			 0xa8e5f695, /// 0x1e8
			 0xeb0fa3c6, /// 0x1ec
			 0xfc63ac54, /// 0x1f0
			 0x96827d1f, /// 0x1f4
			 0x1da8f7bc, /// 0x1f8
			 0xd7562a94, /// 0x1fc
			 0x5cbc6547, /// 0x200
			 0x7cfb86df, /// 0x204
			 0x18d5d333, /// 0x208
			 0xbd60b614, /// 0x20c
			 0x3a1127d8, /// 0x210
			 0x4bb30aa4, /// 0x214
			 0xd51a3308, /// 0x218
			 0xe73cf511, /// 0x21c
			 0x4a409974, /// 0x220
			 0x6db22244, /// 0x224
			 0xec7cf23e, /// 0x228
			 0xddf582b8, /// 0x22c
			 0x50fcc945, /// 0x230
			 0xe7360583, /// 0x234
			 0xb3349b65, /// 0x238
			 0xaa81b6f5, /// 0x23c
			 0xac43336b, /// 0x240
			 0x2cd42121, /// 0x244
			 0x8157e07a, /// 0x248
			 0x6cd20857, /// 0x24c
			 0x241bbe32, /// 0x250
			 0xeb26ac4f, /// 0x254
			 0xf41c3b87, /// 0x258
			 0x39d4d0cf, /// 0x25c
			 0x6921eb04, /// 0x260
			 0xcae7691a, /// 0x264
			 0x6aa766fa, /// 0x268
			 0xfd345bc9, /// 0x26c
			 0xfcfb6266, /// 0x270
			 0xe393f0cd, /// 0x274
			 0xecadc876, /// 0x278
			 0xca0007ad, /// 0x27c
			 0x4978c78d, /// 0x280
			 0x7d3264fc, /// 0x284
			 0xa5486b9f, /// 0x288
			 0x1d1196ec, /// 0x28c
			 0xff55aad6, /// 0x290
			 0xcdeb043b, /// 0x294
			 0xf8b128d7, /// 0x298
			 0xba7a0bd7, /// 0x29c
			 0xf3169cfe, /// 0x2a0
			 0x29429938, /// 0x2a4
			 0xdd490d1b, /// 0x2a8
			 0xe8758902, /// 0x2ac
			 0x0807a737, /// 0x2b0
			 0x83b0f5bf, /// 0x2b4
			 0x5bf8df3d, /// 0x2b8
			 0xe64eda9f, /// 0x2bc
			 0xfb02930e, /// 0x2c0
			 0x358d373f, /// 0x2c4
			 0x51012b63, /// 0x2c8
			 0x4d89b851, /// 0x2cc
			 0x0f8654a3, /// 0x2d0
			 0x3e47487a, /// 0x2d4
			 0xd93e0ca7, /// 0x2d8
			 0x35d78e12, /// 0x2dc
			 0x28f9f2db, /// 0x2e0
			 0xbb7fc12b, /// 0x2e4
			 0x7c9ddc0f, /// 0x2e8
			 0x185fc69a, /// 0x2ec
			 0x4ec5fcd9, /// 0x2f0
			 0x4feae7c7, /// 0x2f4
			 0xd44d9047, /// 0x2f8
			 0x76bc7549, /// 0x2fc
			 0x18ee153d, /// 0x300
			 0x4445c613, /// 0x304
			 0xa35cee10, /// 0x308
			 0x15e81e37, /// 0x30c
			 0x150326b6, /// 0x310
			 0xa717ebae, /// 0x314
			 0xb4e0c340, /// 0x318
			 0x2fc7953a, /// 0x31c
			 0xf59fa28f, /// 0x320
			 0xcaef185c, /// 0x324
			 0x3d366b76, /// 0x328
			 0x92fd1557, /// 0x32c
			 0x2d89ae7b, /// 0x330
			 0x7c38e8d2, /// 0x334
			 0x82faf114, /// 0x338
			 0x8839ec5a, /// 0x33c
			 0xc0c0837a, /// 0x340
			 0x1099d75d, /// 0x344
			 0x5bc1e144, /// 0x348
			 0xe7e6a772, /// 0x34c
			 0xf07c17d4, /// 0x350
			 0x1a10c057, /// 0x354
			 0x66b07d74, /// 0x358
			 0xd901533b, /// 0x35c
			 0xd115105c, /// 0x360
			 0x19d77eb3, /// 0x364
			 0xf27216b2, /// 0x368
			 0x9c621235, /// 0x36c
			 0x8da2dcf8, /// 0x370
			 0x2e154761, /// 0x374
			 0xdf91b453, /// 0x378
			 0xce8338dd, /// 0x37c
			 0x6f68ebdf, /// 0x380
			 0xa2a394ed, /// 0x384
			 0x4eaf1123, /// 0x388
			 0x187d40fd, /// 0x38c
			 0xa7f6a8af, /// 0x390
			 0xda6fd0b8, /// 0x394
			 0x1874b4c2, /// 0x398
			 0x05c5a0dd, /// 0x39c
			 0x45002977, /// 0x3a0
			 0x0efbe913, /// 0x3a4
			 0x2bad35c1, /// 0x3a8
			 0x879f17e4, /// 0x3ac
			 0x84d5b4d1, /// 0x3b0
			 0x513ce4e5, /// 0x3b4
			 0x4d7a9886, /// 0x3b8
			 0x7f35f23b, /// 0x3bc
			 0xbbee95bc, /// 0x3c0
			 0x7676297f, /// 0x3c4
			 0xda0d15e3, /// 0x3c8
			 0xb61fe7ca, /// 0x3cc
			 0x34dca4bd, /// 0x3d0
			 0xce749f27, /// 0x3d4
			 0xf7f1d53c, /// 0x3d8
			 0xc7841bb8, /// 0x3dc
			 0x46208cb5, /// 0x3e0
			 0x3582684e, /// 0x3e4
			 0xf1ddf243, /// 0x3e8
			 0xbb2a1368, /// 0x3ec
			 0xab511d03, /// 0x3f0
			 0xa21268a7, /// 0x3f4
			 0xd7f304ac, /// 0x3f8
			 0xf3012c47, /// 0x3fc
			 0x318dc501, /// 0x400
			 0xb541a192, /// 0x404
			 0xbbf94cef, /// 0x408
			 0x2ec2afc4, /// 0x40c
			 0xefe4151e, /// 0x410
			 0x7d2f0181, /// 0x414
			 0x1b66c79b, /// 0x418
			 0x355573b1, /// 0x41c
			 0x9c866715, /// 0x420
			 0xfd1b863b, /// 0x424
			 0xe73910c9, /// 0x428
			 0x14c6ff64, /// 0x42c
			 0x2c969a17, /// 0x430
			 0x62bccdbf, /// 0x434
			 0x4d694917, /// 0x438
			 0xbf129399, /// 0x43c
			 0x36e1503a, /// 0x440
			 0xad45c3fb, /// 0x444
			 0x229154d0, /// 0x448
			 0xa9128d37, /// 0x44c
			 0x5843ed71, /// 0x450
			 0x2998edad, /// 0x454
			 0x7640d16e, /// 0x458
			 0xe9538efd, /// 0x45c
			 0xf76fe4d2, /// 0x460
			 0x67dfa0a2, /// 0x464
			 0x84ceae85, /// 0x468
			 0x88369a3b, /// 0x46c
			 0xf72293a5, /// 0x470
			 0x2cc159af, /// 0x474
			 0xae73bdea, /// 0x478
			 0xcb909409, /// 0x47c
			 0x2257d14d, /// 0x480
			 0xb9ca79b3, /// 0x484
			 0x9da39ff6, /// 0x488
			 0xf8a770bb, /// 0x48c
			 0x2e9c6966, /// 0x490
			 0x8b2c55ec, /// 0x494
			 0x5ebeb528, /// 0x498
			 0xab1d3f66, /// 0x49c
			 0x2eb1815a, /// 0x4a0
			 0x13283d24, /// 0x4a4
			 0xcb185c10, /// 0x4a8
			 0x304a8262, /// 0x4ac
			 0x10285ebb, /// 0x4b0
			 0xd3afa1e2, /// 0x4b4
			 0x2f9708fb, /// 0x4b8
			 0x17c8c3a0, /// 0x4bc
			 0x758e4574, /// 0x4c0
			 0x87b80e1b, /// 0x4c4
			 0x766637df, /// 0x4c8
			 0xb003e84c, /// 0x4cc
			 0x4e444640, /// 0x4d0
			 0x12e0dae1, /// 0x4d4
			 0x0c7ca451, /// 0x4d8
			 0x349dc037, /// 0x4dc
			 0x90f819ef, /// 0x4e0
			 0x745e8931, /// 0x4e4
			 0x90004c0a, /// 0x4e8
			 0xace86fe3, /// 0x4ec
			 0xe28bc808, /// 0x4f0
			 0xeb84e9c7, /// 0x4f4
			 0x1362c50c, /// 0x4f8
			 0x2adb5714, /// 0x4fc
			 0x5f2c567d, /// 0x500
			 0x4f10b5cb, /// 0x504
			 0xf3833563, /// 0x508
			 0x472d8b52, /// 0x50c
			 0xd61c3f31, /// 0x510
			 0xd35629c8, /// 0x514
			 0x2daca704, /// 0x518
			 0xa8bc4cf7, /// 0x51c
			 0x1076a720, /// 0x520
			 0x7003ca3e, /// 0x524
			 0x176c989a, /// 0x528
			 0x2f27bb91, /// 0x52c
			 0x2b48efb2, /// 0x530
			 0x138af655, /// 0x534
			 0x4eca8655, /// 0x538
			 0x524c3c1e, /// 0x53c
			 0xf096484d, /// 0x540
			 0x1951c1f6, /// 0x544
			 0x452c782e, /// 0x548
			 0xa3eead0c, /// 0x54c
			 0x2a96d3dc, /// 0x550
			 0x9f2bd41c, /// 0x554
			 0x9a4e3abe, /// 0x558
			 0x17d33071, /// 0x55c
			 0x485512fe, /// 0x560
			 0xd4ab853d, /// 0x564
			 0x1473aabd, /// 0x568
			 0x0234ca34, /// 0x56c
			 0x18ae722b, /// 0x570
			 0xf7ba7775, /// 0x574
			 0xe1768dd1, /// 0x578
			 0x85a3335e, /// 0x57c
			 0x1be357af, /// 0x580
			 0x10554559, /// 0x584
			 0xb54a6af2, /// 0x588
			 0x005170a3, /// 0x58c
			 0x918db8b1, /// 0x590
			 0x819334fc, /// 0x594
			 0x0fb4c93f, /// 0x598
			 0x832f4c31, /// 0x59c
			 0x270d19b6, /// 0x5a0
			 0x1fcc4737, /// 0x5a4
			 0x1bdc6912, /// 0x5a8
			 0xe2ace3f0, /// 0x5ac
			 0xbc156334, /// 0x5b0
			 0x0a91353d, /// 0x5b4
			 0x89e5980a, /// 0x5b8
			 0xc4ea6aff, /// 0x5bc
			 0x1761ea80, /// 0x5c0
			 0x65d23764, /// 0x5c4
			 0x84216de4, /// 0x5c8
			 0x0eacac3c, /// 0x5cc
			 0x3556ed5f, /// 0x5d0
			 0xd389a46c, /// 0x5d4
			 0x283bf46e, /// 0x5d8
			 0xac2a1603, /// 0x5dc
			 0xc025dad5, /// 0x5e0
			 0xb8c65181, /// 0x5e4
			 0x515b920b, /// 0x5e8
			 0x1222f19b, /// 0x5ec
			 0x9e21495c, /// 0x5f0
			 0x71d99e43, /// 0x5f4
			 0x0410e410, /// 0x5f8
			 0x9668df31, /// 0x5fc
			 0xac182724, /// 0x600
			 0x3888581f, /// 0x604
			 0x2f0fa3ee, /// 0x608
			 0x31a4884f, /// 0x60c
			 0x155746f1, /// 0x610
			 0x71b3cad4, /// 0x614
			 0xfdce7e90, /// 0x618
			 0x376d43ee, /// 0x61c
			 0x253cda20, /// 0x620
			 0x038e4d11, /// 0x624
			 0xcdff6282, /// 0x628
			 0x2eb616f1, /// 0x62c
			 0x33b6ac5d, /// 0x630
			 0xb0603312, /// 0x634
			 0x4644c494, /// 0x638
			 0xe3574bbd, /// 0x63c
			 0x2102970f, /// 0x640
			 0x62c9461c, /// 0x644
			 0x559748fa, /// 0x648
			 0x575450e0, /// 0x64c
			 0x96ebcad9, /// 0x650
			 0x2998cfd1, /// 0x654
			 0x54329be4, /// 0x658
			 0x22c2d846, /// 0x65c
			 0x8323ea19, /// 0x660
			 0x4f030aa2, /// 0x664
			 0xb12cda55, /// 0x668
			 0xc53eb24e, /// 0x66c
			 0xb5081519, /// 0x670
			 0x53258dcf, /// 0x674
			 0x221ec64f, /// 0x678
			 0xa2b9e084, /// 0x67c
			 0x65a1f44e, /// 0x680
			 0x146d6c2e, /// 0x684
			 0x00a032df, /// 0x688
			 0xbeb01912, /// 0x68c
			 0x31c5214c, /// 0x690
			 0x2fc466ac, /// 0x694
			 0x251f29ac, /// 0x698
			 0xcdf65cd5, /// 0x69c
			 0xd25e0128, /// 0x6a0
			 0x4a1baef5, /// 0x6a4
			 0x174ec230, /// 0x6a8
			 0x793814d9, /// 0x6ac
			 0x496679c0, /// 0x6b0
			 0xe9dd7fc7, /// 0x6b4
			 0xafa55dc1, /// 0x6b8
			 0xe5f97f4b, /// 0x6bc
			 0xcf55e13d, /// 0x6c0
			 0xd00a3331, /// 0x6c4
			 0x4f54bb03, /// 0x6c8
			 0x05b3d8de, /// 0x6cc
			 0x3c279018, /// 0x6d0
			 0x87be6106, /// 0x6d4
			 0x1fa838d1, /// 0x6d8
			 0xb4e390a8, /// 0x6dc
			 0xc9b8c9d6, /// 0x6e0
			 0x6290ec5f, /// 0x6e4
			 0x73397475, /// 0x6e8
			 0x66aa5388, /// 0x6ec
			 0xff26e647, /// 0x6f0
			 0xf462ab59, /// 0x6f4
			 0x5e2b1cd9, /// 0x6f8
			 0x0e0571a3, /// 0x6fc
			 0xcca88d78, /// 0x700
			 0x9d43b767, /// 0x704
			 0x1617eb31, /// 0x708
			 0xbb13459c, /// 0x70c
			 0xe02d0901, /// 0x710
			 0x6e41e181, /// 0x714
			 0xc72429af, /// 0x718
			 0x1b42721e, /// 0x71c
			 0xcd55137f, /// 0x720
			 0x40eb9366, /// 0x724
			 0x4895ffca, /// 0x728
			 0x60f41b8d, /// 0x72c
			 0x3ff91633, /// 0x730
			 0x3b5cf17c, /// 0x734
			 0x403bc481, /// 0x738
			 0x3de05d3a, /// 0x73c
			 0x498326f6, /// 0x740
			 0x9ed8b80d, /// 0x744
			 0xd7ccda3b, /// 0x748
			 0x72f735d3, /// 0x74c
			 0x1227a89d, /// 0x750
			 0x5834373d, /// 0x754
			 0xff934b5a, /// 0x758
			 0x7a543922, /// 0x75c
			 0x3ff23e9a, /// 0x760
			 0x0a181c7f, /// 0x764
			 0x27797c75, /// 0x768
			 0x7773374b, /// 0x76c
			 0xe3a1a013, /// 0x770
			 0x06676bf8, /// 0x774
			 0x1fe0742e, /// 0x778
			 0x3788e7c6, /// 0x77c
			 0x109db324, /// 0x780
			 0x6988164b, /// 0x784
			 0x4dd95622, /// 0x788
			 0x82a02f4f, /// 0x78c
			 0xa820887f, /// 0x790
			 0x35519a22, /// 0x794
			 0xd9da68d0, /// 0x798
			 0xd31378ec, /// 0x79c
			 0x7719c70e, /// 0x7a0
			 0xd3612f86, /// 0x7a4
			 0x516729f1, /// 0x7a8
			 0x0d2c7984, /// 0x7ac
			 0x342a8747, /// 0x7b0
			 0xc7213987, /// 0x7b4
			 0x639b18e6, /// 0x7b8
			 0x81c3fd79, /// 0x7bc
			 0x425d826a, /// 0x7c0
			 0xe10200b5, /// 0x7c4
			 0xa6276be5, /// 0x7c8
			 0xc76d451a, /// 0x7cc
			 0xc5ad10c2, /// 0x7d0
			 0x7af7e50a, /// 0x7d4
			 0x1c33dc85, /// 0x7d8
			 0x211062b6, /// 0x7dc
			 0x61fca0f4, /// 0x7e0
			 0x3ecc6ad3, /// 0x7e4
			 0x97071a93, /// 0x7e8
			 0x674e32dc, /// 0x7ec
			 0x497b2f36, /// 0x7f0
			 0x09112bb7, /// 0x7f4
			 0x4f199cf8, /// 0x7f8
			 0xf738b0db, /// 0x7fc
			 0x9f2b62e3, /// 0x800
			 0xf382792b, /// 0x804
			 0xe2170f69, /// 0x808
			 0x46f3bcb3, /// 0x80c
			 0x3e67f558, /// 0x810
			 0xcbd0c552, /// 0x814
			 0x53fcd433, /// 0x818
			 0xa9a1ddf6, /// 0x81c
			 0xae5cdc69, /// 0x820
			 0x7aac7b46, /// 0x824
			 0x3a52d068, /// 0x828
			 0x57e67660, /// 0x82c
			 0x4d8fc150, /// 0x830
			 0xd510a760, /// 0x834
			 0x8dc976d3, /// 0x838
			 0x8440a061, /// 0x83c
			 0x4648ec8a, /// 0x840
			 0x9be25b8b, /// 0x844
			 0xdaaf2235, /// 0x848
			 0xb943ecd3, /// 0x84c
			 0x40e78576, /// 0x850
			 0x00999b07, /// 0x854
			 0x9c4939e2, /// 0x858
			 0x70c168f7, /// 0x85c
			 0xecbc6f4d, /// 0x860
			 0xde70cd3e, /// 0x864
			 0x66f788f7, /// 0x868
			 0x694c6b8a, /// 0x86c
			 0x25340021, /// 0x870
			 0x52787e3c, /// 0x874
			 0x71528f23, /// 0x878
			 0x9f345c9f, /// 0x87c
			 0xd90d63e4, /// 0x880
			 0x8c6990da, /// 0x884
			 0x4b58aa04, /// 0x888
			 0xa35305c0, /// 0x88c
			 0xc52161b1, /// 0x890
			 0xa516d14a, /// 0x894
			 0x3e09588a, /// 0x898
			 0x7fb052be, /// 0x89c
			 0xa1bb6a56, /// 0x8a0
			 0x0de61323, /// 0x8a4
			 0x43307093, /// 0x8a8
			 0x75351ee9, /// 0x8ac
			 0x17bb3d1c, /// 0x8b0
			 0xff6f33e4, /// 0x8b4
			 0x82420473, /// 0x8b8
			 0x9c5b015a, /// 0x8bc
			 0x9814c8bc, /// 0x8c0
			 0xc0d5b38c, /// 0x8c4
			 0x3300ce01, /// 0x8c8
			 0xbe88e8f5, /// 0x8cc
			 0x3965fdd9, /// 0x8d0
			 0x1eb89a33, /// 0x8d4
			 0xabc26a58, /// 0x8d8
			 0x82e61962, /// 0x8dc
			 0x642e1497, /// 0x8e0
			 0xca2252b9, /// 0x8e4
			 0xc163ee4c, /// 0x8e8
			 0x85a515e5, /// 0x8ec
			 0x3de1b222, /// 0x8f0
			 0x59bdf160, /// 0x8f4
			 0x665d6540, /// 0x8f8
			 0x7668fca8, /// 0x8fc
			 0x5b0a221c, /// 0x900
			 0xe8b97df8, /// 0x904
			 0xae91bc0c, /// 0x908
			 0x1732ace4, /// 0x90c
			 0x11ba30a4, /// 0x910
			 0x37ca9d55, /// 0x914
			 0xc6916744, /// 0x918
			 0xb15310c5, /// 0x91c
			 0x14ff98bf, /// 0x920
			 0x98c7896f, /// 0x924
			 0x0648a638, /// 0x928
			 0x0dd53fda, /// 0x92c
			 0x54c28d4d, /// 0x930
			 0xae328487, /// 0x934
			 0xfbdb55da, /// 0x938
			 0x8568becb, /// 0x93c
			 0x9d920187, /// 0x940
			 0x1bb3d2a4, /// 0x944
			 0x83d0b777, /// 0x948
			 0x2e159962, /// 0x94c
			 0x24249cd8, /// 0x950
			 0x3b8580b6, /// 0x954
			 0x3310870a, /// 0x958
			 0xa30464ef, /// 0x95c
			 0x2421c136, /// 0x960
			 0x695097e0, /// 0x964
			 0x774022d6, /// 0x968
			 0x0d7aaa70, /// 0x96c
			 0xa0537767, /// 0x970
			 0xa4353735, /// 0x974
			 0x5c5c5af5, /// 0x978
			 0x812270a5, /// 0x97c
			 0xb634976d, /// 0x980
			 0x3636a5f6, /// 0x984
			 0x87226f6d, /// 0x988
			 0xde558de9, /// 0x98c
			 0x8ca1d783, /// 0x990
			 0xac2b5a97, /// 0x994
			 0x5dba8131, /// 0x998
			 0x44cc90ac, /// 0x99c
			 0x9f681afc, /// 0x9a0
			 0xdbe1637b, /// 0x9a4
			 0x967e74d8, /// 0x9a8
			 0x6fdb12af, /// 0x9ac
			 0x156906b4, /// 0x9b0
			 0x7b4bc39d, /// 0x9b4
			 0xeca42546, /// 0x9b8
			 0x396aa613, /// 0x9bc
			 0x49e77a2c, /// 0x9c0
			 0xd5e7f4db, /// 0x9c4
			 0xcc3e338e, /// 0x9c8
			 0xfeb86cc2, /// 0x9cc
			 0xe732a559, /// 0x9d0
			 0x7ee1f69b, /// 0x9d4
			 0x21dbc1ce, /// 0x9d8
			 0xd9b4d0ad, /// 0x9dc
			 0xa7e23e8f, /// 0x9e0
			 0x1739715f, /// 0x9e4
			 0x98fe53b8, /// 0x9e8
			 0xedc764bf, /// 0x9ec
			 0xe0a8aef6, /// 0x9f0
			 0x80392157, /// 0x9f4
			 0x5bff2d18, /// 0x9f8
			 0x5d1f54c9, /// 0x9fc
			 0x33151c1e, /// 0xa00
			 0xc226171d, /// 0xa04
			 0xc4050ec4, /// 0xa08
			 0xe2e3082e, /// 0xa0c
			 0x27ce096a, /// 0xa10
			 0x5b558b43, /// 0xa14
			 0x4e1cd146, /// 0xa18
			 0x06f402d5, /// 0xa1c
			 0x625572f0, /// 0xa20
			 0xbb7f0979, /// 0xa24
			 0xca5cca58, /// 0xa28
			 0xdba401df, /// 0xa2c
			 0xa6013dc7, /// 0xa30
			 0x1fc68a9a, /// 0xa34
			 0x893d4a89, /// 0xa38
			 0xd8da5e55, /// 0xa3c
			 0x8ddf5cee, /// 0xa40
			 0x17cd5058, /// 0xa44
			 0x6dc1748e, /// 0xa48
			 0xc9e5e074, /// 0xa4c
			 0xc9fad0e1, /// 0xa50
			 0x777709c7, /// 0xa54
			 0x8cff7b67, /// 0xa58
			 0x051d89a7, /// 0xa5c
			 0x55aa39a2, /// 0xa60
			 0xc5c3b544, /// 0xa64
			 0x2d0b4b5d, /// 0xa68
			 0x6fe0627d, /// 0xa6c
			 0x9ed4146d, /// 0xa70
			 0xc920b97c, /// 0xa74
			 0x3fd4ea66, /// 0xa78
			 0x1926359a, /// 0xa7c
			 0x1f15746e, /// 0xa80
			 0x4da645d2, /// 0xa84
			 0x4c58767d, /// 0xa88
			 0x1c1c7517, /// 0xa8c
			 0xb2a76225, /// 0xa90
			 0xfd42dabb, /// 0xa94
			 0xefc06254, /// 0xa98
			 0x49a825ab, /// 0xa9c
			 0x48a6933d, /// 0xaa0
			 0xd2ce6e86, /// 0xaa4
			 0xa5387d04, /// 0xaa8
			 0x178b76cd, /// 0xaac
			 0x0928652c, /// 0xab0
			 0x647400d4, /// 0xab4
			 0x2d42e673, /// 0xab8
			 0x5b2a4a90, /// 0xabc
			 0x897feac3, /// 0xac0
			 0xf3dd8223, /// 0xac4
			 0x7348d0db, /// 0xac8
			 0x4ac33704, /// 0xacc
			 0x766f0665, /// 0xad0
			 0x4855cb77, /// 0xad4
			 0x5e76776a, /// 0xad8
			 0x4372abb5, /// 0xadc
			 0x4243b7db, /// 0xae0
			 0x5315f6c4, /// 0xae4
			 0xbefe7d0e, /// 0xae8
			 0x0040e113, /// 0xaec
			 0x73af90e5, /// 0xaf0
			 0x88fa4162, /// 0xaf4
			 0x902d50de, /// 0xaf8
			 0x0cfcc67f, /// 0xafc
			 0x6df151cb, /// 0xb00
			 0xc3eb8124, /// 0xb04
			 0x95948c57, /// 0xb08
			 0xcf90c0fd, /// 0xb0c
			 0x2b6289dc, /// 0xb10
			 0x99afc1e7, /// 0xb14
			 0xe302bcd8, /// 0xb18
			 0x65d83121, /// 0xb1c
			 0xac7bd394, /// 0xb20
			 0x485dc374, /// 0xb24
			 0x29fcc930, /// 0xb28
			 0xa378a3c3, /// 0xb2c
			 0x4386e3fd, /// 0xb30
			 0x67c8f04a, /// 0xb34
			 0x05dc75b0, /// 0xb38
			 0xf46b6399, /// 0xb3c
			 0xc305265a, /// 0xb40
			 0xa8f9eadb, /// 0xb44
			 0x358ba7f0, /// 0xb48
			 0x74011892, /// 0xb4c
			 0x3baad3c8, /// 0xb50
			 0xa38cce02, /// 0xb54
			 0x3b47f5bd, /// 0xb58
			 0xa8441c75, /// 0xb5c
			 0x1caf7bf1, /// 0xb60
			 0x6cd94ce9, /// 0xb64
			 0xe1ce4770, /// 0xb68
			 0x9e88fb9a, /// 0xb6c
			 0x83d6054f, /// 0xb70
			 0x37006f18, /// 0xb74
			 0x3ca3dff0, /// 0xb78
			 0x38afa45e, /// 0xb7c
			 0x507b3f91, /// 0xb80
			 0xff7c6760, /// 0xb84
			 0xc09ed040, /// 0xb88
			 0x607d7f18, /// 0xb8c
			 0x18a458dd, /// 0xb90
			 0x835d3829, /// 0xb94
			 0xcf67e40c, /// 0xb98
			 0xebf92a60, /// 0xb9c
			 0xfc182408, /// 0xba0
			 0xcb0f4ab4, /// 0xba4
			 0x2841e9e9, /// 0xba8
			 0xb804f21f, /// 0xbac
			 0x23f7d593, /// 0xbb0
			 0x6850e1a0, /// 0xbb4
			 0x42a31f0b, /// 0xbb8
			 0xc3250d1c, /// 0xbbc
			 0x643f1df5, /// 0xbc0
			 0xae03a4b5, /// 0xbc4
			 0x4f159194, /// 0xbc8
			 0x9f156b80, /// 0xbcc
			 0x16510be5, /// 0xbd0
			 0xfd67947d, /// 0xbd4
			 0x22a7ca26, /// 0xbd8
			 0xeb2739a8, /// 0xbdc
			 0x69ca423b, /// 0xbe0
			 0x15f628c8, /// 0xbe4
			 0x12ec564a, /// 0xbe8
			 0x82fe9e2b, /// 0xbec
			 0xde8ce609, /// 0xbf0
			 0xb437f7c2, /// 0xbf4
			 0xb8cf70bb, /// 0xbf8
			 0x60643b1d, /// 0xbfc
			 0x38827fc9, /// 0xc00
			 0x1b3722c5, /// 0xc04
			 0x7ef9fc9a, /// 0xc08
			 0xcaaaaea9, /// 0xc0c
			 0x3a4832c4, /// 0xc10
			 0xe038dfd8, /// 0xc14
			 0x83513645, /// 0xc18
			 0x50196dd5, /// 0xc1c
			 0x51a8340d, /// 0xc20
			 0x3f962cee, /// 0xc24
			 0x749a4472, /// 0xc28
			 0xdd8b43cd, /// 0xc2c
			 0xdf4e2087, /// 0xc30
			 0x7299021b, /// 0xc34
			 0x6809a818, /// 0xc38
			 0x6f36500f, /// 0xc3c
			 0x309efd5f, /// 0xc40
			 0x6c5d14dc, /// 0xc44
			 0x7df0b954, /// 0xc48
			 0xc26b8c67, /// 0xc4c
			 0x379e10eb, /// 0xc50
			 0x4b404d68, /// 0xc54
			 0x6e2311da, /// 0xc58
			 0x48792a05, /// 0xc5c
			 0xd58a2275, /// 0xc60
			 0xe0df6adc, /// 0xc64
			 0xa1add799, /// 0xc68
			 0x35960905, /// 0xc6c
			 0x485fa25a, /// 0xc70
			 0x92524233, /// 0xc74
			 0x1676abeb, /// 0xc78
			 0x50056b09, /// 0xc7c
			 0xe7886e1d, /// 0xc80
			 0xcd8fe0a0, /// 0xc84
			 0x7642d9aa, /// 0xc88
			 0x979fa3a5, /// 0xc8c
			 0xd8f08c2e, /// 0xc90
			 0x0b573336, /// 0xc94
			 0xbf1ee95d, /// 0xc98
			 0x0156ef45, /// 0xc9c
			 0xd9894296, /// 0xca0
			 0xdb63ff10, /// 0xca4
			 0x083d9301, /// 0xca8
			 0x4d834088, /// 0xcac
			 0x0436b65c, /// 0xcb0
			 0xdb2e2297, /// 0xcb4
			 0x34482b43, /// 0xcb8
			 0x66ed4341, /// 0xcbc
			 0xc8c2127f, /// 0xcc0
			 0x4537f272, /// 0xcc4
			 0x19892c3a, /// 0xcc8
			 0x97af0808, /// 0xccc
			 0xab09bf2d, /// 0xcd0
			 0x722bddac, /// 0xcd4
			 0x49669491, /// 0xcd8
			 0x0eb631eb, /// 0xcdc
			 0x961bb709, /// 0xce0
			 0xd222f5b2, /// 0xce4
			 0x68f5fa8e, /// 0xce8
			 0xa073be03, /// 0xcec
			 0xa9d83222, /// 0xcf0
			 0x68c04ebe, /// 0xcf4
			 0xee64f218, /// 0xcf8
			 0x78125a8f, /// 0xcfc
			 0x5af2b73f, /// 0xd00
			 0xf201d408, /// 0xd04
			 0xc4b2573e, /// 0xd08
			 0x593eaff9, /// 0xd0c
			 0x5e3771bc, /// 0xd10
			 0x8c05d17c, /// 0xd14
			 0xf102b424, /// 0xd18
			 0x4a730ed5, /// 0xd1c
			 0xa49ffed2, /// 0xd20
			 0xa3deff98, /// 0xd24
			 0x7a19f471, /// 0xd28
			 0xedae9bbb, /// 0xd2c
			 0xb271a843, /// 0xd30
			 0x10a8900a, /// 0xd34
			 0x43cd5972, /// 0xd38
			 0xc54e4053, /// 0xd3c
			 0xa52d1782, /// 0xd40
			 0xc6fc47a8, /// 0xd44
			 0x3288bf47, /// 0xd48
			 0xd68c31b1, /// 0xd4c
			 0xc9dc0320, /// 0xd50
			 0xf024a916, /// 0xd54
			 0x210b040b, /// 0xd58
			 0x01fb6805, /// 0xd5c
			 0x956303e6, /// 0xd60
			 0x253f3611, /// 0xd64
			 0x6abd5fb4, /// 0xd68
			 0xf4a2e225, /// 0xd6c
			 0xe5186bc3, /// 0xd70
			 0x07d56deb, /// 0xd74
			 0x49d24b8b, /// 0xd78
			 0x71c9ff1f, /// 0xd7c
			 0x9915c9b0, /// 0xd80
			 0xc2260cae, /// 0xd84
			 0xd057b6dc, /// 0xd88
			 0xda41faec, /// 0xd8c
			 0xc2e98ce1, /// 0xd90
			 0x8ef522ec, /// 0xd94
			 0x3244b37a, /// 0xd98
			 0x33f49923, /// 0xd9c
			 0xb1964529, /// 0xda0
			 0xcd8e898f, /// 0xda4
			 0x758f969e, /// 0xda8
			 0x05e85d0b, /// 0xdac
			 0xeac377dc, /// 0xdb0
			 0xd57f6fa6, /// 0xdb4
			 0xd95d20e8, /// 0xdb8
			 0x36cfc409, /// 0xdbc
			 0x706487c9, /// 0xdc0
			 0xc3404e0a, /// 0xdc4
			 0x5203d6df, /// 0xdc8
			 0x528a5897, /// 0xdcc
			 0x73e28bf9, /// 0xdd0
			 0x10049034, /// 0xdd4
			 0x7f5cf73c, /// 0xdd8
			 0x9ee2eadd, /// 0xddc
			 0x355392f4, /// 0xde0
			 0x1720dc0c, /// 0xde4
			 0x12be2c89, /// 0xde8
			 0x2d520047, /// 0xdec
			 0x23984f31, /// 0xdf0
			 0xffb834ad, /// 0xdf4
			 0x18e09526, /// 0xdf8
			 0xcfd9a383, /// 0xdfc
			 0x180f68ed, /// 0xe00
			 0xdd2ddcb6, /// 0xe04
			 0xb84356d3, /// 0xe08
			 0x78cd08bf, /// 0xe0c
			 0x00ec4a99, /// 0xe10
			 0x9717f830, /// 0xe14
			 0x55500f7e, /// 0xe18
			 0xf0b82a67, /// 0xe1c
			 0x91a33792, /// 0xe20
			 0x5e0bbf48, /// 0xe24
			 0x75559737, /// 0xe28
			 0x3fd86c9a, /// 0xe2c
			 0xf5f4a4c9, /// 0xe30
			 0x2a8b0b8e, /// 0xe34
			 0xda961572, /// 0xe38
			 0x2baa0feb, /// 0xe3c
			 0xd4f3c545, /// 0xe40
			 0x04071f77, /// 0xe44
			 0xcc1732ff, /// 0xe48
			 0x17791aac, /// 0xe4c
			 0xe5c5b6f4, /// 0xe50
			 0xe1bbc8fd, /// 0xe54
			 0xc5f13b4c, /// 0xe58
			 0x248e6806, /// 0xe5c
			 0xef84a765, /// 0xe60
			 0xff375efb, /// 0xe64
			 0x576feb86, /// 0xe68
			 0x8c7e4a37, /// 0xe6c
			 0x4b5dab56, /// 0xe70
			 0xfaf949cc, /// 0xe74
			 0x552e1a96, /// 0xe78
			 0xecf421d0, /// 0xe7c
			 0x3e74031d, /// 0xe80
			 0xfebd5cfd, /// 0xe84
			 0xedda2d17, /// 0xe88
			 0x915ec6da, /// 0xe8c
			 0x3b14a798, /// 0xe90
			 0xfb09cc82, /// 0xe94
			 0x20a98c57, /// 0xe98
			 0x14bd79ea, /// 0xe9c
			 0x90717b19, /// 0xea0
			 0x9a83d0f0, /// 0xea4
			 0x7b2d697a, /// 0xea8
			 0x79e1c87b, /// 0xeac
			 0x88acf3f2, /// 0xeb0
			 0x1544c3b0, /// 0xeb4
			 0xa2d6d4a6, /// 0xeb8
			 0x9b66b80c, /// 0xebc
			 0x9bd0160f, /// 0xec0
			 0x81523097, /// 0xec4
			 0x7e91faa7, /// 0xec8
			 0x44120722, /// 0xecc
			 0xe31d94f2, /// 0xed0
			 0x344b9d21, /// 0xed4
			 0xda2858bd, /// 0xed8
			 0x2771920e, /// 0xedc
			 0x65127fd0, /// 0xee0
			 0xbb5ad087, /// 0xee4
			 0xa3d760ea, /// 0xee8
			 0x414683c0, /// 0xeec
			 0x133647a2, /// 0xef0
			 0x031dd187, /// 0xef4
			 0xdb408225, /// 0xef8
			 0x57f31174, /// 0xefc
			 0x16b7336f, /// 0xf00
			 0xa1f4a847, /// 0xf04
			 0x5b503356, /// 0xf08
			 0x88fbc869, /// 0xf0c
			 0xea4cb4d1, /// 0xf10
			 0xbead558d, /// 0xf14
			 0xc01f8fa8, /// 0xf18
			 0xac44229d, /// 0xf1c
			 0xdab884cb, /// 0xf20
			 0xc85e4ac0, /// 0xf24
			 0x95f8fbdc, /// 0xf28
			 0x8abe3049, /// 0xf2c
			 0x20bb3044, /// 0xf30
			 0xc5d6d67d, /// 0xf34
			 0x2314b3bb, /// 0xf38
			 0x191358ea, /// 0xf3c
			 0x0d5798fc, /// 0xf40
			 0x3e70bdc5, /// 0xf44
			 0x26a3ed70, /// 0xf48
			 0x89d913d3, /// 0xf4c
			 0x2c20bb77, /// 0xf50
			 0x4ad35179, /// 0xf54
			 0x3327a915, /// 0xf58
			 0xa70c7c1d, /// 0xf5c
			 0x0453997d, /// 0xf60
			 0xb8958967, /// 0xf64
			 0x00dab1ae, /// 0xf68
			 0xcbadedcc, /// 0xf6c
			 0x706167f8, /// 0xf70
			 0xef7c12ba, /// 0xf74
			 0x4784d364, /// 0xf78
			 0x83da8cfd, /// 0xf7c
			 0x61b18ed2, /// 0xf80
			 0x459f7709, /// 0xf84
			 0x2672a31f, /// 0xf88
			 0x73aa0e0b, /// 0xf8c
			 0x98e49b5c, /// 0xf90
			 0x13b051b3, /// 0xf94
			 0x677fc10f, /// 0xf98
			 0x7e2e0f5f, /// 0xf9c
			 0x6ea0b7a5, /// 0xfa0
			 0x3e510269, /// 0xfa4
			 0x4072fe92, /// 0xfa8
			 0x48277f5a, /// 0xfac
			 0x0b52c9d7, /// 0xfb0
			 0x96fcb789, /// 0xfb4
			 0x4efbb719, /// 0xfb8
			 0x0980cf04, /// 0xfbc
			 0xaea022c3, /// 0xfc0
			 0xfb8f9658, /// 0xfc4
			 0x587f1428, /// 0xfc8
			 0xa5029221, /// 0xfcc
			 0xb62dcd85, /// 0xfd0
			 0xf9dd0ef5, /// 0xfd4
			 0xa1f83a97, /// 0xfd8
			 0x6447cc3b, /// 0xfdc
			 0x07cf079f, /// 0xfe0
			 0x9fbb6f3f, /// 0xfe4
			 0xaa076027, /// 0xfe8
			 0xd7b1faa9, /// 0xfec
			 0x83e0c30f, /// 0xff0
			 0x22476ea2, /// 0xff4
			 0x1ada0bbe, /// 0xff8
			 0x3e9123cb /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x4b000000,                                                  // 8388608.0                                   /// 00000
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00004
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0000c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00014
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0001c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00020
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00024
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00028
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0002c
			 0xbf028f5c,                                                  // -0.51                                       /// 00030
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00034
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00040
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00048
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00054
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0005c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00064
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x0e000003,                                                  // Trailing 1s:                                /// 0006c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00070
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x0e000001,                                                  // Trailing 1s:                                /// 0007c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00080
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00088
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0008c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00098
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0009c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x3f028f5c,                                                  // 0.51                                        /// 000dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00104
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00108
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0010c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00120
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x00000000,                                                  // zero                                        /// 00128
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00130
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x0c780000,                                                  // Leading 1s:                                 /// 00138
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0014c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00150
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x33333333,                                                  // 4 random values                             /// 00160
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00168
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0c400000,                                                  // Leading 1s:                                 /// 00178
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0017c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00184
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00188
			 0xff800000,                                                  // -inf                                        /// 0018c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00194
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0019c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 001dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001f4
			 0x3f028f5c,                                                  // 0.51                                        /// 001f8
			 0x33333333,                                                  // 4 random values                             /// 001fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00200
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00204
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0020c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00218
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0021c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00220
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00228
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0022c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00230
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00238
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00240
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00244
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00248
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0024c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00254
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0025c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00260
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00264
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00270
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0c600000,                                                  // Leading 1s:                                 /// 00278
			 0xbf028f5c,                                                  // -0.51                                       /// 0027c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00280
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x0c400000,                                                  // Leading 1s:                                 /// 0028c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00290
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00294
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00298
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0029c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002e4
			 0x00000000,                                                  // zero                                        /// 002e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00300
			 0x0c400000,                                                  // Leading 1s:                                 /// 00304
			 0x3f028f5c,                                                  // 0.51                                        /// 00308
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00320
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00324
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00334
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00338
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00344
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0034c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00350
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00354
			 0x55555555,                                                  // 4 random values                             /// 00358
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0035c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00364
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00368
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0036c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00370
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80011111,                                                  // -9.7958E-41                                 /// 00378
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0e000001,                                                  // Trailing 1s:                                /// 0038c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00394
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0039c
			 0x55555555,                                                  // 4 random values                             /// 003a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00400
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00404
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00408
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0040c
			 0x80000000,                                                  // -zero                                       /// 00410
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00414
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00420
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00428
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0042c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0043c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00444
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x7fc00001,                                                  // signaling NaN                               /// 0044c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00454
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00458
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0045c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00460
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00468
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00484
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xbf028f5c,                                                  // -0.51                                       /// 00490
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00494
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00498
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004bc
			 0x55555555,                                                  // 4 random values                             /// 004c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00500
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0050c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00518
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0051c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00520
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00524
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00528
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0052c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00530
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00534
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00548
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0054c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00554
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00558
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00560
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00568
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0056c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00570
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00578
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0057c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x80011111,                                                  // -9.7958E-41                                 /// 00584
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0058c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00590
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0e000007,                                                  // Trailing 1s:                                /// 00598
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0059c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005a4
			 0x33333333,                                                  // 4 random values                             /// 005a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005c8
			 0x33333333,                                                  // 4 random values                             /// 005cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005d4
			 0x4b000000,                                                  // 8388608.0                                   /// 005d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0xff800000,                                                  // -inf                                        /// 005ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x7f800000,                                                  // inf                                         /// 00600
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00604
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00608
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0060c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00610
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00614
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0061c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00624
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x33333333,                                                  // 4 random values                             /// 00630
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00634
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00638
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0063c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00640
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00644
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00648
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0064c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00650
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00654
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00658
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xbf028f5c,                                                  // -0.51                                       /// 00660
			 0x7fc00001,                                                  // signaling NaN                               /// 00664
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00668
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0066c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00670
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00678
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00680
			 0x7fc00001,                                                  // signaling NaN                               /// 00684
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00688
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0068c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00690
			 0x80011111,                                                  // -9.7958E-41                                 /// 00694
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006b8
			 0x7fc00001,                                                  // signaling NaN                               /// 006bc
			 0x55555555,                                                  // 4 random values                             /// 006c0
			 0x4b000000,                                                  // 8388608.0                                   /// 006c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006e0
			 0xff800000,                                                  // -inf                                        /// 006e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00704
			 0xff800000,                                                  // -inf                                        /// 00708
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0070c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00710
			 0x0c400000,                                                  // Leading 1s:                                 /// 00714
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00720
			 0x0e000001,                                                  // Trailing 1s:                                /// 00724
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00728
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0072c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00734
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0073c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00744
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x55555555,                                                  // 4 random values                             /// 00750
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00758
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0075c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00760
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00764
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00768
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0076c
			 0x33333333,                                                  // 4 random values                             /// 00770
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00774
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00780
			 0xbf028f5c,                                                  // -0.51                                       /// 00784
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00788
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0078c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00794
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00798
			 0x80011111,                                                  // -9.7958E-41                                 /// 0079c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007b0
			 0x3f028f5c,                                                  // 0.51                                        /// 007b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 007c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d0
			 0xcb000000,                                                  // -8388608.0                                  /// 007d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007f0
			 0x00000000,                                                  // zero                                        /// 007f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00800
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00804
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0080c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00810
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x7f800000,                                                  // inf                                         /// 00818
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0081c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00820
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0xffc00001,                                                  // -signaling NaN                              /// 00828
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00834
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00844
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00848
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0084c
			 0xcb000000,                                                  // -8388608.0                                  /// 00850
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00858
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00868
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0086c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00870
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0087c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00880
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00884
			 0x0c400000,                                                  // Leading 1s:                                 /// 00888
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00894
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00898
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0089c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008d0
			 0x55555555,                                                  // 4 random values                             /// 008d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008dc
			 0x55555555,                                                  // 4 random values                             /// 008e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00904
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00908
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0090c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00910
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00924
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00928
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00930
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00934
			 0x7f800000,                                                  // inf                                         /// 00938
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00944
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0094c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00958
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0095c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00964
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00968
			 0x0e000003,                                                  // Trailing 1s:                                /// 0096c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00974
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00980
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00990
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00994
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00998
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0099c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009d8
			 0x80000000,                                                  // -zero                                       /// 009dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a08
			 0x7fc00001,                                                  // signaling NaN                               /// 00a0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a14
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a24
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0xffc00001,                                                  // -signaling NaN                              /// 00a54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a5c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a84
			 0x55555555,                                                  // 4 random values                             /// 00a88
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a8c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00aa0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00aac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ab0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ab4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00abc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ac4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac8
			 0x7f800000,                                                  // inf                                         /// 00acc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ad0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ad8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00adc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ae0
			 0x55555555,                                                  // 4 random values                             /// 00ae4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ae8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00af0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00af4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00af8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00afc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b14
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b54
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b68
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b78
			 0x4b000000,                                                  // 8388608.0                                   /// 00b7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b84
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b90
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ba0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ba8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bb8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00be4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bf0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bf4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c0c
			 0x00000000,                                                  // zero                                        /// 00c10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c14
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c34
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x80000000,                                                  // -zero                                       /// 00ca8
			 0x33333333,                                                  // 4 random values                             /// 00cac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cb8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cdc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ce0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ce8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cf0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cf8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xff800000,                                                  // -inf                                        /// 00d00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d0c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d1c
			 0xff800000,                                                  // -inf                                        /// 00d20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x7f800000,                                                  // inf                                         /// 00d2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d30
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d44
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d58
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d5c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x7f800000,                                                  // inf                                         /// 00d6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d7c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00da0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00da8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00db4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dbc
			 0x7fc00001,                                                  // signaling NaN                               /// 00dc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dcc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dd0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00dd8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00de0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00de8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00df8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e08
			 0x33333333,                                                  // 4 random values                             /// 00e0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e18
			 0xffc00001,                                                  // -signaling NaN                              /// 00e1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e28
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e30
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e50
			 0x00000000,                                                  // zero                                        /// 00e54
			 0xffc00001,                                                  // -signaling NaN                              /// 00e58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x00000000,                                                  // zero                                        /// 00e74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e80
			 0x7f800000,                                                  // inf                                         /// 00e84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e88
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ea8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00eb8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ebc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ec4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ecc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ed0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ed8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00edc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ee4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ee8
			 0x00000000,                                                  // zero                                        /// 00eec
			 0x00000000,                                                  // zero                                        /// 00ef0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00efc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f10
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f24
			 0xff800000,                                                  // -inf                                        /// 00f28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f7c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f8c
			 0x33333333,                                                  // 4 random values                             /// 00f90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fa0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fc0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fc8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00fd0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fdc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ff0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff8
			 0x80000100                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00004
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00008
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00010
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00014
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00018
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00024
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00028
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0002c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00030
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0003c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00040
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00044
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00048
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0004c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00060
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00064
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0006c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00070
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00074
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00078
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0007c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00080
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00084
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0009c
			 0x00000000,                                                  // zero                                        /// 000a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0xcb000000,                                                  // -8388608.0                                  /// 000c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00104
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0010c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00110
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00114
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00118
			 0x33333333,                                                  // 4 random values                             /// 0011c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00120
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00124
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00128
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0012c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00130
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00134
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00138
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0013c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00148
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00150
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00158
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0015c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00160
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00164
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00168
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0016c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00170
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00174
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00178
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00180
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00184
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00188
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00190
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00194
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00198
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0019c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001b8
			 0x80000000,                                                  // -zero                                       /// 001bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001e0
			 0x3f028f5c,                                                  // 0.51                                        /// 001e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00214
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00218
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00220
			 0xbf028f5c,                                                  // -0.51                                       /// 00224
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00228
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00230
			 0x7fc00001,                                                  // signaling NaN                               /// 00234
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0xcb000000,                                                  // -8388608.0                                  /// 0023c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00240
			 0x00011111,                                                  // 9.7958E-41                                  /// 00244
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00248
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00250
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x00000000,                                                  // zero                                        /// 00260
			 0x0e000001,                                                  // Trailing 1s:                                /// 00264
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00268
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00270
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00274
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00278
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0027c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00280
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00288
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0028c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00298
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0029c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00300
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00304
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00314
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00318
			 0x80000000,                                                  // -zero                                       /// 0031c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00320
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00328
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00330
			 0xff800000,                                                  // -inf                                        /// 00334
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00338
			 0x0c700000,                                                  // Leading 1s:                                 /// 0033c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00340
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00348
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0034c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x80000000,                                                  // -zero                                       /// 00354
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00360
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00370
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00374
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00378
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0037c
			 0x80000000,                                                  // -zero                                       /// 00380
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00388
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0038c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80000000,                                                  // -zero                                       /// 00394
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80000000,                                                  // -zero                                       /// 0039c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003b4
			 0xbf028f5c,                                                  // -0.51                                       /// 003b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e0
			 0x33333333,                                                  // 4 random values                             /// 003e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00404
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00408
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00410
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00418
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00420
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00424
			 0x0c400000,                                                  // Leading 1s:                                 /// 00428
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0042c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00430
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00434
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00438
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00444
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00448
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00458
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0045c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00460
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0046c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00470
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00478
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00480
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00484
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0048c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00494
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0049c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 004b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d0
			 0xbf028f5c,                                                  // -0.51                                       /// 004d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004e8
			 0xbf028f5c,                                                  // -0.51                                       /// 004ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00500
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00504
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00514
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00518
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00524
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00528
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0052c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00530
			 0xffc00001,                                                  // -signaling NaN                              /// 00534
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00538
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00540
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00548
			 0x0c700000,                                                  // Leading 1s:                                 /// 0054c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00550
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00558
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00560
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0056c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00570
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00574
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0057c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00580
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00584
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0058c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00594
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0059c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005ac
			 0x7f800000,                                                  // inf                                         /// 005b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x55555555,                                                  // 4 random values                             /// 005e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00600
			 0x0c400000,                                                  // Leading 1s:                                 /// 00604
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0060c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00614
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00618
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0061c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00620
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00624
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00628
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00630
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00634
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0063c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00644
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00648
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0064c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x80000000,                                                  // -zero                                       /// 00658
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00660
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00664
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00668
			 0x4b000000,                                                  // 8388608.0                                   /// 0066c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00674
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0067c
			 0xff800000,                                                  // -inf                                        /// 00680
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00684
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0068c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00690
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00694
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x80000000,                                                  // -zero                                       /// 0069c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006d0
			 0xbf028f5c,                                                  // -0.51                                       /// 006d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x00000000,                                                  // zero                                        /// 006f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00700
			 0x0c600000,                                                  // Leading 1s:                                 /// 00704
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0070c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00710
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00714
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0071c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00720
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00724
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00728
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00734
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0073c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00740
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0074c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00754
			 0x55555555,                                                  // 4 random values                             /// 00758
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0075c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00764
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0076c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00770
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00774
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00778
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00780
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00784
			 0xcb000000,                                                  // -8388608.0                                  /// 00788
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00794
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x55555555,                                                  // 4 random values                             /// 007d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00800
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0080c
			 0xff800000,                                                  // -inf                                        /// 00810
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00814
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0081c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00824
			 0x0e000003,                                                  // Trailing 1s:                                /// 00828
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00830
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00834
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00838
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0083c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00844
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00848
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0084c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00850
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00854
			 0xffc00001,                                                  // -signaling NaN                              /// 00858
			 0x0e000001,                                                  // Trailing 1s:                                /// 0085c
			 0x3f028f5c,                                                  // 0.51                                        /// 00860
			 0x55555555,                                                  // 4 random values                             /// 00864
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00868
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00870
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00878
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0087c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00884
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x80011111,                                                  // -9.7958E-41                                 /// 0088c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00890
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00894
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 008e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00900
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00904
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00908
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00910
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00920
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00924
			 0x7f800000,                                                  // inf                                         /// 00928
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00930
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00934
			 0x7fc00001,                                                  // signaling NaN                               /// 00938
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0093c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00940
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00944
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x3f028f5c,                                                  // 0.51                                        /// 0094c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00950
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00958
			 0xbf028f5c,                                                  // -0.51                                       /// 0095c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00968
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0096c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00970
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00978
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0097c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00980
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00988
			 0xcb000000,                                                  // -8388608.0                                  /// 0098c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00990
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00994
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0099c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009b0
			 0xbf028f5c,                                                  // -0.51                                       /// 009b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x7f800000,                                                  // inf                                         /// 009e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x7f800000,                                                  // inf                                         /// 00a04
			 0xbf028f5c,                                                  // -0.51                                       /// 00a08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x00000000,                                                  // zero                                        /// 00a28
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a3c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a40
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a58
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a70
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a90
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0xcb000000,                                                  // -8388608.0                                  /// 00a9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00aa0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aa4
			 0x7f800000,                                                  // inf                                         /// 00aa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ab0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ab4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00abc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ac0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ac4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00acc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ad0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00adc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ae0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ae8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00af0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00af4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00af8
			 0xcb000000,                                                  // -8388608.0                                  /// 00afc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b00
			 0xffc00001,                                                  // -signaling NaN                              /// 00b04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b10
			 0x7f800000,                                                  // inf                                         /// 00b14
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b24
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b28
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b34
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b3c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b50
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b54
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b70
			 0x55555555,                                                  // 4 random values                             /// 00b74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b88
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x3f028f5c,                                                  // 0.51                                        /// 00b9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ba8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bbc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bd4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bdc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00be4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00be8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bf8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bfc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c0c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c30
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c40
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c48
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c50
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c74
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c78
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c88
			 0x7fc00001,                                                  // signaling NaN                               /// 00c8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c98
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cb0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cb4
			 0x33333333,                                                  // 4 random values                             /// 00cb8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cd4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x55555555,                                                  // 4 random values                             /// 00ce0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ce4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ce8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cf0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cf4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d04
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0xbf028f5c,                                                  // -0.51                                       /// 00d2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x7f800000,                                                  // inf                                         /// 00d3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d40
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d50
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d58
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d60
			 0x7f800000,                                                  // inf                                         /// 00d64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d68
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d70
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d8c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00da0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00da4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00db4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00db8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dcc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dd0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ddc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00de0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00de8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00df4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e08
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e30
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e38
			 0x55555555,                                                  // 4 random values                             /// 00e3c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e48
			 0xcb000000,                                                  // -8388608.0                                  /// 00e4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e54
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e68
			 0x4b000000,                                                  // 8388608.0                                   /// 00e6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e78
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eb8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ebc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ec0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ecc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ee4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ef4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ef8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00efc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f10
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f14
			 0xcb000000,                                                  // -8388608.0                                  /// 00f18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f4c
			 0x80000000,                                                  // -zero                                       /// 00f50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f64
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f78
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0xff800000,                                                  // -inf                                        /// 00fac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fc0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fc4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fcc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fd0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fe0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fe4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fe8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ff0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ff8
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
			 0x000001e4,
			 0x00000638,
			 0x00000378,
			 0x000002a4,
			 0x0000044c,
			 0x0000079c,
			 0x000004f8,
			 0x000002bc,

			 /// vpu register f2
			 0x41500000,
			 0x41900000,
			 0x41f00000,
			 0x41d00000,
			 0x40000000,
			 0x41400000,
			 0x41a80000,
			 0x41c00000,

			 /// vpu register f3
			 0x41300000,
			 0x41e80000,
			 0x41a00000,
			 0x3f800000,
			 0x41a80000,
			 0x41a80000,
			 0x40e00000,
			 0x41a00000,

			 /// vpu register f4
			 0x41600000,
			 0x41f00000,
			 0x41980000,
			 0x41b80000,
			 0x41800000,
			 0x41e00000,
			 0x41d00000,
			 0x41b80000,

			 /// vpu register f5
			 0x41900000,
			 0x41c00000,
			 0x41500000,
			 0x40800000,
			 0x41300000,
			 0x40800000,
			 0x41700000,
			 0x41f80000,

			 /// vpu register f6
			 0x41e00000,
			 0x40c00000,
			 0x41500000,
			 0x41d00000,
			 0x41d80000,
			 0x41d00000,
			 0x41900000,
			 0x41800000,

			 /// vpu register f7
			 0x40400000,
			 0x41d00000,
			 0x41980000,
			 0x41c80000,
			 0x41c00000,
			 0x41d80000,
			 0x41200000,
			 0x41a00000,

			 /// vpu register f8
			 0x41980000,
			 0x41400000,
			 0x41980000,
			 0x41a80000,
			 0x41f80000,
			 0x41c00000,
			 0x41e00000,
			 0x41100000,

			 /// vpu register f9
			 0x40a00000,
			 0x41a00000,
			 0x41400000,
			 0x40000000,
			 0x41f00000,
			 0x40400000,
			 0x42000000,
			 0x41c00000,

			 /// vpu register f10
			 0x41a80000,
			 0x41300000,
			 0x41500000,
			 0x41a80000,
			 0x41e80000,
			 0x41b80000,
			 0x41700000,
			 0x3f800000,

			 /// vpu register f11
			 0x41980000,
			 0x41f80000,
			 0x41980000,
			 0x40e00000,
			 0x41d80000,
			 0x41a80000,
			 0x40800000,
			 0x41f80000,

			 /// vpu register f12
			 0x40a00000,
			 0x41880000,
			 0x40e00000,
			 0x41900000,
			 0x41d80000,
			 0x41300000,
			 0x40c00000,
			 0x41880000,

			 /// vpu register f13
			 0x41b00000,
			 0x41600000,
			 0x42000000,
			 0x41880000,
			 0x41880000,
			 0x41c80000,
			 0x41400000,
			 0x41500000,

			 /// vpu register f14
			 0x41e80000,
			 0x41800000,
			 0x41400000,
			 0x41880000,
			 0x41a80000,
			 0x41b00000,
			 0x41b80000,
			 0x41700000,

			 /// vpu register f15
			 0x41800000,
			 0x41880000,
			 0x41c80000,
			 0x41c80000,
			 0x41100000,
			 0x41b80000,
			 0x41500000,
			 0x41f80000,

			 /// vpu register f16
			 0x41300000,
			 0x41500000,
			 0x41b80000,
			 0x41e80000,
			 0x40400000,
			 0x41a80000,
			 0x41e00000,
			 0x41700000,

			 /// vpu register f17
			 0x41b80000,
			 0x41f80000,
			 0x41000000,
			 0x41a80000,
			 0x41900000,
			 0x40a00000,
			 0x41f80000,
			 0x41c80000,

			 /// vpu register f18
			 0x40000000,
			 0x41900000,
			 0x41f80000,
			 0x40400000,
			 0x41f00000,
			 0x41c00000,
			 0x41800000,
			 0x41880000,

			 /// vpu register f19
			 0x41600000,
			 0x41700000,
			 0x41880000,
			 0x41980000,
			 0x41e80000,
			 0x40e00000,
			 0x41d00000,
			 0x40c00000,

			 /// vpu register f20
			 0x41400000,
			 0x41d80000,
			 0x41000000,
			 0x41800000,
			 0x41300000,
			 0x40c00000,
			 0x41700000,
			 0x41c00000,

			 /// vpu register f21
			 0x41800000,
			 0x41300000,
			 0x40c00000,
			 0x41100000,
			 0x40a00000,
			 0x41a00000,
			 0x41f80000,
			 0x41500000,

			 /// vpu register f22
			 0x41a00000,
			 0x41f00000,
			 0x41d00000,
			 0x41d00000,
			 0x41600000,
			 0x40a00000,
			 0x41700000,
			 0x41c80000,

			 /// vpu register f23
			 0x41200000,
			 0x41600000,
			 0x41c00000,
			 0x41100000,
			 0x41e00000,
			 0x41880000,
			 0x41000000,
			 0x40800000,

			 /// vpu register f24
			 0x3f800000,
			 0x40400000,
			 0x41d00000,
			 0x41d80000,
			 0x42000000,
			 0x41e80000,
			 0x40a00000,
			 0x41900000,

			 /// vpu register f25
			 0x41980000,
			 0x41000000,
			 0x41500000,
			 0x41400000,
			 0x41800000,
			 0x41e00000,
			 0x41e80000,
			 0x40800000,

			 /// vpu register f26
			 0x40800000,
			 0x41400000,
			 0x41300000,
			 0x41d00000,
			 0x41980000,
			 0x41300000,
			 0x41880000,
			 0x41a00000,

			 /// vpu register f27
			 0x3f800000,
			 0x41b00000,
			 0x41900000,
			 0x40800000,
			 0x41c80000,
			 0x41e00000,
			 0x41d80000,
			 0x41b00000,

			 /// vpu register f28
			 0x41e00000,
			 0x40000000,
			 0x3f800000,
			 0x41200000,
			 0x41a00000,
			 0x41b00000,
			 0x41000000,
			 0x41200000,

			 /// vpu register f29
			 0x41a00000,
			 0x41e00000,
			 0x41b80000,
			 0x40800000,
			 0x41e00000,
			 0x41e00000,
			 0x41c00000,
			 0x41700000,

			 /// vpu register f30
			 0x40a00000,
			 0x40c00000,
			 0x3f800000,
			 0x41880000,
			 0x40000000,
			 0x41100000,
			 0x41e80000,
			 0x40000000,

			 /// vpu register f31
			 0x41100000,
			 0x41d80000,
			 0x41400000,
			 0x41a00000,
			 0x41e00000,
			 0x40800000,
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
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f25, f1 (x12)\n"                        ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f27, f1 (x14)\n"                        ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f28, f1 (x14)\n"                        ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f8, f1 (x14)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f10, f1 (x14)\n"                        ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f29, f1 (x15)\n"                        ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f11, f1 (x13)\n"                        ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f7, f1 (x12)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f19, f1 (x11)\n"                        ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f13, f1 (x11)\n"                        ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f27, f1 (x15)\n"                        ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f18, f1 (x11)\n"                        ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f22, f1 (x15)\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f25, f1 (x13)\n"                        ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f8, f1 (x15)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f3, f1 (x14)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f5, f1 (x13)\n"                         ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f10, f1 (x13)\n"                        ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f20, f1 (x14)\n"                        ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f14, f1 (x11)\n"                        ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f2, f1 (x14)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f26, f1 (x14)\n"                        ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f16, f1 (x11)\n"                        ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f10, f1 (x14)\n"                        ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f6, f1 (x13)\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f14, f1 (x15)\n"                        ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f24, f1 (x11)\n"                        ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f12, f1 (x14)\n"                        ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f3, f1 (x11)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f17, f1 (x14)\n"                        ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f17, f1 (x11)\n"                        ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f12, f1 (x12)\n"                        ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f21, f1 (x12)\n"                        ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f19, f1 (x13)\n"                        ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f5, f1 (x13)\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f10, f1 (x12)\n"                        ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f3, f1 (x13)\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f25, f1 (x13)\n"                        ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f28, f1 (x15)\n"                        ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f16, f1 (x12)\n"                        ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f3, f1 (x14)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f6, f1 (x11)\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f24, f1 (x13)\n"                        ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f15, f1 (x13)\n"                        ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f7, f1 (x15)\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f3, f1 (x15)\n"                         ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f2, f1 (x11)\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f30, f1 (x11)\n"                        ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f20, f1 (x13)\n"                        ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f28, f1 (x14)\n"                        ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f10, f1 (x15)\n"                        ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f6, f1 (x11)\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f25, f1 (x12)\n"                        ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f6, f1 (x11)\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f16, f1 (x13)\n"                        ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f2, f1 (x12)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f22, f1 (x11)\n"                        ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f3, f1 (x15)\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f28, f1 (x12)\n"                        ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f15, f1 (x12)\n"                        ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f18, f1 (x12)\n"                        ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f30, f1 (x11)\n"                        ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f30, f1 (x12)\n"                        ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f9, f1 (x11)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f20, f1 (x14)\n"                        ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f5, f1 (x13)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f20, f1 (x11)\n"                        ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f20, f1 (x12)\n"                        ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f7, f1 (x15)\n"                         ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f22, f1 (x11)\n"                        ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f5, f1 (x13)\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f27, f1 (x12)\n"                        ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f22, f1 (x14)\n"                        ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f23, f1 (x15)\n"                        ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f8, f1 (x13)\n"                         ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f20, f1 (x14)\n"                        ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f14, f1 (x11)\n"                        ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f25, f1 (x13)\n"                        ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f22, f1 (x14)\n"                        ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f21, f1 (x13)\n"                        ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f19, f1 (x13)\n"                        ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f13, f1 (x14)\n"                        ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f22, f1 (x15)\n"                        ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f12, f1 (x11)\n"                        ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f30, f1 (x11)\n"                        ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f29, f1 (x12)\n"                        ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f20, f1 (x12)\n"                        ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f4, f1 (x13)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f17, f1 (x11)\n"                        ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f8, f1 (x15)\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f26, f1 (x11)\n"                        ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f25, f1 (x13)\n"                        ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f13, f1 (x13)\n"                        ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f15, f1 (x13)\n"                        ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f21, f1 (x14)\n"                        ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f22, f1 (x12)\n"                        ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f26, f1 (x12)\n"                        ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f24, f1 (x11)\n"                        ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominug.pi f27, f1 (x13)\n"                        ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
